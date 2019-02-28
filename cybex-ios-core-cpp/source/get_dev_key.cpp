#include <string.h>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;
using namespace boost::property_tree;

#include <map>
#include <string>
#include <iostream>
#include <string.h>

#include <fc/crypto/elliptic.hpp>
#include <fc/crypto/ripemd160.hpp>
#include <fc/crypto/base58.hpp>
#include <fc/crypto/sha512.hpp>
#include <fc/io/json.hpp>
#include <fc/reflect/reflect.hpp>
#include <fc/io/raw.hpp>
#include <fc/variant_object.hpp>
#include <fc/exception/exception.hpp>
#include <graphene/chain/pts_address.hpp>
#include <graphene/chain/protocol/address.hpp>

#include "wallet_lib.hpp"
#include "walletExtern.h"

using namespace std;

struct binary_key
{
  binary_key(){}
  uint32_t check = 0;
  fc::ecc::public_key_data data;
};

FC_REFLECT( binary_key, (data)(check) )

std::string key_to_wif(const fc::sha256& secret )
{
  const size_t size_of_data_to_hash = sizeof(secret) + 1;
  const size_t size_of_hash_bytes = 4;
  char data[size_of_data_to_hash + size_of_hash_bytes];
  data[0] = (char)0x80;
  memcpy(&data[1], (char*)&secret, sizeof(secret));
  fc::sha256 digest = fc::sha256::hash(data, size_of_data_to_hash);
  digest = fc::sha256::hash(digest);
  memcpy(data + size_of_data_to_hash, (char*)&digest, size_of_hash_bytes);
  return fc::to_base58(data, sizeof(data));
}

/*
 * global parameters to hold the keys
 */

//cybex_priv_key_type active_priv_key, owner_priv_key, memo_priv_key;

static map<string, fc::ecc::private_key> stored_keys;
static map<string, vector<string>> stored_addresses;

static string default_public_key = "";


void set_default_public_key(string pub_key_base58_str)
{
  default_public_key = pub_key_base58_str;
}

void clear_user_key()
{
    stored_keys.clear();
    stored_addresses.clear();
}

void add_user_key(string public_key, fc::ecc::private_key key)
{
    stored_keys.insert(map<string, fc::ecc::private_key>::value_type(public_key, key));
}

void add_address(string public_key, vector<string> addresses)
{
    stored_addresses.insert(map<string, vector<string>>::value_type(public_key, addresses));
}

fc::ecc::private_key& get_private_key(string public_key)
{
    map<string, fc::ecc::private_key>::iterator iter;

    if(public_key == "")
        iter = stored_keys.find(default_public_key);
    else
        iter = stored_keys.find(public_key);
    if(iter != stored_keys.end())
        return iter->second;

    FC_THROW_EXCEPTION(fc::exception, "private key not found");
}

fc::ecc::private_key get_private_key_with_random(string public_key)
{
    fc::ecc::private_key priv_key = fc::ecc::private_key::regenerate(fc::sha256::hash(string(public_key)));
    return priv_key;
}

string get_pubkey_from_address(string address)
{
    if(address == "")
        FC_THROW_EXCEPTION(fc::exception, "private key not found");
    else
        for (auto &i : stored_addresses) {
            if ( std::find(i.second.begin(), i.second.end(), address) != i.second.end() ) {
                return string(i.first.c_str());
            }
        }

    FC_THROW_EXCEPTION(fc::exception, "private key not found");
}


fc::mutable_variant_object get_dev_key(string type, string secret)
{
  fc::ecc::private_key priv_key = fc::ecc::private_key::regenerate(fc::sha256::hash(string(secret)));
  string private_key = key_to_wif( priv_key );
  fc::ecc::public_key pub_key = priv_key.get_public_key();
  struct binary_key bkey;
  bkey.data = pub_key.serialize();
  bkey.check = fc::ripemd160::hash( bkey.data.data, bkey.data.size() )._hash[0];
  
  auto data = fc::raw::pack( bkey );
  string public_key = "CYB" + fc::to_base58( data.data(), data.size() );
  
  auto dat = pub_key.serialize();
  fc::ripemd160 addr = fc::ripemd160::hash( fc::sha512::hash( dat.data, sizeof( dat ) ) );
  fc::array<char,24> bin_addr;
  
  memcpy( (char*)&bin_addr, (char*)&addr, sizeof( addr ) );
  auto checksum = fc::ripemd160::hash( (char*)&addr, sizeof( addr ) );
  memcpy( ((char*)&bin_addr)+20, (char*)&checksum._hash[0], 4 );
  string address = "CYB" + fc::to_base58( bin_addr.data, sizeof( bin_addr ) );
  
  graphene::chain::pts_address compress_pts_addr(pub_key, true, 56);
  graphene::chain::pts_address uncompress_pts_addr(pub_key, false, 56);
  
  fc::mutable_variant_object mvo;
  mvo( "private_key", private_key)
  ( "public_key", public_key)
  ( "address", address)
  ( "compressed", string(graphene::chain::address(compress_pts_addr)))
  ( "uncompressed", string(graphene::chain::address(uncompress_pts_addr)))
  ;

    vector<string> addresses;
    addresses.push_back(address);
    addresses.push_back(string(graphene::chain::address(compress_pts_addr)));
    addresses.push_back(string(graphene::chain::address(uncompress_pts_addr)));

  add_user_key(public_key, priv_key);
    add_address(public_key, addresses);
  if(type == "active")
    default_public_key = public_key;

  return mvo;
  //return fc::json::to_string(mvo);
}

string get_user_key(string user_name, string password)
{
  clear_user_key();
  fc::mutable_variant_object mvo;
  mvo("active-key", get_dev_key("active", user_name + "active" + password))
     ("owner-key", get_dev_key("owner", user_name + "owner" + password))
     ("memo-key", get_dev_key("memo", user_name + "memo" + password))
  ;
  return fc::json::to_string(mvo);
}

string get_active_user_key(string pubkey)
{
    fc::mutable_variant_object mvo;
    graphene::chain::public_key_type to_pub_key = graphene::chain::public_key_type(pubkey);


    fc::ecc::public_key pub_key = to_pub_key;

    auto dat = pub_key.serialize();
    fc::ripemd160 addr = fc::ripemd160::hash( fc::sha512::hash( dat.data, sizeof( dat ) ) );
    fc::array<char,24> bin_addr;

    memcpy( (char*)&bin_addr, (char*)&addr, sizeof( addr ) );
    auto checksum = fc::ripemd160::hash( (char*)&addr, sizeof( addr ) );
    memcpy( ((char*)&bin_addr)+20, (char*)&checksum._hash[0], 4 );
    string address = "CYB" + fc::to_base58( bin_addr.data, sizeof( bin_addr ) );

    graphene::chain::pts_address compress_pts_addr(pub_key, true, 56);
    graphene::chain::pts_address uncompress_pts_addr(pub_key, false, 56);

    fc::mutable_variant_object activemvo;
    activemvo( "private_key", "")
    ( "public_key", pubkey)
    ( "address", address)
    ( "compressed", string(graphene::chain::address(compress_pts_addr)))
    ( "uncompressed", string(graphene::chain::address(uncompress_pts_addr)));

    mvo("active-key", activemvo);
    return fc::json::to_string(mvo);
}

const char *get_user_key(const char *user_name, const char *password) {
    string asss = get_user_key(string(user_name), string(password));
    const char *cstr = asss.c_str();

    return cstr;
}

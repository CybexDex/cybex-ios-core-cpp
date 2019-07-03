#include <string>
#include <fc/container/flat.hpp>
#include <fc/io/json.hpp>
#include <fc/smart_ref_impl.hpp>
#include "graphene/chain/protocol/types.hpp"
#include "graphene/chain/protocol/transfer.hpp"
#include "graphene/chain/protocol/memo.hpp"
#include "cybex/exchange_ops.hpp"

#include "transaction.hpp"
#include "wallet_lib.hpp"
#include "cybex_gateway_query.hpp"
#include "nx_order_op.hpp"

using namespace std;
using namespace fc;
using namespace graphene::chain;

static trx_ext_derived_signature ext_derived_signature;


int char2int(char input)
{
    if(input >= '0' && input <= '9')
        return input - '0';
    if(input >= 'A' && input <= 'F')
        return input - 'A' + 10;
    if(input >= 'a' && input <= 'f')
        return input - 'a' + 10;
    throw std::invalid_argument("Invalid input string");
}

// This function assumes src to be a zero terminated sanitized string with
// an even number of [0-9a-f] characters, and target to be sufficiently large
static void hex2bin( const char* src, unsigned char* target)
{
    while(*src && src[1])
    {
        *(target++) = char2int(*src)*16 + char2int(src[1]);
        src += 2;
    }
}

void clear_ext_derived_signature()
{
    ext_derived_signature.key.nonce = 0;
}

static void _init_transaction(
                              signed_transaction& tx,
                              uint16_t ref_block_num,
                              string ref_block_id_hex_str,
                              uint32_t tx_expiration
                              )
{
    if (ext_derived_signature.key.nonce != 0) {
        tx.extensions.insert(ext_derived_signature);
    }
  block_id_type ref_block_id(ref_block_id_hex_str);
  tx.ref_block_num = ref_block_num;
  tx.ref_block_prefix = ref_block_id._hash[1];
  tx.expiration = fc::time_point_sec(tx_expiration);
    clear_ext_derived_signature();
}

static void _set_balance_claim_operation(
                                         balance_claim_operation& o,
                                         unsigned_int fee_asset_id,
                                         amount_type fee_amount,
                                         unsigned_int deposit_to_account_id,
                                         unsigned_int claimed_id,
                                         string pubkey,
                                         unsigned_int claimed_asset_id,
                                         amount_type claimed_amount
)
{
    o.fee.amount = fee_amount;
    o.fee.asset_id.instance = fee_asset_id;

    o.deposit_to_account = deposit_to_account_id;
    o.balance_to_claim = claimed_id;

    o.balance_owner_key = public_key_type(pubkey);

    o.total_claimed.amount = claimed_amount;
    o.total_claimed.asset_id.instance = claimed_asset_id;
}


static void _set_transfer_operation(
                                    transfer_operation& o,
                                    unsigned_int from_id, /* instance id of from account */
                                    unsigned_int to_id, /* instance id of to account */
                                    amount_type amount, /* amount to be transfered */
                                    unsigned_int asset_id, /* instance id of asset to be transfered */
                                    amount_type fee_amount, /* amount of fee */
                                    unsigned_int fee_asset_id, /* instance id of asset to pay fee */
                                    string memo, /* memo data to be transfered, if no memo data, just use empty string */
                                    string from_memo_pub_key, /* public memo, in base58 str */
                                    string to_memo_pub_key, /* to memo, in base58 str*/
                                    uint64_t vesting_period,
                                    string to_account_pub_key
)
{
    o.fee.amount = fee_amount;
    o.fee.asset_id.instance = fee_asset_id;

    o.from.instance = from_id;
    o.to.instance = to_id;

    o.amount.amount = amount;
    o.amount.asset_id = asset_id;

    if(memo.size())
    {
        o.memo = memo_data();
        //o.memo->from = fc::ecc::public_key::from_base58(from_memo_pub_key);
        //o.memo->to = fc::ecc::public_key::from_base58(to_memo_pub_key);
        fc::ecc::private_key memo_priv_key;
        public_key_type to_pub_key;

        if (from_memo_pub_key.size()) {
            memo_priv_key = get_private_key(from_memo_pub_key);
            to_pub_key = public_key_type(to_memo_pub_key);
        } else {
            memo_priv_key = get_private_key_with_random("xxxmemoxxx");
            to_pub_key = get_private_key_with_random("xxxxmemoxxxx").get_public_key();
        }
        //    fc::ecc::private_key memo_priv_key = fc::ecc::private_key();
        o.memo->set_message(memo_priv_key, to_pub_key, memo);
    }

    if (to_account_pub_key.size()) {
        struct cybex_ext_vesting v;
        v.public_key = public_key_type(to_account_pub_key);
        v.vesting_period = vesting_period;
        o.extensions.insert(v);
    }
}

void set_derived_operation_extensions(
                                      string master_public_key,
                                      string derived_private_key,
                                      string derived_public_key,
                                      uint32_t nonce,
                                      string signature
                                    )
{
    if (master_public_key.size()) {
        trx_ext_derived_signature ext;
        ext.key.temp_key = public_key_type(derived_public_key);
        ext.key.nonce = nonce;
        ext.pubkey = master_public_key;
        hex2bin(signature.c_str(), ext.signature.begin());

        ext_derived_signature = ext;
        set_default_private_key(derived_private_key);
    }
}

string get_account_update_json(
                               uint16_t ref_block_num,
                               string ref_block_id_hex_str,
                               uint32_t expiration,
                               string chain_id_str,

                               string account_update_op_json
                               )
{ try {
    fc::ecc::private_key active_priv_key = get_private_key("");
    account_update_operation o = fc::json::from_string(account_update_op_json).as<graphene::chain::account_update_operation>();

    signed_transaction signed_tx;
    _init_transaction(signed_tx, ref_block_num, ref_block_id_hex_str, expiration);

    signed_tx.operations.push_back(o);

    chain_id_type chain_id(chain_id_str);
    signed_tx.sign(active_priv_key, chain_id);
    set_default_private_key("");

    variant tx(signed_tx);
    return fc::json::to_string(tx);
} catch(...){return "";}}

string transfer(
                uint16_t ref_block_num,
                string ref_block_id_hex_str,
                uint32_t expiration, /* expiration time in utc seconds */
                string chain_id_str, /* chain id in base58 */
                unsigned_int from_id, /* instance id of from account */
                unsigned_int to_id, /* instance id of to account */
                amount_type amount, /* amount to be transfered */
                unsigned_int asset_id, /* instance id of asset to be transfered */
                amount_type fee_amount, /* amount of fee */
                unsigned_int fee_asset_id, /* instance id of asset to pay fee */
                string memo, /* memo data to be transfered, if no memo data, just use empty string */
                string from_memo_pub_key, /* public memo, in base58 str */
                string to_memo_pub_key /* to memo, in base58 str*/
)
{ try{
  fc::ecc::private_key active_priv_key = get_private_key("");
  
  transfer_operation o;
  
  _set_transfer_operation(o, from_id, to_id, amount, asset_id, fee_amount, fee_asset_id, memo, from_memo_pub_key, to_memo_pub_key, 0, "");

  signed_transaction signed_tx;
  _init_transaction(signed_tx, ref_block_num, ref_block_id_hex_str, expiration);
  
  signed_tx.operations.push_back(o);
  
  chain_id_type chain_id(chain_id_str);
  signed_tx.sign(active_priv_key, chain_id);
    set_default_private_key("");

  variant tx(signed_tx);
  return fc::json::to_string(tx);
} catch(...) {return "";}}

string transfer_with_vesting(
                             uint16_t ref_block_num,
                             string ref_block_id_hex_str,
                             uint32_t expiration, /* expiration time in utc seconds */
                             string chain_id_str,

                             unsigned_int from_id, /* instance id of from account */
                             unsigned_int to_id, /* instance id of to account */
                             amount_type amount, /* amount to be transfered */
                             unsigned_int asset_id, /* instance id of asset to be transfered */
                             amount_type fee_amount, /* amount of fee */
                             unsigned_int fee_asset_id, /* instance id of asset to pay fee */
                             string memo, /* memo data to be transfered, if no memo data, just use empty string */
                             string from_memo_pub_key, /* public memo */
                             string to_memo_pub_key,

                             uint64_t vesting_period,
                             string to_account_pub_key
                             )
{
    try{
        fc::ecc::private_key active_priv_key = get_private_key("");

        transfer_operation o;

        _set_transfer_operation(o, from_id, to_id, amount, asset_id, fee_amount, fee_asset_id, memo, from_memo_pub_key, to_memo_pub_key, vesting_period, to_account_pub_key);

        signed_transaction signed_tx;
        _init_transaction(signed_tx, ref_block_num, ref_block_id_hex_str, expiration);

        signed_tx.operations.push_back(o);

        chain_id_type chain_id(chain_id_str);
        signed_tx.sign(active_priv_key, chain_id);
        set_default_private_key("");

        variant tx(signed_tx);
        return fc::json::to_string(tx);
    } catch(...) {return "";}
}

string transaction_id(
                uint16_t ref_block_num,
                string ref_block_id_hex_str,
                uint32_t expiration, /* expiration time in utc seconds */
                string chain_id_str, /* chain id in base58 */
                unsigned_int from_id, /* instance id of from account */
                unsigned_int to_id, /* instance id of to account */
                amount_type amount, /* amount to be transfered */
                unsigned_int asset_id, /* instance id of asset to be transfered */
                amount_type fee_amount, /* amount of fee */
                unsigned_int fee_asset_id, /* instance id of asset to pay fee */
                string memo, /* memo data to be transfered, if no memo data, just use empty string */
                string from_memo_pub_key, /* public memo, in base58 str */
                string to_memo_pub_key /* to memo, in base58 str*/
)
{ try{
    fc::ecc::private_key active_priv_key = get_private_key("");

    transfer_operation o;

    _set_transfer_operation(o, from_id, to_id, amount, asset_id, fee_amount, fee_asset_id, memo, from_memo_pub_key, to_memo_pub_key, 0, "");

    signed_transaction signed_tx;
    _init_transaction(signed_tx, ref_block_num, ref_block_id_hex_str, expiration);

    signed_tx.operations.push_back(o);

    chain_id_type chain_id(chain_id_str);
    signed_tx.sign(active_priv_key, chain_id);
    set_default_private_key("");

    return signed_tx.id().str();
} catch(...) {return "";}}

string get_transfer_op_json(
                            unsigned_int from_id, /* instance id of from account */
                            unsigned_int to_id, /* instance id of to account */
                            amount_type amount, /* amount to be transfered */
                            unsigned_int asset_id, /* instance id of asset to be transfered */
                            amount_type fee_amount, /* amount of fee */
                            unsigned_int fee_asset_id, /* instance id of asset to pay fee */
                            string memo, /* memo data to be transfered, if no memo data, just use empty string */
                            string from_memo_pub_key, /* public memo */
                            string to_memo_pub_key
                            )
{ try {
  transfer_operation o;
  _set_transfer_operation(o, from_id, to_id, amount, asset_id, fee_amount, fee_asset_id, memo, from_memo_pub_key, to_memo_pub_key, 0, "");
  variant op_json(o);
  return fc::json::to_string(op_json);
} catch(...) {return "";}}

string get_transfer_with_vesting_op_json(
                                         unsigned_int from_id, /* instance id of from account */
                                         unsigned_int to_id, /* instance id of to account */
                                         amount_type amount, /* amount to be transfered */
                                         unsigned_int asset_id, /* instance id of asset to be transfered */
                                         amount_type fee_amount, /* amount of fee */
                                         unsigned_int fee_asset_id, /* instance id of asset to pay fee */
                                         string memo, /* memo data to be transfered, if no memo data, just use empty string */
                                         string from_memo_pub_key, /* public memo */
                                         string to_memo_pub_key,

                                         uint64_t vesting_period,
                                         string to_account_pub_key
                                         )
{
    try {
        transfer_operation o;
        _set_transfer_operation(o, from_id, to_id, amount, asset_id, fee_amount, fee_asset_id, memo, from_memo_pub_key, to_memo_pub_key, vesting_period, to_account_pub_key);
        variant op_json(o);
        return fc::json::to_string(op_json);
    } catch(...) {return "";}
}

void _set_limit_order_create_operation(
                                       limit_order_create_operation &o,
                                       unsigned_int seller_id, /* instance id of seller account */
                                       uint32_t order_expiration,
                                       unsigned_int sell_asset_id, /* instance id fo asset to be sold */
                                       amount_type sell_amount, /* amount to be sold */
                                       unsigned_int receive_asset_id, /* instance id of asset to receive */
                                       amount_type min_to_receive, /* min asset to receive */
                                       int fill_or_kill, /* 1 for true, 0 for false */
                                       amount_type fee_amount, /* amount of fee */
                                       unsigned_int fee_asset_id /* instance id of asset to pay fee*/
)
{
  o.seller.instance = seller_id;
  o.expiration = fc::time_point_sec(order_expiration);
  o.amount_to_sell.asset_id = sell_asset_id;
  o.amount_to_sell.amount = sell_amount;
  o.min_to_receive.asset_id = receive_asset_id;
  o.min_to_receive.amount = min_to_receive;
  o.fill_or_kill = (fill_or_kill == 0 ? false:true);
  o.fee.amount = fee_amount;
  o.fee.asset_id = fee_asset_id;
}

string limit_order_create(
                          uint16_t ref_block_num,
                          string ref_block_id_hex_str,
                          uint32_t expiration, /* expiration time in utc seconds */
                          string chain_id_str,
                          unsigned_int seller_id, /* instance id of seller account */
                          uint32_t order_expiration,
                          unsigned_int sell_asset_id, /* instance id fo asset to be sold */
                          amount_type sell_amount, /* amount to be sold */
                          unsigned_int receive_asset_id, /* instance id of asset to receive */
                          amount_type min_to_receive, /* min asset to receive */
                          int fill_or_kill, /* 1 for true, 0 for false */
                          amount_type fee_amount, /* amount of fee */
                          unsigned_int fee_asset_id /* instance id of asset to pay fee*/
)
{ try {
  fc::ecc::private_key active_priv_key = get_private_key("");
  
  limit_order_create_operation o;
  _set_limit_order_create_operation(o, seller_id, order_expiration,
                                    sell_asset_id, sell_amount, receive_asset_id, min_to_receive, fill_or_kill, fee_amount, fee_asset_id);
  
  signed_transaction signed_tx;
  _init_transaction(signed_tx, ref_block_num, ref_block_id_hex_str, expiration);
  
  signed_tx.operations.push_back(o);
  
  chain_id_type chain_id(chain_id_str);
  signed_tx.sign(active_priv_key, chain_id);
    set_default_private_key("");

  variant tx(signed_tx);
  return fc::json::to_string(tx);
} catch(...) {return "";}}

string limit_order_create_by_side(
                                  uint16_t ref_block_num,
                                  string ref_block_id_hex_str,
                                  uint32_t expiration, /* expiration time in utc seconds */
                                  string chain_id_str,

                                  unsigned_int seller_id, /* instance id of seller account */
                                  uint32_t order_expiration,
                                  unsigned_int sell_asset_id, /* instance id fo asset to be sold */
                                  amount_type sell_amount, /* amount to be sold */
                                  unsigned_int receive_asset_id, /* instance id of asset to receive */
                                  amount_type min_to_receive, /* min asset to receive */
                                  int fill_or_kill, /* 1 for true, 0 for false */
                                  amount_type fee_amount, /* amount of fee */
                                  unsigned_int fee_asset_id, /* instance id of asset to pay fee*/
                                  bool is_buy
) {
    try {
        fc::ecc::private_key active_priv_key = get_private_key("");

        limit_order_create_operation o;
        _set_limit_order_create_operation(o, seller_id, order_expiration,
                                          sell_asset_id, sell_amount, receive_asset_id, min_to_receive, fill_or_kill, fee_amount, fee_asset_id);
        struct order_side v;
        v.is_buy = is_buy;
        o.extensions.insert(v);

        signed_transaction signed_tx;
        _init_transaction(signed_tx, ref_block_num, ref_block_id_hex_str, expiration);

        signed_tx.operations.push_back(o);

        chain_id_type chain_id(chain_id_str);
        signed_tx.sign(active_priv_key, chain_id);
        set_default_private_key("");

        variant tx(signed_tx);
        return fc::json::to_string(tx);
    } catch(...) {return "";}
}


string get_limit_order_create_json(
                                   unsigned_int seller_id, /* instance id of seller account */
                                   uint32_t order_expiration,
                                   unsigned_int sell_asset_id, /* instance id fo asset to be sold */
                                   amount_type sell_amount, /* amount to be sold */
                                   unsigned_int receive_asset_id, /* instance id of asset to receive */
                                   amount_type min_to_receive, /* min asset to receive */
                                   int fill_or_kill, /* 1 for true, 0 for false */
                                   amount_type fee_amount, /* amount of fee */
                                   unsigned_int fee_asset_id /* instance id of asset to pay fee*/
)
{ try {
  limit_order_create_operation o;
  _set_limit_order_create_operation(o, seller_id, order_expiration,
                                    sell_asset_id, sell_amount, receive_asset_id, min_to_receive, fill_or_kill, fee_amount, fee_asset_id);
  variant op_json(o);
  return fc::json::to_string(op_json);
} catch(...) {return "";}}

static void _set_cancel_order_operation(
                                        limit_order_cancel_operation &o,
                                        unsigned_int order_id, /* instance id of order */
                                        unsigned_int fee_paying_account_id, /* instance id of fee paying account */
                                        amount_type fee_amount, /* amount of fee */
                                        unsigned_int fee_asset_id /* instance id of asset to pay fee */
)
{
  o.order = order_id;
  o.fee_paying_account = fee_paying_account_id;
  o.fee.amount = fee_amount;
  o.fee.asset_id = fee_asset_id;
}

static void _set_cancel_all_order_operation(
                                        cancel_all_operation &o,
                                        unsigned_int sell_asset_id,
                                        unsigned_int receive_asset_id,
                                        unsigned_int fee_paying_account_id, /* instance id of fee paying account */
                                        amount_type fee_amount, /* amount of fee */
                                        unsigned_int fee_asset_id /* instance id of asset to pay fee */
)
{
    o.seller = fee_paying_account_id;
    o.receive_asset_id = receive_asset_id;
    o.sell_asset_id = sell_asset_id;
    o.fee.amount = fee_amount;
    o.fee.asset_id = fee_asset_id;
}

string cancel_order(
                    uint16_t ref_block_num,
                    string ref_block_id_hex_str,
                    uint32_t expiration, /* expiration time in utc seconds */
                    string chain_id_str,
                    
                    unsigned_int order_id, /* instance id of order */
                    unsigned_int fee_paying_account_id, /* instance id of fee paying account */
                    amount_type fee_amount, /* amount of fee */
                    unsigned_int fee_asset_id /* instance id of asset to pay fee */
)
{ try {
  fc::ecc::private_key active_priv_key = get_private_key("");
  
  limit_order_cancel_operation o;
  _set_cancel_order_operation(o,
                              order_id, fee_paying_account_id, fee_amount, fee_asset_id);
  
  signed_transaction signed_tx;
  _init_transaction(signed_tx, ref_block_num, ref_block_id_hex_str, expiration);
  
  signed_tx.operations.push_back(o);
  
  chain_id_type chain_id(chain_id_str);
  signed_tx.sign(active_priv_key, chain_id);
    set_default_private_key("");

  variant tx(signed_tx);
  return fc::json::to_string(tx);
} catch(...){return "";}}

string cancel_all_order(
                        uint16_t ref_block_num,
                        string ref_block_id_hex_str,
                        uint32_t expiration, /* expiration time in utc seconds */
                        string chain_id_str,

                        unsigned_int sell_asset_id,
                        unsigned_int receive_asset_id,
                        unsigned_int fee_paying_account_id, /* instance id of fee paying account */
                        amount_type fee_amount, /* amount of fee */
                        unsigned_int fee_asset_id /* instance id of asset to pay fee */
)
{
    try {
        fc::ecc::private_key active_priv_key = get_private_key("");

        cancel_all_operation o;
        _set_cancel_all_order_operation(o,
                                    sell_asset_id, receive_asset_id, fee_paying_account_id, fee_amount, fee_asset_id);

        signed_transaction signed_tx;
        _init_transaction(signed_tx, ref_block_num, ref_block_id_hex_str, expiration);

        signed_tx.operations.push_back(o);

        chain_id_type chain_id(chain_id_str);
        signed_tx.sign(active_priv_key, chain_id);
        set_default_private_key("");

        variant tx(signed_tx);
        return fc::json::to_string(tx);
}catch(...){return "";}}

string get_cancel_order_json(
                             unsigned_int order_id, /* instance id of order */
                             unsigned_int fee_paying_account_id, /* instance id of fee paying account */
                             amount_type fee_amount, /* amount of fee */
                             unsigned_int fee_asset_id /* instance id of asset to pay fee */
)
{ try {
  limit_order_cancel_operation o;
  _set_cancel_order_operation(o,
                              order_id, fee_paying_account_id, fee_amount, fee_asset_id);
  variant op_json(o);
  return fc::json::to_string(op_json);
} catch(...){return "";}}

string get_cancel_all_order_json(
                                 unsigned_int sell_asset_id,
                                 unsigned_int receive_asset_id,
                                 unsigned_int fee_paying_account_id, /* instance id of fee paying account */
                                 amount_type fee_amount, /* amount of fee */
                                 unsigned_int fee_asset_id /* instance id of asset to pay fee */
)
{
    try {
        cancel_all_operation o;
        _set_cancel_all_order_operation(o,
                                    sell_asset_id, receive_asset_id, fee_paying_account_id, fee_amount, fee_asset_id);
        variant op_json(o);
        return fc::json::to_string(op_json);
}catch(...){return "";}}

string cybex_gateway_query(
                           string accountName,
                           string asset,
                           string fundType,
                           uint32_t size,
                           uint32_t offset,
                           uint32_t expiration
                           )
{try{
  struct cybex_gateway_query_transaction trx;
  digest_type::encoder enc;
  
  trx.op.accountName = accountName;
//  trx.op.asset = asset;
//  trx.op.fundType = fundType;
//  trx.op.size = size;
//  trx.op.offset = offset;
  trx.op.expiration = expiration; //fc::time_point_sec(expiration);
  
  fc::ecc::private_key active_priv_key = get_private_key("");
  fc::raw::pack(enc, trx.op);
  trx.signer = active_priv_key.sign_compact(enc.result());
    set_default_private_key("");

  return fc::json::to_string(trx);
} catch(...){return "";}}

string get_claim_balance_operation (
                                    unsigned_int fee_asset_id,
                                    amount_type fee_amount,
                                    unsigned_int deposit_to_account_id,
                                    unsigned_int claimed_id,
                                    string claimed_own,
                                    unsigned_int claimed_asset_id,
                                    amount_type claimed_amount
                                    )
{ try {
    balance_claim_operation o;
    _set_balance_claim_operation(o, fee_asset_id, fee_amount, deposit_to_account_id, claimed_id, claimed_own, claimed_asset_id, claimed_amount);
    variant op_json(o);
    return fc::json::to_string(op_json);
} catch(...) {return "";}}

string sign_claim_balance(
                          uint16_t ref_block_num,
                          string ref_block_id_hex_str,
                          uint32_t expiration, /* expiration time in utc seconds */
                          string chain_id_str, /* chain id in base58 */
                          unsigned_int fee_asset_id,
                          amount_type fee_amount,
                          unsigned_int deposit_to_account_id,
                          unsigned_int claimed_id,
                          string claimed_own,
                          unsigned_int claimed_asset_id,
                          amount_type claimed_amount
                          )
{ try{
    fc::ecc::private_key active_priv_key = get_private_key("");

    balance_claim_operation o;
    string pubkey;
    if (ext_derived_signature.key.nonce != 0) {
        pubkey = ext_derived_signature.pubkey;
    } else {
        pubkey = get_pubkey_from_address(claimed_own);
    }

    _set_balance_claim_operation(o, fee_asset_id, fee_amount, deposit_to_account_id, claimed_id, pubkey, claimed_asset_id, claimed_amount);

    signed_transaction signed_tx;
    _init_transaction(signed_tx, ref_block_num, ref_block_id_hex_str, expiration);

    signed_tx.operations.push_back(o);

    chain_id_type chain_id(chain_id_str);
    signed_tx.sign(active_priv_key, chain_id);
    set_default_private_key("");

    variant tx(signed_tx);
    return fc::json::to_string(tx);
} catch(...) {return "";}}

string sign_message(string message) {
    try{
        digest_type::encoder enc;

        fc::ecc::private_key active_priv_key = get_private_key("");
//        fc::raw::pack(enc, message);
        enc.write(message.c_str(), message.length());
        signature_type result = active_priv_key.sign_compact(enc.result());
        set_default_private_key("");

        return fc::json::to_string(result);
    } catch(...){return "";}
}

string decrypt_memo_data(
                         string memo_json_str
                         )
{ try{
  struct memo_data m = fc::json::from_string(memo_json_str).as<memo_data>();
  fc::ecc::private_key memo_priv_key;
  fc::ecc::public_key memo_pub_key;
  try {
    memo_priv_key = get_private_key(std::string(m.from));
    memo_pub_key = m.to;
  } catch(...){
    memo_priv_key = get_private_key(std::string(m.to));
    memo_pub_key = m.from;
  }
  
  return m.get_message(memo_priv_key, memo_pub_key);
} catch(...){return "";}}

string transactionId_from_jsonstr(string jsonStr) {
    struct signed_transaction signed_tx = fc::json::from_string(jsonStr).as<signed_transaction>();
    return signed_tx.id().str();
}

void wallet_test() {
    printf("s");
}

void _set_exchange_participate_operation(
                                       exchange_participate_operation &o,
                                         unsigned_int payer_id,
                                         unsigned_int asset_id,
                                         amount_type amount,
                                         unsigned_int exchange_id,
                                         amount_type fee_amount,
                                         unsigned_int fee_asset_id
)
{
    o.payer.instance = payer_id;
    o.amount.asset_id = asset_id;
    o.amount.amount = amount;
    o.exchange_to_pay = exchange_id;
    o.fee.amount = fee_amount;
    o.fee.asset_id = fee_asset_id;
}

string exchange_participate(
                            uint16_t ref_block_num,
                            string ref_block_id_hex_str,
                            uint32_t expiration, /* expiration time in utc seconds */
                            string chain_id_str,

                            unsigned_int payer_id,
                            unsigned_int asset_id,
                            amount_type amount,
                            unsigned_int exchange_id,
                            amount_type fee_amount,
                            unsigned_int fee_asset_id
)
{
    try{
        fc::ecc::private_key active_priv_key = get_private_key("");

        exchange_participate_operation o;

        _set_exchange_participate_operation(o, payer_id, asset_id, amount, exchange_id, fee_amount, fee_asset_id);

        signed_transaction signed_tx;
        _init_transaction(signed_tx, ref_block_num, ref_block_id_hex_str, expiration);

        signed_tx.operations.push_back(o);

        chain_id_type chain_id(chain_id_str);
        signed_tx.sign(active_priv_key, chain_id);
        set_default_private_key("");

        variant tx(signed_tx);
        return fc::json::to_string(tx);
    } catch(...) {return "";}
}

string exchange_participate_json(
                            unsigned_int payer_id,
                            unsigned_int asset_id,
                            amount_type amount,
                            unsigned_int exchange_id,
                            amount_type fee_amount,
                            unsigned_int fee_asset_id
                            )
{
    try{
        exchange_participate_operation o;

        _set_exchange_participate_operation(o, payer_id, asset_id, amount, exchange_id, fee_amount, fee_asset_id);

        variant op_json(o);
        return fc::json::to_string(op_json);
    } catch(...) {return "";}
}


string amend_order(string ss) {
    try{
        struct nx_order_amend_transaction trx;
        digest_type::encoder enc;

        trx.op.ss = ss;

        fc::ecc::private_key active_priv_key = get_private_key("");
        fc::raw::pack(enc, trx.op);
        trx.signer = active_priv_key.sign_compact(enc.result());
        set_default_private_key("");

        return fc::json::to_string(trx);
    } catch(...){return "";}
}

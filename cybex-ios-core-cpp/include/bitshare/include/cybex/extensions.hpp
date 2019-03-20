
#pragma once

#include <graphene/chain/protocol/types.hpp>

namespace graphene { namespace chain {

struct cybex_ext_vesting {
  uint64_t vesting_period;
  public_key_type   public_key;
};
struct cybex_ext_transfer_vesting {
  uint64_t vesting_cliff;
  uint64_t vesting_duration;
};

struct cybex_ext_swap {
  string   msg;
};

struct cybex_ext_xfer_to_name {
  string   name;
  string   asset_sym;
  string   fee_asset_sym;
  uint8_t  hw_cookie1;
  uint8_t  hw_cookie2;
};
struct cybex_xfer_item {
  string   name;
  float amount;
};
struct cybex_ext_xfer_to_many {
  vector<struct cybex_xfer_item>  list;
};

/*
 * extension for transaction
 */
struct derived_public_key {
    uint32_t        nonce;
  public_key_type temp_key;
  digest_type     digest() const;
};

struct trx_ext_derived_signature {
  derived_public_key key;
  signature_type    signature; /* signature is used to validate */
    string pubkey;
  const signature_type& sign(const private_key_type& pk);
  void verify() const;
};

void cybex_ext_vesting_check(const account_object & to_account, const cybex_ext_vesting & ext1);

} }

FC_REFLECT( graphene::chain::cybex_ext_vesting,(vesting_period)(public_key ))
FC_REFLECT( graphene::chain::cybex_ext_transfer_vesting,(vesting_cliff)(vesting_duration) )
FC_REFLECT( graphene::chain::cybex_ext_swap,(msg) )
FC_REFLECT( graphene::chain::cybex_ext_xfer_to_name,(name)(asset_sym)(fee_asset_sym)(hw_cookie1)(hw_cookie2) )
FC_REFLECT( graphene::chain::cybex_xfer_item,(name)(amount) )
FC_REFLECT( graphene::chain::cybex_ext_xfer_to_many,(list) )
FC_REFLECT( graphene::chain::derived_public_key, (nonce)(temp_key) )
FC_REFLECT( graphene::chain::trx_ext_derived_signature, (key)(signature) )

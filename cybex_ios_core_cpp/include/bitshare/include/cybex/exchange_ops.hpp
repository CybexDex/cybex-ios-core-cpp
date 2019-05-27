#pragma once

#include <graphene/chain/protocol/base.hpp>
#include <graphene/chain/protocol/vesting.hpp>

#define GRAPHENE_MIN_EXCHANGE_NAME_LENGTH 3
#define GRAPHENE_MAX_EXCHANGE_NAME_LENGTH 63

namespace graphene {
namespace chain {
enum exchange_owner_permission_flags {
  exchange_allow_quote_to_base = 1 << 0,  /** allow get base */
  exchange_allow_base_to_quote = 1 << 1,  /** allow get quote */
  exchange_allow_deposit_base = 1 << 2,   /** allow can deposit base */
  exchange_allow_withdraw_base = 1 << 3,  /** allow can withdraw base */
  exchange_allow_deposit_quote = 1 << 4,  /** allow can deposit quote */
  exchange_allow_withdraw_quote = 1 << 5, /** allow can withdraw quote */
  exchange_allow_charge_market_fee =
      1 << 6, /** allow charge market fee when pays out */
  exchange_allow_modify_rate = 1 << 7, /** allow modify rate */
  exchange_allow_only_white_list =
      1 << 8, /** allow only account in whitelist to participate */
};

const static uint32_t EXCHANGE_OWNER_PERMISSION_MASK =
    exchange_allow_quote_to_base | exchange_allow_base_to_quote |
    exchange_allow_deposit_base | exchange_allow_withdraw_base |
    exchange_allow_deposit_quote | exchange_allow_withdraw_quote |
    exchange_allow_charge_market_fee | exchange_allow_modify_rate |
    exchange_allow_only_white_list;

struct exchange_check_once_amount {
  asset_id_type asset_id;
  share_type min;
  share_type max;
};

struct exchange_check_divisible {
  asset divisor;
};

struct exchange_check_amount { /** user amount limit */
  asset_id_type asset_id;
  share_type floor;
  share_type ceil;
};

struct exchange_vesting_policy_wrapper {
  vesting_policy_initializer policy;
};

typedef static_variant<exchange_check_amount, exchange_check_once_amount,
                       exchange_check_divisible, exchange_vesting_policy_wrapper>
    exchange_extension;

struct exchange_options {
  price rate;

  uint32_t owner_permissions = EXCHANGE_OWNER_PERMISSION_MASK;
  uint32_t flags;

  flat_set<account_id_type> whitelist_authorities;
  flat_set<account_id_type> blacklist_authorities;

  flat_set<exchange_extension> extensions;
  string description;
  void validate() const;
};

struct exchange_participate_operation : public base_operation {
  struct fee_parameters_type {
    uint64_t fee = 10 * GRAPHENE_BLOCKCHAIN_PRECISION;
  };

  asset fee;
  account_id_type payer;
  exchange_id_type exchange_to_pay;
  asset amount;

  extensions_type extensions;

  account_id_type fee_payer() const { return payer; }
  void validate() const;
};

struct exchange_create_operation : public base_operation {
  struct fee_parameters_type {
    uint64_t symbol3 = 500000 * GRAPHENE_BLOCKCHAIN_PRECISION;
    uint64_t symbol4 = 300000 * GRAPHENE_BLOCKCHAIN_PRECISION;
    uint64_t long_symbol = 5000 * GRAPHENE_BLOCKCHAIN_PRECISION;
    uint32_t price_per_kbyte = 10;  /// only required for large memos.
  };

  asset fee;
  string name;
  account_id_type owner;
  exchange_options options;

  extensions_type extensions;

  account_id_type fee_payer() const { return owner; }
  void validate() const;
  share_type calculate_fee(const fee_parameters_type& k) const;
};

struct exchange_update_operation : public base_operation {
  struct fee_parameters_type {
    uint64_t fee = 10 * GRAPHENE_BLOCKCHAIN_PRECISION;
    uint32_t price_per_kbyte = 10;
  };

  asset fee;
  account_id_type owner;

  exchange_id_type exchange_to_update;
  optional<account_id_type> new_owner;
  optional<exchange_options> new_options;

  extensions_type extensions;

  account_id_type fee_payer() const { return owner; }
  void validate() const;
};

struct exchange_withdraw_operation : public base_operation {
  struct fee_parameters_type {
    uint64_t fee = 10 * GRAPHENE_BLOCKCHAIN_PRECISION;
  };

  asset fee;
  account_id_type owner;

  exchange_id_type exchange_to_withdraw;
  asset amount;

  extensions_type extensions;

  account_id_type fee_payer() const { return owner; }
  void validate() const;
};

struct exchange_deposit_operation : public base_operation {
  struct fee_parameters_type {
    uint64_t fee = 10 * GRAPHENE_BLOCKCHAIN_PRECISION;
  };

  asset fee;
  account_id_type owner;

  exchange_id_type exchange_to_deposit;
  asset amount;

  extensions_type extensions;

  account_id_type fee_payer() const { return owner; }
  void validate() const;
};

struct exchange_remove_operation : public base_operation {
  struct fee_parameters_type {
    uint64_t fee = 10 * GRAPHENE_BLOCKCHAIN_PRECISION;
  };

  asset fee;
  account_id_type owner;

  exchange_id_type exchange_to_remove;

  extensions_type extensions;

  account_id_type fee_payer() const { return owner; }
  void validate() const;
};

struct exchange_fill_operation : public base_operation {
  struct fee_parameters_type{};
  exchange_fill_operation(){}
  exchange_fill_operation(account_id_type p, exchange_id_type e, asset pa, asset re):payer(p),exchange(e),pay(pa),receive(re){}
  account_id_type payer;
  exchange_id_type exchange;
  asset pay;
  asset receive;
  asset fee;
  void validate() const { FC_ASSERT(!"virtual operation"); }
  account_id_type fee_payer() const { return payer; }
  share_type calculate_fee(const fee_parameters_type& k) const { return 0; }
};

}  // namespace chain
}  // namespace graphene

FC_REFLECT(graphene::chain::exchange_create_operation::fee_parameters_type,
           (symbol3)(symbol4)(long_symbol)(price_per_kbyte))
FC_REFLECT(graphene::chain::exchange_update_operation::fee_parameters_type,
           (fee)(price_per_kbyte))
FC_REFLECT(graphene::chain::exchange_withdraw_operation::fee_parameters_type,
           (fee))
FC_REFLECT(graphene::chain::exchange_deposit_operation::fee_parameters_type,
           (fee))
FC_REFLECT(graphene::chain::exchange_participate_operation::fee_parameters_type,
           (fee))
FC_REFLECT(graphene::chain::exchange_remove_operation::fee_parameters_type,
           (fee))
FC_REFLECT(graphene::chain::exchange_fill_operation::fee_parameters_type,  )

FC_REFLECT(graphene::chain::exchange_check_once_amount, (asset_id)(min)(max))
FC_REFLECT(graphene::chain::exchange_check_divisible, (divisor))
FC_REFLECT(graphene::chain::exchange_check_amount, (asset_id)(floor)(ceil))
FC_REFLECT(graphene::chain::exchange_vesting_policy_wrapper, (policy))

FC_REFLECT(graphene::chain::exchange_options,
           (rate)(owner_permissions)(flags)(whitelist_authorities)(
               blacklist_authorities)(extensions)(description))

FC_REFLECT(graphene::chain::exchange_create_operation,
           (fee)(name)(owner)(options)(extensions))
FC_REFLECT(graphene::chain::exchange_update_operation,
           (fee)(owner)(exchange_to_update)(new_owner)(new_options)(extensions))
FC_REFLECT(graphene::chain::exchange_withdraw_operation,
           (fee)(owner)(exchange_to_withdraw)(amount)(extensions))
FC_REFLECT(graphene::chain::exchange_deposit_operation,
           (fee)(owner)(exchange_to_deposit)(amount)(extensions))
FC_REFLECT(graphene::chain::exchange_remove_operation,
           (fee)(owner)(exchange_to_remove)(extensions))
FC_REFLECT(graphene::chain::exchange_participate_operation,
           (fee)(payer)(exchange_to_pay)(amount)(extensions))
FC_REFLECT(graphene::chain::exchange_fill_operation,
           (payer)(exchange)(pay)(receive))

#define db_notify_exchange                                   \
    void operator()( const exchange_create_operation& op )   \
    {                                                       \
        _impacted.insert( op.owner );                       \
    }                                                       \
    void operator()( const exchange_update_operation& op )   \
    {                                                       \
        _impacted.insert( op.owner );                       \
    }                                                       \
    void operator()( const exchange_withdraw_operation& op ) \
    {                                                       \
        _impacted.insert( op.owner );                       \
    }                                                       \
    void operator()( const exchange_deposit_operation& op )  \
    {                                                       \
        _impacted.insert( op.owner );                       \
    }                                                       \
    void operator()( const exchange_participate_operation& op )          \
    {                                                       \
        _impacted.insert( op.payer );                       \
    }                                                       \
    void operator()( const exchange_remove_operation& op )  \
    {                                                       \
        _impacted.insert( op.owner );                       \
    }                                                       \
    void operator()( const exchange_fill_operation& op )    \
    {                                                       \
        _impacted.insert( op.payer );                       \
    }

#define impact_visit_exchange db_notify_exchange

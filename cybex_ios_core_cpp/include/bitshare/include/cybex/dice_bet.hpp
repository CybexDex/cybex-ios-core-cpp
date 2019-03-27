#pragma once

#include <graphene/chain/database.hpp>
#include <graphene/chain/protocol/base.hpp>

namespace graphene { namespace chain {
    class dice_bet_object : public abstract_object<dice_bet_object>
    {
        public:
            static const uint8_t space_id = protocol_ids;
            static const uint8_t type_id  = dice_bet_object_type;

            account_id_type owner;
            asset_id_type   asset_to_bet;
            share_type      pool_balance;
            share_type      clearing_threshold;
            share_type      pending_balance;
            share_type      min_amount;
            share_type      max_amount;
            uint16_t        pay_discount_percent;
            uint16_t        max_dice;
    };

    struct by_owner;

    using dice_bet_multi_index_type = multi_index_container<
        dice_bet_object,
        indexed_by<
            ordered_unique< tag<by_id>, member< object, object_id_type, &object::id > >,
            ordered_unique< tag<by_owner>, 
                composite_key<
                    dice_bet_object,
                    member< dice_bet_object, account_id_type, &dice_bet_object::owner >,
                    member< object, object_id_type, &object::id >
                >
            >
        >
    >;

    using dice_bet_index = generic_index< dice_bet_object, dice_bet_multi_index_type >;

    class dice_bet_contract_object : public abstract_object<dice_bet_contract_object>
    {
        public:
            static const uint8_t space_id = protocol_ids;
            static const uint8_t type_id  = dice_bet_contract_object_type;

            dice_bet_id_type dice_bet;
            account_id_type payer;
            share_type      amount;
            share_type      pending;
            uint16_t        choice;
    };

    using dice_bet_contract_multi_index_type = multi_index_container<
        dice_bet_contract_object,
        indexed_by<
            ordered_unique< tag<by_id>, member< object, object_id_type, &object::id > >
        >
    >;

    using dice_bet_contract_index = generic_index< dice_bet_contract_object, dice_bet_contract_multi_index_type >;
}}

FC_REFLECT_DERIVED( graphene::chain::dice_bet_object, (graphene::db::object),
                    (owner)(asset_to_bet)(pool_balance)(clearing_threshold)(pending_balance)
                    (min_amount)(max_amount)(pay_discount_percent)(max_dice))
FC_REFLECT_DERIVED( graphene::chain::dice_bet_contract_object, (graphene::db::object),
                    (dice_bet)(payer)(amount)(pending)(choice))

#define dice_bet_object_type_to_accounts                \
        case dice_bet_object_type: {                    \
            const auto& aobj = dynamic_cast<const dice_bet_object*>(obj); \
            assert( aobj != nullptr );                    \
            accounts.insert( aobj->owner );               \
            break;                                        \
        } case dice_bet_contract_object_type: {           \
            const auto& aobj = dynamic_cast<const dice_bet_contract_object*>(obj); \
            assert( aobj != nullptr );                    \
            accounts.insert( aobj->payer );               \
            break;                                        \
        }                                                 \


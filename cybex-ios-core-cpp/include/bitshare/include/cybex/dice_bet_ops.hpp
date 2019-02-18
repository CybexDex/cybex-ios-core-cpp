#pragma once
#include <graphene/chain/protocol/base.hpp>

namespace graphene { namespace chain {

    struct initiate_dice_bet_operation : public base_operation
    {
        struct fee_parameters_type {
            uint64_t fee = GRAPHENE_BLOCKCHAIN_PRECISION;
        };

        asset               fee;
        account_id_type     owner;
        asset_id_type       asset_to_bet;
        share_type          initial_balance;
        share_type          clearing_threshold;

        share_type          min_amount;
        share_type          max_amount;

        uint16_t            pay_discount_percent;
        uint16_t            max_dice;

        extensions_type     extensions;

        account_id_type     fee_payer() const { return owner; }
        void                validate() const;
    };

    struct deposit_dice_bet_operation : public base_operation
    {
        struct fee_parameters_type {
            uint64_t fee = GRAPHENE_BLOCKCHAIN_PRECISION;
        };

        asset               fee;
        account_id_type     owner;
        dice_bet_id_type    dice_bet;
        share_type          amount;

        extensions_type     extensions;

        account_id_type     fee_payer() const { return owner; }
        void                validate() const;
    };

    struct withdraw_dice_bet_operation : public base_operation
    {
        struct fee_parameters_type {
            uint64_t fee = GRAPHENE_BLOCKCHAIN_PRECISION;
        };

        asset               fee;
        account_id_type     owner;
        dice_bet_id_type    dice_bet;
        share_type          amount;

        extensions_type     extensions;

        account_id_type     fee_payer() const { return owner; }
        void                validate() const;
    };

    struct participate_dice_bet_operation : public base_operation
    {
        struct fee_parameters_type {
            uint64_t fee = GRAPHENE_BLOCKCHAIN_PRECISION;
        };

        asset               fee;
        dice_bet_id_type    dice_bet;
        account_id_type     payer;
        share_type          amount;
        uint16_t            choice;

        extensions_type     extensions;

        account_id_type     fee_payer() const { return payer; }
        void                validate() const;
    };

    struct dice_bet_clearing_operation : public base_operation
    {
        struct fee_parameters_type {};

        dice_bet_clearing_operation() {};

        dice_bet_id_type    dice_bet_id;
        dice_bet_contract_id_type dice_bet_ct_id;
        account_id_type     payer;
        asset_id_type       asset_id;
        asset               fee;
        share_type          amount;
        share_type          refund;
        uint16_t            choice;
        uint16_t            dice;
        uint32_t            block_num;
        fc::time_point_sec  timestamp;

        account_id_type     fee_payer() const { return payer; }
        void                validate() const { FC_ASSERT( !"virtual operation" ); }
        share_type          calculate_fee(const fee_parameters_type& k)const { return 0; }
    };
} } // namespace graphene::chain

FC_REFLECT( graphene::chain::initiate_dice_bet_operation::fee_parameters_type, (fee) )
FC_REFLECT( graphene::chain::initiate_dice_bet_operation, 
            (fee)(owner)(asset_to_bet)(initial_balance)(clearing_threshold)
            (min_amount)(max_amount)(pay_discount_percent)(max_dice)(extensions))
FC_REFLECT( graphene::chain::deposit_dice_bet_operation::fee_parameters_type, (fee) )
FC_REFLECT( graphene::chain::deposit_dice_bet_operation, (fee)(owner)(dice_bet)(amount)(extensions))
FC_REFLECT( graphene::chain::withdraw_dice_bet_operation::fee_parameters_type, (fee) )
FC_REFLECT( graphene::chain::withdraw_dice_bet_operation, (fee)(owner)(dice_bet)(amount)(extensions))
FC_REFLECT( graphene::chain::participate_dice_bet_operation::fee_parameters_type, (fee) )
FC_REFLECT( graphene::chain::participate_dice_bet_operation, (fee)(dice_bet)(payer)(amount)(choice)(extensions))
FC_REFLECT( graphene::chain::dice_bet_clearing_operation::fee_parameters_type,  )
FC_REFLECT( graphene::chain::dice_bet_clearing_operation, (dice_bet_id)(dice_bet_ct_id)(payer)(asset_id)(fee)(amount)(refund)(choice)(dice)(block_num)(timestamp) )

#define db_notify_dice_bet                                       \
    void operator()( const initiate_dice_bet_operation& op )     \
    {                                                            \
        _impacted.insert( op.owner );                            \
    }                                                            \
    void operator()( const deposit_dice_bet_operation& op )      \
    {                                                            \
        _impacted.insert( op.owner );                            \
    }                                                            \
    void operator()( const withdraw_dice_bet_operation& op )     \
    {                                                            \
        _impacted.insert( op.owner );                            \
    }                                                            \
    void operator()( const participate_dice_bet_operation& op )  \
    {                                                            \
        _impacted.insert( op.payer);                             \
    }                                                            \
    void operator()( const dice_bet_clearing_operation& op )     \
    {                                                            \
        _impacted.insert( op.payer);                             \
    }                                                            \

#define impact_visit_dice_bet db_notify_dice_bet


#pragma once
#include <graphene/chain/evaluator.hpp>
#include <graphene/chain/database.hpp>
#include <cybex/dice_bet_ops.hpp>

namespace graphene { namespace chain {

    class initiate_dice_bet_evaluator : public evaluator<initiate_dice_bet_evaluator>
    {
        public:
            typedef initiate_dice_bet_operation operation_type;

            void_result do_evaluate(const initiate_dice_bet_operation &o);
            object_id_type do_apply(const initiate_dice_bet_operation &o);
    };

    class deposit_dice_bet_evaluator: public evaluator<deposit_dice_bet_evaluator>
    {
        public:
            typedef deposit_dice_bet_operation operation_type;

            void_result do_evaluate(const deposit_dice_bet_operation &o);
            void_result do_apply(const deposit_dice_bet_operation &o);

            const dice_bet_object * dice_bet = nullptr;
    };

    class withdraw_dice_bet_evaluator: public evaluator<withdraw_dice_bet_evaluator>
    {
        public:
            typedef withdraw_dice_bet_operation operation_type;

            void_result do_evaluate(const withdraw_dice_bet_operation &o);
            void_result do_apply(const withdraw_dice_bet_operation &o);

            const dice_bet_object * dice_bet = nullptr;
    };

    class participate_dice_bet_evaluator : public evaluator<participate_dice_bet_evaluator>
    {
        public:
            typedef participate_dice_bet_operation operation_type;

            void_result do_evaluate(const participate_dice_bet_operation &o);
            object_id_type do_apply(const participate_dice_bet_operation &o);

            const dice_bet_object * dice_bet = nullptr;
            share_type max_payback = 0;
    };
}}

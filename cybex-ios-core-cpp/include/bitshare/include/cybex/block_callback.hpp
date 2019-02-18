#pragma once
#include <graphene/chain/protocol/base.hpp>
#include <graphene/chain/database.hpp>
namespace graphene {
namespace chain {


class block_callback
{
public:
    block_callback() {}
    void handler(database & db);
private:
   void process_crowdfund(database & db) const ;
   void auto_withdraw(database & db,const crowdfund_object & crowdfund) const;
   void crowdfund_ended(database & db,const crowdfund_object & crowdfund) const;

   void process_dice_bet(database & db) const;
   void dice_bet_clearing(database & db, const dice_bet_object & dice_bet) const;
   uint32_t random_number_from_block(database& db) const;
};




} }

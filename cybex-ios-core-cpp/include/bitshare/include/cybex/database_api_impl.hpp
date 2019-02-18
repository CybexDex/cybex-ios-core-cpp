vector<crowdfund_contract_object> database_api_impl::get_crowdfund_contract_objects( const account_id_type id )const
{
   try
   {
      const auto& crowdfund_contract_idx = _db.get_index_type<crowdfund_contract_index>();
      const auto& by_owner_idx = crowdfund_contract_idx.indices().get<by_owner>();

      vector<crowdfund_contract_object> result;


         auto itr = by_owner_idx.lower_bound( boost::make_tuple( id, crowdfund_id_type(0) ) );
         while( itr != by_owner_idx.end() && itr->owner == id )
         {
            result.push_back( *itr );
            ++itr;
         }
      return result;
   }
   FC_CAPTURE_AND_RETHROW( (id) )
}
vector<crowdfund_object> database_api_impl::get_crowdfund_objects( const account_id_type id )const
{
   try
   {
      const auto& crowdfund_idx = _db.get_index_type<crowdfund_index>();
      const auto& by_owner_idx = crowdfund_idx.indices().get<by_owner>();

      vector<crowdfund_object> result;


         auto itr = by_owner_idx.lower_bound( boost::make_tuple( id, asset_id_type(0) ) );
         while( itr != by_owner_idx.end() && itr->owner == id )
         {
            result.push_back( *itr );
            ++itr;
         }
      return result;
   }
   FC_CAPTURE_AND_RETHROW( (id) )
}
vector<crowdfund_object> database_api_impl::list_crowdfund_objects( const crowdfund_id_type  id,uint32_t limit )const
{
   try
   {
      const auto& crowdfund_idx = _db.get_index_type<crowdfund_index>();
      const auto& by_id_idx = crowdfund_idx.indices().get<by_id>();

      vector<crowdfund_object> result;



      {
         auto itr = by_id_idx.lower_bound(id);
         while( itr != by_id_idx.end() && limit )
         {
            result.push_back( *itr );
            ++itr;
            --limit;
         }
      }
      return result;
   }
   FC_CAPTURE_AND_RETHROW( (id) )
}
vector<crowdfund_contract_object> database_api::get_crowdfund_contract_objects( const account_id_type id )const
{
   return my->get_crowdfund_contract_objects( id );
}
vector<crowdfund_object> database_api::get_crowdfund_objects( const  account_id_type id)const
{
   return my->get_crowdfund_objects( id );
}
vector<crowdfund_object> database_api::list_crowdfund_objects( const crowdfund_id_type id,uint32_t limits)const
{
   return my->list_crowdfund_objects( id,limits );
}

//////////////////////
//     dice bet     //
//////////////////////
vector<dice_bet_object> database_api_impl::get_dice_bet_objects( const account_id_type id) const
{ try {
   const auto & _dice_bet_idx = _db.get_index_type<dice_bet_index>();
   const auto & _by_owner = _dice_bet_idx.indices().get<by_owner>();

   vector<dice_bet_object> result;

   auto range = _by_owner.equal_range(boost::make_tuple(id));

   for ( const dice_bet_object & dice_bet : boost::make_iterator_range(range.first, range.second) )
       result.push_back(dice_bet);

   return result;
} FC_CAPTURE_AND_RETHROW( (id) ) }

vector<dice_bet_object> database_api_impl::list_dice_bet_objects( const dice_bet_id_type id, uint32_t limit) const
{ try {
    const auto & _dice_bet_idx = _db.get_index_type<dice_bet_index>();
    const auto & by_id_idx = _dice_bet_idx.indices().get<by_id>();

    vector<dice_bet_object> result;
    auto itr = by_id_idx.lower_bound(id);
    while( itr != by_id_idx.end() && limit )
    {
        result.push_back( *itr );
        ++itr;
        --limit;
    }
    return result;
} FC_CAPTURE_AND_RETHROW( (id) ) }

vector<dice_bet_object> database_api::get_dice_bet_objects( const account_id_type id) const
{
    return my->get_dice_bet_objects( id );
}

vector<dice_bet_object> database_api::list_dice_bet_objects( const dice_bet_id_type id, uint32_t limit) const
{
    return my->list_dice_bet_objects( id, limit );
}

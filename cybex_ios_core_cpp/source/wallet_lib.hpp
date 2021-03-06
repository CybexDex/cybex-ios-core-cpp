#include <fc/crypto/elliptic.hpp>
#include <fc/io/varint.hpp>
//#include <fc/optional.hpp>
#include <string>
#include "graphene/chain/protocol/types.hpp"

using namespace std;
using namespace fc;
//#include "types.hpp"

//typedef fc::optional<fc::ecc::private_key> cybex_priv_key_type;
//extern cybex_priv_key_type active_priv_key, owner_priv_key, memo_priv_key;
typedef int64_t amount_type;
string get_user_key(string user_name, string password);
string get_active_user_key(string pubkey);
ecc::private_key get_private_key(string public_key);
ecc::private_key get_private_key_with_random(string public_key);
string get_private_key_with_message(string message);
void clear_user_key();
void set_default_public_key(string pub_key_base58_str);
void set_default_private_key(string pri_key_base58_str);
string get_pubkey_from_address(string address);

void set_derived_operation_extensions(
                                 string master_public_key,
                                 string derived_private_key,
                                 string derived_public_key,
                                 uint32_t nonce,
                                 string signature
);
string get_account_update_json(
                               uint16_t ref_block_num,
                               string ref_block_id_hex_str,
                               uint32_t expiration,
                               string chain_id_str,
                               string account_update_op_json
                               );
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
);
string transfer(
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
                string to_memo_pub_key
                );
string transferBBB(
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
                   bool is_two,
                   bool use_gateway,
                   unsigned_int bbb_asset_id,
                   unsigned_int gateway_account_id,
                   string gateway_memo_key
                   );

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
                );

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
                            );

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
                            );

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
);

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
);


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
);

string cancel_order(
                    uint16_t ref_block_num,
                    string ref_block_id_hex_str,
                    uint32_t expiration, /* expiration time in utc seconds */
                    string chain_id_str,
                    
                    unsigned_int order_id, /* instance id of order */
                    unsigned_int fee_paying_account_id, /* instance id of fee paying account */
                    amount_type fee_amount, /* amount of fee */
                    unsigned_int fee_asset_id /* instance id of asset to pay fee */
);

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
);


string get_cancel_order_json(
                             unsigned_int order_id, /* instance id of order */
                             unsigned_int fee_paying_account_id, /* instance id of fee paying account */
                             amount_type fee_amount, /* amount of fee */
                             unsigned_int fee_asset_id /* instance id of asset to pay fee */
);
string get_cancel_all_order_json(
                             unsigned_int sell_asset_id,
                             unsigned_int receive_asset_id,
                             unsigned_int fee_paying_account_id, /* instance id of fee paying account */
                             amount_type fee_amount, /* amount of fee */
                             unsigned_int fee_asset_id /* instance id of asset to pay fee */
);

string cybex_gateway_query(
                           string accountName,
                           string asset,
                           string fundType,
                           uint32_t size,
                           uint32_t offset,
                           uint32_t expiration
                           );

string get_claim_balance_operation (
                          unsigned_int fee_asset_id,
                          amount_type fee_amount,
                          unsigned_int deposit_to_account_id,
                          unsigned_int claimed_id,
                          string to_account_pub_key,
                          unsigned_int claimed_asset_id,
                          amount_type claimed_amount
                          );

string sign_claim_balance(
                          uint16_t ref_block_num,
                          string ref_block_id_hex_str,
                          uint32_t expiration, /* expiration time in utc seconds */
                          string chain_id_str, /* chain id in base58 */
                          unsigned_int fee_asset_id,
                          amount_type fee_amount,
                          unsigned_int deposit_to_account_id,
                          unsigned_int claimed_id,
                          string to_account_pub_key,
                          unsigned_int claimed_asset_id,
                          amount_type claimed_amount
                        );

string sign_message(string message);
string sign_message_from_hex(string message);
string decrypt_memo_data(
                         string memo_json_str
                         );

string transactionId_from_jsonstr(string jsonStr);

string exchange_participate(
                          uint16_t ref_block_num,
                          string ref_block_id_hex_str,
                          uint32_t expiration, /* expiration time in utc seconds */
                          string chain_id_str,

                          unsigned_int payer_id, /* instance id of seller account */
                          unsigned_int asset_id, /* instance id fo asset to be sold */
                          amount_type amount, /* amount to be sold */
                          unsigned_int exchange_id, /* instance id of asset to receive */
                          amount_type fee_amount, /* amount of fee */
                          unsigned_int fee_asset_id /* instance id of asset to pay fee*/
);
string exchange_participate_json(
                                 unsigned_int payer_id,
                                 unsigned_int asset_id,
                                 amount_type amount,
                                 unsigned_int exchange_id,
                                 amount_type fee_amount,
                                 unsigned_int fee_asset_id
                                 );



/* BBB Only */
string amend_order(string ss);

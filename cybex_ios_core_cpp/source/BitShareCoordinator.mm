//
//  BitShareCoordinator.m
//  cybexMobile
//
//  Created by koofrank on 2018/4/23.
//  Copyright © 2018年 Cybex. All rights reserved.
//
#import <Foundation/Foundation.h>
#import "BitShareCoordinator.h"

#include "wallet_lib.hpp"
#include "transaction.hpp"

@implementation BitShareCoordinator

+ (NSString *)getUserKeys:(NSString *)username password:(NSString *)password {
  string keys = get_user_key([username UTF8String], [password UTF8String]);
  
  NSString *result = @(keys.c_str());
  
  return result;
}

+ (NSString *)getActiveUserKeys:(NSString *)publicKey {
    string keys = get_active_user_key([publicKey UTF8String]);

    NSString *result = @(keys.c_str());

    return result;
}

+ (NSString *)updateAccount:(int)block_num block_id:(NSString *)block_id
                 expiration:(NSTimeInterval)expiration chain_id:(NSString *)chain_id
                  operation:(NSString *)operation {
    string jsonString = get_account_update_json(block_num, [block_id UTF8String], expiration, [chain_id UTF8String], [operation UTF8String]);

    return @(jsonString.c_str());
}
  
+ (NSString *)getLimitOrder:(int)block_num block_id:(NSString *)block_id
                 expiration:(NSTimeInterval)expiration chain_id:(NSString *)chain_id
                    user_id:(int)user_id order_expiration:(NSTimeInterval)order_expiration
                   asset_id:(int)asset_id amount:(int64_t)amount receive_asset_id:(int)receive_asset_id
             receive_amount:(int64_t)receive_amount fee_id:(int)fee_id fee_amount:(int64_t)fee_amount fillOrKill:(BOOL)fillOrKill {
  string jsonString = limit_order_create(block_num, [block_id UTF8String], expiration, [chain_id UTF8String], user_id, order_expiration, asset_id, amount, receive_asset_id, receive_amount, @(fillOrKill).intValue, fee_amount, fee_id);
  
  return @(jsonString.c_str());
}

+ (NSString *)getLimitOrderBySide:(bool)is_buy block_num:(int)block_num block_id:(NSString *)block_id
                 expiration:(NSTimeInterval)expiration chain_id:(NSString *)chain_id
                    user_id:(int)user_id order_expiration:(NSTimeInterval)order_expiration
                   asset_id:(int)asset_id amount:(int64_t)amount receive_asset_id:(int)receive_asset_id
             receive_amount:(int64_t)receive_amount fee_id:(int)fee_id fee_amount:(int64_t)fee_amount fillOrKill:(BOOL)fillOrKill {
    string jsonString = limit_order_create_by_side(block_num, [block_id UTF8String], expiration, [chain_id UTF8String], user_id, order_expiration, asset_id, amount, receive_asset_id, receive_amount, @(fillOrKill).intValue, fee_amount, fee_id, is_buy);

    return @(jsonString.c_str());
}
  
+ (NSString *)getLimitOrderOperation:(int)user_id expiration:(NSTimeInterval)expiration asset_id:(int)asset_id amount:(int64_t)amount receive_asset_id:(int)receive_asset_id receive_amount:(int64_t)receive_amount fee_id:(int)fee_id fee_amount:(int64_t)fee_amount fillOrKill:(BOOL)fillOrKill {
  string jsonString= get_limit_order_create_json(user_id, expiration, asset_id, amount, receive_asset_id, receive_amount, @(fillOrKill).intValue, fee_amount, fee_id);
  return @(jsonString.c_str());
}
  
+ (NSString *)cancelLimitOrder:(int)block_num block_id:(NSString *)block_id
                    expiration:(NSTimeInterval)expiration chain_id:(NSString *)chain_id
                       user_id:(int)user_id order_id:(int)order_id
                        fee_id:(int)fee_id fee_amount:(int64_t)fee_amount {
  string jsonString = cancel_order(block_num, [block_id UTF8String], expiration, [chain_id UTF8String], order_id, user_id, fee_amount, fee_id);
  return @(jsonString.c_str());
}
  
+ (NSString *)cancelLimitOrderOperation:(int)order_id user_id:(int)user_id fee_id:(int)fee_id fee_amount:(int64_t)fee_amount {
  string jsonString = get_cancel_order_json(order_id, user_id, fee_amount, fee_id);
  return @(jsonString.c_str());
}

+ (NSString *)cancelAllLimitOrder:(int)block_num block_id:(NSString *)block_id
                       expiration:(NSTimeInterval)expiration chain_id:(NSString *)chain_id
                          user_id:(int)user_id asset_id:(int)asset_id receive_asset_id:(int)receive_asset_id
                           fee_id:(int)fee_id fee_amount:(int64_t)fee_amount {
    string jsonString = cancel_all_order(block_num, [block_id UTF8String], expiration, [chain_id UTF8String], asset_id, receive_asset_id, user_id, fee_amount, fee_id);
    return @(jsonString.c_str());
}

+ (NSString *)cancelAllLimitOrderOperation:(int)asset_id receive_asset_id:(int)receive_asset_id user_id:(int)user_id fee_id:(int)fee_id fee_amount:(int64_t)fee_amount {
    string jsonString = get_cancel_all_order_json(asset_id, receive_asset_id, user_id, fee_amount, fee_id);
    return @(jsonString.c_str());
}

+ (void)setDerivedOperationExtensions:(NSString *)master_public_key derived_private_key:(NSString *)derived_private_key derived_public_key:(NSString *)derived_public_key nonce:(int)nonce signature:(NSString *)signature {
    set_derived_operation_extensions([master_public_key UTF8String], [derived_private_key UTF8String], [derived_public_key UTF8String], nonce, [signature UTF8String]);
}

+ (NSString *)getTransaction:(int)block_num block_id:(NSString *)block_id
                  expiration:(NSTimeInterval)expiration chain_id:(NSString *)chain_id
                from_user_id:(int)from_user_id to_user_id:(int)to_user_id
                    asset_id:(int)asset_id receive_asset_id:(int)receive_asset_id
                      amount:(int64_t)amount fee_id:(int)fee_id
                  fee_amount:(int64_t)fee_amount memo:(NSString *)memo
               from_memo_key:(NSString *)from_memo_key to_memo_key:(NSString *)to_memo_key {
  string jsonString = transfer(block_num, [block_id UTF8String], expiration, [chain_id UTF8String], from_user_id, to_user_id, amount, asset_id, fee_amount, fee_id, [memo UTF8String], [from_memo_key UTF8String], [to_memo_key UTF8String]);
  
  return @(jsonString.c_str());
}

+ (NSString *)getTransactionWithVesting:(int)block_num block_id:(NSString *)block_id
                             expiration:(NSTimeInterval)expiration chain_id:(NSString *)chain_id
                           from_user_id:(int)from_user_id to_user_id:(int)to_user_id
                               asset_id:(int)asset_id receive_asset_id:(int)receive_asset_id
                                 amount:(int64_t)amount fee_id:(int)fee_id fee_amount:(int64_t)fee_amount
                                   memo:(NSString *)memo from_memo_key:(NSString *)from_memo_key to_memo_key:(NSString *)to_memo_key vestingPeroid:(uint64_t)peroid toPubKey:(NSString *)toPubKey {
    string jsonString = transfer_with_vesting(block_num, [block_id UTF8String], expiration, [chain_id UTF8String], from_user_id, to_user_id, amount, asset_id, fee_amount, fee_id, [memo UTF8String], [from_memo_key UTF8String], [to_memo_key UTF8String], peroid, [toPubKey UTF8String]);

    return @(jsonString.c_str());
}

+ (NSString *)getTransactionId:(int)block_num block_id:(NSString *)block_id
                    expiration:(NSTimeInterval)expiration chain_id:(NSString *)chain_id
                  from_user_id:(int)from_user_id to_user_id:(int)to_user_id
                      asset_id:(int)asset_id receive_asset_id:(int)receive_asset_id
                        amount:(int64_t)amount fee_id:(int)fee_id fee_amount:(int64_t)fee_amount memo:(NSString *)memo from_memo_key:(NSString *)from_memo_key to_memo_key:(NSString *)to_memo_key {
    string jsonString = transaction_id(block_num, [block_id UTF8String], expiration, [chain_id UTF8String], from_user_id, to_user_id, amount, asset_id, fee_amount, fee_id, [memo UTF8String], [from_memo_key UTF8String], [to_memo_key UTF8String]);

    return @(jsonString.c_str());
}

+ (NSString *)getTransterOperation:(int)from_user_id to_user_id:(int)to_user_id asset_id:(int)asset_id amount:(int64_t)amount fee_id:(int)fee_id fee_amount:(int64_t)fee_amount memo:(NSString *)memo from_memo_key:(NSString *)from_memo_key to_memo_key:(NSString *)to_memo_key {
  string jsonString = get_transfer_op_json(from_user_id, to_user_id, amount, asset_id, fee_amount, fee_id, [memo UTF8String], [from_memo_key UTF8String], [to_memo_key UTF8String]);
  
  return @(jsonString.c_str());
}

+ (NSString *)getTransterWithVestingOperation:(int)from_user_id to_user_id:(int)to_user_id asset_id:(int)asset_id amount:(int64_t)amount fee_id:(int)fee_id fee_amount:(int64_t)fee_amount memo:(NSString *)memo from_memo_key:(NSString *)from_memo_key to_memo_key:(NSString *)to_memo_key vestingPeroid:(uint64_t)peroid toPubKey:(NSString *)toPubKey {
    string jsonString = get_transfer_with_vesting_op_json(from_user_id, to_user_id, amount, asset_id, fee_amount, fee_id, [memo UTF8String], [from_memo_key UTF8String], [to_memo_key UTF8String], peroid, [toPubKey UTF8String]);

    return @(jsonString.c_str());
}

+ (NSString *)exchangeParticipateJSON:(int)user_id exchange_id:(int)exchange_id
                             asset_id:(int)asset_id amount:(int64_t)amount
                               fee_id:(int)fee_id fee_amount:(int64_t)fee_amount {
    string jsonString = exchange_participate_json(user_id, asset_id, amount, exchange_id, fee_amount, fee_id);
    return @(jsonString.c_str());

}

+ (NSString *)exchangeParticipate:(int)block_num block_id:(NSString *)block_id
                       expiration:(NSTimeInterval)expiration chain_id:(NSString *)chain_id
                          user_id:(int)user_id exchange_id:(int)exchange_id
                         asset_id:(int)asset_id amount:(int64_t)amount
                           fee_id:(int)fee_id fee_amount:(int64_t)fee_amount {
    string jsonString = exchange_participate(block_num, [block_id UTF8String], expiration, [chain_id UTF8String], user_id, asset_id, amount, exchange_id, fee_amount, fee_id);
    return @(jsonString.c_str());
}

+ (NSString *)transactionIdFromSigned:(NSString *)jsonStr {
    string jsonString = transactionId_from_jsonstr([jsonStr UTF8String]);
    return @(jsonString.c_str());
}

+ (void)resetDefaultPublicKey:(NSString *)str {
  set_default_public_key([str UTF8String]);
}

+ (void)setDefaultPrivateKey:(NSString *)str {
    set_default_private_key([str UTF8String]);
}
  
+ (void)cancelUserKey {
  clear_user_key();
}
  
  
+ (NSString *)getRecodeLoginOperation:(NSString *)accountName asset:(NSString *)asset fundType:(NSString *)fundType size:(int)size offset:(int)offset expiration:(int)expiration {
    
  string jsonString = cybex_gateway_query([accountName UTF8String], [asset UTF8String], [fundType UTF8String], size, offset, expiration);
  return @(jsonString.c_str());
}
  
+ (NSString *)getMemo:(NSString *)memo {
  string jsonString = decrypt_memo_data([memo UTF8String]);
  return @(jsonString.c_str());
}

+ (NSString *)sign:(NSString *)str {
    string jsonString = sign_message([str UTF8String]);

    return @(jsonString.c_str());
}

+ (NSString *)signFromHex:(NSString *)str {
    string jsonString = sign_message_from_hex([str UTF8String]);

    return @(jsonString.c_str());
}

+ (NSString *)signMessage:(NSString *)username message:(NSString *)message {
    string jsonString = sign_message([[NSString stringWithFormat:@"%@_%@", username, message] UTF8String]);

    return @(jsonString.c_str());
}

+ (NSString *)getClaimedSign:(int)block_num
                    block_id:(NSString *)block_id
                  expiration:(NSTimeInterval)expiration
                    chain_id:(NSString *)chain_id
                fee_asset_id:(int)fee_asset_id
                  fee_amount:(int64_t)fee_amount
       deposit_to_account_id:(int)deposit_to_account_id
                  claimed_id:(int)claimed_id
            claimed_asset_id:(int)claimed_asset_id
              claimed_amount:(int64_t)claimed_amount
          claimed_own:(NSString *)claimed_own {
    string jsonString = sign_claim_balance(block_num, [block_id UTF8String], expiration, [chain_id UTF8String], fee_asset_id, fee_amount, deposit_to_account_id, claimed_id, [claimed_own UTF8String], claimed_asset_id, claimed_amount);
    return @(jsonString.c_str());
}

+ (NSString *)getClaimedOperation:(int)fee_asset_id
                       fee_amount:(int64_t)fee_amount
            deposit_to_account_id:(int)deposit_to_account_id
                       claimed_id:(int)claimed_id
                 claimed_asset_id:(int)claimed_asset_id
                   claimed_amount:(int64_t)claimed_amount
               claimed_own:(NSString *)claimed_own {
    string jsonString = get_claim_balance_operation(fee_asset_id, fee_amount, deposit_to_account_id, claimed_id, [claimed_own UTF8String], claimed_asset_id, claimed_amount);
    return @(jsonString.c_str());
}

+ (NSString *)generatePrivateKey:(NSString *)message {
    return @(get_private_key_with_message([message UTF8String]).c_str());
}

@end

//
//  NXSig.m
//  cybex_ios_core_cpp
//
//  Created by koofrank on 2019/7/1.
//  Copyright © 2019 com.nbltrustdev. All rights reserved.
//

#import "NXSig.h"
#include "wallet_lib.hpp"
#include "transaction.hpp"

@implementation NXSig

+ (NSString *)amendOrderWith:(NSString *)refId
                   cutLossPx:(NSString *)cutLossPx
                takeProfitPx:(NSString *)takeProfit
                   execNowPx:(NSString *)execNowPx
                  expiration:(NSString *)expiration
                      seller:(NSString *)seller {
    string json_str = amend_order([refId UTF8String], [cutLossPx UTF8String], [takeProfit UTF8String], [execNowPx UTF8String], [expiration UTF8String], [seller UTF8String]);
    return @(json_str.c_str());
    }
@end

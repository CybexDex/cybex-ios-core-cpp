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

+ (NSString *)amendOrderWith:(NSString *)streamString {
    string json_str = amend_order([streamString UTF8String]);
    return @(json_str.c_str());
    }
@end

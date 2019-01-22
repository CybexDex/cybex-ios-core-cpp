//
//  walletExtern.h
//  cybex-ios-core-cpp
//
//  Created by koofrank on 2019/1/21.
//  Copyright © 2019 com.nbltrustdev. All rights reserved.
//

#ifndef walletExtern_h
#define walletExtern_h

#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif
    const char *get_user_key(const char *user_name, const char *password);

#ifdef __cplusplus
}
#endif

#endif /* walletExtern_h */

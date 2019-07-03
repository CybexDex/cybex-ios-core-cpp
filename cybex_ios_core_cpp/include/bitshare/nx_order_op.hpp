//
//  nx_order_op.hpp
//  cybex_ios_core_cpp
//
//  Created by koofrank on 2019/7/1.
//  Copyright © 2019 com.nbltrustdev. All rights reserved.
//

#ifndef nx_order_op_hpp
#define nx_order_op_hpp

#include <stdio.h>

#include <fc/reflect/reflect.hpp>
#include "graphene/chain/protocol/types.hpp"

using namespace fc;


struct nx_order_amend_operation
{
    string ss;
};

struct nx_order_amend_transaction
{
    struct nx_order_amend_operation op;
    graphene::chain::signature_type signer;
};

FC_REFLECT(nx_order_amend_operation, (ss))
FC_REFLECT(nx_order_amend_transaction, (op)(signer))


#endif /* nx_order_op_hpp */

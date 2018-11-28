//
//  ViewController.swift
//  cybex-ios-core-cpp-demo
//
//  Created by koofrank on 2018/10/19.
//  Copyright © 2018 com.nbltrustdev. All rights reserved.
//

import UIKit
import cybex_ios_core_cpp

class ViewController: UIViewController {

    override func viewDidLoad() {
        super.viewDidLoad()

        login()
        testSignMessage()
        testClaim()
    }

    func login() {
        let keys = BitShareCoordinator.getUserKeys("cybex-test", password: "cybextest123456")
        print(keys)
    }

    func testSignMessage() {
        let signString = BitShareCoordinator.signMessage("xxx", message: "xxx")

        print(signString)

    }

    func testClaim() {
        let sign = BitShareCoordinator.getClaimedSign(0, block_id: "", expiration: 600, chain_id: "", fee_asset_id: 0, fee_amount: 0, deposit_to_account_id: 0, claimed_id: 0, claimed_asset_id: 0, claimed_amount: 0, to_account_pub_key: "CYB6G6u3jAYXL1Vt81vMh1r4CKaxkPgHwXdfrUDFfqDdPPSqJQLTH")

        print(sign)
    }
}


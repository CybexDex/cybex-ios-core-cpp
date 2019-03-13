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
//        testSignMessage()
//        testClaim()
//
//        testTransactionId()
//        testTransactionWithMemoOperation()
        getActiveKeys()
        testDerivedOperation()
        testSignMessage()
    }

    func testDerivedOperation() {
        let pri = BitShareCoordinator.generatePrivateKey("xxx")

        BitShareCoordinator.setDerivedOperationExtensions("CYB7LBj58E9EPuSpgyF46PeCVUvZXBT8WEmGSuRc6nNNKWH9Rg5UZ", derived_private_key: pri, derived_public_key: "CYB5GbPs1FoZMUA3SJe44kUz7XQzMGAMdbjfNkV2ts7EjXyAnrc3j", nonce: 32, signature: "202f2bb55b1fe76709e51588c6e64155a09e13241b481cc72ce9d557287641cbf17c399f7b44c3ef240b6b3201b718686ef688ee09a0529e24e04c44dd54b4b1ef")
//        BitShareCoordinator.setDerivedOperationExtensions("CYB7LBj58E9EPuSpgyF46PeCVUvZXBT8WEmGSuRc6nNNKWH9Rg5UZ", derived_private_key: "", derived_public_key: "CYB5GbPs1FoZMUA3SJe44kUz7XQzMGAMdbjfNkV2ts7EjXyAnrc3j", nonce: 100, signature: "202f2bb55b1fe76709e51588c6e64155a09e13241b481cc72ce9d557287641cbf17c399f7b44c3ef240b6b3201b718686ef688ee09a0529e24e04c44dd54b4b1ef")
//        let transaction = BitShareCoordinator.getTransaction(0, block_id: "0", expiration: 600, chain_id: "0", from_user_id: 0, to_user_id: 0, asset_id: 0, receive_asset_id: 0, amount: 0, fee_id: 0, fee_amount: 0, memo: "", from_memo_key: "", to_memo_key: "")
//        print(transaction)
    }

    func login() {
        let keys = BitShareCoordinator.getUserKeys("cybex-test", password: "cybextest123456")
        print(keys)
    }

    func getActiveKeys() {
        let keys = BitShareCoordinator.getActiveUserKeys("CYB6G6u3jAYXL1Vt81vMh1r4CKaxkPgHwXdfrUDFfqDdPPSqJQLTH")
        print(keys)
    }

    func testSignMessage() {
        let signString = BitShareCoordinator.signMessage("xxx", message: "xxx")

        print(signString)

    }

    func testClaim() {
        let sign = BitShareCoordinator.getClaimedSign(0, block_id: "", expiration: 600, chain_id: "", fee_asset_id: 0, fee_amount: 0, deposit_to_account_id: 0, claimed_id: 0, claimed_asset_id: 0, claimed_amount: 0, claimed_own: "CYBLanbfQMeMHCkowkpD7CDV2t36WfXfLnrh")

        print(sign)
    }

    func testTransactionWithMemoOperation() {
        let operation = BitShareCoordinator.getTransterOperation(0, to_user_id: 0, asset_id: 0, amount: 0, fee_id: 0, fee_amount: 100, memo: "", from_memo_key: "", to_memo_key: "")
        print(operation)
    }

    func testTransactionId() {
        let id = BitShareCoordinator.getTransactionId(0, block_id: "", expiration: 600, chain_id: "", from_user_id: 0, to_user_id: 0, asset_id: 0, receive_asset_id: 0, amount: 0, fee_id: 0, fee_amount: 0, memo: "", from_memo_key: "", to_memo_key: "")
        print(id)
    }

    func testCancelAllOrders() {
        let operation = BitShareCoordinator.cancelAllLimitOrder(0, block_id: "", expiration: 600, chain_id: "", user_id: 27765, asset_id: 10, receive_asset_id: 20, fee_id: 0, fee_amount: 10000)
        print(operation)
    }
}


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
//        testAmendOrder()
//        testTransactionId()
//        testTransactionWithMemoOperation()
//        getActiveKeys()
//        testDerivedOperation()
//        testUpdateAccount()
//        testParticipateExchange()
        getTransactionIdFromSigned(str: testAmendOrder())
    }

    func testAmendOrder() -> String {
        let result = NXSig.amendOrder(with: "7e4fb06fa75bd1f8e49aeee3f4862914db678fdb")
        print(result)
        return result
    }


    func testParticipateExchange() {
        let jsonStr = BitShareCoordinator.exchangeParticipate(0, block_id: "0", expiration: 600, chain_id: "0", user_id: 0, exchange_id: 0, asset_id: 2, amount: 20, fee_id: 0, fee_amount: 40)
        print(jsonStr)
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

    func testUpdateAccount() {
        let operation = """
        {"fee": { "amount": 105, "asset_id": "1.3.0" },"account": "1.2.28828","active": { "weight_threshold": 1, "account_auths": [], "key_auths": [ [ "CYB6PGuvsw3u1Z2AgfyadLkAMYXWiUbtRSeA6q4z5ZJ8TqRnhHEAb", 1 ], [ "CYB7ef4mgvLCMU1VERyE6CzZcm8q2WfjrXk5z3nDVL83mUDbSbZmg", 1 ] ], "address_auths": [] },"new_options": { "memo_key": "CYB6PGuvsw3u1Z2AgfyadLkAMYXWiUbtRSeA6q4z5ZJ8TqRnhHEAb", "voting_account": "1.2.5", "num_witness": 0, "num_committee": 0, "votes": [], "extensions": [] },"extensions": {} }
        """
        let json = BitShareCoordinator.updateAccount(0, block_id: "", expiration: 600, chain_id: "", operation: operation)
        print(json)
    }

    func testSignMessage() {
        let signString = BitShareCoordinator.signMessage("xxx", message: "xxx")

        print(signString)

    }

    func testClaim() -> String {
        let sign = BitShareCoordinator.getClaimedSign(40, block_id: "2", expiration: 600, chain_id: "3", fee_asset_id: 0, fee_amount: 50, deposit_to_account_id: 10, claimed_id: 10, claimed_asset_id: 20, claimed_amount: 550, claimed_own: "CYBLanbfQMeMHCkowkpD7CDV2t36WfXfLnrh")

        return sign
    }

    func testTransactionWithMemoOperation() -> String {
        let operation = BitShareCoordinator.getTransterOperation(0, to_user_id: 20, asset_id: 0, amount: 20, fee_id: 0, fee_amount: 100, memo: "xxx", from_memo_key: "", to_memo_key: "")
        return operation
    }

    func testTransactionId() {
        let id = BitShareCoordinator.getTransactionId(0, block_id: "", expiration: 600, chain_id: "", from_user_id: 0, to_user_id: 0, asset_id: 0, receive_asset_id: 0, amount: 0, fee_id: 0, fee_amount: 0, memo: "", from_memo_key: "", to_memo_key: "")
        print(id)
    }

    func getTransactionIdFromSigned(str: String) {
        let id = BitShareCoordinator.transactionId(fromSigned: str)
        print(id)
    }

    func testCancelAllOrders() {
        let operation = BitShareCoordinator.cancelAllLimitOrder(0, block_id: "", expiration: 600, chain_id: "", user_id: 27765, asset_id: 10, receive_asset_id: 20, fee_id: 0, fee_amount: 10000)
        print(operation)
    }
}


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
    }

    func login() {
        let keys = BitShareCoordinator.getUserKeys("cybex-test", password: "cybextest123456")
        print(keys)
    }

    func testSignMessage() {
        let signString = BitShareCoordinator.signMessage("xxx", message: "xxx")

        print(signString)

    }
}


// swift-tools-version:5.0
import PackageDescription

let package = Package(
    name: "cybex-ios-core-cpp",
    platforms: [
        .iOS(.v11),
    ],
    products: [
        .library(
            name: "cybex-ios-core-cpp",
            targets: ["cybex_ios_core_cpp"]),
    ],
    dependencies: [
        // no dependencies
    ],
    targets: [
        .target(
            name: "cybex-ios-core-cpp",
            dependencies: []),
    ]
)

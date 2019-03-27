#!/bin/bash

######################
# Create log file
######################
exec > /tmp/UniversalBuild_Log_$(date +"%Y%m%d%H%M%S").log 2>&1

# exit on failure
set -ex

######################
# Param setup
######################
TARGET_NAME=cybex_ios_core_cpp
SCHEME=cybex_ios_core_cpp
CONFIGURATION=Release
PROJECT_NAME=cybex-ios-core-cpp
SRCROOT=.
SYMROOT=$(xcodebuild -scheme ${SCHEME} -showBuildSettings | grep -w BUILD_DIR | awk -F' = ' '/BUILD_DIR =/{print $2}')
TARGET_NAME="${TARGET_NAME//-/_}"

echo "BUILD_DIR is '${SYMROOT}'"

######################
# Build with both targets
######################
echo "Clean ${TARGET_NAME} for simulator"
xcodebuild -destination 'platform=iOS Simulator,name=iPhone X,OS=latest' -scheme ${SCHEME} clean
echo "Clean ${TARGET_NAME} for generic device"
xcodebuild -configuration ${CONFIGURATION} -destination generic/platform=iOS -scheme ${SCHEME} clean

echo "Build ${PROJECT_NAME} for simulator"
xcrun xcodebuild -project ${PROJECT_NAME}.xcodeproj -scheme ${SCHEME} -configuration ${CONFIGURATION} -destination 'platform=iOS Simulator,name=iPhone X,OS=latest' -sdk iphonesimulator

echo "Build ${PROJECT_NAME} for generice device"
xcrun xcodebuild -project ${PROJECT_NAME}.xcodeproj -scheme ${SCHEME} -configuration ${CONFIGURATION} -destination generic/platform=iOS -sdk iphoneos

######################
# Options
######################

BUILD_PRODUCTS="${SYMROOT}"
DEVICE_BIN="${BUILD_PRODUCTS}/${CONFIGURATION}-iphoneos/${EXECUTABLE_PATH}"
SIMULATOR_BIN="${BUILD_PRODUCTS}/${CONFIGURATION}-iphonesimulator/${EXECUTABLE_PATH}"

ARCHIVE_PATH="${SRCROOT}/_Archive"
rm -rf "${ARCHIVE_PATH}"
mkdir "${ARCHIVE_PATH}"

echo "product path:${DEVICE_BIN}"

echo "simu path : ${SIMULATOR_BIN}"

######################
# Copy frameworks
######################
if [ "${CONFIGURATION}" = "Release" ]; then
	if [ -d "${DEVICE_BIN}" ]; then
		DEVICE_PATH="${ARCHIVE_PATH}/Release"
		mkdir "${DEVICE_PATH}"
		cp -r "${DEVICE_BIN}" "${DEVICE_PATH}"
		
		echo "Copy device framework done"
	else
		echo "Device framework doesn't exist in given path"
	fi

	if [ -d "${SIMULATOR_BIN}" ]; then
		SIMULATOR_PATH="${ARCHIVE_PATH}/Debug"
		mkdir "${SIMULATOR_PATH}"
		cp -r "${SIMULATOR_BIN}" "${SIMULATOR_PATH}"
		echo "Copy simulator framework done"
	else
		echo "Simulator framework doesn't exist in given path"
	fi
	
######################
# Merge into universal framework
######################
	UNIVERSAL_PATH="${ARCHIVE_PATH}/Universal"
	mkdir "${UNIVERSAL_PATH}"
	cp -r "${SIMULATOR_BIN}" "${UNIVERSAL_PATH}"
	if [ -d "${DEVICE_BIN}${TARGET_NAME}.framework/Modules/${TARGET_NAME}.swiftmodule/" ]; then
		cp -f ${DEVICE_BIN}${TARGET_NAME}.framework/Modules/${TARGET_NAME}.swiftmodule/* "${UNIVERSAL_PATH}/${TARGET_NAME}.framework/Modules/${TARGET_NAME}.swiftmodule/" | echo
	fi

	lipo -create "${DEVICE_BIN}${TARGET_NAME}.framework/${TARGET_NAME}" "${SIMULATOR_BIN}${TARGET_NAME}.framework/${TARGET_NAME}" -output  "${UNIVERSAL_PATH}/${TARGET_NAME}.framework/${TARGET_NAME}"
	zip -r "${UNIVERSAL_PATH}/${TARGET_NAME}.zip" "${UNIVERSAL_PATH}/${TARGET_NAME}.framework" 
	echo "Universal framework created successfully"
fi

exit 0
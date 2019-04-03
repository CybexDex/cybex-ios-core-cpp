#
#  Be sure to run `pod spec lint cybex-ios-core.podspec' to ensure this is a
#  valid spec and to remove all comments including this before submitting the spec.
#
#  To learn more about Podspec attributes see https://guides.cocoapods.org/syntax/podspec.html
#  To see working Podspecs in the CocoaPods repo see https://github.com/CocoaPods/Specs/
#

Pod::Spec.new do |spec|

  spec.name         = "cybex-ios-core"
  spec.version      = "2.0.2"
  spec.summary      = "cybex crypto with cpp"

  spec.description  = <<-DESC
                  cybex signature helper with cpp
                   DESC

  spec.homepage     = "https://github.com/CybexDex/cybex-ios-core-cpp"
  # spec.screenshots  = "www.example.com/screenshots_1.gif", "www.example.com/screenshots_2.gif"

  spec.license      = { :type => 'MIT', :file => 'LICENSE' }

  spec.author             = { "koofrank" => "koofranker@gmail.com" }
  spec.social_media_url   = "https://twitter.com/koofrank"

  spec.swift_version = "5.0"
  spec.platform     = :ios, "10.0"
  spec.module_name = "cybex_ios_core_cpp"
  
  #  When using multiple platforms
  # spec.ios.deployment_target = "5.0"
  # spec.osx.deployment_target = "10.7"
  # spec.watchos.deployment_target = "2.0"
  # spec.tvos.deployment_target = "9.0"


  # ――― Source Location ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #
  #
  #  Specify the location from where the source should be retrieved.
  #  Supports git, hg, bzr, svn and HTTP.
  #

  spec.source       = { :git => "https://github.com/CybexDex/cybex-ios-core-cpp.git", :tag => "#{spec.version}" }


  # ――― Source Code ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #
  #
  #  CocoaPods is smart about how it includes source code. For source files
  #  giving a folder will include any swift, h, m, mm, c & cpp files.
  #  For header files it will include any header in the folder.
  #  Not including the public_header_files will make all headers public.
  #

  spec.source_files  = "#{spec.module_name}/include/**/*", "#{spec.module_name}/cybex_ios_core_cpp.h", "#{spec.module_name}/source/*.{cpp,m,h,hpp}", "#{spec.module_name}/include/openssl/include/**/*"
  #spec.exclude_files = "Classes/Exclude"

  spec.public_header_files = "#{spec.module_name}/source/BitShareCoordinator.h"
  #spec.private_header_files = "#{spec.module_name}/include/**/*.hpp"
  #spec.header_mappings_dir = "#{spec.module_name}/include/**"

  # ――― Resources ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #
  #
  #  A list of resources included with the Pod. These are copied into the
  #  target bundle with a build phase script. Anything else will be cleaned.
  #  You can preserve files from being cleaned, please don't preserve
  #  non-essential files like tests, examples and documentation.
  #

  # spec.resource  = "icon.png"
  # spec.resources = "Resources/*.png"

  # spec.preserve_paths = "FilesToSave", "MoreFilesToSave"

  # ――― Project Linking ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #
  #
  #  Link your library with frameworks, or libraries. Libraries do not include
  #  the lib prefix of their name.
  #

  # spec.frameworks = "SomeFramework", "AnotherFramework"

  # spec.library   = "iconv"
  spec.vendored_libraries = "#{spec.module_name}/library/*.a"

  spec.requires_arc = true
  dir = File.dirname(__FILE__) + "/#{spec.module_name}"
  spec.xcconfig = { "HEADER_SEARCH_PATHS" => ["#{dir}/include/bitshare/include", "#{dir}/include/libfc/include", "#{dir}/include/openssl/include", "#{dir}/include/boost/include"],
  'ENABLE_BITCODE' => 'NO',
  'VALID_ARCHS' => 'arm64'
   }

end

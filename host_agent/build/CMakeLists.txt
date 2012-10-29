cmake_minimum_required( VERSION 2.8 )
project( host_agent )
enable_testing()

include( ${MASA_MACROS} )

# 4127 conditional expression is constant
add_flag( CMAKE_CXX_FLAGS "/wd4127" )
# 4505 'function' : unreferenced local function has been removed
add_flag( CMAKE_CXX_FLAGS "/wd4505" )
# set warnings as errors
add_flag( CMAKE_CXX_FLAGS "/WX" )

set( root_dir ${CMAKE_SOURCE_DIR}/.. )

set( COMPANY "MASA Group" )
set( PRODUCT "Sword Cloud" )

include_directories( ${root_dir}/src/libraries )
add_definitions( -D_CRT_SECURE_NO_WARNINGS )
add_definitions( -D_SCL_SECURE_NO_WARNINGS )
add_definitions( -DBOOST_SPIRIT_THREADSAFE )

if( WIN32 )
    add_definitions( -D_WIN32_WINNT=0x0600 )
    add_definitions( -DWINVER=0x0600 )
endif()

set( Boost_USE_STATIC_LIBS ON )
find_package( Boost COMPONENTS
    chrono
    date_time
    filesystem
    iostreams
    regex
    system
    thread
    unit_test_framework
    REQUIRED )
masa_find_with_prefix( ZLIB zlib REQUIRED )
set( OPENSSL_LIBRARIES libeay32 ssleay32 )
masa_find_qt4( REQUIRED QtCore QtGui QtNetwork )

################################################################################
# runtime
################################################################################
masa_glob_dir( files src
    "${root_dir}/src/libraries/runtime/*.h"
    "${root_dir}/src/libraries/runtime/*.cpp"
)
if( MSVC )
    masa_glob_dir( win32_files win32
        "${root_dir}/src/libraries/runtime/win32/*.cpp"
        "${root_dir}/src/libraries/runtime/win32/*.h"
    )
    list( APPEND files ${win32_files} )
endif()
masa_add_static_library( runtime ${files} )
masa_set_target_folder( runtime libraries )
if( MSVC )
    masa_add_target_external_library( runtime archive_static_d archive_static )
    masa_add_target_external_library( runtime zlibd zlib )
    target_link_libraries( runtime shlwapi )
endif()

################################################################################
# web_server
################################################################################
masa_glob_dir( files src
    "${root_dir}/src/libraries/web/*.h"
    "${root_dir}/src/libraries/web/*.cpp"
)
masa_add_static_library( web ${files} )
masa_set_target_folder( web libraries )
if( MSVC )
    masa_add_target_external_library( web cpp-netlib_client-${platform}-mt_d cpp-netlib_client-${platform}-mt )
    masa_add_target_external_library( web cpp-netlib_uri-${platform}-mt_d cpp-netlib_uri-${platform}-mt )
    masa_add_target_external_library( web mongoose_d mongoose )
    target_link_libraries( web ws2_32 )
endif()
target_link_libraries( web ${OPENSSL_LIBRARIES} )

################################################################################
# host
################################################################################
masa_glob_dir( files src
    "${root_dir}/src/libraries/host/*.h"
    "${root_dir}/src/libraries/host/*.cpp"
)
masa_add_static_library( host ${files} )
masa_set_target_folder( host libraries )
if( MSVC )
    masa_add_target_library( host bcrypt )
    masa_add_target_library( host sqlite3 )
endif()

################################################################################
# cloud_server
################################################################################
masa_glob_dir( files src
    "${root_dir}/src/applications/cloud_server/*.cpp"
)
masa_configure_rc( cloud_server files res
    "${root_dir}/src/applications/cloud_server/version.rc.in"
)
masa_add_executable_target( cloud_server ${files} )
masa_set_target_folder( cloud_server applications )
target_link_libraries( cloud_server
    host
    runtime
    web
)

################################################################################
# client
################################################################################
set( root "${root_dir}/src/libraries/client" )
masa_glob_dir( files src "${root}/*.cpp" )
masa_glob_qt4_headers( files src "${root}/*.h" )
masa_glob_qt4_ui( files ui "${root}/*.ui" )
masa_add_static_library( client ${files} )
masa_set_target_folder( client libraries )
masa_set_target_qt4( client )

################################################################################
# cloud_client
################################################################################
set( root "${root_dir}/src/applications/cloud_client" )
masa_glob_dir( files src "${root}/*.cpp" )
masa_configure_rc( cloud_client files res "${root}/version.rc.in" )
masa_glob_qt4_rc( files res "${root}/*.qrc" )
masa_add_executable_target( cloud_client WIN32 ${files} )
masa_set_target_folder( cloud_client applications )
masa_set_target_qt4( cloud_client )
target_link_libraries( cloud_client
    client
    host
    runtime
)

################################################################################
# runtime_test
################################################################################
masa_glob_dir( files src
    "${root_dir}/src/tests/runtime_test/*.cpp"
    "${root_dir}/src/tests/runtime_test/*.h"
)
if( WIN32 )
    masa_glob_dir( win32_files win32
        "${root_dir}/src/tests/runtime_test/Win32*.cpp"
        "${root_dir}/src/tests/runtime_test/Win32*.h"
    )
    list( REMOVE_ITEM files ${win32_files} )
    list( APPEND files ${win32_files} )
endif()
masa_add_executable_target( runtime_test ${files} )
masa_set_target_folder( runtime_test tests )
target_link_libraries( runtime_test
    runtime
)
masa_add_test_target( runtime_test runtime
    "--data_directory=${root_dir}/src/tests/runtime_test" )

################################################################################
# host_test
################################################################################
masa_glob_dir( files src
    "${root_dir}/src/tests/host_test/*.cpp"
    "${root_dir}/src/tests/host_test/*.h"
)
masa_add_executable_target( host_test ${files} )
masa_set_target_folder( host_test tests )
target_link_libraries( host_test
    host
    runtime
    web
)
masa_add_test_target( host_test host
   "--data_directory=${root_dir}/src/tests/host_test" )

################################################################################
# web_test
################################################################################
masa_glob_dir( files src
    "${root_dir}/src/tests/web_test/*.cpp"
    "${root_dir}/src/tests/web_test/*.h"
)
masa_add_executable_target( web_test ${files} )
masa_set_target_folder( web_test tests )
target_link_libraries( web_test
    runtime
    web
)
masa_add_test_target( web_test web
    "--data_directory=${root_dir}/src/tests/web_test" )

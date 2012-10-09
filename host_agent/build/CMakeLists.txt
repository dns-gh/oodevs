cmake_minimum_required( VERSION 2.8 )
project( host_agent )
enable_testing()

include( ${MASA_MACROS} )

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
find_with_prefix( ZLIB zlib REQUIRED )
find_package( OpenSSL REQUIRED )
find_qt4( REQUIRED QtCore QtGui QtNetwork )

################################################################################
# runtime
################################################################################
glob_dir( files src
    "${root_dir}/src/libraries/runtime/*.h"
    "${root_dir}/src/libraries/runtime/*.cpp"
)
if( MSVC )
    glob_dir( win32_files win32
        "${root_dir}/src/libraries/runtime/win32/*.cpp"
        "${root_dir}/src/libraries/runtime/win32/*.h"
    )
    list( APPEND files ${win32_files} )
endif()
add_static_library( runtime ${files} )
set_target_folder( runtime libraries )
if( MSVC )
    add_target_external_library( runtime archive_static_d archive_static )
    add_target_external_library( runtime zlibd zlib )
    target_link_libraries( runtime shlwapi )
endif()

################################################################################
# web_server
################################################################################
glob_dir( files src
    "${root_dir}/src/libraries/web/*.h"
    "${root_dir}/src/libraries/web/*.cpp"
)
add_static_library( web ${files} )
set_target_folder( web libraries )
if( MSVC )
    add_target_external_library( web cpp-netlib_client-${platform}-mt_d cpp-netlib_client-${platform}-mt )
    add_target_external_library( web cpp-netlib_uri-${platform}-mt_d cpp-netlib_uri-${platform}-mt )
    add_target_external_library( web mongoose_d mongoose )
    target_link_libraries( web ws2_32 )
endif()
target_link_libraries( web ${OPENSSL_LIBRARIES} )

################################################################################
# host
################################################################################
glob_dir( files src
    "${root_dir}/src/libraries/host/*.h"
    "${root_dir}/src/libraries/host/*.cpp"
)
add_static_library( host ${files} )
set_target_folder( host libraries )
if( MSVC )
    add_target_external_library( host bcrypt-${platform}_d bcrypt-${platform} )
    add_target_external_library( host libsqlite3-${platform}_d libsqlite3-${platform} )
endif()

################################################################################
# cloud_server
################################################################################
glob_dir( files src
    "${root_dir}/src/applications/cloud_server/*.cpp"
)
configure_rc( cloud_server files res
    "${root_dir}/src/applications/cloud_server/version.rc.in"
)
add_executable_target( cloud_server ${files} )
set_target_folder( cloud_server applications )
target_link_libraries( cloud_server
    host
    runtime
    web
)

################################################################################
# client
################################################################################
set( root "${root_dir}/src/libraries/client" )
glob_dir( files src "${root}/*.cpp" )
glob_qt4_headers( files src "${root}/*.h" )
glob_qt4_ui( files ui "${root}/*.ui" )
add_static_library( client ${files} )
set_target_folder( client libraries )
set_target_qt4( client )

################################################################################
# cloud_client
################################################################################
set( root "${root_dir}/src/applications/cloud_client" )
glob_dir( files src "${root}/*.cpp" )
configure_rc( cloud_client files res "${root}/version.rc.in" )
glob_qt4_rc( files res "${root}/*.qrc" )
add_executable_target( cloud_client WIN32 ${files} )
set_target_folder( cloud_client applications )
set_target_qt4( cloud_client )
target_link_libraries( cloud_client
    client
    host
    runtime
)

################################################################################
# runtime_test
################################################################################
glob_dir( files src
    "${root_dir}/src/tests/runtime_test/*.cpp"
    "${root_dir}/src/tests/runtime_test/*.h"
)
if( WIN32 )
    glob_dir( win32_files win32
        "${root_dir}/src/tests/runtime_test/Win32*.cpp"
        "${root_dir}/src/tests/runtime_test/Win32*.h"
    )
    list( REMOVE_ITEM files ${win32_files} )
    list( APPEND files ${win32_files} )
endif()
add_executable_target( runtime_test ${files} )
set_target_folder( runtime_test tests )
target_link_libraries( runtime_test
    runtime
)
add_test_target( runtime_test runtime
    "--data_directory=${root_dir}/src/tests/runtime_test" )

################################################################################
# host_test
################################################################################
glob_dir( files src
    "${root_dir}/src/tests/host_test/*.cpp"
    "${root_dir}/src/tests/host_test/*.h"
)
add_executable_target( host_test ${files} )
set_target_folder( host_test tests )
target_link_libraries( host_test
    host
    runtime
    web
)
add_test_target( host_test host
   "--data_directory=${root_dir}/src/tests/host_test" )

################################################################################
# web_test
################################################################################
glob_dir( files src
    "${root_dir}/src/tests/web_test/*.cpp"
    "${root_dir}/src/tests/web_test/*.h"
)
add_executable_target( web_test ${files} )
set_target_folder( web_test tests )
target_link_libraries( web_test
    runtime
    web
)
add_test_target( web_test web
    "--data_directory=${root_dir}/src/tests/web_test" )

cmake_minimum_required( VERSION 2.8 )
project( host_agent )
enable_testing()

include( MasaMacros.cmake )

set( root_dir ${CMAKE_SOURCE_DIR}/.. )
set( run_dir  ${root_dir}/run/${platform} )

include_directories( ${root_dir}/src/libraries )
add_definitions( -D_CRT_SECURE_NO_WARNINGS )
add_definitions( -D_SCL_SECURE_NO_WARNINGS )
add_definitions( -DBOOST_SPIRIT_THREADSAFE )

if( WIN32 )
    add_definitions( -D_WIN32_WINNT=0x0600 )
    add_definitions( -DWINVER=0x0600 )
endif()

set( Boost_USE_STATIC_LIBS ON )
set( BOOST_INCLUDEDIR ${MASA_INCLUDE} )
set( BOOST_LIBRARYDIR ${MASA_LIB} )
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
set( ZLIB_ROOT ${MASA_INCLUDE}/zlib ${MASA_LIB} )
find_package( ZLIB REQUIRED )

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
    add_target_external_library( web cpp-netlib_server-${platform}-mt_d cpp-netlib_server-${platform}-mt )
    add_target_external_library( web cpp-netlib_uri-${platform}-mt_d cpp-netlib_uri-${platform}-mt )
endif()

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
add_executable_target( cloud_server ${files} )
set_target_folder( cloud_server applications )
target_link_libraries( cloud_server
    host
    runtime
    web
)
if( WIN32 )
    target_link_libraries( cloud_server
        libeay32
        shlwapi
        ssleay32
        ws2_32
    )
endif()

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
add_test_target( host_test host )

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

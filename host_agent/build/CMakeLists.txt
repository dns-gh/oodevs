cmake_minimum_required( VERSION 2.8 )
project( host_agent )
enable_testing()

include( MasaMacros.cmake )

set( root_dir ${CMAKE_SOURCE_DIR}/.. )
set( run_dir  ${root_dir}/run/${platform} )

include_directories( ${root_dir}/src/libraries )
add_definitions( -D_CRT_SECURE_NO_WARNINGS )
add_definitions( -D_SCL_SECURE_NO_WARNINGS )

if( WIN32 )
    add_definitions( -D_WIN32_WINNT=0x0600 )
    add_definitions( -DWINVER=0x0600 )
endif()

set( BOOST_INCLUDEDIR ${MASA_INCLUDE} )
find_package( Boost REQUIRED )
set( ZLIB_ROOT ${MASA_INCLUDE}/zlib ${MASA_LIB} )
find_package( ZLIB REQUIRED )

################################################################################
# runtime
################################################################################
file( GLOB files
    "${root_dir}/src/libraries/runtime/*.h"
    "${root_dir}/src/libraries/runtime/*.cpp"
)
if( MSVC )
    file( GLOB win32_files
        "${root_dir}/src/libraries/runtime/win32/*.cpp"
        "${root_dir}/src/libraries/runtime/win32/*.h"
    )
    list( APPEND files ${win32_files} )
endif()
add_static_library( runtime ${files} )
set_target_folder( runtime libraries )

################################################################################
# web_server
################################################################################
file( GLOB files
    "${root_dir}/src/libraries/web/*.h"
    "${root_dir}/src/libraries/web/*.cpp"
)
add_static_library( web ${files} )
set_target_folder( web libraries )

################################################################################
# host
################################################################################
file( GLOB files
    "${root_dir}/src/libraries/host/*.h"
    "${root_dir}/src/libraries/host/*.cpp"
)
add_static_library( host ${files} )
set_target_folder( host libraries )

################################################################################
# cloud_server
################################################################################
file( GLOB files
    "${root_dir}/src/applications/cloud_server/*.cpp"
)
add_executable_target( cloud_server ${files} )
set_target_folder( cloud_server applications )
add_target_external_library( cloud_server archive_static_d archive_static )
add_target_external_library( cloud_server bcrypt-${platform}_d bcrypt-${platform} )
add_target_external_library( cloud_server cpp-netlib_client-${platform}-mt_d cpp-netlib_client-${platform}-mt )
add_target_external_library( cloud_server cpp-netlib_server-${platform}-mt_d cpp-netlib_server-${platform}-mt )
add_target_external_library( cloud_server cpp-netlib_uri-${platform}-mt_d cpp-netlib_uri-${platform}-mt )
add_target_external_library( cloud_server libsqlite3-${platform}_d libsqlite3-${platform} )
add_target_external_library( cloud_server zlibd zlib )
target_link_libraries( cloud_server
    host
    runtime
    web
)
if( WIN32 )
    target_link_libraries( cloud_server
        ws2_32
        shlwapi
    )
endif()

################################################################################
# runtime_test
################################################################################
file( GLOB files
    "${root_dir}/src/tests/runtime_test/runtime_test.cpp"
    "${root_dir}/src/tests/runtime_test/runtime_test.h"
    "${root_dir}/src/tests/runtime_test/Utf8Test.cpp"
)
if( WIN32 )
    file( GLOB win32_files
        "${root_dir}/src/tests/runtime_test/Win32*.cpp"
        "${root_dir}/src/tests/runtime_test/Win32*.h"
    )
    list( APPEND files ${win32_files} )
endif()
add_executable_target( runtime_test ${files} )
set_target_folder( runtime_test tests )
target_link_libraries( runtime_test
    runtime
)
add_test_target( runtime_test runtime )

################################################################################
# host_test
################################################################################
file( GLOB files
    "${root_dir}/src/tests/host_test/*.cpp"
    "${root_dir}/src/tests/host_test/*.h"
)
add_executable_target( host_test ${files} )
set_target_folder( host_test tests )
add_target_external_library( host_test bcrypt-${platform}_d bcrypt-${platform} )
target_link_libraries( host_test
    host
    runtime
)
add_test_target( host_test host )

################################################################################
# web_test
################################################################################
file( GLOB files
    "${root_dir}/src/tests/web_test/*.cpp"
    "${root_dir}/src/tests/web_test/*.h"
)
add_executable_target( web_test ${files} )
set_target_folder( web_test tests )
target_link_libraries( web_test
    runtime
    web
)
add_test_target( web_test web )

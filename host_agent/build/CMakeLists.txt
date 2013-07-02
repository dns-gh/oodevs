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

get_filename_component( root_dir "${CMAKE_SOURCE_DIR}/.." REALPATH )

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
masa_set_library( OPENSSL_LIBRARIES libeay32 ssleay32 )
masa_find_qt4( REQUIRED QtCore QtGui QtNetwork )

masa_set_external_library( ARCHIVE_LIBRARIES            archive_static_d archive_static )
masa_set_external_library( CPP_NETLIB_CLIENT_LIBRARIES  cpp-netlib_client-${platform}-mt_d cpp-netlib_client-${platform}-mt )
masa_set_external_library( CPP_NETLIB_URI_LIBRARIES     cpp-netlib_uri-${platform}-mt_d cpp-netlib_uri-${platform}-mt )
masa_set_external_library( MONGOOSE_LIBRARIES           mongoose_d mongoose )
masa_set_external_library( ZLIB_LIBRARIES               zlibd zlib )

masa_set_internal_library( BCRYPT_LIBRARIES             bcrypt )
masa_set_internal_library( SQLITE3_LIBRARIES            sqlite3 )

masa_set_external_library( TOOLS_LIBRARIES              tools_d tools )

# runtime
masa_add_target( runtime libraries "libraries/runtime" )
target_link_libraries( runtime
    shlwapi
    ${ARCHIVE_LIBRARIES}
    ${ZLIB_LIBRARIES}
)
if( WIN32 )
    masa_add_target( runtime_win32 libraries "libraries/runtime/win32" )
    target_link_libraries( runtime runtime_win32 )
endif()

# web_server
masa_add_target( web libraries "libraries/web" )
target_link_libraries( web
    ws2_32
    ${CPP_NETLIB_CLIENT_LIBRARIES}
    ${CPP_NETLIB_URI_LIBRARIES}
    ${MONGOOSE_LIBRARIES}
    ${OPENSSL_LIBRARIES}
)

# host
masa_add_target( host libraries "libraries/host" )
target_link_libraries( host
    web
    version
    ${BCRYPT_LIBRARIES}
    ${SQLITE3_LIBRARIES}
    ${TOOLS_LIBRARIES}
)

# cloud_server
masa_add_target( cloud_server applications "applications/cloud_server" OPTIONS executable )
target_link_libraries( cloud_server
    host
    runtime
)

# client
masa_add_target( client libraries "libraries/client" OPTIONS qt )

# cloud_client
masa_add_target( cloud_client applications "applications/cloud_client" OPTIONS win32 qt executable )
target_link_libraries( cloud_client
    client
    host
    runtime
)

# runtime_test
masa_add_target( runtime_test tests "tests/runtime_test" OPTIONS test )
target_link_libraries( runtime_test
    runtime
    ${TOOLS_LIBRARIES}
)

# host_test
masa_add_target( host_test tests "tests/host_test" OPTIONS test )
masa_set_target_folder( host_test tests )
target_link_libraries( host_test
    host
    runtime
)

# web_test
masa_add_target( web_test tests "tests/web_test" OPTIONS test )
target_link_libraries( web_test
    runtime
    web
    ${TOOLS_LIBRARIES}
)

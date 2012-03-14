cmake_minimum_required( VERSION 2.8 )
project( process_server )
enable_testing()

include( MasaMacros.cmake )

set( dev_dir  e:/dev )
set( root_dir ${CMAKE_SOURCE_DIR}/.. )

include_directories( ${dev_dir}/include )
link_directories( ${dev_dir}/lib/${platform} )

################################################################################
# runtime
################################################################################
file( GLOB files
    "${root_dir}/src/libraries/runtime/*.h"
)
if( WIN32 )
    file( GLOB win32_files
        "${root_dir}/src/libraries/runtime/win32/*.cpp"
        "${root_dir}/src/libraries/runtime/win32/*.h"
    )
    list( APPEND files ${win32_files} )
endif()
add_static_library( runtime ${files} )

################################################################################
# web_server
################################################################################
file( GLOB files
    "${root_dir}/src/libraries/web/*.h"
    "${root_dir}/src/libraries/web/*.cpp"
)
add_static_library( web ${files} )
add_target_include( web ${root_dir}/src/libraries )

################################################################################
# sample_server
################################################################################
file( GLOB files
    "${root_dir}/src/samples/sample_server/*.cpp"
)
add_executable_target( sample_server ${files} )
add_target_include( sample_server ${root_dir}/src/libraries )
add_target_external_library( sample_server mongoose_d mongoose )
target_link_libraries( sample_server
    runtime
    web
    ws2_32
)

################################################################################
# win32_runtime_test
################################################################################
file( GLOB files
    "${root_dir}/src/tests/win32_runtime_test/*.cpp"
    "${root_dir}/src/tests/win32_runtime_test/*.h"
)
add_executable_target( win32_runtime_test ${files} )
set_target_folder( win32_runtime_test tests )
add_target_include( win32_runtime_test ${root_dir}/src/libraries )
target_link_libraries( win32_runtime_test
    runtime
)
add_test_target( win32_runtime_test win32_runtime )

################################################################################
# web_test
################################################################################
file( GLOB files
    "${root_dir}/src/tests/web_test/*.cpp"
    "${root_dir}/src/tests/web_test/*.h"
)
add_executable_target( web_test ${files} )
set_target_folder( web_test tests )
add_target_include( web_test ${root_dir}/src/libraries )
target_link_libraries( web_test
    web
)
add_test_target( web_test web )

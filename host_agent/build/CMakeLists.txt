cmake_minimum_required( VERSION 2.8 )
project( host_agent )
enable_testing()

include( MasaMacros.cmake )

set( dev_dir  e:/dev )
set( root_dir ${CMAKE_SOURCE_DIR}/.. )
set( run_dir  ${root_dir}/run/${platform} )

include_directories( ${dev_dir}/include )
link_directories( ${dev_dir}/lib/${platform} )
include_directories( ${root_dir}/src/libraries )

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
target_link_libraries( host
    runtime
    xerces-c_3
)
set_target_folder( host libraries )

################################################################################
# sample_agent
################################################################################
file( GLOB files
    "${root_dir}/src/samples/sample_agent/*.cpp"
)
add_executable_target( sample_agent ${files} )
set_target_folder( sample_agent samples )
add_target_external_library( sample_agent mongoose_d mongoose )
target_link_libraries( sample_agent
    runtime
    host
    web
    ws2_32
)

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
        "${root_dir}/src/tests/runtime_test/Win32ProcessTest.cpp"
        "${root_dir}/src/tests/runtime_test/Win32RuntimeTest.cpp"
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
target_link_libraries( host_test
    host
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
    web
)
add_test_target( web_test web )

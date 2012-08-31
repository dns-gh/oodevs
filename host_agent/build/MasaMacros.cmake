#*****************************************************************************
#
#  This file is part of a MASA library or program.
#  Refer to the included end-user license agreement for restrictions.
#
#  Copyright (c) 2012 MASA Group
#
#*****************************************************************************

if( MSVC )
    set_property( GLOBAL PROPERTY USE_FOLDERS ON )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    math( EXPR msvc_platform "(${MSVC_VERSION} - 600) / 10" )
    set( msvc_suffix "_x64" )
    if( ${CMAKE_SIZEOF_VOID_P} EQUAL 4 )
        set( msvc_suffix "" )
    endif()
    set( platform vc${msvc_platform}${msvc_suffix} )
endif()

if(NOT MASA_INCLUDE)
    set( MASA_INCLUDE $ENV{MASA_HOME}/include )
endif()
if(NOT MASA_LIB)
    set( MASA_LIB $ENV{MASA_HOME}/lib/${platform} )
endif()
if(NOT MASA_VERSION )
    set( MASA_VERSION "0.0.0.0" )
endif()
include_directories( ${MASA_INCLUDE} )
link_directories( ${MASA_LIB} )

# Hack around protobuf output directory limitations
macro( protobuf_make srcs hdrs output_dir )
    file( MAKE_DIRECTORY ${output_dir} )
    set( backup ${CMAKE_CURRENT_BINARY_DIR} )
    set( CMAKE_CURRENT_BINARY_DIR ${output_dir} )
    PROTOBUF_GENERATE_CPP( ${srcs} ${hdrs} ${ARGN} )
    set( CMAKE_CURRENT_BINARY_DIR ${backup} )
endmacro()

macro( add_target_definition target define )
    set_property( TARGET ${target} APPEND PROPERTY COMPILE_DEFINITIONS "${define}" )
endmacro()

macro( add_target_include target dir )
    add_target_compile_flag( ${target} "-I${dir}" )
endmacro()

macro( add_target_compile_flag target flag )
    get_property( has_flag TARGET ${target} PROPERTY COMPILE_FLAGS SET )
    if( ${has_flag} )
        set_property( TARGET ${target} APPEND_STRING PROPERTY COMPILE_FLAGS " ${flag}" )
    else()
        set_property( TARGET ${target} APPEND PROPERTY COMPILE_FLAGS "${flag}" )
    endif()
endmacro()

macro( glob_dir var group )
    file( GLOB ${var} ${ARGN} )
    source_group( ${group} FILES ${${var}} )
endmacro()

macro( set_target_folder target folder )
    set_property( TARGET ${target} PROPERTY FOLDER ${folder} )
endmacro()

macro( add_target_external_library target debug release )
    target_link_libraries( ${target}
        debug     ${debug}
        optimized ${release}
    )
endmacro()

macro( set_target_output_name target debug release )
    set_target_properties( ${target} PROPERTIES
        DEBUG_OUTPUT_NAME           ${debug}
        MINSIZEREL_OUTPUT_NAME      ${release}
        RELEASE_OUTPUT_NAME         ${release}
        RELWITHDEBINFO_OUTPUT_NAME  ${release}
    )
endmacro()

macro( add_static_library target )
    add_library( ${target} ${ARGN} )
    set_target_output_name( ${target} ${target}-${platform}_d ${target}-${platform} )
endmacro()

macro( add_executable_target target )
    add_executable( ${target} ${ARGN} )
    set_target_output_name( ${target} ${target}_d ${target} )
endmacro()

macro( add_test_target target test_name )
    if( MSVC )
        add_target_compile_flag( ${target} "/bigobj" )
    endif()
    add_test( NAME              ${test_name}
              WORKING_DIRECTORY ${run_dir}
              COMMAND           ${target} ${ARGN}
    )
endmacro()

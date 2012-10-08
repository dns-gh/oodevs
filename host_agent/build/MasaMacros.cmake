#*****************************************************************************
#
#  This file is part of a MASA library or program.
#  Refer to the included end-user license agreement for restrictions.
#
#  Copyright (c) 2012 MASA Group
#
#*****************************************************************************

macro( set_flag property regexp value )
    if( ${property} MATCHES ${regexp} )
        string( REGEX REPLACE ${regexp} ${value} ${property} "${${property}}" )
    else()
        set( ${property} "${${property}} ${value}" )
    endif()
endmacro()

macro( add_flag property value )
    set_flag( ${property} ${value} ${value} )
endmacro()

if( MSVC )
    set_property( GLOBAL PROPERTY USE_FOLDERS ON )
    # 4127 conditional expression is constant
    # 4505 'function' : unreferenced local function has been removed
    add_flag( CMAKE_CXX_FLAGS "/wd4127" )
    add_flag( CMAKE_CXX_FLAGS "/wd4505" )
    add_flag( CMAKE_CXX_FLAGS "/WX" )
    set_flag( CMAKE_CXX_FLAGS "/W[0-4]" "/W4" )
    add_flag( CMAKE_RC_FLAGS "/nologo" )
    math( EXPR msvc_platform "(${MSVC_VERSION} - 600) / 10" )
    set( msvc_suffix "_x64" )
    if( ${CMAKE_SIZEOF_VOID_P} EQUAL 4 )
        set( msvc_suffix "" )
    endif()
    set( platform vc${msvc_platform}${msvc_suffix} )
endif()

# set common includes
if(NOT MASA_INCLUDE)
    set( MASA_INCLUDE $ENV{MASA_HOME}/include )
endif()
message( "-- Includes  " ${MASA_INCLUDE} )
set( CMAKE_INCLUDE_PATH ${MASA_INCLUDE} )
include_directories( ${MASA_INCLUDE} )

# set common libraries
if(NOT MASA_LIB)
    set( MASA_LIB $ENV{MASA_HOME}/lib/${platform} )
endif()
message( "-- Libraries " ${MASA_LIB} )
set( CMAKE_LIBRARY_PATH ${MASA_LIB} )
link_directories( ${MASA_LIB} )

# set common binaries
if(NOT MASA_BIN)
    set( MASA_BIN $ENV{MASA_HOME}/bin )
endif()
message( "-- Binaries  " ${MASA_BIN} )
set( CMAKE_PROGRAM_PATH ${MASA_BIN} )

if(NOT MASA_VERSION )
    set( MASA_VERSION "0.0.0.0" )
endif()

macro( find_with_prefix target prefix )
    set( CMAKE_INCLUDE_PATH ${MASA_INCLUDE}/${prefix} )
    find_package( ${target} ${ARGN} )
    set( CMAKE_INCLUDE_PATH ${MASA_INCLUDE} )
endmacro()

macro( find_qt4 )
    set( QT_BINARY_DIR ${MASA_BIN} )
    set( QT_USE_QTMAIN true )
    find_with_prefix( Qt4 qt ${ARGN} )
    include( ${QT_USE_FILE} )
endmacro()

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

macro( glob_qt4_ui output group )
    glob_dir( src ${group} ${ARGN} )
    set( dst )
    qt4_wrap_ui( dst ${src} )
    source_group( autogen FILES ${dst} )
    list( APPEND ${output} ${dst} )
endmacro()

macro( glob_qt4_headers output group )
    glob_dir( src ${group} ${ARGN} )
    set( dst )
    qt4_wrap_cpp( dst ${src} OPTIONS "-nw" )
    source_group( autogen FILES ${dst} )
    list( APPEND ${output} ${src} ${dst} )
endmacro()

macro( glob_qt4_rc output group )
    glob_dir( src ${group} ${ARGN} )
    set( dst )
    qt4_add_resources( dst ${src} )
    source_group( autogen FILES ${dst} )
    list( APPEND ${output} ${dst} )
endmacro()

macro( set_target_qt4 target )
    # include dir where ui/moc files are generated
    add_target_include( ${target} ${CMAKE_CURRENT_BINARY_DIR} )
    target_link_libraries( ${target} ${QT_LIBRARIES} )
endmacro()

macro( configure_rc target files group )
    if( WIN32 )
        set( FILENAME ${target}${CMAKE_EXECUTABLE_SUFFIX} )
        set( VERSION ${MASA_VERSION} )
        STRING( REGEX REPLACE "\\." ", " VERSION_ID ${MASA_VERSION} )
        EXECUTE_PROCESS( COMMAND "cmd" " /C date /T" OUTPUT_VARIABLE result )
        STRING( REGEX REPLACE "[0-9]+/[0-9]+/([0-9]+)" "\\1" YEAR ${result} )
        set( output "${CMAKE_CURRENT_BINARY_DIR}/${target}.rc" )
        configure_file( ${ARGN} ${output} @ONLY )
        source_group( ${group} FILES ${output} )
        list( APPEND ${files} ${output} )
    endif()
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

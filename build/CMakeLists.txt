cmake_minimum_required( VERSION 2.8 )
project( sword )

enable_testing()

get_filename_component( root_dir "${CMAKE_SOURCE_DIR}/.." REALPATH )
include_directories( ${root_dir}/src/libraries )

include( ${MASA_MACROS} )

add_definitions(
    -DBOOST_SPIRIT_THREADSAFE
    -DPLATFORM=${platform}
    -D_WIN32_WINNT=0x501
    -DWINVER=0x501
)
if( MSVC )
    add_definitions(
        # disable msvc non-portable warnings
        -D_CRT_SECURE_NO_WARNINGS
        -D_SCL_SECURE_NO_WARNINGS
        # disable #define small char from rpcndr.h
        -DWIN32_LEAN_AND_MEAN
    )
endif()

set( COMPANY "MASA Group" )
set( PRODUCT "Sword" )

find_package( Protobuf REQUIRED )
find_package( OpenGL REQUIRED )
masa_find_with_prefix( ZLIB zlib REQUIRED )
masa_find_qt4( REQUIRED QtCore QtGui QtNetwork QtOpenGL QtWebKit phonon )

masa_set_library( FLEX_LIBRARIES
    lm_new_md
    libnoact_md
    libsb_md
    libcrvs_md
    lmgr_md
    lmgr_dongle_stub
)
masa_set_library( OPENSSL_LIBRARIES
    libeay32
    ssleay32
)

masa_set_library( TIMELINE_LIBRARIES
   debug:timeline-${platform}-mt-gd
   release:timeline-${platform}-mt
   debug:timeline_controls-${platform}-mt-gd
   release:timeline_controls-${platform}-mt
)


masa_set_external_library( DIRECTIA_LIBRARIES         directia-${platform}-mt-gd-4_6 directia-${platform}-mt-4_6 )
masa_set_external_library( EXCEL_LIBRARIES            excel_d excel )
masa_set_external_library( GDAL_I_LIBRARIES           gdal_i_d gdal_i )
masa_set_external_library( GEOS_I_LIBRARIES           geos_i_d geos_i )
masa_set_external_library( HLA_LIBRARIES              hla_d hla )
masa_set_external_library( ICONV_LIBRARIES            libiconv_d libiconv )
masa_set_external_library( LICENSE_GUI_LIBRARIES      license_gui_d license_gui )
masa_set_external_library( MONGOOSE_LIBRARIES         mongoose_d mongoose )
masa_set_external_library( PROJ_I_LIBRARIES           proj_i_d proj_i )
masa_set_external_library( PROTOBUF_LIBRARIES         libprotobufd libprotobuf )
masa_set_external_library( SPATIALITE_LIBRARIES       spatialite_d spatialite )
masa_set_external_library( TOOLS_LIBRARIES            tools_d tools )
masa_set_external_library( XALAN_LIBRARIES            Xalan-C_1D Xalan-C_1 )
masa_set_external_library( XERCES_LIBRARIES           xerces-c_3D xerces-c_3 )
masa_set_external_library( MAPNIK_LIBRARIES           mapnik_d mapnik )
masa_set_external_library( MINIZIP_LIBRARIES          minizipd minizip )

masa_set_internal_library( ANALYSYS_LIBRARIES         libanalysis )
masa_set_internal_library( EXTRACTOR_LIBRARIES        libextractor )
masa_set_internal_library( GDAL_OGR_LIBRARIES         libgdal_ogr )
masa_set_internal_library( GEOCOORD_LIBRARIES         libgeocoord )
masa_set_internal_library( GEODATA_LIBRARIES          libgeodata )
masa_set_internal_library( GLEW_LIBRARIES             glew )
masa_set_internal_library( GRAPHICS_LIBRARIES         libgraphics )
masa_set_internal_library( PATHFIND_LIBRARIES         libpathfind )
masa_set_internal_library( SPATIALCONTAINER_LIBRARIES libspatialcontainer )
masa_set_internal_library( SQLITE3_LIBRARIES          sqlite3 )
masa_set_internal_library( TERRAIN_LIBRARIES          libterrain )


macro( grab_item output name items )
    list( LENGTH ${items} len )
    if( ${len} )
        masa_has_item( ${output} ${name} ${${items}} )
        list( REMOVE_ITEM ${items} ${name} )
    else()
        set( ${output} )
    endif()
endmacro()

function( add_target target group dir )
    message( "-- ${target}" )
    masa_extract_options( options ${ARGN} )
    grab_item( has_protocol "protocol" options )
    grab_item( is_plugin "plugin" options )
    masa_has_item( executable "executable" ${options} )
    masa_has_item( shared "shared" ${options} )
    masa_has_item( test "test" ${options} )
    masa_add_target( ${target} ${group} ${dir} OPTIONS ${options} )
    masa_set_output_directory( ${target}
        "${root_dir}/out/${platform}/release"
        "${root_dir}/out/${platform}/debug"
    )
    if( executable )
        # nothing
    elseif( shared )
        # remove _dll suffix from output name
        if( ${target} MATCHES "_dll$" )
            string( REGEX REPLACE "(.+)_dll" "\\1" next ${target} )
            masa_get_library_full_name( ${next} debug release )
            masa_set_target_output_name( ${target} ${debug} ${release} )
        endif()
        set( rundir "${MASA_RUN}" )
        if( is_plugin )
            string( REGEX REPLACE "(.+)_plugin_dll" "\\1" next ${target} )
            set( rundir "${MASA_RUN}/plugins/${next}" )
            file( MAKE_DIRECTORY "${rundir}" )
        endif()
        add_custom_command( TARGET ${target} POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${target}> "${rundir}" )
    elseif( test )
        if( MSVC AND CMAKE_CONFIGURATION_TYPES )
            add_custom_command( TARGET ${target} POST_BUILD COMMAND ${target}
                --temp_directory=${CMAKE_CURRENT_BINARY_DIR}/${target}.tmp
                --data_directory=${root_dir}/data/${dir}
                WORKING_DIRECTORY ${MASA_RUN} )
        endif()
    else()
        # add lib prefix
        masa_get_library_full_name( lib${target} debug release )
        masa_set_target_output_name( ${target} ${debug} ${release} )
    endif()
    if( has_protocol )
        masa_add_target_include( ${target} "${CMAKE_CURRENT_BINARY_DIR}/protocol.out" )
        target_link_libraries( ${target} protocol )
    endif()
    if( executable OR shared )
        masa_add_version( ${target} "${root_dir}/build/template.res" "${PRODUCT}" )
    endif()
endfunction()

# protocol
add_target( protocol common "libraries/protocol" OPTIONS protobuf )

# mt_tools
add_target( mt_tools common "libraries/MT_Tools" )
target_link_libraries( mt_tools
    psapi
    ${TOOLS_LIBRARIES}
)

# sword_tools
add_target( sword_tools common "libraries/tools" OPTIONS )
target_link_libraries( sword_tools
    mt_tools
    ${PROTOBUF_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${XALAN_LIBRARIES}
    ${XERCES_LIBRARIES}
)

# ent
add_target( ent common "libraries/ent" OPTIONS qt protocol )
target_link_libraries( ent ${QT_QTCORE_LIBRARY} )

# meteo
add_target( meteo common "libraries/meteo" OPTIONS protocol )
target_link_libraries( meteo
    ${TOOLS_LIBRARIES}
    ${XERCES_LIBRARIES}
)

# propagation
add_target( propagation common "libraries/propagation" )
target_link_libraries( propagation
    ${GDAL_I_LIBRARIES}
    ${GDAL_OGR_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${TOOLS_LIBRARIES}
    ${XERCES_LIBRARIES}
)

# 3a
add_target( 3a common "libraries/3a" OPTIONS protocol )
target_link_libraries( 3a
    sword_tools
    ${GEOCOORD_LIBRARIES}
)

# clients_kernel
add_target( clients_kernel common "libraries/clients_kernel" OPTIONS qt3 qt protocol )
target_link_libraries( clients_kernel
    ent
    sword_tools
    ${GEOCOORD_LIBRARIES}
    ${GRAPHICS_LIBRARIES}
    ${QT_QTGUI_LIBRARY}
)

# clients_kernel_test
add_target( clients_kernel_test tests "tests/clients_kernel_test" OPTIONS test )
target_link_libraries( clients_kernel_test clients_kernel )

# actions
add_target( actions clients "libraries/actions" OPTIONS qt3 qt protocol )
target_link_libraries( actions
    clients_gui
    clients_kernel
)

# reports
add_target( reports common "libraries/reports" OPTIONS protocol qt )
target_link_libraries( reports clients_kernel )

# indicators
add_target( indicators common "libraries/indicators" OPTIONS qt )
target_link_libraries( indicators clients_kernel )

# geostore
add_target( geostore common "libraries/geostore" )
# spatialite is stupid and has spatialite.h and spatialite/spatialite.h
masa_add_target_include( geostore "${MASA_INCLUDE}/spatialite" )
target_link_libraries( geostore
    ${ICONV_LIBRARIES}
    ${GEOCOORD_LIBRARIES}
    ${GEODATA_LIBRARIES}
    ${GEOS_I_LIBRARIES}
    ${GRAPHICS_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${PROJ_I_LIBRARIES}
    ${SQLITE3_LIBRARIES}
    ${SPATIALCONTAINER_LIBRARIES}
    ${SPATIALITE_LIBRARIES}
    ${TERRAIN_LIBRARIES}
    ${TOOLS_LIBRARIES}
    ${XERCES_LIBRARIES}
)

# geostore_test
add_target( geostore_test tests "tests/geostore_test" OPTIONS test )
masa_add_target_include( geostore_test "${MASA_INCLUDE}/spatialite" )
target_link_libraries( geostore_test
    geostore
    ${TOOLS_LIBRARIES}
)

# 3a_test
add_target( 3a_test tests "tests/3a_test" OPTIONS protocol test )
target_link_libraries( 3a_test 3a )

# actions_test
add_target( actions_test tests "tests/actions_test" OPTIONS protocol test )
target_link_libraries( actions_test actions )

# propagation_test
add_target( propagation_test tests "tests/propagation_test" OPTIONS test protocol )
target_link_libraries( propagation_test
    propagation
    sword_tools
    ${GEOCOORD_LIBRARIES}
)

# protocol_test
add_target( protocol_test tests "tests/protocol_test" OPTIONS protobuf protocol test )
target_link_libraries( protocol_test sword_tools )

# tools_test
add_target( tools_test tests "tests/tools_test" OPTIONS protocol test )
target_link_libraries( tools_test sword_tools
    ${MINIZIP_LIBRARIES}
)

function( add_plugin target )
    masa_extract_options( options ${ARGN} )
    add_target( ${target} plugins "libraries/${target}" OPTIONS protocol ${options} )
endfunction()

# plugins
add_plugin( aar_plugin )
add_plugin( dis_plugin )
add_plugin( hla_plugin )
target_link_libraries( hla_plugin
    dispatcher
    rpr
    tic
    propagation
    ${HLA_LIBRARIES}
)
add_plugin( logger_plugin )
add_plugin( logistic_plugin OPTIONS qt )
add_plugin( messenger_plugin )
add_plugin( order_plugin )
add_plugin( positions_plugin )
add_plugin( replay_plugin )
add_plugin( rights_plugin )
target_link_libraries( rights_plugin
    ${OPENSSL_LIBRARIES}
)
add_plugin( rpr )
add_plugin( saver_plugin )
add_plugin( score_plugin )
add_plugin( script_plugin )
add_plugin( vision_plugin )
add_plugin( tic )
add_plugin( timeline_plugin )
target_link_libraries( timeline_plugin
    ${OPENSSL_LIBRARIES}
)
add_plugin( web_control_plugin )
target_link_libraries( web_control_plugin
    ws2_32
    ${MONGOOSE_LIBRARIES}
    ${XERCES_LIBRARIES}
    ${TOOLS_LIBRARIES}
)

# positions_plugin_test
add_target( positions_plugin_test tests "tests/positions_plugin_test" OPTIONS protocol test )
target_link_libraries( positions_plugin_test
    positions_plugin
    ${TOOLS_LIBRARIES}
)

# plugins_test
add_target( plugins_test tests "tests/plugins_test" OPTIONS protocol test )
target_link_libraries( plugins_test
    clients_kernel
    logistic_plugin
    rpr
    script_plugin
    tic
    timeline_plugin
    ${DIRECTIA_LIBRARIES}
)

# dispatcher
add_target( dispatcher server "libraries/dispatcher" OPTIONS protocol )
target_link_libraries( dispatcher
    3a
    aar_plugin
    clients_kernel
    indicators
    logger_plugin
    logistic_plugin
    messenger_plugin
    meteo
    order_plugin
    replay_plugin
    reports
    rights_plugin
    saver_plugin
    score_plugin
    script_plugin
    vision_plugin
    web_control_plugin
    ${DIRECTIA_LIBRARIES}
)

# dis_plugin_dll
add_target( dis_plugin_dll plugins "libraries/dis_plugin_dll" OPTIONS plugin shared )
masa_add_target_definition( dis_plugin_dll DIS_PLUGIN_DLL_EXPORTS )
target_link_libraries( dis_plugin_dll
    clients_kernel
    dis_plugin
    rpr
    tic
)

# hla_plugin_dll
add_target( hla_plugin_dll plugins "libraries/hla_plugin_dll" OPTIONS plugin shared )
masa_add_target_definition( hla_plugin_dll HLA_PLUGIN_DLL_EXPORTS )
target_link_libraries( hla_plugin_dll hla_plugin )

# hla_plugin_test
add_target( hla_plugin_test tests "tests/hla_plugin_test" OPTIONS protocol test )
target_link_libraries( hla_plugin_test hla_plugin )
add_dependencies( hla_plugin_test hla_plugin_dll )

# web_control_plugin_dll
add_target( web_control_plugin_dll plugins "libraries/web_control_plugin_dll" OPTIONS protocol plugin shared )
masa_add_target_definition( web_control_plugin_dll WEB_CONTROL_PLUGIN_DLL_EXPORTS )
masa_set_target_output_name( web_control_plugin_dll web_control_plugin_d web_control_plugin )
target_link_libraries( web_control_plugin_dll web_control_plugin )

# web_control_plugin_test
add_target( web_control_plugin_test tests "tests/web_control_plugin_test" OPTIONS protocol test )
target_link_libraries( web_control_plugin_test web_control_plugin )
add_dependencies( web_control_plugin_test web_control_plugin_dll )

# timeline_plugin_dll
add_target( timeline_plugin_dll plugins "libraries/timeline_plugin_dll" OPTIONS protocol plugin shared )
masa_add_target_definition( timeline_plugin_dll TIMELINE_PLUGIN_DLL_EXPORTS )
target_link_libraries( timeline_plugin_dll
    timeline_plugin
    dispatcher
    ${OPENSSL_LIBRARIES}
)

set( PLUGINS_DLL
    dis_plugin_dll
    hla_plugin_dll
    timeline_plugin_dll
    web_control_plugin_dll
)


# dispatcher_app
add_target( dispatcher_app server "applications/dispatcher_app" OPTIONS executable )
target_link_libraries( dispatcher_app
    dispatcher
    ${LICENSE_GUI_LIBRARIES}
)
add_dependencies( dispatcher_app dispatcher_dll )

# dispatcher_dll
add_target( dispatcher_dll server "libraries/dispatcher_dll" OPTIONS shared )
target_link_libraries( dispatcher_dll
    dispatcher
    positions_plugin
)
add_dependencies( dispatcher_dll ${PLUGINS_DLL} )

# dispatcher_test
add_target( dispatcher_test tests "tests/dispatcher_test" OPTIONS protocol test )
target_link_libraries( dispatcher_test dispatcher )

# resource_network
add_target( resource_network server "libraries/resource_network" OPTIONS protocol )
target_link_libraries( resource_network ${XERCES_LIBRARIES} )

# simulation_terrain
add_target( simulation_terrain server "libraries/simulation_terrain" )
target_link_libraries( simulation_terrain
    sword_tools
    ${ANALYSYS_LIBRARIES}
    ${GEOCOORD_LIBRARIES}
    ${PATHFIND_LIBRARIES}
    ${SPATIALCONTAINER_LIBRARIES}
)

# simulation_terrain_test
add_target( simulation_terrain_test tests "tests/simulation_terrain_test" OPTIONS test )
target_link_libraries( simulation_terrain_test simulation_terrain )

# simulation_kernel
# fixme not working in shared mode
add_target( simulation_kernel server "libraries/simulation_kernel" OPTIONS recurse protocol )
masa_add_target_include( simulation_kernel "${root_dir}/src/libraries/simulation_kernel" )
masa_add_target_compile_flag( simulation_kernel "/bigobj" )
target_link_libraries( simulation_kernel
    propagation
    resource_network
    simulation_terrain
    ${GEODATA_LIBRARIES}
    ${TERRAIN_LIBRARIES}
)

# simulation_kernel_test
add_target( simulation_kernel_test tests "tests/simulation_kernel_test" OPTIONS protocol test )
masa_add_target_include( simulation_kernel_test "${root_dir}/src/libraries/simulation_kernel" )
target_link_libraries( simulation_kernel_test
    ent
    meteo
    simulation_kernel
    ${DIRECTIA_LIBRARIES}
    ${GRAPHICS_LIBRARIES}
)

# replayer_app
add_target( replayer_app server "applications/replayer_app" OPTIONS executable )
target_link_libraries( replayer_app
    dispatcher
    ${LICENSE_GUI_LIBRARIES}
)
add_dependencies( replayer_app
    dispatcher_dll
)

# simulation_app
add_target( simulation_app server "applications/simulation_app" OPTIONS executable )
masa_add_target_include( simulation_app "${root_dir}/src/libraries/simulation_kernel" )
set_property( TARGET simulation_app APPEND PROPERTY LINK_FLAGS "/LARGEADDRESSAWARE" )
set_property( TARGET simulation_app APPEND_STRING PROPERTY LINK_FLAGS " /STACK:15000000" )
target_link_libraries( simulation_app
    simulation_kernel
    dispatcher
    ${LICENSE_GUI_LIBRARIES}
)
add_dependencies( simulation_app
    dispatcher_dll
)

# svgl
add_target( svgl clients "libraries/svgl" OPTIONS qt )
target_link_libraries( svgl ${XERCES_LIBRARIES} )

# svgl_app
add_target( svgl_app clients "applications/svgl_app" OPTIONS executable qt )
target_link_libraries( svgl_app svgl )

# clients_gui
add_target( clients_gui clients "libraries/clients_gui" OPTIONS qt3 qt protocol )
target_link_libraries( clients_gui
    clients_kernel
    htmlhelp
    meteo
    svgl
    ${GDAL_I_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${LICENSE_GUI_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${QT_PHONON_LIBRARY}
    ${SPATIALCONTAINER_LIBRARIES}
    ${TERRAIN_LIBRARIES}
    ${MINIZIP_LIBRARIES}
)

# frontend
add_target( frontend clients "libraries/frontend" OPTIONS qt3 qt protocol )
target_link_libraries( frontend
    clients_gui
)

# actions_gui
add_target( actions_gui clients "libraries/actions_gui" OPTIONS qt3 qt protocol )
target_link_libraries( actions_gui
    actions
    clients_gui
)

# gaming
add_target( gaming clients "libraries/gaming" OPTIONS qt3 qt protocol )
target_link_libraries( gaming
    actions
    clients_gui
    indicators
    propagation
    reports
)

# gaming_app
add_target( gaming_app clients "applications/gaming_app" OPTIONS qt3 qt protocol executable )
target_link_libraries( gaming_app
    actions_gui
    gaming
    ${TIMELINE_LIBRARIES}
    ${EXCEL_LIBRARIES}
    ${EXTRACTOR_LIBRARIES}
    ${TERRAIN_LIBRARIES}
    ${GEODATA_LIBRARIES}
    ${GRAPHICS_LIBRARIES}
    ${MAPNIK_LIBRARIES}
    ${GEOS_I_LIBRARIES}
)

# launcher_test
add_target( launcher_test tests "tests/launcher_test" OPTIONS protocol test )
target_link_libraries( launcher_test frontend )

# preparation
# fixme need qt gui
add_target( preparation clients "libraries/preparation" OPTIONS qt3 qt protocol )
target_link_libraries( preparation
    clients_gui
    geostore
    indicators
    propagation
)

# preparation_app
add_target( preparation_app clients "applications/preparation_app" OPTIONS qt3 qt protocol executable )
target_link_libraries( preparation_app
    actions_gui
    frontend
    preparation
    ${EXTRACTOR_LIBRARIES}
)

# selftraining_app
add_target( selftraining_app clients "applications/selftraining_app" OPTIONS qt3 qt executable )
target_link_libraries( selftraining_app
    clients_gui
    frontend
)

# adaptation_app
add_target( adaptation_app clients "applications/adaptation_app" OPTIONS qt3 qt protocol executable )
target_link_libraries( adaptation_app
    clients_gui
    ${EXCEL_LIBRARIES}
)

# clients_test
add_target( clients_test tests "tests/clients_test" OPTIONS qt3 qt test )
target_link_libraries( clients_test
    actions
    preparation
)

# client_sample_app
add_target( client_sample_app samples "samples/client_sample_app" OPTIONS protocol executable )
masa_add_target_include( client_sample_app "${CMAKE_CURRENT_BINARY_DIR}/protocol.out/protocol" )
target_link_libraries( client_sample_app ws2_32 ${TOOLS_LIBRARIES} )

# core
add_target( core common "libraries/core" )
target_link_libraries( core
    ${CMAKE_THREAD_LIBS_INIT}
    ${XERCES_LIBRARIES}
    ${Boost_LIBRARIES}
)

# core_test
add_target( core_test tests "tests/core_test" OPTIONS test )
target_link_libraries( core_test core )

# xeuseudeu
add_target( xeuseudeu common "libraries/xeuseudeu" )
target_link_libraries( xeuseudeu core )

# xeuseudeu_test
add_target( xeuseudeu_test tests "tests/xeuseudeu_test" OPTIONS protobuf test )
target_link_libraries( xeuseudeu_test
    xeuseudeu
    ${TOOLS_LIBRARIES}
)

# model_generator_app
add_target( model_generator_app server "applications/model_generator_app" OPTIONS executable )
target_link_libraries( model_generator_app
    xeuseudeu
    ${TOOLS_LIBRARIES}
)

# model_app
add_target( model_app server "applications/model_app" OPTIONS executable )
target_link_libraries( model_app
    core
    ${TOOLS_LIBRARIES}
    psapi
)

# casava_odb_gen_app
add_target( casava_odb_gen_app clients "applications/casava_odb_gen_app" OPTIONS qt executable )
target_link_libraries( casava_odb_gen_app
    clients_gui
    sword_tools
    ${GEOCOORD_LIBRARIES}
)

# migratexml 
add_target( migratexml clients "applications/migratexml" OPTIONS executable )
target_link_libraries( migratexml
    sword_tools
)

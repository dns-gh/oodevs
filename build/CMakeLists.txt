cmake_minimum_required( VERSION 2.8 )
project( sword )

enable_testing()
include( ${MASA_MACROS} )

set( root_dir ${CMAKE_SOURCE_DIR}/.. )
include_directories( ${root_dir}/src/libraries )
add_definitions( -DBOOST_SPIRIT_THREADSAFE )

find_package( Protobuf REQUIRED )
find_package( OpenGL REQUIRED )
masa_find_with_prefix( ZLIB zlib REQUIRED )
masa_find_qt4( REQUIRED QtCore QtGui QtNetwork QtOpenGL )

set( FLEX_LIBRARIES
    lm_new_md
    libnoact_md
    libsb_md
    libcrvs_md
    lmgr_md
    lmgr_dongle_stub
)
masa_set_external_library( ANALYSYS_LIBRARIES         analysys_d analysis )
masa_set_external_library( CORE_LIBRARIES             core_d core )
masa_set_external_library( DIRECTIA_LIBRARIES         directia-${platform}-mt-gd-4_6 directia-${platform}-mt-4_6 )
masa_set_external_library( EXCEL_LIBRARIES            excel_d excel )
masa_set_external_library( GDAL_I_LIBRARIES           gdal_i_d gdal_i )
masa_set_external_library( GDAL_OGR_LIBRARIES         gdal_ogr_d gdal_ogr )
masa_set_external_library( GEOCOORD_LIBRARIES         geocoord_d geocoord )
masa_set_external_library( GEODATA_LIBRARIES          geodata_d geodata )
masa_set_external_library( GEOS_I_LIBRARIES           geos_i_d geos_i )
masa_set_external_library( GRAPHICS_LIBRARIES         graphics_d graphics )
masa_set_external_library( HLA_LIBRARIES              hla_a hla )
masa_set_external_library( LOG4CXX_LIBRARIES          log4cxx_d log4cxx )
masa_set_external_library( MONGOOSE_LIBRARIES         mongoose_d mongoose )
masa_set_external_library( PATHFIND_LIBRARIES         pathfind_d pathfind )
masa_set_external_library( PROJ_I_LIBRARIES           proj_i_d proj_i )
masa_set_external_library( PROTOBUF_LIBRARIES         libprotobufd libprotobuf )
masa_set_external_library( SPATIALCONTAINER_LIBRARIES spatialcontainer_d spatialcontainer )
masa_set_external_library( SPATIALITE_LIBRARIES       spatialite_d spatialite )
masa_set_external_library( SVGL_LIBRARIES             svgl_d svgl )
masa_set_external_library( TERRAIN_LIBRARIES          terrain_d terrain )
masa_set_external_library( TOOLS_LIBRARIES            tools_d     tools )
masa_set_external_library( XALAN_LIBRARIES            Xalan-C_1D  Xalan-C_1 )
masa_set_external_library( XERCES_LIBRARIES           xerces-c_3D xerces-c_3 )
masa_set_external_library( ZIPSTREAM_LIBRARIES        zipstream_d zipstream )

macro( add_protocol_dependency target )
    masa_add_target_include( ${target} "${CMAKE_CURRENT_BINARY_DIR}/protocol.out" )
    target_link_libraries( ${target} protocol )
endmacro()

# protocol
masa_add_target( protocol common "libraries/protocol" OPTIONS protobuf )

# mt_tools
masa_add_target( mt_tools common "libraries/MT_Tools" )
target_link_libraries( mt_tools
    psapi
    ${TOOLS_LIBRARIES}
)

# sword_tools
masa_add_target( sword_tools common "libraries/tools" )
target_link_libraries( sword_tools
    mt_tools
    ${QT_LIBRARIES}
    ${XALAN_LIBRARIES}
    ${XERCES_LIBRARIES}
)

# ent
masa_add_target( ent common "libraries/ent" )
target_link_libraries( ent ${QT_LIBRARIES} )

# meteo
masa_add_target( meteo common "libraries/meteo" )
add_protocol_dependency( meteo )
target_link_libraries( meteo ${XERCES_LIBRARIES} )

# flood
masa_add_target( flood common "libraries/flood" )
target_link_libraries( flood ${OPENGL_LIBRARIES} )

# 3a
masa_add_target( 3a common "libraries/3a" )
add_protocol_dependency( 3a )
target_link_libraries( 3a
    sword_tools
    ${GEOCOORD_LIBRARIES}
)

# clients_kernel
masa_add_target( clients_kernel common "libraries/clients_kernel" OPTIONS qt3 )
add_protocol_dependency( clients_kernel )
target_link_libraries( clients_kernel
    ent
    sword_tools
    ${GDAL_I_LIBRARIES}
    ${GDAL_OGR_LIBRARIES}
    ${GEOCOORD_LIBRARIES}
    ${GRAPHICS_LIBRARIES}
    ${OPENGL_LIBRARIES}
)

# actions
masa_add_target( actions common "libraries/actions" OPTIONS qt3 )
add_protocol_dependency( actions )
target_link_libraries( actions clients_kernel )

# reports
masa_add_target( reports common "libraries/reports" OPTIONS qt3 )
add_protocol_dependency( reports )
target_link_libraries( reports clients_kernel )

# indicators
masa_add_target( indicators common "libraries/indicators" OPTIONS qt3 )
target_link_libraries( indicators clients_kernel )

# license_gui
masa_add_target( license_gui common "libraries/license_gui" )
target_link_libraries( license_gui
    mt_tools
    ${FLEX_LIBRARIES}
    ${XERCES_LIBRARIES}
)

# shield
masa_add_target( shield common "libraries/shield" OPTIONS protobuf )
add_protocol_dependency( shield )
target_link_libraries( shield sword_tools )

# shield_app
masa_add_target( shield_app clients "applications/shield_app" OPTIONS executable )
masa_add_target_include( shield_app "${CMAKE_CURRENT_BINARY_DIR}/shield.out" )
target_link_libraries( shield_app shield )

# geostore
# fixme not working in shared mode
masa_add_target( geostore common "libraries/geostore" )
target_link_libraries( geostore
    libiconv
    ${GEODATA_LIBRARIES}
    ${GEOS_I_LIBRARIES}
    ${PROJ_I_LIBRARIES}
    ${SPATIALITE_LIBRARIES}
)

# 3a_test
masa_add_target( 3a_test tests "tests/3a_test" OPTIONS test )
add_protocol_dependency( 3a_test )
target_link_libraries( 3a_test 3a )

# actions_test
masa_add_target( actions_test tests "tests/actions_test" OPTIONS test )
add_protocol_dependency( actions_test )
target_link_libraries( actions_test actions )

# flood_test
masa_add_target( flood_test tests "tests/flood_test" OPTIONS test )
target_link_libraries( flood_test flood )

# protocol_test
masa_add_target( protocol_test tests "tests/protocol_test" OPTIONS protobuf test )
add_protocol_dependency( protocol_test )
target_link_libraries( protocol_test sword_tools )

# shield_test
masa_add_target( shield_test tests "tests/shield_test" OPTIONS protobuf test )
add_protocol_dependency( shield_test )
masa_add_target_include( shield_test "${CMAKE_CURRENT_BINARY_DIR}/shield.out" )
target_link_libraries( shield_test shield )

# tools_test
masa_add_target( tools_test tests "tests/tools_test" OPTIONS test )
target_link_libraries( tools_test sword_tools )

function( add_plugin target )
    masa_extract_options( options ${ARGN} )
    masa_add_target( ${target} plugins "libraries/${target}" OPTIONS ${options} )
    add_protocol_dependency( ${target} )
endfunction()

# plugins
add_plugin( aar_plugin )
add_plugin( dis_plugin )
add_plugin( hla_plugin OPTIONS qt3 )
add_plugin( logger_plugin )
add_plugin( logistic_plugin )
add_plugin( messenger_plugin )
add_plugin( order_plugin )
add_plugin( positions_plugin )
add_plugin( replay_plugin )
add_plugin( rights_plugin )
add_plugin( rpr )
add_plugin( saver_plugin )
add_plugin( score_plugin )
add_plugin( script_plugin OPTIONS qt3 )
add_plugin( tic )
add_plugin( web_control_plugin )
target_link_libraries( web_control_plugin
    ws2_32
    ${MONGOOSE_LIBRARIES}
    ${XERCES_LIBRARIES}
)

# positions_plugin_test
masa_add_target( positions_plugin_test tests "tests/positions_plugin_test" OPTIONS test )
add_protocol_dependency( positions_plugin_test )
target_link_libraries( positions_plugin_test positions_plugin )

# plugins_test
masa_add_target( plugins_test tests "tests/plugins_test" OPTIONS test )
add_protocol_dependency( plugins_test )
target_link_libraries( plugins_test
    clients_kernel
    logistic_plugin
    rpr
    script_plugin
    tic
    ${DIRECTIA_LIBRARIES}
)

# dispatcher
masa_add_target( dispatcher server "libraries/dispatcher" OPTIONS qt3 )
add_protocol_dependency( dispatcher )
target_link_libraries( dispatcher
    3a
    aar_plugin
    actions
    indicators
    logger_plugin
    messenger_plugin
    meteo
    order_plugin
    replay_plugin
    reports
    rights_plugin
    saver_plugin
    score_plugin
    script_plugin
    shield
    web_control_plugin
    ${DIRECTIA_LIBRARIES}
)

# dispatcher_app
masa_add_target( dispatcher_app server "applications/dispatcher_app" OPTIONS win32 executable )
masa_add_target_definition( dispatcher_app PLATFORM=${platform} )
target_link_libraries( dispatcher_app
    dispatcher
    license_gui
)

# dispatcher_dll
masa_add_target( dispatcher_dll server "libraries/dispatcher_dll" OPTIONS shared )
target_link_libraries( dispatcher_dll
    dispatcher
    positions_plugin
)

# dispatcher_test
masa_add_target( dispatcher_test tests "tests/dispatcher_test" OPTIONS test )
add_protocol_dependency( dispatcher_test )
target_link_libraries( dispatcher_test dispatcher )

# dis_plugin_dll
masa_add_target( dis_plugin_dll plugins "libraries/dis_plugin_dll" OPTIONS shared )
masa_add_target_definition( dis_plugin_dll DIS_PLUGIN_DLL_EXPORTS )
target_link_libraries( dis_plugin_dll
    clients_kernel
    dis_plugin
    rpr
    tic
)

# hla_plugin_dll
masa_add_target( hla_plugin_dll plugins "libraries/hla_plugin_dll" OPTIONS shared )
masa_add_target_definition( hla_plugin_dll HLA_PLUGIN_DLL_EXPORTS )
target_link_libraries( hla_plugin_dll
    dispatcher
    hla_plugin
    rpr
    tic
    ${HLA_LIBRARIES}
)

# hla_plugin_test
masa_add_target( hla_plugin_test tests "tests/hla_plugin_test" OPTIONS test )
add_protocol_dependency( hla_plugin_test )
target_link_libraries( hla_plugin_test hla_plugin_dll )

# logistic_plugin_dll
masa_add_target( logistic_plugin_dll plugins "libraries/logistic_plugin_dll" OPTIONS shared )
add_protocol_dependency( logistic_plugin_dll )
masa_add_target_definition( logistic_plugin_dll LOGISTIC_PLUGIN_DLL_EXPORTS )
target_link_libraries( logistic_plugin_dll
    clients_kernel
    logistic_plugin
)

# web_control_plugin_dll
masa_add_target( web_control_plugin_dll plugins "libraries/web_control_plugin_dll" OPTIONS shared )
add_protocol_dependency( web_control_plugin_dll )
masa_add_target_definition( web_control_plugin_dll WEB_CONTROL_PLUGIN_DLL_EXPORTS )
target_link_libraries( web_control_plugin_dll web_control_plugin )

# web_control_plugin_test
masa_add_target( web_control_plugin_test tests "tests/web_control_plugin_test" OPTIONS test )
add_protocol_dependency( web_control_plugin_test )
target_link_libraries( web_control_plugin_test web_control_plugin_dll )

# resource_network
masa_add_target( resource_network server "libraries/resource_network" )
add_protocol_dependency( resource_network )
target_link_libraries( resource_network ${XERCES_LIBRARIES} )

# simulation_terrain
# fixme simulation_terrain has a dependency on simulation_kernel
masa_add_target( simulation_terrain server "libraries/simulation_terrain" )
target_link_libraries( simulation_terrain
    sword_tools
    ${ANALYSYS_LIBRARIES}
    ${GEOCOORD_LIBRARIES}
    ${PATHFIND_LIBRARIES}
    ${SPATIALCONTAINER_LIBRARIES}
)

# simulation_kernel
# fixme not working in shared mode
masa_add_target( simulation_kernel server "libraries/simulation_kernel" OPTIONS recurse )
add_protocol_dependency( simulation_kernel )
masa_add_target_definition( simulation_kernel PLATFORM=${platform} )
masa_add_target_include( simulation_kernel "${root_dir}/src/libraries/simulation_kernel" )
target_link_libraries( simulation_kernel
    dispatcher
    flood
    resource_network
    simulation_terrain
    ${CORE_LIBRARIES}
    ${GEODATA_LIBRARIES}
    ${LOG4CXX_LIBRARIES}
    ${TERRAIN_LIBRARIES}
)

# simulation_kernel_test
masa_add_target( simulation_kernel_test tests "tests/simulation_kernel_test" OPTIONS test )
add_protocol_dependency( simulation_kernel_test )
masa_add_target_include( simulation_kernel_test "${root_dir}/src/libraries/simulation_kernel" )
target_link_libraries( simulation_kernel_test simulation_kernel )

# replayer_app
masa_add_target( replayer_app server "applications/replayer_app" OPTIONS win32 executable )
target_link_libraries( replayer_app
    dispatcher
    license_gui
)

# simulation_app
masa_add_target( simulation_app server "applications/simulation_app" OPTIONS win32 executable )
masa_add_target_definition( simulation_app PLATFORM=${platform} )
masa_add_target_include( simulation_app "${root_dir}/src/libraries/simulation_kernel" )
masa_add_target_definition( simulation_app _WIN32_WINNT=0x501 )
masa_add_target_definition( simulation_app WINVER=0x501 )
target_link_libraries( simulation_app
    simulation_kernel
    dispatcher
    license_gui
)

# wrapper
masa_add_target( wrapper modules "libraries/wrapper" )

function( add_module target )
    masa_add_target( ${target} modules "libraries/${target}" )
    target_link_libraries( ${target} ${XERCES_LIBRARIES} )
    masa_add_target( ${target}_dll modules "libraries/${target}_dll" OPTIONS shared )
    masa_set_target_output_name( ${target}_dll ${target}_d ${target} )
    target_link_libraries( ${target}_dll
        ${target}
        wrapper
     )
    masa_add_target( ${target}_test modules "tests/${target}_test" OPTIONS test )
    target_link_libraries( ${target}_test
        ${target}_dll
        ${CORE_LIBRARIES}
    )
endfunction()

# fire_module
add_module( fire_module )

# movement_module
add_module( movement_module )
masa_add_target_include( movement_module "${root_dir}/src/libraries/simulation_kernel" )
target_link_libraries( movement_module
    mt_tools
    ${SPATIALCONTAINER_LIBRARIES}
)
masa_add_target_include( movement_module_test "${root_dir}/src/libraries/simulation_kernel" )

# perception_module
add_module( perception_module )
masa_add_target_include( perception_module "${root_dir}/src/libraries/simulation_kernel" )

# propagation_module
add_module( propagation_module )

# module_tester_test
masa_add_target( module_tester_test modules "tests/module_tester_test" OPTIONS test )
target_link_libraries( module_tester_test ${CORE_LIBRARIES} )

# decisional_test
masa_add_target( decisional_test tests "tests/integration_decisionnal_test" OPTIONS executable )
masa_add_target_definition( decisional_test PLATFORM=${platform} )
target_link_libraries( decisional_test ${DIRECTIA_LIBRARIES} )
masa_add_test_target( decisional_test decisional_test
    --temp_directory=${CMAKE_CURRENT_BINARY_DIR}/decisional_test.tmp
    --data_directory=${root_dir}/run/${platform}/resources
)

# client_proxy
masa_add_target( client_proxy clients "libraries/client_proxy" )
add_protocol_dependency( client_proxy )
target_link_libraries( client_proxy sword_tools )

# client_proxy_test
masa_add_target( client_proxy_test tests "tests/client_proxy_test" OPTIONS test )
add_protocol_dependency( client_proxy_test )
target_link_libraries( client_proxy_test client_proxy )

# clients_gui
masa_add_target( clients_gui clients "libraries/clients_gui" OPTIONS qt3 qt )
add_protocol_dependency( clients_gui )
target_link_libraries( clients_gui
    clients_kernel
    htmlhelp
    license_gui
    meteo
    ${SPATIALCONTAINER_LIBRARIES}
    ${SVGL_LIBRARIES}
    ${TERRAIN_LIBRARIES}
    ${ZIPSTREAM_LIBRARIES}
)

# frontend
masa_add_target( frontend clients "libraries/frontend" OPTIONS qt3 qt )
add_protocol_dependency( frontend )
masa_add_target_definition( frontend PLATFORM=${platform} )
target_link_libraries( frontend
    clients_kernel
    ${ZIPSTREAM_LIBRARIES}
)

# actions_gui
masa_add_target( actions_gui clients "libraries/actions_gui" OPTIONS qt3 qt )
add_protocol_dependency( actions_gui )
target_link_libraries( actions_gui
    actions
    clients_gui
)

# gaming
masa_add_target( gaming clients "libraries/gaming" OPTIONS qt3 )
add_protocol_dependency( gaming )
target_link_libraries( gaming
    actions
    clients_gui
    flood
    indicators
    reports
)

# gaming_app
masa_add_target( gaming_app clients "applications/gaming_app" OPTIONS qt3 qt executable )
add_protocol_dependency( gaming_app )
target_link_libraries( gaming_app
    actions_gui
    gaming
)

# launcher_dll
masa_add_target( launcher_dll clients "libraries/launcher_dll" OPTIONS qt3 shared )
add_protocol_dependency( launcher_dll )
masa_add_target_definition( launcher_dll LAUNCHER_DLL_EXPORTS )
target_link_libraries( launcher_dll
    client_proxy
    frontend
    shield
)

# launcher_test
masa_add_target( launcher_test tests "tests/launcher_test" OPTIONS test )
add_protocol_dependency( launcher_test )
target_link_libraries( launcher_test launcher_dll )

# launcher_app
masa_add_target( launcher_app clients "applications/launcher_app" OPTIONS executable )
target_link_libraries( launcher_app launcher_dll )

# launcher_test_app
masa_add_target( launcher_test_app clients "applications/launcher_test_app" OPTIONS executable )
add_protocol_dependency( launcher_test_app )
masa_add_target_include( launcher_test_app "${CMAKE_CURRENT_BINARY_DIR}/shield.out" )
target_link_libraries( launcher_test_app launcher_dll )

# preparation
# fixme need qt gui
masa_add_target( preparation clients "libraries/preparation" OPTIONS qt3 qt )
add_protocol_dependency( preparation )
target_link_libraries( preparation
    clients_gui
    flood
    geostore
    indicators
)

# preparation_app
masa_add_target( preparation_app clients "applications/preparation_app" OPTIONS qt3 qt executable )
add_protocol_dependency( preparation_app )
masa_add_target_library( preparation_app terrainextractor )
target_link_libraries( preparation_app
    actions_gui
    frontend
    preparation
)

# selftraining_app
masa_add_target( selftraining_app clients "applications/selftraining_app" OPTIONS qt3 qt executable )
target_link_libraries( selftraining_app
    clients_gui
    launcher_dll
)

# adaptation_app
masa_add_target( adaptation_app clients "applications/adaptation_app" OPTIONS qt3 qt executable )
target_link_libraries( adaptation_app
    clients_gui
    ${EXCEL_LIBRARIES}
)

# loadtester_app
masa_add_target( loadtester_app clients "applications/loadtester_app" OPTIONS executable )
add_protocol_dependency( loadtester_app )
target_link_libraries( loadtester_app sword_tools )

# package_app
masa_add_target( package_app clients "applications/package_app" OPTIONS executable )

# clients_test
masa_add_target( clients_test tests "tests/clients_test" OPTIONS qt3 test )
target_link_libraries( clients_test preparation )

# client_sample_app
masa_add_target( client_sample_app samples "samples/client_sample_app" OPTIONS executable )
add_protocol_dependency( client_sample_app )
masa_add_target_include( client_sample_app "${CMAKE_CURRENT_BINARY_DIR}/protocol.out/protocol" )
target_link_libraries( client_sample_app ws2_32 )

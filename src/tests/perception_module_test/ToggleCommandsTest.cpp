// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "perception_module_test_pch.h"
#include "ModuleFixture.h"

namespace
{
    struct PerceptionFixture : public sword::perception::ModuleFixture
    {
        PerceptionFixture()
            : identifier( 42 )
        {}
        void TogglePerception( bool isActivated )
        {
            StartCommand( "toggle perception",
                core::MakeModel( "identifier", identifier )
                               ( "perception", "my_perception" )
                               ( "activated", isActivated ) );
            ExecuteCommands();
        }
        const unsigned int identifier;
    };
}

BOOST_FIXTURE_TEST_CASE( toggle_perception_command_activates_perception, PerceptionFixture )
{
    core::Model& scan = model[ "entities" ][ identifier ][ "perceptions/my_perception/activated" ];
    scan = false;
    ExpectEffect( scan, sword::test::MakeModel( true ) );
    TogglePerception( true );
}

BOOST_FIXTURE_TEST_CASE( toggle_perception_command_deactivates_perception, PerceptionFixture )
{
    core::Model& scan = model[ "entities" ][ identifier ][ "perceptions/my_perception/activated" ];
    scan = true;
    ExpectEffect( scan, sword::test::MakeModel( false ) );
    TogglePerception( false );
}

BOOST_FIXTURE_TEST_CASE( already_activated_perception_does_nothing_if_activated_again, PerceptionFixture )
{
    core::Model& scan = model[ "entities" ][ identifier ][ "perceptions/my_perception/activated" ];
    scan = true;
    TogglePerception( true );
}

BOOST_FIXTURE_TEST_CASE( already_deactivated_perception_does_nothing_if_deactivated_again, PerceptionFixture )
{
    core::Model& scan = model[ "entities" ][ identifier ][ "perceptions/my_perception/activated" ];
    scan = false;
    TogglePerception( false );
}

namespace
{
    struct RadarFixture : public sword::perception::ModuleFixture
    {
        RadarFixture()
            : identifier( 42 )
        {}
        void ToggleRadar( bool isActivated, int radarClass, const std::string& radarName )
        {
            core::Model& radar = model[ "entities" ][ identifier ][ "perceptions/radars" ][ radarName ][ "activated" ];
            radar = false;
            ExpectEffect( radar, sword::test::MakeModel( true ) );
            const std::size_t command = StartCommand( "toggle radar",
                                            core::MakeModel( "identifier", identifier )
                                                           ( "radar-class", radarClass )
                                                           ( "activated", isActivated ) );
            ExecuteCommands();
            commands.Stop( command );
        }
        const unsigned int identifier;
    };
}

BOOST_FIXTURE_TEST_CASE( toggle_radar_command_activates_radars_class, RadarFixture )
{
    ToggleRadar( true, 0, "radar" );
    ToggleRadar( true, 1, "tapping" );
    ToggleRadar( true, 2, "tapping-radar" );
}

BOOST_FIXTURE_TEST_CASE( toggle_radar_command_notifies_error_log_if_radar_class_is_unknown, RadarFixture )
{
    model[ "entities" ][ identifier ][ "perceptions/radars/unknown/activated" ] = false;
    const int unknwownRadar = 3;
    StartCommand( "toggle radar",
        core::MakeModel( "identifier", identifier )
                       ( "radar-class", unknwownRadar )
                       ( "activated", true ) );
    MOCK_EXPECT( Log ).once().with( SWORD_LOG_LEVEL_ERROR, mock::any );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( activated_radar_localization_is_forwarded_to_effect, sword::perception::ModuleFixture )
{
    const unsigned int identifier = 42;
    const unsigned int perceptionId = 1337;
    const int tapping = 1;
    const int localization = 0;
    core::Model& radar = model[ "entities" ][ identifier ][ "perceptions/localized-radars/tapping" ];
    ExpectEffect( radar, sword::test::MakeModel( perceptionId,
                            sword::test::MakeModel( "localization", sword::test::MakeUserData( &localization ) )
                                                  ( "perception-id", perceptionId ) ) );
    StartCommand( "toggle localized radar",
        core::MakeModel( "identifier", identifier )
                       ( "radar-class", tapping )
                       ( "activated", true )
                       ( "perception-id", perceptionId )
                       ( "localization", sword::test::MakeUserData( &localization ) ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( deactivated_radar_resets_localization, sword::perception::ModuleFixture )
{
    const unsigned int identifier = 42;
    const unsigned int perceptionId = 1337;
    const int tapping = 1;
    core::Model& radar = model[ "entities" ][ identifier ][ "perceptions/localized-radars/tapping" ];
    ExpectRemove( radar[ perceptionId ] );
    StartCommand( "toggle localized radar",
        core::MakeModel( "identifier", identifier )
                       ( "radar-class", tapping )
                       ( "activated", false )
                       ( "perception-id", perceptionId ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( activating_localized_detection_localization_is_forwarded_to_effect, sword::perception::ModuleFixture )
{
    const unsigned int identifier = 42;
    const unsigned int perceptionId = 1337;
    const int localization = 0;
    core::Model& perception = model[ "entities" ][ identifier ][ "perceptions/my-perception/child" ];
    ExpectEffect( perception, sword::test::MakeModel( perceptionId,
                                sword::test::MakeModel( "localization", sword::test::MakeUserData( &localization ) ) ) );
    StartCommand( "toggle localized perception",
        core::MakeModel( "identifier", identifier )
                       ( "activated", true )
                       ( "perception", "my-perception/child" )
                       ( "perception-id", perceptionId )
                       ( "localization", sword::test::MakeUserData( &localization ) ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( deactivating_localized_perception_removes_perception_node, sword::perception::ModuleFixture )
{
    const unsigned int identifier = 42;
    const unsigned int perceptionId = 1337;
    core::Model& perception = model[ "entities" ][ identifier ][ "perceptions/my-perception" ];
    ExpectRemove( perception[ perceptionId ] );
    StartCommand( "toggle localized perception",
        core::MakeModel( "identifier", identifier )
                       ( "activated", false )
                       ( "perception", "my-perception" )
                       ( "perception-id", perceptionId ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( activating_reco_forwards_localization_and_growth_speed_to_effect, sword::perception::ModuleFixture )
{
    const unsigned int identifier = 42;
    const unsigned int perceptionId = 1337;
    const int localization = 0;
    core::Model& reco = model[ "entities" ][ identifier ][ "perceptions/reco" ];
    ExpectEffect( reco, sword::test::MakeModel( perceptionId, sword::test::MakeModel( "has-growth-speed", true )
                                                                                    ( "growth-speed", 31 )
                                                                                    ( "radius", 0 )
                                                                                    ( "max-radius-reached", false )
                                                                                    ( "perception-id", perceptionId )
                                                                                    ( "localization", sword::test::MakeUserData( &localization ) ) ) );
    StartCommand( "toggle reco",
        core::MakeModel( "identifier", identifier )
                       ( "activated", true )
                       ( "perception-id", perceptionId )
                       ( "has-growth-speed", true )
                       ( "growth-speed", 31 )
                       ( "localization", sword::test::MakeUserData( &localization ) ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( deactivating_reco_resets_localization, sword::perception::ModuleFixture )
{
    const unsigned int identifier = 42;
    const unsigned int perceptionId = 1337;
    core::Model& reco = model[ "entities" ][ identifier ][ "perceptions/reco" ];
    ExpectRemove( reco[ perceptionId ] );
    StartCommand( "toggle reco",
        core::MakeModel( "identifier", identifier )
                       ( "activated", false )
                       ( "perception-id", perceptionId ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( activating_object_detection_forwards_localization_and_speed_to_effect, sword::perception::ModuleFixture )
{
    const unsigned int identifier = 42;
    const unsigned int perceptionId = 1337;
    const double speed = 13;
    core::Model& object = model[ "entities" ][ identifier ][ "perceptions/object-detection" ];
    ExpectEffect( object, sword::test::MakeModel( perceptionId,
                                                    sword::test::MakeModel( "growth-speed", speed )
                                                                          ( "radius", 0 )
                                                                          ( "max-radius-reached", false )
                                                                          ( "perception-id", perceptionId )
                                                                          ( "center/x", 1 )
                                                                          ( "center/y", 2 )
                                                                          ( "localization", 43 ) ) );
    StartCommand( "toggle object detection",
        core::MakeModel( "identifier", identifier )
                       ( "activated", true )
                       ( "perception-id", perceptionId )
                       ( "growth-speed", speed )
                       ( "center/x", 1 )
                       ( "center/y", 2 )
                       ( "localization", 43 ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( deactivating_object_detection_resets_localization, sword::perception::ModuleFixture )
{
    const unsigned int identifier = 42;
    const unsigned int perceptionId = 1337;
    core::Model& object = model[ "entities" ][ identifier ][ "perceptions/object-detection" ];
    ExpectRemove( object[ perceptionId ] );
    StartCommand( "toggle object detection",
        core::MakeModel( "identifier", identifier )
                       ( "activated", false )
                       ( "perception-id", perceptionId ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( activating_recognition_point_forwards_center_and_speed_to_effect, sword::perception::ModuleFixture )
{
    const unsigned int identifier = 42;
    const unsigned int perceptionId = 1337;
    const double growthSpeed = 13;
    const double size = 45;
    core::Model& point = model[ "entities" ][ identifier ][ "perceptions/recognition-point" ];
    ExpectEffect( point, sword::test::MakeModel( perceptionId,
                            sword::test::MakeModel( "growth-speed", growthSpeed )
                                                  ( "radius", 0. )
                                                  ( "perception-id", perceptionId )
                                                  ( "max-radius", size )
                                                  ( "max-radius-reached", false )
                                                  ( "center/x", 1 )
                                                  ( "center/y", 2 ) ) );
    StartCommand( "toggle recognition point",
        core::MakeModel( "identifier", identifier )
                       ( "activated", true )
                       ( "perception-id", perceptionId )
                       ( "growth-speed", growthSpeed )
                       ( "max-radius", size )
                       ( "center/x", 1 )
                       ( "center/y", 2 ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( deactivating_recognition_point, sword::perception::ModuleFixture )
{
    const unsigned int identifier = 42;
    const unsigned int perceptionId = 1337;
    core::Model& object = model[ "entities" ][ identifier ][ "perceptions/recognition-point" ];
    ExpectRemove( object[ perceptionId ] );
    StartCommand( "toggle recognition point",
        core::MakeModel( "identifier", identifier )
                       ( "activated", false )
                       ( "perception-id", perceptionId ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( activating_alat_monitoring_localization_computes_vision_objects_steps, sword::perception::ModuleFixture )
{
    model[ "tick" ] = 0;
    const unsigned int identifier = 42;
    const unsigned int perceptionId = 1337;
    const int localization = 0;
    core::Model& perception = model[ "entities" ][ identifier ][ "perceptions/alat/monitoring" ];
    ExpectEffect( perception, sword::test::MakeModel( perceptionId,
                                sword::test::MakeModel( "localization", sword::test::MakeUserData( &localization ) )
                                                      ( "forest-detection-time-step", mock::any )
                                                      ( "empty-detection-time-step", mock::any )
                                                      ( "urban-detection-time-step", mock::any ) ) );
    MOCK_EXPECT( GetVisionObjectsInSurface ).once();
    StartCommand( "toggle alat monitoring",
        core::MakeModel( "identifier", identifier )
                       ( "activated", true )
                       ( "perception-id", perceptionId )
                       ( "localization", sword::test::MakeUserData( &localization ) ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( deactivating_alat_monitoring_perception_removes_perception_node_and_sends_event, sword::perception::ModuleFixture )
{
    const unsigned int identifier = 42;
    const unsigned int perceptionId = 1337;
    model[ "entities" ][ identifier ][ "data" ] = "data";
    core::Model& perception = model[ "entities" ][ identifier ][ "perceptions/alat/monitoring" ];
    ExpectRemove( perception[ perceptionId ] );
    ExpectEvent( "alat monitoring disabled", sword::test::MakeModel( "entity/data", "data" ) );
    StartCommand( "toggle alat monitoring",
        core::MakeModel( "identifier", identifier )
                       ( "activated", false )
                       ( "perception-id", perceptionId ) );
    ExecuteCommands();
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "crossbow_test_pch.h"
#include "crossbow_plugin/OrderParameterSerializer.h"
#include "crossbow_plugin/OGR_Workspace.h"
#include "crossbow_plugin/DatabaseFactory.h"
#include "crossbow_plugin/Database_ABC.h"
#include "tools/ExerciseConfig.h"
#include "dispatcher/Model_ABC.h"

#include "clients_kernel/OrderParameter.h"

#pragma warning( push, 0 )
#pragma warning( disable: 4505 )
#include <turtle/mock.hpp>
#pragma warning( pop )

using namespace plugins;

namespace
{
    std::string GetHost()
    {
        static std::string host = "localhost";
        return host;
    }

    template< typename T >
    void CheckEncoding( T& message )
    {
        tools::AsnMessageEncoder< T > asnEncoder( message );
    }
}

MOCK_BASE_CLASS( MockWorkspace, crossbow::Workspace_ABC )     // defines a 'MockWorkspace' class implementing 'Workspace_ABC'
{
    MOCK_METHOD( GetDatabase, 1 )
    MOCK_METHOD( Release, 1 )
};


MOCK_BASE_CLASS( MockDatabase, crossbow::Database_ABC )     // defines a 'MockDatabase' class implementing 'Database_ABC'
{
    MOCK_METHOD( OpenBufferedTable, 2 )
    MOCK_METHOD( OpenTable, 1 )                   // implements the 'display' method from 'view' (taking 1 argument)
    MOCK_METHOD( GetTableName, 1 )
    MOCK_METHOD( ClearTable, 2 )
    MOCK_METHOD( Execute, 1 )
    MOCK_METHOD( Flush, 0 )
};

MOCK_BASE_CLASS( MockModel, dispatcher::Model_ABC )     // defines a 'MockDatabase' class implementing 'Database_ABC'
{
    MOCK_METHOD( Accept, 1 );

    MOCK_METHOD( sides, 0 )
    MOCK_METHOD( knowledgeGroups, 0 )
    MOCK_METHOD( formations, 0 )
    MOCK_METHOD( automats, 0 )
    MOCK_METHOD( agents, 0 )
    MOCK_METHOD( objects, 0 )
    MOCK_METHOD( populations, 0 )
    MOCK_METHOD( agentKnowledges, 0 )
    MOCK_METHOD( objectKnowledges, 0 )
    MOCK_METHOD( populationKnowledges, 0 )
    MOCK_METHOD( logConsignsMaintenance, 0 )
    MOCK_METHOD( logConsignsSupply, 0 )
    MOCK_METHOD( logConsignsMedical, 0 )
    MOCK_METHOD( fires, 0 )
    MOCK_METHOD( populationFires, 0 )
    MOCK_METHOD( fireEffects, 0 )
    MOCK_METHOD( reports, 0 )

    MOCK_METHOD( GetAgentTypes, 0 );
    MOCK_METHOD( GetMissionTypes, 0 );
    MOCK_METHOD( GetFragOrderTypes, 0 );
    MOCK_METHOD( GetObjectTypes, 0 );
    MOCK_METHOD( GetMedicalTreatmentTypes, 0 );
};


BOOST_AUTO_TEST_CASE( crossbow_test_serialize_bool )
{
    MockWorkspace mockWorkspace;
    MockModel mockModel;

    crossbow::OrderParameterSerializer serializer( mockWorkspace, mockModel );
    ASN1T_MissionParameter asn;
    kernel::OrderParameter parameter( "param", "bool", false );
    serializer.Serialize( asn, parameter, 0, "true" );
    BOOST_CHECK_EQUAL( asn.value.t, T_MissionParameter_value_aBool );
    BOOST_CHECK_EQUAL( asn.value.u.aBool, static_cast< asn::ASN1BOOL >( true ) );
    BOOST_CHECK_NO_THROW( CheckEncoding( asn ) );
}

BOOST_AUTO_TEST_CASE( crossbow_test_serialize_real )
{
    MockWorkspace mockWorkspace;
    MockModel mockModel;

    crossbow::OrderParameterSerializer serializer( mockWorkspace, mockModel );
    ASN1T_MissionParameter asn;
    kernel::OrderParameter parameter( "param", "real", false );
    serializer.Serialize( asn, parameter, 0, "42.5" );
    BOOST_CHECK_EQUAL( asn.value.t, T_MissionParameter_value_aReal );
    BOOST_CHECK_EQUAL( asn.value.u.aReal, static_cast< asn::ASN1REAL >( 42.5 ) );
    BOOST_CHECK_NO_THROW( CheckEncoding( asn ) );
}

/*
// Need real database
// Parameter 32 must be registered in db orderparameters_point
*/
/* $$$ --- SQL
DELETE FROM sword.exercises WHERE id=4242;
DELETE FROM sword.sessions WHERE id=4242;
DELETE FROM sword.orders;
DELETE FROM sword.orderparameters;
DELETE FROM sword.orderparameters_point;
DELETE FROM sword.orderparameters_line;
DELETE FROM sword.orderparameters_area;

INSERT INTO sword.exercises (id, name, description) VALUES (4242, 'test', 'run tests');
INSERT INTO sword.sessions (id, exercise_id) VALUES (4242, 4242 );
INSERT INTO sword.orders (id, unit_id, session_id) VALUES (321, 42, 4242 );

INSERT INTO sword.orderparameters (id, order_id, value) VALUES (321, 321, 'orderparameters_point' );
INSERT INTO sword.orderparameters_point (id, parameter_id, shape) VALUES
    (nextval('sword.tacticalobject_point_id_seq'::regclass), 321, sde.st_point( 0.14297806372070312, 49.53863960876465, 1 ) );

INSERT INTO sword.orderparameters (id, order_id, value) VALUES (322, 321, 'orderparameters_line' );
INSERT INTO sword.orderparameters_line (id, parameter_id, shape) VALUES
    (nextval('sword.tacticalobject_line_id_seq'::regclass), 322,
        sde.st_linestring( 'linestring( 0.1283868466796875 49.52336174621582, 0.11705719580078125 49.52353340759277 )', 1 ) );

INSERT INTO sword.orderparameters (id, order_id, value) VALUES (323, 321, 'orderparameters_area' );
INSERT INTO sword.orderparameters_area (id, parameter_id, shape) VALUES
    (nextval('sword.tacticalobject_area_id_seq'::regclass), 323,
        sde.st_polygon( 'polygon(( 0.1283868466796875 49.52336174621582,
                      0.11705719580078125 49.52353340759277,
                      0.12169205297851562 49.51735359802246, 0.1283868466796875 49.52336174621582 ))', 1 ) );
*/

namespace
{
template< typename T >
    void CleanLocationElem( T& location )
    {
        if( location.coordinates.n > 0 )
            delete [] location.coordinates.elem;
    }

    template< typename T >
    void CleanAsnLocation( T*& asnLocation )
    {
        if( asnLocation )
        {
            CleanLocationElem( *asnLocation );
            delete asnLocation;
            asnLocation = 0;
        }
    }

    template< typename T >
    void CleanAsnLocationList( T*& list )
    {
        if( list )
        {
            for ( unsigned i = 0; i < list->n; ++i )
                CleanLocationElem( list->elem[ i ] );
            if( list->n > 0 )
                delete [] list->elem;
            delete list;
            list = 0;
        }
    }

    template< typename T >
    void CleanAsnList( T*& list )
    {
        if( list )
        {
            if( list->n > 0 )
                delete [] list->elem;
            delete list;
            list = 0;
        }
    }
}

#define CROSSBOW_TEST_ESRI_CONNECTION
#ifdef CROSSBOW_TEST_ESRI_CONNECTION
# undef CROSSBOW_TEST_ESRI_CONNECTION


BOOST_AUTO_TEST_CASE( crossbow_test_serialize_point_lehavre )
{
    MockModel mockModel;
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        workspace.InitializeConnectionReference( "geometry", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );
        workspace.InitializeConnectionReference( "flat", "postgres://sword:sword@" + GetHost() + ":5432/sword_crossbow_db.sword" );

        crossbow::OrderParameterSerializer serializer( workspace, mockModel );
        ASN1T_MissionParameter asn;
        kernel::OrderParameter parameter( "param", "point", false );
        serializer.Serialize( asn, parameter, 321, "orderparameters_point_test" ); // point is registered with sql script
        BOOST_CHECK_EQUAL( asn.value.t, T_MissionParameter_value_point );
        BOOST_CHECK_EQUAL( asn.value.u.point->type, EnumLocationType::point );
        BOOST_CHECK_EQUAL( asn.value.u.point->coordinates.n, 1u );
        BOOST_CHECK_CLOSE( asn.value.u.point->coordinates.elem[0].latitude, 49.53863960876465, 0.0001 );
        BOOST_CHECK_CLOSE( asn.value.u.point->coordinates.elem[0].longitude, 0.14297806372070312, 0.0001 );
        BOOST_CHECK_NO_THROW( CheckEncoding( asn ) );
        CleanAsnLocation( asn.value.u.point );
    }
}

BOOST_AUTO_TEST_CASE( crossbow_test_serialize_point_monterey )
{
    MockModel mockModel;
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        workspace.InitializeConnectionReference( "geometry", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );

        crossbow::OrderParameterSerializer serializer( workspace, mockModel );
        ASN1T_MissionParameter asn;
        kernel::OrderParameter parameter( "param", "point", false );
        serializer.Serialize( asn, parameter, 325, "orderparameters_point_test" ); // point is registered with sql script
        BOOST_CHECK_EQUAL( asn.value.t, T_MissionParameter_value_point );
        BOOST_CHECK_EQUAL( asn.value.u.point->type, EnumLocationType::point );
        BOOST_CHECK_EQUAL( asn.value.u.point->coordinates.n, 1u );
        BOOST_CHECK_CLOSE( asn.value.u.point->coordinates.elem[0].latitude, 36.645834, 0.0001 );
        BOOST_CHECK_CLOSE( asn.value.u.point->coordinates.elem[0].longitude, -121.815033, 0.0001 );
        BOOST_CHECK_NO_THROW( CheckEncoding( asn ) );
        CleanAsnLocation( asn.value.u.point );
    }
}

BOOST_AUTO_TEST_CASE( crossbow_test_serialize_pointlist )
{
    MockModel mockModel;
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        workspace.InitializeConnectionReference( "geometry", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );

        crossbow::OrderParameterSerializer serializer( workspace, mockModel );
        ASN1T_MissionParameter asn;
        kernel::OrderParameter parameter( "param", "pointlist", false );
        serializer.Serialize( asn, parameter, 324, "orderparameters_point_test" ); // point is registered with sql script
        BOOST_CHECK_EQUAL( asn.value.t, T_MissionParameter_value_pointList );
        BOOST_CHECK( asn.value.u.pointList != 0 );
        BOOST_CHECK_EQUAL( asn.value.u.pointList->n, 3u );
        BOOST_CHECK_EQUAL( asn.value.u.pointList->elem[ 0 ].type, EnumLocationType::point );
        BOOST_CHECK_EQUAL( asn.value.u.pointList->elem[ 0 ].coordinates.n, 1u );
        BOOST_CHECK_CLOSE( asn.value.u.pointList->elem[ 0 ].coordinates.elem[0].latitude, 49.53863960876465, 0.0001 );
        BOOST_CHECK_CLOSE( asn.value.u.pointList->elem[ 0 ].coordinates.elem[0].longitude, 0.14297806372070312, 0.0001 );
        BOOST_CHECK_EQUAL( asn.value.u.pointList->elem[ 1 ].type, EnumLocationType::point );
        BOOST_CHECK_EQUAL( asn.value.u.pointList->elem[ 1 ].coordinates.n, 1u );
        BOOST_CHECK_CLOSE( asn.value.u.pointList->elem[ 1 ].coordinates.elem[0].latitude, 49.53863960876465, 0.0001 );
        BOOST_CHECK_CLOSE( asn.value.u.pointList->elem[ 1 ].coordinates.elem[0].longitude, 0.14297806372070312, 0.0001 );
        BOOST_CHECK_EQUAL( asn.value.u.pointList->elem[ 2 ].type, EnumLocationType::point );
        BOOST_CHECK_EQUAL( asn.value.u.pointList->elem[ 2 ].coordinates.n, 1u );
        BOOST_CHECK_CLOSE( asn.value.u.pointList->elem[ 2 ].coordinates.elem[0].latitude, 49.53863960876465, 0.0001 );
        BOOST_CHECK_CLOSE( asn.value.u.pointList->elem[ 2 ].coordinates.elem[0].longitude, 0.14297806372070312, 0.0001 );
        BOOST_CHECK_NO_THROW( CheckEncoding( asn ) );
        CleanAsnLocationList( asn.value.u.pointList );
    }
}

BOOST_AUTO_TEST_CASE( crossbow_test_charge_memory_orderparameter_point_serialization )
{
    MockModel mockModel;
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        workspace.InitializeConnectionReference( "geometry", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );

        for ( int i = 1000; i > 0; --i )
        {
            crossbow::OrderParameterSerializer serializer( workspace, mockModel );
            ASN1T_MissionParameter asn;
            kernel::OrderParameter parameter( "param", "point", false );
            serializer.Serialize( asn, parameter, 321, "orderparameters_point_test" ); // point is registered with sql script
            BOOST_CHECK_NO_THROW( CheckEncoding( asn ) );
            CleanAsnLocation( asn.value.u.point );
        }
    }
}

BOOST_AUTO_TEST_CASE( crossbow_test_charge_memory_orderparameter_pointlist_serialization )
{
    MockModel mockModel;
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        workspace.InitializeConnectionReference( "geometry", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );

        for ( int i = 1000; i > 0; --i )
        {
            crossbow::OrderParameterSerializer serializer( workspace, mockModel );
            ASN1T_MissionParameter asn;
            kernel::OrderParameter parameter( "param", "pointlist", false );
            serializer.Serialize( asn, parameter, 324, "orderparameters_point_test" ); // point is registered with sql script
            BOOST_CHECK_NO_THROW( CheckEncoding( asn ) );
            CleanAsnLocationList( asn.value.u.pointList );
        }
    }
}


BOOST_AUTO_TEST_CASE( crossbow_test_serialize_line )
{
    MockModel mockModel;
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        workspace.InitializeConnectionReference( "geometry", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );

        crossbow::OrderParameterSerializer serializer( workspace, mockModel );
        ASN1T_MissionParameter asn;
        kernel::OrderParameter parameter( "param", "path", false );
        serializer.Serialize( asn, parameter, 322, "orderparameters_line_test" ); // point is registered with sql script
        BOOST_CHECK_EQUAL( asn.value.t, T_MissionParameter_value_path );
        BOOST_CHECK_EQUAL( asn.value.u.path->type, EnumLocationType::line );
        BOOST_CHECK_EQUAL( asn.value.u.path->coordinates.n, 2u );
        BOOST_CHECK_CLOSE( asn.value.u.path->coordinates.elem[0].latitude, 49.52336174621582, 0.0001 );
        BOOST_CHECK_CLOSE( asn.value.u.path->coordinates.elem[0].longitude, 0.1283868466796875, 0.0001 );
        BOOST_CHECK_CLOSE( asn.value.u.path->coordinates.elem[1].latitude, 49.52353340759277, 0.0001 );
        BOOST_CHECK_CLOSE( asn.value.u.path->coordinates.elem[1].longitude, 0.11705719580078125, 0.0001 );
        BOOST_CHECK_NO_THROW( CheckEncoding( asn ) );
        CleanAsnLocation( asn.value.u.path );
    }
}

BOOST_AUTO_TEST_CASE( crossbow_test_serialize_area )
{
    MockModel mockModel;
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        workspace.InitializeConnectionReference( "geometry", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );

        crossbow::OrderParameterSerializer serializer( workspace, mockModel );
        ASN1T_MissionParameter asn;
        kernel::OrderParameter parameter( "param", "polygon", false );
        serializer.Serialize( asn, parameter, 323, "orderparameters_area_test" ); // point is registered with sql script
        BOOST_CHECK_EQUAL( asn.value.t, T_MissionParameter_value_polygon );
        BOOST_CHECK_EQUAL( asn.value.u.polygon->type, EnumLocationType::polygon );
        BOOST_CHECK_EQUAL( asn.value.u.polygon->coordinates.n, 4u );
        BOOST_CHECK_CLOSE( asn.value.u.polygon->coordinates.elem[0].latitude, 49.52336174621582, 0.0001 );
        BOOST_CHECK_CLOSE( asn.value.u.polygon->coordinates.elem[0].longitude, 0.1283868466796875, 0.0001 );
        BOOST_CHECK_CLOSE( asn.value.u.polygon->coordinates.elem[1].latitude, 49.52353340759277, 0.0001 );
        BOOST_CHECK_CLOSE( asn.value.u.polygon->coordinates.elem[1].longitude, 0.11705719580078125, 0.0001 );
        BOOST_CHECK_CLOSE( asn.value.u.polygon->coordinates.elem[2].latitude, 49.51735359802246, 0.0001 );
        BOOST_CHECK_CLOSE( asn.value.u.polygon->coordinates.elem[2].longitude, 0.12169205297851562, 0.0001 );
        BOOST_CHECK_CLOSE( asn.value.u.polygon->coordinates.elem[3].latitude, 49.52336174621582, 0.0001 );
        BOOST_CHECK_CLOSE( asn.value.u.polygon->coordinates.elem[3].longitude, 0.1283868466796875, 0.0001 );
        BOOST_CHECK_NO_THROW( CheckEncoding( asn ) );
        CleanAsnLocation( asn.value.u.polygon );
    }
}

/*
BOOST_AUTO_TEST_CASE( CrossbowTest_OrderSerialize_Agent )
{
    MockDatabase mockDatabase;
    MockModel mockModel;

    MOCK_EXPECT( mockModel, agents ).once();

    crossbow::OrderParameterSerializer serializer( mockDatabase, mockModel );
    ASN1T_MissionParameter asn;
    kernel::OrderParameter parameter( "param", "Agent", false );
    serializer.Serialize( asn, parameter, 0, "5" );
    BOOST_CHECK_EQUAL( asn.value.t, T_MissionParameter_value_unit );
    BOOST_CHECK_EQUAL( asn.value.u.unit, 5 );
}

BOOST_AUTO_TEST_CASE( CrossbowTest_OrderSerialize_AgentList_0 )
{
    MockDatabase mockDatabase;
    MockModel mockModel;

    MOCK_EXPECT( mockModel, agents ).never();

    crossbow::OrderParameterSerializer serializer( mockDatabase, mockModel );
    ASN1T_MissionParameter asn;
    kernel::OrderParameter parameter( "param", "AgentList", false );
    serializer.Serialize( asn, parameter, 0, "" );
    BOOST_CHECK_EQUAL( asn.value.t, T_MissionParameter_value_unitList );
    BOOST_CHECK_EQUAL( asn.value.u.unitList->n, 0u );
    BOOST_CHECK_EQUAL( asn.value.u.unitList->elem, static_cast< ASN1T_Unit* >( 0 ) );
}

BOOST_AUTO_TEST_CASE( CrossbowTest_OrderSerialize_AgentList_1 )
{
    MockDatabase mockDatabase;
    MockModel mockModel;

    MOCK_EXPECT( mockModel, agents ).once();

    crossbow::OrderParameterSerializer serializer( mockDatabase, mockModel );
    ASN1T_MissionParameter asn;
    kernel::OrderParameter parameter( "param", "AgentList", false );
    serializer.Serialize( asn, parameter, 0, "12" );
    BOOST_CHECK_EQUAL( asn.value.t, T_MissionParameter_value_unitList );
    BOOST_CHECK_EQUAL( asn.value.u.unitList->n, 1u );
    BOOST_CHECK_EQUAL( asn.value.u.unitList->elem[ 0 ], 12 );
}

BOOST_AUTO_TEST_CASE( CrossbowTest_OrderSerialize_AgentList_5 )
{
    MockDatabase mockDatabase;
    MockModel mockModel;

    MOCK_EXPECT( mockModel, agents ).exactly( 5 );

    crossbow::OrderParameterSerializer serializer( mockDatabase, mockModel );
    ASN1T_MissionParameter asn;
    kernel::OrderParameter parameter( "param", "AgentList", false );
    serializer.Serialize( asn, parameter, 0, "12,35468,56,5,3" );
    BOOST_CHECK_EQUAL( asn.value.t, T_MissionParameter_value_unitList );
    BOOST_CHECK_EQUAL( asn.value.u.unitList->n, 5u );
    BOOST_CHECK_EQUAL( asn.value.u.unitList->elem[ 0 ], 12 );
    BOOST_CHECK_EQUAL( asn.value.u.unitList->elem[ 1 ], 35468 );
    BOOST_CHECK_EQUAL( asn.value.u.unitList->elem[ 2 ], 56 );
    BOOST_CHECK_EQUAL( asn.value.u.unitList->elem[ 3 ], 5 );
    BOOST_CHECK_EQUAL( asn.value.u.unitList->elem[ 5 ], 3 );
}
*/
/*
        resolver.Register( "location", T_MissionParameter_value_location )
                .Register( "locationlist", T_MissionParameter_value_locationList )
                .Register( "point", T_MissionParameter_value_point )
                .Register( "pointlist", T_MissionParameter_value_pointList )
                .Register( "polygon", T_MissionParameter_value_polygon )
                .Register( "polygonlist", T_MissionParameter_value_polygonList )
                .Register( "path", T_MissionParameter_value_path )
                .Register( "bool", T_MissionParameter_value_aBool )
                .Register( "real", T_MissionParameter_value_aReal )
                .Register( "enumeration", T_MissionParameter_value_enumeration )
                .Register( "string", T_MissionParameter_value_aCharStr )

                .Register( "automate", T_MissionParameter_value_automat )
                .Register( "automatelist", T_MissionParameter_value_automatList )
                .Register( "agent", T_MissionParameter_value_unit )
                .Register( "agentlist", T_MissionParameter_value_unitList )
                .Register( "agentknowledge", T_MissionParameter_value_unitKnowledge )
                .Register( "agentknowledgelist", T_MissionParameter_value_unitKnowledgeList )
                .Register( "objectknowledge", T_MissionParameter_value_objectKnowledge )
                .Register( "objectknowledgelist", T_MissionParameter_value_objectKnowledgeList )
                .Register( "objective", T_MissionParameter_value_missionObjective )
                .Register( "objectivelist", T_MissionParameter_value_missionObjectiveList )

                .Register( "dangerdirection", T_MissionParameter_value_heading )
                .Register( "direction", T_MissionParameter_value_heading )
                .Register( "phaselinelist", T_MissionParameter_value_limasOrder )

                .Register( "limit", T_MissionParameter_value_line )
                .Register( "intelligencelist", T_MissionParameter_value_intelligenceList );
*/

#endif
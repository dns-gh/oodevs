// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "plugins_test_pch.h"
#include "crossbow_plugin/OrderParameterSerializer.h"
#include "crossbow_plugin/Database_ABC.h"
#include "dispatcher/Model_ABC.h"
#include "clients_kernel/OrderParameter.h"

#include <turtle/mock.hpp>

using namespace plugins;
using namespace plugins::crossbow;


MOCK_BASE_CLASS( MockDatabase, Database_ABC )     // defines a 'MockDatabase' class implementing 'Database_ABC'
{
    MOCK_METHOD( OpenBufferedTable, 2 )
    MOCK_METHOD( OpenTable, 1 )                   // implements the 'display' method from 'view' (taking 1 argument)
    MOCK_METHOD( GetTableName, 1 )
    MOCK_METHOD( ClearTable, 1 )
    MOCK_METHOD( Execute, 1 )
    MOCK_METHOD( Flush, 0 )
};

MOCK_BASE_CLASS( MockModel, dispatcher::Model_ABC )     // defines a 'MockModel' class implementing 'Model_ABC'
{
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
};


BOOST_AUTO_TEST_CASE( CrossbowTest_OrderSerialize_Bool )
{
    MockDatabase mockDatabase;
    MockModel mockModel;

    OrderParameterSerializer serializer( mockDatabase, mockModel );
    ASN1T_MissionParameter asn;    
    kernel::OrderParameter parameter( "param", "bool", false );
    serializer.Serialize( asn, parameter, 0, "true" );
    BOOST_CHECK_EQUAL( asn.value.t, T_MissionParameter_value_aBool );
    BOOST_CHECK_EQUAL( asn.value.u.aBool, static_cast< asn::ASN1BOOL >( true ) );
}

/*
BOOST_AUTO_TEST_CASE( CrossbowTest_OrderSerialize_Agent )
{
    MockDatabase mockDatabase;
    MockModel mockModel;
    
    MOCK_EXPECT( mockModel, agents ).once();
    
    OrderParameterSerializer serializer( mockDatabase, mockModel );
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

    OrderParameterSerializer serializer( mockDatabase, mockModel );
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

    OrderParameterSerializer serializer( mockDatabase, mockModel );
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

    OrderParameterSerializer serializer( mockDatabase, mockModel );
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
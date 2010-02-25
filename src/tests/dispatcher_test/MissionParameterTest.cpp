// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_kernel_test_pch.h"
#include "dispatcher/MissionParameter_Agent.h"
#include "dispatcher/MissionParameter_AgentKnowledge.h"
#include "dispatcher/MissionParameter_AgentKnowledgeList.h"
#include "dispatcher/MissionParameter_AgentList.h"
#include "dispatcher/MissionParameter_Automat.h"
#include "dispatcher/MissionParameter_AutomatList.h"
#include "dispatcher/MissionParameter_Bool.h"
#include "dispatcher/MissionParameter_Direction.h"
#include "dispatcher/MissionParameter_DotationType.h"
#include "dispatcher/MissionParameter_Enumeration.h"
#include "dispatcher/MissionParameter_EquipmentType.h"
#include "dispatcher/MissionParameter_GDH.h"
#include "dispatcher/MissionParameter_GenObject.h"
#include "dispatcher/MissionParameter_GenObjectList.h"
#include "dispatcher/MissionParameter_IndirectFire.h"
#include "dispatcher/MissionParameter_IntelligenceList.h"
#include "dispatcher/MissionParameter_LimasOrder.h"
#include "dispatcher/MissionParameter_Line.h"
#include "dispatcher/MissionParameter_Location.h"
#include "dispatcher/MissionParameter_LocationList.h"
#include "dispatcher/MissionParameter_MaintenancePriorities.h"
#include "dispatcher/MissionParameter_MedicalPriorities.h"
#include "dispatcher/MissionParameter_NatureAtlas.h"
#include "dispatcher/MissionParameter_Numeric.h"
#include "dispatcher/MissionParameter_ObjectKnowledge.h"
#include "dispatcher/MissionParameter_ObjectKnowledgeList.h"
#include "dispatcher/MissionParameter_Objective.h"
#include "dispatcher/MissionParameter_ObjectiveList.h"
#include "dispatcher/MissionParameter_Path.h"
#include "dispatcher/MissionParameter_PathList.h"
#include "dispatcher/MissionParameter_Point.h"
#include "dispatcher/MissionParameter_PointList.h"
#include "dispatcher/MissionParameter_Polygon.h"
#include "dispatcher/MissionParameter_PolygonList.h"
#include "dispatcher/MissionParameter_PopulationKnowledge.h"
#include "dispatcher/MissionParameter_String.h"
#include "dispatcher/MissionParameter_UrbanBlock.h"
#include "protocol/protocol.h"
#include <google/protobuf/Descriptor.h>

namespace
{
    template< typename T >
    void CheckSerialization( Common::MsgMissionParameter& input )
    {
        input.set_null_value( false );
        if( !input.IsInitialized() )
        {
            std::stringstream ss;
            ss << "Message of type \"" << input.GetDescriptor()->full_name()
               << "\" is missing required fields: " << input.InitializationErrorString();
            BOOST_FAIL( ss.str() );
        }
        T parameter( input );
        Common::MsgMissionParameter output;
        parameter.Send( output );
        std::string inputString, outputString;
        BOOST_CHECK( input.SerializeToString( &inputString ) );
        BOOST_CHECK( output.SerializeToString( &outputString ) );
        BOOST_CHECK_EQUAL( inputString, outputString );
    }
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Agent
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Agent )
{
    Common::MsgMissionParameter input;
    input.mutable_value()->mutable_unit()->set_oid( 42 );
    CheckSerialization< dispatcher::MissionParameter_Agent >( input );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_AgentKnowledge
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_AgentKnowledge )
{
    Common::MsgMissionParameter input;
    input.mutable_value()->mutable_unitknowledge()->set_oid( 42 );
    CheckSerialization< dispatcher::MissionParameter_AgentKnowledge >( input );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_AgentKnowledgeList
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_AgentKnowledgeList )
{
    {
        Common::MsgMissionParameter input;
        input.mutable_value()->mutable_unitknowledgelist();
        CheckSerialization< dispatcher::MissionParameter_AgentKnowledgeList >( input );
    }
    {
        Common::MsgMissionParameter input;
        input.mutable_value()->mutable_unitknowledgelist()->add_elem()->set_oid( 42 );
        input.mutable_value()->mutable_unitknowledgelist()->add_elem()->set_oid( 51 );
        input.mutable_value()->mutable_unitknowledgelist()->add_elem()->set_oid( 69 );
        CheckSerialization< dispatcher::MissionParameter_AgentKnowledgeList >( input );
    }
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_AgentList
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_AgentList )
{
    {
        Common::MsgMissionParameter input;
        input.mutable_value()->mutable_unitlist();
        CheckSerialization< dispatcher::MissionParameter_AgentList >( input );
    }
    {
        Common::MsgMissionParameter input;
        input.mutable_value()->mutable_unitlist()->add_elem()->set_oid( 42 );
        input.mutable_value()->mutable_unitlist()->add_elem()->set_oid( 51 );
        input.mutable_value()->mutable_unitlist()->add_elem()->set_oid( 69 );
        CheckSerialization< dispatcher::MissionParameter_AgentList >( input );
    }
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Automat
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Automat )
{
    Common::MsgMissionParameter input;
    input.mutable_value()->mutable_automat()->set_oid( 42 );
    CheckSerialization< dispatcher::MissionParameter_Automat >( input );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_AutomatList
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_AutomatList )
{
    {
        Common::MsgMissionParameter input;
        input.mutable_value()->mutable_automatlist();
        CheckSerialization< dispatcher::MissionParameter_AutomatList >( input );
    }
    {
        Common::MsgMissionParameter input;
        input.mutable_value()->mutable_automatlist()->add_elem()->set_oid( 42 );
        input.mutable_value()->mutable_automatlist()->add_elem()->set_oid( 51 );
        input.mutable_value()->mutable_automatlist()->add_elem()->set_oid( 69 );
        CheckSerialization< dispatcher::MissionParameter_AutomatList >( input );
    }
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Bool
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Bool )
{
    Common::MsgMissionParameter input;
    input.mutable_value()->set_abool( true );
    CheckSerialization< dispatcher::MissionParameter_Bool >( input );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Direction
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Direction )
{
    Common::MsgMissionParameter input;
    input.mutable_value()->mutable_heading()->set_heading( 180 );
    CheckSerialization< dispatcher::MissionParameter_Direction >( input );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_DotationType
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_DotationType )
{
    Common::MsgMissionParameter input;
    input.mutable_value()->mutable_dotationtype()->set_oid( 42 );
    CheckSerialization< dispatcher::MissionParameter_DotationType >( input );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Enumeration
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Enumeration )
{
    Common::MsgMissionParameter input;
    input.mutable_value()->set_enumeration( 42 );
    CheckSerialization< dispatcher::MissionParameter_Enumeration >( input );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_EquipmentType
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_EquipmentType )
{
    Common::MsgMissionParameter input;
    input.mutable_value()->mutable_equipmenttype()->set_equipment( 42 );
    CheckSerialization< dispatcher::MissionParameter_EquipmentType >( input );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Datetime
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Datetime )
{
    Common::MsgMissionParameter input;
    input.mutable_value()->mutable_datetime()->set_data( "20100225T105942" );
    CheckSerialization< dispatcher::MissionParameter_GDH >( input );
}

namespace
{
    void MakePoint( Common::MsgCoordLatLong& coord )
    {
        coord.set_latitude( 42.f );
        coord.set_longitude( 1.42f );
    }

    void MakePoint( Common::MsgLocation& location )
    {
        location.set_type( Common::MsgLocation_Geometry_point );
        MakePoint( *location.mutable_coordinates()->add_elem() );
    }
    
    void MakeLine( Common::MsgLocation& location )
    {
        location.set_type( Common::MsgLocation_Geometry_line );
        for( unsigned int i = 0; i < 5; ++i )
            MakePoint( *location.mutable_coordinates()->add_elem() );
    }

    void MakePolygon( Common::MsgLocation& location )
    {
        location.set_type( Common::MsgLocation_Geometry_polygon );
        for( unsigned int i = 0; i < 5; ++i )
            MakePoint( *location.mutable_coordinates()->add_elem() );
    }

    void MakeGenObject( Common::MsgPlannedWork& object )
    {
        object.set_type( "MyObject" );
        object.set_type_obstacle( Common::ObstacleType_DemolitionTargetType_preliminary );
        object.set_activity_time( 1 );
        object.set_densite( 0.5f );
        object.set_tc2( 42 );
        MakePoint( *object.mutable_position() );
    }
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_GenObject
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_GenObject )
{
    Common::MsgMissionParameter input;
    MakeGenObject( *input.mutable_value()->mutable_plannedwork() );
    CheckSerialization< dispatcher::MissionParameter_GenObject >( input );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_GenObjectList
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_GenObjectList )
{
    {
        Common::MsgMissionParameter input;
        input.mutable_value()->mutable_plannedworklist();
        CheckSerialization< dispatcher::MissionParameter_GenObjectList >( input );
    }
    {
        Common::MsgMissionParameter input;
        for( unsigned int i = 0; i < 5; ++i )
            MakeGenObject( *input.mutable_value()->mutable_plannedworklist()->add_elem() );
        CheckSerialization< dispatcher::MissionParameter_GenObjectList >( input );
    }
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_IndirectFire
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_IndirectFire )
{
    Common::MsgMissionParameter input;
    input.mutable_value()->set_tirindirect( 42 );
    CheckSerialization< dispatcher::MissionParameter_IndirectFire >( input );
}

namespace
{
    void MakeIntelligence( Common::MsgIntelligence& intel )
    {
        intel.set_diplomacy( Common::EnumDiplomacy::enemy_diplo );
        intel.set_embarked( true );
        intel.set_level( Common::EnumNatureLevel::i );
        intel.set_name( "MyName" );
        intel.set_nature( "my/nature" );
        intel.mutable_formation()->set_oid( 42 );
        MakePoint( *intel.mutable_location() );
    }
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_IntelligenceList
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_IntelligenceList )
{
    {
        Common::MsgMissionParameter input;
        input.mutable_value()->mutable_intelligencelist();
        CheckSerialization< dispatcher::MissionParameter_IntelligenceList >( input );
    }
    {
        Common::MsgMissionParameter input;
        for( unsigned int i = 0; i < 5; ++i )
            MakeIntelligence( *input.mutable_value()->mutable_intelligencelist()->add_elem() );
        CheckSerialization< dispatcher::MissionParameter_IntelligenceList >( input );
    }
}

namespace
{
    void MakePhaseLine( Common::MsgLimaOrder& message )
    {
        message.set_fonctions( 0, Common::MsgLimaOrder_Function_ligne_coordination );
        message.set_fonctions( 1, Common::MsgLimaOrder_Function_ligne_debouche );
        message.mutable_horaire()->set_data( "20100225T105942" );
        for( unsigned int i = 0; i < 4; ++i )
            MakeLine( *message.mutable_lima()->mutable_location() );
    }
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_LimasOrder
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_LimasOrder )
{
    {
        Common::MsgMissionParameter input;
        input.mutable_value()->mutable_limasorder();
        CheckSerialization< dispatcher::MissionParameter_LimasOrder >( input );
    }
    {
        Common::MsgMissionParameter input;
        for( unsigned int i = 0; i < 5; ++i )
            MakePhaseLine( *input.mutable_value()->mutable_limasorder()->add_elem() );
        CheckSerialization< dispatcher::MissionParameter_LimasOrder >( input );
    }
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Line
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Line )
{
    Common::MsgMissionParameter input;
    MakeLine( *input.mutable_value()->mutable_line()->mutable_location() );
    CheckSerialization< dispatcher::MissionParameter_Line >( input );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Location
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Location )
{
    {
        Common::MsgMissionParameter input;
        MakePoint( *input.mutable_value()->mutable_location() );
        CheckSerialization< dispatcher::MissionParameter_Location >( input );
    }
    {
        Common::MsgMissionParameter input;
        MakeLine( *input.mutable_value()->mutable_location() );
        CheckSerialization< dispatcher::MissionParameter_Location >( input );
    }
    {
        Common::MsgMissionParameter input;
        MakePolygon( *input.mutable_value()->mutable_location() );
        CheckSerialization< dispatcher::MissionParameter_Location >( input );
    }
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_LocationList
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_LocationList )
{
    {
        Common::MsgMissionParameter input;
        input.mutable_value()->mutable_locationlist();
        CheckSerialization< dispatcher::MissionParameter_LocationList >( input );
    }
    {
        Common::MsgMissionParameter input;
        for( unsigned int i = 0; i < 5; ++i )
            MakeLine( *input.mutable_value()->mutable_locationlist()->add_elem() );
        CheckSerialization< dispatcher::MissionParameter_LocationList >( input );
    }
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_MaintenancePriorities
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_MaintenancePriorities )
{
    {
        Common::MsgMissionParameter input;
        input.mutable_value()->mutable_logmaintenancepriorities();
        CheckSerialization< dispatcher::MissionParameter_MaintenancePriorities >( input );
    }
    {
        Common::MsgMissionParameter input;
        for( unsigned int i = 0; i < 5; ++i )
            input.mutable_value()->mutable_logmaintenancepriorities()->add_elem()->set_equipment( 42 + i );
        CheckSerialization< dispatcher::MissionParameter_MaintenancePriorities >( input );
    }
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_MedicalPriorities
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_MedicalPriorities )
{
    {
        Common::MsgMissionParameter input;
        input.mutable_value()->mutable_logmedicalpriorities();
        CheckSerialization< dispatcher::MissionParameter_MedicalPriorities >( input );
    }
    {
        Common::MsgMissionParameter input;
        for( unsigned int i = 0; i < Common::EnumHumanWound_MAX; ++i )
            input.mutable_value()->mutable_logmedicalpriorities()->add_elem( Common::EnumHumanWound( i ) );
        CheckSerialization< dispatcher::MissionParameter_MedicalPriorities >( input );
    }
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_NatureAtlas
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_NatureAtlas )
{
    Common::MsgMissionParameter input;
    input.mutable_value()->mutable_atlasnature()->set_nature( 0xfe );
    CheckSerialization< dispatcher::MissionParameter_NatureAtlas >( input );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Numeric
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Numeric )
{
    Common::MsgMissionParameter input;
    input.mutable_value()->set_areal( 42.f );
    CheckSerialization< dispatcher::MissionParameter_Numeric >( input );
}

namespace
{
    void MakeObjective( Common::MsgMissionObjective& message )
    {
        message.mutable_horaire()->set_data( "20100225T105942" );
        MakePoint( *message.mutable_localisation() );
    }
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Objective
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Objective )
{
    Common::MsgMissionParameter input;
    MakeObjective( *input.mutable_value()->mutable_missionobjective() );
    CheckSerialization< dispatcher::MissionParameter_Objective >( input );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_ObjectiveList
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_ObjectiveList )
{
    Common::MsgMissionParameter input;
    for( unsigned int i = 0; i < 5; ++i )
        MakeObjective( *input.mutable_value()->mutable_missionobjectivelist()->add_elem() );
    CheckSerialization< dispatcher::MissionParameter_ObjectiveList >( input );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_ObjectKnowledge
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_ObjectKnowledge )
{
    Common::MsgMissionParameter input;
    input.mutable_value()->mutable_objectknowledge()->set_oid( 42 );
    CheckSerialization< dispatcher::MissionParameter_ObjectKnowledge >( input );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_ObjectKnowledgeList
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_ObjectKnowledgeList )
{
    {
        Common::MsgMissionParameter input;
        input.mutable_value()->mutable_objectknowledgelist();
        CheckSerialization< dispatcher::MissionParameter_ObjectKnowledgeList >( input );
    }
    {
        Common::MsgMissionParameter input;
        input.mutable_value()->mutable_objectknowledgelist()->add_elem()->set_oid( 42 );
        input.mutable_value()->mutable_objectknowledgelist()->add_elem()->set_oid( 51 );
        input.mutable_value()->mutable_objectknowledgelist()->add_elem()->set_oid( 69 );
        CheckSerialization< dispatcher::MissionParameter_ObjectKnowledgeList >( input );
    }
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Path
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Path )
{
    Common::MsgMissionParameter input;
    MakeLine( *input.mutable_value()->mutable_path()->mutable_location() );
    CheckSerialization< dispatcher::MissionParameter_Path >( input );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_PathList
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_PathList )
{
    {
        Common::MsgMissionParameter input;
        input.mutable_value()->mutable_pathlist();
        CheckSerialization< dispatcher::MissionParameter_PathList >( input );
    }
    {
        Common::MsgMissionParameter input;
        for( unsigned int i = 0; i < 5; ++i )
            MakeLine( *input.mutable_value()->mutable_pathlist()->add_elem()->mutable_location() );
        CheckSerialization< dispatcher::MissionParameter_PathList >( input );
    }
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Point
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Point )
{
    Common::MsgMissionParameter input;
    MakePoint( *input.mutable_value()->mutable_point()->mutable_location() );
    CheckSerialization< dispatcher::MissionParameter_Point >( input );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_PointList
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_PointList )
{
    {
        Common::MsgMissionParameter input;
        input.mutable_value()->mutable_pointlist();
        CheckSerialization< dispatcher::MissionParameter_PointList >( input );
    }
    {
        Common::MsgMissionParameter input;
        for( unsigned int i = 0; i < 5; ++i )
            MakePoint( *input.mutable_value()->mutable_pointlist()->add_elem()->mutable_location() );
        CheckSerialization< dispatcher::MissionParameter_PointList >( input );
    }
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Polygon
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Polygon )
{
    Common::MsgMissionParameter input;
    MakePolygon( *input.mutable_value()->mutable_polygon()->mutable_location() );
    CheckSerialization< dispatcher::MissionParameter_Polygon >( input );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_PolygonList
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_PolygonList )
{
    {
        Common::MsgMissionParameter input;
        input.mutable_value()->mutable_polygonlist();
        CheckSerialization< dispatcher::MissionParameter_PolygonList >( input );
    }
    {
        Common::MsgMissionParameter input;
        for( unsigned int i = 0; i < 5; ++i )
            MakePolygon( *input.mutable_value()->mutable_polygonlist()->add_elem()->mutable_location() );
        CheckSerialization< dispatcher::MissionParameter_PolygonList >( input );
    }
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_PopulationKnowledge
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_PopulationKnowledge )
{
    Common::MsgMissionParameter input;
    input.mutable_value()->mutable_populationknowledge()->set_oid( 42 );
    CheckSerialization< dispatcher::MissionParameter_PopulationKnowledge >( input );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_String
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_String )
{
    Common::MsgMissionParameter input;
    input.mutable_value()->set_acharstr( "My String" );
    CheckSerialization< dispatcher::MissionParameter_String >( input );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_UrbanBlock
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_UrbanBlock )
{
    Common::MsgMissionParameter input;
    input.mutable_value()->mutable_urbanblock()->set_oid( 42 );
    CheckSerialization< dispatcher::MissionParameter_UrbanBlock >( input );
}

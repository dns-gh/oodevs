// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "SingletonTerminator.h"
#include "protocol/Protocol.h"
#include "Fixture.h"
#include "MockDEC_KnowledgeResolver_ABC.h"
#include "MockMIL_EntityManager_ABC.h"
#include "MockMIL_ObjectType_ABC.h"
#include "MockObjectTypeResolver.h"
#include "MockArmy.h"
#include "StubMIL_Object_ABC.h"
#include "StubMIL_PopulationType.h"
#include "StubMIL_Population.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/MIL_KnowledgeGroupType.h"
#include "MockNET_Publisher_ABC.h"
#include "MockMIL_Object_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "MockAgent.h"
#include "ActionManager.h"
#include "MissionController.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Communications/PHY_RolePion_Communications.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationNature.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationLogisticType.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Orders/MIL_AgentKnowledgeParameter.h"
#include "Entities/Orders/MIL_AgentParameter.h"
#include "Entities/Orders/MIL_AutomatParameter.h"
#include "Entities/Orders/MIL_AtlasNatureParameter.h"
#include "Entities/Orders/MIL_BoolParameter.h"
#include "Entities/Orders/MIL_DateTimeParameter.h"
#include "Entities/Orders/MIL_DirectionParameter.h"
#include "Entities/Orders/MIL_DotationTypeParameter.h"
#include "Entities/Orders/MIL_EnumerationParameter.h"
#include "Entities/Orders/MIL_ListParameter.h"
#include "Entities/Orders/MIL_LocationParameter.h"
#include "Entities/Orders/MIL_LogMedicalPrioritiesParameter.h"
#include "Entities/Orders/MIL_ObjectKnowledgeParameter.h"
#include "Entities/Orders/MIL_OrderTypeParameter.h"
#include "Entities/Orders/MIL_PathParameter.h"
#include "Entities/Orders/MIL_PlannedWorkParameter.h"
#include "Entities/Orders/MIL_PointParameter.h"
#include "Entities/Orders/MIL_PolygonParameter.h"
#include "Entities/Orders/MIL_PopulationKnowledgeParameter.h"
#include "Entities/Orders/MIL_RealParameter.h"
#include "Entities/Orders/MIL_StringParameter.h"
#include "Entities/Orders/MIL_TirIndirectParameter.h"
#include "Entities/Orders/MIL_PionMissionType.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Entities/Orders/MIL_ParameterType_ABC.h"
#include "simulation_terrain/TER_World.h"

using namespace sword;

namespace
{
    struct Fixture : SingletonTerminator
    {
        Fixture()
            : xis( "<knowledge-groups>"
                   "  <knowledge-group name='Standard' communication-delay='01m'>"
                   "    <unit-knowledge max-lifetime='03h' max-unit-to-knowledge-distance='60000'/>"
                   "    <population-knowledge max-lifetime='2m'/>"
                   "  </knowledge-group>"
                   "</knowledge-groups>" )
            , controller( actions )
        {
            MIL_KnowledgeGroupType::InitializeWithTime( xis, 0.5f );
            MOCK_EXPECT( publisher.Send );
            MOCK_EXPECT( army.GetID ).returns( 29u );
            MOCK_EXPECT( army.UnregisterKnowledgeGroup );
        }
        ~Fixture()
        {
            MIL_KnowledgeGroupType::Terminate();
        }
        xml::xistringstream xis;
        MockNET_Publisher_ABC publisher;
        ActionManager actions;
        MissionController controller;
        MockArmy army;
        MockDEC_KnowledgeResolver_ABC resolver;
        MockMIL_EntityManager_ABC manager;
    };
}

// -----------------------------------------------------------------------------
// Name: TestMIL_AgentKnowledgeParameter
// Created: LDC 2009-06-09
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( TestMIL_AgentKnowledgeParameter, Fixture )
{
    Id in;
    in.set_id( 35 );
    xml::xistringstream xis( "<main dia-type='PionTest' file='PionTest.bms' id='12' name='stuff'/>" );
    std::map< std::string, const MIL_MissionType_ABC* > missionTypes;
    DEC_Model model( "test", xis >> xml::start( "main" ), testOptions.GetDataPath( "." ), missionTypes, false, testOptions.GetDataPath( "resources" ) );
    StubMIL_AgentTypePion type( model );
    MIL_AgentPion pion( type, controller );
    xml::xistringstream xisID( "<root id='35'/>" );
    MockAgent agent( xisID >> xml::start( "root" ) );
    MOCK_EXPECT( manager.FindAgentPion ).once().returns( &pion );
    MOCK_EXPECT( agent.BelongsTo ).once().returns( false );
    MOCK_EXPECT( agent.GetType ).once().returns( boost::cref( type ) );
    boost::shared_ptr< MIL_KnowledgeGroup > group(
        new MIL_KnowledgeGroup(
            xml::xistringstream( "<root id='30' type='Standard' name='group'/>" )
                >> xml::start( "root" ),
            army, 0 ) );
    boost::shared_ptr< DEC_Knowledge_Agent > knowledge( new DEC_Knowledge_Agent( *group, agent ) );
    MOCK_EXPECT( resolver.ResolveKnowledgeAgent ).once().returns( knowledge );
    MIL_AgentKnowledgeParameter param( in, resolver, manager );
    MissionParameter_Value out;
    BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
    BOOST_CHECK_EQUAL( true, out.has_agentknowledge() );
    BOOST_CHECK_EQUAL( 35u, out.agentknowledge().id() );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_ObjectKnowledgeParameter
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( TestMIL_ObjectKnowledgeParameter, Fixture )
{
    Id in;
    in.set_id( 56 );
    MockMIL_ObjectType_ABC objectType;
    StubMIL_Object_ABC object( &army, objectType, 56u );
    MOCK_EXPECT( objectType.GetName ).once().returns( "type" );
    MOCK_EXPECT( manager.FindObject ).once().returns( &object );
    boost::shared_ptr< DEC_Knowledge_Object > knowledge( new DEC_Knowledge_Object( army, object ) );
    MOCK_EXPECT( resolver.ResolveKnowledgeObject ).once().returns( knowledge );
    MIL_ObjectKnowledgeParameter param( in, resolver, manager );
    MissionParameter_Value out;
    BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
    BOOST_CHECK_EQUAL( true, out.has_objectknowledge() );
    BOOST_CHECK_EQUAL( 56u, out.objectknowledge().id() );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PopulationKnowledgeParameter
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( TestMIL_PopulationKnowledgeParameter, Fixture )
{
    Id in;
    in.set_id( 0 );
    xml::xistringstream xis( "<main dia-type='PionTest' file='PionTest.bms'/>" );
    std::map< std::string, const MIL_MissionType_ABC* > missionTypes;
    DEC_Model model( "test", xis >> xml::start( "main" ), testOptions.GetDataPath( "." ), missionTypes, false, testOptions.GetDataPath( "resources" ) );
    StubMIL_PopulationType type( model );
    StubMIL_Population population( type, controller, army );
    MOCK_EXPECT( manager.FindPopulation ).once().returns( &population );
    boost::shared_ptr< MIL_KnowledgeGroup > group(
        new MIL_KnowledgeGroup(
            xml::xistringstream( "<root id='30' type='Standard' name='group'/>" )
                >> xml::start( "root" ),
            army, 0 ) );
    boost::shared_ptr< DEC_Knowledge_Population > knowledge( new DEC_Knowledge_Population( *group, population ) );
    MOCK_EXPECT( resolver.ResolveKnowledgePopulation ).once().returns( knowledge );
    MIL_PopulationKnowledgeParameter param( in, resolver, manager );
    MissionParameter_Value out;
    BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
    BOOST_CHECK_EQUAL( true, out.has_crowdknowledge() );
    BOOST_CHECK_EQUAL( 0u, out.crowdknowledge().id() );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_AgentParameter
// Created: LDC 2009-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_AgentParameter )
{
    Id in;
    in.set_id( 12 );
    MockMIL_EntityManager_ABC entityManager;
    ActionManager actions;
    MissionController controller( actions );
    MIL_EffectManager effectManager;
    FixturePion fixture( controller, effectManager );
    fixture.pPion_->RegisterRole( *new DEC_RolePion_Decision( *fixture.pPion_, 100, 100, false ) );
    MOCK_EXPECT( entityManager.FindAgentPion ).once().returns( fixture.pPion_.get() );
    MIL_AgentParameter param( in, entityManager );
    MissionParameter_Value out;
    BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
    BOOST_CHECK_EQUAL( true, out.has_agent() );
    BOOST_CHECK_EQUAL( 12u, out.agent().id() );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_AutomatParameter
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_AutomatParameter )
{
    Id in;
    in.set_id( 0 );
    MockMIL_EntityManager_ABC entityManager;
    ActionManager actions;
    MissionController controller( actions );
    FixtureAutomate fixture( controller );
    //fixture.pAutomat_->RegisterRole( *new DEC_AutomateDecision( *fixture.pAutomat_ ) );
    MOCK_EXPECT( entityManager.FindAutomate ).once().returns( fixture.pAutomat_.get() );
    MIL_AutomatParameter param( in, entityManager );
    MissionParameter_Value out;
    BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
    BOOST_CHECK_EQUAL( true, out.has_automat() );
    BOOST_CHECK_EQUAL( 0u, out.automat().id() );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_AtlasNatureParameter
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_AtlasNatureParameter )
{
    Nature in;
    in.set_flags( Nature::tank | Nature::logistics | Nature::anti_aircraft );
    MIL_AtlasNatureParameter param( in );
    MissionParameter_Value out;
    BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
    BOOST_CHECK_EQUAL( true, out.has_nature() );
    BOOST_CHECK_EQUAL( Nature::tank, out.nature().flags() & Nature::tank );
    BOOST_CHECK_EQUAL( Nature::logistics, out.nature().flags() & Nature::logistics );
    BOOST_CHECK_EQUAL( Nature::anti_aircraft, out.nature().flags() & Nature::anti_aircraft );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_BoolParameter
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_BoolParameter )
{
    {
        MIL_BoolParameter param( true );
        MissionParameter_Value out;
        BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
        BOOST_CHECK_EQUAL( true, out.has_booleanvalue() );
        BOOST_CHECK_EQUAL( true, out.booleanvalue() );
    }
    {
        MIL_BoolParameter param( false );
        MissionParameter_Value out;
        BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
        BOOST_CHECK_EQUAL( true, out.has_booleanvalue() );
        BOOST_CHECK_EQUAL( false, out.booleanvalue() );
    }
}

// -----------------------------------------------------------------------------
// Name: TestMIL_DateTimeParameter
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_DateTimeParameter )
{
    DateTime in;
    NET_ASN_Tools::WriteGDH( 42, in );
    MIL_DateTimeParameter param( in );
    MissionParameter_Value out;
    BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
    BOOST_CHECK_EQUAL( true, out.has_datetime() );
    BOOST_CHECK_EQUAL( out.datetime().data(), "19700101T000042" );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_DirectionParameter
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_DirectionParameter )
{
    Heading in;
    in.set_heading( 42 );
    MIL_DirectionParameter param( in );
    MissionParameter_Value out;
    BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
    BOOST_CHECK_EQUAL( true, out.has_heading() );
    BOOST_CHECK_EQUAL( 42, out.heading().heading() );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_DotationTypeParameter
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_DotationTypeParameter )
{
    SingletonTerminator terminator;

    xml::xistringstream xisNat( "<natures>"
                                "   <nature type='Solid' id='1'/>"
                                "</natures>" );
    PHY_DotationNature::Initialize( xisNat );
    xml::xistringstream xisLogClass( "<logistic-supply-classes>"
                                     "   <logistic-supply-class id='12' type='whatever'/>"
                                     "</logistic-supply-classes>" );
    PHY_DotationLogisticType::Initialize( xisLogClass );
    xml::xistringstream xis( "<resources>"
                             "   <resource name='Food' id='42' category='ration' nature='Solid' logistic-supply-class='whatever' id-nature='1' package-mass='0.015' package-size='10' package-volume='0.01'/>"
                             "</resources>" );
    MockMIL_EntityManager_ABC entityManager;
    PHY_DotationType::Initialize( xis );
    Id in;
    in.set_id( 42 );
    MIL_DotationTypeParameter param( in );
    MissionParameter_Value out;
    BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
    BOOST_CHECK_EQUAL( true, out.has_resourcetype() );
    BOOST_CHECK_EQUAL( 42u, out.resourcetype().id() );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_EnumerationParameter
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_EnumerationParameter )
{
    int in = 42;
    MIL_EnumerationParameter param( in );
    MissionParameter_Value out;
    BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
    BOOST_CHECK_EQUAL( true, out.has_enumeration() );
    BOOST_CHECK_EQUAL( 42, out.enumeration() );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_EquipmentTypeParameter
// Created: LDC 2009-06-10
// $$$$ LDC: Impossible to test without getting all the singletons:
// call to MIL_AgentServer::GetWorkspace().GetTimeStepDuration()...
// -----------------------------------------------------------------------------
//BOOST_AUTO_TEST_CASE( TestMIL_EquipmentTypeParameter )
//{
//    PHY_ComposanteState::Initialize();
//    tools::Xifstream xisVol( "../../data/data/models/bmstest/physical/Volumes.xml" );
//    PHY_Volume::Initialize( xisVol );
//    tools::Xifstream xisProt( "../../data/data/models/bmstest/physical/Protections.xml" );
//    PHY_Protection::Initialize ( xisProt );
//    tools::Xifstream xis( "../../data/data/models/bmstest/physical/Composantes.xml" );
//    MockMIL_Time_ABC time;
//    PHY_ComposanteTypePion::Initialize( time, xis );
//    EquipmentType in = 22;
//    MIL_EquipmentTypeParameter param( in );
//    EquipmentType out;
//    BOOST_CHECK_EQUAL( true, param.ToEquipmentType( out ) );
//    BOOST_CHECK_EQUAL( 22, out );
//}

namespace
{
    void FillRlyehLocation( Location& in )
    {
        in.set_type( sword::Location_Geometry_point );
        in.mutable_coordinates()->add_elem()->set_latitude( 48.52f );
        in.mutable_coordinates()->mutable_elem(0)->set_longitude( 2.17f );
    }

    void CompareLocationToRlyeh( const Location& out )
    {
        BOOST_CHECK_EQUAL( sword::Location_Geometry_point, out.type() );
        BOOST_CHECK_EQUAL( 1, out.coordinates().elem_size() );
        BOOST_CHECK_CLOSE( 48.52, out.coordinates().elem(0).latitude(), 1. );
        BOOST_CHECK_CLOSE( 2.17, out.coordinates().elem(0).longitude(), 1. );
    }

    void FillPolygonLocation( Location& in, float offset )
    {
        in.set_type( sword::Location_Geometry_polygon );
        in.mutable_coordinates()->add_elem()->set_latitude( 50.f + offset );
        in.mutable_coordinates()->mutable_elem(0)->set_longitude( 2.17f + offset );
        in.mutable_coordinates()->add_elem()->set_latitude( 49.52f + offset );
        in.mutable_coordinates()->mutable_elem(1)->set_longitude( 3.f + offset );
        in.mutable_coordinates()->add_elem()->set_latitude( 48.52f + offset );
        in.mutable_coordinates()->mutable_elem(2)->set_longitude( 3.17f + offset );
    }

    void CompareLocationToPolygon( const Location& out, float offset )
    {
        BOOST_CHECK_EQUAL( sword::Location_Geometry_polygon, out.type() );
        BOOST_CHECK_EQUAL( 4, out.coordinates().elem_size() );
        BOOST_CHECK_CLOSE( 50. + offset, out.coordinates().elem(0).latitude(), 1. );
        BOOST_CHECK_CLOSE( 2.17 + offset, out.coordinates().elem(0).longitude(), 1. );
        BOOST_CHECK_CLOSE( 49.52 + offset, out.coordinates().elem(1).latitude(), 1. );
        BOOST_CHECK_CLOSE( 3. + offset, out.coordinates().elem(1).longitude(), 1. );
        BOOST_CHECK_CLOSE( 48.52 + offset, out.coordinates().elem(2).latitude(), 1. );
        BOOST_CHECK_CLOSE( 3.17 + offset, out.coordinates().elem(2).longitude(), 1. );
        BOOST_CHECK_EQUAL( out.coordinates().elem(0).latitude(), out.coordinates().elem(3).latitude() );
        BOOST_CHECK_EQUAL( out.coordinates().elem(0).longitude(), out.coordinates().elem(3).longitude() );
    }
}

// -----------------------------------------------------------------------------
// Name: TestMIL_LocationParameter
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_LocationParameter )
{
    FakeWorld world( "worldwide/tests/EmptyParis-ML" );
    Location in;
    FillRlyehLocation( in );
    MIL_LocationParameter param( in );
    in.mutable_coordinates()->clear_elem();
    MissionParameter_Value out;
    BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
    BOOST_CHECK_EQUAL( true, out.has_location() );
    CompareLocationToRlyeh( out.location() );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_LogMaintenancePrioritiesParameter
// Created: LDC 2009-06-11
// $$$$ LDC: Impossible to test without getting all the singletons:
// Same as MIL_EquipmentTypeParameter
// -----------------------------------------------------------------------------
//BOOST_AUTO_TEST_CASE( TestMIL_LogMaintenancePrioritiesParameter )
//{
//    LogMaintenancePriorities in;
//    MIL_LogMaintenancePrioritiesParameter param( in );
//    LogMaintenancePriorities out;
//    BOOST_CHECK_EQUAL( true, param.ToMaintenancePriorities( out ) );
//}

// -----------------------------------------------------------------------------
// Name: TestMIL_LogMaintenancePrioritiesParameter
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_LogMaintenancePrioritiesParameter )
{
    PHY_HumanWound::Initialize();
    LogMedicalPriorities in;
    in.add_elem( sword::wounded_urgency_2 );
    MIL_LogMedicalPrioritiesParameter param( in );
    in.clear_elem();
    MissionParameter_Value out;
    BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
    BOOST_CHECK_EQUAL( true, out.has_logmedicalpriorities() );
    BOOST_CHECK_EQUAL( 1, out.logmedicalpriorities().elem_size() );
    BOOST_CHECK_EQUAL( sword::wounded_urgency_2, out.logmedicalpriorities().elem(0) );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_MissionObjectiveListParameter
// Created: LDC 2009-06-11
// $$$$ LDC: Impossible to test without getting all the singletons:
// Calls MIL_AgentServer::GetWorkspace().RealTimeToTick( value );
// -----------------------------------------------------------------------------
//BOOST_AUTO_TEST_CASE( TestMIL_MissionObjectiveListParameter )
//{
//    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );
//    MissionObjectiveList in;
//    in.set_n( 1 );
//    in.elem = new MissionObjective[1];
//    NET_ASN_Tools::WriteGDH( 42, in.elem[0].horaire );
//    FillRlyehLocation( in.elem[0].localisation );
//
//    MIL_MissionObjectiveListParameter param( in );
//
//    delete[] in.elem[0].localisation.coordinates.elem;
//    delete in.elem;
//
//    MissionObjectiveList out;
//    BOOST_CHECK_EQUAL( true, param.ToObjectiveList( out ) );
//    BOOST_CHECK_EQUAL( 1, out.n );
//    CompareDateTo42( out.elem[0].horaire );
//    CompareLocationToRlyeh( out.elem[0].localisation );
//    delete[] out.elem;
//}

// -----------------------------------------------------------------------------
// Name: TestMIL_PathParameter_Throw
// Created: LDC 2009-06-11
// Checsk an error is given if the path is not a line.
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PathParameter_Throw )
{
    FakeWorld world( "worldwide/tests/EmptyParis-ML" );
    Path in;
    in.mutable_location()->mutable_coordinates()->add_elem();
    FillRlyehLocation( *in.mutable_location() );
    BOOST_CHECK_THROW( MIL_PathParameter param( in ), std::exception );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PathParameter
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PathParameter )
{
    FakeWorld world( "worldwide/tests/EmptyParis-ML" );
    Path in;
    in.mutable_location()->set_type( sword::Location_Geometry_line );
    in.mutable_location()->mutable_coordinates()->add_elem()->set_latitude( 48.52f );
    in.mutable_location()->mutable_coordinates()->mutable_elem(0)->set_longitude( 2.17f );
    in.mutable_location()->mutable_coordinates()->add_elem()->set_latitude( 49.52f );
    in.mutable_location()->mutable_coordinates()->mutable_elem(1)->set_longitude( 3.17f );
    MIL_PathParameter param( in );
    in.mutable_location()->mutable_coordinates()->clear_elem();
    MissionParameter_Value out;
    BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
    BOOST_CHECK_EQUAL( true, out.has_path() );
    BOOST_CHECK_EQUAL( sword::Location_Geometry_line , out.path().location().type() );
    BOOST_CHECK_EQUAL( 2, out.path().location().coordinates().elem_size() );
    BOOST_CHECK_CLOSE( 48.52, out.path().location().coordinates().elem(0).latitude(), 1. );
    BOOST_CHECK_CLOSE( 2.17, out.path().location().coordinates().elem(0).longitude(), 1. );
    BOOST_CHECK_CLOSE( 49.52, out.path().location().coordinates().elem(1).latitude(), 1. );
    BOOST_CHECK_CLOSE( 3.17, out.path().location().coordinates().elem(1).longitude(), 1. );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PlannedWorkParameter
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PlannedWorkParameter )
{
    FakeWorld world( "worldwide/tests/EmptyParis-ML" );
    const std::string typeName( "type" );
    PlannedWork in;
    in.set_type( typeName.c_str() );
    FillRlyehLocation( *in.mutable_position() );
    in.set_type_obstacle( ObstacleType_DemolitionTargetType_reserved );
    in.set_density( 1 );
    in.mutable_combat_train()->set_id( 0 );
    in.set_activity_time( 2 );
    MockMIL_EntityManager_ABC entityManager;
    MockMIL_ObjectType_ABC objectType;
    MockObjectTypeResolver resolver;
    MOCK_EXPECT( resolver.FindType ).returns( boost::cref( objectType ) );
    MOCK_EXPECT( objectType.GetPointSize ).returns( 250. );
    MIL_PlannedWorkParameter param( in, entityManager, resolver );
    in.mutable_position()->mutable_coordinates()->Clear();
    MissionParameter_Value out;
    BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
    BOOST_CHECK_EQUAL( true, out.has_plannedwork() );
    BOOST_CHECK_EQUAL( typeName, out.plannedwork().type() );
    CompareLocationToRlyeh( out.plannedwork().position() );
    BOOST_CHECK_EQUAL( ObstacleType_DemolitionTargetType_reserved, out.plannedwork().type_obstacle() );
    BOOST_CHECK_EQUAL( 1, out.plannedwork().density() );
    BOOST_CHECK_EQUAL( 0u, out.plannedwork().combat_train().id() );
    BOOST_CHECK_EQUAL( 2, out.plannedwork().activity_time() );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PointParameter
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PointParameter )
{
    FakeWorld world( "worldwide/tests/EmptyParis-ML" );
    Point in;
    FillRlyehLocation( *in.mutable_location() );
    MIL_PointParameter param( in );
    in.mutable_location()->mutable_coordinates()->clear_elem();
    MissionParameter_Value out;
    BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
    BOOST_CHECK_EQUAL( true, out.has_point() );
    CompareLocationToRlyeh( out.point().location() );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PolygonParameter
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PolygonParameter )
{
    FakeWorld world( "worldwide/tests/EmptyParis-ML" );
    Polygon in;
    FillPolygonLocation( *in.mutable_location(), 0.f );
    MIL_PolygonParameter param( in );
    in.mutable_location()->mutable_coordinates()->clear_elem();
    MissionParameter_Value out;
    BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
    BOOST_CHECK_EQUAL( true, out.has_area() );
    CompareLocationToPolygon( out.area().location(), 0.f );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_RealParameter
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_RealParameter )
{
    float in = 42;
    MIL_RealParameter param( in );
    MissionParameter_Value out;
    BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
    BOOST_CHECK_EQUAL( true, out.has_areal() );
    BOOST_CHECK_EQUAL( 42, out.areal() );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_StringParameter
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_StringParameter )
{
    std::string in( "value" );
    MIL_StringParameter param( in );
    MissionParameter_Value out;
    BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
    BOOST_CHECK_EQUAL( true, out.has_acharstr() );
    BOOST_CHECK_EQUAL( in, out.acharstr() );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_TirIndirectParameter
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_TirIndirectParameter )
{
    Id in;
    in.set_id( 33 );
    MIL_TirIndirectParameter param( in );
    MissionParameter_Value out;
    BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
    BOOST_CHECK_EQUAL( true, out.has_indirectfire() );
    BOOST_CHECK_EQUAL( 33u, out.indirectfire().id() );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_CompositeLocationParameter
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_CompositeLocationParameter )
{
    xml::xistringstream xis( "<mission dia-behavior='whatever' dia-type='T_whatever' id='42' name='a mission'>"
                             "    <parameter dia-name='dangerDirection_' name='direction dangereuse' optional='false' type='Heading'/>"
                             "    <parameter dia-name='phaseLines_' max-occurs='unbounded' min-occurs='1' name='Limas' optional='true' type='PhaseLine'/>"
                             "    <parameter dia-name='boundaryLimit1_' name='Limit 1' optional='true' type='Limit'/>"
                             "    <parameter dia-name='boundaryLimit2_' name='Limit 2' optional='true' type='Limit'/>"
                             "    <parameter dia-name='objectif' name='Objectif' type='LocationComposite'>"
                             "        <choice>"
                             "            <parameter type='Point'/>"
                             "            <parameter type='UrbanKnowledge'/>"
                             "            <parameter type='AgentKnowledge'/>"
                             "        </choice>"
                             "    </parameter>"
                             "</mission>" );
    MIL_ParameterType_ABC::Initialize();
    MIL_PionMissionType type( 0, xis >> xml::start( "mission" ) );
    const MIL_ParameterType_ABC& paramType = type.GetParameterType( 0 ).GetType();
    BOOST_CHECK_EQUAL( paramType.GetName(), "LocationComposite" );
}

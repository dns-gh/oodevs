// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "protocol/Protocol.h"
#include "Fixture.h"
#include "MockDEC_KnowledgeResolver_ABC.h"
#include "MockMIL_EntityManager_ABC.h"
#include "MockMIL_ObjectType_ABC.h"
#include "MockArmy.h"
#include "StubMIL_Object_ABC.h"
#include "StubMIL_PopulationType.h"
#include "StubMIL_Population.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/MIL_KnowledgeGroupType.h"
#include "MockNET_Publisher_ABC.h"
#include "MockMIL_Object_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "StubTER_World.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Communications/PHY_RolePion_Communications.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationNature.h"
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

using namespace sword;

/*
// -----------------------------------------------------------------------------
// Name: TestMIL_ListParameter
$$$$ MGD need mil_entitymanager singleton
// Created: LDC 2009-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_AgentKnowledgeListParameter )
{
    MissionParameter in;
    in.mutable_value()->Add()->mutable_agentknowledge()->set_id( 0 );
    MockDEC_KnowledgeResolver_ABC resolver;
    MockMIL_Time_ABC time;
    MOCK_EXPECT( time, GetCurrentTick ).returns( 1u );
    boost::shared_ptr< DEC_Knowledge_Agent > knowledge( new DEC_Knowledge_Agent() ); // $$$$ LDC: id == 0... :(
    MOCK_EXPECT( resolver, ResolveKnowledgeAgentFromMessage ).once().returns( knowledge );

    xml::xistringstream xisParam("<parameter dia-name='pointAReconnaitre_' name='Point a reconnaitre' optional='false' type='AgentKnowledge'/>");
    xisParam >> xml::start( "parameter" );
    MIL_ParameterType_ABC::Initialize();
    MIL_OrderTypeParameter orderType( xisParam );
    xisParam >> xml::end;
    boost::shared_ptr<MIL_MissionParameter_ABC> param = MIL_MissionParameterFactory::Create( orderType, in, resolver );

    MissionParameter out;
    BOOST_CHECK_EQUAL( true, param->ToList( *out.mutable_value() ) );
    BOOST_CHECK_EQUAL( 1, out.value_size() );
    BOOST_CHECK_EQUAL( true, out.value().Get( 0 ).has_agentknowledge() );
    BOOST_CHECK_EQUAL( 0u, out.value().Get( 0 ).agentknowledge().id() ); // $$$$ LDC: = knowledge's id
    out.Clear();
}*/

namespace
{
    class KnowledgeFixture
    {
    public:
        KnowledgeFixture()
            : group( "<knowledge-groups>"
                     "    <knowledge-group name='Standard' communication-delay='01m'>"
                     "        <unit-knowledge max-lifetime='03h' max-unit-to-knowledge-distance='60000'/>"
                     "        <population-knowledge max-lifetime='2m'/>"
                     "    </knowledge-group>"
                     "</knowledge-groups>" )
        {
            mock::reset();
            MOCK_EXPECT( mockPublisher, Send );
            MIL_KnowledgeGroupType::InitializeWithTime( group, 0.5f );
            MOCK_EXPECT( army, GetID ).returns( 29u );
            MOCK_EXPECT( army, RegisterKnowledgeGroup );
            MOCK_EXPECT( army, UnregisterKnowledgeGroup );
        }
        ~KnowledgeFixture()
        {
            MIL_KnowledgeGroupType::Terminate();
        }
        MockNET_Publisher_ABC mockPublisher;
        MockArmy army;
        MockDEC_KnowledgeResolver_ABC resolver;
        MockMIL_EntityManager_ABC manager;
        xml::xistringstream group;
        MIL_KnowledgeGroup groupArmy;
    };
}
#include "MockAgent.h"

// -----------------------------------------------------------------------------
// Name: TestMIL_AgentKnowledgeParameter
// Created: LDC 2009-06-09
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( TestMIL_AgentKnowledgeParameter, KnowledgeFixture )
{
    UnitKnowledgeId in;
    in.set_id( 35 );
    xml::xistringstream xis( "<main dia-type='PionTest' file='PionTest.bms' id='12' name='stuff'/>" );
    std::map< std::string, const MIL_MissionType_ABC* > missionTypes;
    DEC_Model model( "test", xis >> xml::start( "main" ), BOOST_RESOLVE( "." ), missionTypes, false );
    StubMIL_AgentTypePion type( model );
    AlgorithmsFactories algorithmsFactories;
    MIL_AgentPion pion( type, algorithmsFactories );
    xml::xistringstream xisID( "<root id='35'/>" );
    MockAgent agent( xisID >> xml::start( "root" ) );
    MOCK_EXPECT( manager, FindAgentPion ).once().returns( &pion );
    MOCK_EXPECT( agent, BelongsTo ).once().returns( false );
    MOCK_EXPECT( agent, GetType ).once().returns( boost::cref( type ) );
    boost::shared_ptr< DEC_Knowledge_Agent > knowledge( new DEC_Knowledge_Agent( groupArmy, agent ) );
    MOCK_EXPECT( resolver, ResolveKnowledgeAgent ).once().returns( knowledge );
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
BOOST_FIXTURE_TEST_CASE( TestMIL_ObjectKnowledgeParameter, KnowledgeFixture )
{
    ObjectKnowledgeId in;
    in.set_id( 56 );
    MockMIL_ObjectType_ABC objectType;
    StubMIL_Object_ABC object( &army, objectType, 56u );
    MOCK_EXPECT( objectType, GetName ).once().returns( "type" );
    MOCK_EXPECT( manager, FindObject ).once().returns( &object );
    boost::shared_ptr< DEC_Knowledge_Object > knowledge( new DEC_Knowledge_Object( army, object ) );
    MOCK_EXPECT( resolver, ResolveKnowledgeObject ).once().returns( knowledge );
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
BOOST_FIXTURE_TEST_CASE( TestMIL_PopulationKnowledgeParameter, KnowledgeFixture )
{
    CrowdKnowledgeId in;
    in.set_id( 0 );
    xml::xistringstream xis( "<main dia-type='PionTest' file='PionTest.bms'/>" );
    std::map< std::string, const MIL_MissionType_ABC* > missionTypes;
    DEC_Model model( "test", xis >> xml::start( "main" ), BOOST_RESOLVE( "." ), missionTypes, false );
    StubMIL_PopulationType type( model );
    StubMIL_Population population( type, army );
    MOCK_EXPECT( manager, FindPopulation ).once().returns( &population );
    MIL_KnowledgeGroup groupArmy( *MIL_KnowledgeGroupType::FindType( "Standard" ), 30, army );
    DEC_Knowledge_Population knowledge( groupArmy, population );
    MOCK_EXPECT( resolver, ResolveKnowledgePopulation ).once().returns( &knowledge );

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
    UnitId in;
    in.set_id( 12 );
    MockMIL_EntityManager_ABC entityManager;
    MIL_EffectManager effectManager;
    FixturePion fixture( effectManager );
    fixture.pPion_->RegisterRole( *new DEC_RolePion_Decision( *fixture.pPion_, 100, 100 ) );
    MOCK_EXPECT( entityManager, FindAgentPion ).once().returns( fixture.pPion_.get() );
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
    AutomatId in;
    in.set_id( 0 );
    MockMIL_EntityManager_ABC entityManager;
    FixtureAutomate fixture;
    //fixture.pAutomat_->RegisterRole( *new DEC_AutomateDecision( *fixture.pAutomat_ ) );
    MOCK_EXPECT( entityManager, FindAutomate ).once().returns( fixture.pAutomat_.get() );
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
    xml::xistringstream xisNat( "<natures>"
                                "   <nature type='Solid' id='1'/>"
                                "</natures>" );
    PHY_DotationNature::Initialize( xisNat );
    xml::xistringstream xis( "<resources>"
                             "   <resource name='Food' id='42' category='ration' nature='Solid' id-nature='1' package-mass='0.015' package-size='10' package-volume='0.01'/>"
                             "</resources>" );
    MockMIL_EntityManager_ABC entityManager;
    PHY_DotationType::Initialize( xis );
    ResourceType in;
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
//    xml::xifstream xisVol( "../../data/data/models/bmstest/physical/Volumes.xml" );
//    PHY_Volume::Initialize( xisVol );
//    xml::xifstream xisProt( "../../data/data/models/bmstest/physical/Protections.xml" );
//    PHY_Protection::Initialize ( xisProt );
//    xml::xifstream xis( "../../data/data/models/bmstest/physical/Composantes.xml" );
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
    WorldInitialize( "Paris" );
    Location in;
    FillRlyehLocation( in );
    MIL_LocationParameter param( in );
    in.mutable_coordinates()->clear_elem();
    MissionParameter_Value out;
    BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
    BOOST_CHECK_EQUAL( true, out.has_location() );
    CompareLocationToRlyeh( out.location() );
    TER_World::DestroyWorld();
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
    WorldInitialize( "Paris" );
    Path in;
    in.mutable_location()->mutable_coordinates()->add_elem();
    FillRlyehLocation( *in.mutable_location() );
    BOOST_CHECK_THROW( MIL_PathParameter param( in ), std::runtime_error );
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PathParameter
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PathParameter )
{
    WorldInitialize( "Paris" );
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
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PlannedWorkParameter
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PlannedWorkParameter )
{
    WorldInitialize( "Paris" );
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
    MOCK_EXPECT( entityManager, FindObjectType ).once().returns( boost::cref( objectType ) );
    MIL_PlannedWorkParameter param( in, entityManager );
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
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PointParameter
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PointParameter )
{
    WorldInitialize( "Paris" );
    Point in;
    FillRlyehLocation( *in.mutable_location() );
    MIL_PointParameter param( in );
    in.mutable_location()->mutable_coordinates()->clear_elem();
    MissionParameter_Value out;
    BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
    BOOST_CHECK_EQUAL( true, out.has_point() );
    CompareLocationToRlyeh( out.point().location() );
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PolygonParameter
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PolygonParameter )
{
    WorldInitialize( "Paris" );
    Polygon in;
    FillPolygonLocation( *in.mutable_location(), 0.f );
    MIL_PolygonParameter param( in );
    in.mutable_location()->mutable_coordinates()->clear_elem();
    MissionParameter_Value out;
    BOOST_CHECK_EQUAL( true, param.ToElement( out ) );
    BOOST_CHECK_EQUAL( true, out.has_area() );
    CompareLocationToPolygon( out.area().location(), 0.f );
    TER_World::DestroyWorld();
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
    FireId in;
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

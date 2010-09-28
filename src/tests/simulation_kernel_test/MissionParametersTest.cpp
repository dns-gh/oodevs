// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "protocol/protocol.h"
#include "Fixture.h"
#include "MockDEC_KnowledgeResolver_ABC.h"
#include "MockMIL_EntityManager_ABC.h"
#include "MockMIL_ObjectType_ABC.h"
#include "MockMIL_Time_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationNature.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Orders/MIL_AgentKnowledgeListParameter.h"
#include "Entities/Orders/MIL_AgentKnowledgeParameter.h"
#include "Entities/Orders/MIL_AgentListParameter.h"
#include "Entities/Orders/MIL_AgentParameter.h"
#include "Entities/Orders/MIL_AutomatParameter.h"
#include "Entities/Orders/MIL_AutomatListParameter.h"
#include "Entities/Orders/MIL_AtlasNatureParameter.h"
#include "Entities/Orders/MIL_BoolParameter.h"
#include "Entities/Orders/MIL_DateTimeParameter.h"
#include "Entities/Orders/MIL_DirectionParameter.h"
#include "Entities/Orders/MIL_DotationTypeParameter.h"
#include "Entities/Orders/MIL_EnumerationParameter.h"
//#include "Entities/Orders/MIL_EquipmentTypeParameter.h"
#include "Entities/Orders/MIL_LocationParameter.h"
#include "Entities/Orders/MIL_LocationListParameter.h"
//#include "Entities/Orders/MIL_LogMaintenancePrioritiesParameter.h"
#include "Entities/Orders/MIL_LogMedicalPrioritiesParameter.h"
//#include "Entities/Orders/MIL_MissionObjectiveListParameter.h"
#include "Entities/Orders/MIL_ObjectKnowledgeParameter.h"
#include "Entities/Orders/MIL_ObjectKnowledgeListParameter.h"
#include "Entities/Orders/MIL_PathParameter.h"
#include "Entities/Orders/MIL_PathListParameter.h"
#include "Entities/Orders/MIL_PlannedWorkParameter.h"
#include "Entities/Orders/MIL_PlannedWorkListParameter.h"
#include "Entities/Orders/MIL_PointParameter.h"
#include "Entities/Orders/MIL_PointListParameter.h"
#include "Entities/Orders/MIL_PolygonParameter.h"
#include "Entities/Orders/MIL_PolygonListParameter.h"
#include "Entities/Orders/MIL_PopulationKnowledgeParameter.h"
#include "Entities/Orders/MIL_RealParameter.h"
#include "Entities/Orders/MIL_StringParameter.h"
#include "Entities/Orders/MIL_TirIndirectParameter.h"
#include "Entities/Orders/MIL_PionMissionType.h"
#include "UrbanType.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Network/NET_ASN_Tools.h"
#include "simulation_orders/MIL_ParameterType_ABC.h"
#include "simulation_terrain/TER_World.h"

using namespace Common;

// -----------------------------------------------------------------------------
// Name: TestMIL_AgentKnowledgeListParameter_ToASN
// Created: LDC 2009-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_AgentKnowledgeListParameter_ToASN )
{
    UnitKnowledgeIdList asnIn;
    asnIn.add_elem()->set_id( 0 );
    MockDEC_KnowledgeResolver_ABC resolver;
    MockMIL_Time_ABC time;
    MOCK_EXPECT( time, GetCurrentTick ).returns( 1u );
    boost::shared_ptr< DEC_Knowledge_Agent > knowledge( new DEC_Knowledge_Agent() ); // $$$$ LDC: id == 0... :(
    MOCK_EXPECT( resolver, ResolveKnowledgeAgentFromMessage ).once().returns( knowledge );
    MIL_AgentKnowledgeListParameter param( asnIn, resolver );
    asnIn.Clear();
    UnitKnowledgeIdList asnOut;
    BOOST_CHECK_EQUAL( true, param.ToAgentKnowledgeList( asnOut ) );
    BOOST_CHECK_EQUAL( 1, asnOut.elem_size() );
    BOOST_CHECK_EQUAL( 0u, asnOut.elem( 0 ).id() ); // $$$$ LDC: = knowledge's id
    asnOut.Clear();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_AgentKnowledgeParameter_ToASN
// Created: LDC 2009-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_AgentKnowledgeParameter_ToASN )
{
    UnitKnowledgeId asnIn;
    asnIn.set_id( 0 );
    MockDEC_KnowledgeResolver_ABC resolver;
    MockMIL_Time_ABC time;
    MOCK_EXPECT( time, GetCurrentTick ).returns( 1u );
    boost::shared_ptr< DEC_Knowledge_Agent > knowledge( new DEC_Knowledge_Agent() ); // $$$$ LDC: id == 0... :(
    MOCK_EXPECT( resolver, ResolveKnowledgeAgentFromMessage ).once().returns( knowledge );
    MIL_AgentKnowledgeParameter param( asnIn, resolver );
    UnitKnowledgeId asnOut;
    BOOST_CHECK_EQUAL( true, param.ToAgentKnowledge( asnOut ) );
    BOOST_CHECK_EQUAL( 0u, asnOut.id() ); // $$$$ LDC: = knowledge's id
}

// -----------------------------------------------------------------------------
// Name: TestMIL_AgentListParameter_ToASN
// Created: LDC 2009-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_AgentListParameter_ToASN )
{
    UnitIdList asnIn;
    asnIn.add_elem()->set_id( 12 );
    MockMIL_EntityManager_ABC entityManager;
    MIL_EffectManager effectManager;
    FixturePion fixture( effectManager );
    StubDEC_Database database;
    fixture.pPion_->RegisterRole( *new DEC_RolePion_Decision( *fixture.pPion_, database, 100, 100 ) );
    MOCK_EXPECT( entityManager, FindAgentPion ).once().returns( fixture.pPion_.get() );
    MIL_AgentListParameter param( asnIn, entityManager );
    asnIn.Clear();
    UnitIdList asnOut;
    BOOST_CHECK_EQUAL( true, param.ToAgentList( asnOut ) );
    BOOST_CHECK_EQUAL( 1, asnOut.elem_size() );
    BOOST_CHECK_EQUAL( 12u, asnOut.elem(0).id() );
    asnOut.Clear();
}


// -----------------------------------------------------------------------------
// Name: TestMIL_AgentParameter_ToASN
// Created: LDC 2009-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_AgentParameter_ToASN )
{
    UnitId asnIn;
    asnIn.set_id( 12 );
    MockMIL_EntityManager_ABC entityManager;
    MIL_EffectManager effectManager;
    FixturePion fixture( effectManager );
    StubDEC_Database database;
    fixture.pPion_->RegisterRole( *new DEC_RolePion_Decision( *fixture.pPion_, database, 100, 100 ) );
    MOCK_EXPECT( entityManager, FindAgentPion ).once().returns( fixture.pPion_.get() );
    MIL_AgentParameter param( asnIn, entityManager );
    UnitId asnOut;
    BOOST_CHECK_EQUAL( true, param.ToAgent( asnOut ) );
    BOOST_CHECK_EQUAL( 12u, asnOut.id() );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_AutomatParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_AutomatParameter_ToASN )
{
    AutomatId asnIn;
    asnIn.set_id( 0 );
    MockMIL_EntityManager_ABC entityManager;
    FixtureAutomate fixture;
    //fixture.pAutomat_->RegisterRole( *new DEC_AutomateDecision( *fixture.pAutomat_ ) );
    MOCK_EXPECT( entityManager, FindAutomate ).once().returns( fixture.pAutomat_.get() );
    MIL_AutomatParameter param( asnIn, entityManager );
    AutomatId asnOut;
    BOOST_CHECK_EQUAL( true, param.ToAutomat( asnOut ) );
    BOOST_CHECK_EQUAL( 0u, asnOut.id() );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_AutomatListParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_AutomatListParameter_ToASN )
{
    AutomatIdList asnIn;
    asnIn.add_elem()->set_id( 0 );
    MockMIL_EntityManager_ABC entityManager;
    FixtureAutomate fixture;
    //fixture.pAutomat_->RegisterRole( *new DEC_AutomateDecision( *fixture.pAutomat_ ) );
    MOCK_EXPECT( entityManager, FindAutomate ).once().returns( fixture.pAutomat_.get() );
    MIL_AutomatListParameter param( asnIn, entityManager );
    asnIn.Clear();
    AutomatIdList asnOut;
    BOOST_CHECK_EQUAL( true, param.ToAutomatList( asnOut ) );
    BOOST_CHECK_EQUAL( 1, asnOut.elem_size() );
    BOOST_CHECK_EQUAL( 0u, asnOut.elem(0).id() );
    asnOut.Clear();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_AtlasNatureParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_AtlasNatureParameter_ToASN )
{
    MsgAtlasNature asnIn;
    asnIn.set_nature( MsgAtlasNature::blinde | MsgAtlasNature::log | MsgAtlasNature::asa );

    MIL_AtlasNatureParameter param( asnIn );
    MsgAtlasNature asnOut;
    BOOST_CHECK_EQUAL( true, param.ToNatureAtlas( asnOut ) );
    BOOST_CHECK_EQUAL( MsgAtlasNature::blinde, asnOut.nature() & MsgAtlasNature::blinde );
    BOOST_CHECK_EQUAL( MsgAtlasNature::log,    asnOut.nature() & MsgAtlasNature::log );
    BOOST_CHECK_EQUAL( MsgAtlasNature::asa,    asnOut.nature() & MsgAtlasNature::asa );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_BoolParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_BoolParameter_ToASN )
{
    {
        MIL_BoolParameter param( true );
        bool asnOut;
        BOOST_CHECK_EQUAL( true, param.ToBool( asnOut ) );
        BOOST_CHECK_EQUAL( true, asnOut );
    }
    {
        MIL_BoolParameter param( false );
        bool asnOut;
        BOOST_CHECK_EQUAL( true, param.ToBool( asnOut ) );
        BOOST_CHECK_EQUAL( false, asnOut );
    }
}

// -----------------------------------------------------------------------------
// Name: TestMIL_DateTimeParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_DateTimeParameter_ToASN )
{
    MsgDateTime asnIn;
    NET_ASN_Tools::WriteGDH( 42, asnIn );
    MIL_DateTimeParameter param( asnIn );
    MsgDateTime asnOut;
    BOOST_CHECK_EQUAL( true, param.ToGDH( asnOut ) );
    BOOST_CHECK_EQUAL( asnOut.data(), "19700101T000042" );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_DirectionParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_DirectionParameter_ToASN )
{
    MsgHeading asnIn;
    asnIn.set_heading( 42 );
    MIL_DirectionParameter param( asnIn );
    MsgHeading asnOut;
    BOOST_CHECK_EQUAL( true, param.ToDirection( asnOut ) );
    BOOST_CHECK_EQUAL( 42, asnOut.heading() );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_DotationTypeParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_DotationTypeParameter_ToASN )
{
    xml::xistringstream xisUrbanType(
        "<urban>"
        "   <urban-block-types>"
        "       <material-composition-types/>"
        "       <facade-types/>"
        "       <roof-shape-types/>"
        "   </urban-block-types>"
        "</urban>" );
    UrbanType::Initialize( xisUrbanType );
    xml::xistringstream xisNat(
        "<natures>"
        "   <nature type='Solid'/>"
        "</natures>" );
    PHY_DotationNature::Initialize( xisNat );
    xml::xistringstream xis(
        "<dotations>"
        "   <dotation category='Food' id='42' name='ration' nature='Solid' package-mass='0.015' package-size='10' package-volume='0.01'/>"
        "</dotations>" );
    MockMIL_EntityManager_ABC entityManager;
    PHY_DotationType::Initialize( xis );
    ResourceType asnIn;
    asnIn.set_id( 42 );
    MIL_DotationTypeParameter param( asnIn );
    ResourceType asnOut;
    BOOST_CHECK_EQUAL( true, param.ToDotationType( asnOut ) );
    BOOST_CHECK_EQUAL( 42u, asnOut.id() );
    UrbanType::Terminate();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_EnumerationParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_EnumerationParameter_ToASN )
{
    int asnIn = 42;
    MIL_EnumerationParameter param( asnIn );
    int asnOut;
    BOOST_CHECK_EQUAL( true, param.ToId( asnOut ) );
    BOOST_CHECK_EQUAL( 42, asnOut );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_EquipmentTypeParameter_ToASN
// Created: LDC 2009-06-10
// $$$$ LDC: Impossible to test without getting all the singletons:
// call to MIL_AgentServer::GetWorkspace().GetTimeStepDuration()...
// -----------------------------------------------------------------------------
//BOOST_AUTO_TEST_CASE( TestMIL_EquipmentTypeParameter_ToASN )
//{
//    PHY_ComposanteState::Initialize();
//    xml::xifstream xisVol( "../../data/data/models/bmstest/physical/Volumes.xml" );
//    PHY_Volume::Initialize( xisVol );
//    xml::xifstream xisProt( "../../data/data/models/bmstest/physical/Protections.xml" );
//    PHY_Protection::Initialize ( xisProt );
//    xml::xifstream xis( "../../data/data/models/bmstest/physical/Composantes.xml" );
//    MockMIL_Time_ABC time;
//    PHY_ComposanteTypePion::Initialize( time, xis );
//    EquipmentType asnIn = 22;
//    MIL_EquipmentTypeParameter param( asnIn );
//    EquipmentType asnOut;
//    BOOST_CHECK_EQUAL( true, param.ToEquipmentType( asnOut ) );
//    BOOST_CHECK_EQUAL( 22, asnOut );
//}

namespace
{
    void FillRlyehLocation( MsgLocation& asnIn )
    {
        asnIn.set_type( Common::MsgLocation_Geometry_point );
        asnIn.mutable_coordinates()->add_elem()->set_latitude( 48.52f );
        asnIn.mutable_coordinates()->mutable_elem(0)->set_longitude( 2.17f );
    }

    void CompareLocationToRlyeh( MsgLocation& asnOut )
    {
        BOOST_CHECK_EQUAL( Common::MsgLocation_Geometry_point, asnOut.type() );
        BOOST_CHECK_EQUAL( 1, asnOut.coordinates().elem_size() );
        BOOST_CHECK_CLOSE( 48.52, asnOut.coordinates().elem(0).latitude(), 1. );
        BOOST_CHECK_CLOSE( 2.17, asnOut.coordinates().elem(0).longitude(), 1. );
        asnOut.mutable_coordinates()->Clear();
    }

    void FillPolygonLocation( MsgLocation& asnIn, float offset )
    {
        asnIn.set_type( Common::MsgLocation_Geometry_polygon );
        asnIn.mutable_coordinates()->add_elem()->set_latitude( 50.f + offset );
        asnIn.mutable_coordinates()->mutable_elem(0)->set_longitude( 2.17f + offset );
        asnIn.mutable_coordinates()->add_elem()->set_latitude( 49.52f + offset );
        asnIn.mutable_coordinates()->mutable_elem(1)->set_longitude( 3.f + offset );
        asnIn.mutable_coordinates()->add_elem()->set_latitude( 48.52f + offset );
        asnIn.mutable_coordinates()->mutable_elem(2)->set_longitude( 3.17f + offset );
    }

    void CompareLocationToPolygon( MsgLocation& asnOut, float offset )
    {
        BOOST_CHECK_EQUAL( Common::MsgLocation_Geometry_polygon, asnOut.type() );
        BOOST_CHECK_EQUAL( 4, asnOut.coordinates().elem_size() );
        BOOST_CHECK_CLOSE( 50. + offset, asnOut.coordinates().elem(0).latitude(), 1. );
        BOOST_CHECK_CLOSE( 2.17 + offset, asnOut.coordinates().elem(0).longitude(), 1. );
        BOOST_CHECK_CLOSE( 49.52 + offset, asnOut.coordinates().elem(1).latitude(), 1. );
        BOOST_CHECK_CLOSE( 3. + offset, asnOut.coordinates().elem(1).longitude(), 1. );
        BOOST_CHECK_CLOSE( 48.52 + offset, asnOut.coordinates().elem(2).latitude(), 1. );
        BOOST_CHECK_CLOSE( 3.17 + offset, asnOut.coordinates().elem(2).longitude(), 1. );
        BOOST_CHECK_EQUAL( asnOut.coordinates().elem(0).latitude(), asnOut.coordinates().elem(3).latitude() );
        BOOST_CHECK_EQUAL( asnOut.coordinates().elem(0).longitude(), asnOut.coordinates().elem(3).longitude() );
        asnOut.mutable_coordinates()->clear_elem();
    }
}

// -----------------------------------------------------------------------------
// Name: TestMIL_LocationParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_LocationParameter_ToASN )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );
    MsgLocation asnIn;
    FillRlyehLocation( asnIn );
    MIL_LocationParameter param( asnIn );
    asnIn.mutable_coordinates()->clear_elem();
    MsgLocation asnOut;
    BOOST_CHECK_EQUAL( true, param.ToLocation( asnOut ) );
    CompareLocationToRlyeh( asnOut );
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_LocationListParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_LocationListParameter_ToASN )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );
    MsgLocationList asnIn;
    FillRlyehLocation( *asnIn.add_elem() );
    MIL_LocationListParameter param( asnIn );
    asnIn.mutable_elem(0)->mutable_coordinates()->Clear();
    //asnIn.Clear();
    MsgLocationList asnOut;
    //asnOut.add_elem();
    BOOST_CHECK_EQUAL( true, param.ToLocationList( asnOut ) );
    BOOST_CHECK_EQUAL( 1, asnOut.elem_size() );
    CompareLocationToRlyeh( *asnOut.mutable_elem(0) );
    asnOut.Clear();
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_LogMaintenancePrioritiesParameter_ToASN
// Created: LDC 2009-06-11
// $$$$ LDC: Impossible to test without getting all the singletons:
// Same as MIL_EquipmentTypeParameter
// -----------------------------------------------------------------------------
//BOOST_AUTO_TEST_CASE( TestMIL_LogMaintenancePrioritiesParameter_ToASN )
//{
//    LogMaintenancePriorities asnIn;
//    MIL_LogMaintenancePrioritiesParameter param( asnIn );
//    LogMaintenancePriorities asnOut;
//    BOOST_CHECK_EQUAL( true, param.ToMaintenancePriorities( asnOut ) );
//}

// -----------------------------------------------------------------------------
// Name: TestMIL_LogMaintenancePrioritiesParameter_ToASN
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_LogMaintenancePrioritiesParameter_ToASN )
{
    PHY_HumanWound::Initialize();
    MsgLogMedicalPriorities asnIn;
    asnIn.add_elem( Common::blesse_urgence_2 );
    MIL_LogMedicalPrioritiesParameter param( asnIn );
    asnIn.clear_elem();
    MsgLogMedicalPriorities asnOut;
    BOOST_CHECK_EQUAL( true, param.ToMedicalPriorities( asnOut ) );
    BOOST_CHECK_EQUAL( 1, asnOut.elem_size() );
    BOOST_CHECK_EQUAL( Common::blesse_urgence_2, asnOut.elem(0) );
    asnOut.clear_elem();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_MissionObjectiveListParameter_ToASN
// Created: LDC 2009-06-11
// $$$$ LDC: Impossible to test without getting all the singletons:
// Calls MIL_AgentServer::GetWorkspace().RealTimeToTick( value );
// -----------------------------------------------------------------------------
//BOOST_AUTO_TEST_CASE( TestMIL_MissionObjectiveListParameter_ToASN )
//{
//    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );
//    MissionObjectiveList asnIn;
//    asnIn.set_n( 1 );
//    asnIn.elem = new MissionObjective[1];
//    NET_ASN_Tools::WriteGDH( 42, asnIn.elem[0].horaire );
//    FillRlyehLocation( asnIn.elem[0].localisation );
//
//    MIL_MissionObjectiveListParameter param( asnIn );
//
//    delete[] asnIn.elem[0].localisation.coordinates.elem;
//    delete asnIn.elem;
//
//    MissionObjectiveList asnOut;
//    BOOST_CHECK_EQUAL( true, param.ToObjectiveList( asnOut ) );
//    BOOST_CHECK_EQUAL( 1, asnOut.n );
//    CompareDateTo42( asnOut.elem[0].horaire );
//    CompareLocationToRlyeh( asnOut.elem[0].localisation );
//    delete[] asnOut.elem;
//}

// -----------------------------------------------------------------------------
// Name: TestMIL_ObjectKnowledgeParameter_ToASN
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_ObjectKnowledgeParameter_ToASN )
{
    ObjectKnowledgeId asnIn;
    asnIn.set_id( 0 );
    MockDEC_KnowledgeResolver_ABC resolver;
    MockMIL_Time_ABC time;
    MOCK_EXPECT( time, GetCurrentTick ).returns( 1u );
    boost::shared_ptr< DEC_Knowledge_Object > knowledge( new DEC_Knowledge_Object() ); // $$$$ LDC: id == 0... :(
    MOCK_EXPECT( resolver, ResolveKnowledgeObjectFromMessage ).once().returns( knowledge );

    MIL_ObjectKnowledgeParameter param( asnIn, resolver );
    ObjectKnowledgeId asnOut;
    BOOST_CHECK_EQUAL( true, param.ToObjectKnowledge( asnOut ) );
    BOOST_CHECK_EQUAL( 0u, asnOut.id() ); // $$$$ LDC: = knowledge's id
}

// -----------------------------------------------------------------------------
// Name: TestMIL_ObjectKnowledgeListParameter_ToASN
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_ObjectKnowledgeListParameter_ToASN )
{
    ObjectKnowledgeIdList asnIn;
    asnIn.add_elem()->set_id( 0 );
    MockDEC_KnowledgeResolver_ABC resolver;
    MockMIL_Time_ABC time;
    MOCK_EXPECT( time, GetCurrentTick ).returns( 1u );
    boost::shared_ptr< DEC_Knowledge_Object > knowledge( new DEC_Knowledge_Object() ); // $$$$ LDC: id == 0... :(
    MOCK_EXPECT( resolver, ResolveKnowledgeObjectFromMessage ).once().returns( knowledge );

    MIL_ObjectKnowledgeListParameter param( asnIn, resolver );
    asnIn.clear_elem();
    ObjectKnowledgeIdList asnOut;
    BOOST_CHECK_EQUAL( true, param.ToObjectKnowledgeList( asnOut ) );
    BOOST_CHECK_EQUAL( 1, asnOut.elem_size() );
    BOOST_CHECK_EQUAL( 0u, asnOut.elem(0).id() ); // $$$$ LDC: = knowledge's id
    asnOut.clear_elem();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PathParameter_Throw
// Created: LDC 2009-06-11
// Checsk an error is given if the path is not a line.
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PathParameter_Throw )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );
    MsgPath asnIn;
    asnIn.mutable_location()->mutable_coordinates()->add_elem();
    FillRlyehLocation( *asnIn.mutable_location() );
    BOOST_CHECK_THROW( MIL_PathParameter param( asnIn ), std::runtime_error );
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PathParameter_ToASN
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PathParameter_ToASN )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );
    MsgPath asnIn;
    asnIn.mutable_location()->set_type( Common::MsgLocation_Geometry_line );
    asnIn.mutable_location()->mutable_coordinates()->add_elem()->set_latitude( 48.52f );
    asnIn.mutable_location()->mutable_coordinates()->mutable_elem(0)->set_longitude( 2.17f );
    asnIn.mutable_location()->mutable_coordinates()->add_elem()->set_latitude( 49.52f );
    asnIn.mutable_location()->mutable_coordinates()->mutable_elem(1)->set_longitude( 3.17f );
    MIL_PathParameter param( asnIn );
    asnIn.mutable_location()->mutable_coordinates()->clear_elem();
    MsgPath asnOut;
    BOOST_CHECK_EQUAL( true, param.ToPath( asnOut ) );
    BOOST_CHECK_EQUAL( Common::MsgLocation_Geometry_line , asnOut.location().type() );
    BOOST_CHECK_EQUAL( 2, asnOut.location().coordinates().elem_size() );
    BOOST_CHECK_CLOSE( 48.52, asnOut.location().coordinates().elem(0).latitude(), 1. );
    BOOST_CHECK_CLOSE( 2.17, asnOut.location().coordinates().elem(0).longitude(), 1. );
    BOOST_CHECK_CLOSE( 49.52, asnOut.location().coordinates().elem(1).latitude(), 1. );
    BOOST_CHECK_CLOSE( 3.17, asnOut.location().coordinates().elem(1).longitude(), 1. );
    asnOut.mutable_location()->mutable_coordinates()->clear_elem();
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PathListParameter_ToASN
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PathListParameter_ToASN )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );
    MsgPathList asnIn;

    asnIn.add_elem()->mutable_location()->set_type( Common::MsgLocation_Geometry_line );
    asnIn.mutable_elem(0)->mutable_location()->mutable_coordinates()->add_elem()->set_latitude( 48.52f );
    asnIn.mutable_elem(0)->mutable_location()->mutable_coordinates()->mutable_elem(0)->set_longitude( 2.17f );
    asnIn.mutable_elem(0)->mutable_location()->mutable_coordinates()->add_elem()->set_latitude( 49.52f );
    asnIn.mutable_elem(0)->mutable_location()->mutable_coordinates()->mutable_elem(1)->set_longitude( 3.17f );
    MIL_PathListParameter param( asnIn );
    asnIn.mutable_elem(0)->mutable_location()->mutable_coordinates()->Clear();
    MsgPathList asnOut;
    BOOST_CHECK_EQUAL( true, param.ToPathList( asnOut ) );
    BOOST_CHECK_EQUAL( 1, asnOut.elem_size() );
    BOOST_CHECK_EQUAL( Common::MsgLocation_Geometry_line, asnOut.elem(0).location().type() );
    BOOST_CHECK_EQUAL( 2, asnOut.elem(0).location().coordinates().elem_size() );
    BOOST_CHECK_CLOSE( 48.52, asnOut.elem(0).location().coordinates().elem(0).latitude(), 1. );
    BOOST_CHECK_CLOSE( 2.17, asnOut.elem(0).location().coordinates().elem(0).longitude(), 1. );
    BOOST_CHECK_CLOSE( 49.52, asnOut.elem(0).location().coordinates().elem(1).latitude(), 1. );
    BOOST_CHECK_CLOSE( 3.17, asnOut.elem(0).location().coordinates().elem(1).longitude(), 1. );
    asnOut.mutable_elem(0)->mutable_location()->mutable_coordinates()->Clear();
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PlannedWorkParameter_ToASN
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PlannedWorkParameter_ToASN )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );

    const std::string typeName( "type" );
    MsgPlannedWork asnIn;
    asnIn.set_type( typeName.c_str() );
    FillRlyehLocation( *asnIn.mutable_position() );
    asnIn.set_type_obstacle( ObstacleType_DemolitionTargetType_reserved );
    asnIn.set_densite( 1 );
    asnIn.mutable_tc2()->set_id( 0 );
    asnIn.set_activity_time( 2 );
    MockMIL_EntityManager_ABC entityManager;
    MockMIL_ObjectType_ABC objectType;
    MOCK_EXPECT( entityManager, FindObjectType ).once().returns( boost::cref( objectType ) );
    // $$$$ _RC_ SBO 2010-04-27: was not verify'ed
//    FixtureAutomate fixture;
//    MOCK_EXPECT( entityManager, FindAutomate ).once().returns( fixture.pAutomat_.get() );
    MIL_PlannedWorkParameter param( asnIn, entityManager );
    asnIn.mutable_position()->mutable_coordinates()->Clear();
    MsgPlannedWork asnOut;
    BOOST_CHECK_EQUAL( true, param.ToGenObject( asnOut ) );
    BOOST_CHECK_EQUAL( typeName, asnOut.type() );
    CompareLocationToRlyeh( *asnOut.mutable_position() );
    BOOST_CHECK_EQUAL( ObstacleType_DemolitionTargetType_reserved, asnOut.type_obstacle() );
    BOOST_CHECK_EQUAL( 1, asnOut.densite() );
    BOOST_CHECK_EQUAL( 0u, asnOut.tc2().id() );
    BOOST_CHECK_EQUAL( 2, asnOut.activity_time() );
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PlannedWorkListParameter_ToASN
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PlannedWorkListParameter_ToASN )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );

    try
    {
        const std::string typeName( "type" );
        MsgPlannedWorkList asnIn;
        asnIn.add_elem()->set_type( typeName.c_str() );
        FillRlyehLocation( *asnIn.mutable_elem(0)->mutable_position());
        asnIn.mutable_elem(0)->set_type_obstacle( ObstacleType_DemolitionTargetType_reserved );
        asnIn.mutable_elem(0)->set_densite( 1 );
        asnIn.mutable_elem(0)->mutable_tc2()->set_id( 0 );
        asnIn.mutable_elem(0)->set_activity_time( 2 );
        MockMIL_EntityManager_ABC entityManager;
        MockMIL_ObjectType_ABC objectType;
        MOCK_EXPECT( entityManager, FindObjectType ).once().returns( boost::cref( objectType ) );
        // $$$$ _RC_ SBO 2010-04-27: was not verify'ed
    //    FixtureAutomate fixture;
    //    MOCK_EXPECT( entityManager, FindAutomate ).once().returns( fixture.pAutomat_.get() );
        MIL_PlannedWorkListParameter param( asnIn, entityManager );
        asnIn.mutable_elem(0)->mutable_position()->mutable_coordinates()->Clear();
        asnIn.clear_elem();
        MsgPlannedWorkList asnOut;
        BOOST_CHECK_EQUAL( true, param.ToGenObjectList( asnOut ) );
        BOOST_CHECK_EQUAL( 1, asnOut.elem_size() );
        BOOST_CHECK_EQUAL( typeName, asnOut.elem(0).type() );
        CompareLocationToRlyeh( *asnOut.mutable_elem(0)->mutable_position() );
        BOOST_CHECK_EQUAL( ObstacleType_DemolitionTargetType_reserved, asnOut.elem(0).type_obstacle() );
        BOOST_CHECK_EQUAL( 1, asnOut.elem(0).densite() );
        BOOST_CHECK_EQUAL( 0u, asnOut.elem(0).tc2().id() );
        BOOST_CHECK_EQUAL( 2, asnOut.elem(0).activity_time() );
    }
    catch( ... )
    {
        TER_World::DestroyWorld();
        throw;
    }
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PointParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PointParameter_ToASN )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );
    MsgPoint asnIn;
    FillRlyehLocation( *asnIn.mutable_location() );
    MIL_PointParameter param( asnIn );
    asnIn.mutable_location()->mutable_coordinates()->clear_elem();
    MsgPoint asnOut;
    BOOST_CHECK_EQUAL( true, param.ToPoint( asnOut ) );
    CompareLocationToRlyeh( *asnOut.mutable_location() );
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PointListParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PointListParameter_ToASN )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );
    MsgPointList asnIn;
    FillRlyehLocation( *asnIn.add_elem()->mutable_location() );
    //FillRlyehLocation( *asnIn.mutable_elem( 0 )->mutable_location() );
    MIL_PointListParameter param( asnIn );
    asnIn.mutable_elem(0)->mutable_location()->mutable_coordinates()->Clear();
    asnIn.clear_elem();
    MsgPointList asnOut;

    BOOST_CHECK_EQUAL( true, param.ToPointList( asnOut ) );
    BOOST_CHECK_EQUAL( 1, asnOut.elem_size() );
    CompareLocationToRlyeh( *asnOut.mutable_elem(0)->mutable_location() );
    asnOut.clear_elem();
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PolygonParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PolygonParameter_ToASN )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );
    MsgPolygon asnIn;
    FillPolygonLocation( *asnIn.mutable_location(), 0.f );
    MIL_PolygonParameter param( asnIn );
    asnIn.mutable_location()->mutable_coordinates()->clear_elem();
    MsgPolygon asnOut;
    BOOST_CHECK_EQUAL( true, param.ToPolygon( asnOut ) );
    CompareLocationToPolygon( *asnOut.mutable_location(), 0.f );
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PolygonListParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PolygonListParameter_ToASN )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );
    MsgPolygonList asnIn;
    FillPolygonLocation( *asnIn.add_elem()->mutable_location(), 0.f );
    FillPolygonLocation( *asnIn.add_elem()->mutable_location(), 0.01f );
    MIL_PolygonListParameter param( asnIn );
    asnIn.mutable_elem(0)->mutable_location()->mutable_coordinates()->clear_elem();
    asnIn.mutable_elem(1)->mutable_location()->mutable_coordinates()->clear_elem();
    asnIn.clear_elem();
    MsgPolygonList asnOut;
    BOOST_CHECK_EQUAL( true, param.ToPolygonList( asnOut ) );
    BOOST_CHECK_EQUAL( 2, asnOut.elem_size() );
    CompareLocationToPolygon( *asnOut.mutable_elem(0)->mutable_location(), 0.f );
    CompareLocationToPolygon( *asnOut.mutable_elem(1)->mutable_location(), 0.01f );
    asnOut.clear_elem();
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PopulationKnowledgeParameter_ToASN
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PopulationKnowledgeParameter_ToASN )
{
    CrowdKnowledgeId asnIn;
    asnIn.set_id( 0 );
    MockDEC_KnowledgeResolver_ABC resolver;
    MockMIL_Time_ABC time;
    MOCK_EXPECT( time, GetCurrentTick ).returns( 1u );
    DEC_Knowledge_Population knowledge; // $$$$ LDC: id == 0... :(
    MOCK_EXPECT( resolver, ResolveKnowledgePopulationFromMessage ).once().returns( &knowledge );
    MIL_PopulationKnowledgeParameter param( asnIn, resolver );
    CrowdKnowledgeId asnOut;
    BOOST_CHECK_EQUAL( true, param.ToPopulationKnowledge( asnOut ) );
    BOOST_CHECK_EQUAL( 0u, asnOut.id() ); // $$$$ LDC: = knowledge's id
}

// -----------------------------------------------------------------------------
// Name: TestMIL_RealParameter_ToASN
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_RealParameter_ToASN )
{
    float asnIn = 42;
    MIL_RealParameter param( asnIn );
    float asnOut;
    BOOST_CHECK_EQUAL( true, param.ToNumeric( asnOut ) );
    BOOST_CHECK_EQUAL( 42, asnOut );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_StringParameter_ToASN
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_StringParameter_ToASN )
{
    std::string asnIn( "value" );
    MIL_StringParameter param( asnIn );
    std::string asnOut;
    BOOST_CHECK_EQUAL( true, param.ToString( asnOut ) );
    BOOST_CHECK_EQUAL( asnIn, asnOut );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_TirIndirectParameter_ToASN
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_TirIndirectParameter_ToASN )
{
    FireId asnIn;
    asnIn.set_id( 33 );
    MIL_TirIndirectParameter param( asnIn );
    FireId asnOut;
    BOOST_CHECK_EQUAL( true, param.ToIndirectFire( asnOut ) );
    BOOST_CHECK_EQUAL( 33u, asnOut.id() );
}

namespace
{
    void AddNullParameter( Common::MsgUnitOrder& order )
    {
        order.mutable_parameters()->add_elem()->set_null_value( true );
    }

    void AddPoint( Common::MsgLocation& location, const std::string& /*utm*/ ) // $$$$ _RC_ LGY 2010-08-10: ???
    {
        location.mutable_coordinates()->add_elem()->set_latitude( 0 );
        location.mutable_coordinates()->add_elem()->set_longitude( 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: TestMIL_MissionWithNullParameters
// Created: FHD 2009-11-06
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_MissionWithNullParameters )
{
    Common::MsgUnitOrder order;
    order.mutable_type()->set_id( 173 );
    order.mutable_tasker()->set_id( 159 );
    {
        // danger direction
        Common::MsgMissionParameter& parameter = *order.mutable_parameters()->add_elem();
        parameter.set_null_value( false );
        parameter.mutable_value()->mutable_heading()->set_heading( 128 );
    }
    AddNullParameter( order ); // phase lines
    AddNullParameter( order ); // limit 1
    AddNullParameter( order ); // limit 2
    AddNullParameter( order ); // intelligences
    {
        // polygon area
        Common::MsgMissionParameter& parameter = *order.mutable_parameters()->add_elem();
        parameter.set_null_value( false );
        Common::MsgLocation& location = *parameter.mutable_value()->mutable_polygon()->mutable_location();
        location.set_type( Common::MsgLocation::polygon );
        AddPoint( location, "35RPQ8696412999" );
        AddPoint( location, "35RPQ8729112778" );
        AddPoint( location, "35RPQ8755612970" );
        AddPoint( location, "35RPQ8718513365" );
        AddPoint( location, "35RPQ8696412999" );
    }
     //    const MIL_MissionType_ABC& type;
    //FixturePion pion; //MIL_AgentPion pion;
    //MIL_MissionPion missionpion( MIL_PionMissionType::Find( id ), pion, order );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_CompositeLocationParameter
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_CompositeLocationParameter )
{
    std::string missionDefinition(
        "<mission dia-behavior='whatever' dia-type='T_whatever' id='42' name='a mission'>"
            "<parameter dia-name='objectif' name='Objectif' type='LocationComposite'>"
                "<choice>"
                    "<parameter type='Point'/>"
                    "<parameter type='UrbanKnowledge'/>"
                    "<parameter type='AgentKnowledge'/>"
                "</choice>"
            "</parameter>"
        "</mission>"
    );
    xml::xistringstream xis( missionDefinition );
    xis >> xml::start( "mission" );
    MIL_ParameterType_ABC::Initialize();
    MIL_PionMissionType type( 0, xis );
    const MIL_ParameterType_ABC& paramType = type.GetParameterType( 0 );
    BOOST_CHECK_EQUAL( paramType.GetName(), "LocationComposite" );
}

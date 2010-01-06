#include "simulation_kernel_test_pch.h"

#include "game_asn/Simulation.h"

#include "Fixture.h"
#include "MockDEC_KnowledgeResolver_ABC.h"
#include "MockMIL_EntityManager_ABC.h"
#include "MockMIL_ObjectType_ABC.h"
#include "StubMIL_Time_ABC.h"

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

#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_Population.h"

#include "Network/NET_ASN_Tools.h"

#include "simulation_terrain/TER_World.h"

using namespace mockpp;

// -----------------------------------------------------------------------------
// Name: TestMIL_AgentKnowledgeListParameter_ToASN
// Created: LDC 2009-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_AgentKnowledgeListParameter_ToASN )
{
    ASN1T_UnitKnowledgeList asnIn;
    asnIn.n = 1;
    asnIn.elem = new ASN1T_UnitKnowledge[1];
    asnIn.elem[0] = 0;
    MockDEC_KnowledgeResolver_ABC resolver;
    boost::shared_ptr< DEC_Knowledge_Agent > knowledge( new DEC_Knowledge_Agent() ); // $$$$ LDC: id == 0... :(
    resolver.ResolveKnowledgeAgent_mocker.expects( once() ).will( returnValue( knowledge ) );
    MIL_AgentKnowledgeListParameter param( asnIn, resolver );
    delete[] asnIn.elem;
    ASN1T_UnitKnowledgeList asnOut;
    BOOST_CHECK_EQUAL( true, param.ToAgentKnowledgeList( asnOut ) );
    BOOST_CHECK_EQUAL( 1, asnOut.n );
    BOOST_CHECK_EQUAL( 0, asnOut.elem[0] ); // $$$$ LDC: = knowledge's id
    delete[] asnOut.elem;
}

// -----------------------------------------------------------------------------
// Name: TestMIL_AgentKnowledgeParameter_ToASN
// Created: LDC 2009-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_AgentKnowledgeParameter_ToASN )
{
    ASN1T_UnitKnowledge asnIn = 0;
    MockDEC_KnowledgeResolver_ABC resolver;
    boost::shared_ptr< DEC_Knowledge_Agent > knowledge( new DEC_Knowledge_Agent() ); // $$$$ LDC: id == 0... :(
    resolver.ResolveKnowledgeAgent_mocker.expects( once() ).will( returnValue( knowledge ) );
    MIL_AgentKnowledgeParameter param( asnIn, resolver );
    ASN1T_UnitKnowledge asnOut;
    BOOST_CHECK_EQUAL( true, param.ToAgentKnowledge( asnOut ) );
    BOOST_CHECK_EQUAL( 0, asnOut ); // $$$$ LDC: = knowledge's id
}

// -----------------------------------------------------------------------------
// Name: TestMIL_AgentListParameter_ToASN
// Created: LDC 2009-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_AgentListParameter_ToASN )
{
    ASN1T_UnitList asnIn;
    asnIn.n = 1;
    asnIn.elem = new ASN1T_Unit[1];
    asnIn.elem[0] = 0;
    MockMIL_EntityManager_ABC entityManager;
    FixturePion fixture;
    fixture.pPion_->RegisterRole( *new DEC_RolePion_Decision( *fixture.pPion_, StubDEC_Database() ) );
    entityManager.FindAgentPion_mocker.expects( once() ).will( returnValue( static_cast< MIL_AgentPion* >( fixture.pPion_.get() ) ) );
    MIL_AgentListParameter param( asnIn, entityManager );
    delete[] asnIn.elem;
    ASN1T_UnitList asnOut;
    BOOST_CHECK_EQUAL( true, param.ToAgentList( asnOut ) );
    BOOST_CHECK_EQUAL( 1, asnOut.n );
    BOOST_CHECK_EQUAL( 0, asnOut.elem[0] );
    delete[] asnOut.elem;
}

// -----------------------------------------------------------------------------
// Name: TestMIL_AgentParameter_ToASN
// Created: LDC 2009-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_AgentParameter_ToASN )
{
    ASN1T_Unit asnIn = 0;
    MockMIL_EntityManager_ABC entityManager;
    FixturePion fixture;
    fixture.pPion_->RegisterRole( *new DEC_RolePion_Decision( *fixture.pPion_,  StubDEC_Database() ) );
    entityManager.FindAgentPion_mocker.expects( once() ).will( returnValue( static_cast< MIL_AgentPion* >( fixture.pPion_.get() ) ) );
    MIL_AgentParameter param( asnIn, entityManager );
    ASN1T_Unit asnOut;
    BOOST_CHECK_EQUAL( true, param.ToAgent( asnOut ) );
    BOOST_CHECK_EQUAL( 0, asnOut );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_AutomatParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_AutomatParameter_ToASN )
{
    ASN1T_Automat asnIn = 0;
    MockMIL_EntityManager_ABC entityManager;
    FixtureAutomate fixture;
    //fixture.pAutomat_->RegisterRole( *new DEC_AutomateDecision( *fixture.pAutomat_ ) );
    entityManager.FindAutomate_mocker.expects( once() ).will( returnValue( static_cast< MIL_Automate* >( fixture.pAutomat_.get() ) ) );
    MIL_AutomatParameter param( asnIn, entityManager );
    ASN1T_Automat asnOut;
    BOOST_CHECK_EQUAL( true, param.ToAutomat( asnOut ) );
    BOOST_CHECK_EQUAL( 0, asnOut );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_AutomatListParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_AutomatListParameter_ToASN )
{
    ASN1T_AutomatList asnIn;
    asnIn.n = 1;
    asnIn.elem = new ASN1T_Unit[1];
    asnIn.elem[0] = 0;
    MockMIL_EntityManager_ABC entityManager;
    FixtureAutomate fixture;
    //fixture.pAutomat_->RegisterRole( *new DEC_AutomateDecision( *fixture.pAutomat_ ) );
    entityManager.FindAutomate_mocker.expects( once() ).will( returnValue( static_cast< MIL_Automate* >( fixture.pAutomat_.get() ) ) );
    MIL_AutomatListParameter param( asnIn, entityManager );
    delete[] asnIn.elem;
    ASN1T_AutomatList asnOut;
    BOOST_CHECK_EQUAL( true, param.ToAutomatList( asnOut ) );
    BOOST_CHECK_EQUAL( 1, asnOut.n );
    BOOST_CHECK_EQUAL( 0, asnOut.elem[0] );
    delete[] asnOut.elem;
}

// -----------------------------------------------------------------------------
// Name: TestMIL_AtlasNatureParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_AtlasNatureParameter_ToASN )
{
    ASN1T_AtlasNature asnIn;
    asnIn.numbits = 2;
    asnIn.data[0] = 1;
    asnIn.data[1] = 3;
    MIL_AtlasNatureParameter param( asnIn );
    ASN1T_AtlasNature asnOut;
    BOOST_CHECK_EQUAL( true, param.ToNatureAtlas( asnOut ) );
    BOOST_CHECK_EQUAL( 11, asnOut.numbits ); // $$$$ LDC: Why is this always 11?
    BOOST_CHECK_EQUAL( 1, asnOut.data[0] );
    BOOST_CHECK_EQUAL( 3, asnOut.data[1] );
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

namespace
{
    void CompareDateTo42( const ASN1T_DateTime& asnOut )
    {
        BOOST_CHECK_EQUAL( 15, asnOut.numocts );
        BOOST_CHECK_EQUAL( '1', asnOut.data[0] );
        BOOST_CHECK_EQUAL( '9', asnOut.data[1] );
        BOOST_CHECK_EQUAL( '7', asnOut.data[2] );
        BOOST_CHECK_EQUAL( '0', asnOut.data[3] );
        BOOST_CHECK_EQUAL( '0', asnOut.data[4] );
        BOOST_CHECK_EQUAL( '1', asnOut.data[5] );
        BOOST_CHECK_EQUAL( '0', asnOut.data[6] );
        BOOST_CHECK_EQUAL( '1', asnOut.data[7] );
        BOOST_CHECK_EQUAL( 'T', asnOut.data[8] );
        BOOST_CHECK_EQUAL( '0', asnOut.data[9] );
        BOOST_CHECK_EQUAL( '0', asnOut.data[10] );
        BOOST_CHECK_EQUAL( '0', asnOut.data[11] );
        BOOST_CHECK_EQUAL( '0', asnOut.data[12] );
        BOOST_CHECK_EQUAL( '4', asnOut.data[13] );
        BOOST_CHECK_EQUAL( '2', asnOut.data[14] );
    }
}

// -----------------------------------------------------------------------------
// Name: TestMIL_DateTimeParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_DateTimeParameter_ToASN )
{
    ASN1T_DateTime asnIn;
    NET_ASN_Tools::WriteGDH( 42, asnIn );
    MIL_DateTimeParameter param( asnIn );
    ASN1T_DateTime asnOut;
    BOOST_CHECK_EQUAL( true, param.ToGDH( asnOut ) );
    CompareDateTo42( asnOut );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_DirectionParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_DirectionParameter_ToASN )
{
    ASN1T_Heading asnIn = 42;
    MIL_DirectionParameter param( asnIn );
    ASN1T_Heading asnOut;
    BOOST_CHECK_EQUAL( true, param.ToDirection( asnOut ) );
    BOOST_CHECK_EQUAL( 42, asnOut );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_DotationTypeParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_DotationTypeParameter_ToASN )
{
    xml::xistringstream xisNat( "<natures><nature type='Solid'/></natures>" );
    PHY_DotationNature::Initialize( xisNat );
    xml::xistringstream xis( "<dotations>"
        "<dotation category='Food' id='42' name='ration' nature='Solid' package-mass='0.015' package-size='10' package-volume='0.01'/>"
        "</dotations>" );
    MockMIL_EntityManager_ABC entityManager;
    PHY_DotationType::Initialize( xis );
    ASN1T_DotationType asnIn = 42;
    MIL_DotationTypeParameter param( asnIn );
    ASN1T_DotationType asnOut;
    BOOST_CHECK_EQUAL( true, param.ToDotationType( asnOut ) );
    BOOST_CHECK_EQUAL( 42, asnOut );
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
//    StubMIL_Time_ABC time;
//    PHY_ComposanteTypePion::Initialize( time, xis );
//    ASN1T_EquipmentType asnIn = 22;
//    MIL_EquipmentTypeParameter param( asnIn );
//    ASN1T_EquipmentType asnOut;
//    BOOST_CHECK_EQUAL( true, param.ToEquipmentType( asnOut ) );
//    BOOST_CHECK_EQUAL( 22, asnOut );
//}

namespace
{
    void FillRlyehLocation( ASN1T_Location& asnIn )
    {        
        asnIn.type = EnumLocationType::point;
        asnIn.coordinates.n = 1;
        asnIn.coordinates.elem = new ASN1T_CoordLatLong[1];
        asnIn.coordinates.elem->latitude = 48.52f;
        asnIn.coordinates.elem->longitude = 2.17f;
    }

    void CompareLocationToRlyeh( ASN1T_Location& asnOut )
    {        
        BOOST_CHECK_EQUAL( EnumLocationType::point, asnOut.type );
        BOOST_CHECK_EQUAL( 1, asnOut.coordinates.n );
        BOOST_CHECK_CLOSE( 48.52, asnOut.coordinates.elem->latitude, 1. );
        BOOST_CHECK_CLOSE( 2.17, asnOut.coordinates.elem->longitude, 1. );
        delete[] asnOut.coordinates.elem;
    }
    
    void FillPolygonLocation( ASN1T_Location& asnIn, float offset )
    {        
        asnIn.type = EnumLocationType::polygon;
        asnIn.coordinates.n = 3;
        asnIn.coordinates.elem = new ASN1T_CoordLatLong[3];
        asnIn.coordinates.elem->latitude = 50.f + offset;
        asnIn.coordinates.elem->longitude = 2.17f + offset;
        asnIn.coordinates.elem[1].latitude = 49.52f + offset;
        asnIn.coordinates.elem[1].longitude = 3.f + offset;
        asnIn.coordinates.elem[2].latitude = 48.52f + offset;
        asnIn.coordinates.elem[2].longitude = 3.17f + offset;
    }

    void CompareLocationToPolygon( ASN1T_Location& asnOut, float offset )
    {        
        BOOST_CHECK_EQUAL( EnumLocationType::polygon, asnOut.type );
        BOOST_CHECK_EQUAL( 4, asnOut.coordinates.n );
        BOOST_CHECK_CLOSE( 50. + offset, asnOut.coordinates.elem->latitude, 1. );
        BOOST_CHECK_CLOSE( 2.17 + offset, asnOut.coordinates.elem->longitude, 1. );
        BOOST_CHECK_CLOSE( 49.52 + offset, asnOut.coordinates.elem[1].latitude, 1. );
        BOOST_CHECK_CLOSE( 3. + offset, asnOut.coordinates.elem[1].longitude, 1. );
        BOOST_CHECK_CLOSE( 48.52 + offset, asnOut.coordinates.elem[2].latitude, 1. );
        BOOST_CHECK_CLOSE( 3.17 + offset, asnOut.coordinates.elem[2].longitude, 1. );
        BOOST_CHECK_EQUAL( asnOut.coordinates.elem->latitude, asnOut.coordinates.elem[3].latitude );
        BOOST_CHECK_EQUAL( asnOut.coordinates.elem->longitude, asnOut.coordinates.elem[3].longitude );
        delete[] asnOut.coordinates.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: TestMIL_LocationParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_LocationParameter_ToASN )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );
    ASN1T_Location asnIn;
    FillRlyehLocation( asnIn );
    MIL_LocationParameter param( asnIn );
    delete[] asnIn.coordinates.elem;
    ASN1T_Location asnOut;
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
    ASN1T_LocationList asnIn;
    asnIn.n = 1;
    asnIn.elem = new ASN1T_Location[1];
    FillRlyehLocation( asnIn.elem[0] );
    MIL_LocationListParameter param( asnIn );
    delete[] asnIn.elem[0].coordinates.elem;
    delete[] asnIn.elem;
    ASN1T_LocationList asnOut;
    BOOST_CHECK_EQUAL( true, param.ToLocationList( asnOut ) );
    BOOST_CHECK_EQUAL( 1, asnOut.n );
    CompareLocationToRlyeh( asnOut.elem[0] );
    delete[] asnOut.elem;
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
//    ASN1T_LogMaintenancePriorities asnIn;
//    MIL_LogMaintenancePrioritiesParameter param( asnIn );
//    ASN1T_LogMaintenancePriorities asnOut;
//    BOOST_CHECK_EQUAL( true, param.ToMaintenancePriorities( asnOut ) );
//}

// -----------------------------------------------------------------------------
// Name: TestMIL_LogMaintenancePrioritiesParameter_ToASN
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_LogMaintenancePrioritiesParameter_ToASN )
{
    PHY_HumanWound::Initialize();
    ASN1T_LogMedicalPriorities asnIn;
    asnIn.n = 1;
    asnIn.elem = new ASN1T_EnumHumanWound[1];
    asnIn.elem[0] = EnumHumanWound ::blesse_urgence_2;
    MIL_LogMedicalPrioritiesParameter param( asnIn );
    delete[] asnIn.elem;
    ASN1T_LogMedicalPriorities asnOut;
    BOOST_CHECK_EQUAL( true, param.ToMedicalPriorities( asnOut ) );
    BOOST_CHECK_EQUAL( 1, asnOut.n );
    BOOST_CHECK_EQUAL( EnumHumanWound ::blesse_urgence_2, asnOut.elem[0] );
    delete[] asnOut.elem;
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
//    ASN1T_MissionObjectiveList asnIn;
//    asnIn.n = 1;
//    asnIn.elem = new ASN1T_MissionObjective[1];
//    NET_ASN_Tools::WriteGDH( 42, asnIn.elem[0].horaire );
//    FillRlyehLocation( asnIn.elem[0].localisation );
//    
//    MIL_MissionObjectiveListParameter param( asnIn );
//    
//    delete[] asnIn.elem[0].localisation.coordinates.elem;
//    delete asnIn.elem;
//    
//    ASN1T_MissionObjectiveList asnOut;
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
    ASN1T_ObjectKnowledge asnIn = 0;
    MockDEC_KnowledgeResolver_ABC resolver;
    boost::shared_ptr< DEC_Knowledge_Object > knowledge( new DEC_Knowledge_Object() ); // $$$$ LDC: id == 0... :(
    resolver.ResolveKnowledgeObject_mocker.expects( once() ).will( returnValue( knowledge ) );

    MIL_ObjectKnowledgeParameter param( asnIn, resolver );
    ASN1T_ObjectKnowledge asnOut;
    BOOST_CHECK_EQUAL( true, param.ToObjectKnowledge( asnOut ) );
    BOOST_CHECK_EQUAL( 0, asnOut ); // $$$$ LDC: = knowledge's id
}

// -----------------------------------------------------------------------------
// Name: TestMIL_ObjectKnowledgeListParameter_ToASN
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_ObjectKnowledgeListParameter_ToASN )
{
    ASN1T_ObjectKnowledgeList asnIn;
    asnIn.n = 1;
    asnIn.elem = new ASN1T_ObjectKnowledge[1];
    asnIn.elem[0] = 0;
    MockDEC_KnowledgeResolver_ABC resolver;
    boost::shared_ptr< DEC_Knowledge_Object > knowledge( new DEC_Knowledge_Object() ); // $$$$ LDC: id == 0... :(
    resolver.ResolveKnowledgeObject_mocker.expects( once() ).will( returnValue( knowledge ) );
    MIL_ObjectKnowledgeListParameter param( asnIn, resolver );
    delete[] asnIn.elem;
    ASN1T_ObjectKnowledgeList asnOut;
    BOOST_CHECK_EQUAL( true, param.ToObjectKnowledgeList( asnOut ) );
    BOOST_CHECK_EQUAL( 1, asnOut.n );
    BOOST_CHECK_EQUAL( 0, asnOut.elem[0] ); // $$$$ LDC: = knowledge's id
    delete[] asnOut.elem;
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PathParameter_Throw
// Created: LDC 2009-06-11
// Checsk an error is given if the path is not a line.
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PathParameter_Throw )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );
    ASN1T_Path asnIn;
    FillRlyehLocation( asnIn );
    BOOST_CHECK_THROW( MIL_PathParameter param( asnIn );, std::runtime_error );
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PathParameter_ToASN
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PathParameter_ToASN )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );
    ASN1T_Path asnIn;
    asnIn.type = EnumLocationType::line;
    asnIn.coordinates.n = 2;
    asnIn.coordinates.elem = new ASN1T_CoordLatLong[2];
    asnIn.coordinates.elem->latitude = 48.52f;
    asnIn.coordinates.elem->longitude = 2.17f;
    asnIn.coordinates.elem[1].latitude = 49.52f;
    asnIn.coordinates.elem[1].longitude = 3.17f;
    MIL_PathParameter param( asnIn );
    delete[] asnIn.coordinates.elem;
    ASN1T_Path asnOut;
    BOOST_CHECK_EQUAL( true, param.ToPath( asnOut ) );
    BOOST_CHECK_EQUAL( EnumLocationType::line, asnOut.type );
    BOOST_CHECK_EQUAL( 2, asnOut.coordinates.n );
    BOOST_CHECK_CLOSE( 48.52, asnOut.coordinates.elem->latitude, 1. );
    BOOST_CHECK_CLOSE( 2.17, asnOut.coordinates.elem->longitude, 1. );
    BOOST_CHECK_CLOSE( 49.52, asnOut.coordinates.elem[1].latitude, 1. );
    BOOST_CHECK_CLOSE( 3.17, asnOut.coordinates.elem[1].longitude, 1. );
    delete[] asnOut.coordinates.elem;
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PathListParameter_ToASN
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PathListParameter_ToASN )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );
    ASN1T_PathList asnIn;
    asnIn.n = 1;
    asnIn.elem = new ASN1T_Path[1];
    asnIn.elem->type = EnumLocationType::line;
    asnIn.elem->coordinates.n = 2;
    asnIn.elem->coordinates.elem = new ASN1T_CoordLatLong[2];
    asnIn.elem->coordinates.elem->latitude = 48.52f;
    asnIn.elem->coordinates.elem->longitude = 2.17f;
    asnIn.elem->coordinates.elem[1].latitude = 49.52f;
    asnIn.elem->coordinates.elem[1].longitude = 3.17f;
    MIL_PathListParameter param( asnIn );
    delete[] asnIn.elem->coordinates.elem;
    ASN1T_PathList asnOut;
    BOOST_CHECK_EQUAL( true, param.ToPathList( asnOut ) );
    BOOST_CHECK_EQUAL( 1, asnOut.n );
    BOOST_CHECK_EQUAL( EnumLocationType::line, asnOut.elem->type );
    BOOST_CHECK_EQUAL( 2, asnOut.elem->coordinates.n );
    BOOST_CHECK_CLOSE( 48.52, asnOut.elem->coordinates.elem->latitude, 1. );
    BOOST_CHECK_CLOSE( 2.17, asnOut.elem->coordinates.elem->longitude, 1. );
    BOOST_CHECK_CLOSE( 49.52, asnOut.elem->coordinates.elem[1].latitude, 1. );
    BOOST_CHECK_CLOSE( 3.17, asnOut.elem->coordinates.elem[1].longitude, 1. );
    delete[] asnOut.elem->coordinates.elem;
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
    ASN1T_PlannedWork asnIn;
    asnIn.type = typeName.c_str();
    FillRlyehLocation( asnIn.position );
    asnIn.type_obstacle = EnumDemolitionTargetType::reserved;
    asnIn.densite = 1;
    asnIn.tc2 = 0;
    asnIn.activity_time = 2;
    MockMIL_EntityManager_ABC entityManager;
    MockMIL_ObjectType_ABC objectType;
    objectType.GetName_mocker.expects( once() ).will( returnValue( &typeName ) );
    entityManager.FindObjectType_mocker.expects( once() ).will( returnValue( static_cast< MIL_ObjectType_ABC* >( &objectType ) ) );
    FixtureAutomate fixture;
    entityManager.FindAutomate_mocker.expects( once() ).will( returnValue( static_cast< MIL_Automate* >( fixture.pAutomat_.get() ) ) );
    MIL_PlannedWorkParameter param( asnIn, entityManager );
    delete[] asnIn.position.coordinates.elem;
    ASN1T_PlannedWork asnOut;
    BOOST_CHECK_EQUAL( true, param.ToGenObject( asnOut ) );
    BOOST_CHECK_EQUAL( typeName, asnOut.type );
    CompareLocationToRlyeh( asnOut.position );
    BOOST_CHECK_EQUAL( EnumDemolitionTargetType::reserved, asnOut.type_obstacle );
    BOOST_CHECK_EQUAL( 1, asnOut.densite );
    BOOST_CHECK_EQUAL( 0, asnOut.tc2 );
    BOOST_CHECK_EQUAL( 2, asnOut.activity_time );
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PlannedWorkListParameter_ToASN
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PlannedWorkListParameter_ToASN )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );
    
    const std::string typeName( "type" );
    ASN1T_PlannedWorkList asnIn;
    asnIn.n = 1;
    asnIn.elem = new ASN1T_PlannedWork[1];
    asnIn.elem->type = typeName.c_str();
    FillRlyehLocation( asnIn.elem->position );
    asnIn.elem->type_obstacle = EnumDemolitionTargetType::reserved;
    asnIn.elem->densite = 1;
    asnIn.elem->tc2 = 0;
    asnIn.elem->activity_time = 2;
    MockMIL_EntityManager_ABC entityManager;
    MockMIL_ObjectType_ABC objectType;
    objectType.GetName_mocker.expects( once() ).will( returnValue( &typeName ) );
    entityManager.FindObjectType_mocker.expects( once() ).will( returnValue( static_cast< MIL_ObjectType_ABC* >( &objectType ) ) );
    FixtureAutomate fixture;
    entityManager.FindAutomate_mocker.expects( once() ).will( returnValue( static_cast< MIL_Automate* >( fixture.pAutomat_.get() ) ) );
    MIL_PlannedWorkListParameter param( asnIn, entityManager );
    delete[] asnIn.elem->position.coordinates.elem;
    delete[] asnIn.elem;
    ASN1T_PlannedWorkList asnOut;
    BOOST_CHECK_EQUAL( true, param.ToGenObjectList( asnOut ) );
    BOOST_CHECK_EQUAL( 1, asnOut.n );
    BOOST_CHECK_EQUAL( typeName, asnOut.elem->type );
    CompareLocationToRlyeh( asnOut.elem->position );
    BOOST_CHECK_EQUAL( EnumDemolitionTargetType::reserved, asnOut.elem->type_obstacle );
    BOOST_CHECK_EQUAL( 1, asnOut.elem->densite );
    BOOST_CHECK_EQUAL( 0, asnOut.elem->tc2 );
    BOOST_CHECK_EQUAL( 2, asnOut.elem->activity_time );
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PointParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PointParameter_ToASN )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );
    ASN1T_Point asnIn;
    FillRlyehLocation( asnIn );
    MIL_PointParameter param( asnIn );
    delete[] asnIn.coordinates.elem;
    ASN1T_Point asnOut;
    BOOST_CHECK_EQUAL( true, param.ToPoint( asnOut ) );
    CompareLocationToRlyeh( asnOut );
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PointListParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PointListParameter_ToASN )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );
    ASN1T_PointList asnIn;
    asnIn.n = 1;
    asnIn.elem = new ASN1T_Location[1];
    FillRlyehLocation( asnIn.elem[0] );
    MIL_PointListParameter param( asnIn );
    delete[] asnIn.elem[0].coordinates.elem;
    delete[] asnIn.elem;
    ASN1T_PointList asnOut;
    BOOST_CHECK_EQUAL( true, param.ToPointList( asnOut ) );
    BOOST_CHECK_EQUAL( 1, asnOut.n );
    CompareLocationToRlyeh( asnOut.elem[0] );
    delete[] asnOut.elem;
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PolygonParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PolygonParameter_ToASN )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );
    ASN1T_Polygon asnIn;
    FillPolygonLocation( asnIn, 0.f );
    MIL_PolygonParameter param( asnIn );
    delete[] asnIn.coordinates.elem;
    ASN1T_Polygon asnOut;
    BOOST_CHECK_EQUAL( true, param.ToPolygon( asnOut ) );
    CompareLocationToPolygon( asnOut, 0.f );
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PolygonListParameter_ToASN
// Created: LDC 2009-06-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PolygonListParameter_ToASN )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );
    ASN1T_PolygonList asnIn;
    asnIn.n = 2;
    asnIn.elem = new ASN1T_Location[2];
    FillPolygonLocation( asnIn.elem[0], 0.f );
    FillPolygonLocation( asnIn.elem[1], 0.01f );
    MIL_PolygonListParameter param( asnIn );
    delete[] asnIn.elem[0].coordinates.elem;
    delete[] asnIn.elem[1].coordinates.elem;
    delete[] asnIn.elem;
    ASN1T_PolygonList asnOut;
    BOOST_CHECK_EQUAL( true, param.ToPolygonList( asnOut ) );
    BOOST_CHECK_EQUAL( 2, asnOut.n );
    CompareLocationToPolygon( asnOut.elem[0], 0.f );
    CompareLocationToPolygon( asnOut.elem[1], 0.01f );
    delete[] asnOut.elem;
    TER_World::DestroyWorld();
}

// -----------------------------------------------------------------------------
// Name: TestMIL_PopulationKnowledgeParameter_ToASN
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_PopulationKnowledgeParameter_ToASN )
{
    ASN1T_PopulationKnowledge asnIn = 0;
    MockDEC_KnowledgeResolver_ABC resolver;
    DEC_Knowledge_Population knowledge; // $$$$ LDC: id == 0... :(
    resolver.ResolveKnowledgePopulation_mocker.expects( once() ).will( returnValue( &knowledge ) );

    MIL_PopulationKnowledgeParameter param( asnIn, resolver );
    ASN1T_PopulationKnowledge asnOut;
    BOOST_CHECK_EQUAL( true, param.ToPopulationKnowledge( asnOut ) );
    BOOST_CHECK_EQUAL( 0, asnOut ); // $$$$ LDC: = knowledge's id
}

// -----------------------------------------------------------------------------
// Name: TestMIL_RealParameter_ToASN
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_RealParameter_ToASN )
{
    ASN1REAL asnIn = 42;
    MIL_RealParameter param( asnIn );
    ASN1REAL asnOut;
    BOOST_CHECK_EQUAL( true, param.ToNumeric( asnOut ) );
    BOOST_CHECK_EQUAL( 42, asnOut );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_StringParameter_ToASN
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_StringParameter_ToASN )
{
    std::string value( "value" );
    ASN1VisibleString asnIn = value.c_str();
    MIL_StringParameter param( asnIn );
    ASN1VisibleString asnOut;
    BOOST_CHECK_EQUAL( true, param.ToString( asnOut ) );
    BOOST_CHECK_EQUAL( value.c_str(), asnOut );
}

// -----------------------------------------------------------------------------
// Name: TestMIL_TirIndirectParameter_ToASN
// Created: LDC 2009-06-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestMIL_TirIndirectParameter_ToASN )
{
    ASN1T_UnitFire asnIn = 33;
    MIL_TirIndirectParameter param( asnIn );
    ASN1T_UnitFire asnOut;
    BOOST_CHECK_EQUAL( true, param.ToIndirectFire( asnOut ) );
    BOOST_CHECK_EQUAL( 33, asnOut );
}

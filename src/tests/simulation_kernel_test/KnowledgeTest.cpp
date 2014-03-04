// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
// LTO
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "MockAgent.h"
#include "MockArmy.h"
#include "MockMIL_Time_ABC.h"
#include "MockRoleLocation.h"
#include "StubMIL_PopulationType.h"
#include "StubMIL_Population.h"
#include "MockPHY_RoleInterface_Perceiver.h"
#include "MockKnowledgeGroupFactory.h"
#include "StubMIL_AgentTypePion.h"
#include "StubMIL_Object_ABC.h"
#include "MockMIL_ObjectType_ABC.h"
#include "MockMIL_Object_ABC.h"
#include "MockNET_Publisher_ABC.h"
#include "StubTER_World.h"
#include "MissionController.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgePopulation.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_PopulationConcentration.h"
#include "Knowledge/MIL_KnowledgeGroupType.h"
#include "Knowledge/DEC_Knowledge_PopulationFlow.h"
#include "Decision/DEC_Model.h"
#include "Entities/Orders/MIL_MissionType_ABC.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"
#include "Knowledge/DEC_Knowledge_PopulationPerception.h"
#include "Knowledge/DEC_Knowledge_PopulationConcentrationPerception.h"
#include "Knowledge/DEC_Knowledge_PopulationFlowPerception.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include <boost/lexical_cast.hpp>
#include <xeumeuleu/xml.hpp>

namespace
{
    const double timeFactor = 0.5;

    struct MockAgentWithPerceiver : MockAgent
    {
        MockAgentWithPerceiver()
        {
            RegisterRole( *new MockPHY_RoleInterface_Perceiver() );
        }
    };
    struct Initialization
    {
        Initialization()
        {
            xml::xistringstream xis( "<knowledge-groups>"
                                     "  <knowledge-group name='GTIA' communication-delay='01m'>"
                                     "    <unit-knowledge max-lifetime='03h' max-unit-to-knowledge-distance='60000' interpolation-time='010m'/>"
                                     "    <population-knowledge max-lifetime='2m'/>"
                                    "  </knowledge-group>"
                                    "</knowledge-groups>" );
            MIL_KnowledgeGroupType::InitializeWithTime( xis, timeFactor );
        }
        ~Initialization()
        {
            MIL_KnowledgeGroupType::Terminate();
        }
    };
}

// -----------------------------------------------------------------------------
// Name: TestKnowledgeGroupType
// Created: LDC 2009-12-10
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( TestKnowledgeGroupType, Initialization )
{
    const MIL_KnowledgeGroupType* type = MIL_KnowledgeGroupType::FindType( "GTIA" );
    BOOST_REQUIRE( type );
    BOOST_CHECK_EQUAL( 60 * timeFactor, type->GetKnowledgeCommunicationDelay() );
    BOOST_CHECK_EQUAL( 3 * 60 * 60 * timeFactor, type->GetKnowledgeAgentMaxLifeTime() );
    BOOST_CHECK_EQUAL( 60000, type->GetKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit() );
    BOOST_CHECK_EQUAL( 60 * timeFactor, type->GetKnowledgeCommunicationDelay() );
    BOOST_CHECK_EQUAL( "GTIA", type->GetName() );
    BOOST_CHECK_EQUAL( 10 * 60 * timeFactor, type->GetKnowledgeAgentExtrapolationTime() );
}

namespace
{
    struct ArmyFixture : Initialization
    {
        ArmyFixture()
        {
            MOCK_EXPECT( army.GetID ).returns( 42u );
            MOCK_EXPECT( time.GetCurrentTimeStep ).returns( 1u );
        }
        boost::shared_ptr< MIL_KnowledgeGroup > CreateKnowledgeGroup( unsigned int id )
        {
            return boost::shared_ptr< MIL_KnowledgeGroup >(
                new MIL_KnowledgeGroup(
                    xml::xistringstream( "<root id='" + boost::lexical_cast< std::string >( id ) + "' type='GTIA' name='root'/>" )
                        >> xml::start( "root" ),
                    army, 0 ) );
        }
        boost::shared_ptr< MIL_KnowledgeGroup > CreateKnowledgeGroup( const boost::shared_ptr< MIL_KnowledgeGroup >& group, unsigned int id, const std::string& type )
        {
            xml::xistringstream xis( "<root id='" + boost::lexical_cast< std::string >( id ) + "' type='" + type + "' name='group'/>" );
            xis >> xml::start( "root" );
    //        MOCK_EXPECT( group.RegisterKnowledgeGroup ).once(); // $$$$ _RC_ SBO 2010-04-27: TODO: check registration of nested KG
            boost::shared_ptr< MIL_KnowledgeGroup > result( new MIL_KnowledgeGroup( xis, army, group.get() ) );
            group->RegisterKnowledgeGroup( result );
            return result;
        }
        MockArmy army;
        MockMIL_Time_ABC time;
    };
}

// -----------------------------------------------------------------------------
// Name: TestPropagationInKnowledgeGroups
// Created: HBD 2009-12-10
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( TestPropagationInKnowledgeGroups, ArmyFixture )
{
    boost::shared_ptr< MIL_KnowledgeGroup > group( CreateKnowledgeGroup( 1 ) );
    boost::shared_ptr< MIL_KnowledgeGroup > group1( CreateKnowledgeGroup( group, 2, "GTIA" ) );
    boost::shared_ptr< MIL_KnowledgeGroup > group2( CreateKnowledgeGroup( group, 3, "GTIA" ) );
    MockNET_Publisher_ABC mockPublisher;
    MOCK_EXPECT( mockPublisher.Send );
    DEC_KnowledgeBlackBoard_Army blackboard( army );
    MOCK_EXPECT( army.GetKnowledge ).returns( boost::ref( blackboard ) );
    MockAgentWithPerceiver jammedAgent;
    MOCK_EXPECT( jammedAgent.IsMarkedForDestruction ).returns( false );
    MOCK_EXPECT( jammedAgent.IsDead ).returns( false );
    boost::shared_ptr< MIL_KnowledgeGroup > groupJammed1( new MIL_KnowledgeGroup( *group1, jammedAgent, 0 ) );
    MOCK_EXPECT( jammedAgent.GetKnowledgeGroup ).returns( boost::ref( groupJammed1 ) );
    MockAgentWithPerceiver mockAgent;
    MOCK_EXPECT( mockAgent.IsMarkedForDestruction ).returns( false );
    MOCK_EXPECT( mockAgent.BelongsTo ).with( boost::cref( *group1 ) ).returns( true );
    MOCK_EXPECT( mockAgent.GetKnowledgeGroup ).returns( boost::ref( group1 ) );
    MOCK_EXPECT( mockAgent.GetType );
    MOCK_EXPECT( mockAgent.IsDead ).returns( false );
    //{
        // TestAgentKnowledgeIsCreated
        boost::shared_ptr< DEC_Knowledge_Agent > knowledge( new DEC_Knowledge_Agent( *group1, mockAgent, 0.5 ) );

        DEC_BlackBoard_CanContainKnowledgeAgent& test1 = group1->GetKnowledge()->GetKnowledgeAgentContainer();
        MOCK_EXPECT( mockAgent.CreateKnowledge ).with( mock::same( *group1 ) ).once().returns( knowledge );

        DEC_Knowledge_Agent& obj = test1.CreateKnowledgeAgent( *group1, mockAgent );
        BOOST_CHECK_EQUAL( &obj, knowledge.get() );
        mock::verify( mockAgent );
    //}
    MockAgentWithPerceiver mockAgentJammed1;
    MOCK_EXPECT( mockAgentJammed1.IsMarkedForDestruction ).returns( false );
    MOCK_EXPECT( mockAgentJammed1.BelongsTo ).with( boost::cref( *groupJammed1 ) ).returns( true );
    MOCK_EXPECT( mockAgentJammed1.GetKnowledgeGroup ).returns( boost::ref( groupJammed1 ) );
    MOCK_EXPECT( mockAgentJammed1.GetType );
    MOCK_EXPECT( mockAgentJammed1.IsDead ).returns( false );
    //{
        // TestJammedUnitHasItsOwnKnowledgeGroup
        boost::shared_ptr< DEC_Knowledge_Agent > knowledgeJammed1( new DEC_Knowledge_Agent( *groupJammed1, mockAgentJammed1, 0.5 ) );

        MOCK_EXPECT( mockAgentJammed1.CreateKnowledge ).with( mock::same( *groupJammed1 ) ).once().returns( knowledgeJammed1 );
        DEC_BlackBoard_CanContainKnowledgeAgent& testjammed1 = groupJammed1->GetKnowledge()->GetKnowledgeAgentContainer();
        DEC_Knowledge_Agent& objJammed1 = testjammed1.CreateKnowledgeAgent( *groupJammed1, mockAgentJammed1 );

        BOOST_CHECK_EQUAL( &objJammed1, knowledgeJammed1.get() );
        BOOST_CHECK_EQUAL( true, testjammed1.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( true, test1.HasKnowledgeAgent( mockAgent ) );
        mock::verify( mockAgentJammed1 );
    //}
    //{
        // TestKnowledgePropagationNeedsUpdate
        DEC_BlackBoard_CanContainKnowledgeAgent& test2 = group2->GetKnowledge()->GetKnowledgeAgentContainer();
        BOOST_CHECK_EQUAL( false, test2.HasKnowledgeAgent( mockAgent ) );
        BOOST_CHECK_EQUAL( false, test1.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( false, test2.HasKnowledgeAgent( mockAgentJammed1 ) );
    //}
    //{
        // TestPerceivedUnitCreatesKnowledge
        MT_Vector2D position;
        MockRoleLocation* mockRoleLocation = new MockRoleLocation();
        MOCK_EXPECT( mockRoleLocation->HasDoneMagicMove ).at_least( 1 ).returns( false );
        MOCK_EXPECT( mockRoleLocation->GetPosition ).at_least( 1 ).returns( position );
        mockAgent.RegisterRole( *mockRoleLocation );
        MOCK_EXPECT( mockAgent.GetRole< MockPHY_RoleInterface_Perceiver >().ExecutePerceptions );
        group1->UpdateKnowledges( 1 );
        mock::verify( *mockRoleLocation );
    //}
    //{
        // TestPerceivedUnitCreatesKnowledgeForJammedUnits
        DEC_KnowledgeBlackBoard_AgentPion pionBlackboard( jammedAgent );
        MOCK_EXPECT( jammedAgent.GetKnowledge ).returns( boost::ref( pionBlackboard ) );
        MOCK_EXPECT( jammedAgent.GetRole< MockPHY_RoleInterface_Perceiver >().ExecutePerceptions );
        MockRoleLocation* mockJammedRoleLocation = new MockRoleLocation();
        mockAgentJammed1.RegisterRole( *mockJammedRoleLocation );
        groupJammed1->UpdateKnowledges( 1 );
        group2->UpdateKnowledges( 1 );
        mock::verify( jammedAgent );
    //}
    //{
        // TestKnowledgeIsNotPropagatedWhenSourceIsJammed
        DEC_BlackBoard_CanContainKnowledgeAgent& test3 = group->GetKnowledge()->GetKnowledgeAgentContainer();
        MOCK_EXPECT( mockAgent.CreateKnowledge ).once().returns( knowledge );
//        MOCK_EXPECT( mockAgentJammed1.CreateKnowledge ).once().returns( knowledgeJammed1 );
        group->UpdateKnowledges( 1 );
        BOOST_CHECK_EQUAL( true,  test3.HasKnowledgeAgent( mockAgent ) );
        BOOST_CHECK_EQUAL( false, test2.HasKnowledgeAgent( mockAgent ) );
        BOOST_CHECK_EQUAL( false, testjammed1.HasKnowledgeAgent( mockAgent ) );
        BOOST_CHECK_EQUAL( true,  testjammed1.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( false, test1.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( false, test2.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( false, test3.HasKnowledgeAgent( mockAgentJammed1 ) );
        mock::verify( mockAgent );
        mock::verify( mockAgentJammed1 );
    //}
    //{
        // TestKnowledgeIsPropagatedWhenSourceIsNotJammed
        MOCK_EXPECT( mockJammedRoleLocation->HasDoneMagicMove ).at_least( 1 ).returns( false );
        MOCK_EXPECT( mockJammedRoleLocation->GetPosition ).at_least( 1 ).returns( position );
        group1->UpdateKnowledges( 200 );
        groupJammed1->UpdateKnowledges( 200 );
        mock::verify( *mockJammedRoleLocation );

        MOCK_EXPECT( mockAgent.CreateKnowledge ).once().returns( knowledge );
//        MOCK_EXPECT( mockAgentJammed1.CreateKnowledge ).once().returns( knowledgeJammed1 );
        group2->UpdateKnowledges( 200 );
        group->UpdateKnowledges( 200 );
        BOOST_CHECK_EQUAL( true,  test3.HasKnowledgeAgent( mockAgent ) );
        BOOST_CHECK_EQUAL( true,  test2.HasKnowledgeAgent( mockAgent ) );
        BOOST_CHECK_EQUAL( false, testjammed1.HasKnowledgeAgent( mockAgent ) );
        BOOST_CHECK_EQUAL( true,  testjammed1.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( false, test1.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( false, test2.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( false, test3.HasKnowledgeAgent( mockAgentJammed1 ) );
        mock::verify( mockAgent );
        mock::verify( mockAgentJammed1 );
    //}
}

// -----------------------------------------------------------------------------
// Name: TestExtrapolationTimeInKnowledgeGroup
// Created: FDS 2010-04-28
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( TestExtrapolationTimeInKnowledgeGroup, ArmyFixture )
{
    boost::shared_ptr< MIL_KnowledgeGroup > group( CreateKnowledgeGroup( 1 ) );
    BOOST_CHECK_EQUAL( 10 * 60 * timeFactor, group->GetType().GetKnowledgeAgentExtrapolationTime() );
}

// -----------------------------------------------------------------------------
// Name: TestLatentRelevance
// Created: FDS 2010-04-28
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( TestLatentRelevance, ArmyFixture )
{
    boost::shared_ptr< MIL_KnowledgeGroup > group( CreateKnowledgeGroup( 1 ) );
    boost::shared_ptr< MIL_KnowledgeGroup > knowledgeGroup( CreateKnowledgeGroup( group, 2, "GTIA" ) );
    DEC_KnowledgeBlackBoard_Army blackboard( army );
    MOCK_EXPECT( army.GetKnowledge ).returns( boost::ref( blackboard ) );
    MockAgentWithPerceiver mockAgent;
    MOCK_EXPECT( mockAgent.BelongsTo ).with( mock::same( *knowledgeGroup ) ).returns( true );
    MOCK_EXPECT( mockAgent.GetKnowledgeGroup ).returns( boost::ref( knowledgeGroup ) );
    MOCK_EXPECT( mockAgent.GetType );
    //{
        // TestAgentKnowledgeIsCreatedWithRelevanceMax
        boost::shared_ptr< DEC_Knowledge_Agent > knowledge( new DEC_Knowledge_Agent( *knowledgeGroup, mockAgent, 1. ) ); // 1 for relevance

        DEC_BlackBoard_CanContainKnowledgeAgent& knowledgeAgentContainer = knowledgeGroup->GetKnowledge()->GetKnowledgeAgentContainer();
        MOCK_EXPECT( mockAgent.CreateKnowledge ).once().returns( knowledge );

        DEC_Knowledge_Agent& obj = knowledgeAgentContainer.CreateKnowledgeAgent( *knowledgeGroup, mockAgent );
        BOOST_CHECK_EQUAL( knowledge.get(), &obj );
        BOOST_CHECK_EQUAL( 1., obj.GetRelevance() );
    //}
    //{
        // TestRelevanceDownUntilNull
        MT_Vector2D position;
        MockRoleLocation* mockRoleLocation = new MockRoleLocation();
        MOCK_EXPECT( mockRoleLocation->HasDoneMagicMove ).at_least( 1 ).returns( false );
        MOCK_EXPECT( mockRoleLocation->GetPosition ).at_least( 1 ).returns( position );
        mockAgent.RegisterRole( *mockRoleLocation );
        MOCK_EXPECT( mockAgent.GetRole< MockPHY_RoleInterface_Perceiver >().ExecutePerceptions );
        float relevance = static_cast< float>( obj.GetRelevance() );
        knowledgeGroup->UpdateKnowledges( 1 );
        BOOST_CHECK_LT( obj.GetRelevance(), relevance );
        relevance = static_cast< float>( obj.GetRelevance() );
        knowledgeGroup->UpdateKnowledges( 10 );
        BOOST_CHECK_LT( obj.GetRelevance(), relevance );
        relevance = static_cast< float>( obj.GetRelevance() );
        knowledgeGroup->UpdateKnowledges( 100 );
        BOOST_CHECK_LT( obj.GetRelevance(), relevance );
        relevance = static_cast< float>( obj.GetRelevance() );
        knowledgeGroup->UpdateKnowledges( 1000 );
        BOOST_CHECK_LT( obj.GetRelevance(), relevance );
        relevance = static_cast< float>( obj.GetRelevance() );
        knowledgeGroup->UpdateKnowledges( 10000 );
        BOOST_CHECK_LT( obj.GetRelevance(), relevance );
        knowledgeGroup->UpdateKnowledges( 100000 );
        BOOST_CHECK_LT( obj.GetRelevance(), 0.001 );
    //}
}

// TESTS MERGE AGENT KNOWLEDGE BETWEEN TWO KNOWLEDGE GROUPS

namespace
{
    struct Configuration : ArmyFixture
    {
        Configuration()
            : blackboard( army )
        {
            MOCK_EXPECT( publisher.Send );
            MOCK_EXPECT( army.GetKnowledge ).returns( boost::ref( blackboard ) );
        }
        MockNET_Publisher_ABC publisher;
        DEC_KnowledgeBlackBoard_Army blackboard;
    };
    struct Fixture : Configuration
    {
        Fixture()
            : group1( CreateKnowledgeGroup( 1 ) )
            , group2( CreateKnowledgeGroup( 2 ) )
        {
            group2->Clone( *group1 );
        }
        boost::shared_ptr< MIL_KnowledgeGroup > group1;
        boost::shared_ptr< MIL_KnowledgeGroup > group2;
    };
    struct AgentFixture : Fixture
    {
        AgentFixture()
            : blackBoardGroup1( group1->GetKnowledge()->GetKnowledgeAgentContainer() )
            , blackBoardGroup2( group2->GetKnowledge()->GetKnowledgeAgentContainer() )
        {}
        DEC_BlackBoard_CanContainKnowledgeAgent& blackBoardGroup1;
        DEC_BlackBoard_CanContainKnowledgeAgent& blackBoardGroup2;
    };
    struct ObjectFixture : Fixture
    {
        ObjectFixture()
            : objectBlackBoardGroup1( *group1->GetKnowledgeObjectContainer() )
            , objectBlackBoardGroup2( *group2->GetKnowledgeObjectContainer() )
            , object( &army, objectType, 15u )
            , object2( &army, objectType, 16u )
        {
            objectBlackBoardGroup1.SetKnowledgeGroup( group1.get() );
            objectBlackBoardGroup2.SetKnowledgeGroup( group2.get() );
            MOCK_EXPECT( objectType.GetName ).returns( "type" );
        }
        DEC_BlackBoard_CanContainKnowledgeObject& objectBlackBoardGroup1;
        DEC_BlackBoard_CanContainKnowledgeObject& objectBlackBoardGroup2;
        MockMIL_ObjectType_ABC objectType;
        MockMIL_Object_ABC object;
        MockMIL_Object_ABC object2;
    };
    struct PopulationFixture : Fixture
    {
        PopulationFixture()
            : world_( "worldwide/tests/EmptyParis-ML" )
            , blackBoardGroup1( group1->GetKnowledge()->GetKnowledgePopulationContainer() )
            , blackBoardGroup2( group2->GetKnowledge()->GetKnowledgePopulationContainer() )
            , xis( "<main dia-type='PionTest' file='PionTest.bms'/>" )
            , model( "test", xis >> xml::start( "main" ), testOptions.GetDataPath( "." ), missionTypes, false, testOptions.GetDataPath( "resources" ) )
            , type( model )
        {
            MIL_PopulationAttitude::Initialize();
            MOCK_EXPECT( time.GetCurrentTimeStep ).returns( 1 );
        }

        FakeWorld world_;
        DEC_BlackBoard_CanContainKnowledgePopulation& blackBoardGroup1;
        DEC_BlackBoard_CanContainKnowledgePopulation& blackBoardGroup2;
        MissionController controller;
        MockAgent agent;
        xml::xistringstream xis;
        std::map< std::string, const MIL_MissionType_ABC* > missionTypes;
        DEC_Model model;
        StubMIL_PopulationType type;
    };
    void CreateAgentKnowledge( MockAgent& agent, DEC_BlackBoard_CanContainKnowledgeAgent& blackBoard, boost::shared_ptr< MIL_KnowledgeGroup > group, double relevance )
    {
        MOCK_EXPECT( agent.BelongsTo ).returns( true );
        xml::xistringstream xis( "<main dia-type='PionTest' file='PionTest.bms' id='12' name='stuff'/>" );
        std::map< std::string, const MIL_MissionType_ABC* > missionTypes;
        DEC_Model model( "test", xis >> xml::start( "main" ), testOptions.GetDataPath( "." ), missionTypes, false, testOptions.GetDataPath( "resources" ) );
        StubMIL_AgentTypePion type( model );
        MOCK_EXPECT( agent.GetType ).returns( boost::cref( type ) );
        boost::shared_ptr< DEC_Knowledge_Agent > knowledge( new DEC_Knowledge_Agent( *group, agent, relevance ) );
        MOCK_EXPECT( agent.CreateKnowledge ).with( mock::same( *group ) ).once().returns( knowledge );
        blackBoard.CreateKnowledgeAgent( *group, agent );
    }

    template< typename Knowledge >
    void CheckKnowledge( Knowledge knowledge, double relevance )
    {
        BOOST_REQUIRE( knowledge );
        BOOST_CHECK_EQUAL( knowledge->GetRelevance(), relevance );
    }
}

BOOST_FIXTURE_TEST_CASE( agent_merge_knowledge_group_in_empty_knowledge_group, AgentFixture )
{
    // check group1 is empty
    BOOST_CHECK_EQUAL( blackBoardGroup1.GetKnowledgeAgents().size(), 0u );
    mock::verify();

    // knowledge creation in group2
    MockAgent agent;
    CreateAgentKnowledge( agent, blackBoardGroup2, group2, 0.1 );

    // check group2 is not empty
    BOOST_CHECK_EQUAL( blackBoardGroup2.GetKnowledgeAgents().size(), 1u );
    mock::verify();

    // merge two groups
    blackBoardGroup1.Merge( blackBoardGroup2 );

    // check group1 is not empty
    BOOST_CHECK_EQUAL( blackBoardGroup1.GetKnowledgeAgents().size(), 1u );

    // retrieve knowledge
    CheckKnowledge( blackBoardGroup1.GetKnowledgeAgent( agent ), 0.1 );
}

BOOST_FIXTURE_TEST_CASE( agent_merge_knowledge_group_in_knowledge_group, AgentFixture )
{
    // check group1 is empty
    BOOST_CHECK_EQUAL( blackBoardGroup1.GetKnowledgeAgents().size(), 0u );
    mock::verify();

    // knowledge creation in group2
    MockAgent agent1;
    CreateAgentKnowledge( agent1, blackBoardGroup2, group2, 0.2 );

    // check group2 is not empty
    BOOST_CHECK_EQUAL( blackBoardGroup2.GetKnowledgeAgents().size(), 1u );
    mock::verify();

    // knowledge creation in group1
    MockAgent agent2;
    CreateAgentKnowledge( agent2, blackBoardGroup1, group1, 0.3 );

    // check group1 is not empty
    BOOST_CHECK_EQUAL( blackBoardGroup1.GetKnowledgeAgents().size(), 1u );
    mock::verify();

    // merge two groups
    blackBoardGroup1.Merge( blackBoardGroup2 );

    // check group1 is not empty
    BOOST_CHECK_EQUAL( blackBoardGroup1.GetKnowledgeAgents().size(), 2u );

    // retrieve knowledges
    CheckKnowledge( blackBoardGroup1.GetKnowledgeAgent( agent1 ), 0.2 );
    CheckKnowledge( blackBoardGroup1.GetKnowledgeAgent( agent2 ), 0.3 );
}

BOOST_FIXTURE_TEST_CASE( agent_merge_knowledge_group_in_knowledge_group_with_same_knowledge, AgentFixture )
{
    // check group1 is empty
    BOOST_CHECK_EQUAL( blackBoardGroup1.GetKnowledgeAgents().size(), 0u );
    mock::verify();

    // knowledge creation
    MockAgent agent1;
    CreateAgentKnowledge( agent1, blackBoardGroup1, group1, 0.3 );
    CreateAgentKnowledge( agent1, blackBoardGroup2, group2, 0.5 );

    // knowledge creation
    MockAgent agent2;
    CreateAgentKnowledge( agent2, blackBoardGroup1, group1, 0.9 );
    CreateAgentKnowledge( agent2, blackBoardGroup2, group2, 0.5 );

    // merge two groups
    blackBoardGroup1.Merge( blackBoardGroup2 );

    // check group1 is not empty
    BOOST_CHECK_EQUAL( blackBoardGroup1.GetKnowledgeAgents().size(), 2u );

    // retrieve knowledges
    CheckKnowledge( blackBoardGroup1.GetKnowledgeAgent( agent1 ), 0.5 );
    CheckKnowledge( blackBoardGroup1.GetKnowledgeAgent( agent2 ), 0.9 );
}

namespace
{
    boost::shared_ptr< DEC_Knowledge_Object > CreateObjectKnowledge( MockMIL_Object_ABC& object,
                                                                     DEC_BlackBoard_CanContainKnowledgeObject& blackBoard,
                                                                     boost::shared_ptr< MIL_KnowledgeGroup > group, double relevance )
    {
        MOCK_EXPECT( object.GetName ).returns( "object" );
        boost::shared_ptr< DEC_Knowledge_Object > knowledge( new DEC_Knowledge_Object( *group, object, relevance ) );
        MOCK_EXPECT( object.CreateKnowledgeKnowledgeGroup ).once().returns( knowledge );
        blackBoard.CreateKnowledgeObject( object );
        return knowledge;
    }
}

// TESTS MERGE OBJECT KNOWLEDGE BETWEEN TWO KNOWLEDGE GROUPS

BOOST_FIXTURE_TEST_CASE( object_merge_knowledge_group_in_empty_knowledge_group, ObjectFixture )
{
    // check group1 is empty
    BOOST_CHECK_EQUAL( objectBlackBoardGroup1.GetKnowledgeObjects().size(), 0u );
    mock::verify();

    // knowledge creation
    boost::shared_ptr< DEC_Knowledge_Object > knowledge = CreateObjectKnowledge( object, objectBlackBoardGroup2, group2, 0.5 );

    // check group2 is not empty
    BOOST_CHECK_EQUAL( objectBlackBoardGroup2.GetKnowledgeObjects().size(), 1u );

    // merge
    boost::shared_ptr< DEC_Knowledge_Object > knowledgeMerged( new DEC_Knowledge_Object( *knowledge, army ) );
    MOCK_EXPECT( object.CreateKnowledgeArmyKnowledgeGroup ).once().returns( knowledgeMerged );
    objectBlackBoardGroup1.Merge( objectBlackBoardGroup2 );

    // check group1 is not empty
    BOOST_CHECK_EQUAL( objectBlackBoardGroup1.GetKnowledgeObjects().size(), 1u );

    // check knowledges
    CheckKnowledge( objectBlackBoardGroup1.GetKnowledgeObject( object ), 0.5 );

    // need detroy object knowledge before object
    objectBlackBoardGroup2.DestroyKnowledgeObject( *knowledge );
}

BOOST_FIXTURE_TEST_CASE( object_merge_knowledge_group_in_knowledge_group, ObjectFixture )
{
    // check group1 is empty
    BOOST_CHECK_EQUAL( objectBlackBoardGroup1.GetKnowledgeObjects().size(), 0u );
    mock::verify();

    // knowledge creation
    boost::shared_ptr< DEC_Knowledge_Object > knowledge = CreateObjectKnowledge( object, objectBlackBoardGroup2, group2, 0.5 );

    // knowledge creation
    boost::shared_ptr< DEC_Knowledge_Object > knowledge2 = CreateObjectKnowledge( object2, objectBlackBoardGroup1, group2, 0.4 );

    // merge
    boost::shared_ptr< DEC_Knowledge_Object > knowledgeMerged( new DEC_Knowledge_Object( *knowledge, army ) );
    MOCK_EXPECT( object.CreateKnowledgeArmyKnowledgeGroup ).once().returns( knowledgeMerged );
    objectBlackBoardGroup1.Merge( objectBlackBoardGroup2 );

    // check group1 is not empty
    BOOST_CHECK_EQUAL( objectBlackBoardGroup1.GetKnowledgeObjects().size(), 2u );

    // check knowledges
    CheckKnowledge( objectBlackBoardGroup1.GetKnowledgeObject( object ), 0.5 );
    CheckKnowledge( objectBlackBoardGroup1.GetKnowledgeObject( object2 ), 0.4 );
}

BOOST_FIXTURE_TEST_CASE( object_merge_knowledge_group_in_knowledge_group_with_same_knowledge, ObjectFixture )
{
    // check group1 is empty
    BOOST_CHECK_EQUAL( objectBlackBoardGroup1.GetKnowledgeObjects().size(), 0u );
    mock::verify();

    // knowledge creation
    boost::shared_ptr< DEC_Knowledge_Object > knowledge = CreateObjectKnowledge( object, objectBlackBoardGroup2, group2, 0.5 );
    boost::shared_ptr< DEC_Knowledge_Object > knowledge2 = CreateObjectKnowledge( object, objectBlackBoardGroup1, group2, 0.6 );

    // check group1 is empty
    BOOST_CHECK_EQUAL( objectBlackBoardGroup1.GetKnowledgeObjects().size(), 1u );
    BOOST_CHECK_EQUAL( objectBlackBoardGroup2.GetKnowledgeObjects().size(), 1u );
    mock::verify();

    // knowledge creation
    boost::shared_ptr< DEC_Knowledge_Object > knowledge3 = CreateObjectKnowledge( object2, objectBlackBoardGroup2, group2, 0.7 );
    boost::shared_ptr< DEC_Knowledge_Object > knowledge4 = CreateObjectKnowledge( object2, objectBlackBoardGroup1, group2, 0.4 );

    // check group1 is empty
    BOOST_CHECK_EQUAL( objectBlackBoardGroup1.GetKnowledgeObjects().size(), 2u );
    BOOST_CHECK_EQUAL( objectBlackBoardGroup2.GetKnowledgeObjects().size(), 2u );
    mock::verify();

    // merge
    boost::shared_ptr< DEC_Knowledge_Object > knowledgeMerged2( new DEC_Knowledge_Object( *knowledge3, army ) );
    MOCK_EXPECT( object2.CreateKnowledgeArmyKnowledgeGroup ).once().returns( knowledgeMerged2 );
    objectBlackBoardGroup1.Merge( objectBlackBoardGroup2 );

    // check group1 is not empty
    BOOST_CHECK_EQUAL( objectBlackBoardGroup1.GetKnowledgeObjects().size(), 2u );

    // check knowledges
    CheckKnowledge( objectBlackBoardGroup1.GetKnowledgeObject( object ), 0.6 );
    CheckKnowledge( objectBlackBoardGroup1.GetKnowledgeObject( object2 ), 0.7 );
}

// TESTS MERGE POPULATION KNOWLEDGE BETWEEN TWO KNOWLEDGE GROUPS

namespace
{
    void UpdatePopulationKnowledge( MockAgent& agent, StubMIL_Population& population, MIL_PopulationConcentration& concentration,
                                    MIL_PopulationFlow& flow, DEC_Knowledge_Population& knowledge )
    {
        DEC_Knowledge_PopulationPerception perception( agent, population );
        DEC_Knowledge_PopulationConcentrationPerception concentrationPerception( perception, concentration );
        DEC_Knowledge_PopulationFlowPerception flowPerception( perception, flow );
        knowledge.Update( concentrationPerception );
        knowledge.Update( flowPerception );
    }

    void CheckPopulationKnowledge( StubMIL_Population& population, DEC_BlackBoard_CanContainKnowledgePopulation& blackBoardGroup,
                                   MIL_PopulationConcentration& concentration, MIL_PopulationFlow& flow, double flowRelevance = 1., double concentrationRelevance = 1. )
    {
        boost::shared_ptr< DEC_Knowledge_Population > knowledge = blackBoardGroup.GetKnowledgePopulation( population );
        BOOST_REQUIRE( knowledge );
        const DEC_Knowledge_Population::T_ConcentrationMap& concentrationMap = knowledge->GetConcentrationMap();
        BOOST_CHECK_EQUAL( concentrationMap.size(), 1u );
        const DEC_Knowledge_Population::T_FlowMap& flowMap = knowledge->GetFlowMap();
        BOOST_CHECK_EQUAL( flowMap.size(), 1u );
        mock::verify();

        BOOST_CHECK_CLOSE( concentrationMap.at( concentration.GetID() )->GetRelevance(), concentrationRelevance, 0.1 );
        BOOST_CHECK_CLOSE( flowMap.at( flow.GetID() )->GetRelevance(), flowRelevance, 0.1 );
    }
}

BOOST_FIXTURE_TEST_CASE( population_merge_knowledge_group_in_empty_knowledge_group, PopulationFixture )
{
    // check group1 is empty
    BOOST_CHECK_EQUAL( blackBoardGroup1.GetKnowledgePopulations().size(), 0u );
    mock::verify();

    // knowledge creation in group2
    StubMIL_Population population( type, controller, army );
    DEC_Knowledge_Population& knowledge = blackBoardGroup2.CreateKnowledgePopulation( *group2, population );
    BOOST_CHECK_EQUAL( blackBoardGroup2.GetKnowledgePopulations().size(), 1u );
    mock::verify();

    MIL_PopulationConcentration concentration( population, 29u );
    MIL_PopulationFlow flow( population, concentration );

    UpdatePopulationKnowledge( agent, population, concentration, flow, knowledge );

    // merge two group
    blackBoardGroup1.Merge( blackBoardGroup2 );

    // check group2 is not empty
    BOOST_CHECK_EQUAL( blackBoardGroup1.GetKnowledgePopulations().size(), 1u );
    mock::verify();

    // retrieve knowledge
    CheckPopulationKnowledge( population, blackBoardGroup1, concentration, flow );
    flow.UnregisterSourceConcentration();
}

BOOST_FIXTURE_TEST_CASE( population_merge_knowledge_group_in_knowledge_group, PopulationFixture )
{
    // check group1 is empty
    BOOST_CHECK_EQUAL( blackBoardGroup1.GetKnowledgePopulations().size(), 0u );
    mock::verify();

    // knowledge creation in group2
    StubMIL_Population population2( type, controller, army );
    DEC_Knowledge_Population& knowledge2 = blackBoardGroup2.CreateKnowledgePopulation( *group2, population2 );
    BOOST_CHECK_EQUAL( blackBoardGroup2.GetKnowledgePopulations().size(), 1u );
    mock::verify();

    // knowledge creation in group1
    StubMIL_Population population1( type, controller, army );
    DEC_Knowledge_Population& knowledge1 = blackBoardGroup1.CreateKnowledgePopulation( *group1, population1 );
    BOOST_CHECK_EQUAL( blackBoardGroup1.GetKnowledgePopulations().size(), 1u );
    mock::verify();

    // update knowledges
    MIL_PopulationConcentration concentration1( population1, 29u );
    MIL_PopulationFlow flow1( population1, concentration1 );
    MIL_PopulationConcentration concentration2( population2, 21u );
    MIL_PopulationFlow flow2( population2, concentration2 );

    UpdatePopulationKnowledge( agent, population2, concentration2, flow2, knowledge2 );
    UpdatePopulationKnowledge( agent, population1, concentration1, flow1, knowledge1 );

    // merge two group
    blackBoardGroup1.Merge( blackBoardGroup2 );

    // check group2 is not empty
    BOOST_CHECK_EQUAL( blackBoardGroup1.GetKnowledgePopulations().size(), 2u );
    mock::verify();

    // retrieve knowledge
    CheckPopulationKnowledge( population1, blackBoardGroup1, concentration1, flow1 );
    CheckPopulationKnowledge( population2, blackBoardGroup1, concentration2, flow2 );

    flow1.UnregisterSourceConcentration();
    flow2.UnregisterSourceConcentration();
}

BOOST_FIXTURE_TEST_CASE( population_merge_knowledge_group_in_knowledge_group_with_same_knowledge, PopulationFixture )
{
    // check group1 is empty
    BOOST_CHECK_EQUAL( blackBoardGroup1.GetKnowledgePopulations().size(), 0u );
    mock::verify();

    StubMIL_Population population1( type, controller, army );
    StubMIL_Population population2( type, controller, army );

    // knowledge creation in group2
    DEC_Knowledge_Population& knowledge21 = blackBoardGroup2.CreateKnowledgePopulation( *group2, population1 );
    DEC_Knowledge_Population& knowledge22 = blackBoardGroup2.CreateKnowledgePopulation( *group2, population2 );
    BOOST_CHECK_EQUAL( blackBoardGroup2.GetKnowledgePopulations().size(), 2u );
    mock::verify();

    // knowledge creation in group1
    DEC_Knowledge_Population& knowledge11 = blackBoardGroup1.CreateKnowledgePopulation( *group1, population1 );
    DEC_Knowledge_Population& knowledge12 = blackBoardGroup1.CreateKnowledgePopulation( *group1, population2 );
    BOOST_CHECK_EQUAL( blackBoardGroup1.GetKnowledgePopulations().size(), 2u );
    mock::verify();

    // update knowledges
    MIL_PopulationConcentration concentration1( population1, 29u );
    MIL_PopulationFlow flow1( population1, concentration1 );
    MIL_PopulationConcentration concentration2( population2, 21u );
    MIL_PopulationFlow flow2( population2, concentration2 );

    UpdatePopulationKnowledge( agent, population2, concentration2, flow2, knowledge22 );
    UpdatePopulationKnowledge( agent, population1, concentration1, flow1, knowledge11 );
    UpdatePopulationKnowledge( agent, population2, concentration2, flow2, knowledge12 );
    UpdatePopulationKnowledge( agent, population1, concentration1, flow1, knowledge21 );

    MOCK_RESET( time.GetCurrentTimeStep );
    MOCK_EXPECT( time.GetCurrentTimeStep ).returns( 2 );

    knowledge22.UpdateRelevance();
    knowledge11.UpdateRelevance();

    CheckPopulationKnowledge( population2, blackBoardGroup2, concentration2, flow2, 0, 0.983 );
    CheckPopulationKnowledge( population1, blackBoardGroup1, concentration1, flow1, 0, 0.983 );

    mock::verify();

    // merge two group
    blackBoardGroup1.Merge( blackBoardGroup2 );

    // check group2 is not empty
    BOOST_CHECK_EQUAL( blackBoardGroup1.GetKnowledgePopulations().size(), 2u );
    mock::verify();

    // retrieve knowledge
    CheckPopulationKnowledge( population2, blackBoardGroup1, concentration2, flow2 );

    flow1.UnregisterSourceConcentration();
    flow2.UnregisterSourceConcentration();
}

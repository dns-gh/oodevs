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
#include "MockPHY_RoleInterface_Perceiver.h"
#include "MockKnowledgeGroupFactory.h"
#include "MockNET_Publisher_ABC.h"

#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/MIL_KnowledgeGroupType.h"
#include <boost/lexical_cast.hpp>
#include <xeumeuleu/xml.hpp>

namespace
{
    class MockAgentWithPerceiver : public MockAgent
    {
    public:
        MockAgentWithPerceiver()
        {
            RegisterRole( *new MockPHY_RoleInterface_Perceiver() );
        }
        virtual ~MockAgentWithPerceiver() {}
    };
    const double timeFactor = 0.5f;
}

// -----------------------------------------------------------------------------
// Name: TestKnowledgeGroupType
// Created: LDC 2009-12-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestKnowledgeGroupType )
{
    const std::string initialisation =
        "<knowledge-groups>"
            "<knowledge-group name='GTIA' communication-delay='01m'>"
                "<unit-knowledge max-lifetime='03h' max-unit-to-knowledge-distance='60000' interpolation-time='010m'/>"
                "<population-knowledge max-lifetime='2m'/>"
            "</knowledge-group>"
        "</knowledge-groups>";
    xml::xistringstream xis( initialisation );
    MIL_KnowledgeGroupType::InitializeWithTime( xis, timeFactor );
    const MIL_KnowledgeGroupType &kgType = *MIL_KnowledgeGroupType::FindType("GTIA");
    BOOST_CHECK_EQUAL( 60 * timeFactor, kgType.GetKnowledgeCommunicationDelay() );
    BOOST_CHECK_EQUAL( 3 * 60 * 60 * timeFactor, kgType.GetKnowledgeAgentMaxLifeTime() );
    BOOST_CHECK_EQUAL( 60000, kgType.GetKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit() );
    BOOST_CHECK_EQUAL( 60 * timeFactor, kgType.GetKnowledgeCommunicationDelay() );
    BOOST_CHECK_EQUAL( "GTIA", kgType.GetName() );
    BOOST_CHECK_EQUAL( 10 * 60 * timeFactor, kgType.GetKnowledgeAgentExtrapolationTime() );
}

namespace
{
    std::auto_ptr< MIL_KnowledgeGroup > CreateKnowledgeGroup( MockArmy& army, unsigned int id )
    {
        const MIL_KnowledgeGroupType& kgType = *MIL_KnowledgeGroupType::FindType( "GTIA" );
        MOCK_EXPECT( army, RegisterKnowledgeGroup ).once();
        std::auto_ptr< MIL_KnowledgeGroup > result( new MIL_KnowledgeGroup( kgType, id, army ) );
        return result;
    }

    std::auto_ptr< MIL_KnowledgeGroup > CreateKnowledgeGroup( MockArmy& army, MIL_KnowledgeGroup& group, unsigned int id, const std::string& type )
    {
        MockKnowledgeGroupFactory factory;
        xml::xistringstream xis( "<root id='" + boost::lexical_cast< std::string >( id ) + "' type='" + type + "' name='group'/>" );
        xis >> xml::start( "root" );
//        MOCK_EXPECT( group, RegisterKnowledgeGroup ).once(); // $$$$ _RC_ SBO 2010-04-27: TODO: check registration of nested KG
        std::auto_ptr< MIL_KnowledgeGroup > result( new MIL_KnowledgeGroup( xis, army, &group, factory ) );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: TestPropagationInKnowledgeGroups
// Created: HBD 2009-12-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestPropagationInKnowledgeGroups )
{
    MockArmy army;
    MOCK_EXPECT( army, GetID ).returns( 42u );
    MockMIL_Time_ABC time;
    MOCK_EXPECT( time, GetCurrentTick ).returns( 1u );
    std::auto_ptr< MIL_KnowledgeGroup > armyGroup( CreateKnowledgeGroup( army, 1 ) );
    std::auto_ptr< MIL_KnowledgeGroup > group1( CreateKnowledgeGroup( army, *armyGroup, 2, "GTIA" ) );
    std::auto_ptr< MIL_KnowledgeGroup > group2( CreateKnowledgeGroup( army, *armyGroup, 3, "GTIA" ) );
    MockNET_Publisher_ABC mockPublisher;
    MOCK_EXPECT( mockPublisher, Send ).once();
    DEC_KnowledgeBlackBoard_Army blackboard( army );
    MOCK_EXPECT( army, GetKnowledge ).returns( boost::ref( blackboard ) );
    MockAgentWithPerceiver jammedAgent;
    MOCK_EXPECT( army, RegisterKnowledgeGroup ).once();
    MIL_KnowledgeGroup groupJammed1( *group1, jammedAgent, 0 );
    MockAgentWithPerceiver mockAgent;
    MOCK_EXPECT( mockAgent, BelongsTo ).with( boost::cref( *group1 ) ).returns( true );
    MOCK_EXPECT( mockAgent, GetType );
    //{
        // TestAgentKnowledgeIsCreated
        boost::shared_ptr< DEC_Knowledge_Agent > knowledge( new DEC_Knowledge_Agent( *group1, mockAgent, 0.5 ) );

        DEC_BlackBoard_CanContainKnowledgeAgent& test1 = group1->GetKnowledge().GetKnowledgeAgentContainer();
        MOCK_EXPECT( mockAgent, CreateKnowledge ).with( mock::same( *group1 ) ).once().returns( knowledge );

        DEC_Knowledge_Agent& obj = test1.CreateKnowledgeAgent( *group1, mockAgent );
        BOOST_CHECK_EQUAL( &obj, knowledge.get() );
        mockAgent.verify();
    //}
    MockAgentWithPerceiver mockAgentJammed1;
    MOCK_EXPECT( mockAgentJammed1, BelongsTo ).with( boost::cref( groupJammed1 ) ).returns( true );
    MOCK_EXPECT( mockAgentJammed1, GetType );
    //{
        // TestJammedUnitHasItsOwnKnowledgeGroup
        boost::shared_ptr< DEC_Knowledge_Agent > knowledgeJammed1( new DEC_Knowledge_Agent( groupJammed1, mockAgentJammed1, 0.5 ) );

        MOCK_EXPECT( mockAgentJammed1, CreateKnowledge ).with( mock::same( groupJammed1 ) ).once().returns( knowledgeJammed1 );
        DEC_BlackBoard_CanContainKnowledgeAgent& testjammed1 = groupJammed1.GetKnowledge().GetKnowledgeAgentContainer();
        DEC_Knowledge_Agent& objJammed1 = testjammed1.CreateKnowledgeAgent( groupJammed1, mockAgentJammed1 );

        BOOST_CHECK_EQUAL( &objJammed1, knowledgeJammed1.get() );
        BOOST_CHECK_EQUAL( true, testjammed1.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( true, test1.HasKnowledgeAgent( mockAgent ) );
        mockAgentJammed1.verify();
    //}
    //{
        // TestKnowledgePropagationNeedsUpdate
        DEC_BlackBoard_CanContainKnowledgeAgent& test2 = group2->GetKnowledge().GetKnowledgeAgentContainer();
        BOOST_CHECK_EQUAL( false, test2.HasKnowledgeAgent( mockAgent ) );
        BOOST_CHECK_EQUAL( false, test1.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( false, test2.HasKnowledgeAgent( mockAgentJammed1 ) );
    //}
    //{
        // TestPerceivedUnitCreatesKnowledge
        MT_Vector2D position;
        MockRoleLocation* mockRoleLocation = new MockRoleLocation();
        MOCK_EXPECT( mockRoleLocation, HasDoneMagicMove ).at_least( 1 ).returns( false );
        MOCK_EXPECT( mockRoleLocation, GetPosition ).at_least( 1 ).returns( position );
        mockAgent.RegisterRole( *mockRoleLocation );
        MOCK_EXPECT( mockAgent.GetRole< MockPHY_RoleInterface_Perceiver >(), ExecutePerceptions );
        group1->UpdateKnowledges( 1 );
        mockRoleLocation->verify();
    //}
    //{
        // TestPerceivedUnitCreatesKnowledgeForJammedUnits
        DEC_KnowledgeBlackBoard_AgentPion pionBlackboard( jammedAgent );
        MOCK_EXPECT( jammedAgent, GetKnowledge ).returns( boost::ref( pionBlackboard ) );
        MOCK_EXPECT( jammedAgent.GetRole< MockPHY_RoleInterface_Perceiver >(), ExecutePerceptions );
        MockRoleLocation* mockJammedRoleLocation = new MockRoleLocation();
        mockAgentJammed1.RegisterRole( *mockJammedRoleLocation );
        groupJammed1.UpdateKnowledges( 1 );
        group2->UpdateKnowledges( 1 );
        jammedAgent.verify();
    //}
    //{
        // TestKnowledgeIsNotPropagatedWhenSourceIsJammed
        DEC_BlackBoard_CanContainKnowledgeAgent& test3 = armyGroup->GetKnowledge().GetKnowledgeAgentContainer();
        MOCK_EXPECT( mockAgent, CreateKnowledge ).once().returns( knowledge );
//        MOCK_EXPECT( mockAgentJammed1, CreateKnowledge ).once().returns( knowledgeJammed1 );
        armyGroup->UpdateKnowledges( 1 );
        BOOST_CHECK_EQUAL( true,  test3.HasKnowledgeAgent( mockAgent ) );
        BOOST_CHECK_EQUAL( false, test2.HasKnowledgeAgent( mockAgent ) );
        BOOST_CHECK_EQUAL( false, testjammed1.HasKnowledgeAgent( mockAgent ) );
        BOOST_CHECK_EQUAL( true,  testjammed1.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( false, test1.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( false, test2.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( false, test3.HasKnowledgeAgent( mockAgentJammed1 ) );
        mockAgent.verify();
        mockAgentJammed1.verify();
    //}
    //{
        // TestKnowledgeIsPropagatedWhenSourceIsNotJammed
        MOCK_EXPECT( mockJammedRoleLocation, HasDoneMagicMove ).at_least( 1 ).returns( false );
        MOCK_EXPECT( mockJammedRoleLocation, GetPosition ).at_least( 1 ).returns( position );
        group1->UpdateKnowledges( 200 );
        groupJammed1.UpdateKnowledges( 200 );
        mockJammedRoleLocation->verify();

        MOCK_EXPECT( mockAgent, CreateKnowledge ).once().returns( knowledge );
//        MOCK_EXPECT( mockAgentJammed1, CreateKnowledge ).once().returns( knowledgeJammed1 );
        group2->UpdateKnowledges( 200 );
        armyGroup->UpdateKnowledges( 200 );
        BOOST_CHECK_EQUAL( true,  test3.HasKnowledgeAgent( mockAgent ) );
        BOOST_CHECK_EQUAL( true,  test2.HasKnowledgeAgent( mockAgent ) );
        BOOST_CHECK_EQUAL( false, testjammed1.HasKnowledgeAgent( mockAgent ) );
        BOOST_CHECK_EQUAL( true,  testjammed1.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( false, test1.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( false, test2.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( false, test3.HasKnowledgeAgent( mockAgentJammed1 ) );
        mockAgent.verify();
        mockAgentJammed1.verify();
    //}
    MOCK_EXPECT( army, UnregisterKnowledgeGroup ).with( mock::same( groupJammed1 ) ).once();
    MOCK_EXPECT( army, UnregisterKnowledgeGroup ).with( mock::same( *armyGroup ) ).once();
}

// -----------------------------------------------------------------------------
// Name: TestExtrapolationTimeInKnowledgeGroup
// Created: FDS 2010-04-28
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestExtrapolationTimeInKnowledgeGroup )
{
    MockArmy army;
    std::auto_ptr< MIL_KnowledgeGroup > armyGroup( CreateKnowledgeGroup( army, 1 ) );
    BOOST_CHECK_EQUAL( 10 * 60 * timeFactor, armyGroup->GetType().GetKnowledgeAgentExtrapolationTime() );
    MOCK_EXPECT( army, UnregisterKnowledgeGroup ).with( mock::same( *armyGroup ) ).once();
}

// -----------------------------------------------------------------------------
// Name: TestLatentRelevance
// Created: FDS 2010-04-28
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestLatentRelevance )
{
    MockArmy army;
    MockMIL_Time_ABC time;
    MOCK_EXPECT( time, GetCurrentTick ).returns( 1u );
    std::auto_ptr< MIL_KnowledgeGroup > armyGroup( CreateKnowledgeGroup( army, 1 ) );
    std::auto_ptr< MIL_KnowledgeGroup > knowledgeGroup   ( CreateKnowledgeGroup( army, *armyGroup, 2, "GTIA" ) );
    DEC_KnowledgeBlackBoard_Army blackboard( army );
    MOCK_EXPECT( army, GetKnowledge ).returns( boost::ref( blackboard ) );
    MockAgentWithPerceiver mockAgent;
    MOCK_EXPECT( mockAgent, BelongsTo ).with( mock::same( *knowledgeGroup ) ).returns( true );
    MOCK_EXPECT( mockAgent, GetType );
    //{
        // TestAgentKnowledgeIsCreatedWithRelevanceMax
        boost::shared_ptr< DEC_Knowledge_Agent > knowledge( new DEC_Knowledge_Agent( *knowledgeGroup, mockAgent, 1. ) ); // 1 for relevance

        DEC_BlackBoard_CanContainKnowledgeAgent& knowledgeAgentContainer = knowledgeGroup->GetKnowledge().GetKnowledgeAgentContainer();
        MOCK_EXPECT( mockAgent, CreateKnowledge ).once().returns( knowledge );

        DEC_Knowledge_Agent& obj = knowledgeAgentContainer.CreateKnowledgeAgent( *knowledgeGroup, mockAgent );
        BOOST_CHECK_EQUAL( knowledge.get(), &obj );
        BOOST_CHECK_EQUAL( 1., obj.GetRelevance() );
    //}
    //{
        // TestRelevanceDownUntilNull
        MT_Vector2D position;
        MockRoleLocation* mockRoleLocation = new MockRoleLocation();
        MOCK_EXPECT( mockRoleLocation, HasDoneMagicMove ).at_least( 1 ).returns( false );
        MOCK_EXPECT( mockRoleLocation, GetPosition ).at_least( 1 ).returns( position );
        mockAgent.RegisterRole( *mockRoleLocation );
        MOCK_EXPECT( mockAgent.GetRole< MockPHY_RoleInterface_Perceiver >(), ExecutePerceptions );
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
    MOCK_EXPECT( army, UnregisterKnowledgeGroup ).with( mock::same( *armyGroup ) ).once();
}

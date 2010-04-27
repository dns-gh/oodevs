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
#include "MockRoleLocation.h"
#include "MockPHY_RoleInterface_Perceiver.h"
#include "MockKnowledgeGroupFactory.h"
#include "MockNET_Publisher_ABC.h"
#include "MT_Tools/MT_Profiler.h"
#include "Tools/MIL_Config.h"

#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/MIL_KnowledgeGroupType.h"
#include "UrbanModel.h"
#include <boost/lexical_cast.hpp>
#include <xeumeuleu/xml.h>

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

}

// -----------------------------------------------------------------------------
// Name: TestKnowledgeGroupType
// Created: LDC 2009-12-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestKnowledgeGroupType )
{
    const std::string initialisation =
        "<knowledge-groups>"
            "<knowledge-group name=\"GTIA\" communication-delay=\"01m\">"
                "<unit-knowledge max-lifetime=\"03h\" max-unit-to-knowledge-distance=\"60000\"/>"
                "<population-knowledge max-lifetime=\"2m\"/>"
            "</knowledge-group>"
        "</knowledge-groups>";

    xml::xistringstream xis( initialisation );

    const double timeFactor = 0.5f;
    MIL_KnowledgeGroupType::Initialize( xis, timeFactor );

    const MIL_KnowledgeGroupType &kgType = *MIL_KnowledgeGroupType::FindType("GTIA");
    BOOST_CHECK_EQUAL( 60 * timeFactor, kgType.GetKnowledgeCommunicationDelay() );
    BOOST_CHECK_EQUAL( 3 * 60 * 60 * timeFactor, kgType.GetKnowledgeAgentMaxLifeTime() );
    BOOST_CHECK_EQUAL( 60000, kgType.GetKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit() );
    BOOST_CHECK_EQUAL( 60 * timeFactor, kgType.GetKnowledgeCommunicationDelay() );
    BOOST_CHECK_EQUAL( "GTIA", kgType.GetName() );
    BOOST_CHECK_EQUAL( 1, kgType.GetKnowledgeAgentExtrapolationTime() );
}

namespace
{
    std::auto_ptr< MIL_KnowledgeGroup > CreateKnowledgeGroup( MockArmy& army, unsigned int id, const std::string& type )
    {
        const MIL_KnowledgeGroupType& kgType = *MIL_KnowledgeGroupType::FindType( "GTIA" );
        MOCKPP_CHAINER_FOR( MockArmy, RegisterKnowledgeGroup ) ( &army ).expects( mockpp::once() );
        MOCKPP_CHAINER_FOR( MockArmy, GetID ) ( &army ).expects( mockpp::once() ).will( mockpp::returnValue( 42u ) );
        std::auto_ptr< MIL_KnowledgeGroup > result( new MIL_KnowledgeGroup( kgType, id, army ) );
        return result;
    }

    std::auto_ptr< MIL_KnowledgeGroup > CreateKnowledgeGroup( MockArmy& army, MIL_KnowledgeGroup& group, unsigned int id, const std::string& type )
    {
        MockKnowledgeGroupFactory factory;
        xml::xistringstream xis( "<root id='" + boost::lexical_cast< std::string >( id ) + "' type='" + type + "'/>" );
        xis >> xml::start( "root" );
        MOCKPP_CHAINER_FOR( MockArmy, RegisterKnowledgeGroup ) ( &army ).expects( mockpp::atLeastOnce() );
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
    std::auto_ptr< MIL_KnowledgeGroup > armyGroup( CreateKnowledgeGroup( army, 1, "GTIA" ) );
    std::auto_ptr< MIL_KnowledgeGroup > group1   ( CreateKnowledgeGroup( army, *armyGroup, 2, "GTIA" ) );
    std::auto_ptr< MIL_KnowledgeGroup > group2   ( CreateKnowledgeGroup( army, *armyGroup, 3, "GTIA" ) );

    MockNET_Publisher_ABC mockPublisher;
    MOCK_EXPECT( mockPublisher, Send ).once();
    
    UrbanModel urbanModel; // (needed for the blackboard through singleton...)
    DEC_KnowledgeBlackBoard_Army blackboard( army );
    MOCKPP_CHAINER_FOR( MockArmy, GetKnowledgeShadow )( &army ).expects( mockpp::once() ).will( mockpp::returnValue( &blackboard ) );

    MockAgentWithPerceiver jammedAgent;
    MIL_KnowledgeGroup groupJammed1( *group1, jammedAgent );
    MOCKPP_CHAINER_FOR( MockArmy, GetKnowledgeShadow )( &army ).expects( mockpp::once() );

    MockAgentWithPerceiver mockAgent;
    //{
        // TestAgentKnowledgeIsCreated
        MOCKPP_CHAINER_FOR( MockAgent, BelongsTo )( &mockAgent ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( true ) );
        MOCKPP_CHAINER_FOR( MockAgent, GetTypeShadow )( &mockAgent ).expects( mockpp::once() );
        boost::shared_ptr< DEC_Knowledge_Agent > knowledge( new DEC_Knowledge_Agent( *group1, mockAgent, 0.5 ) );

        DEC_BlackBoard_CanContainKnowledgeAgent& test1 = group1->GetKnowledge().GetKnowledgeAgentContainer();
        MOCKPP_CHAINER_FOR( MockAgent, CreateKnowledgeShadow ) ( &mockAgent ).expects( mockpp::once() ).will( mockpp::returnValue( knowledge ) );

        DEC_Knowledge_Agent& obj = test1.CreateKnowledgeAgent( *group1, mockAgent );
        BOOST_CHECK_EQUAL( &obj, knowledge.get() );
    //}
    //{
        // TestJammedUnitHasItsOwnKnowledgeGroup
        MockAgentWithPerceiver mockAgentJammed1;
        MOCKPP_CHAINER_FOR( MockAgent, BelongsTo )( &mockAgentJammed1 ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( true ) );
        MOCKPP_CHAINER_FOR( MockAgent, GetTypeShadow )( &mockAgentJammed1 ).expects( mockpp::once() );
        boost::shared_ptr< DEC_Knowledge_Agent > knowledgeJammed1( new DEC_Knowledge_Agent( groupJammed1, mockAgentJammed1, 0.5 ) );

        MOCKPP_CHAINER_FOR( MockAgent, CreateKnowledgeShadow ) ( &mockAgentJammed1 ).expects( mockpp::once() ).will( mockpp::returnValue( knowledgeJammed1 ) );
        DEC_BlackBoard_CanContainKnowledgeAgent& testjammed1 = groupJammed1.GetKnowledge().GetKnowledgeAgentContainer();
        DEC_Knowledge_Agent& objJammed1 = testjammed1.CreateKnowledgeAgent( groupJammed1, mockAgentJammed1 );

        BOOST_CHECK_EQUAL( &objJammed1, knowledgeJammed1.get() );
        BOOST_CHECK_EQUAL( true, testjammed1.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( true, test1.HasKnowledgeAgent( mockAgent ) );
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
        MOCKPP_CHAINER_FOR( MockRoleLocation, HasDoneMagicMove ) ( mockRoleLocation ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( false ) );
        MOCKPP_CHAINER_FOR( MockRoleLocation, GetPositionShadow ) ( mockRoleLocation ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( static_cast< const MT_Vector2D* >( &position ) ) );
        mockAgent.RegisterRole( *mockRoleLocation );
        MOCK_EXPECT( mockAgent.GetRole< MockPHY_RoleInterface_Perceiver >(), ExecutePerceptions );
        group1->UpdateKnowledges( 1 );
    //}
    //{
        // TestPerceivedUnitCreatesKnowledgeForJammedUnits
        DEC_KnowledgeBlackBoard_AgentPion pionBlackboard( jammedAgent );
        jammedAgent.GetKnowledge_mocker.expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( &pionBlackboard ) );
        MOCK_EXPECT( jammedAgent.GetRole< MockPHY_RoleInterface_Perceiver >(), ExecutePerceptions );
        groupJammed1.UpdateKnowledges( 1 );
        group2->UpdateKnowledges( 1 );
    //}
    //{
        // TestKnowledgeIsNotPropagatedWhenSourceIsJammed
        DEC_BlackBoard_CanContainKnowledgeAgent& test3 = armyGroup->GetKnowledge().GetKnowledgeAgentContainer();
        MOCKPP_CHAINER_FOR( MockAgent, CreateKnowledgeShadow ) ( &mockAgent ).expects( mockpp::once() ).will( mockpp::returnValue( knowledge ) );
        MOCKPP_CHAINER_FOR( MockAgent, CreateKnowledgeShadow ) ( &mockAgentJammed1 ).expects( mockpp::once() ).will( mockpp::returnValue( knowledgeJammed1 ) );    
        armyGroup->UpdateKnowledges( 1 );
        BOOST_CHECK_EQUAL( true,  test3.HasKnowledgeAgent( mockAgent ) );
        BOOST_CHECK_EQUAL( false, test2.HasKnowledgeAgent( mockAgent ) );
        BOOST_CHECK_EQUAL( false, testjammed1.HasKnowledgeAgent( mockAgent ) );
        BOOST_CHECK_EQUAL( true,  testjammed1.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( false, test1.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( false, test2.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( false, test3.HasKnowledgeAgent( mockAgentJammed1 ) );
    //}
    //{
        // TestKnowledgeIsPropagatedWhenSourceIsNotJammed
        group1->UpdateKnowledges( 200 );
        groupJammed1.UpdateKnowledges( 200 );
        MOCKPP_CHAINER_FOR( MockAgent, CreateKnowledgeShadow ) ( &mockAgent ).expects( mockpp::once() ).will( mockpp::returnValue( knowledge ) );
        MOCKPP_CHAINER_FOR( MockAgent, CreateKnowledgeShadow ) ( &mockAgentJammed1 ).expects( mockpp::once() ).will( mockpp::returnValue( knowledgeJammed1 ) );    
        group2->UpdateKnowledges( 200 );
        armyGroup->UpdateKnowledges( 200 );
        BOOST_CHECK_EQUAL( true,  test3.HasKnowledgeAgent( mockAgent ) );
        BOOST_CHECK_EQUAL( true,  test2.HasKnowledgeAgent( mockAgent ) );
        BOOST_CHECK_EQUAL( false, testjammed1.HasKnowledgeAgent( mockAgent ) );
        BOOST_CHECK_EQUAL( true,  testjammed1.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( false, test1.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( false, test2.HasKnowledgeAgent( mockAgentJammed1 ) );
        BOOST_CHECK_EQUAL( false, test3.HasKnowledgeAgent( mockAgentJammed1 ) );
    //}

    MOCKPP_CHAINER_FOR( MockArmy, UnregisterKnowledgeGroup ) ( &army ).expects( mockpp::exactly( 3 ) );
}

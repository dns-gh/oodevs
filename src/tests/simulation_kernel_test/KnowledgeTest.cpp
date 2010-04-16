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

#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: TestKnowledgeGroupType
// Created: LDC 2009-12-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestKnowledgeGroupType )
{
    const std::string initialisation ="<knowledge-groups><knowledge-group name=\"GTIA\" communication-delay=\"01m\">"
        "<unit-knowledge max-lifetime=\"03h\" max-unit-to-knowledge-distance=\"60000\"/>"
        "<population-knowledge max-lifetime=\"2m\"/>"
        "</knowledge-group>"
        "</knowledge-groups>";

    xml::xistringstream xis( initialisation );

    double timeFactor = 0.5f;
    MIL_KnowledgeGroupType::Initialize(xis, timeFactor);


    const MIL_KnowledgeGroupType &kgType = *MIL_KnowledgeGroupType::FindType("GTIA");
    BOOST_CHECK_EQUAL( 60 * timeFactor, kgType.GetKnowledgeCommunicationDelay() );
    BOOST_CHECK_EQUAL( 3 * 60 * 60 * timeFactor, kgType.GetKnowledgeAgentMaxLifeTime() );
    BOOST_CHECK_EQUAL( 60000, kgType.GetKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit() );
    BOOST_CHECK_EQUAL( 60 * timeFactor, kgType.GetKnowledgeCommunicationDelay() );
    BOOST_CHECK_EQUAL( "GTIA", kgType.GetName() );
    BOOST_CHECK_EQUAL( 1, kgType.GetKnowledgeAgentExtrapolationTime() );
}
    


// -----------------------------------------------------------------------------
// Name: TestPropagationInKnowledgeGroups
// Created: HBD 2009-12-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestPropagationInKnowledgeGroups )
{
    MockNET_Publisher_ABC mockPublisher;
    MockArmy army;
    MockKnowledgeGroupFactory mockKnowledgeGroupFactory;
    
    // Use previously defined type
    const MIL_KnowledgeGroupType &kgType = *MIL_KnowledgeGroupType::FindType("GTIA");
  
    MOCKPP_CHAINER_FOR( MockArmy, RegisterKnowledgeGroup ) ( &army ).expects( mockpp::once() );
    MOCKPP_CHAINER_FOR( MockArmy, GetID ) ( &army ).expects( mockpp::once() ).will( mockpp::returnValue( 42u ) );

    MIL_KnowledgeGroup armyGroup( kgType, 1, army );

    xml::xistringstream xis2( "<root id='2' type='GTIA'/>" );
    xis2 >> xml::start( "root" );
    MIL_KnowledgeGroup group1( xis2, army, &armyGroup, mockKnowledgeGroupFactory );
    MOCKPP_CHAINER_FOR( MockArmy, RegisterKnowledgeGroup ) ( &army ).expects( mockpp::atLeastOnce() );
    xml::xistringstream xis3( "<root id='3' type='GTIA'/>" );
    xis3 >> xml::start( "root" );
    MIL_KnowledgeGroup group2( xis3, army, &armyGroup, mockKnowledgeGroupFactory );


    xml::xistringstream xis4( "<root id='4' type='GTIA'/>" );
    xis4 >> xml::start( "root" );
    mockPublisher.Send_mocker.expects( mockpp::once() );
    MockAgent jammedAgent;

    UrbanModel urbanModel; // (needed for the blackboard through singleton...)
    DEC_KnowledgeBlackBoard_Army blackboard( army );
    MOCKPP_CHAINER_FOR( MockArmy, GetKnowledgeShadow )( &army ).expects( mockpp::once() ).will( mockpp::returnValue( &blackboard ) );

    MIL_KnowledgeGroup groupJammed1( group1, jammedAgent );

    MOCKPP_CHAINER_FOR( MockArmy, GetKnowledgeShadow )( &army ).expects( mockpp::once() );

    MockAgent mockAgent;
    
    const DEC_KnowledgeBlackBoard_KnowledgeGroup& bb1 = group1.GetKnowledge();
    DEC_BlackBoard_CanContainKnowledgeAgent& test1 = bb1.GetKnowledgeAgentContainer();
    
    MOCKPP_CHAINER_FOR( MockAgent, BelongsTo )( &mockAgent ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( true ) );
    MOCKPP_CHAINER_FOR( MockAgent, GetTypeShadow )( &mockAgent ).expects( mockpp::once() );

    boost::shared_ptr< DEC_Knowledge_Agent > knowledge( new DEC_Knowledge_Agent( group1, mockAgent, 0.5 ) );
   
    MOCKPP_CHAINER_FOR( MockAgent, CreateKnowledgeShadow ) ( &mockAgent ).expects( mockpp::once() ).will( mockpp::returnValue( knowledge ) );
    DEC_Knowledge_Agent& obj = test1.CreateKnowledgeAgent( group1, mockAgent );
    BOOST_CHECK_EQUAL( &obj, knowledge.get() );


    MockAgent mockAgentJammed1;
    MOCKPP_CHAINER_FOR( MockAgent, BelongsTo )( &mockAgentJammed1 ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( true ) );
    MOCKPP_CHAINER_FOR( MockAgent, GetTypeShadow )( &mockAgentJammed1 ).expects( mockpp::once() );
    boost::shared_ptr< DEC_Knowledge_Agent > knowledgeJammed1( new DEC_Knowledge_Agent( groupJammed1, mockAgentJammed1, 0.5 ) );
    MOCKPP_CHAINER_FOR( MockAgent, CreateKnowledgeShadow ) ( &mockAgentJammed1 ).expects( mockpp::once() ).will( mockpp::returnValue( knowledgeJammed1 ) );
    
    const DEC_KnowledgeBlackBoard_KnowledgeGroup& bbjammed1 = groupJammed1.GetKnowledge();
    DEC_BlackBoard_CanContainKnowledgeAgent& testjammed1 = bbjammed1.GetKnowledgeAgentContainer();

    DEC_Knowledge_Agent& objJammed1 = testjammed1.CreateKnowledgeAgent( groupJammed1, mockAgentJammed1 );
    BOOST_CHECK_EQUAL( &objJammed1, knowledgeJammed1.get() );
    BOOST_CHECK_EQUAL( true, testjammed1.HasKnowledgeAgent( mockAgentJammed1 ) );

    BOOST_CHECK_EQUAL( true, test1.HasKnowledgeAgent( mockAgent ) );
    const DEC_KnowledgeBlackBoard_KnowledgeGroup& bb2 = group2.GetKnowledge();
    DEC_BlackBoard_CanContainKnowledgeAgent& test2 = bb2.GetKnowledgeAgentContainer();
    BOOST_CHECK_EQUAL( false, test2.HasKnowledgeAgent( mockAgent ) );
    BOOST_CHECK_EQUAL( false, test1.HasKnowledgeAgent( mockAgentJammed1 ) );
    BOOST_CHECK_EQUAL( false, test2.HasKnowledgeAgent( mockAgentJammed1 ) );

    MockRoleLocation* mockRoleLocation = new MockRoleLocation();
 
    const DEC_KnowledgeBlackBoard_KnowledgeGroup& bb3 = armyGroup.GetKnowledge();
    DEC_BlackBoard_CanContainKnowledgeAgent& test3 = bb3.GetKnowledgeAgentContainer();

    MOCKPP_CHAINER_FOR( MockRoleLocation, HasDoneMagicMove ) ( mockRoleLocation ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( false ) );

    MT_Vector2D position;
    MOCKPP_CHAINER_FOR( MockRoleLocation, GetPositionShadow ) ( mockRoleLocation ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( static_cast< const MT_Vector2D* >( &position ) ) );
    mockAgent.RegisterRole( *mockRoleLocation );

    group1.UpdateKnowledges( 1 );
    DEC_KnowledgeBlackBoard_AgentPion pionBlackboard( jammedAgent );
    jammedAgent.GetKnowledge_mocker.expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( &pionBlackboard ) );
    groupJammed1.UpdateKnowledges( 1 );
    group2.UpdateKnowledges( 1 );
    MOCKPP_CHAINER_FOR( MockAgent, CreateKnowledgeShadow ) ( &mockAgent ).expects( mockpp::once() ).will( mockpp::returnValue( knowledge ) );
    MOCKPP_CHAINER_FOR( MockAgent, CreateKnowledgeShadow ) ( &mockAgentJammed1 ).expects( mockpp::once() ).will( mockpp::returnValue( knowledgeJammed1 ) );    
    armyGroup.UpdateKnowledges( 1 );
    BOOST_CHECK_EQUAL( true, test3.HasKnowledgeAgent( mockAgent ) );
    BOOST_CHECK_EQUAL( false, test2.HasKnowledgeAgent( mockAgent ) );
    
    BOOST_CHECK_EQUAL( false, testjammed1.HasKnowledgeAgent( mockAgent ) );
    BOOST_CHECK_EQUAL( true, testjammed1.HasKnowledgeAgent( mockAgentJammed1 ) );
    BOOST_CHECK_EQUAL( false, test1.HasKnowledgeAgent( mockAgentJammed1 ) );
    BOOST_CHECK_EQUAL( false, test2.HasKnowledgeAgent( mockAgentJammed1 ) );
    BOOST_CHECK_EQUAL( false, test3.HasKnowledgeAgent( mockAgentJammed1 ) );


    group1.UpdateKnowledges( 200 );
    groupJammed1.UpdateKnowledges( 200 );
    MOCKPP_CHAINER_FOR( MockAgent, CreateKnowledgeShadow ) ( &mockAgent ).expects( mockpp::once() ).will( mockpp::returnValue( knowledge ) );
    MOCKPP_CHAINER_FOR( MockAgent, CreateKnowledgeShadow ) ( &mockAgentJammed1 ).expects( mockpp::once() ).will( mockpp::returnValue( knowledgeJammed1 ) );    
    group2.UpdateKnowledges( 200 );
    armyGroup.UpdateKnowledges( 200 );
    BOOST_CHECK_EQUAL( true, test2.HasKnowledgeAgent( mockAgent ) );

    BOOST_CHECK_EQUAL( false, testjammed1.HasKnowledgeAgent( mockAgent ) );
    BOOST_CHECK_EQUAL( true, testjammed1.HasKnowledgeAgent( mockAgentJammed1 ) );
    BOOST_CHECK_EQUAL( false, test1.HasKnowledgeAgent( mockAgentJammed1 ) );
    BOOST_CHECK_EQUAL( false, test2.HasKnowledgeAgent( mockAgentJammed1 ) );
    BOOST_CHECK_EQUAL( false, test3.HasKnowledgeAgent( mockAgentJammed1 ) );


    MOCKPP_CHAINER_FOR( MockArmy, UnregisterKnowledgeGroup ) ( &army ).expects( mockpp::atLeastOnce() );
}

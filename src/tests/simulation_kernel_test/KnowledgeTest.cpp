// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "MockAgent.h"
#include "MockArmy.h"
#include "MockRoleLocation.h"
#include "MockKnowledgeGroupFactory.h"
#include "MT_Tools/MT_Profiler.h"
#include "Tools/MIL_Config.h"

#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/MIL_KnowledgeGroupType.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
//#include "simulation_kernel/MIL_AgentServer.h"

#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: TestKnowledge
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
// Name: TestKnowledge
// Created: HBD 2009-12-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestKnowledgeGroup1 )
{
    MockArmy army;
    MockKnowledgeGroupFactory mockKnowledgeGroupFactory;
    
    // Use previously define type
    const MIL_KnowledgeGroupType &kgType = *MIL_KnowledgeGroupType::FindType("GTIA");
  
    MOCKPP_CHAINER_FOR( MockArmy, RegisterKnowledgeGroup ) ( &army ).expects( mockpp::once() );
    MIL_KnowledgeGroup groupArmy( kgType, 1, army );

    xml::xistringstream xis2( "<toto id='2' type='GTIA'/>" );
    xis2 >> xml::start( "toto" );
    MIL_KnowledgeGroup group1( xis2, army, &groupArmy, mockKnowledgeGroupFactory );
    MOCKPP_CHAINER_FOR( MockArmy, RegisterKnowledgeGroup ) ( &army ).expects( mockpp::atLeastOnce() );
    xml::xistringstream xis3( "<toto id='3' type='GTIA'/>" );
    xis3 >> xml::start( "toto" );
    MIL_KnowledgeGroup group2( xis3, army, &groupArmy, mockKnowledgeGroupFactory );

    MockAgent mockAgent;
    
    const DEC_KnowledgeBlackBoard_KnowledgeGroup& bb1 = group1.GetKnowledge();
    DEC_BlackBoard_CanContainKnowledgeAgent& test1 = bb1.GetKnowledgeAgentContainer();
    
    MOCKPP_CHAINER_FOR( MockAgent, BelongsTo )( &mockAgent ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( true ) );
    MOCKPP_CHAINER_FOR( MockAgent, GetTypeShadow )( &mockAgent ).expects( mockpp::once() );
    
    boost::shared_ptr< DEC_Knowledge_Agent > knowledge( new DEC_Knowledge_Agent( group1, mockAgent, 0.5 ) );
   
    MOCKPP_CHAINER_FOR( MockAgent, CreateKnowledgeShadow ) ( &mockAgent ).expects( mockpp::once() ).will( mockpp::returnValue( knowledge ) );
    DEC_Knowledge_Agent& obj = test1.CreateKnowledgeAgent( group1, mockAgent );
    BOOST_CHECK_EQUAL( &obj, knowledge.get() );

    BOOST_CHECK_EQUAL( true, test1.HasKnowledgeAgent( mockAgent ) );
    const DEC_KnowledgeBlackBoard_KnowledgeGroup& bb2 = group2.GetKnowledge();
    DEC_BlackBoard_CanContainKnowledgeAgent& test2 = bb2.GetKnowledgeAgentContainer();
    BOOST_CHECK_EQUAL( false, test2.HasKnowledgeAgent( mockAgent ) );

    MockRoleLocation* mockRoleLocation = new MockRoleLocation();
 
    const DEC_KnowledgeBlackBoard_KnowledgeGroup& bb3 = groupArmy.GetKnowledge();
    DEC_BlackBoard_CanContainKnowledgeAgent& test3 = bb3.GetKnowledgeAgentContainer();

    MOCKPP_CHAINER_FOR( MockRoleLocation, HasDoneMagicMove ) ( mockRoleLocation ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( false ) );

    MT_Vector2D position;
    MOCKPP_CHAINER_FOR( MockRoleLocation, GetPositionShadow ) ( mockRoleLocation ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( static_cast< const MT_Vector2D* >( &position ) ) );
    mockAgent.RegisterRole( *mockRoleLocation );

    group1.UpdateKnowledges( 1 );
    group2.UpdateKnowledges( 1 );
    MOCKPP_CHAINER_FOR( MockAgent, CreateKnowledgeShadow ) ( &mockAgent ).expects( mockpp::once() ).will( mockpp::returnValue( knowledge ) );
    groupArmy.UpdateKnowledges( 1 );
    BOOST_CHECK_EQUAL( true, test3.HasKnowledgeAgent( mockAgent ) );
    BOOST_CHECK_EQUAL( false, test2.HasKnowledgeAgent( mockAgent ) );

    group1.UpdateKnowledges( 200 );
    MOCKPP_CHAINER_FOR( MockAgent, CreateKnowledgeShadow ) ( &mockAgent ).expects( mockpp::once() ).will( mockpp::returnValue( knowledge ) );
    group2.UpdateKnowledges( 200 );
    groupArmy.UpdateKnowledges( 200 );
    BOOST_CHECK_EQUAL( true, test2.HasKnowledgeAgent( mockAgent ) );

    MOCKPP_CHAINER_FOR( MockArmy, UnregisterKnowledgeGroup ) ( &army ).expects( mockpp::atLeastOnce() );

}

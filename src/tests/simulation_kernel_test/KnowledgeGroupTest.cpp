// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
// LTO
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"

#include <xeumeuleu/xml.h>

#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroupType.h"
#include "simulation_kernel/Entities/MIL_Army_ABC.h"
#include "tools/resolver.h"

#include "MockNET_Publisher_ABC.h"
#include "MockArmy.h"
#include "MockKnowledgeGroupFactory.h"

#include "protocol/SimulationSenders.h"
#include "protocol/ClientSenders.h"


using namespace mockpp;


// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: FHD 2010-01-19: 
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ReceiveChangeSuperiorKnowledgeGroupUnderKnowledgeGroup )
{
    MockArmy army;
    MockKnowledgeGroupFactory mockKnowledgeGroupFactory;
    
    // define knowledgeGroup type
    const std::string initialisation ="<knowledge-groups><knowledge-group name=\"TOTO\" communication-delay=\"01m\">"
        "<unit-knowledge max-lifetime=\"03h\" max-unit-to-knowledge-distance=\"60000\"/>"
        "<population-knowledge max-lifetime=\"2m\"/>"
        "</knowledge-group>"
        "</knowledge-groups>";
    xml::xistringstream xis( initialisation );
    double timeFactor = 0.5f;
    MIL_KnowledgeGroupType::Initialize(xis, timeFactor);

    const MIL_KnowledgeGroupType &kgType = *MIL_KnowledgeGroupType::FindType("TOTO");
  
    // register army sub knowledge group
    MOCKPP_CHAINER_FOR( MockArmy, RegisterKnowledgeGroup ) ( &army ).expects( mockpp::once() );
    MIL_KnowledgeGroup groupArmy( kgType, 1, army );

    // register 1st sub knowledge group
    xml::xistringstream xis2( "<root id='2' type='TOTO'/>" );
    xis2 >> xml::start( "root" );
    MIL_KnowledgeGroup group1( xis2, army, &groupArmy, mockKnowledgeGroupFactory );
    MOCKPP_CHAINER_FOR( MockArmy, RegisterKnowledgeGroup ) ( &army ).expects( mockpp::atLeastOnce() );        

    // register 2nd sub knowledge group
    xml::xistringstream xis3( "<root id='3' type='TOTO'/>" );
    xis3 >> xml::start( "root" );
    MIL_KnowledgeGroup group2( xis3, army, &groupArmy, mockKnowledgeGroupFactory );
    
    MsgsClientToSim::MsgKnowledgeMagicAction msg;
    msg.set_oid( group2.GetId() );
    msg.set_type( MsgsClientToSim::MsgKnowledgeMagicAction_Type_update_side_parent );
    MOCKPP_CHAINER_FOR( MockArmy, GetID ) ( &army ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue< uint >( 1 ) );
    msg.mutable_parametres()->add_elem()->mutable_value()->set_army( army.GetID() );
    msg.mutable_parametres()->add_elem()->mutable_value()->set_knowledgegroup( group1.GetId() );

    tools::Resolver< MIL_Army_ABC > armies;
    armies.Register( army.GetID(), army );

    MOCKPP_CHAINER_FOR( MockArmy, FindKnowledgeGroup ) ( &army ).expects( mockpp::once() ).will( mockpp::returnValue( &group1 ) );

    // initialize publisher
    MockNET_Publisher_ABC mockPublisher;
    MOCK_EXPECT( mockPublisher, Send ).at_least( 1 );

    // moves group2 under group1
    group2.OnReceiveMsgKnowledgeGroupUpdate( msg, armies );

    // verify
    BOOST_CHECK_EQUAL( &group1, group2.GetParent() );
    mockPublisher.verify();
    MOCKPP_CHAINER_FOR( MockArmy, UnregisterKnowledgeGroup ) ( &army ).expects( mockpp::once() );   
    
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: FHD 2010-01-19: 
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ReceiveChangeSuperiorKnowledgeGroupUnderArmy )
{
    MockArmy army;
    MockKnowledgeGroupFactory mockKnowledgeGroupFactory;
    
    // Use previously define type
    const MIL_KnowledgeGroupType &kgType = *MIL_KnowledgeGroupType::FindType("TOTO");
  
    // register army sub knowledge group
    MOCKPP_CHAINER_FOR( MockArmy, RegisterKnowledgeGroup ) ( &army ).expects( mockpp::once() );
    MIL_KnowledgeGroup groupArmy( kgType, 10, army );

    // register 1st sub knowledge group
    xml::xistringstream xis2( "<root id='12' type='TOTO'/>" );
    xis2 >> xml::start( "root" );
    MIL_KnowledgeGroup group1( xis2, army, &groupArmy, mockKnowledgeGroupFactory );
    MOCKPP_CHAINER_FOR( MockArmy, RegisterKnowledgeGroup ) ( &army ).expects( mockpp::atLeastOnce() );        

    // register 2nd sub knowledge group
    xml::xistringstream xis3( "<root id='13' type='TOTO'/>" );
    xis3 >> xml::start( "root" );
    MIL_KnowledgeGroup group2( xis3, army, &group1, mockKnowledgeGroupFactory );
    
    MsgsClientToSim::MsgKnowledgeMagicAction msg;
    msg.set_oid( group2.GetId() );
    msg.set_type( MsgsClientToSim::MsgKnowledgeMagicAction_Type_update_side );
    MOCKPP_CHAINER_FOR( MockArmy, GetID ) ( &army ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue< uint >( 10 ) );
    msg.mutable_parametres()->add_elem()->mutable_value()->set_army( army.GetID() );

    tools::Resolver< MIL_Army_ABC > armies;
    armies.Register( army.GetID(), army );

    MIL_KnowledgeGroup* ptr_null = 0;
    MOCKPP_CHAINER_FOR( MockArmy, FindKnowledgeGroup ) ( &army ).expects( mockpp::once() ).will( mockpp::returnValue( ptr_null ) );

    // initialize publisher
    MockNET_Publisher_ABC mockPublisher;
    MOCK_EXPECT( mockPublisher, Send ).at_least( 1 );

    // moves group2 under group1
    group2.OnReceiveMsgKnowledgeGroupUpdate( msg, armies );

    // verify
    BOOST_CHECK( !group2.GetParent() );
    mockPublisher.verify();
    MOCKPP_CHAINER_FOR( MockArmy, UnregisterKnowledgeGroup ) ( &army ).expects( mockpp::exactly( 2 ) );
    
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: FHD 2010-01-19: 
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ReceiveChangeSuperiorArmyUnderKnowledgeGroup )
{
    MockArmy army;
    MockKnowledgeGroupFactory mockKnowledgeGroupFactory;
    
    // Use previously define type
    const MIL_KnowledgeGroupType &kgType = *MIL_KnowledgeGroupType::FindType("TOTO");
  
    // register army sub knowledge group
    MOCKPP_CHAINER_FOR( MockArmy, RegisterKnowledgeGroup ) ( &army ).expects( mockpp::once() );
    MIL_KnowledgeGroup groupArmy( kgType, 20, army );

    // register 1st sub knowledge group
    xml::xistringstream xis2( "<root id='21' type='TOTO'/>" );
    xis2 >> xml::start( "root" );
    MIL_KnowledgeGroup group1( xis2, army, &groupArmy, mockKnowledgeGroupFactory );
    MOCKPP_CHAINER_FOR( MockArmy, RegisterKnowledgeGroup ) ( &army ).expects( mockpp::atLeastOnce() );        

    // register 2nd sub knowledge group
    xml::xistringstream xis3( "<root id='22' type='TOTO'/>" );
    xis3 >> xml::start( "root" );
    MIL_KnowledgeGroup group2( xis3, army, 0, mockKnowledgeGroupFactory );
    
    MsgsClientToSim::MsgKnowledgeMagicAction msg;
    msg.set_oid( group2.GetId() );
    msg.set_type( MsgsClientToSim::MsgKnowledgeMagicAction_Type_update_side_parent );
    MOCKPP_CHAINER_FOR( MockArmy, GetID ) ( &army ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue< uint >( 20 ) );
    msg.mutable_parametres()->add_elem()->mutable_value()->set_army( army.GetID() );
    msg.mutable_parametres()->add_elem()->mutable_value()->set_knowledgegroup( group1.GetId() );

    tools::Resolver< MIL_Army_ABC > armies;
    armies.Register( army.GetID(), army );

    MOCKPP_CHAINER_FOR( MockArmy, FindKnowledgeGroup ) ( &army ).expects( mockpp::once() ).will( mockpp::returnValue( &group1 ) );

    // initialize publisher
    MockNET_Publisher_ABC mockPublisher;
    MOCK_EXPECT( mockPublisher, Send ).at_least( 1 );

    MOCKPP_CHAINER_FOR( MockArmy, UnregisterKnowledgeGroup ) ( &army ).expects( mockpp::once() );
    // moves group2 under group1
    group2.OnReceiveMsgKnowledgeGroupUpdate( msg, armies );

    // verify
    BOOST_CHECK_EQUAL( &group1, group2.GetParent() );
    mockPublisher.verify();
    MOCKPP_CHAINER_FOR( MockArmy, UnregisterKnowledgeGroup ) ( &army ).expects( mockpp::once() );   
   
}


// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: FHD 2010-01-19: 
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ReceiveKnowledgeGroupSetType )
{
    MockArmy army;
    MockKnowledgeGroupFactory mockKnowledgeGroupFactory;
    
    // define a new knowledge group type
    const std::string initialisation ="<knowledge-groups><knowledge-group name=\"Standard\" communication-delay=\"01m\">"
    "<unit-knowledge max-lifetime=\"03h\" max-unit-to-knowledge-distance=\"60000\"/>"
    "<population-knowledge max-lifetime=\"2m\"/>"
    "</knowledge-group>"
    "</knowledge-groups>";
    xml::xistringstream xis( initialisation );
    double timeFactor = 0.5f;
    MIL_KnowledgeGroupType::Initialize(xis, timeFactor);

    // register army sub knowledge group
    const MIL_KnowledgeGroupType &kgType = *MIL_KnowledgeGroupType::FindType("Standard");
    MOCKPP_CHAINER_FOR( MockArmy, RegisterKnowledgeGroup ) ( &army ).expects( mockpp::once() );
    MIL_KnowledgeGroup groupArmy( kgType, 30, army );

    // prepare message    
    MsgsClientToSim::MsgKnowledgeMagicAction msg;
    msg.set_oid( groupArmy.GetId() );
    msg.set_type( MsgsClientToSim::MsgKnowledgeMagicAction_Type_update_type );
    const MIL_KnowledgeGroupType &kgType_new = *MIL_KnowledgeGroupType::FindType("TOTO");
    msg.mutable_parametres()->add_elem()->mutable_value()->set_acharstr( kgType_new.GetName().c_str() );

    tools::Resolver< MIL_Army_ABC > armies;
    MOCKPP_CHAINER_FOR( MockArmy, GetID ) ( &army ).expects( mockpp::once() ).will( mockpp::returnValue< uint >( 1 ) );
    armies.Register( army.GetID(), army );

    // initialize publisher
    MockNET_Publisher_ABC mockPublisher;
    MOCK_EXPECT( mockPublisher, Send ).at_least( 1 );

    // change knowledge group type
    groupArmy.OnReceiveMsgKnowledgeGroupUpdate( msg, armies );

    // verify
    BOOST_CHECK_EQUAL( "TOTO", groupArmy.GetType().GetName().c_str() );
    mockPublisher.verify();
    MOCKPP_CHAINER_FOR( MockArmy, UnregisterKnowledgeGroup ) ( &army ).expects( mockpp::once() );   
   
}

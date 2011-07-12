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
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroupType.h"
#include "simulation_kernel/Entities/MIL_Army_ABC.h"
#include "MockNET_Publisher_ABC.h"
#include "MockArmy.h"
#include "MockKnowledgeGroupFactory.h"
#include "protocol/SimulationSenders.h"
#include "protocol/ClientSenders.h"
#include "tools/resolver.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: FHD 2010-01-19:
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ReceiveChangeSuperiorKnowledgeGroupUnderKnowledgeGroup )
{
    MockArmy army;
    MOCK_EXPECT( army, GetID ).returns( 1 );
    MockKnowledgeGroupFactory mockKnowledgeGroupFactory;

    // define knowledgeGroup type
    const std::string initialisation ="<knowledge-groups><knowledge-group name=\"TOTO\" communication-delay=\"01m\">"
        "<unit-knowledge max-lifetime=\"03h\" max-unit-to-knowledge-distance=\"60000\"/>"
        "<population-knowledge max-lifetime=\"2m\"/>"
        "</knowledge-group>"
        "</knowledge-groups>";
    xml::xistringstream xis( initialisation );
    double timeFactor = 0.5f;
    MIL_KnowledgeGroupType::InitializeWithTime(xis, timeFactor);

    const MIL_KnowledgeGroupType &kgType = *MIL_KnowledgeGroupType::FindType("TOTO");

    // register army sub knowledge group
    MOCK_EXPECT( army, RegisterKnowledgeGroup ).once();
    MIL_KnowledgeGroup groupArmy( kgType, 1, army );

    // register 1st sub knowledge group
    xml::xistringstream xis2( "<root id='2' type='TOTO' name='toto1'/>" );
    xis2 >> xml::start( "root" );
//    MOCK_EXPECT( groupArmy, RegisterKnowledgeGroup ).once(); // $$$$ _RC_ SBO 2010-04-27: TODO: check registration into parent KG
    MIL_KnowledgeGroup group1( xis2, army, &groupArmy, mockKnowledgeGroupFactory );
//    army.verify();

    // register 2nd sub knowledge group
    xml::xistringstream xis3( "<root id='3' type='TOTO' name='toto2'/>" );
    xis3 >> xml::start( "root" );
//    MOCK_EXPECT( groupArmy, RegisterKnowledgeGroup ).once(); // $$$$ _RC_ SBO 2010-04-27: TODO: check registration into parent KG
    MIL_KnowledgeGroup group2( xis3, army, &groupArmy, mockKnowledgeGroupFactory );
//    army.verify();

    sword::KnowledgeMagicAction msg;
    msg.mutable_knowledge_group()->set_id( group2.GetId() );
    msg.set_type( sword::KnowledgeMagicAction::update_party_parent );
    msg.mutable_parameters()->add_elem()->mutable_value()->Add()->mutable_party()->set_id( army.GetID() );
    msg.mutable_parameters()->add_elem()->mutable_value()->Add()->mutable_knowledgegroup()->set_id( group1.GetId() );

    tools::Resolver< MIL_Army_ABC > armies;
    armies.Register( army.GetID(), army );
    army.verify();

    MOCK_EXPECT( army, FindKnowledgeGroup ).once().returns( &group1 );

    // initialize publisher
    MockNET_Publisher_ABC mockPublisher;
    MOCK_EXPECT( mockPublisher, Send ).at_least( 1 );

    // moves group2 under group1
    group2.OnReceiveKnowledgeGroupUpdate( msg, armies );

    // verify
    BOOST_CHECK_EQUAL( &group1, group2.GetParent() );
    mockPublisher.verify();
    MOCK_EXPECT( army, UnregisterKnowledgeGroup ).with( mock::same( groupArmy ) ).once();
    MIL_KnowledgeGroupType::Terminate();
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: FHD 2010-01-19:
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ReceiveChangeSuperiorKnowledgeGroupUnderArmy )
{
    MockArmy army;
    MOCK_EXPECT( army, GetID ).returns( 10u );
    MockKnowledgeGroupFactory mockKnowledgeGroupFactory;

    // Use previously define type
    const MIL_KnowledgeGroupType &kgType = *MIL_KnowledgeGroupType::FindType("TOTO");

    // register army sub knowledge group
    MOCK_EXPECT( army, RegisterKnowledgeGroup ).once();
    MIL_KnowledgeGroup groupArmy( kgType, 10, army );

    // register 1st sub knowledge group
    xml::xistringstream xis2( "<root id='12' type='TOTO' name='toto12'/>" );
    xis2 >> xml::start( "root" );
//    MOCK_EXPECT( groupArmy, RegisterKnowledgeGroup ).once(); // $$$$ _RC_ SBO 2010-04-27: TODO: check registration into parent KG
    MIL_KnowledgeGroup group1( xis2, army, &groupArmy, mockKnowledgeGroupFactory );
//    groupArmy.verify();

    // register 2nd sub knowledge group
    xml::xistringstream xis3( "<root id='13' type='TOTO' name='toto13'/>" );
    xis3 >> xml::start( "root" );
//    MOCK_EXPECT( group1, RegisterKnowledgeGroup ).once(); // $$$$ _RC_ SBO 2010-04-27: TODO: check registration into parent KG
    MIL_KnowledgeGroup group2( xis3, army, &group1, mockKnowledgeGroupFactory );
//    group1.verify();

    sword::KnowledgeMagicAction msg;
    msg.mutable_knowledge_group()->set_id( group2.GetId() );
    msg.set_type( sword::KnowledgeMagicAction::update_party );
    msg.mutable_parameters()->add_elem()->mutable_value()->Add()->mutable_party()->set_id( army.GetID() );

    tools::Resolver< MIL_Army_ABC > armies;
    armies.Register( army.GetID(), army );

//    MIL_KnowledgeGroup* ptr_null = 0;
//    MOCK_EXPECT( army, FindKnowledgeGroup ).once().returns( ptr_null ); // $$$$ _RC_ SBO 2010-04-27: never verify'ed

    // initialize publisher
    MockNET_Publisher_ABC mockPublisher;
    MOCK_EXPECT( mockPublisher, Send ).at_least( 1 );

    // moves group2 under army
//    MOCK_EXPECT( army, UnregisterKnowledgeGroup ).with( mock::same( group2 ) ).once(); // $$$$ _RC_ SBO 2010-04-28: TODO: check unregistration from parent KG
    MOCK_EXPECT( army, RegisterKnowledgeGroup ).with( mock::same( group2 ) ).once();
    group2.OnReceiveKnowledgeGroupUpdate( msg, armies );
    army.verify();
    mockPublisher.verify();

    // verify
    BOOST_CHECK( !group2.GetParent() );
    MOCK_EXPECT( army, UnregisterKnowledgeGroup ).with( mock::same( group2 ) ).once();
    MOCK_EXPECT( army, UnregisterKnowledgeGroup ).with( mock::same( groupArmy ) ).once();
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: FHD 2010-01-19:
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ReceiveChangeSuperiorArmyUnderKnowledgeGroup )
{
    MockArmy army;
    MOCK_EXPECT( army, GetID ).returns( 20u );
    MockKnowledgeGroupFactory mockKnowledgeGroupFactory;

    // Use previously define type
    const MIL_KnowledgeGroupType &kgType = *MIL_KnowledgeGroupType::FindType("TOTO");

    // register army sub knowledge group
    MOCK_EXPECT( army, RegisterKnowledgeGroup ).once();
    MIL_KnowledgeGroup groupArmy( kgType, 20, army );

    // register 1st sub knowledge group
    xml::xistringstream xis2( "<root id='21' type='TOTO' name='toto21'/>" );
    xis2 >> xml::start( "root" );
//    MOCK_EXPECT( groupArmy, RegisterKnowledgeGroup ).once(); // $$$$ _RC_ SBO 2010-04-27: TODO: check registration into parent KG
    MIL_KnowledgeGroup group1( xis2, army, &groupArmy, mockKnowledgeGroupFactory );
//    groupArmy.verify();

    // register 2nd sub knowledge group
    xml::xistringstream xis3( "<root id='22' type='TOTO' name='toto22'/>" );
    xis3 >> xml::start( "root" );
    MOCK_EXPECT( army, RegisterKnowledgeGroup ).once();
    MIL_KnowledgeGroup group2( xis3, army, 0, mockKnowledgeGroupFactory );
    army.verify();

    sword::KnowledgeMagicAction msg;
    msg.mutable_knowledge_group()->set_id( group2.GetId() );
    msg.set_type( sword::KnowledgeMagicAction::update_party_parent );

    msg.mutable_parameters()->add_elem()->mutable_value()->Add()->mutable_party()->set_id( army.GetID() );
    msg.mutable_parameters()->add_elem()->mutable_value()->Add()->mutable_knowledgegroup()->set_id( group1.GetId() );

    tools::Resolver< MIL_Army_ABC > armies;
    armies.Register( army.GetID(), army );

    MOCK_EXPECT( army, FindKnowledgeGroup ).once().returns( &group1 );

    // initialize publisher
    MockNET_Publisher_ABC mockPublisher;
    MOCK_EXPECT( mockPublisher, Send ).at_least( 1 );

    // moves group2 under group1
    MOCK_EXPECT( army, UnregisterKnowledgeGroup ).with( mock::same( group2 ) ).once();
//    MOCK_EXPECT( group1, RegisterKnowledgeGroup ).with( mock::same( group2 ) ).once(); // $$$$ _RC_ SBO 2010-04-27: TODO: check registration into parent KG
    group2.OnReceiveKnowledgeGroupUpdate( msg, armies );
    army.verify();

    // verify
    BOOST_CHECK_EQUAL( &group1, group2.GetParent() );
    mockPublisher.verify();
    MOCK_EXPECT( army, UnregisterKnowledgeGroup ).with( mock::same( groupArmy ) ).once();
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
    xml::xistringstream xis(
        "<knowledge-groups>"
        "    <knowledge-group name='Standard' communication-delay='01m'>"
        "        <unit-knowledge max-lifetime='03h' max-unit-to-knowledge-distance='60000'/>"
        "        <population-knowledge max-lifetime='2m'/>"
        "    </knowledge-group>"
        "</knowledge-groups>" );
    double timeFactor = 0.5f;
    MIL_KnowledgeGroupType::InitializeWithTime(xis, timeFactor);

    // register army sub knowledge group
    const MIL_KnowledgeGroupType &kgType = *MIL_KnowledgeGroupType::FindType( "Standard" );
    MOCK_EXPECT( army, RegisterKnowledgeGroup ).once();
    MIL_KnowledgeGroup groupArmy( kgType, 30, army );

    // prepare message
    sword::KnowledgeMagicAction msg;
    msg.mutable_knowledge_group()->set_id( groupArmy.GetId() );
    msg.set_type( sword::KnowledgeMagicAction::update_type );
    const MIL_KnowledgeGroupType &kgType_new = *MIL_KnowledgeGroupType::FindType( "TOTO" );
    msg.mutable_parameters()->add_elem()->mutable_value()->Add()->set_acharstr( kgType_new.GetName().c_str() );

    tools::Resolver< MIL_Army_ABC > armies;
    MOCK_EXPECT( army, GetID ).once().returns( 1u );
    armies.Register( army.GetID(), army );

    // initialize publisher
    MockNET_Publisher_ABC mockPublisher;
    MOCK_EXPECT( mockPublisher, Send ).at_least( 1 );

    // change knowledge group type
    groupArmy.OnReceiveKnowledgeGroupUpdate( msg, armies );

    // verify
    BOOST_CHECK_EQUAL( "TOTO", groupArmy.GetType().GetName().c_str() );
    mockPublisher.verify();
    MOCK_EXPECT( army, UnregisterKnowledgeGroup ).with( mock::same( groupArmy ) ).once();
    MIL_KnowledgeGroupType::Terminate();
}

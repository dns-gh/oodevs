// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_test_pch.h"
#include "Helpers.h"
#include "MockAutomat.h"
#include "MockClientPublisher.h"
#include "MockFormation.h"
#include "MockKnowledgeGroup.h"
#include "MockModel.h"
#include "MockSide.h"
#include "dispatcher/Automat.h"
#include "protocol/ClientSenders.h"

// -----------------------------------------------------------------------------
// Name: Automat_CanBeUnderAFormation
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Automat_CanBeUnderAFormation )
{
    // sides
    tools::Resolver< dispatcher::Team_ABC > sides;
    MockSide side( 2 );
    MOCK_EXPECT( side, GetId ).returns( 2 );
    sides.Register( side.GetId(), side );

    // formations
    tools::Resolver< dispatcher::Formation_ABC > formations;
    MockFormation formation( 52 );
    MOCK_EXPECT( formation, GetId ).returns( 52 );
    formations.Register( formation.GetId(), formation );

    // automats
    tools::Resolver< dispatcher::Automat_ABC > automats;
    MockAutomat automat( 51 );
    MOCK_EXPECT( automat, GetId ).returns( 51 );
    automats.Register( automat.GetId(), automat );

    // knowledge groups
    tools::Resolver< dispatcher::KnowledgeGroup_ABC > knowledgeGroups;
    MockKnowledgeGroup knowledgeGroup( 3 );
    MOCK_EXPECT( knowledgeGroup, GetId ).returns( 3 );
    knowledgeGroups.Register( knowledgeGroup.GetId(), knowledgeGroup );

    MockModel model;
    MOCK_EXPECT( model, Sides ).returns( boost::ref( sides ) );
    MOCK_EXPECT( model, Formations ).returns( boost::ref( formations ) );
    MOCK_EXPECT( model, Automats ).returns( boost::ref( automats ) );
    MOCK_EXPECT( model, KnowledgeGroups ).returns( boost::ref( knowledgeGroups ) );
    {
        sword::SimToClient expected;
        expected.set_context( 0 );
        sword::AutomatCreation& message = *expected.mutable_message()->mutable_automat_creation();
        message.mutable_automat()->set_id( 1 );
        message.mutable_type()->set_id( 42 );
        message.set_name( "test" );
        message.mutable_parent()->mutable_formation()->set_id( formation.GetId() );
        message.mutable_party()->set_id( side.GetId() );
        message.mutable_knowledge_group()->set_id( knowledgeGroup.GetId() );
        message.set_logistic_level( sword::none );
        message.set_app6symbol( "sfgpu----------" );
        CheckInitialized( message );
        
        // creation
        MOCK_EXPECT( formation, RegisterAutomat ).once();
        MOCK_EXPECT( knowledgeGroup, RegisterAutomat ).once();
        dispatcher::Automat result( model, message );

        // network serialization
        MockClientPublisher publisher;
        MOCK_EXPECT( publisher, SendSimToClient ).once().with( expected );
        result.SendCreation( publisher );
        publisher.verify();

        // cleaning
        MOCK_EXPECT( formation, RemoveAutomat ).once();
        MOCK_EXPECT( knowledgeGroup, RemoveAutomat ).once();
    }
}

// -----------------------------------------------------------------------------
// Name: Automat_CanBeUnderAnAutomat
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Automat_CanBeUnderAnAutomat )
{
    // sides
    tools::Resolver< dispatcher::Team_ABC > sides;
    MockSide side( 2 );
    MOCK_EXPECT( side, GetId ).returns( 2 );
    sides.Register( side.GetId(), side );

    // automats
    tools::Resolver< dispatcher::Automat_ABC > automats;
    MockAutomat automat( 51 );
    MOCK_EXPECT( automat, GetId ).returns( 51 );
    automats.Register( automat.GetId(), automat );

    // formations
    tools::Resolver< dispatcher::Formation_ABC > formations;
    MockFormation formation( 52 );
    MOCK_EXPECT( formation, GetId ).returns( 52 );
    formations.Register( formation.GetId(), formation );

    // knowledge groups
    tools::Resolver< dispatcher::KnowledgeGroup_ABC > knowledgeGroups;
    MockKnowledgeGroup knowledgeGroup( 3 );
    MOCK_EXPECT( knowledgeGroup, GetId ).returns( 3 );
    knowledgeGroups.Register( knowledgeGroup.GetId(), knowledgeGroup );

    MockModel model;
    MOCK_EXPECT( model, Sides ).returns( boost::ref( sides ) );
    MOCK_EXPECT( model, Automats ).returns( boost::ref( automats ) );
    MOCK_EXPECT( model, Formations ).returns( boost::ref( formations ) );
    MOCK_EXPECT( model, KnowledgeGroups ).returns( boost::ref( knowledgeGroups ) );
    {
        sword::SimToClient expected;
        expected.set_context( 0 );
        sword::AutomatCreation& message = *expected.mutable_message()->mutable_automat_creation();
        message.mutable_automat()->set_id( 1 );
        message.mutable_type()->set_id( 42 );
        message.set_name( "test" );
        message.mutable_parent()->mutable_automat()->set_id( automat.GetId() );
        message.mutable_party()->set_id( side.GetId() );
        message.mutable_knowledge_group()->set_id( knowledgeGroup.GetId() );
        message.set_logistic_level( sword::none );
        message.set_app6symbol( "sfgpu----------" );
        CheckInitialized( message );

        // creation
        MOCK_EXPECT( automat, RegisterAutomat ).once();
        MOCK_EXPECT( knowledgeGroup, RegisterAutomat ).once();
        dispatcher::Automat result( model, message );

        // network serialization
        MockClientPublisher publisher;
        MOCK_EXPECT( publisher, SendSimToClient ).once().with( expected );
        result.SendCreation( publisher );
        publisher.verify();

        // cleaning
        MOCK_EXPECT( automat, RemoveAutomat ).once();
        MOCK_EXPECT( knowledgeGroup, RemoveAutomat ).once();
    }
}

// -----------------------------------------------------------------------------
// Name: Automat_SuperiorCanBeChanged
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Automat_SuperiorCanBeChanged )
{
    // sides
    tools::Resolver< dispatcher::Team_ABC > sides;
    MockSide side( 2 );
    MOCK_EXPECT( side, GetId ).returns( 2 );
    sides.Register( side.GetId(), side );

    // formations
    tools::Resolver< dispatcher::Formation_ABC > formations;
    MockFormation formation( 52 );
    MOCK_EXPECT( formation, GetId ).returns( 52 );
    formations.Register( formation.GetId(), formation );

    // automats
    tools::Resolver< dispatcher::Automat_ABC > automats;
    MockAutomat automat( 51 );
    MOCK_EXPECT( automat, GetId ).returns( 51 );
    automats.Register( automat.GetId(), automat );

    // knowledge groups
    tools::Resolver< dispatcher::KnowledgeGroup_ABC > knowledgeGroups;
    MockKnowledgeGroup knowledgeGroup( 3 );
    MOCK_EXPECT( knowledgeGroup, GetId ).returns( 3 );
    knowledgeGroups.Register( knowledgeGroup.GetId(), knowledgeGroup );

    MockModel model;
    MOCK_EXPECT( model, Sides ).returns( boost::ref( sides ) );
    MOCK_EXPECT( model, Automats ).returns( boost::ref( automats ) );
    MOCK_EXPECT( model, Formations ).returns( boost::ref( formations ) );
    MOCK_EXPECT( model, KnowledgeGroups ).returns( boost::ref( knowledgeGroups ) );
    {
        std::auto_ptr< dispatcher::Automat > result;
        {
            // creation
            sword::AutomatCreation message;
            message.mutable_automat()->set_id( 1 );
            message.mutable_type()->set_id( 42 );
            message.set_name( "test" );
            message.mutable_parent()->mutable_automat()->set_id( automat.GetId() );
            message.mutable_party()->set_id( side.GetId() );
            message.mutable_knowledge_group()->set_id( knowledgeGroup.GetId() );
            message.set_logistic_level( sword::none );
            message.set_app6symbol( "sfgpu----------" );
            CheckInitialized( message );

            MOCK_EXPECT( automat, RegisterAutomat ).once();
            MOCK_EXPECT( knowledgeGroup, RegisterAutomat ).once();
            result.reset( new dispatcher::Automat( model, message ) );
            automats.Register( result->GetId(), *result );
        }
        {
            // change superior
            sword::AutomatChangeSuperior message;
            message.mutable_automat()->set_id( 1 );
            message.mutable_superior()->mutable_formation()->set_id( 52 );
            CheckInitialized( message );

            MOCK_EXPECT( automat, RemoveAutomat ).once();
            MOCK_EXPECT( formation, RegisterAutomat ).once();
            automats.Get( 1 ).Update( message );
            automat.verify();
            formation.verify();
        }

        // cleaning
        MOCK_EXPECT( formation, RemoveAutomat ).once();
        MOCK_EXPECT( knowledgeGroup, RemoveAutomat ).once();
    }
}

// -----------------------------------------------------------------------------
// Name: Automat_KnowledgeGroupCanBeChanged
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Automat_KnowledgeGroupCanBeChanged )
{
    // sides
    tools::Resolver< dispatcher::Team_ABC > sides;
    MockSide side( 2 );
    MOCK_EXPECT( side, GetId ).returns( 2 );
    sides.Register( side.GetId(), side );

    // automats
    tools::Resolver< dispatcher::Automat_ABC > automats;
    MockAutomat automat( 51 );
    MOCK_EXPECT( automat, GetId ).returns( 51 );
    automats.Register( automat.GetId(), automat );

    // formations
    tools::Resolver< dispatcher::Formation_ABC > formations;
    MockFormation formation( 52 );
    MOCK_EXPECT( formation, GetId ).returns( 52 );
    formations.Register( formation.GetId(), formation );

    // knowledge groups
    tools::Resolver< dispatcher::KnowledgeGroup_ABC > knowledgeGroups;
    MockKnowledgeGroup knowledgeGroup1( 3 );
    MOCK_EXPECT( knowledgeGroup1, GetId ).returns( 3 );
    knowledgeGroups.Register( knowledgeGroup1.GetId(), knowledgeGroup1 );
    MockKnowledgeGroup knowledgeGroup2( 4 );
    MOCK_EXPECT( knowledgeGroup2, GetId ).returns( 4 );
    knowledgeGroups.Register( knowledgeGroup2.GetId(), knowledgeGroup2 );

    MockModel model;
    MOCK_EXPECT( model, Sides ).returns( boost::ref( sides ) );
    MOCK_EXPECT( model, Automats ).returns( boost::ref( automats ) );
    MOCK_EXPECT( model, Formations ).returns( boost::ref( formations ) );
    MOCK_EXPECT( model, KnowledgeGroups ).returns( boost::ref( knowledgeGroups ) );
    {
        std::auto_ptr< dispatcher::Automat > result;
        {
            // creation
            sword::AutomatCreation message;
            message.mutable_automat()->set_id( 1 );
            message.mutable_type()->set_id( 42 );
            message.set_name( "test" );
            message.mutable_parent()->mutable_automat()->set_id( automat.GetId() );
            message.mutable_party()->set_id( side.GetId() );
            message.mutable_knowledge_group()->set_id( knowledgeGroup1.GetId() );
            message.set_logistic_level( sword::none );
            message.set_app6symbol( "sfgpu----------" );
            CheckInitialized( message );

            MOCK_EXPECT( automat, RegisterAutomat ).once();
            MOCK_EXPECT( knowledgeGroup1, RegisterAutomat ).once();
            result.reset( new dispatcher::Automat( model, message ) );
            automats.Register( result->GetId(), *result );
        }
        {
            // change knowledge group
            sword::AutomatChangeKnowledgeGroup message;
            message.mutable_automat()->set_id( 1 );
            message.mutable_party()->set_id( side.GetId() );
            message.mutable_knowledge_group()->set_id( knowledgeGroup2.GetId() );
            CheckInitialized( message );

            MOCK_EXPECT( knowledgeGroup1, RemoveAutomat ).once();
            MOCK_EXPECT( knowledgeGroup2, RegisterAutomat ).once();
            automats.Get( 1 ).Update( message );
            knowledgeGroup1.verify();
            knowledgeGroup2.verify();
        }

        // cleaning
        MOCK_EXPECT( automat, RemoveAutomat ).once();
        MOCK_EXPECT( knowledgeGroup2, RemoveAutomat ).once();
    }
}

// -----------------------------------------------------------------------------
// Name: Automat_DecisionalStateCanBeChanged
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Automat_DecisionalStateCanBeChanged )
{
    // sides
    tools::Resolver< dispatcher::Team_ABC > sides;
    MockSide side( 2 );
    MOCK_EXPECT( side, GetId ).returns( 2 );
    sides.Register( side.GetId(), side );

    // automats
    tools::Resolver< dispatcher::Automat_ABC > automats;
    MockAutomat automat( 51 );
    MOCK_EXPECT( automat, GetId ).returns( 51 );
    automats.Register( automat.GetId(), automat );

    // formations
    tools::Resolver< dispatcher::Formation_ABC > formations;
    MockFormation formation( 52 );
    MOCK_EXPECT( formation, GetId ).returns( 52 );
    formations.Register( formation.GetId(), formation );

    // knowledge groups
    tools::Resolver< dispatcher::KnowledgeGroup_ABC > knowledgeGroups;
    MockKnowledgeGroup knowledgeGroup( 3 );
    MOCK_EXPECT( knowledgeGroup, GetId ).returns( 3 );
    knowledgeGroups.Register( knowledgeGroup.GetId(), knowledgeGroup );

    MockModel model;
    MOCK_EXPECT( model, Sides ).returns( boost::ref( sides ) );
    MOCK_EXPECT( model, Automats ).returns( boost::ref( automats ) );
    MOCK_EXPECT( model, Formations ).returns( boost::ref( formations ) );
    MOCK_EXPECT( model, KnowledgeGroups ).returns( boost::ref( knowledgeGroups ) );
    {
        std::auto_ptr< dispatcher::Automat > result;
        {
            // creation
            sword::AutomatCreation message;
            message.mutable_automat()->set_id( 1 );
            message.mutable_type()->set_id( 42 );
            message.set_name( "test" );
            message.mutable_parent()->mutable_automat()->set_id( automat.GetId() );
            message.mutable_party()->set_id( side.GetId() );
            message.mutable_knowledge_group()->set_id( knowledgeGroup.GetId() );
            message.set_logistic_level( sword::none );
            message.set_app6symbol( "sfgpu----------" );
            CheckInitialized( message );

            MOCK_EXPECT( automat, RegisterAutomat ).once();
            MOCK_EXPECT( knowledgeGroup, RegisterAutomat ).once();
            result.reset( new dispatcher::Automat( model, message ) );
            automats.Register( result->GetId(), *result );
        }
        {
            // change decisional state
            sword::SimToClient expected;
            expected.set_context( 0 );
            sword::DecisionalState& message = *expected.mutable_message()->mutable_decisional_state();
            message.mutable_source()->mutable_automat()->set_id( 1 );
            message.set_key( "my variable" );
            message.set_value( "my value" );
            CheckInitialized( message );
            automats.Get( 1 ).Update( message );

            // network serialization
            MockClientPublisher publisher;
            MOCK_EXPECT( publisher, SendSimToClient ).exactly( 5 ); // TODO! AutomatAttributes, AutomatChangeKnowledgeGroup, AutomatChangeSuperior, AutomatChangeLogisticLinks, AutomatOrder
            MOCK_EXPECT( publisher, SendSimToClient ).once().with( expected );
            result->SendFullUpdate( publisher );
            publisher.verify();
        }

        // cleaning
        MOCK_EXPECT( automat, RemoveAutomat ).once();
        MOCK_EXPECT( knowledgeGroup, RemoveAutomat ).once();
    }
}

// -----------------------------------------------------------------------------
// Name: Automat_AttributesCanBeChanged
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Automat_AttributesCanBeChanged )
{
    // sides
    tools::Resolver< dispatcher::Team_ABC > sides;
    MockSide side( 2 );
    MOCK_EXPECT( side, GetId ).returns( 2 );
    sides.Register( side.GetId(), side );

    // automats
    tools::Resolver< dispatcher::Automat_ABC > automats;
    MockAutomat automat( 51 );
    MOCK_EXPECT( automat, GetId ).returns( 51 );
    automats.Register( automat.GetId(), automat );

    // formations
    tools::Resolver< dispatcher::Formation_ABC > formations;
    MockFormation formation( 52 );
    MOCK_EXPECT( formation, GetId ).returns( 52 );
    formations.Register( formation.GetId(), formation );

    // knowledge groups
    tools::Resolver< dispatcher::KnowledgeGroup_ABC > knowledgeGroups;
    MockKnowledgeGroup knowledgeGroup( 3 );
    MOCK_EXPECT( knowledgeGroup, GetId ).returns( 3 );
    knowledgeGroups.Register( knowledgeGroup.GetId(), knowledgeGroup );

    MockModel model;
    MOCK_EXPECT( model, Sides ).returns( boost::ref( sides ) );
    MOCK_EXPECT( model, Automats ).returns( boost::ref( automats ) );
    MOCK_EXPECT( model, Formations ).returns( boost::ref( formations ) );
    MOCK_EXPECT( model, KnowledgeGroups ).returns( boost::ref( knowledgeGroups ) );
    {
        std::auto_ptr< dispatcher::Automat > result;
        {
            // creation
            sword::AutomatCreation message;
            message.mutable_automat()->set_id( 1 );
            message.mutable_type()->set_id( 42 );
            message.set_name( "test" );
            message.mutable_parent()->mutable_automat()->set_id( automat.GetId() );
            message.mutable_party()->set_id( side.GetId() );
            message.mutable_knowledge_group()->set_id( knowledgeGroup.GetId() );
            message.set_logistic_level( sword::none );
            message.set_app6symbol( "sfgpu----------" );
            CheckInitialized( message );

            MOCK_EXPECT( automat, RegisterAutomat ).once();
            MOCK_EXPECT( knowledgeGroup, RegisterAutomat ).once();
            result.reset( new dispatcher::Automat( model, message ) );
            automats.Register( result->GetId(), *result );
        }
        {
            // change attributes
            sword::SimToClient expected;
            expected.set_context( 0 );
            sword::AutomatAttributes& message = *expected.mutable_message()->mutable_automat_attributes();
            message.mutable_automat()->set_id( 1 );
            message.set_etat_automate( sword::debraye );
            message.set_force_ratio( sword::ForceRatio::favorable );
            message.set_meeting_engagement( sword::etat_esquive );
            message.set_operational_state( sword::detruit_tactiquement );
            message.set_roe( sword::RulesOfEngagement::retaliation_only );
            CheckInitialized( message );
            automats.Get( 1 ).Update( message );

            // network serialization
            MockClientPublisher publisher;
            MOCK_EXPECT( publisher, SendSimToClient ).once().with( expected );
            MOCK_EXPECT( publisher, SendSimToClient ).exactly( 4 ); // TODO! AutomatChangeKnowledgeGroup, AutomatChangeSuperior, AutomatChangeLogisticLinks, AutomatOrder
            // no DecisionalStates if none is set
            result->SendFullUpdate( publisher );
            publisher.verify();
        }

        // cleaning
        MOCK_EXPECT( automat, RemoveAutomat ).once();
        MOCK_EXPECT( knowledgeGroup, RemoveAutomat ).once();
    }
}

// -----------------------------------------------------------------------------
// Name: Automat_LogSupplyQuotasCanBeChanged
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Automat_LogSupplyQuotasCanBeChanged )
{
    // sides
    tools::Resolver< dispatcher::Team_ABC > sides;
    MockSide side( 2 );
    MOCK_EXPECT( side, GetId ).returns( 2 );
    sides.Register( side.GetId(), side );

    // automats
    tools::Resolver< dispatcher::Automat_ABC > automats;
    MockAutomat automat( 51 );
    MOCK_EXPECT( automat, GetId ).returns( 51 );
    automats.Register( automat.GetId(), automat );

    // formations
    tools::Resolver< dispatcher::Formation_ABC > formations;
    MockFormation formation( 52 );
    MOCK_EXPECT( formation, GetId ).returns( 52 );
    formations.Register( formation.GetId(), formation );

    // knowledge groups
    tools::Resolver< dispatcher::KnowledgeGroup_ABC > knowledgeGroups;
    MockKnowledgeGroup knowledgeGroup( 3 );
    MOCK_EXPECT( knowledgeGroup, GetId ).returns( 3 );
    knowledgeGroups.Register( knowledgeGroup.GetId(), knowledgeGroup );

    MockModel model;
    MOCK_EXPECT( model, Sides ).returns( boost::ref( sides ) );
    MOCK_EXPECT( model, Automats ).returns( boost::ref( automats ) );
    MOCK_EXPECT( model, Formations ).returns( boost::ref( formations ) );
    MOCK_EXPECT( model, KnowledgeGroups ).returns( boost::ref( knowledgeGroups ) );
    {
        std::auto_ptr< dispatcher::Automat > result;
        {
            // creation
            sword::AutomatCreation message;
            message.mutable_automat()->set_id( 1 );
            message.mutable_type()->set_id( 42 );
            message.set_name( "test" );
            message.mutable_parent()->mutable_automat()->set_id( automat.GetId() );
            message.mutable_party()->set_id( side.GetId() );
            message.mutable_knowledge_group()->set_id( knowledgeGroup.GetId() );
            message.set_logistic_level( sword::tc2 );
            message.set_app6symbol( "sfgpu----------" );
            CheckInitialized( message );

            MOCK_EXPECT( automat, RegisterAutomat ).once();
            MOCK_EXPECT( knowledgeGroup, RegisterAutomat ).once();
            result.reset( new dispatcher::Automat( model, message ) );
            automats.Register( result->GetId(), *result );
        }
        {
            // change log supply quotas
            sword::SimToClient expected;
            expected.set_context( 0 );
            sword::LogSupplyQuotas& message = *expected.mutable_message()->mutable_log_supply_quotas();
            message.mutable_supplied()->mutable_automat()->set_id( 1 );
            message.mutable_quotas()->add_elem();
            message.mutable_quotas()->mutable_elem( 0 )->mutable_ressource_id()->set_id( 42 );
            message.mutable_quotas()->mutable_elem( 0 )->set_quota_disponible( 5112 );
            message.mutable_quotas()->add_elem();
            message.mutable_quotas()->mutable_elem( 1 )->mutable_ressource_id()->set_id( 69 );
            message.mutable_quotas()->mutable_elem( 1 )->set_quota_disponible( 6945 );
            CheckInitialized( message );
            automats.Get( 1 ).Update( message );

            // network serialization
            MockClientPublisher publisher;
            MOCK_EXPECT( publisher, SendSimToClient ).once(); // TODO! AutomatCreation
            MOCK_EXPECT( publisher, SendSimToClient ).once().with( expected );
            result->SendCreation( publisher );
            publisher.verify();
        }

        // cleaning
        MOCK_EXPECT( automat, RemoveAutomat ).once();
        MOCK_EXPECT( knowledgeGroup, RemoveAutomat ).once();
    }
}

// -----------------------------------------------------------------------------
// Name: Automat_LogisticLinksCanBeChanged
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Automat_LogisticLinksCanBeChanged )
{
    // sides
    tools::Resolver< dispatcher::Team_ABC > sides;
    MockSide side( 2 );
    MOCK_EXPECT( side, GetId ).returns( 2 );
    sides.Register( side.GetId(), side );

    // automats
    tools::Resolver< dispatcher::Automat_ABC > automats;
    MockAutomat automat( 51 );
    MOCK_EXPECT( automat, GetId ).returns( 51 );
    automats.Register( automat.GetId(), automat );

    // formations
    tools::Resolver< dispatcher::Formation_ABC > formations;
    MockFormation formation( 52 );
    MOCK_EXPECT( formation, GetId ).returns( 52 );
    formations.Register( formation.GetId(), formation );

    // knowledge groups
    tools::Resolver< dispatcher::KnowledgeGroup_ABC > knowledgeGroups;
    MockKnowledgeGroup knowledgeGroup( 3 );
    MOCK_EXPECT( knowledgeGroup, GetId ).returns( 3 );
    knowledgeGroups.Register( knowledgeGroup.GetId(), knowledgeGroup );

    MockModel model;
    MOCK_EXPECT( model, Sides ).returns( boost::ref( sides ) );
    MOCK_EXPECT( model, Automats ).returns( boost::ref( automats ) );
    MOCK_EXPECT( model, Formations ).returns( boost::ref( formations ) );
    MOCK_EXPECT( model, KnowledgeGroups ).returns( boost::ref( knowledgeGroups ) );
    {
        std::auto_ptr< dispatcher::Automat > result;
        {
            // creation
            sword::AutomatCreation message;
            message.mutable_automat()->set_id( 1 );
            message.mutable_type()->set_id( 42 );
            message.set_name( "test" );
            message.mutable_parent()->mutable_automat()->set_id( automat.GetId() );
            message.mutable_party()->set_id( side.GetId() );
            message.mutable_knowledge_group()->set_id( knowledgeGroup.GetId() );
            message.set_logistic_level( sword::none );
            message.set_app6symbol( "sfgpu----------" );
            CheckInitialized( message );

            MOCK_EXPECT( automat, RegisterAutomat ).once();
            MOCK_EXPECT( knowledgeGroup, RegisterAutomat ).once();
            result.reset( new dispatcher::Automat( model, message ) );
            automats.Register( result->GetId(), *result );
        }
        {
            // change logistic links
            sword::SimToClient expected;
            expected.set_context( 0 );
            sword::ChangeLogisticLinks& message = *expected.mutable_message()->mutable_automat_change_logistic_links();
            message.mutable_requester()->mutable_automat()->set_id( 1 );
            message.mutable_tc2()->set_id( 1 );
            message.mutable_logistic_base()->mutable_automat()->set_id( 1 );
            CheckInitialized( message );
            automats.Get( 1 ).Update( message );

            // network serialization
            MockClientPublisher publisher;
            MOCK_EXPECT( publisher, SendSimToClient ).exactly( 3 ); // TODO! AutomatAttributes, AutomatChangeKnowledgeGroup, AutomatChangeSuperior
            MOCK_EXPECT( publisher, SendSimToClient ).once().with( expected );
            MOCK_EXPECT( publisher, SendSimToClient ).once(); // TODO! AutomatOrder (no DecisionalStates if none is set)
            result->SendFullUpdate( publisher );
            publisher.verify();
        }

        // cleaning
        MOCK_EXPECT( automat, RemoveAutomat ).once();
        MOCK_EXPECT( knowledgeGroup, RemoveAutomat ).once();
    }
}

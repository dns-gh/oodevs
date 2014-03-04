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
#include "MockLogisticEntity.h"
#include "dispatcher/Automat.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/AutomatType.h"
#include "protocol/ClientSenders.h"

namespace
{
    struct Fixture
    {
        Fixture()
            : side( 2 )
            , formation( 52 )
            , automat( 51 )
            , knowledgeGroup( 3 )
            , type( staticModel.MakeAutomatType( symbolFactory ) )
        {
            types.Register( type->GetId(), *type );

            // sides
            MOCK_EXPECT( side.GetId ).returns( 2 );
            sides.Register( side.GetId(), side );

            // formations
            MOCK_EXPECT( formation.GetId ).returns( 52 );
            formations.Register( formation.GetId(), formation );

            // automats
            MOCK_EXPECT( automat.GetId ).returns( 51 );
            automats.Register( automat.GetId(), automat );

            // knowledge groups
            MOCK_EXPECT( knowledgeGroup.GetId ).returns( 3 );
            knowledgeGroups.Register( knowledgeGroup.GetId(), knowledgeGroup );

            MOCK_EXPECT( model.Sides ).returns( boost::ref( sides ) );
            MOCK_EXPECT( model.Formations ).returns( boost::ref( formations ) );
            MOCK_EXPECT( model.Automats ).returns( boost::ref( automats ) );
            MOCK_EXPECT( model.KnowledgeGroups ).returns( boost::ref( knowledgeGroups ) );
        }

        dispatcher_test::StaticModel staticModel;
        kernel::SymbolFactory symbolFactory;

        // automat types
        tools::Resolver< kernel::AutomatType > types;
        boost::shared_ptr< kernel::AutomatType > type;

        // sides
        tools::Resolver< dispatcher::Team_ABC > sides;
        MockSide side;

        // formations
        tools::Resolver< dispatcher::Formation_ABC > formations;
        MockFormation formation;

        // automats
        tools::Resolver< dispatcher::Automat_ABC > automats;
        MockAutomat automat;

        // knowledge groups
        tools::Resolver< dispatcher::KnowledgeGroup_ABC > knowledgeGroups;
        MockKnowledgeGroup knowledgeGroup;
        MockModel model;
    };
}

// -----------------------------------------------------------------------------
// Name: Automat_CanBeUnderAFormation
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Automat_CanBeUnderAFormation, Fixture )
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
    message.set_symbol( "sfgpu----------" );
    message.set_log_maintenance_manual( false );
    message.set_log_supply_manual( false );
    BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

    // creation
    MOCK_EXPECT( formation.RegisterAutomat ).once();
    MOCK_EXPECT( knowledgeGroup.RegisterAutomat ).once();
    dispatcher::Automat result( model, message, types );

    // network serialization
    MockClientPublisher publisher;
    MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
    result.SendCreation( publisher );
    mock::verify( publisher );

    // cleaning
    MOCK_EXPECT( formation.RemoveAutomat ).once();
    MOCK_EXPECT( knowledgeGroup.RemoveAutomat ).once();
}

// -----------------------------------------------------------------------------
// Name: Automat_CanBeUnderAnAutomat
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Automat_CanBeUnderAnAutomat, Fixture )
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
    message.set_symbol( "sfgpu----------" );
    message.set_log_maintenance_manual( false );
    message.set_log_supply_manual( false );
    BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

    // creation
    MOCK_EXPECT( automat.RegisterAutomat ).once();
    MOCK_EXPECT( knowledgeGroup.RegisterAutomat ).once();
    dispatcher::Automat result( model, message, types );

    // network serialization
    MockClientPublisher publisher;
    MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
    result.SendCreation( publisher );
    mock::verify( publisher );

    // cleaning
    MOCK_EXPECT( automat.RemoveAutomat ).once();
    MOCK_EXPECT( knowledgeGroup.RemoveAutomat ).once();
}

// -----------------------------------------------------------------------------
// Name: Automat_SuperiorCanBeChanged
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Automat_SuperiorCanBeChanged, Fixture )
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
        message.set_symbol( "sfgpu----------" );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        MOCK_EXPECT( automat.RegisterAutomat ).once();
        MOCK_EXPECT( knowledgeGroup.RegisterAutomat ).once();
        result.reset( new dispatcher::Automat( model, message, types ) );
        automats.Register( result->GetId(), *result );
    }
    {
        // change superior
        sword::AutomatChangeSuperior message;
        message.mutable_automat()->set_id( 1 );
        message.mutable_superior()->mutable_formation()->set_id( 52 );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        MOCK_EXPECT( automat.RemoveAutomat ).once();
        MOCK_EXPECT( formation.RegisterAutomat ).once();
        automats.Get( 1 ).Update( message );
        mock::verify( automat );
        mock::verify( formation );
    }

    // cleaning
    MOCK_EXPECT( formation.RemoveAutomat ).once();
    MOCK_EXPECT( knowledgeGroup.RemoveAutomat ).once();
}

// -----------------------------------------------------------------------------
// Name: Automat_KnowledgeGroupCanBeChanged
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Automat_KnowledgeGroupCanBeChanged, Fixture )
{
    MockKnowledgeGroup knowledgeGroup2( 4 );
    MOCK_EXPECT( knowledgeGroup2.GetId ).returns( 4 );
    knowledgeGroups.Register( knowledgeGroup2.GetId(), knowledgeGroup2 );

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
        message.set_symbol( "sfgpu----------" );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        MOCK_EXPECT( automat.RegisterAutomat ).once();
        MOCK_EXPECT( knowledgeGroup.RegisterAutomat ).once();
        result.reset( new dispatcher::Automat( model, message, types ) );
        automats.Register( result->GetId(), *result );
    }
    {
        // change knowledge group
        sword::AutomatChangeKnowledgeGroup message;
        message.mutable_automat()->set_id( 1 );
        message.mutable_party()->set_id( side.GetId() );
        message.mutable_knowledge_group()->set_id( knowledgeGroup2.GetId() );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        MOCK_EXPECT( knowledgeGroup.RemoveAutomat ).once();
        MOCK_EXPECT( knowledgeGroup2.RegisterAutomat ).once();
        automats.Get( 1 ).Update( message );
        mock::verify( knowledgeGroup );
        mock::verify( knowledgeGroup2 );
    }

    // cleaning
    MOCK_EXPECT( automat.RemoveAutomat ).once();
    MOCK_EXPECT( knowledgeGroup2.RemoveAutomat ).once();
}

// -----------------------------------------------------------------------------
// Name: Automat_DecisionalStateCanBeChanged
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Automat_DecisionalStateCanBeChanged, Fixture )
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
        message.set_symbol( "sfgpu----------" );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        MOCK_EXPECT( automat.RegisterAutomat ).once();
        MOCK_EXPECT( knowledgeGroup.RegisterAutomat ).once();
        result.reset( new dispatcher::Automat( model, message, types ) );
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
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );
        automats.Get( 1 ).Update( message );

        // network serialization
        MockClientPublisher publisher;
        MOCK_EXPECT( publisher.SendSimToClient ).exactly( 2 ); // TODO! AutomatAttributes, AutomatOrder
        MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
        MOCK_EXPECT( publisher.SendSimToClient ).exactly( 1 ); // AutomatChangeLogisticLinks
        result->SendFullUpdate( publisher );
        mock::verify( publisher );
    }

    // cleaning
    MOCK_EXPECT( automat.RemoveAutomat ).once();
    MOCK_EXPECT( knowledgeGroup.RemoveAutomat ).once();
}

// -----------------------------------------------------------------------------
// Name: Automat_AttributesCanBeChanged
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Automat_AttributesCanBeChanged, Fixture )
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
        message.set_symbol( "sfgpu----------" );
        message.set_log_maintenance_manual( false );
        message.set_log_supply_manual( false );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        MOCK_EXPECT( automat.RegisterAutomat ).once();
        MOCK_EXPECT( knowledgeGroup.RegisterAutomat ).once();
        result.reset( new dispatcher::Automat( model, message, types ) );
        automats.Register( result->GetId(), *result );
    }
    {
        // change attributes
        sword::SimToClient expected;
        expected.set_context( 0 );
        sword::AutomatAttributes& message = *expected.mutable_message()->mutable_automat_attributes();
        message.mutable_automat()->set_id( 1 );
        message.set_mode( sword::disengaged );
        message.set_force_ratio( sword::ForceRatio::favorable );
        message.set_meeting_engagement( sword::avoiding );
        message.set_operational_state( sword::tactically_destroyed );
        message.set_roe( sword::RulesOfEngagement::retaliation_only );
        message.set_decisional_model( "brain" );
        message.set_brain_debug( false );
        message.set_log_maintenance_manual( false );
        message.set_log_supply_manual( false );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );
        automats.Get( 1 ).Update( message );

        // network serialization
        MockClientPublisher publisher;
        MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
        MOCK_EXPECT( publisher.SendSimToClient ).exactly( 2 ); // TODO! AutomatChangeLogisticLinks, AutomatOrder
        // no DecisionalStates if none is set
        result->SendFullUpdate( publisher );
        mock::verify( publisher );
    }

    // cleaning
    MOCK_EXPECT( automat.RemoveAutomat ).once();
    MOCK_EXPECT( knowledgeGroup.RemoveAutomat ).once();
}

namespace {

    struct ParentEntitySender
    {
        ParentEntitySender( int id ) : id_( id ) {}
        void operator()( sword::ParentEntity& msg ) const
        {
            msg.mutable_automat()->set_id( id_ );
        }
        int id_;
    };

}

// -----------------------------------------------------------------------------
// Name: Automat_LogSupplyQuotasCanBeChanged
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Automat_LogLinksAndSupplyQuotasCanBeChanged, Fixture )
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
        message.set_symbol( "sfgpu----------" );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        MOCK_EXPECT( automat.RegisterAutomat ).once();
        MOCK_EXPECT( knowledgeGroup.RegisterAutomat ).once();
        result.reset( new dispatcher::Automat( model, message, types ) );
        automats.Register( result->GetId(), *result );
    }
    {
        // Link
        sword::SimToClient expectedLink;
        expectedLink.set_context( 0 );
        sword::ChangeLogisticLinks& message = *expectedLink.mutable_message()->mutable_automat_change_logistic_links();
        message.mutable_requester()->mutable_automat()->set_id( 1 );
        message.add_superior()->mutable_automat()->set_id( 51 );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        MockLogisticEntity logisticEntity;
        MOCK_EXPECT( automat.GetLogisticEntity ).once().returns( &logisticEntity );
        automats.Get( 1 ).Update( message );

        // change log supply quotas
        sword::SimToClient expectedQuotas;
        expectedQuotas.set_context( 0 );
        sword::LogSupplyQuotas& message2 = *expectedQuotas.mutable_message()->mutable_log_supply_quotas();
        message2.mutable_supplied()->mutable_automat()->set_id( 1 );
        message2.mutable_supplier()->mutable_automat()->set_id( 51 );
        message2.mutable_quotas()->add_elem();
        message2.mutable_quotas()->mutable_elem( 0 )->mutable_resource()->set_id( 42 );
        message2.mutable_quotas()->mutable_elem( 0 )->set_quantity( 5112 );
        message2.mutable_quotas()->add_elem();
        message2.mutable_quotas()->mutable_elem( 1 )->mutable_resource()->set_id( 69 );
        message2.mutable_quotas()->mutable_elem( 1 )->set_quantity( 6945 );

        MOCK_EXPECT( automat.GetLogisticEntity ).once().returns( &logisticEntity );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        automats.Get( 1 ).Update( message2 );

        // network serialization
        MockClientPublisher publisher;
        MOCK_EXPECT( publisher.SendSimToClient ).exactly( 2 ); // TODO! AutomatChangeLogisticLinks, AutomatOrder
        MOCK_EXPECT( logisticEntity.Send ).exactly( 2 ).calls( ParentEntitySender( 51 ) ); // 1 for logistic links, 1 for quotas
        MOCK_EXPECT( publisher.SendSimToClient ).once().with( expectedLink );
        MOCK_EXPECT( publisher.SendSimToClient ).once().with( expectedQuotas );
        automats.Get( 1 ).SendFullUpdate( publisher );
        mock::verify( publisher );
    }

    // cleaning
    MOCK_EXPECT( automat.RemoveAutomat ).once();
    MOCK_EXPECT( knowledgeGroup.RemoveAutomat ).once();
}

// -----------------------------------------------------------------------------
// Name: Automat_CreationWithExtensions
// Created: AHC 2012-10-08
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Automat_CreationWithExtensions, Fixture )
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
    message.set_symbol( "sfgpu----------" );
    message.set_log_maintenance_manual( false );
    message.set_log_supply_manual( false );
    sword::Extension_Entry* entry = message.mutable_extension()->add_entries();
    entry->set_name( "extension" );
    entry->set_value( "value" );
    BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

    // creation
    MOCK_EXPECT( formation.RegisterAutomat ).once();
    MOCK_EXPECT( knowledgeGroup.RegisterAutomat ).once();
    dispatcher::Automat result( model, message, types );

    std::string extVal;
    BOOST_CHECK( result.GetExtension( "extension", extVal ) );
    BOOST_CHECK_EQUAL( extVal, "value" );

    // network serialization
    MockClientPublisher publisher;
    MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
    result.SendCreation( publisher );
    mock::verify( publisher );

    // cleaning
    MOCK_EXPECT( formation.RemoveAutomat ).once();
    MOCK_EXPECT( knowledgeGroup.RemoveAutomat ).once();
}

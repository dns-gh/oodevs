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
#include "MockClientPublisher.h"
#include "MockModel.h"
#include "MockSide.h"
#include "dispatcher/Formation.h"
#include "protocol/ClientSenders.h"

inline std::ostream& operator<<( std::ostream& os, const QString& s )
{
    return os << s.toStdString();
}

namespace
{
    struct Fixture
    {
    public:
        Fixture()
            : side( 2 )
        {
            MOCK_EXPECT( side.GetId ).returns( 2 );
            sides.Register( side.GetId(), side );
            MOCK_EXPECT( model.Sides ).returns( boost::ref( sides ) );
            MOCK_EXPECT( model.Formations ).returns( boost::ref( formations ) );
        }
        tools::Resolver< dispatcher::Team_ABC > sides;
        MockSide side;
        tools::Resolver< dispatcher::Formation_ABC > formations;
        MockModel model;
    };
}

// -----------------------------------------------------------------------------
// Name: Formation_CanBeCreated
// Created: SBO 2010-11-18
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Formation_CanBeCreated, Fixture )
{
    sword::SimToClient expected;
    expected.set_context( 0 );
    sword::FormationCreation& message = *expected.mutable_message()->mutable_formation_creation();
    message.mutable_formation()->set_id( 1 );
    message.mutable_party()->set_id( side.GetId() );
    message.set_level( sword::oo );
    message.set_name( "test" );
    message.set_app6symbol( "sfgpu----------" );
    message.set_symbol( "sfgpu----------" );
    message.set_logistic_level( sword::none );
    message.set_log_maintenance_manual( false );
    message.set_log_supply_manual( false );
    BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

    // creation
    MOCK_EXPECT( side.RegisterFormation ).once();
    std::auto_ptr< dispatcher::Formation_ABC > result( new dispatcher::Formation( model, message ) ); // $$$$ MCO : why is it an auto_ptr ?

    // network serialization
    MockClientPublisher publisher;
    MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
    result->SendCreation( publisher );
    mock::verify( publisher );

    // cleaning
    MOCK_EXPECT( side.RemoveFormation ).once();
}

// -----------------------------------------------------------------------------
// Name: Formation_CanBeCreated_WithExtensions
// Created: AHC 2012-10-08
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Formation_CanBeCreated_WithExtensions, Fixture )
{
    sword::SimToClient expected;
    expected.set_context( 0 );
    sword::FormationCreation& message = *expected.mutable_message()->mutable_formation_creation();
    message.mutable_formation()->set_id( 1 );
    message.mutable_party()->set_id( side.GetId() );
    message.set_level( sword::oo );
    message.set_name( "test" );
    message.set_app6symbol( "sfgpu----------" );
    message.set_symbol( "sfgpu----------" );
    message.set_logistic_level( sword::none );
    message.set_log_maintenance_manual( false );
    message.set_log_supply_manual( false );
    sword::Extension_Entry* entry = message.mutable_extension()->add_entries();
    entry->set_name( "extension" );
    entry->set_value( "value" );
    BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

    // creation
    MOCK_EXPECT( side.RegisterFormation ).once();
    dispatcher::Formation result( model, message );

    std::string extVal;
    BOOST_CHECK( result.GetExtension( "extension", extVal ) );
    BOOST_CHECK_EQUAL( extVal, "value" );

    // network serialization
    MockClientPublisher publisher;
    MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
    result.SendCreation( publisher );
    mock::verify( publisher );

    // cleaning
    MOCK_EXPECT( side.RemoveFormation ).once();
}

namespace
{
    std::auto_ptr< dispatcher::Formation > CreateFormation( const std::string& name, unsigned int id, dispatcher::Model_ABC& model, MockSide& side )
    {
        sword::SimToClient expected;
        expected.set_context( 0 );
        sword::FormationCreation& message = *expected.mutable_message()->mutable_formation_creation();
        message.mutable_formation()->set_id( id );
        message.mutable_party()->set_id( side.GetId() );
        message.set_level( sword::oo );
        message.set_name( name );
        message.set_app6symbol( "sfgpu----------" );
        message.set_symbol( "sfgpu----------" );
        message.set_logistic_level( sword::none );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        // creation
        MOCK_EXPECT( side.RegisterFormation ).once();
        return std::auto_ptr< dispatcher::Formation >( new dispatcher::Formation( model, message ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Formation_SuperiorCanBeChanged
// Created: AHC 2012-01-15
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Formation_SuperiorCanBeChanged, Fixture )
{
    std::auto_ptr< dispatcher::Formation > child( CreateFormation("child", 1, model, side ) );
    std::auto_ptr< dispatcher::Formation > parent( CreateFormation("parent", 52, model, side ) );

    formations.Register( child->GetId(), *child );
    formations.Register( parent->GetId(), *parent );
    BOOST_CHECK_EQUAL( formations.Get( 1 ).GetParent(), (dispatcher::Formation_ABC*)0 );

    {
        // change superior
        sword::FormationChangeSuperior message;
        message.mutable_formation()->set_id( 1 );
        message.mutable_superior()->mutable_formation()->set_id( 52 );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        MOCK_EXPECT( side.RemoveFormation ).once();
        MOCK_EXPECT( side.RegisterFormation ).once();
        formations.Get( 1 ).Update( message );
        BOOST_CHECK_EQUAL( formations.Get( 1 ).GetParent()->GetId(), 52ul );
    }

    {
        // change superior
        sword::FormationChangeSuperior message;
        message.mutable_formation()->set_id( 1 );
        message.mutable_superior()->mutable_party()->set_id( side.GetId() );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        formations.Get( 1 ).Update( message );
        BOOST_CHECK_EQUAL( formations.Get( 1 ).GetParent(), (dispatcher::Formation_ABC*)0 );
    }

    // cleaning
    MOCK_EXPECT( side.RemoveFormation ).exactly( 2 );
}


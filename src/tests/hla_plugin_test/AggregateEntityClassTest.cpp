// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/AggregateEntityClass.h"
#include "hla_plugin/AgentListener_ABC.h"
#include "hla_plugin/Aggregate_ABC.h"
#include "MockAgentSubject.h"
#include "MockFederate.h"
#include "MockAgent.h"
#include "MockClassBuilder.h"
#include "MockAggregateFactory.h"
#include "MockAggregate.h"
#include "rpr/EntityType.h"
#include "rpr/ForceIdentifier.h"
#include <hla/Class.h>
#include <hla/AttributeFunctor_ABC.h>
#include <hla/AttributeIdentifier.h>
#include <hla/ClassIdentifier.h>
#include <hla/ObjectIdentifier.h>
#include <hla/ObjectIdentifierFactory_ABC.h>

using namespace plugins::hla;

namespace
{
    MOCK_BASE_CLASS( MockObjectIdentifierFactory, ::hla::ObjectIdentifierFactory_ABC )
    {
        MOCK_METHOD( CreateIdentifier, 1 );
        MOCK_METHOD( ReleaseIdentifier, 1 );
    };
    class Fixture
    {
    public:
        Fixture()
            : factory( new MockObjectIdentifierFactory() ) // $$$$ _RC_ SLI 2011-06-10: wtf hla library?
            , listener( 0 )
            , hlaClass( 0 )
        {
            MOCK_EXPECT( subject, Register ).once().with( mock::retrieve( listener ) );
            MOCK_EXPECT( builder, Build ).once().with( mock::any, mock::retrieve( hlaClass ), true, false );
        }
        MockFederate federate;
        MockAgentSubject subject;
        MockObjectIdentifierFactory* factory;
        MockAgent agent;
        MockClassBuilder builder;
        MockAggregateFactory aggregateFactory;
        AgentListener_ABC* listener;
        hla::Class< Aggregate_ABC >* hlaClass;
    };
}

BOOST_FIXTURE_TEST_CASE( aggregate_entity_class_creates_instance_when_notified, Fixture )
{
    AggregateEntityClass entity( federate, subject, aggregateFactory, builder );
    BOOST_REQUIRE( listener );
    BOOST_REQUIRE( hlaClass );
    hlaClass->SetFactory( *factory );
    MOCK_EXPECT( aggregateFactory, Create ).once().returns( std::auto_ptr< Aggregate_ABC >( new MockAggregate() ) );
    MOCK_EXPECT( factory, CreateIdentifier ).once().with( "identifier" ).returns( hla::ObjectIdentifier( 42u ) );
    listener->Created( agent, "identifier", "name", rpr::Friendly, rpr::EntityType() );
    mock::verify();
    MOCK_EXPECT( factory, ReleaseIdentifier ).once().with( 42u );
    MOCK_EXPECT( subject, Unregister ).once();
}

BOOST_FIXTURE_TEST_CASE( aggregate_entity_class_destroys_instance_when_notified, Fixture )
{
    AggregateEntityClass entity( federate, subject, aggregateFactory, builder );
    BOOST_REQUIRE( listener );
    BOOST_REQUIRE( hlaClass );
    hlaClass->SetFactory( *factory );
    MOCK_EXPECT( aggregateFactory, Create ).once().returns( std::auto_ptr< Aggregate_ABC >( new MockAggregate() ) );
    MOCK_EXPECT( factory, CreateIdentifier ).once().with( "identifier" ).returns( hla::ObjectIdentifier( 42u ) );
    listener->Created( agent, "identifier", "name", rpr::Friendly, rpr::EntityType() );
    mock::verify();
    MOCK_EXPECT( factory, ReleaseIdentifier ).once().with( hla::ObjectIdentifier( 42u ) );
    listener->Destroyed( "identifier" );
    mock::verify();
    MOCK_EXPECT( subject, Unregister ).once();
}

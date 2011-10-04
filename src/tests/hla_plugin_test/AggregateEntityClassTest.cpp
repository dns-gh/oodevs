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
#include "hla_plugin/HlaObject_ABC.h"
#include "MockAgentSubject.h"
#include "MockFederate.h"
#include "MockAgent.h"
#include "MockClassBuilder.h"
#include "MockHlaObjectFactory.h"
#include "MockRemoteAggregateFactory.h"
#include "MockHlaObject.h"
#include "MockLocalAgentResolver.h"
#include "MockRemoteAgentListener.h"
#include "MockObjectIdentifierFactory.h"
#include "MockContextFactory.h"
#include "rpr/EntityType.h"
#include "rpr/ForceIdentifier.h"
#include <hla/Class.h>
#include <hla/AttributeFunctor_ABC.h>
#include <hla/AttributeIdentifier.h>
#include <hla/ClassIdentifier.h>

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : factory( new ::hla::MockObjectIdentifierFactory() ) // $$$$ _RC_ SLI 2011-06-10: wtf hla library?
            , listener( 0 )
            , hlaClass( 0 )
        {
            MOCK_EXPECT( subject, Register ).once().with( mock::retrieve( listener ) );
            MOCK_EXPECT( builder, BuildAggregate ).once().with( mock::any, mock::retrieve( hlaClass ), true, true );
            MOCK_EXPECT( identifierFactory, Create ).returns( 42 );
        }
        MockFederate federate;
        MockAgentSubject subject;
        ::hla::MockObjectIdentifierFactory* factory;
        MockAgent agent;
        MockClassBuilder builder;
        MockHlaObjectFactory hlaObjectFactory;
        MockRemoteAggregateFactory remoteFactory;
        MockRemoteAgentListener remoteListener;
        MockLocalAgentResolver localResolver;
        MockContextFactory identifierFactory;
        AgentListener_ABC* listener;
        hla::Class< HlaObject_ABC >* hlaClass;
    };
    class RegisteredFixture : public Fixture
    {
    public:
        RegisteredFixture()
            : entity( federate, subject, localResolver, hlaObjectFactory, remoteFactory, builder, identifierFactory )
        {
            BOOST_REQUIRE( listener );
            BOOST_REQUIRE( hlaClass );
            hlaClass->SetFactory( *factory );
        }
        ~RegisteredFixture()
        {
            MOCK_EXPECT( subject, Unregister ).once();
        }
        AggregateEntityClass entity;
    };
}

BOOST_FIXTURE_TEST_CASE( aggregate_entity_class_creates_instance_when_notified, RegisteredFixture )
{
    MOCK_EXPECT( hlaObjectFactory, CreateAggregate ).once().returns( std::auto_ptr< HlaObject_ABC >( new MockHlaObject() ) );
    MOCK_EXPECT( factory, CreateIdentifier ).once().with( "123" ).returns( hla::ObjectIdentifier( 42u ) );
    MOCK_EXPECT( localResolver, Add ).once().with( 123u, "42" );
    listener->AggregateCreated( agent, 123, "name", rpr::Friendly, rpr::EntityType() );
    mock::verify();
    MOCK_EXPECT( factory, ReleaseIdentifier ).once().with( 42u );
}

BOOST_FIXTURE_TEST_CASE( aggregate_entity_class_creates_remote_instances, RegisteredFixture )
{
    entity.Register( remoteListener );
    MOCK_EXPECT( remoteFactory, Create ).once().with( "42", mock::any ).returns( std::auto_ptr< HlaObject_ABC >( new MockHlaObject() ) );
    MOCK_EXPECT( remoteListener, Created ).once().with( "42" );
    hlaClass->Create( ::hla::ObjectIdentifier( 42u ), "name" );
    mock::verify();
    MOCK_EXPECT( remoteListener, Destroyed ).once().with( "42" );
}

BOOST_FIXTURE_TEST_CASE( aggregate_entity_class_destroys_remote_instances, RegisteredFixture )
{
    MOCK_EXPECT( remoteFactory, Create ).once().returns( std::auto_ptr< HlaObject_ABC >( new MockHlaObject() ) );
    hlaClass->Create( ::hla::ObjectIdentifier( 42u ), "name" );
    MOCK_EXPECT( remoteListener, Created ).once().with( "42" );
    entity.Register( remoteListener );
    MOCK_EXPECT( remoteListener, Destroyed ).once().with( "42" );
    hlaClass->Destroy( 42u );
}

BOOST_FIXTURE_TEST_CASE( aggregate_entity_class_does_nothing_when_surface_vessel_is_created, RegisteredFixture )
{
    listener->SurfaceVesselCreated( agent, 123, "name", rpr::Friendly, rpr::EntityType() );
}

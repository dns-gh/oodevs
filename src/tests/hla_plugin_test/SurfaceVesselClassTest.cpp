// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/SurfaceVesselClass.h"
#include "hla_plugin/AgentListener_ABC.h"
#include "hla_plugin/HlaObject_ABC.h"
#include "MockAgentSubject.h"
#include "MockFederate.h"
#include "MockAgent.h"
#include "MockClassBuilder.h"
#include "MockHlaObjectFactory.h"
#include "MockHlaObject.h"
#include "MockLocalAgentResolver.h"
#include "MockRemoteAgentListener.h"
#include "MockRemoteHlaObjectFactory.h"
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
            : factory         ( new ::hla::MockObjectIdentifierFactory() ) // $$$$ _RC_ SLI 2011-06-10: wtf hla library?
            , builder         ( new MockClassBuilder() )
            , hlaObjectFactory( new MockHlaObjectFactory() )
            , remoteFactory   ( new MockRemoteHlaObjectFactory() )
            , listener        ( 0 )
            , hlaClass        ( 0 )
        {
            MOCK_EXPECT( subject, Register ).once().with( mock::retrieve( listener ) );
            MOCK_EXPECT( builder, Build ).once().with( mock::any, mock::retrieve( hlaClass ), true, true );
            MOCK_EXPECT( identifierFactory, Create ).returns( 42 );
        }
        MockFederate federate;
        MockAgentSubject subject;
        ::hla::MockObjectIdentifierFactory* factory;
        MockAgent agent;
        MockClassBuilder* builder;
        MockHlaObjectFactory* hlaObjectFactory;
        MockRemoteHlaObjectFactory* remoteFactory;
        MockLocalAgentResolver localResolver;
        MockRemoteAgentListener remoteListener;
        MockContextFactory identifierFactory;
        AgentListener_ABC* listener;
        hla::Class< HlaObject_ABC >* hlaClass;
    };
    class RegisteredFixture : public Fixture
    {
    public:
        RegisteredFixture()
            : surfaceVessel( federate, subject, localResolver,
                             std::auto_ptr< HlaObjectFactory_ABC >( hlaObjectFactory ),
                             std::auto_ptr< RemoteHlaObjectFactory_ABC >( remoteFactory ),
                             std::auto_ptr< ClassBuilder_ABC >( builder ), identifierFactory )
        {
            BOOST_REQUIRE( listener );
            BOOST_REQUIRE( hlaClass );
            hlaClass->SetFactory( *factory );
        }
        ~RegisteredFixture()
        {
            MOCK_EXPECT( subject, Unregister ).once();
        }
        SurfaceVesselClass surfaceVessel;
    };
}

BOOST_FIXTURE_TEST_CASE( surface_vessel_class_creates_instance_when_notified, RegisteredFixture )
{
    MOCK_EXPECT( hlaObjectFactory, Create ).once().returns( std::auto_ptr< HlaObject_ABC >( new MockHlaObject() ) );
    MOCK_EXPECT( factory, CreateIdentifier ).once().with( "123" ).returns( hla::ObjectIdentifier( 42u ) );
    MOCK_EXPECT( localResolver, Add ).once().with( 123u, "42" );
    listener->SurfaceVesselCreated( agent, 123, "name", rpr::Friendly, rpr::EntityType() );
    mock::verify();
    MOCK_EXPECT( factory, ReleaseIdentifier ).once().with( 42u );
}

BOOST_FIXTURE_TEST_CASE( surface_vessel_class_creates_remote_instances, RegisteredFixture )
{
    surfaceVessel.Register( remoteListener );
    MOCK_EXPECT( remoteFactory, Create ).once().with( "42", mock::any ).returns( std::auto_ptr< HlaObject_ABC >( new MockHlaObject() ) );
    MOCK_EXPECT( remoteListener, Created ).once().with( "42" );
    hlaClass->Create( ::hla::ObjectIdentifier( 42u ), "name" );
    mock::verify();
    MOCK_EXPECT( remoteListener, Destroyed ).once().with( "42" );
}

BOOST_FIXTURE_TEST_CASE( surface_vessel_class_destroys_remote_instances, RegisteredFixture )
{
    MOCK_EXPECT( remoteFactory, Create ).once().returns( std::auto_ptr< HlaObject_ABC >( new MockHlaObject() ) );
    hlaClass->Create( ::hla::ObjectIdentifier( 42u ), "name" );
    MOCK_EXPECT( remoteListener, Created ).once().with( "42" );
    surfaceVessel.Register( remoteListener );
    MOCK_EXPECT( remoteListener, Destroyed ).once().with( "42" );
    hlaClass->Destroy( 42u );
}

BOOST_FIXTURE_TEST_CASE( surface_vessel_does_nothing_when_aggregate_is_created, RegisteredFixture )
{
    listener->AggregateCreated( agent, 123, "name", rpr::Friendly, rpr::EntityType() );
}

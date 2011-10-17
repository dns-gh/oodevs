// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/HlaClass.h"
#include "hla_plugin/HlaObject_ABC.h"
#include "MockFederate.h"
#include "MockAgent.h"
#include "MockClassBuilder.h"
#include "MockHlaObjectFactory.h"
#include "MockRemoteHlaObjectFactory.h"
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
            : factory         ( new ::hla::MockObjectIdentifierFactory() ) // $$$$ _RC_ SLI 2011-06-10: wtf hla library?
            , builder         ( new MockClassBuilder() )
            , hlaObjectFactory( new MockHlaObjectFactory() )
            , remoteFactory   ( new MockRemoteHlaObjectFactory() )
            , hlaObjectClass  ( 0 )
        {
            MOCK_EXPECT( builder, Build ).once().with( mock::any, mock::retrieve( hlaObjectClass ), true, true );
            MOCK_EXPECT( identifierFactory, Create ).returns( 42 );
        }
        MockFederate federate;
        ::hla::MockObjectIdentifierFactory* factory;
        MockAgent agent;
        MockClassBuilder* builder;
        MockHlaObjectFactory* hlaObjectFactory;
        MockRemoteHlaObjectFactory* remoteFactory;
        MockRemoteAgentListener remoteListener;
        MockLocalAgentResolver localResolver;
        MockContextFactory identifierFactory;
        hla::Class< HlaObject_ABC >* hlaObjectClass;
    };
    class RegisteredFixture : public Fixture
    {
    public:
        RegisteredFixture()
            : hlaClass( federate, localResolver,
                        std::auto_ptr< HlaObjectFactory_ABC >( hlaObjectFactory ),
                        std::auto_ptr< RemoteHlaObjectFactory_ABC >( remoteFactory ),
                        std::auto_ptr< ClassBuilder_ABC >( builder ) )
        {
            BOOST_REQUIRE( hlaObjectClass );
            hlaObjectClass->SetFactory( *factory );
        }
        HlaClass hlaClass;
    };
}

BOOST_FIXTURE_TEST_CASE( hla_class_creates_instance_when_notified, RegisteredFixture )
{
    MOCK_EXPECT( hlaObjectFactory, Create ).once().returns( std::auto_ptr< HlaObject_ABC >( new MockHlaObject() ) );
    MOCK_EXPECT( factory, CreateIdentifier ).once().with( "123" ).returns( hla::ObjectIdentifier( 42u ) );
    MOCK_EXPECT( localResolver, Add ).once().with( 123u, "42" );
    hlaClass.Created( agent, 123, "name", rpr::Friendly, rpr::EntityType() );
    mock::verify();
    MOCK_EXPECT( factory, ReleaseIdentifier ).once().with( 42u );
}

BOOST_FIXTURE_TEST_CASE( hla_class_creates_remote_instances, RegisteredFixture )
{
    hlaClass.Register( remoteListener );
    MOCK_EXPECT( remoteFactory, Create ).once().with( "name", mock::any ).returns( std::auto_ptr< HlaObject_ABC >( new MockHlaObject() ) );
    MOCK_EXPECT( remoteListener, Created ).once().with( "name" );
    hlaObjectClass->Create( ::hla::ObjectIdentifier( 42u ), "name" );
    mock::verify();
    MOCK_EXPECT( remoteListener, Destroyed ).once().with( "name" );
}

BOOST_FIXTURE_TEST_CASE( hla_class_destroys_remote_instances, RegisteredFixture )
{
    MOCK_EXPECT( remoteFactory, Create ).once().returns( std::auto_ptr< HlaObject_ABC >( new MockHlaObject() ) );
    hlaObjectClass->Create( ::hla::ObjectIdentifier( 42u ), "name" );
    MOCK_EXPECT( remoteListener, Created ).once().with( "name" );
    hlaClass.Register( remoteListener );
    MOCK_EXPECT( remoteListener, Destroyed ).once().with( "name" );
    hlaObjectClass->Destroy( 42u );
}

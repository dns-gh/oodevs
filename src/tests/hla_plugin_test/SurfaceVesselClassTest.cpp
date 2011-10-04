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
            MOCK_EXPECT( builder, BuildSurfaceVessel ).once().with( mock::any, mock::retrieve( hlaClass ), true, false );
            MOCK_EXPECT( identifierFactory, Create ).returns( 42 );
        }
        MockFederate federate;
        MockAgentSubject subject;
        ::hla::MockObjectIdentifierFactory* factory;
        MockAgent agent;
        MockClassBuilder builder;
        MockHlaObjectFactory hlaObjectFactory;
        MockLocalAgentResolver localResolver;
        MockContextFactory identifierFactory;
        AgentListener_ABC* listener;
        hla::Class< HlaObject_ABC >* hlaClass;
    };
    class RegisteredFixture : public Fixture
    {
    public:
        RegisteredFixture()
            : surfaceVessel( federate, subject, localResolver, hlaObjectFactory, builder, identifierFactory )
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
    MOCK_EXPECT( hlaObjectFactory, CreateSurfaceVessel ).once().returns( std::auto_ptr< HlaObject_ABC >( new MockHlaObject() ) );
    MOCK_EXPECT( factory, CreateIdentifier ).once().with( "123" ).returns( hla::ObjectIdentifier( 42u ) );
    MOCK_EXPECT( localResolver, Add ).once().with( 123u, "42" );
    listener->SurfaceVesselCreated( agent, 123, "name", rpr::Friendly, rpr::EntityType() );
    mock::verify();
    MOCK_EXPECT( factory, ReleaseIdentifier ).once().with( 42u );
}

BOOST_FIXTURE_TEST_CASE( surface_vessel_does_nothing_when_aggregate_is_created, RegisteredFixture )
{
    listener->AggregateCreated( agent, 123, "name", rpr::Friendly, rpr::EntityType() );
}

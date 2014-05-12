// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "MockObjectIdentifierFactory.h"
#include "hla_plugin/HlaClass.h"
#include "hla_plugin/HlaObject_ABC.h"
#include "MockFederate.h"
#include "MockAgent.h"
#include "MockClassBuilder.h"
#include "MockHlaObjectFactory.h"
#include "MockRemoteHlaObjectFactory.h"
#include "MockHlaObject.h"
#include "MockLocalAgentResolver.h"
#include "MockClassListener.h"
#include "MockContextFactory.h"
#include "MockHlaObjectNameFactory.h"
#include "MockOwnershipStrategy.h"
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
            MOCK_EXPECT( builder->Build ).once().with( mock::any, mock::retrieve( hlaObjectClass ) );
            MOCK_EXPECT( builder->GetAttributes );
            MOCK_EXPECT( identifierFactory.Create ).returns( 42 );
        }
        MockFederate federate;
        ::hla::MockObjectIdentifierFactory* factory;
        MockAgent agent;
        MockClassBuilder* builder;
        MockHlaObjectFactory* hlaObjectFactory;
        MockRemoteHlaObjectFactory* remoteFactory;
        MockClassListener remoteListener;
        MockLocalAgentResolver localResolver;
        MockContextFactory identifierFactory;
        hla::Class< HlaObject_ABC >* hlaObjectClass;
        MockHlaObjectNameFactory objectNameFactory;
        MockOwnershipStrategy ownershipStrategy;
    };
    class RegisteredFixture : public Fixture
    {
    public:
        RegisteredFixture()
            : hlaClass( federate, localResolver, objectNameFactory,
                        std::unique_ptr< HlaObjectFactory_ABC >( hlaObjectFactory ),
                        std::unique_ptr< RemoteHlaObjectFactory_ABC >( remoteFactory ),
                        std::unique_ptr< ClassBuilder_ABC >( builder ), ownershipStrategy )
        {
            BOOST_REQUIRE( hlaObjectClass );
            hlaObjectClass->SetFactory( *factory );
        }
        HlaClass hlaClass;
    };

    struct NUllObject : public HlaObject_ABC
    {
        virtual void Serialize( ::hla::UpdateFunctor_ABC& , bool  ) const {}
        virtual void Deserialize( const ::hla::AttributeIdentifier& , ::hla::Deserializer_ABC&  ) {}
        virtual void Register( ObjectListener_ABC&  ) {}
        virtual void Unregister( ObjectListener_ABC&  ) {}
        virtual void Attach( Agent_ABC* , unsigned long  ) {}
        virtual void SetIdentifier( const std::string&  ) {}
        virtual const std::string& GetIdentifier() const { static std::string DEFVAL("DEFINT"); return DEFVAL; }
        virtual void ResetAttributes() {}
    };

    // workwaround needed because turtle does not support unique_ptr returned
    std::unique_ptr< HlaObject_ABC > createLocal(Agent_ABC&, const std::string&, unsigned long, rpr::ForceIdentifier, const rpr::EntityType&, 
        const std::string&, const std::string&, const std::vector< char >&, HlaObject_ABC* ptr )
    {
        return std::unique_ptr< HlaObject_ABC >( ptr );
    }
    std::unique_ptr< HlaObject_ABC > createRemote( const std::string&, HlaObject_ABC* ptr )
    {
        return std::unique_ptr< HlaObject_ABC >( ptr );
    }
}

BOOST_FIXTURE_TEST_CASE( hla_class_creates_instance_when_notified, RegisteredFixture )
{
    MOCK_EXPECT( hlaObjectFactory->Create ).once().calls( boost::bind( &createLocal, _1, _2, _3, _4, _5, _6, _7, _8, new NUllObject() ) );
    MOCK_EXPECT( objectNameFactory.CreateName ).once().returns( "123" );
    MOCK_EXPECT( factory->CreateIdentifier ).once().with( "123" ).returns( hla::ObjectIdentifier( 42u ) );
    MOCK_EXPECT( localResolver.Add ).once().with( 123u, "123" );
    hlaClass.Created( agent, 123, "name", rpr::Friendly, rpr::EntityType(), "symbol", MakeUniqueId( "unique_1" ) );
    mock::verify();
    MOCK_EXPECT( factory->ReleaseIdentifier ).once().with( 42u );
}

BOOST_FIXTURE_TEST_CASE( hla_class_creates_remote_instances, RegisteredFixture )
{
    hlaClass.Register( remoteListener );
    MOCK_EXPECT( remoteFactory->Create ).once().with( "name" ).calls( boost::bind( &createRemote, _1, new MockHlaObject() ) );
    MOCK_EXPECT( remoteListener.RemoteCreated ).once().with( "name", mock::any, mock::any );
    hlaObjectClass->Create( ::hla::ObjectIdentifier( 42u ), "name" );
    mock::verify();
    MOCK_EXPECT( remoteListener.RemoteDestroyed ).once().with( "name" );
}

BOOST_FIXTURE_TEST_CASE( hla_class_destroys_remote_instances, RegisteredFixture )
{
    MOCK_EXPECT( remoteFactory->Create ).once().calls( boost::bind( &createRemote, _1, new MockHlaObject() ) );
    hlaObjectClass->Create( ::hla::ObjectIdentifier( 42u ), "name" );
    MOCK_EXPECT( remoteListener.RemoteCreated ).once().with( "name", mock::any, mock::any );
    hlaClass.Register( remoteListener );
    MOCK_EXPECT( remoteListener.RemoteDestroyed ).once().with( "name" );
    hlaObjectClass->Destroy( 42u );
}

BOOST_FIXTURE_TEST_CASE( hla_class_creates_duplicate_remote_instances, RegisteredFixture )
{
    hlaClass.Register( remoteListener );
    HlaObject_ABC* obj1 = new NUllObject();
    MOCK_EXPECT( remoteFactory->Create ).once().with( "name" ).calls( boost::bind( &createRemote, _1, obj1 ) );
    MOCK_EXPECT( remoteListener.RemoteCreated ).once().with( "name", mock::any, mock::any );
    hlaObjectClass->Create( ::hla::ObjectIdentifier( 42u ), "name" );
    mock::verify();

    HlaObject_ABC* obj2 = new NUllObject();
    MOCK_EXPECT( remoteFactory->Create ).once().with( "name" ).calls( boost::bind( &createRemote, _1, obj2 ) );
    MOCK_EXPECT( remoteListener.RemoteCreated ).once().with( "name", mock::any, mock::any );
    hlaObjectClass->Create( ::hla::ObjectIdentifier( 43u ), "name" );
    mock::verify();

    obj1->GetIdentifier();
    obj2->GetIdentifier();
    MOCK_EXPECT( remoteListener.RemoteDestroyed ).once().with( "name" );
    hlaObjectClass->Destroy( 42u );
    hlaObjectClass->Destroy( 43u );
}

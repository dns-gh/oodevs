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
#include "MockAgentSubject.h"
#include "MockFederate.h"
#include "MockAgent.h"
#include "rpr/EntityType.h"
#include "rpr/ForceIdentifier.h"
#include <hla/Class_ABC.h>
#include <hla/AttributeFunctor_ABC.h>
#include <hla/AttributeIdentifier.h>
#include <hla/ClassIdentifier.h>
#include <hla/ObjectIdentifier.h>
#include <hla/ObjectIdentifierFactory_ABC.h>
#include <boost/foreach.hpp>
#include <boost/assign.hpp>

using namespace plugins::hla;

namespace
{
    MOCK_BASE_CLASS( MockAttributeFunctor, ::hla::AttributeFunctor_ABC )
    {
        MOCK_METHOD( Visit, 1 );
    };
    MOCK_BASE_CLASS( MockObjectIdentifierFactory, ::hla::ObjectIdentifierFactory_ABC )
    {
        MOCK_METHOD( CreateIdentifier, 1 );
        MOCK_METHOD( ReleaseIdentifier, 1 );
    };
    class Fixture
    {
    public:
        Fixture()
        {
            MOCK_EXPECT( subject, Register ).once();
            MOCK_EXPECT( subject, Unregister ).once();
        }
        MockFederate federate;
        MockAgentSubject subject;
    };
    bool CheckAttributes( const ::hla::Class_ABC& hlaClass, const std::vector< std::string >& attributes )
    {
        MockAttributeFunctor visitor;
        mock::sequence s;
        BOOST_FOREACH( const std::string& attribute, attributes )
            MOCK_EXPECT( visitor, Visit ).once().in( s ).with( attribute );
        hlaClass.Apply( visitor );
        return true;
    }
}

BOOST_FIXTURE_TEST_CASE( aggregate_entity_class_registers_attributes, Fixture )
{
    const std::vector< std::string > attributes = boost::assign::list_of( "EntityType" )
                                                                        ( "EntityIdentifier" )
                                                                        ( "Spatial" )
                                                                        ( "AggregateMarking" )
                                                                        ( "AggregateState" )
                                                                        ( "Dimensions" )
                                                                        ( "EntityIdentifiers" )
                                                                        ( "ForceIdentifier" )
                                                                        ( "Formation" )
                                                                        ( "NumberOfSilentEntities" )
                                                                        ( "SilentEntities" )
                                                                        ( "SilentAggregates" )
                                                                        ( "Mounted" )
                                                                        ( "Echelon" );
    MOCK_EXPECT( federate, RegisterClass ).once().with( "BaseEntity.AggregateEntity.NETN_Aggregate", boost::bind( &CheckAttributes, _1, attributes ), true, false );
    AggregateEntityClass entity( federate, subject );
}

BOOST_FIXTURE_TEST_CASE( aggregate_entity_class_creates_instance_when_notified, Fixture )
{
    MockAgent agent;
    MockObjectIdentifierFactory* factory = new MockObjectIdentifierFactory(); // $$$$ _RC_ SLI 2011-06-10: wtf hla library?
    subject.reset();
    AgentListener_ABC* listener = 0;
    hla::Class_ABC* hlaClass = 0;
    MOCK_EXPECT( subject, Register ).once().with( mock::retrieve( listener ) );
    MOCK_EXPECT( federate, RegisterClass ).once().with( mock::any, mock::retrieve( hlaClass ), true, false );
    AggregateEntityClass entity( federate, subject );
    BOOST_REQUIRE( listener );
    BOOST_REQUIRE( hlaClass );
    hlaClass->SetFactory( *factory );
    MOCK_EXPECT( agent, Register ).once();
    MOCK_EXPECT( factory, CreateIdentifier ).once().with( "identifier" ).returns( hla::ObjectIdentifier( 42u ) );
    listener->Created( agent, "identifier", "name", rpr::Friendly, rpr::EntityType() );
    MOCK_EXPECT( agent, Unregister ).once();
    MOCK_EXPECT( subject, Unregister ).once();
    MOCK_EXPECT( factory, ReleaseIdentifier ).once().with( 42u );
}

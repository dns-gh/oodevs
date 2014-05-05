// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/FederateFacade.h"
#include "protocol/Simulation.h"
#include "MockAgentSubject.h"
#include "MockRtiAmbassadorFactory.h"
#include "MockFederateAmbassadorFactory.h"
#include "MockFederate.h"
#include "MockRtiAmbassador.h"
#include "MockMessageController.h"
#include "MockLocalAgentResolver.h"
#include "MockOwnershipStrategy.h"
#include "MockCallsignResolver.h"
#include "MockTacticalObjectSubject.h"
#include "MockEntityIdentifierResolver.h"
#include "MockFOM_Serialization.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : federate( new MockFederate() )
            , listener( 0 )
        {}
        MockAgentSubject subject;
        MockTacticalObjectSubject tacticalObjectSubject;
        MockRtiAmbassadorFactory rtiFactory;
        MockFederateAmbassadorFactory federateFactory;
        MockFederate* federate;
        tools::MockMessageController< sword::SimToClient_Content > controller;
        MockLocalAgentResolver localResolver;
        tools::MessageHandler_ABC< sword::SimToClient_Content >* listener;
        MockCallsignResolver callsignResolver;
        mock::sequence s;
        MockOwnershipStrategy ownershipStrategy;
        MockEntityIdentifierResolver entityIdResolver;
        MockFOM_Serialization fomSerialization;
    };
    std::unique_ptr< Federate_ABC > returnFederate( ::hla::RtiAmbassador_ABC&, const std::string&, int, Federate_ABC* ptr )
    {
        return std::unique_ptr< Federate_ABC >( ptr );
    }
}
// TODO leaks
BOOST_FIXTURE_TEST_CASE( hla_plugin_initialization_declares_publications_with_netn_by_default, Fixture )
{
    xml::xistringstream xis( "<root disaggregate='1'/>" );
    xis >> xml::start( "root" );
    ::hla::MockRtiAmbassador ambassador;
    MOCK_EXPECT( rtiFactory.CreateAmbassador ).once().in( s ).with( mock::any, mock::any, hla::RtiAmbassador_ABC::TimeStampOrder, "" ).returns( &ambassador );
    MOCK_EXPECT( federateFactory.Create ).once().in( s ).with( mock::any, "SWORD", -1 ).calls( boost::bind( &returnFederate, _1, _2, _3, federate ) );
    MOCK_EXPECT( federate->Connect ).once().in( s ).returns( true );
    MOCK_EXPECT( federate->Join ).once().in( s ).with( "Federation", true, true ).returns( true );
    MOCK_EXPECT( federate->RegisterClass ).once().in( s ).with( "BaseEntity.AggregateEntity.NETN_Aggregate", mock::any, true, true );
    MOCK_EXPECT( federate->RegisterClass ).once().in( s ).with( "BaseEntity.PhysicalEntity.Platform.SurfaceVessel.NETN_SurfaceVessel", mock::any, true, true );
    MOCK_EXPECT( federate->RegisterClass ).once().in( s ).with( "BaseEntity.PhysicalEntity.Platform.Aircraft.NETN_Aircraft", mock::any, true, true );
    MOCK_EXPECT( federate->RegisterClass ).once().in( s ).with( "BaseEntity.PhysicalEntity.Platform.GroundVehicle.NETN_GroundVehicle", mock::any, true, false );
    MOCK_EXPECT( federate->RegisterClass ).once().in( s ).with( "BaseEntity.PhysicalEntity.Lifeform.Human.NETN_Human", mock::any, true, false );
    MOCK_EXPECT( federate->RegisterClass ).once().in( s ).with( "BaseEntity.AggregateEntity", mock::any, true, true );
    MOCK_EXPECT( federate->RegisterClass ).once().in( s ).with( "BaseEntity.PhysicalEntity.Platform.SurfaceVessel", mock::any, true, true );
    MOCK_EXPECT( federate->RegisterClass ).once().in( s ).with( "BaseEntity.PhysicalEntity.Platform.Aircraft", mock::any, true, true );
    MOCK_EXPECT( federate->RegisterClass ).once().in( s ).with( "Minefield", mock::any, true, true );
    MOCK_EXPECT( federate->RegisterClass ).once().in( s ).with( "BaseEntity.PhysicalEntity.CulturalFeature", mock::any, true, true );
    MOCK_EXPECT( federate->RegisterClass ).once().in( s ).with( "EnvironmentObject.LinearObject.BreachableLinearObject", mock::any, true, true );
    MOCK_EXPECT( federate->RegisterClass ).once().in( s ).with( "EnvironmentObject.PointObject.BreachablePointObject", mock::any, true, true );
    MOCK_EXPECT( federate->RegisterClass ).once().in( s ).with( "EnvironmentObject.PointObject.OtherPointObject", mock::any, true, true );
    MOCK_EXPECT( federate->RegisterClass ).once().in( s ).with( "EnvironmentObject.ArealObject.OtherArealObject", mock::any, true, true );

    MOCK_EXPECT( subject.Register ).once();
    MOCK_EXPECT( tacticalObjectSubject.Register ).once();
    MOCK_EXPECT( controller.Register ).once().in( s );
    FederateFacade facade( xis, controller, subject, localResolver, rtiFactory, federateFactory, "directory", callsignResolver,
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization );
    MOCK_EXPECT( tacticalObjectSubject.Unregister ).once();
    MOCK_EXPECT( subject.Unregister ).once();
    MOCK_EXPECT( controller.Unregister ).once().in( s );
    MOCK_EXPECT( rtiFactory.DeleteAmbassador ).once().with( &ambassador );
}

BOOST_FIXTURE_TEST_CASE( netn_use_can_be_desactivated, Fixture )
{
    xml::xistringstream xis( "<root netn='false' disaggregate='1'/>" );
    xis >> xml::start( "root" );
    ::hla::MockRtiAmbassador ambassador;
    MOCK_EXPECT( rtiFactory.CreateAmbassador ).once().returns( &ambassador );
    MOCK_EXPECT( federateFactory.Create ).once().calls( boost::bind( &returnFederate, _1, _2, _3, federate ) );
    MOCK_EXPECT( federate->Connect ).once().returns( true );
    MOCK_EXPECT( federate->Join ).once().returns( true );
    MOCK_EXPECT( federate->RegisterClass ).once().with( "BaseEntity.AggregateEntity", mock::any, true, true );
    MOCK_EXPECT( federate->RegisterClass ).once().with( "BaseEntity.PhysicalEntity.Platform.SurfaceVessel", mock::any, true, true );
    MOCK_EXPECT( federate->RegisterClass ).once().with( "BaseEntity.PhysicalEntity.Platform.Aircraft", mock::any, true, true );
    MOCK_EXPECT( federate->RegisterClass ).once().with( "BaseEntity.PhysicalEntity.Platform.GroundVehicle", mock::any, true, false );
    MOCK_EXPECT( federate->RegisterClass ).once().with( "BaseEntity.PhysicalEntity.Lifeform.Human", mock::any, true, false );
    MOCK_EXPECT( federate->RegisterClass ).once().with( "Minefield", mock::any, true, true );
    MOCK_EXPECT( federate->RegisterClass ).once().with( "BaseEntity.PhysicalEntity.CulturalFeature", mock::any, true, true );
    MOCK_EXPECT( federate->RegisterClass ).once().with( "EnvironmentObject.LinearObject.BreachableLinearObject", mock::any, true, true );
    MOCK_EXPECT( federate->RegisterClass ).once().with( "EnvironmentObject.PointObject.BreachablePointObject", mock::any, true, true );
    MOCK_EXPECT( federate->RegisterClass ).once().with( "EnvironmentObject.PointObject.OtherPointObject", mock::any, true, true );
    MOCK_EXPECT( federate->RegisterClass ).once().with( "EnvironmentObject.ArealObject.OtherArealObject", mock::any, true, true );
    MOCK_EXPECT( subject.Register ).once();
    MOCK_EXPECT( tacticalObjectSubject.Register ).once();
    MOCK_EXPECT( controller.Register ).once();
    FederateFacade facade( xis, controller, subject, localResolver, rtiFactory, federateFactory, "directory", callsignResolver,
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization );
    MOCK_EXPECT( tacticalObjectSubject.Unregister ).once();
    MOCK_EXPECT( subject.Unregister ).once();
    MOCK_EXPECT( controller.Unregister ).once();
    MOCK_EXPECT( rtiFactory.DeleteAmbassador ).once().with( &ambassador );
}

namespace
{
    class BuildFixture : public Fixture
    {
    public:
        BuildFixture()
        {
            MOCK_EXPECT( subject.Register );
            MOCK_EXPECT( tacticalObjectSubject.Register ).once();
            MOCK_EXPECT( subject.Unregister );
            MOCK_EXPECT( tacticalObjectSubject.Unregister ).once();
            MOCK_EXPECT( controller.Register ).once().with( mock::retrieve( listener ) );
            MOCK_EXPECT( controller.Unregister ).once();
            MOCK_EXPECT( federate->RegisterClass ).exactly( 12 );
            MOCK_EXPECT( federate->Connect ).once().returns( true );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( hla_plugin_xml_options_overrides_default_values, BuildFixture )
{
    xml::xistringstream xis( "<root name='name' federation='federation' lrcSettings='crcHost=localhost;crcPort=8989'"
                             "      time-constrained='false' time-regulating='false' lookahead='3' />" );
    xis >> xml::start( "root" );
    ::hla::MockRtiAmbassador ambassador;
    MOCK_EXPECT( rtiFactory.CreateAmbassador ).once().with( mock::any, mock::any, hla::RtiAmbassador_ABC::TimeStampOrder, "crcHost=localhost;crcPort=8989" ).returns( &ambassador );
    MOCK_EXPECT( federateFactory.Create ).once().with( mock::any, "name", 3 ).calls( boost::bind( &returnFederate, _1, _2, _3, federate ) );
    MOCK_EXPECT( federate->Join ).once().with( "federation", false, false ).returns( true );
    FederateFacade facade( xis, controller, subject, localResolver, rtiFactory, federateFactory, "directory", callsignResolver,
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization );
    MOCK_EXPECT( rtiFactory.DeleteAmbassador ).once().with( &ambassador );
}

BOOST_FIXTURE_TEST_CASE( hla_plugin_can_create_federation, BuildFixture )
{
    xml::xistringstream xis( "<root name='name' creation='true' fom='fom'/>" );
    xis >> xml::start( "root" );
    ::hla::MockRtiAmbassador ambassador;
    MOCK_EXPECT( rtiFactory.CreateAmbassador ).once().returns( &ambassador );
    MOCK_EXPECT( federateFactory.Create ).once().calls( boost::bind( &returnFederate, _1, _2, _3, federate ) );
    MOCK_EXPECT( federate->Join ).once().in( s ).returns( false );
    MOCK_EXPECT( federate->Create1 ).once().in( s ).with( "Federation", "directory/fom" ).returns( true );
    MOCK_EXPECT( federate->Join ).once().in( s ).returns( true );
    FederateFacade facade( xis, controller, subject, localResolver, rtiFactory, federateFactory, "directory", callsignResolver,
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization );
    MOCK_EXPECT( rtiFactory.DeleteAmbassador ).once().with( &ambassador );
}

BOOST_FIXTURE_TEST_CASE( hla_plugin_can_create_federation_with_absolute_fom, BuildFixture )
{
    xml::xistringstream xis( "<root name='name' creation='true' fom='c:/fom'/>" );
    xis >> xml::start( "root" );
    ::hla::MockRtiAmbassador ambassador;
    MOCK_EXPECT( rtiFactory.CreateAmbassador ).once().returns( &ambassador );
    MOCK_EXPECT( federateFactory.Create ).once().calls( boost::bind( &returnFederate, _1, _2, _3, federate ) );
    MOCK_EXPECT( federate->Join ).once().in( s ).returns( false );
    MOCK_EXPECT( federate->Create1 ).once().in( s ).with( "Federation", "c:/fom" ).returns( true );
    MOCK_EXPECT( federate->Join ).once().in( s ).returns( true );
    FederateFacade facade( xis, controller, subject, localResolver, rtiFactory, federateFactory, "directory", callsignResolver,
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization );
    MOCK_EXPECT( rtiFactory.DeleteAmbassador ).once().with( &ambassador );
}

BOOST_FIXTURE_TEST_CASE( hla_plugin_can_create_federation_with_fom_modules, BuildFixture )
{
    std::vector< std::string > FOM_FILES;
    FOM_FILES.push_back("c:/fom1");
    FOM_FILES.push_back("c:/fom2");

    xml::xistringstream xis( "<root name='name' creation='true' fom='c:/fom1;c:/fom2'/>" );
    xis >> xml::start( "root" );
    ::hla::MockRtiAmbassador ambassador;
    MOCK_EXPECT( rtiFactory.CreateAmbassador ).once().returns( &ambassador );
    MOCK_EXPECT( federateFactory.Create ).once().calls( boost::bind( &returnFederate, _1, _2, _3, federate ) );
    MOCK_EXPECT( federate->Join ).once().in( s ).returns( false );
    MOCK_EXPECT( federate->CreateV ).once().in( s ).with( "Federation", FOM_FILES ).returns( true );
    MOCK_EXPECT( federate->Join ).once().in( s ).returns( true );
    FederateFacade facade( xis, controller, subject, localResolver, rtiFactory, federateFactory, "directory", callsignResolver,
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization );
    MOCK_EXPECT( rtiFactory.DeleteAmbassador ).once().with( &ambassador );
}

BOOST_FIXTURE_TEST_CASE( hla_plugin_can_destroy_federation, BuildFixture )
{
    xml::xistringstream xis( "<root destruction='true'/>" );
    xis >> xml::start( "root" );
    ::hla::MockRtiAmbassador ambassador;
    MOCK_EXPECT( rtiFactory.CreateAmbassador ).once().returns( &ambassador );
    MOCK_EXPECT( federateFactory.Create ).once().calls( boost::bind( &returnFederate, _1, _2, _3, federate ) );
    MOCK_EXPECT( federate->Join ).once().returns( true );
    FederateFacade facade( xis, controller, subject, localResolver, rtiFactory, federateFactory, "directory", callsignResolver,
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization );
    MOCK_EXPECT( federate->Destroy ).once().with( "Federation" ).returns( true );
    MOCK_EXPECT( rtiFactory.DeleteAmbassador ).once().with( &ambassador );
}

BOOST_FIXTURE_TEST_CASE( hla_plugin_steps, BuildFixture )
{
    xml::xistringstream xis( "<root/>" );
    xis >> xml::start( "root" );
    ::hla::MockRtiAmbassador ambassador;
    MOCK_EXPECT( rtiFactory.CreateAmbassador ).once().returns( &ambassador );
    MOCK_EXPECT( federateFactory.Create ).once().calls( boost::bind( &returnFederate, _1, _2, _3, federate ) );
    MOCK_EXPECT( federate->Join ).once().returns( true );
    FederateFacade facade( xis, controller, subject, localResolver, rtiFactory, federateFactory, "directory", callsignResolver,
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization );
    BOOST_REQUIRE( listener );
    MOCK_EXPECT( federate->Step ).once();
    sword::SimToClient_Content message;
    message.mutable_control_end_tick()->set_current_tick( 3 );
    listener->Notify( message, 42 );
    MOCK_EXPECT( rtiFactory.DeleteAmbassador ).once().with( &ambassador );
}

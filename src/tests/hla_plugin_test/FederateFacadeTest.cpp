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
#include "MockLogger.h"
#include "MockAgent.h"
#include "MockTacticalObject.h"
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
        dispatcher::MockLogger logger;
    };
    std::unique_ptr< Federate_ABC > returnFederate( ::hla::RtiAmbassador_ABC&, const std::string&, int, Federate_ABC* ptr )
    {
        return std::unique_ptr< Federate_ABC >( ptr );
    }
}
// TODO leaks
BOOST_FIXTURE_TEST_CASE( hla_plugin_initialization_declares_publications_with_netn_by_default, Fixture )
{
    xml::xistringstream xis( "<root disaggregate='true' netn-subscribe-rpr='true'/>" );
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
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization, logger );
    MOCK_EXPECT( tacticalObjectSubject.Unregister ).once();
    MOCK_EXPECT( subject.Unregister ).once();
    MOCK_EXPECT( controller.Unregister ).once().in( s );
    MOCK_EXPECT( rtiFactory.DeleteAmbassador ).once().with( &ambassador );
}

BOOST_FIXTURE_TEST_CASE( netn_use_can_be_desactivated, Fixture )
{
    xml::xistringstream xis( "<root netn='false' disaggregate='true'/>" );
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
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization, logger );
    MOCK_EXPECT( tacticalObjectSubject.Unregister ).once();
    MOCK_EXPECT( subject.Unregister ).once();
    MOCK_EXPECT( controller.Unregister ).once();
    MOCK_EXPECT( rtiFactory.DeleteAmbassador ).once().with( &ambassador );
}

namespace
{
    class MissingClassFixture : public Fixture
    {
    public:
        MissingClassFixture()
            : agentListener( 0 )
            , tacticalAgentListener( 0 )
            , xis( "<root disaggregate='true' netn-subscribe-rpr='true' netn='true'/>" )
        {
            MOCK_EXPECT( rtiFactory.CreateAmbassador ).once().in( s ).returns( &ambassador );
            MOCK_EXPECT( federateFactory.Create ).once().in( s ).calls( boost::bind( &returnFederate, _1, _2, _3, federate ) );
            MOCK_EXPECT( federate->Connect ).once().in( s ).returns( true );
            MOCK_EXPECT( federate->Join ).once().in( s ).returns( true );
            MOCK_EXPECT( subject.Register ).once().with( mock::retrieve( agentListener ) );
            MOCK_EXPECT( tacticalObjectSubject.Register ).once().with( mock::retrieve( tacticalAgentListener ) );
            MOCK_EXPECT( controller.Register ).once();
            xis >> xml::start( "root" );
        }
        ::hla::MockRtiAmbassador ambassador;
        xml::xistringstream xis;
        AgentListener_ABC *agentListener;
        TacticalObjectListener_ABC *tacticalAgentListener;
    };
    void ThrowOnClassName( const ::hla::ClassIdentifier& classId, ::hla::Class_ABC&, bool, bool, const std::string& className )
    {
        if( classId.ToString() == className )
        {
            throw std::runtime_error( "error" );
        }
    }
}

BOOST_FIXTURE_TEST_CASE( handle_missing_aggregate_class, MissingClassFixture )
{
    MOCK_EXPECT( federate->RegisterClass ).exactly( 14 ).calls( boost::bind( &ThrowOnClassName, _1, _2, _3, _4, "BaseEntity.AggregateEntity.NETN_Aggregate" ) );
    MOCK_EXPECT( logger.LogError ).once();
    FederateFacade facade( xis, controller, subject, localResolver, rtiFactory, federateFactory, "directory", callsignResolver,
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization, logger );
    BOOST_REQUIRE( agentListener );

    MockAgent agent;
    std::vector< char > uniqueId;
    agentListener->AggregateCreated( agent, 42, "agentName", rpr::Friendly, rpr::EntityType( "1 2 3 4 5 6 7" ), "agentSymbol", true, uniqueId );
    // cleanup
    MOCK_EXPECT( tacticalObjectSubject.Unregister ).once();
    MOCK_EXPECT( subject.Unregister ).once();
    MOCK_EXPECT( controller.Unregister ).once();
    MOCK_EXPECT( rtiFactory.DeleteAmbassador ).once().with( &ambassador );
}

BOOST_FIXTURE_TEST_CASE( handle_missing_aircraft_class, MissingClassFixture )
{
    MOCK_EXPECT( federate->RegisterClass ).exactly( 14 ).calls( boost::bind( &ThrowOnClassName, _1, _2, _3, _4, "BaseEntity.PhysicalEntity.Platform.Aircraft.NETN_Aircraft" ) );
    MOCK_EXPECT( logger.LogError ).once();
    FederateFacade facade( xis, controller, subject, localResolver, rtiFactory, federateFactory, "directory", callsignResolver,
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization, logger );
    BOOST_REQUIRE( agentListener );

    MockAgent agent;
    std::vector< char > uniqueId;
    agentListener->PlatformCreated( agent, 42, "agentName", rpr::Friendly, rpr::EntityType( "1 2 3 4 5 6 7" ), "agentSymbol", uniqueId );
    // cleanup
    MOCK_EXPECT( tacticalObjectSubject.Unregister ).once();
    MOCK_EXPECT( subject.Unregister ).once();
    MOCK_EXPECT( controller.Unregister ).once();
    MOCK_EXPECT( rtiFactory.DeleteAmbassador ).once().with( &ambassador );
}

BOOST_FIXTURE_TEST_CASE( handle_missing_vessel_class, MissingClassFixture )
{
    MOCK_EXPECT( federate->RegisterClass ).exactly( 14 ).calls( boost::bind( &ThrowOnClassName, _1, _2, _3, _4, "BaseEntity.PhysicalEntity.Platform.SurfaceVessel.NETN_SurfaceVessel" ) );
    MOCK_EXPECT( logger.LogError ).once();
    FederateFacade facade( xis, controller, subject, localResolver, rtiFactory, federateFactory, "directory", callsignResolver,
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization, logger );
    BOOST_REQUIRE( agentListener );

    MockAgent agent;
    std::vector< char > uniqueId;
    agentListener->PlatformCreated( agent, 42, "agentName", rpr::Friendly, rpr::EntityType( "1 3 3 4 5 6 7" ), "agentSymbol", uniqueId );
    // cleanup
    MOCK_EXPECT( tacticalObjectSubject.Unregister ).once();
    MOCK_EXPECT( subject.Unregister ).once();
    MOCK_EXPECT( controller.Unregister ).once();
    MOCK_EXPECT( rtiFactory.DeleteAmbassador ).once().with( &ambassador );
}

BOOST_FIXTURE_TEST_CASE( handle_missing_groundvehicle_class, MissingClassFixture )
{
    MOCK_EXPECT( federate->RegisterClass ).exactly( 14 ).calls( boost::bind( &ThrowOnClassName, _1, _2, _3, _4, "BaseEntity.PhysicalEntity.Platform.GroundVehicle.NETN_GroundVehicle" ) );
    MOCK_EXPECT( logger.LogError ).once();
    FederateFacade facade( xis, controller, subject, localResolver, rtiFactory, federateFactory, "directory", callsignResolver,
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization, logger );
    BOOST_REQUIRE( agentListener );

    MockAgent agent;
    std::vector< char > uniqueId;
    agentListener->PlatformCreated( agent, 42, "agentName", rpr::Friendly, rpr::EntityType( "1 1 3 4 5 6 7" ), "agentSymbol", uniqueId );
    // cleanup
    MOCK_EXPECT( tacticalObjectSubject.Unregister ).once();
    MOCK_EXPECT( subject.Unregister ).once();
    MOCK_EXPECT( controller.Unregister ).once();
    MOCK_EXPECT( rtiFactory.DeleteAmbassador ).once().with( &ambassador );
}

BOOST_FIXTURE_TEST_CASE( handle_missing_human_class, MissingClassFixture )
{
    MOCK_EXPECT( federate->RegisterClass ).exactly( 14 ).calls( boost::bind( &ThrowOnClassName, _1, _2, _3, _4, "BaseEntity.PhysicalEntity.Lifeform.Human.NETN_Human" ) );
    MOCK_EXPECT( logger.LogError ).once();
    FederateFacade facade( xis, controller, subject, localResolver, rtiFactory, federateFactory, "directory", callsignResolver,
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization, logger );
    BOOST_REQUIRE( agentListener );

    MockAgent agent;
    std::vector< char > uniqueId;
    agentListener->PlatformCreated( agent, 42, "agentName", rpr::Friendly, rpr::EntityType( "3 1 3 4 5 6 7" ), "agentSymbol", uniqueId );
    // cleanup
    MOCK_EXPECT( tacticalObjectSubject.Unregister ).once();
    MOCK_EXPECT( subject.Unregister ).once();
    MOCK_EXPECT( controller.Unregister ).once();
    MOCK_EXPECT( rtiFactory.DeleteAmbassador ).once().with( &ambassador );
}

BOOST_FIXTURE_TEST_CASE( handle_missing_minefield_class, MissingClassFixture )
{
    MOCK_EXPECT( federate->RegisterClass ).exactly( 14 ).calls( boost::bind( &ThrowOnClassName, _1, _2, _3, _4, "Minefield" ) );
    MOCK_EXPECT( logger.LogError ).once();
    FederateFacade facade( xis, controller, subject, localResolver, rtiFactory, federateFactory, "directory", callsignResolver,
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization, logger );
    BOOST_REQUIRE( tacticalAgentListener );

    MockTacticalObject object;
    std::vector< char > uniqueId;
    tacticalAgentListener->ObjectCreated( object, 42, "objectName", rpr::Friendly, rpr::EntityType( "3 1 3 4 5 6 7" ), true, TacticalObjectListener_ABC::eGeometryType_Polygon, false );
    // cleanup
    MOCK_EXPECT( tacticalObjectSubject.Unregister ).once();
    MOCK_EXPECT( subject.Unregister ).once();
    MOCK_EXPECT( controller.Unregister ).once();
    MOCK_EXPECT( rtiFactory.DeleteAmbassador ).once().with( &ambassador );
}

BOOST_FIXTURE_TEST_CASE( handle_missing_breachablepointobject_class, MissingClassFixture )
{
    MOCK_EXPECT( federate->RegisterClass ).exactly( 14 ).calls( boost::bind( &ThrowOnClassName, _1, _2, _3, _4, "EnvironmentObject.PointObject.BreachablePointObject" ) );
    MOCK_EXPECT( logger.LogError ).once();
    FederateFacade facade( xis, controller, subject, localResolver, rtiFactory, federateFactory, "directory", callsignResolver,
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization, logger );
    BOOST_REQUIRE( tacticalAgentListener );

    MockTacticalObject object;
    std::vector< char > uniqueId;
    tacticalAgentListener->ObjectCreated( object, 42, "objectName", rpr::Friendly, rpr::EntityType( "3 1 3 4 5 6 7" ), true, TacticalObjectListener_ABC::eGeometryType_Point, false );
    // cleanup
    MOCK_EXPECT( tacticalObjectSubject.Unregister ).once();
    MOCK_EXPECT( subject.Unregister ).once();
    MOCK_EXPECT( controller.Unregister ).once();
    MOCK_EXPECT( rtiFactory.DeleteAmbassador ).once().with( &ambassador );
}

BOOST_FIXTURE_TEST_CASE( handle_missing_breachablelinearobject_class, MissingClassFixture )
{
    MOCK_EXPECT( federate->RegisterClass ).exactly( 14 ).calls( boost::bind( &ThrowOnClassName, _1, _2, _3, _4, "EnvironmentObject.LinearObject.BreachableLinearObject" ) );
    MOCK_EXPECT( logger.LogError ).once();
    FederateFacade facade( xis, controller, subject, localResolver, rtiFactory, federateFactory, "directory", callsignResolver,
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization, logger );
    BOOST_REQUIRE( tacticalAgentListener );

    MockTacticalObject object;
    std::vector< char > uniqueId;
    tacticalAgentListener->ObjectCreated( object, 42, "objectName", rpr::Friendly, rpr::EntityType( "3 1 3 4 5 6 7" ), true, TacticalObjectListener_ABC::eGeometryType_Line, false );
    // cleanup
    MOCK_EXPECT( tacticalObjectSubject.Unregister ).once();
    MOCK_EXPECT( subject.Unregister ).once();
    MOCK_EXPECT( controller.Unregister ).once();
    MOCK_EXPECT( rtiFactory.DeleteAmbassador ).once().with( &ambassador );
}

BOOST_FIXTURE_TEST_CASE( handle_missing_otherpointobject_class, MissingClassFixture )
{
    MOCK_EXPECT( federate->RegisterClass ).exactly( 14 ).calls( boost::bind( &ThrowOnClassName, _1, _2, _3, _4, "EnvironmentObject.PointObject.OtherPointObject" ) );
    MOCK_EXPECT( logger.LogError ).once();
    FederateFacade facade( xis, controller, subject, localResolver, rtiFactory, federateFactory, "directory", callsignResolver,
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization, logger );
    BOOST_REQUIRE( tacticalAgentListener );

    MockTacticalObject object;
    std::vector< char > uniqueId;
    tacticalAgentListener->ObjectCreated( object, 42, "objectName", rpr::Friendly, rpr::EntityType( "3 1 3 4 5 6 7" ), false, TacticalObjectListener_ABC::eGeometryType_Point, false );
    // cleanup
    MOCK_EXPECT( tacticalObjectSubject.Unregister ).once();
    MOCK_EXPECT( subject.Unregister ).once();
    MOCK_EXPECT( controller.Unregister ).once();
    MOCK_EXPECT( rtiFactory.DeleteAmbassador ).once().with( &ambassador );
}

BOOST_FIXTURE_TEST_CASE( handle_missing_otherareaolbject_class, MissingClassFixture )
{
    MOCK_EXPECT( federate->RegisterClass ).exactly( 14 ).calls( boost::bind( &ThrowOnClassName, _1, _2, _3, _4, "EnvironmentObject.ArealObject.OtherArealObject" ) );
    MOCK_EXPECT( logger.LogError ).once();
    FederateFacade facade( xis, controller, subject, localResolver, rtiFactory, federateFactory, "directory", callsignResolver,
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization, logger );
    BOOST_REQUIRE( tacticalAgentListener );

    MockTacticalObject object;
    std::vector< char > uniqueId;
    tacticalAgentListener->ObjectCreated( object, 42, "objectName", rpr::Friendly, rpr::EntityType( "3 1 3 4 5 6 7" ), false, TacticalObjectListener_ABC::eGeometryType_Polygon, false );
    // cleanup
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
            MOCK_EXPECT( federate->RegisterClass ).exactly( 9 );
            MOCK_EXPECT( federate->Connect ).once().returns( true );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( hla_plugin_xml_options_overrides_default_values, BuildFixture )
{
    xml::xistringstream xis( "<root name='name' federation='federation' lrcSettings='crcHost=localhost;crcPort=8989'"
                             "      time-constrained='false' time-regulating='false' lookahead='3'/>" );
    xis >> xml::start( "root" );
    ::hla::MockRtiAmbassador ambassador;
    MOCK_EXPECT( rtiFactory.CreateAmbassador ).once().with( mock::any, mock::any, hla::RtiAmbassador_ABC::TimeStampOrder, "crcHost=localhost;crcPort=8989" ).returns( &ambassador );
    MOCK_EXPECT( federateFactory.Create ).once().with( mock::any, "name", 3 ).calls( boost::bind( &returnFederate, _1, _2, _3, federate ) );
    MOCK_EXPECT( federate->Join ).once().with( "federation", false, false ).returns( true );
    FederateFacade facade( xis, controller, subject, localResolver, rtiFactory, federateFactory, "directory", callsignResolver,
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization, logger );
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
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization, logger );
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
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization, logger );
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
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization, logger );
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
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization, logger );
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
                           tacticalObjectSubject, ownershipStrategy, entityIdResolver, fomSerialization, logger );
    BOOST_REQUIRE( listener );
    MOCK_EXPECT( federate->Step ).once();
    sword::SimToClient_Content message;
    message.mutable_control_end_tick()->set_current_tick( 3 );
    listener->Notify( message, 42 );
    MOCK_EXPECT( rtiFactory.DeleteAmbassador ).once().with( &ambassador );
}

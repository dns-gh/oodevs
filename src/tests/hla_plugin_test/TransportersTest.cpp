// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/Transporters.h"
#include "MockTransportedUnitsVisitor.h"
#include "MockCallsignResolver.h"
#include "MockAgent.h"
#include "MockAgentSubject.h"
#include "MockSideChecker.h"

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : agentListener                      ( 0 )
            , eventListener                      ( 0 )
            , transporterIdentifier              ( 42u )
            , transportedUnitUniqueId            ( "uniqueId" )
            , transportedUnitSimulationIdentifier( 43u )
            , embarkmentPoint                    ( 6., 7. )
        {
            MOCK_EXPECT( agentSubject, Register ).once().with( mock::retrieve( agentListener ) );
            MOCK_EXPECT( agentSubject, Unregister ).once();
        }
        MockAgentSubject agentSubject;
        MockCallsignResolver callsignResolver;
        MockSideChecker sideChecker;
        MockAgent transporter;
        MockTransportedUnitsVisitor visitor;
        AgentListener_ABC* agentListener;
        EventListener_ABC* eventListener;
        const unsigned int transporterIdentifier;
        const std::string transportedUnitUniqueId;
        const unsigned int transportedUnitSimulationIdentifier;
        const geometry::Point2d embarkmentPoint;
    };
    class RegisteredFixture : public Fixture
    {
    public:
        RegisteredFixture()
            : transporters( agentSubject, callsignResolver, sideChecker )
        {
            BOOST_REQUIRE( agentListener );
            MOCK_EXPECT( transporter, Register ).once().with( mock::retrieve( eventListener ) );
            agentListener->SurfaceVesselCreated( transporter, transporterIdentifier, "transporter", rpr::Friendly, rpr::EntityType() );
            BOOST_REQUIRE( eventListener );
        }
        Transporters transporters;
    };
}

BOOST_FIXTURE_TEST_CASE( transporters_finds_transporter, RegisteredFixture )
{
    eventListener->SpatialChanged( 1., 2., 3., 4., 5. );
    MOCK_EXPECT( callsignResolver, ResolveCallsign ).once().with( transporterIdentifier ).returns( "surface vessel" );
    MOCK_EXPECT( callsignResolver, ResolveUniqueId ).once().with( transporterIdentifier ).returns( "unique vessel" );
    MOCK_EXPECT( callsignResolver, ResolveSimulationIdentifier ).once().with( transportedUnitUniqueId ).returns( transportedUnitSimulationIdentifier );
    MOCK_EXPECT( sideChecker, AreSameSide ).with( transporterIdentifier, transportedUnitSimulationIdentifier ).returns( true );
    MOCK_EXPECT( visitor, Notify ).once().with( "surface vessel", "unique vessel" );
    transporters.Apply( transportedUnitUniqueId, embarkmentPoint, visitor );
}

BOOST_FIXTURE_TEST_CASE( transporters_does_not_find_transporter_on_different_side, RegisteredFixture )
{
    eventListener->SpatialChanged( 1., 2., 3., 4., 5. );
    MOCK_EXPECT( callsignResolver, ResolveSimulationIdentifier ).once().with( transportedUnitUniqueId ).returns( transportedUnitSimulationIdentifier );
    MOCK_EXPECT( sideChecker, AreSameSide ).with( transporterIdentifier, transportedUnitSimulationIdentifier ).returns( false );
    transporters.Apply( transportedUnitUniqueId, embarkmentPoint, visitor );
}

BOOST_FIXTURE_TEST_CASE( transporters_finds_closest_transporter, RegisteredFixture )
{
    eventListener->SpatialChanged( 1., 2., 3., 4., 5. );
    MockAgent closestTransporter;
    EventListener_ABC* closestListener = 0;
    const unsigned int closesTransporterIdentifier = 142u;
    MOCK_EXPECT( closestTransporter, Register ).once().with( mock::retrieve( closestListener ) );
    agentListener->SurfaceVesselCreated( closestTransporter, closesTransporterIdentifier, "closest transporter", rpr::Friendly, rpr::EntityType() );
    BOOST_REQUIRE( closestListener );
    closestListener->SpatialChanged( 6., 7., 3., 4., 5. );
    MOCK_EXPECT( callsignResolver, ResolveCallsign ).once().with( closesTransporterIdentifier ).returns( "closest vessel" );
    MOCK_EXPECT( callsignResolver, ResolveUniqueId ).once().with( closesTransporterIdentifier ).returns( "unique closest" );
    MOCK_EXPECT( callsignResolver, ResolveSimulationIdentifier ).once().with( transportedUnitUniqueId ).returns( transportedUnitSimulationIdentifier );
    MOCK_EXPECT( sideChecker, AreSameSide ).with( transporterIdentifier, transportedUnitSimulationIdentifier ).returns( true );
    MOCK_EXPECT( sideChecker, AreSameSide ).with( closesTransporterIdentifier, transportedUnitSimulationIdentifier ).returns( true );
    MOCK_EXPECT( visitor, Notify ).once().with( "closest vessel", "unique closest" );
    transporters.Apply( transportedUnitUniqueId, embarkmentPoint, visitor );
}

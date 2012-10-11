// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/RemoteTacticalObjectController.h"

#include "MockExtentResolver.h"
#include "MockSideResolver.h"
#include "MockEntityTypeResolver.h"
#include "MockSimulationPublisher.h"
#include "MockRemoteTacticalObjectSubject.h"
#include "MockHlaObject.h"
#include "MockHlaClass.h"
#include "MockTeam.h"
#include "MockLogger.h"

#include "protocol/SimulationSenders.h"

#include <boost/foreach.hpp>

using namespace plugins::hla;

namespace
{
struct Fixture
{
    Fixture()
        : remoteListener( 0 )
    {
        MOCK_EXPECT( remoteSubject.RegisterTactical ).once().with( mock::retrieve( remoteListener ) );
        MOCK_EXPECT( remoteSubject.UnregisterTactical ).once();
    }
    MockExtentResolver extent;
    MockSideResolver sideResolver;
    rpr::MockEntityTypeResolver objectTypeResolver;
    dispatcher::MockSimulationPublisher publisher;
    MockRemoteTacticalObjectSubject remoteSubject;
    ClassListener_ABC* remoteListener;
    dispatcher::MockLogger logger;
};

struct ControllerFixture : Fixture
{
    ControllerFixture()
        : controller( extent, sideResolver, objectTypeResolver, publisher, remoteSubject, logger )
    {
        BOOST_CHECK( remoteListener );
    }
    RemoteTacticalObjectController controller;
};

}

BOOST_FIXTURE_TEST_CASE( remote_tactical_object_controller_creates_point_object, ControllerFixture )
{
    MockHlaClass clazz;
    MockHlaObject object;
    dispatcher::MockTeam team( 18 );
    ObjectListener_ABC* objectListener = 0;
    rpr::EntityType objectType("1 2 3 4 5 6 7");
    std::vector< rpr::PerimeterPoint > v;
    v.push_back( rpr::PerimeterPoint( 12.f, 13.f ) ) ;
    
    MOCK_EXPECT( object.Register ).once().with( mock::retrieve( objectListener ) );
    remoteListener->RemoteCreated( "an_object", clazz, object ); 
    BOOST_CHECK( objectListener );

    MOCK_EXPECT( objectTypeResolver.Resolve ).once().with( mock::same( objectType ), mock::assign( "an_object_type" ) ).returns( true );
    objectListener->TypeChanged( "an_object", objectType );

    objectListener->Moved( "an_object", 42.0, 42.1 );

    MOCK_EXPECT( sideResolver.ResolveTeam ).once().with( rpr::Friendly ).returns( team.GetId() );
    objectListener->SideChanged( "an_object", rpr::Friendly );
    objectListener->NameChanged( "an_object", "an_object_name");
    
    sword::ClientToSim actual;
    MOCK_EXPECT( publisher.SendClientToSim ).once().with( mock::retrieve( actual ) );
    objectListener->PerimeterChanged( "an_object", v);

    BOOST_ASSERT( actual.message().has_object_magic_action() );
    const sword::ObjectMagicAction& action = actual.message().object_magic_action();
    BOOST_CHECK_EQUAL( action.type(), sword::ObjectMagicAction::create );
    BOOST_CHECK_EQUAL( action.parameters().elem_size(), 4 );
    BOOST_CHECK_EQUAL( action.parameters().elem( 0 ).value( 0 ).acharstr(), std::string( "an_object_type" ) );
    BOOST_CHECK_EQUAL( action.parameters().elem( 2 ).value( 0 ).acharstr(), std::string( "an_object_name" ) );
    BOOST_CHECK_EQUAL( action.parameters().elem( 3 ).value( 0 ).party().id(), team.GetId() );
    const sword::Location& loc = action.parameters().elem( 1 ).value( 0 ).location();
    BOOST_CHECK_EQUAL( loc.type(), sword::Location_Geometry_point );
    BOOST_CHECK_EQUAL( loc.coordinates().elem_size(), (int)v.size() );
    const sword::CoordLatLong& c( loc.coordinates().elem( 0 ) );
    BOOST_CHECK_EQUAL( c.latitude(), 42.0 );
    BOOST_CHECK_EQUAL( c.longitude(), 42.1 );
}

BOOST_FIXTURE_TEST_CASE( remote_tactical_object_controller_creates_polygon_object, ControllerFixture )
{
    MockHlaClass clazz;
    MockHlaObject object;
    dispatcher::MockTeam team( 18 );
    ObjectListener_ABC* objectListener = 0;
    rpr::EntityType objectType("1 2 3 4 5 6 7");
    std::vector< rpr::PerimeterPoint > v;
    v.push_back( rpr::PerimeterPoint( 12.f, 13.f ) ) ;
    v.push_back( rpr::PerimeterPoint( 16.f, 17.f ) ) ;
    v.push_back( rpr::PerimeterPoint( 12.f, 13.f ) ) ;
    
    MOCK_EXPECT( object.Register ).once().with( mock::retrieve( objectListener ) );
    remoteListener->RemoteCreated( "an_object", clazz, object ); 
    BOOST_CHECK( objectListener );

    MOCK_EXPECT( objectTypeResolver.Resolve ).once().with( mock::same( objectType ), mock::assign( "an_object_type" ) ).returns( true );
    objectListener->TypeChanged( "an_object", objectType );

    objectListener->Moved( "an_object", 42.0, 42.1 );

    MOCK_EXPECT( sideResolver.ResolveTeam ).once().with( rpr::Friendly ).returns( team.GetId() );
    objectListener->SideChanged( "an_object", rpr::Friendly );
    objectListener->NameChanged( "an_object", "an_object_name");
    
    sword::ClientToSim actual;
    MOCK_EXPECT( publisher.SendClientToSim ).once().with( mock::retrieve( actual ) );
    objectListener->PerimeterChanged( "an_object", v);

    BOOST_ASSERT( actual.message().has_object_magic_action() );
    const sword::ObjectMagicAction& action = actual.message().object_magic_action();
    BOOST_CHECK_EQUAL( action.type(), sword::ObjectMagicAction::create );
    BOOST_CHECK_EQUAL( action.parameters().elem_size(), 4 );
    BOOST_CHECK_EQUAL( action.parameters().elem( 0 ).value( 0 ).acharstr(), std::string( "an_object_type" ) );
    BOOST_CHECK_EQUAL( action.parameters().elem( 2 ).value( 0 ).acharstr(), std::string( "an_object_name" ) );
    BOOST_CHECK_EQUAL( action.parameters().elem( 3 ).value( 0 ).party().id(), team.GetId() );
    const sword::Location& loc = action.parameters().elem( 1 ).value( 0 ).location();
    BOOST_CHECK_EQUAL( loc.type(), sword::Location_Geometry_polygon );
    BOOST_CHECK_EQUAL( loc.coordinates().elem_size(), (int)v.size() );
}
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
#include "MockContextHandler.h"
#include "MockPropagationManager.h"

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
    ~Fixture()
    {
    }
    MockExtentResolver extent;
    MockSideResolver sideResolver;
    rpr::MockEntityTypeResolver objectTypeResolver;
    MockContextHandler< sword::ObjectMagicActionAck > ctxHandler;
    MockRemoteTacticalObjectSubject remoteSubject;
    ClassListener_ABC* remoteListener;
    dispatcher::MockLogger logger;
};

struct ControllerFixture : Fixture
{
    ControllerFixture()
        : controller( extent, sideResolver, objectTypeResolver, ctxHandler, remoteSubject, logger, propagationManager )
    {
        BOOST_CHECK( remoteListener );
    }
    MockPropagationManager propagationManager;
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
    
    MOCK_EXPECT( logger.LogInfo ).exactly( 4 );

    MOCK_EXPECT( object.Register ).once().with( mock::retrieve( objectListener ) );
    remoteListener->RemoteCreated( "an_object", clazz, object ); 
    BOOST_CHECK( objectListener );

    MOCK_EXPECT( objectTypeResolver.Resolve ).once().with( mock::same( objectType ), mock::assign( "an_object_type" ) ).returns( true );
    objectListener->TypeChanged( "an_object", objectType );


    MOCK_EXPECT( sideResolver.ResolveTeam ).once().with( rpr::Friendly ).returns( team.GetId() );
    objectListener->SideChanged( "an_object", rpr::Friendly );
    objectListener->NameChanged( "an_object", "an_object_name");
    
    simulation::ObjectMagicAction actual;
    MOCK_EXPECT( ctxHandler.SendObject ).once().with( mock::retrieve( actual ), mock::any );
    objectListener->Moved( "an_object", 42.0, 42.1 );

    const sword::ObjectMagicAction& action=actual();
    BOOST_CHECK_EQUAL( action.type(), sword::ObjectMagicAction::create );
    BOOST_CHECK_EQUAL( action.parameters().elem_size(), 6 );
    BOOST_CHECK_EQUAL( action.parameters().elem( 0 ).value( 0 ).acharstr(), std::string( "an_object_type" ) );
    BOOST_CHECK_EQUAL( action.parameters().elem( 2 ).value( 0 ).acharstr(), std::string( "an_object_name" ) );
    BOOST_CHECK_EQUAL( action.parameters().elem( 3 ).value( 0 ).party().id(), team.GetId() );
    const sword::Location& loc = action.parameters().elem( 1 ).value( 0 ).location();
    BOOST_CHECK_EQUAL( loc.type(), sword::Location_Geometry_point );
    BOOST_CHECK_EQUAL( loc.coordinates().elem_size(), 1 );
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
    std::vector< rpr::WorldLocation > v;
    v.push_back( rpr::WorldLocation( 12.f, 13.f, 0.f ) ) ;
    v.push_back( rpr::WorldLocation( 16.f, 17.f, 0.f ) ) ;
    v.push_back( rpr::WorldLocation( 12.f, 13.f, 0.f ) ) ;
    
    MOCK_EXPECT( logger.LogInfo ).exactly( 4 );

    MOCK_EXPECT( object.Register ).once().with( mock::retrieve( objectListener ) );
    remoteListener->RemoteCreated( "an_object", clazz, object ); 
    BOOST_CHECK( objectListener );

    MOCK_EXPECT( objectTypeResolver.Resolve ).once().with( mock::same( objectType ), mock::assign( "an_object_type" ) ).returns( true );
    objectListener->TypeChanged( "an_object", objectType );

    MOCK_EXPECT( sideResolver.ResolveTeam ).once().with( rpr::Friendly ).returns( team.GetId() );
    objectListener->SideChanged( "an_object", rpr::Friendly );
    objectListener->NameChanged( "an_object", "an_object_name");
    
    simulation::ObjectMagicAction actual;
    MOCK_EXPECT( ctxHandler.SendObject ).once().with( mock::retrieve( actual ), mock::any );
    objectListener->GeometryChanged( "an_object", v, ObjectListener_ABC::eGeometryType_Polygon );

    const sword::ObjectMagicAction& action=actual();
    BOOST_CHECK_EQUAL( action.type(), sword::ObjectMagicAction::create );
    BOOST_CHECK_EQUAL( action.parameters().elem_size(), 6 );
    BOOST_CHECK_EQUAL( action.parameters().elem( 0 ).value( 0 ).acharstr(), std::string( "an_object_type" ) );
    BOOST_CHECK_EQUAL( action.parameters().elem( 2 ).value( 0 ).acharstr(), std::string( "an_object_name" ) );
    BOOST_CHECK_EQUAL( action.parameters().elem( 3 ).value( 0 ).party().id(), team.GetId() );
    const sword::Location& loc = action.parameters().elem( 1 ).value( 0 ).location();
    BOOST_CHECK_EQUAL( loc.type(), sword::Location_Geometry_polygon );
    BOOST_CHECK_EQUAL( loc.coordinates().elem_size(), (int)v.size() );
}

BOOST_FIXTURE_TEST_CASE( remote_tactical_object_controller_creates_line_object, ControllerFixture )
{
    MockHlaClass clazz;
    MockHlaObject object;
    dispatcher::MockTeam team( 18 );
    ObjectListener_ABC* objectListener = 0;
    rpr::EntityType objectType("1 2 3 4 5 6 7");
    std::vector< rpr::WorldLocation > v;
    v.push_back( rpr::WorldLocation( 12.f, 13.f, 0.f ) ) ;
    v.push_back( rpr::WorldLocation( 16.f, 17.f, 0.f ) ) ;
    v.push_back( rpr::WorldLocation( 12.f, 13.f, 0.f ) ) ;

    MOCK_EXPECT( logger.LogInfo ).exactly( 4 );

    MOCK_EXPECT( object.Register ).once().with( mock::retrieve( objectListener ) );
    remoteListener->RemoteCreated( "an_object", clazz, object );
    BOOST_CHECK( objectListener );

    MOCK_EXPECT( objectTypeResolver.Resolve ).once().with( mock::same( objectType ), mock::assign( "an_object_type" ) ).returns( true );
    objectListener->TypeChanged( "an_object", objectType );

    MOCK_EXPECT( sideResolver.ResolveTeam ).once().with( rpr::Friendly ).returns( team.GetId() );
    objectListener->SideChanged( "an_object", rpr::Friendly );
    objectListener->NameChanged( "an_object", "an_object_name");

    simulation::ObjectMagicAction actual;
    MOCK_EXPECT( ctxHandler.SendObject ).once().with( mock::retrieve( actual ), mock::any );
    objectListener->GeometryChanged( "an_object", v, ObjectListener_ABC::eGeometryType_Line );

    const sword::ObjectMagicAction& action=actual();
    BOOST_CHECK_EQUAL( action.type(), sword::ObjectMagicAction::create );
    BOOST_CHECK_EQUAL( action.parameters().elem_size(), 6 );
    BOOST_CHECK_EQUAL( action.parameters().elem( 0 ).value( 0 ).acharstr(), std::string( "an_object_type" ) );
    BOOST_CHECK_EQUAL( action.parameters().elem( 2 ).value( 0 ).acharstr(), std::string( "an_object_name" ) );
    BOOST_CHECK_EQUAL( action.parameters().elem( 3 ).value( 0 ).party().id(), team.GetId() );
    const sword::Location& loc = action.parameters().elem( 1 ).value( 0 ).location();
    BOOST_CHECK_EQUAL( loc.type(), sword::Location_Geometry_line );
    BOOST_CHECK_EQUAL( loc.coordinates().elem_size(), (int)v.size() );
}

BOOST_FIXTURE_TEST_CASE( remote_tactical_object_controller_creates_disaster_object, ControllerFixture )
{
    MockHlaClass clazz;
    MockHlaObject object;
    dispatcher::MockTeam team( 18 );
    ObjectListener_ABC* objectListener = 0;
    rpr::EntityType objectType("1 2 3 4 5 6 7");

    MOCK_EXPECT( logger.LogInfo );

    MOCK_EXPECT( object.Register ).once().with( mock::retrieve( objectListener ) );
    remoteListener->RemoteCreated( "an_object", clazz, object );
    BOOST_CHECK( objectListener );

    MOCK_EXPECT( objectTypeResolver.Resolve ).once().with( mock::same( objectType ), mock::assign( "an_object_type" ) ).returns( true );
    objectListener->TypeChanged( "an_object", objectType );

    std::vector< ObjectListener_ABC::PropagationData > datas;
    int col=5, lig=5;
    for( int l = 0; l< lig; ++l)
        for( int c = 0; c < col; ++c)
        {
            ObjectListener_ABC::PropagationData data(3+c*1./10, 42+l*1./10, 1+(l+c*1.)/100.);
            datas.push_back( data );
        }
    simulation::ObjectMagicAction actual;
    MOCK_EXPECT( ctxHandler.SendObject ).once();
    MOCK_EXPECT( propagationManager.saveDataFile ).once();
    objectListener->PropagationChanged( "an_object", datas, col, lig, 3, 42, 1, 2 );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/LocationOwnershipPolicy.h"
#include "hla_plugin/ObjectListener_ABC.h"
#include "MockMessageController.h"
#include "MockOwnershipController.h"
#include "MockRemoteAgentSubject.h"
#include "MockTransferSender.h"
#include "MockHlaClass.h"
#include "MockHlaObject.h"
#include "protocol/Protocol.h"

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : classListener( 0 )
        {
            MOCK_EXPECT( messageController.Register ).once().with( mock::retrieve( shapeCreationListener ) );
            MOCK_EXPECT( messageController.Register ).once().with( mock::retrieve( shapeDestructionListener ) );
            MOCK_EXPECT( remoteSubject.Register ).once().with( mock::retrieve( classListener ) );
            mock::reset( transferSender );
        }
        ::tools::MockMessageController< ::sword::MessengerToClient_Content > messageController;
        tools::MessageHandler_ABC< sword::MessengerToClient_Content >* shapeCreationListener;
        tools::MessageHandler_ABC< sword::MessengerToClient_Content >* shapeDestructionListener;
        MockRemoteAgentSubject remoteSubject;
        MockTransferSender transferSender;
        ClassListener_ABC* classListener;
        std::vector< ::hla::AttributeIdentifier > attributes;
    };
    class StrategyFixture : public Fixture
    {
    public:
        StrategyFixture()
            : policy( messageController, ownershipController, remoteSubject, transferSender, "Misc./remote zone" )
            , objectListener( 0 )
        {
            BOOST_REQUIRE( shapeCreationListener );
            BOOST_REQUIRE( shapeDestructionListener );
            BOOST_REQUIRE( classListener );
            MOCK_EXPECT( object.Register ).once().with( mock::retrieve( objectListener ) );
            classListener->LocalCreated( "identifier", hlaClass, object );
        }
        ~StrategyFixture()
        {
            MOCK_EXPECT( object.Unregister );
            MOCK_EXPECT( messageController.Unregister );
            MOCK_EXPECT( remoteSubject.Unregister );
        }
        MockHlaClass hlaClass;
        MockHlaObject object;
        LocationOwnershipPolicy policy;
        MockOwnershipController ownershipController;
        ObjectListener_ABC* objectListener;
    };
    void AddPoint( sword::CoordLatLongList& list, double lat, double lon)
    {
        sword::CoordLatLong p;
        p.set_latitude( lat );
        p.set_longitude( lon );
        list.add_elem()->Swap(&p);
    }
    sword::MessengerToClient_Content MakeShapeCreation( unsigned long id )
    {
        sword::MessengerToClient_Content message;
        sword::ShapeCreation* shape = message.mutable_shape_creation();
        shape->mutable_shape()->set_category( "Misc." );
        shape->mutable_shape()->set_pattern( "remote zone" );
        AddPoint( *shape->mutable_shape()->mutable_points(), 10, 10 );
        AddPoint( *shape->mutable_shape()->mutable_points(), 10, 100 );
        AddPoint( *shape->mutable_shape()->mutable_points(), 100, 100 );
        AddPoint( *shape->mutable_shape()->mutable_points(), 100, 10 );
        shape->mutable_id()->set_id( id );
        return message;
    }
    sword::MessengerToClient_Content MakeShapeDestruction( unsigned long id )
    {
        sword::MessengerToClient_Content message;
        sword::ShapeDestruction* shape = message.mutable_shape_destruction();
        shape->mutable_id()->set_id( id );
        return message;
    }
}

BOOST_FIXTURE_TEST_CASE( ownership_strategy_no_shape, StrategyFixture )
{
    BOOST_CHECK_EQUAL( policy.IsInDivestureArea( 42, 43), false );
}

BOOST_FIXTURE_TEST_CASE( ownership_strategy_with_shape, StrategyFixture )
{
    sword::MessengerToClient_Content message(MakeShapeCreation( 42 ) );
    shapeCreationListener->Notify( message, 42 );
    BOOST_CHECK_EQUAL( policy.IsInDivestureArea( 42, 43), true );
}

BOOST_FIXTURE_TEST_CASE( ownership_strategy_add_remove, StrategyFixture )
{
    sword::MessengerToClient_Content create( MakeShapeCreation( 42 ) );
    shapeCreationListener->Notify( create, 42 );
    BOOST_CHECK_EQUAL( policy.IsInDivestureArea( 42, 43), true );

    sword::MessengerToClient_Content destr( MakeShapeDestruction( 42 ) );
    shapeDestructionListener->Notify( destr, 42 );
}

namespace
{
    class FilledFixture : public StrategyFixture
    {
    public:
        FilledFixture()
            : StrategyFixture()
        {
            sword::MessengerToClient_Content create( MakeShapeCreation( 42 ) );
            shapeCreationListener->Notify( create, 42 );
            BOOST_CHECK_EQUAL( policy.IsInDivestureArea( 42, 43), true );
        }
        ~FilledFixture()
        {
            sword::MessengerToClient_Content destr( MakeShapeDestruction( 42 ) );
            shapeDestructionListener->Notify( destr, 42 );
        }
    };
    struct Callback
    {
        void Capture(const std::string& , const TransferSender_ABC::TransferRequestCallback& callback )
        {
            cb_ =callback;
        }
        void operator()(bool v)
        {
            cb_(v);
        }
    private:
        TransferSender_ABC::TransferRequestCallback cb_;
    };

}

BOOST_FIXTURE_TEST_CASE( location_ownership_stategy_move_notransfer, FilledFixture )
{
    objectListener->Moved( "identifier", 120, 120 );
}

BOOST_FIXTURE_TEST_CASE( location_ownership_stategy_move_transfer_negative_ack, FilledFixture )
{
    Callback callback;
    MOCK_EXPECT( hlaClass.GetAttributes ).once().returns( attributes );
    MOCK_EXPECT( transferSender.RequestTransfer ).once().with( "identifier", mock::any, TransferSender_ABC::E_EntityPush, attributes, mock::any, mock::any ).calls( boost::bind( &Callback::Capture, boost::ref(callback), _1, _2 ) );
    objectListener->Moved( "identifier", 42, 43 );
    callback( false );
}

/*
BOOST_FIXTURE_TEST_CASE( location_ownership_stategy_move_transfer_negative_ack_moveagain, FilledFixture )
{
    Callback callback;
    MOCK_EXPECT( hlaClass.GetAttributes ).returns( boost::cref( attributes ) );
    MOCK_EXPECT( transferSender.RequestTransfer ).once().with( "identifier", mock::any, TransferSender_ABC::E_EntityPush, attributes ).calls( boost::bind( &Callback::Capture, boost::ref(callback), _1, _2 ) );
    objectListener->Moved( "identifier", 42, 43 );
    callback( false );
    objectListener->Moved( "identifier", 42, 43 );
}

BOOST_FIXTURE_TEST_CASE( location_ownership_stategy_move_transfer_negative_ack_out_in, FilledFixture )
{
    Callback callback;
    MOCK_EXPECT( hlaClass.GetAttributes ).once().returns( attributes );
    MOCK_EXPECT( transferSender.RequestTransfer ).once().with( "identifier", mock::any, TransferSender_ABC::E_EntityPush, attributes ).calls( boost::bind( &Callback::Capture, boost::ref(callback), _1, _2 ) );
    objectListener->Moved( "identifier", 42, 43 );
    callback( false );
    objectListener->Moved( "identifier", 42, 120 );
    MOCK_EXPECT( hlaClass.GetAttributes ).once().returns( attributes );
    MOCK_EXPECT( transferSender.RequestTransfer ).once().with( "identifier", mock::any, TransferSender_ABC::E_EntityPush, attributes ).calls( boost::bind( &Callback::Capture, boost::ref(callback), _1, _2 ) );
    objectListener->Moved( "identifier", 42, 43 );

}

BOOST_FIXTURE_TEST_CASE( location_ownership_stategy_transfer_pull_back, FilledFixture )
{
    Callback callback;
    MOCK_EXPECT( hlaClass.GetAttributes ).once().returns( attributes );
    MOCK_EXPECT( transferSender.RequestTransfer ).once().with( "identifier", mock::any, TransferSender_ABC::E_EntityPush, attributes ).calls( boost::bind( &Callback::Capture, boost::ref(callback), _1, _2 ) );
    objectListener->Moved( "identifier", 42, 43 );
    MOCK_EXPECT( hlaClass.GetAttributes ).once().returns( attributes );
    MOCK_EXPECT(ownershipController.PerformDivestiture );
    callback( true );
    classListener->Divested( "identifier" );
    MOCK_EXPECT( hlaClass.GetAttributes ).once().returns( attributes );
    MOCK_EXPECT( transferSender.RequestTransfer ).once().with( "identifier", mock::any, TransferSender_ABC::E_EntityPull, attributes ).calls( boost::bind( &Callback::Capture, boost::ref(callback), _1, _2 ) );
    objectListener->Moved( "identifier", 42, 120 );
    classListener->Acquired( "identifier" );
}*/

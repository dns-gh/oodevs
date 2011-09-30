// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/EquipmentUpdater.h"
#include "hla_plugin/ComponentTypeVisitor_ABC.h"
#include "MockRemoteAgentSubject.h"
#include "MockContextHandler.h"
#include "MockContextFactory.h"
#include "MockSimulationPublisher.h"
#include "MockComponentTypes.h"
#include "MockEntityTypeResolver.h"
#include "protocol/SimulationSenders.h"

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : remoteAgentListener( 0 )
            , responseObserver   ( 0 )
        {
            MOCK_EXPECT( factory, Create ).returns( 888 );
            MOCK_EXPECT( subject, Register ).once().with( mock::retrieve( remoteAgentListener ) );
            MOCK_EXPECT( subject, Unregister );
            MOCK_EXPECT( handler, Register ).once().with( mock::retrieve( responseObserver ) );
            MOCK_EXPECT( handler, Unregister );
        }
        RemoteAgentListener_ABC* remoteAgentListener;
        ResponseObserver_ABC< sword::UnitCreation >* responseObserver;
        MockRemoteAgentSubject subject;
        MockContextHandler< sword::UnitCreation > handler;
        dispatcher::MockSimulationPublisher publisher;
        MockContextFactory factory;
        rpr::MockEntityTypeResolver resolver;
        MockComponentTypes componentTypes;
        sword::ClientToSim message;
    };
    class RegisteredFixture : public Fixture
    {
    public:
        RegisteredFixture()
            : updater            ( subject, handler, publisher, factory, resolver, componentTypes )
            , componentEntityType( "1 1 2 0 0 0 0" )
            , componentTypeName  ( "component type name" )
            , unitId             ( 1337 )
            , agentTypeId        ( 42 )
            , componentTypeId    ( 43 )
        {
            BOOST_REQUIRE( remoteAgentListener );
            BOOST_REQUIRE( responseObserver );
        }
        sword::UnitCreation MakeMessage( unsigned int agentTypeId, unsigned int unitId )
        {
            sword::UnitCreation creationMessage;
            creationMessage.mutable_type()->set_id( agentTypeId );
            creationMessage.mutable_unit()->set_id( unitId );
            return creationMessage;
        }
        void CheckMessage( const sword::ClientToSim& message, unsigned int unitId, unsigned int componentTypeId,
                           int undamaged, int destroyed )
        {
            BOOST_CHECK( message.message().has_unit_magic_action() );
            const sword::UnitMagicAction& action = message.message().unit_magic_action();
            BOOST_CHECK_EQUAL( action.type(), sword::UnitMagicAction::change_equipment_state );
            BOOST_CHECK_EQUAL( action.tasker().unit().id(), unitId );
            BOOST_CHECK_EQUAL( action.parameters().elem_size(), 1 );
            BOOST_CHECK_EQUAL( action.parameters().elem( 0 ).value_size(), 1 );
            const sword::MissionParameter_Value& componentChanged = action.parameters().elem( 0 ).value( 0 );
            BOOST_CHECK_EQUAL( componentChanged.list_size(), 8 );
            BOOST_CHECK_EQUAL( componentChanged.list( 0 ).identifier(), componentTypeId );
            BOOST_CHECK_EQUAL( componentChanged.list( 1 ).quantity(), undamaged );
            BOOST_CHECK_EQUAL( componentChanged.list( 2 ).quantity(), destroyed );
            BOOST_CHECK_EQUAL( componentChanged.list( 3 ).quantity(), 0 );
            BOOST_CHECK_EQUAL( componentChanged.list( 4 ).quantity(), 0 );
            BOOST_CHECK_EQUAL( componentChanged.list( 5 ).quantity(), 0 );
            BOOST_CHECK_EQUAL( componentChanged.list( 6 ).quantity(), 0 );
            BOOST_CHECK_EQUAL( componentChanged.list( 7 ).list_size(), 0 );
        }
        EquipmentUpdater updater;
        const std::string componentEntityType;
        const std::string componentTypeName;
        const unsigned int unitId;
        const unsigned int agentTypeId;
        const unsigned int componentTypeId;
    };
}

BOOST_FIXTURE_TEST_CASE( equipment_updater_sends_message_when_unit_creation_is_received_after_remote_component_update, RegisteredFixture )
{
    const int undamaged = 2;
    const int componentNumber = 10;
    const int destroyed = componentNumber - undamaged;
    MOCK_EXPECT( resolver, Resolve ).once().with( rpr::EntityType( componentEntityType ) ).returns( componentTypeName );
    remoteAgentListener->EquipmentUpdated( "remote", rpr::EntityType( componentEntityType ), undamaged );
    MOCK_EXPECT( componentTypes, Apply ).once().with( agentTypeId, mock::any ).calls( boost::bind( &ComponentTypeVisitor_ABC::NotifyEquipment, _2, componentTypeId, componentTypeName, componentNumber ) );
    MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( message ) );
    responseObserver->Notify( MakeMessage( agentTypeId, unitId ), "remote" );
    CheckMessage( message, unitId, componentTypeId, undamaged, destroyed );
}

BOOST_FIXTURE_TEST_CASE( equipment_updater_sends_message_when_remote_component_update_is_received_after_unit_creation, RegisteredFixture )
{
    const int undamaged = 2;
    const int componentNumber = 10;
    const int destroyed = componentNumber - undamaged;
    MOCK_EXPECT( componentTypes, Apply ).once().with( agentTypeId, mock::any ).calls( boost::bind( &ComponentTypeVisitor_ABC::NotifyEquipment, _2, componentTypeId, componentTypeName, componentNumber ) );
    responseObserver->Notify( MakeMessage( agentTypeId, unitId ), "remote" );
    MOCK_EXPECT( resolver, Resolve ).once().with( rpr::EntityType( componentEntityType ) ).returns( componentTypeName );
    MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( message ) );
    remoteAgentListener->EquipmentUpdated( "remote", rpr::EntityType( componentEntityType ), undamaged );
    CheckMessage( message, unitId, componentTypeId, undamaged, destroyed );
}

BOOST_FIXTURE_TEST_CASE( equipment_updater_sends_nothing_if_message_is_empty, RegisteredFixture )
{
    const int undamaged = 2;
    const int componentNumber = 10;
    MOCK_EXPECT( componentTypes, Apply ).calls( boost::bind( &ComponentTypeVisitor_ABC::NotifyEquipment, _2, componentTypeId, componentTypeName, componentNumber ) );
    responseObserver->Notify( MakeMessage( agentTypeId, unitId ), "remote" );
    MOCK_EXPECT( resolver, Resolve ).returns( "unknown remote" );
    remoteAgentListener->EquipmentUpdated( "remote", rpr::EntityType( componentEntityType ), undamaged );
}

BOOST_FIXTURE_TEST_CASE( equipment_updater_does_not_send_message_if_no_change, RegisteredFixture )
{
    const int undamaged = 2;
    const int componentNumber = 10;
    MOCK_EXPECT( componentTypes, Apply ).calls( boost::bind( &ComponentTypeVisitor_ABC::NotifyEquipment, _2, componentTypeId, componentTypeName, componentNumber ) );
    responseObserver->Notify( MakeMessage( agentTypeId, unitId ), "remote" );
    MOCK_EXPECT( resolver, Resolve ).returns( componentTypeName );
    MOCK_EXPECT( publisher, SendClientToSim ).once();
    remoteAgentListener->EquipmentUpdated( "remote", rpr::EntityType( componentEntityType ), undamaged );
    mock::verify();
    remoteAgentListener->EquipmentUpdated( "remote", rpr::EntityType( componentEntityType ), undamaged );
}

BOOST_FIXTURE_TEST_CASE( equipment_updater_does_not_send_message_if_remote_number_is_greater_than_static_number, RegisteredFixture )
{
    const int undamaged = 4;
    const int componentNumber = 2;
    MOCK_EXPECT( componentTypes, Apply ).calls( boost::bind( &ComponentTypeVisitor_ABC::NotifyEquipment, _2, componentTypeId, componentTypeName, componentNumber ) );
    responseObserver->Notify( MakeMessage( agentTypeId, unitId ), "remote" );
    MOCK_EXPECT( resolver, Resolve ).returns( componentTypeName );
    remoteAgentListener->EquipmentUpdated( "remote", rpr::EntityType( componentEntityType ), undamaged );
}

BOOST_FIXTURE_TEST_CASE( equipment_updater_does_not_send_message_if_remote_number_is_equal_to_static_number, RegisteredFixture )
{
    const int undamaged = 4;
    const int componentNumber = 4;
    MOCK_EXPECT( componentTypes, Apply ).calls( boost::bind( &ComponentTypeVisitor_ABC::NotifyEquipment, _2, componentTypeId, componentTypeName, componentNumber ) );
    responseObserver->Notify( MakeMessage( agentTypeId, unitId ), "remote" );
    MOCK_EXPECT( resolver, Resolve ).returns( componentTypeName );
    remoteAgentListener->EquipmentUpdated( "remote", rpr::EntityType( componentEntityType ), undamaged );
}

BOOST_FIXTURE_TEST_CASE( equipment_updater_sends_only_mapped_components, RegisteredFixture )
{
    const int undamaged = 2;
    const int componentNumber = 10;
    const int destroyed = componentNumber - undamaged;
    MOCK_EXPECT( componentTypes, Apply ).calls( boost::bind( &ComponentTypeVisitor_ABC::NotifyEquipment, _2, componentTypeId, componentTypeName, componentNumber ) );
    responseObserver->Notify( MakeMessage( agentTypeId, unitId ), "remote" );
    MOCK_EXPECT( resolver, Resolve ).once().returns( "unknown remote" );
    remoteAgentListener->EquipmentUpdated( "remote", rpr::EntityType( componentEntityType ), undamaged );
    MOCK_EXPECT( resolver, Resolve ).once().returns( componentTypeName );
    MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( message ) );
    remoteAgentListener->EquipmentUpdated( "remote", rpr::EntityType( componentEntityType ), undamaged );
    CheckMessage( message, unitId, componentTypeId, undamaged, destroyed );
}

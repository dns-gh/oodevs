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
#include "MockMessageController.h"
#include "MockEntityTypeResolver.h"
#include "MockLocalAgentResolver.h"
#include "MockHlaObject.h"
#include "MockHlaClass.h"
#include "MockLogger.h"
#include "tools/MessageHandler_ABC.h"
#include "protocol/SimulationSenders.h"

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : remoteClassListener  ( 0 )
            , responseObserver     ( 0 )
            , unitAttributesHandler( 0 )
        {
            MOCK_EXPECT( factory.Create ).returns( 888 );
            MOCK_EXPECT( subject.Register ).once().with( mock::retrieve( remoteClassListener ) );
            MOCK_EXPECT( subject.Unregister );
            MOCK_EXPECT( handler.Register ).once().with( mock::retrieve( responseObserver ) );
            MOCK_EXPECT( handler.Unregister );
            MOCK_EXPECT( messageController.Register ).once().with( mock::retrieve( unitAttributesHandler ) );
            MOCK_EXPECT( messageController.Unregister );
        }

        sword::UnitCreation MakeMessage( unsigned int agentTypeId, unsigned int unitId )
        {
            sword::UnitCreation creationMessage;
            creationMessage.mutable_type()->set_id( agentTypeId );
            creationMessage.mutable_unit()->set_id( unitId );
            return creationMessage;
        }
        void CheckMessage( const sword::ClientToSim& message, unsigned int unitId, unsigned int componentTypeId,
                           int undamaged, int destroyed, int lightDamages, int heavyDamages )
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
            BOOST_CHECK_EQUAL( componentChanged.list( 3 ).quantity(), heavyDamages );
            BOOST_CHECK_EQUAL( componentChanged.list( 4 ).quantity(), lightDamages );
            BOOST_CHECK_EQUAL( componentChanged.list( 5 ).quantity(), 0 );
            BOOST_CHECK_EQUAL( componentChanged.list( 6 ).quantity(), 0 );
            BOOST_CHECK_EQUAL( componentChanged.list( 7 ).list_size(), 0 );
        }

        ClassListener_ABC* remoteClassListener;
        ResponseObserver_ABC< sword::UnitCreation >* responseObserver;
        MockRemoteAgentSubject subject;
        MockContextHandler< sword::UnitCreation > handler;
        dispatcher::MockSimulationPublisher publisher;
        MockContextFactory factory;
        rpr::MockEntityTypeResolver resolver;
        MockComponentTypes componentTypes;
        tools::MockMessageController< sword::SimToClient_Content > messageController;
        tools::MessageHandler_ABC< sword::SimToClient_Content >* unitAttributesHandler;
        sword::ClientToSim message;
        dispatcher::MockLogger logger;
        MockLocalAgentResolver localAgentResolver;
    };
    class RegisteredFixture : public Fixture
    {
    public:
        RegisteredFixture()
            : updater            ( subject, handler, publisher, factory, resolver, componentTypes, messageController, logger, localAgentResolver )
            , componentEntityType( "1 1 2 0 0 0 0" )
            , componentTypeName  ( "component type name" )
            , unitId             ( 1337 )
            , agentTypeId        ( 42 )
            , componentTypeId    ( 43 )
            , remoteAgentListener( 0 )
        {
            BOOST_REQUIRE( remoteClassListener );
            MOCK_EXPECT( object.Register ).once().with( mock::retrieve( remoteAgentListener ) );
            remoteClassListener->RemoteCreated( "id", hlaClass, object );
            BOOST_REQUIRE( remoteAgentListener );
            BOOST_REQUIRE( responseObserver );
            BOOST_REQUIRE( unitAttributesHandler );
        }
        EquipmentUpdater updater;
        const std::string componentEntityType;
        const std::string componentTypeName;
        const unsigned int unitId;
        const unsigned int agentTypeId;
        const unsigned int componentTypeId;
        ObjectListener_ABC* remoteAgentListener;
        MockHlaClass hlaClass;
        MockHlaObject object;
    };
}

BOOST_FIXTURE_TEST_CASE( equipment_updater_sends_message_when_unit_creation_is_received_after_remote_component_update, RegisteredFixture )
{
    const int undamaged = 2;
    const int componentNumber = 10;
    const int destroyed = componentNumber - undamaged;
    MOCK_EXPECT( resolver.Resolve ).once().with( rpr::EntityType( componentEntityType ), mock::assign( componentTypeName ) ).returns( true );
    remoteAgentListener->EquipmentUpdated( "remote", rpr::EntityType( componentEntityType ), undamaged, destroyed, 0, 0 );
    MOCK_EXPECT( componentTypes.Apply ).once().with( agentTypeId, mock::any ).calls( boost::bind( &ComponentTypeVisitor_ABC::NotifyEquipment, _2, componentTypeId, componentTypeName, componentNumber ) );
    MOCK_EXPECT( publisher.SendClientToSim ).once().with( mock::retrieve( message ) );
    responseObserver->Notify( MakeMessage( agentTypeId, unitId ), "remote" );
    CheckMessage( message, unitId, componentTypeId, undamaged, destroyed, 0, 0 );
}

BOOST_FIXTURE_TEST_CASE( equipment_updater_sends_message_when_remote_component_update_is_received_after_unit_creation, RegisteredFixture )
{
    const int undamaged = 2;
    const int componentNumber = 10;
    const int destroyed = componentNumber - undamaged;
    MOCK_EXPECT( componentTypes.Apply ).once().with( agentTypeId, mock::any ).calls( boost::bind( &ComponentTypeVisitor_ABC::NotifyEquipment, _2, componentTypeId, componentTypeName, componentNumber ) );
    responseObserver->Notify( MakeMessage( agentTypeId, unitId ), "remote" );
    MOCK_EXPECT( resolver.Resolve ).once().with( rpr::EntityType( componentEntityType ), mock::assign( componentTypeName ) ).returns( true );
    MOCK_EXPECT( publisher.SendClientToSim ).once().with( mock::retrieve( message ) );
    remoteAgentListener->EquipmentUpdated( "remote", rpr::EntityType( componentEntityType ), undamaged, destroyed, 0, 0 );
    CheckMessage( message, unitId, componentTypeId, undamaged, destroyed, 0, 0 );
}

BOOST_FIXTURE_TEST_CASE( equipment_updater_sends_light_heavy_damages, RegisteredFixture )
{
    const int undamaged = 2;
    const int componentNumber = 10;
    const int destroyed = 3;
    const int lightDamages = 3;
    const int heavyDamages = 2;
    MOCK_EXPECT( componentTypes.Apply ).once().with( agentTypeId, mock::any ).calls( boost::bind( &ComponentTypeVisitor_ABC::NotifyEquipment, _2, componentTypeId, componentTypeName, componentNumber ) );
    responseObserver->Notify( MakeMessage( agentTypeId, unitId ), "remote" );
    MOCK_EXPECT( resolver.Resolve ).once().with( rpr::EntityType( componentEntityType ), mock::assign( componentTypeName ) ).returns( true );
    MOCK_EXPECT( publisher.SendClientToSim ).once().with( mock::retrieve( message ) );
    remoteAgentListener->EquipmentUpdated( "remote", rpr::EntityType( componentEntityType ), undamaged, destroyed, lightDamages, heavyDamages );
    CheckMessage( message, unitId, componentTypeId, undamaged, destroyed, lightDamages, heavyDamages );
}

BOOST_FIXTURE_TEST_CASE( equipment_updater_sends_nothing_if_message_is_empty, RegisteredFixture )
{
    const int undamaged = 2;
    const int componentNumber = 10;
    const int destroyed = componentNumber - undamaged;
    MOCK_EXPECT( componentTypes.Apply ).calls( boost::bind( &ComponentTypeVisitor_ABC::NotifyEquipment, _2, componentTypeId, componentTypeName, componentNumber ) );
    responseObserver->Notify( MakeMessage( agentTypeId, unitId ), "remote" );
    MOCK_EXPECT( resolver.Resolve ).once().with( mock::any, mock::assign( "unknown remote" ) ).returns( true );
    remoteAgentListener->EquipmentUpdated( "remote", rpr::EntityType( componentEntityType ), undamaged, destroyed, 0, 0 );
}

BOOST_FIXTURE_TEST_CASE( equipment_updater_does_not_send_message_if_no_change, RegisteredFixture )
{
    const int undamaged = 2;
    const int componentNumber = 10;
    const int destroyed = componentNumber - undamaged;
    MOCK_EXPECT( componentTypes.Apply ).calls( boost::bind( &ComponentTypeVisitor_ABC::NotifyEquipment, _2, componentTypeId, componentTypeName, componentNumber ) );
    responseObserver->Notify( MakeMessage( agentTypeId, unitId ), "remote" );
    MOCK_EXPECT( resolver.Resolve ).with( mock::any, mock::assign( componentTypeName ) ).returns( true );
    MOCK_EXPECT( publisher.SendClientToSim ).once();
    remoteAgentListener->EquipmentUpdated( "remote", rpr::EntityType( componentEntityType ), undamaged, destroyed, 0, 0 );
    mock::verify();
    remoteAgentListener->EquipmentUpdated( "remote", rpr::EntityType( componentEntityType ), undamaged, destroyed, 0, 0 );
}

BOOST_FIXTURE_TEST_CASE( equipment_updater_does_not_send_message_if_remote_number_is_greater_than_static_number, RegisteredFixture )
{
    const int undamaged = 4;
    const int componentNumber = 2;
    MOCK_EXPECT( componentTypes.Apply ).calls( boost::bind( &ComponentTypeVisitor_ABC::NotifyEquipment, _2, componentTypeId, componentTypeName, componentNumber ) );
    responseObserver->Notify( MakeMessage( agentTypeId, unitId ), "remote" );
    MOCK_EXPECT( resolver.Resolve ).once().with( mock::any, mock::assign( componentTypeName ) ).returns( true );
    remoteAgentListener->EquipmentUpdated( "remote", rpr::EntityType( componentEntityType ), undamaged, 0, 0, 0 );
}

BOOST_FIXTURE_TEST_CASE( equipment_updater_sends_only_mapped_components, RegisteredFixture )
{
    const int undamaged = 2;
    const int componentNumber = 10;
    const int destroyed = componentNumber - undamaged;
    MOCK_EXPECT( componentTypes.Apply ).calls( boost::bind( &ComponentTypeVisitor_ABC::NotifyEquipment, _2, componentTypeId, componentTypeName, componentNumber ) );
    responseObserver->Notify( MakeMessage( agentTypeId, unitId ), "remote" );
    MOCK_EXPECT( resolver.Resolve ).once().with( mock::any, mock::assign( "unknown remote" ) ).returns( true );
    remoteAgentListener->EquipmentUpdated( "remote", rpr::EntityType( componentEntityType ), undamaged, destroyed, 0, 0 );
    MOCK_EXPECT( resolver.Resolve ).once().with( mock::any, mock::assign( componentTypeName ) ).returns( true );
    MOCK_EXPECT( publisher.SendClientToSim ).once().with( mock::retrieve( message ) );
    remoteAgentListener->EquipmentUpdated( "remote", rpr::EntityType( componentEntityType ), undamaged, destroyed, 0, 0 );
    CheckMessage( message, unitId, componentTypeId, undamaged, destroyed, 0, 0 );
}

namespace
{
    class SendFixture : public RegisteredFixture
    {
    public:
        SendFixture()
            : undamaged      ( 2 )
            , componentNumber( 10 )
            , destroyed      ( componentNumber - undamaged )
        {
            MOCK_EXPECT( componentTypes.Apply ).calls( boost::bind( &ComponentTypeVisitor_ABC::NotifyEquipment, _2, componentTypeId, componentTypeName, componentNumber ) );
            responseObserver->Notify( MakeMessage( agentTypeId, unitId ), "remote" );
            MOCK_EXPECT( resolver.Resolve ).once().with( mock::any, mock::assign( componentTypeName ) ).returns( true );
            MOCK_EXPECT( publisher.SendClientToSim ).once().with( mock::retrieve( message ) );
            remoteAgentListener->EquipmentUpdated( "remote", rpr::EntityType( componentEntityType ), undamaged, destroyed, 0, 0 );
            CheckMessage( message, unitId, componentTypeId, undamaged, destroyed, 0, 0 );
            mock::verify();
        }
        sword::SimToClient_Content MakeUnitAttributesMessage( int available, int destroyed, unsigned int unitId, unsigned int componentTypeId )
        {
            sword::SimToClient_Content unitAttributesMessage;
            sword::UnitAttributes* attributesMessage = unitAttributesMessage.mutable_unit_attributes();
            attributesMessage->mutable_unit()->set_id( unitId );
            sword::EquipmentDotations::EquipmentDotation* equipmentMessage = attributesMessage->mutable_equipment_dotations()->add_elem();
            equipmentMessage->mutable_type()->set_id( componentTypeId );
            equipmentMessage->set_available( available );
            equipmentMessage->set_unavailable( destroyed );
            return unitAttributesMessage;
        }
        const int undamaged;
        const int componentNumber;
        const int destroyed;
    };
}

BOOST_FIXTURE_TEST_CASE( equipment_updater_resends_components_state_if_destroyed_from_simulation, SendFixture )
{
    MOCK_EXPECT( publisher.SendClientToSim ).once().with( mock::retrieve( message ) );
    unitAttributesHandler->Notify( MakeUnitAttributesMessage( undamaged - 1, destroyed + 1, unitId, componentTypeId ), 42 );
    CheckMessage( message, unitId, componentTypeId, undamaged, destroyed, 0, 0 );
}

BOOST_FIXTURE_TEST_CASE( equipment_updater_resends_components_state_if_recompleted_from_simulation, SendFixture )
{
    MOCK_EXPECT( publisher.SendClientToSim ).once().with( mock::retrieve( message ) );
    unitAttributesHandler->Notify( MakeUnitAttributesMessage( undamaged + 1, destroyed - 1, unitId, componentTypeId ), 42 );
    CheckMessage( message, unitId, componentTypeId, undamaged, destroyed, 0, 0 );
}

BOOST_FIXTURE_TEST_CASE( equipment_updater_resends_nothing_if_no_change, SendFixture )
{
    unitAttributesHandler->Notify( MakeUnitAttributesMessage( undamaged, destroyed, unitId, componentTypeId ), 42 );
}

BOOST_FIXTURE_TEST_CASE( equipment_updater_resends_nothing_if_unit_is_not_remote, SendFixture )
{
    const unsigned int notRemoteId = 888;
    unitAttributesHandler->Notify( MakeUnitAttributesMessage( undamaged - 1, destroyed + 1, notRemoteId, componentTypeId ), 42 );
}

BOOST_FIXTURE_TEST_CASE( equipment_updater_resends_nothing_if_component_is_unknown, SendFixture )
{
    const unsigned int unknownComponentTypeId = 888;
    unitAttributesHandler->Notify( MakeUnitAttributesMessage( undamaged + 1, destroyed - 1, unitId, unknownComponentTypeId ), 42 );
}

namespace
{
    class LocalObjectFixture : public Fixture
    {
    public:
        LocalObjectFixture()
            : updater ( subject, handler, publisher, factory, resolver, componentTypes, messageController, logger, localAgentResolver )
            , componentEntityType( "1 1 2 0 0 0 0" )
            , componentTypeName  ( "component type name" )
            , agentTypeId        ( 42 )
            , remoteAgentListener( 0 )
            , unitId             ( 1337 )
            , componentTypeId    ( 43 )
        {
            BOOST_REQUIRE( remoteClassListener );
            BOOST_REQUIRE( responseObserver );
            BOOST_REQUIRE( unitAttributesHandler );
        }
        EquipmentUpdater updater;
        const std::string componentEntityType;
        const std::string componentTypeName;
        const unsigned int agentTypeId;
        ObjectListener_ABC* remoteAgentListener;
        const unsigned int unitId;
        const unsigned int componentTypeId;
        MockHlaClass hlaClass;
        MockHlaObject object;
    };
}

BOOST_FIXTURE_TEST_CASE( divested_local_object_update, LocalObjectFixture )
{
    const int componentNumber = 10;

    MOCK_EXPECT( localAgentResolver.ResolveName ).with("id_local").returns(unitId);
    MOCK_EXPECT( localAgentResolver.AgentType ).with(unitId).returns(agentTypeId);
    MOCK_EXPECT( componentTypes.Apply ).once().with( agentTypeId, mock::any ).calls( boost::bind( &ComponentTypeVisitor_ABC::NotifyEquipment, _2, componentTypeId, componentTypeName, componentNumber ) );
    remoteClassListener->LocalCreated( "id_local", hlaClass, object );

    MOCK_EXPECT( object.Register ).once().with( mock::retrieve( remoteAgentListener ) );
    remoteClassListener->Divested( "id_local" );
    BOOST_REQUIRE( remoteAgentListener );

    const int undamaged = 2;
    const int destroyed = 3;
    const int lightDamages = 3;
    const int heavyDamages = 2;

    sword::ClientToSim message;
    MOCK_EXPECT( publisher.SendClientToSim ).once().with( mock::retrieve( message ) );
    MOCK_EXPECT( resolver.Resolve ).once().with( rpr::EntityType( componentEntityType ), mock::assign( componentTypeName ) ).returns( true );
    remoteAgentListener->EquipmentUpdated( "id_local", rpr::EntityType( componentEntityType ), undamaged, destroyed, lightDamages, heavyDamages );
    CheckMessage( message, unitId, componentTypeId, undamaged, destroyed, lightDamages, heavyDamages );
}

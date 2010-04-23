// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ObjectMagicOrdersInterface.h"
#include "moc_ObjectMagicOrdersInterface.cpp"

#include "actions/ObjectMagicAction.h"
#include "actions/ParameterList.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "gaming/ActionPublisher.h"
#include "gaming/ActionTiming.h"
#include "gaming/Object.h"
#include "gaming/StaticModel.h"
#include "protocol/simulationsenders.h"

using namespace actions;
using namespace Common;
using namespace kernel;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface constructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
ObjectMagicOrdersInterface::ObjectMagicOrdersInterface( QWidget* parent, Controllers& controllers, ActionPublisher& actionPublisher, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const Simulation& simulation, const Profile_ABC& profile )
    : QObject( parent )
    , controllers_( controllers )
    , actionPublisher_( actionPublisher )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation )
    , profile_( profile )
    , selectedEntity_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface destructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
ObjectMagicOrdersInterface::~ObjectMagicOrdersInterface()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::NotifyContextMenu
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::NotifyContextMenu( const Object_ABC& entity, ContextMenu& menu )
{
    if( !profile_.CanDoMagic( entity ) )
        return;
    selectedEntity_ = &entity;
    QPopupMenu* magicMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
    AddMagic( tr( "Build" ), SLOT( BuildObject() ), magicMenu );
    AddMagic( tr( "Destroy" ), SLOT( DestroyObject() ), magicMenu );
    if( entity.GetType().CanBeValorized() )
    {
        AddMagic( tr( "Mine" ), SLOT( MineObject() ), magicMenu );
        AddMagic( tr( "Sweep mines" ), SLOT( SweepMineObject() ), magicMenu );
    }
    
    const Object& obj = static_cast< const Object& >( entity );
    if( obj.IsReservedObstacle() )
    {
        if( obj.IsReservedObstacleActivated() ) 
            AddMagic( tr( "Deactivate reserved obstacle" ), SLOT( DeactivateReservedObstacle() ), magicMenu );  
        else
            AddMagic( tr( "Activate reserved obstacle" ), SLOT( ActivateReservedObstacle() ), magicMenu );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::SendObjectMagic
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::SendObjectMagic( ParameterList& attribute )
{
    if( selectedEntity_ )
    {
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "update_object" );
        ObjectMagicAction* action = new ObjectMagicAction( selectedEntity_, actionType, controllers_.controller_, true );
        tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
        ParameterList* attributesList = new ParameterList( it.NextElement() );
        action->AddParameter( *attributesList );
        attributesList->AddParameter( attribute );
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
        action->RegisterAndPublish( actionsModel_, actionPublisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::BuildObject
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::BuildObject()
{
    ParameterList& list = *new ParameterList( OrderParameter( "Construction", "list", false ) );
    list.AddIdentifier( "AttributeId", MsgsClientToSim::MsgObjectMagicAction_Attribute_construction );
    list.AddIdentifier( "Type", 0 );
    list.AddQuantity( "Number", 0 );
    list.AddNumeric( "Density", 0 );
    list.AddQuantity( "Percentage", 100 );
    SendObjectMagic( list );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::DestroyObject
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::DestroyObject()
{
    ParameterList& list = *new ParameterList( OrderParameter( "Construction", "list", false ) );
    list.AddIdentifier( "AttributeId", MsgsClientToSim::MsgObjectMagicAction_Attribute_construction );
    list.AddIdentifier( "Type", 0 );
    list.AddQuantity( "Number", 0 );
    list.AddNumeric( "Density", 0 );
    list.AddQuantity( "Percentage", 0 );
    SendObjectMagic( list );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::MineObject
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::MineObject()
{
    ParameterList& list = *new ParameterList( OrderParameter( "Mine", "list", false ) );
    list.AddIdentifier( "AttributeId", MsgsClientToSim::MsgObjectMagicAction_Attribute_mine );
    list.AddIdentifier( "Type", 0 );
    list.AddQuantity( "Number", 0 );
    list.AddNumeric( "Density", 0 );
    list.AddQuantity( "Percentage", 100 );
    SendObjectMagic( list );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::SweepMineObject
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::SweepMineObject()
{
    ParameterList& list = *new ParameterList( OrderParameter( "Mine", "list", false ) );
    list.AddIdentifier( "AttributeId", MsgsClientToSim::MsgObjectMagicAction_Attribute_mine );
    list.AddIdentifier( "Type", 0 );
    list.AddQuantity( "Number", 0 );
    list.AddNumeric( "Density", 0 );
    list.AddQuantity( "Percentage", 0 );
    SendObjectMagic( list );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::ActivateReservedObstacle
// Created: NLD 2007-05-23
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::ActivateReservedObstacle()
{
    ParameterList& list = *new ParameterList( OrderParameter( "Obstacle", "list", false ) );
    list.AddIdentifier( "AttributeId", MsgsClientToSim::MsgObjectMagicAction_Attribute_obstacle );
    list.AddIdentifier( "TargetType", Common::ObstacleType_DemolitionTargetType_reserved );
    list.AddBool( "Activation", true );
    SendObjectMagic( list );
}
    
// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::DeactivateReservedObstacle
// Created: NLD 2007-05-23
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::DeactivateReservedObstacle()
{
    ParameterList& list = *new ParameterList( OrderParameter( "Obstacle", "list", false ) );
    list.AddIdentifier( "AttributeId", MsgsClientToSim::MsgObjectMagicAction_Attribute_obstacle );
    list.AddIdentifier( "TargetType", Common::ObstacleType_DemolitionTargetType_reserved );
    list.AddBool( "Activation", false );
    SendObjectMagic( list );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::AddMagic
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
int ObjectMagicOrdersInterface::AddMagic( const QString& label, const char* slot, QPopupMenu* menu )
{
    return menu->insertItem( label, this, slot );
}

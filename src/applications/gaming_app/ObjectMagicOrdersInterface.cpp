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

#include "clients_kernel/Controllers.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "gaming/Object.h"
#include "protocol/simulationsenders.h"

using namespace Common;

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface constructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
ObjectMagicOrdersInterface::ObjectMagicOrdersInterface( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher, const Profile_ABC& profile )
    : QObject( parent )
    , controllers_( controllers )
    , publisher_( publisher )
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
void ObjectMagicOrdersInterface::SendObjectMagic( MsgsClientToSim::MsgMagicActionUpdateObject& message )
{
    if( selectedEntity_ )
    {
        message.set_oid( selectedEntity_->GetId() );
        simulation::ObjectMagicAction magicAction;
        *magicAction().mutable_action()->mutable_update_object() = message;
        magicAction.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::BuildObject
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::BuildObject()
{
    MsgsClientToSim::MsgMagicActionUpdateObject magicAction;
    magicAction.mutable_attributes()->mutable_construction()->set_percentage( 100 );
    SendObjectMagic( magicAction );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::DestroyObject
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::DestroyObject()
{
    MsgsClientToSim::MsgMagicActionUpdateObject magicAction;
    magicAction.mutable_attributes()->mutable_construction()->set_percentage( 0 );
    SendObjectMagic( magicAction );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::MineObject
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::MineObject()
{
    MsgsClientToSim::MsgMagicActionUpdateObject magicAction;
    magicAction.mutable_attributes()->mutable_mine()->set_percentage( 100 );
    SendObjectMagic( magicAction );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::SweepMineObject
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::SweepMineObject()
{
    MsgsClientToSim::MsgMagicActionUpdateObject magicAction;
    magicAction.mutable_attributes()->mutable_mine()->set_percentage( 0 );
    SendObjectMagic( magicAction );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::ActivateReservedObstacle
// Created: NLD 2007-05-23
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::ActivateReservedObstacle()
{
    MsgsClientToSim::MsgMagicActionUpdateObject magicAction;
    magicAction.mutable_attributes()->mutable_obstacle()->set_activated( 1 );
    magicAction.mutable_attributes()->mutable_obstacle()->set_type( Common::ObstacleType_DemolitionTargetType_reserved ); 
    SendObjectMagic( magicAction );
}
    
// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::DeactivateReservedObstacle
// Created: NLD 2007-05-23
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::DeactivateReservedObstacle()
{
    MsgsClientToSim::MsgMagicActionUpdateObject magicAction;
    magicAction.mutable_attributes()->mutable_obstacle()->set_activated( 0 );
    magicAction.mutable_attributes()->mutable_obstacle()->set_type( Common::ObstacleType_DemolitionTargetType_reserved ); 
    SendObjectMagic( magicAction );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::AddMagic
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
int ObjectMagicOrdersInterface::AddMagic( const QString& label, const char* slot, QPopupMenu* menu )
{
    return menu->insertItem( label, this, slot );
}

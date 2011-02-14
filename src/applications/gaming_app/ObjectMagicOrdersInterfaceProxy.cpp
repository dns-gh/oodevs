// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ObjectMagicOrdersInterfaceProxy.h"
#include "ObjectMagicOrdersInterface.h"
#include "UrbanMagicOrdersInterface.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Object_ABC.h"
#include "gaming/StaticModel.h"

using namespace actions;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterfaceProxy constructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
ObjectMagicOrdersInterfaceProxy::ObjectMagicOrdersInterfaceProxy( QWidget* parent, Controllers& controllers, actions::ActionsModel& actionsModel, const ::StaticModel& staticModel, const kernel::Time_ABC& simulation, const Profile_ABC& profile )
    : parent_( parent )
    , controllers_( controllers )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation )
    , profile_( profile )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterfaceProxy destructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
ObjectMagicOrdersInterfaceProxy::~ObjectMagicOrdersInterfaceProxy()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterfaceProxy::NotifyContextMenu
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterfaceProxy::NotifyContextMenu( const Object_ABC& entity, ContextMenu& menu )
{
    if( !profile_.CanDoMagic( entity ) )
        return;

    if( entity.GetType().IsUrban() )
    {
        magicOrdersStrategy_.reset( new UrbanMagicOrdersInterface( parent_, controllers_, actionsModel_, static_, simulation_ ) );
        magicOrdersStrategy_->NotifyContextMenu( entity, menu );
    }
    else
    {
        magicOrdersStrategy_.reset( new ObjectMagicOrdersInterface( parent_, controllers_, actionsModel_, static_, simulation_ ) );
        magicOrdersStrategy_->NotifyContextMenu( entity, menu );
    }
}

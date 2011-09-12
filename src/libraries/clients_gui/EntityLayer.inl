// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ColorStrategy_ABC.h"
#include "LayerFilter_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/GlTools_ABC.h"
#include <graphics/MapWidget.h>

namespace gui
{

// -----------------------------------------------------------------------------
// Name: EntityLayer constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
EntityLayer< ConcreteEntity >::EntityLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const kernel::Profile_ABC& profile, const LayerFilter_ABC& filter )
    : EntityLayerBase( controllers, tools, strategy, view, profile, filter )
    , controllers_( controllers )
    , strategy_( strategy )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EntityLayer destructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
EntityLayer< ConcreteEntity >::~EntityLayer()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::NotifyCreated
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityLayer< ConcreteEntity >::NotifyCreated( const ConcreteEntity& entity )
{
    if( filter_.IsAllowed( entity.GetTypeName() ) )
        AddEntity( entity );
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::NotifyDeleted
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityLayer< ConcreteEntity >::NotifyDeleted( const ConcreteEntity& entity )
{
    RemoveEntity( entity );
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::NotifyActivated
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityLayer< ConcreteEntity >::NotifyActivated( const ConcreteEntity& entity )
{
    if( filter_.IsAllowed( entity.GetTypeName() ) )
        ActivateEntity( entity );
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::NotifySelected
// Created: AGE 2006-06-19
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityLayer< ConcreteEntity >::NotifySelected( const ConcreteEntity* entity )
{
    if( entity )
        SelectEntity( *entity );
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::SelectColor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityLayer< ConcreteEntity >::SelectColor( const kernel::Entity_ABC& entity )
{
    strategy_.SelectColor( static_cast< const ConcreteEntity& >( entity ) );
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::ContextMenu
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityLayer< ConcreteEntity >::ContextMenu( const kernel::Entity_ABC& entity, const geometry::Point2f& geoPoint, const QPoint& point )
{
    controllers_.actions_.ContextMenu( static_cast< const ConcreteEntity& >( entity ), geoPoint, point );
}

}
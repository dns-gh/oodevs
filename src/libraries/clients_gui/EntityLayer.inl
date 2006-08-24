// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel/Controllers.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Positions.h"
#include "ColorStrategy_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "graphics/MapWidget.h"

namespace gui
{

// -----------------------------------------------------------------------------
// Name: EntityLayer constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
EntityLayer< ConcreteEntity >::EntityLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view )
    : EntityLayerBase( controllers, tools, view )
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
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::NotifyCreated
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityLayer< ConcreteEntity >::NotifyCreated( const ConcreteEntity& entity )
{
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
// Name: EntityLayer::IsInTeam
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
bool EntityLayer< ConcreteEntity >::IsInTeam( const kernel::Entity_ABC& entity, const kernel::Team_ABC& team )
{
    return static_cast< const ConcreteEntity& >( entity ).IsInTeam( team );
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

// -----------------------------------------------------------------------------
// Name: EntityLayer::DisplayTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
bool EntityLayer< ConcreteEntity >::DisplayTooltip( const kernel::Entity_ABC& entity, kernel::Displayer_ABC& displayer )
{
    DisplayTooltip( static_cast< const ConcreteEntity& >( entity ), displayer );
    return EntityLayerBase::DisplayTooltip( entity, displayer );
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::DisplayTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityLayer< ConcreteEntity >::DisplayTooltip( const ConcreteEntity&, kernel::Displayer_ABC& )
{
    // NOTHING
}

}
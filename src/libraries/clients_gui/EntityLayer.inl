// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ColorStrategy_ABC.h"
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
EntityLayer< ConcreteEntity >::EntityLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const kernel::Profile_ABC& profile, const QString& name )
    : EntityLayerBase( controllers, tools, strategy, view, profile, name )
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
// Name: EntityLayer::NotifySelectionChanged
// Created: JSR 2012-05-22
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityLayer< ConcreteEntity >::NotifySelectionChanged( const std::vector< const ConcreteEntity* >& elements )
{
    if( elements.size() >= 1 )
        SelectEntity( *elements.front() );
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::HandleRectangleSelection
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityLayer< ConcreteEntity >::HandleRectangleSelection( const geometry::Point2f& topLeft, const geometry::Point2f& bottomRight )
{
    EntityLayerBase::SelectInRectangle( topLeft, bottomRight );
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
void EntityLayer< ConcreteEntity >::ContextMenu( const kernel::GraphicalEntity_ABC& selectable, const geometry::Point2f& geoPoint, const QPoint& point )
{
    controllers_.actions_.ContextMenu( static_cast< const ConcreteEntity& >( selectable ), geoPoint, point );
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::IsIn
// Created: JSR 2012-05-24
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
bool EntityLayer< ConcreteEntity >::IsIn( const kernel::GraphicalEntity_ABC& selectable ) const
{
    return dynamic_cast< const ConcreteEntity* >( &selectable ) != 0;
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::ShouldDisplayTooltip
// Created: NPT 2013-03-14
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
bool EntityLayer< ConcreteEntity >::ShouldDisplayTooltip( const kernel::Entity_ABC& entity, const geometry::Point2f& point )
{
    if( dynamic_cast< const ConcreteEntity* >( &entity ) == 0 )
        return false;
    return EntityLayerBase::ShouldDisplayTooltip( entity, point );
}

}

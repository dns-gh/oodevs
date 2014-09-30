// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ColorStrategy_ABC.h"
#include "Drawable_ABC.h"
#include "GlTools_ABC.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Positions.h"

namespace gui
{

// -----------------------------------------------------------------------------
// Name: EntityLayer constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
EntityLayer< ConcreteEntity >::EntityLayer( kernel::Controllers& controllers,
                                            GlTools_ABC& tools,
                                            ColorStrategy_ABC& strategy,
                                            View_ABC& view,
                                            const kernel::Profile_ABC& profile,
                                            E_LayerTypes type )
    : EntityLayerBase( controllers, tools, strategy, view, profile, type )
    , strategy_( strategy )
{
    controllers_.Update( *this );
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
    controllers_.actions_.ContextMenu( this, static_cast< const ConcreteEntity& >( selectable ), geoPoint, point );
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::FillContextMenu
// Created: LGY 2013-04-05
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityLayer< ConcreteEntity >::FillContextMenu( const kernel::GraphicalEntity_ABC& selectable, kernel::ContextMenu& menu )
{
    controllers_.actions_.ContextMenu( this, static_cast< const ConcreteEntity& >( selectable ), menu );
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

}

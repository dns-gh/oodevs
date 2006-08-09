// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_kernel/Controllers.h"
#include "astec_kernel/ActionController.h"
#include "astec_kernel/Drawable_ABC.h"
#include "astec_kernel/Positions.h"
#include "ColorStrategy_ABC.h"
#include "astec_kernel/GlTools_ABC.h"
#include "graphics/MapWidget.h"

// -----------------------------------------------------------------------------
// Name: EntityLayer constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
EntityLayer< ConcreteEntity >::EntityLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view )
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
void EntityLayer< ConcreteEntity >::SelectColor( const Entity_ABC& entity )
{
    strategy_.SelectColor( static_cast< const ConcreteEntity& >( entity ) );
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::IsInTeam
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
bool EntityLayer< ConcreteEntity >::IsInTeam( const Entity_ABC& entity, const Team_ABC& team )
{
    return static_cast< const ConcreteEntity& >( entity ).IsInTeam( team );
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::DisplayTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
bool EntityLayer< ConcreteEntity >::DisplayTooltip( const Entity_ABC& entity, Displayer_ABC& displayer )
{
    DisplayTooltip( static_cast< const ConcreteEntity& >( entity ), displayer );
    return EntityLayerBase::DisplayTooltip( entity, displayer );
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::DisplayTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityLayer< ConcreteEntity >::DisplayTooltip( const ConcreteEntity& , Displayer_ABC& )
{
    // NOTHING
}

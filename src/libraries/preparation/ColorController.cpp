// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ColorController.h"
#include "LogisticHierarchiesBase.h"
#include "LogisticBaseStates.h"
#include "clients_kernel/Color_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: ColorController constructor
// Created: LGY 2011-06-23
// -----------------------------------------------------------------------------
ColorController::ColorController( kernel::Controllers& controllers )
    : controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ColorController destructor
// Created: LGY 2011-06-23
// -----------------------------------------------------------------------------
ColorController::~ColorController()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ColorController::Apply
// Created: LGY 2011-06-23
// -----------------------------------------------------------------------------
QColor ColorController::Apply( const kernel::Entity_ABC& entity, const QColor& base )
{
    CIT_Colors it = colors_.find( entity.GetId() );
    if( it != colors_.end() )
        return it->second;
    return base;
}

// -----------------------------------------------------------------------------
// Name: ColorController::Add
// Created: LGY 2011-06-23
// -----------------------------------------------------------------------------
void ColorController::Add( const kernel::Entity_ABC& entity, const QColor& newColor )
{
    CIT_Colors it = colors_.find( entity.GetId() );
    boost::optional< QColor > oldColor = boost::none;
    if( it != colors_.end() )
        oldColor = it->second;
    AddSubordinate( entity, newColor, oldColor );
    UpdateHierarchies( entity );
}

// -----------------------------------------------------------------------------
// Name: ColorController::AddSubordinate
// Created: LGY 2011-06-24
// -----------------------------------------------------------------------------
void ColorController::AddSubordinate( const kernel::Entity_ABC& entity, const QColor& newColor, const boost::optional< QColor >& oldColor )
{
    colors_[ entity.GetId() ] = newColor;
    if( const kernel::Color_ABC* pColor = entity.Retrieve< kernel::Color_ABC >() )
        const_cast< kernel::Color_ABC* >( pColor )->ChangeColor( newColor );
    tools::Iterator< const kernel::Entity_ABC& > it = entity.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Entity_ABC& child = it.NextElement();
        CIT_Colors it = colors_.find( child.GetId() );
        if( it == colors_.end() || ( it != colors_.end() && oldColor && it->second == *oldColor ) )
            AddSubordinate( child, newColor, oldColor );
    }
}

// -----------------------------------------------------------------------------
// Name: ColorController::Remove
// Created: LGY 2011-06-23
// -----------------------------------------------------------------------------
void ColorController::Remove( const kernel::Entity_ABC& entity )
{
    CIT_Colors it = colors_.find( entity.GetId() );
    if( it != colors_.end() )
    {
        QColor color = it->second;
        RemoveSubordinate( entity, color );
        if( const kernel::TacticalHierarchies* pHierarchies = entity.Retrieve< kernel::TacticalHierarchies >() )
            if( const kernel::Entity_ABC* pSuperior = pHierarchies->GetSuperior() )
                if( const kernel::Color_ABC* pColor = pSuperior->Retrieve< kernel::Color_ABC >() )
                    if( pColor->IsOverride() )
                        Add( entity, static_cast< QColor >( *pColor ) );
        UpdateHierarchies( entity );
    }
}

// -----------------------------------------------------------------------------
// Name: ColorController::RemoveSubordinate
// Created: LGY 2011-06-24
// -----------------------------------------------------------------------------
void ColorController::RemoveSubordinate( const kernel::Entity_ABC& entity, const QColor& color )
{
    colors_.erase( entity.GetId() );
    if( const kernel::Color_ABC* pColor = entity.Retrieve< kernel::Color_ABC >() )
        const_cast< kernel::Color_ABC* >( pColor )->Clear();
    tools::Iterator< const kernel::Entity_ABC& > it = entity.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Entity_ABC& child = it.NextElement();
        CIT_Colors it = colors_.find( child.GetId() );
        if( it != colors_.end() && it->second == color )
            RemoveSubordinate( child, color );
    }
}

// -----------------------------------------------------------------------------
// Name: ColorController::UpdateHierarchies
// Created: LGY 2011-06-24
// -----------------------------------------------------------------------------
void ColorController::UpdateHierarchies( const kernel::Entity_ABC& entity )
{
    if( const kernel::TacticalHierarchies* pTactical = entity.Retrieve< kernel::TacticalHierarchies >() )
    {
        controllers_.controller_.Update( *pTactical );
        if( const kernel::CommunicationHierarchies* pCommunication = entity.Retrieve< kernel::CommunicationHierarchies >() )
            controllers_.controller_.Update( *pCommunication );
        else if( const kernel::CommunicationHierarchies* pCommunication = pTactical->GetTop().Retrieve< kernel::CommunicationHierarchies >() )
            controllers_.controller_.Update( *pCommunication );
        if( const LogisticBaseStates* pLogistic = static_cast< const LogisticBaseStates* >( entity.Retrieve< LogisticHierarchiesBase >() ) )
            controllers_.controller_.Update( *pLogistic );
        UpdateLogisticBaseStates( *pTactical );
    }
}

// -----------------------------------------------------------------------------
// Name: ColorController::UpdateLogisticBaseStates
// Created: LGY 2011-11-29
// -----------------------------------------------------------------------------
void ColorController::UpdateLogisticBaseStates( const kernel::TacticalHierarchies& tactical )
{
    tools::Iterator< const kernel::Entity_ABC& > children = tactical.CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const kernel::Entity_ABC& child = children.NextElement();
        if( const kernel::TacticalHierarchies* pTactical = child.Retrieve< kernel::TacticalHierarchies >() )
        {
            if( const LogisticBaseStates* pLogistic = static_cast< const LogisticBaseStates* >( child.Retrieve< LogisticHierarchiesBase >() ) )
                controllers_.controller_.Update( *pLogistic );
            UpdateLogisticBaseStates( *pTactical );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ColorController::Reset
// Created: MMC 2012-01-10
// -----------------------------------------------------------------------------
void ColorController::Reset( const kernel::Entity_ABC& entity, const QColor& newColor )
{
    ResetSubordinate( entity, newColor );
    UpdateHierarchies( entity );
}

// -----------------------------------------------------------------------------
// Name: ColorController::ResetSubordinate
// Created: MMC 2012-01-10
// -----------------------------------------------------------------------------
void ColorController::ResetSubordinate( const kernel::Entity_ABC& entity, const QColor& newColor )
{
    colors_[ entity.GetId() ] = newColor;
    if( const kernel::Color_ABC* pColor = entity.Retrieve< kernel::Color_ABC >() )
        const_cast< kernel::Color_ABC* >( pColor )->ChangeColor( newColor );
    tools::Iterator< const kernel::Entity_ABC& > it = entity.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Entity_ABC& child = it.NextElement();
        ResetSubordinate( child, newColor );
    }
}

// -----------------------------------------------------------------------------
// Name: ColorController::NotifyCreated
// Created: LGY 2011-06-24
// -----------------------------------------------------------------------------
void ColorController::NotifyCreated( const kernel::Entity_ABC& entity )
{
    if( const kernel::Color_ABC* color = entity.Retrieve< kernel::Color_ABC >() )
        if( color->IsOverride() )
        {
            colors_[ entity.GetId() ] = static_cast< QColor >( *color );
            UpdateHierarchies( entity );
        }
}

// -----------------------------------------------------------------------------
// Name: ColorController::NotifyDeleted
// Created: LGY 2011-06-24
// -----------------------------------------------------------------------------
void ColorController::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    colors_.erase( entity.GetId() );
}

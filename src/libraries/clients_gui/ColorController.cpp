// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ColorController.h"
#include "LogisticHierarchiesBase.h"
#include "clients_kernel/Color_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/LogisticHierarchies.h"
#include "clients_kernel/Color_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ColorController constructor
// Created: LDC 2012-05-04
// -----------------------------------------------------------------------------
ColorController::ColorController( kernel::Controllers& controllers )
    : controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ColorController destructor
// Created: LDC 2012-05-04
// -----------------------------------------------------------------------------
ColorController::~ColorController()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ColorController::Apply
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
QColor ColorController::Apply( const kernel::Entity_ABC& entity, const QColor& base ) const
{
    auto it = colors_.find( entity.GetId() );
    if( it != colors_.end() )
        return it->second;
    return base;
}

// -----------------------------------------------------------------------------
// Name: ColorController::Apply
// Created: LGY 2013-03-06
// -----------------------------------------------------------------------------
float ColorController::Apply( const kernel::Entity_ABC& /*entity*/, float alpha ) const
{
    return alpha;
}

// -----------------------------------------------------------------------------
// Name: ColorController::ChangeColor
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
void ColorController::ChangeColor( const kernel::Entity_ABC& entity )
{
    if( const kernel::Color_ABC* color = entity.Retrieve< kernel::Color_ABC >() )
        if( color->IsOverriden() )
        {
            colors_[ entity.GetId() ] = static_cast< QColor >( *color );
            UpdateHierarchies( entity );
        }
}

// -----------------------------------------------------------------------------
// Name: ColorController::NotifyCreated
// Created: LGY 2011-06-24
// -----------------------------------------------------------------------------
void ColorController::NotifyCreated( const kernel::Entity_ABC& entity )
{
    ChangeColor( entity );
}

// -----------------------------------------------------------------------------
// Name: ColorController::NotifyDeleted
// Created: LGY 2011-06-24
// -----------------------------------------------------------------------------
void ColorController::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    colors_.erase( entity.GetId() );
}

// -----------------------------------------------------------------------------
// Name: ColorController::Add
// Created: LGY 2011-06-23
// -----------------------------------------------------------------------------
void ColorController::Add( const kernel::Entity_ABC& entity, const QColor& newColor, bool applyToSubordinates /*=true*/, bool force /*= false*/ )
{
    auto it = colors_.find( entity.GetId() );
    boost::optional< QColor > oldColor;
    if( it != colors_.end() )
        oldColor = it->second;
    AddSubordinate( entity, newColor, oldColor, applyToSubordinates, force );
    UpdateHierarchies( entity );
}

// -----------------------------------------------------------------------------
// Name: ColorController::AddSubordinate
// Created: LGY 2011-06-24
// -----------------------------------------------------------------------------
void ColorController::AddSubordinate( const kernel::Entity_ABC& entity, const QColor& newColor, const boost::optional< QColor >& oldColor, bool applyToSubordinates, bool force )
{
    AddColor( entity, newColor );
    if( !applyToSubordinates )
        return;
    if( const kernel::TacticalHierarchies* pTacticalHierarchies =  entity.Retrieve< kernel::TacticalHierarchies >() )
    {
        auto it = pTacticalHierarchies->CreateSubordinateIterator();
        while( it.HasMoreElements() )
        {
            const kernel::Entity_ABC& child = it.NextElement();
            auto it = colors_.find( child.GetId() );
            if( force || it == colors_.end() || ( it != colors_.end() && oldColor && it->second == *oldColor ) )
                AddSubordinate( child, newColor, oldColor, applyToSubordinates, force );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ColorController::Remove
// Created: LGY 2011-06-23
// -----------------------------------------------------------------------------
void ColorController::Remove( const kernel::Entity_ABC& entity, bool applyToSubordinates /*= true*/, bool force /*= false*/ )
{
    auto it = colors_.find( entity.GetId() );
    if( it != colors_.end() )
    {
        QColor color = it->second;
        RemoveSubordinate( entity, color, applyToSubordinates, force );
        if( const kernel::TacticalHierarchies* pHierarchies = entity.Retrieve< kernel::TacticalHierarchies >() )
            if( const kernel::Entity_ABC* pSuperior = pHierarchies->GetSuperior() )
                if( const kernel::Color_ABC* pColor = pSuperior->Retrieve< kernel::Color_ABC >() )
                    if( pColor->IsOverriden() )
                        Add( entity, static_cast< QColor >( *pColor ) );
        UpdateHierarchies( entity );
    }
}

// -----------------------------------------------------------------------------
// Name: ColorController::RemoveSubordinate
// Created: LGY 2011-06-24
// -----------------------------------------------------------------------------
void ColorController::RemoveSubordinate( const kernel::Entity_ABC& entity, const QColor& color, bool applyToSubordinates, bool force )
{
    ClearColor( entity );
    if( !applyToSubordinates )
        return;
    if( const kernel::TacticalHierarchies* pHierarchies = entity.Retrieve< kernel::TacticalHierarchies >() )
    {
        auto it = pHierarchies->CreateSubordinateIterator();
        while( it.HasMoreElements() )
        {
            const kernel::Entity_ABC& child = it.NextElement();
            auto itColor = colors_.find( child.GetId() );
            if( force || itColor != colors_.end() && itColor->second == color )
                RemoveSubordinate( child, color, applyToSubordinates, force );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ColorController::UpdateHierarchies
// Created: LGY 2011-06-24
// -----------------------------------------------------------------------------
void ColorController::UpdateHierarchies( const kernel::Entity_ABC& entity )
{
    controllers_.controller_.Update( entity );
    if( entity.GetTypeName() != kernel::Object_ABC::typeName_ )
        if( const kernel::TacticalHierarchies* pTactical = entity.Retrieve< kernel::TacticalHierarchies >() )
        {
            controllers_.controller_.Update( *pTactical );
            if( const kernel::CommunicationHierarchies* pCommunication = entity.Retrieve< kernel::CommunicationHierarchies >() )
                controllers_.controller_.Update( *pCommunication );
            else if( const kernel::CommunicationHierarchies* pCommunication = pTactical->GetTop().Retrieve< kernel::CommunicationHierarchies >() )
                controllers_.controller_.Update( *pCommunication );
            if( const kernel::LogisticHierarchies* pLogistic = static_cast< const kernel::LogisticHierarchies* >( entity.Retrieve< gui::LogisticHierarchiesBase >() ) )
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
    auto children = tactical.CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const kernel::Entity_ABC& child = children.NextElement();
        if( const kernel::TacticalHierarchies* pTactical = child.Retrieve< kernel::TacticalHierarchies >() )
        {
            if( const kernel::LogisticHierarchies* pLogistic = static_cast< const kernel::LogisticHierarchies* >( child.Retrieve< gui::LogisticHierarchiesBase >() ) )
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
    AddColor( entity, newColor );
    auto it = entity.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
    while( it.HasMoreElements() )
        ResetSubordinate( it.NextElement(), newColor );
}

// -----------------------------------------------------------------------------
// Name: ColorController::AddColor
// Created: LGY 2013-03-27
// -----------------------------------------------------------------------------
void ColorController::AddColor( const kernel::Entity_ABC& entity, const QColor& color )
{
    colors_[ entity.GetId() ] = color;
    if( const kernel::Color_ABC* pColor = entity.Retrieve< kernel::Color_ABC >() )
        const_cast< kernel::Color_ABC* >( pColor )->ChangeColor( color );
}

// -----------------------------------------------------------------------------
// Name: ColorController::ClearColor
// Created: LGY 2013-03-27
// -----------------------------------------------------------------------------
void ColorController::ClearColor( const kernel::Entity_ABC& entity )
{
    colors_.erase( entity.GetId() );
    if( const kernel::Color_ABC* pColor = entity.Retrieve< kernel::Color_ABC >() )
        const_cast< kernel::Color_ABC* >( pColor )->Clear();
}

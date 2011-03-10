// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::AutomatsLayer */

#include "clients_gui_pch.h"
#include "AutomatsLayer.h"
#include "moc_AutomatsLayer.cpp"
#include "clients_kernel/Aggregatable_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Formation_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AutomatsLayer constructor
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
AutomatsLayer::AutomatsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, 
                              const Profile_ABC& profile, const gui::LayerFilter_ABC& filter )
    : EntityLayer< Automat_ABC >( controllers, tools, strategy, view, profile, filter )
    , selected_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer destructor
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
AutomatsLayer::~AutomatsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::Aggregate
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void AutomatsLayer::Aggregate( const Entity_ABC& automat )
{
    Toggle( automat, true );
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::Disaggregate
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void AutomatsLayer::Disaggregate( const Entity_ABC& automat )
{
     Toggle( automat, false );
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::Aggregate
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void AutomatsLayer::Aggregate()
{
    if( selected_ )
        Aggregate( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::Disaggregate
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void AutomatsLayer::Disaggregate()
{
    if( selected_ )
        Disaggregate( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::NotifyContextMenu
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void AutomatsLayer::NotifyContextMenu( const Automat_ABC& automat, kernel::ContextMenu& menu )
{
    selected_ = &automat;
    if( !IsAggregated( automat ) )
    {
        if( !HasAggregatedSubordinate( automat ) )
            menu.InsertItem( "Interface", tr( "Aggregate" ), this, SLOT( Aggregate() ) );
        else
            menu.InsertItem( "Interface", tr( "Disaggregate" ), this, SLOT( Disaggregate() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::Select
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void AutomatsLayer::Select( const Entity_ABC& entity, bool )
{
    entity.Select( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::Toggle
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void AutomatsLayer::Toggle( const Entity_ABC& entity, bool aggregate )
{
    tools::Iterator< const kernel::Entity_ABC& > it = entity.Get< TacticalHierarchies >().CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Entity_ABC& child = it.NextElement();
        if( IsAggregated( child ) != aggregate )
            child.Interface().Apply( &Aggregatable_ABC::Aggregate, aggregate );
        Toggle( child, aggregate );
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::ContextMenu
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void AutomatsLayer::ContextMenu( const Entity_ABC& entity, const geometry::Point2f& point, const QPoint& where )
{
    const Automat_ABC* automat = static_cast< const Automat_ABC* >( &entity );
    if( !IsAggregated( entity ) && HasAggregatedSubordinate( entity ) )
        controllers_.actions_.ContextMenu( *automat, entity, point, where );
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::ShouldDisplay
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
bool AutomatsLayer::ShouldDisplay( const kernel::Entity_ABC& entity )
{
    return EntityLayer< Automat_ABC >::ShouldDisplay( entity ) && !IsAggregated( entity ) && HasAggregatedSubordinate( entity );
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::NotifySelected
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
void AutomatsLayer::NotifySelected( const kernel::Automat_ABC* automat )
{
    EntityLayer< Automat_ABC >::NotifySelected( automat );
    selected_ = automat;
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::IsAggregated
// Created: LGY 2011-03-07
// -----------------------------------------------------------------------------
bool AutomatsLayer::IsAggregated( const kernel::Entity_ABC& entity ) const
{
    if( const kernel::Positions* positions = entity.Retrieve< kernel::Positions >() )
        return positions->IsAggregated();
    return false;
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::HasAggregatedSubordinate
// Created: LGY 2011-03-07
// -----------------------------------------------------------------------------
bool AutomatsLayer::HasAggregatedSubordinate( const kernel::Entity_ABC& entity ) const
{
    bool children = false;
    tools::Iterator< const kernel::Entity_ABC& > it = entity.Get< TacticalHierarchies >().CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Entity_ABC& child = it.NextElement();
        if( !IsAggregated( child ) )
            return false;
        children = true;
    }
    return children;
}

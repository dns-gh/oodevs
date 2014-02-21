// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "AutomatsLayer.h"
#include "moc_AutomatsLayer.cpp"
#include "AggregatedTools.h"
#include "clients_kernel/Aggregatable_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AutomatsLayer constructor
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
AutomatsLayer::AutomatsLayer( Controllers& controllers, GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                              View_ABC& view, const Profile_ABC& profile )
    : EntityLayer< Automat_ABC >( controllers, tools, strategy, view, profile, eLayerTypes_Automats )
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
    if( !profile_.IsVisible( automat ) )
        return;
    selected_ = &automat;
    if( !IsAggregated( automat ) )
    {
        if( !HasSubordinate( automat, &IsAggregated ) )
            menu.InsertItem( "Interface", tr( "Aggregate" ), this, SLOT( Aggregate() ) );
        else
            menu.InsertItem( "Interface", tr( "Disaggregate" ), this, SLOT( Disaggregate() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::NotifyActivated
// Created: LGY 2011-07-18
// -----------------------------------------------------------------------------
void AutomatsLayer::NotifyActivated( const kernel::Automat_ABC& automat )
{
    if( HasSubordinate( automat, boost::function< bool( const kernel::Entity_ABC& ) >( boost::lambda::constant( true ) ) ) )
        EntityLayer< kernel::Automat_ABC >::NotifyActivated( automat );
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
            child.GetInterfaces().Apply( &Aggregatable_ABC::Aggregate, aggregate );
        Toggle( child, aggregate );
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::ContextMenu
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void AutomatsLayer::ContextMenu( const GraphicalEntity_ABC& selectable, const geometry::Point2f& point, const QPoint& where )
{
    const Entity_ABC& entity = static_cast< const Entity_ABC& >( selectable );
    const Automat_ABC& automat = static_cast< const Automat_ABC& >( selectable );
    if( !IsAggregated( entity ) && HasSubordinate( entity, &IsAggregated ) )
        controllers_.actions_.ContextMenu( automat, entity, point, where );
}


// -----------------------------------------------------------------------------
// Name: AutomatsLayer::NotifySelectionChanged
// Created: JSR 2012-05-31
// -----------------------------------------------------------------------------
void AutomatsLayer::NotifySelectionChanged( const std::vector< const kernel::Automat_ABC* >& elements )
{
    EntityLayer< Automat_ABC >::NotifySelectionChanged( elements );
    selected_ = elements.size() == 1 ? elements.front() : 0;
}


// -----------------------------------------------------------------------------
// Name: AutomatsLayer::HasSubordinate
// Created: LGY 2011-03-07
// -----------------------------------------------------------------------------
bool AutomatsLayer::HasSubordinate( const kernel::Entity_ABC& entity, boost::function< bool( const kernel::Entity_ABC& ) > fun ) const
{
    bool children = false;
    tools::Iterator< const kernel::Entity_ABC& > it = entity.Get< TacticalHierarchies >().CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Entity_ABC& child = it.NextElement();
        if( !fun( child ) )
            return false;
        children = true;
    }
    return children;
}

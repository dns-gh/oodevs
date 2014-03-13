// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "FormationLayer.h"
#include "moc_FormationLayer.cpp"
#include "AggregatedTools.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include <boost/bind.hpp>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: FormationLayer constructor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
FormationLayer::FormationLayer( kernel::Controllers& controllers, GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                                View_ABC& view, const kernel::Profile_ABC& profile )
    : EntityLayer< kernel::Formation_ABC >( controllers, tools, strategy, view, profile, eLayerTypes_Formations )
    , strategy_( strategy )
    , selected_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationLayer destructor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
FormationLayer::~FormationLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::Aggregate
// Created: LGY 2011-03-03
// -----------------------------------------------------------------------------
void FormationLayer::Aggregate( const kernel::Entity_ABC& formation )
{
    Toggle( formation, true );
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::Disaggregate
// Created: LGY 2011-03-03
// -----------------------------------------------------------------------------
void FormationLayer::Disaggregate( const kernel::Entity_ABC& formation )
{
    Toggle( formation, false );
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::Aggregate
// Created: LGY 2011-03-03
// -----------------------------------------------------------------------------
void FormationLayer::Aggregate()
{
    if( selected_ )
        Aggregate( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::Disaggregate
// Created: LGY 2011-03-03
// -----------------------------------------------------------------------------
void FormationLayer::Disaggregate()
{
    if( selected_ )
        Disaggregate( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::NotifyContextMenu
// Created: LGY 2011-03-03
// -----------------------------------------------------------------------------
void FormationLayer::NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu )
{
    if( !profile_.IsVisible( formation ) )
        return;
    selected_ = &formation;
    if( !formation.IsAggregated() )
    {
        if( !HasAggregatedSubordinate( formation )  )
            menu.InsertItem( "Interface", tr( "Aggregate" ), this, SLOT( Aggregate() ) );
        else
            menu.InsertItem( "Interface", tr( "Disaggregate" ), this, SLOT( Disaggregate() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::NotifyActivated
// Created: LGY 2011-07-18
// -----------------------------------------------------------------------------
void FormationLayer::NotifyActivated( const kernel::Formation_ABC& formation )
{
    if( HasAggregatedSubordinate( formation, boost::bind( &FormationLayer::IsAgent, this, _1 ) ) )
        EntityLayer< kernel::Formation_ABC >::NotifyActivated( formation );
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::ContextMenu
// Created: LGY 2011-03-07
// -----------------------------------------------------------------------------
void FormationLayer::ContextMenu( const GraphicalEntity_ABC& selectable, const geometry::Point2f& point, const QPoint& where )
{
    const Entity_ABC& entity = static_cast< const Entity_ABC& >( selectable );
    const Formation_ABC& formation = static_cast< const Formation_ABC& >( entity );
    if( !formation.IsAggregated() && HasAggregatedSubordinate( formation ) )
        controllers_.actions_.ContextMenu( formation, entity, point, where );
}

// ----------------------------------------------------------------------------
// Name: FormationLayer::Toggle
// Created: LGY 2011-03-03
// -----------------------------------------------------------------------------
void FormationLayer::Toggle( const kernel::Entity_ABC& entity, bool aggregate )
{
    tools::Iterator< const kernel::Entity_ABC& > it = entity.Get< TacticalHierarchies >().CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        auto& child = const_cast< kernel::Entity_ABC& >( it.NextElement() );
        child.Aggregate( aggregate );
        Toggle( child, aggregate );
    }
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::NotifySelectionChanged
// Created: JSR 2012-05-31
// -----------------------------------------------------------------------------
void FormationLayer::NotifySelectionChanged( const std::vector< const kernel::Formation_ABC* >& elements )
{
    EntityLayer< kernel::Formation_ABC >::NotifySelectionChanged( elements );
    selected_ = elements.size() == 1 ? elements.front() : 0;
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::IsAgent
// Created: LGY 2011-07-18
// -----------------------------------------------------------------------------
bool FormationLayer::IsAgent( const kernel::Entity_ABC& entity ) const
{
    if( entity.GetTypeName() == kernel::Agent_ABC::typeName_ )
        return true;
    return HasAggregatedSubordinate( entity, boost::bind( &FormationLayer::IsAgent, this, _1 ) );
}

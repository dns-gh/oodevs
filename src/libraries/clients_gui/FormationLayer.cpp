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
#include "clients_kernel/Positions.h"
#include "clients_kernel/Aggregatable_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Agent_ABC.h"
#include <boost/bind.hpp>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: FormationLayer constructor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
FormationLayer::FormationLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view,
                                const kernel::Profile_ABC& profile, const gui::LayerFilter_ABC& filter )
    : EntityLayer< kernel::Formation_ABC >( controllers, tools, strategy, view, profile, filter )
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
    selected_ = &formation;
    if( !IsAggregated( formation ) )
    {
        if( !HasSubordinate( formation, boost::bind( &FormationLayer::IsAggregated, this, _1 ) ) )
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
    if( HasSubordinate( formation, boost::bind( &FormationLayer::IsAgent, this, _1 ) ) )
        EntityLayer< kernel::Formation_ABC >::NotifyActivated( formation );
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::ContextMenu
// Created: LGY 2011-03-07
// -----------------------------------------------------------------------------
void FormationLayer::ContextMenu( const kernel::Entity_ABC& entity, const geometry::Point2f& point, const QPoint& where )
{
    const Formation_ABC* formation = static_cast< const Formation_ABC* >( &entity );
    if( !IsAggregated( *formation ) && HasSubordinate( *formation, boost::bind( &FormationLayer::IsAggregated, this, _1 ) ) )
        controllers_.actions_.ContextMenu( *formation, entity, point, where );
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
        const kernel::Entity_ABC& child = it.NextElement();
        if( IsAggregated( child ) != aggregate )
            child.Interface().Apply( &Aggregatable_ABC::Aggregate, aggregate );
        Toggle( child, aggregate );
    }
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::NotifySelected
// Created: LGY 2011-03-04
// -----------------------------------------------------------------------------
void FormationLayer::NotifySelected( const kernel::Formation_ABC* formation )
{
    EntityLayer< kernel::Formation_ABC >::NotifySelected( formation );
    selected_ = formation;
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::Select
// Created: LGY 2011-03-08
// -----------------------------------------------------------------------------
void FormationLayer::Select( const kernel::Entity_ABC& entity, bool )
{
    entity.Select( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::IsAggregated
// Created: LGY 2011-03-07
// -----------------------------------------------------------------------------
bool FormationLayer::IsAggregated( const kernel::Entity_ABC& entity ) const
{
    if( const kernel::Positions* positions = entity.Retrieve< kernel::Positions >() )
        return positions->IsAggregated();
    return false;
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::IsAgent
// Created: LGY 2011-07-18
// -----------------------------------------------------------------------------
bool FormationLayer::IsAgent( const kernel::Entity_ABC& entity ) const
{
    if( entity.GetTypeName() == kernel::Agent_ABC::typeName_ )
        return true;
    return HasSubordinate( entity, boost::bind( &FormationLayer::IsAgent, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::HasSubordinate
// Created: LGY 2011-03-07
// -----------------------------------------------------------------------------
bool FormationLayer::HasSubordinate( const kernel::Entity_ABC& entity, boost::function< bool( const kernel::Entity_ABC& ) > fun ) const
{
    tools::Iterator< const kernel::Entity_ABC& > it = entity.Get< TacticalHierarchies >().CreateSubordinateIterator();
    while( it.HasMoreElements() )
        return fun( it.NextElement() );
    return false;
}

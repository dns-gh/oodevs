// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "FormationPositions.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_gui/AggregatedTools.h"
#include <boost/bind.hpp>

// -----------------------------------------------------------------------------
// Name: FormationPositions constructor
// Created: SBO 2009-02-02
// -----------------------------------------------------------------------------
FormationPositions::FormationPositions( const kernel::Entity_ABC& formation )
    : formation_ ( formation )
    , aggregated_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationPositions destructor
// Created: SBO 2009-02-02
// -----------------------------------------------------------------------------
FormationPositions::~FormationPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationPositions::GetPosition
// Created: SBO 2009-02-02
// -----------------------------------------------------------------------------
geometry::Point2f FormationPositions::GetPosition( bool aggregated ) const
{
    if( !aggregated || !aggregated_ )
    {
        geometry::Point2f aggregatedPosition;
        unsigned int count = 0u;
        tools::Iterator< const kernel::Entity_ABC& > children = formation_.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
        while( children.HasMoreElements() )
        {
            const kernel::Entity_ABC& child = children.NextElement();
            if( HasAggregatedSubordinate( child, boost::bind( &FormationPositions::IsAgent, this, _1 ) ) )
            {
                const geometry::Point2f& childPosition = child.Get< kernel::Positions >().GetPosition( false );
                aggregatedPosition.Set( aggregatedPosition.X() + childPosition.X(), aggregatedPosition.Y() + childPosition.Y() );
                ++count;
            }
        }
        return count ? geometry::Point2f( aggregatedPosition.X() / count, aggregatedPosition.Y() / count ) : aggregatedPosition;
    }
    const kernel::Entity_ABC* superior = formation_.Get< kernel::TacticalHierarchies >().GetSuperior();
    return superior->Get< kernel::Positions >().GetPosition();
}

// -----------------------------------------------------------------------------
// Name: FormationPositions::GetHeight
// Created: SBO 2009-02-02
// -----------------------------------------------------------------------------
float FormationPositions::GetHeight( bool aggregated ) const
{
    if( !aggregated || !aggregated_ )
    {
        float height = 0;
        unsigned count = 0;
        tools::Iterator< const kernel::Entity_ABC& > children = formation_.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
        while( children.HasMoreElements() )
        {
            height += children.NextElement().Get< kernel::Positions >().GetHeight( false );
            ++count;
        }
        return count ? height / count : height;
    }
    const kernel::Entity_ABC* superior = formation_.Get< kernel::TacticalHierarchies >().GetSuperior();
    return superior->Get< kernel::Positions >().GetHeight();
}

// -----------------------------------------------------------------------------
// Name: FormationPositions::IsIn
// Created: SBO 2009-02-02
// -----------------------------------------------------------------------------
bool FormationPositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return rectangle.IsInside( GetPosition( true ) );
}

// -----------------------------------------------------------------------------
// Name: FormationPositions::GetBoundingBox
// Created: SBO 2009-02-02
// -----------------------------------------------------------------------------
geometry::Rectangle2f FormationPositions::GetBoundingBox() const
{
    const geometry::Point2f center = GetPosition( true );
    return geometry::Rectangle2f( center.X() - 1000, center.Y(), center.X() + 1000, center.Y() + 1600 );
}

// -----------------------------------------------------------------------------
// Name: FormationPositions::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void FormationPositions::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    visitor.VisitPoint( GetPosition( true ) );
}

// -----------------------------------------------------------------------------
// Name: FormationPositions::Move
// Created: SBO 2009-02-02
// -----------------------------------------------------------------------------
void FormationPositions::Move( const geometry::Point2f& point )
{
    const geometry::Vector2f vect( GetPosition( false ), point );
    tools::Iterator< const kernel::Entity_ABC& > children = formation_.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const kernel::Positions* positions = children.NextElement().Retrieve< kernel::Positions >();
        if( const kernel::Moveable_ABC* childPositions = dynamic_cast< const kernel::Moveable_ABC* >( positions ) )
            const_cast< kernel::Moveable_ABC& >( *childPositions ).Move( positions->GetPosition( false ) + vect );
    }
}

// -----------------------------------------------------------------------------
// Name: FormationPositions::CanAggregate
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool FormationPositions::CanAggregate() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: FormationPositions::IsAggregated
// Created: LGY 2011-03-04
// -----------------------------------------------------------------------------
bool FormationPositions::IsAggregated() const
{
    return aggregated_;
}

// -----------------------------------------------------------------------------
// Name: FormationPositions::Aggregate
// Created: LGY 2011-03-04
// -----------------------------------------------------------------------------
void FormationPositions::Aggregate( const bool& bDummy )
{
    aggregated_ = bDummy;
}

// -----------------------------------------------------------------------------
// Name: FormationPositions::IsAgent
// Created: LGY 2011-07-18
// -----------------------------------------------------------------------------
bool FormationPositions::IsAgent( const kernel::Entity_ABC& entity ) const
{
    if( entity.GetTypeName() == kernel::Agent_ABC::typeName_ )
        return true;
    return HasAggregatedSubordinate( entity, boost::bind( &FormationPositions::IsAgent, this, _1 ) );
}

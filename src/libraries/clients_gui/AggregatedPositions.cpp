// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "AggregatedPositions.h"
#include "AggregatedTools.h"
#include "GLView_ABC.h"
#include "Viewport_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include <boost/bind.hpp>


using namespace gui;
using namespace kernel;
using namespace geometry;

// -----------------------------------------------------------------------------
// Name: AggregatedPositions constructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
AggregatedPositions::AggregatedPositions( const Entity_ABC& entity )
    : entity_( entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AggregatedPositions destructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
AggregatedPositions::~AggregatedPositions()
{
    // NOTHING
}

namespace
{
    bool HasSubordinate( const Entity_ABC& entity, std::function< bool( const Entity_ABC& ) > fun )
    {
        auto it = entity.Get< TacticalHierarchies >().CreateSubordinateIterator();
        while( it.HasMoreElements() )
            if( fun( it.NextElement() ) )
                return true;
        return false;
    }
    bool IsAgent( const Entity_ABC& entity )
    {
        if( entity.GetTypeName() == Agent_ABC::typeName_ )
            return true;
        return HasSubordinate( entity, &IsAgent );
    }
}

// -----------------------------------------------------------------------------
// Name: AggregatedPositions::GetPosition
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Point2f AggregatedPositions::GetPosition( bool aggregated ) const
{
    if( !aggregated || !entity_.IsAggregated() )
    {
        Point2f aggregatedPosition;
        unsigned int count = 0;
        Point2f fallback;
        auto children = entity_.Get< TacticalHierarchies >().CreateSubordinateIterator();
        while( children.HasMoreElements() )
        {
            const Entity_ABC& child = children.NextElement();
            const Positions& childPositions = child.Get< Positions >();
            fallback = childPositions.GetPosition( false );
            if( childPositions.CanAggregate() && ( IsAgent( child ) || HasSubordinate( child, IsAgent ) ) )
            {
                const Point2f childPosition = fallback;
                aggregatedPosition.Set( aggregatedPosition.X() + childPosition.X(), aggregatedPosition.Y() + childPosition.Y() );
                ++count;
            }
        }
        return count ? Point2f( aggregatedPosition.X() / count, aggregatedPosition.Y() / count ) : fallback;
    }
    return entity_.Get< TacticalHierarchies >().GetUp().Get< Positions >().GetPosition( aggregated );
}

// -----------------------------------------------------------------------------
// Name: AggregatedPositions::GetHeight
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
float AggregatedPositions::GetHeight( bool aggregated ) const
{
    if( !aggregated || !entity_.IsAggregated() )
    {
        float height = 0;
        unsigned int count = 0;
        auto children = entity_.Get< TacticalHierarchies >().CreateSubordinateIterator();
        while( children.HasMoreElements() )
        {
            const Positions& childPositions = children.NextElement().Get< Positions >();
            height += childPositions.GetHeight( false );
            ++count;
        }
        return count ? height / count : height;
    }
    return entity_.Get< TacticalHierarchies >().GetUp().Get< Positions >().GetHeight( aggregated );
}


// -----------------------------------------------------------------------------
// Name: AggregatedPositions::IsIn
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
bool AggregatedPositions::IsIn( const Rectangle2f& rectangle ) const
{
    return rectangle.IsInside( GetPosition( true ) );
}

// -----------------------------------------------------------------------------
// Name: AggregatedPositions::GetBoundingBox
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Rectangle2f AggregatedPositions::GetBoundingBox() const
{
    Rectangle2f result;
    auto children = entity_.Get< TacticalHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const Positions& childPositions = children.NextElement().Get< Positions >();
        result.Incorporate( childPositions.GetPosition( false ) );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: AggregatedPositions::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void AggregatedPositions::Accept( LocationVisitor_ABC& visitor ) const
{
    visitor.VisitPoint( GetPosition( true ) );
}

// -----------------------------------------------------------------------------
// Name: AggregatedPositions::CanAggregate
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool AggregatedPositions::CanAggregate() const
{
    auto children = entity_.Get< TacticalHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const Positions& childPositions = children.NextElement().Get< Positions >();
        if( childPositions.CanAggregate() )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: AggregatedPositions::Draw
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void AggregatedPositions::Draw( const Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    if( viewport.IsHotpointVisible() && !entity_.IsAggregated() && HasSubordinate( entity_, &::IsAggregated ) )
        tools.DrawCross( where, GL_CROSSSIZE, gui::GLView_ABC::pixels );
}

// -----------------------------------------------------------------------------
// Name: AggregatedPositions::Move
// Created: SBO 2007-11-12
// -----------------------------------------------------------------------------
void AggregatedPositions::Move( const Point2f& point )
{
    const Vector2f vect( GetPosition( false ), point );
    auto children = entity_.Get< TacticalHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const Positions* positions = children.NextElement().Retrieve< Positions >();
        if( const Moveable_ABC* childPositions = dynamic_cast< const Moveable_ABC* >( positions ) )
            const_cast< Moveable_ABC& >( *childPositions ).Move( positions->GetPosition( false ) + vect );
    }
}

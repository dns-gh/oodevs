// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AggregatedPositions.h"
#include "clients_gui/AggregatedTools.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include <boost/bind.hpp>

using namespace kernel;
using namespace geometry;

// -----------------------------------------------------------------------------
// Name: AggregatedPositions constructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
AggregatedPositions::AggregatedPositions( const Entity_ABC& entity, float factor )
    : entity_( entity )
    , factor_( factor )
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
    // -----------------------------------------------------------------------------
    // Name: HasSubordinate
    // Created: LGY 2011-03-10
    // -----------------------------------------------------------------------------
    bool HasSubordinate( const kernel::Entity_ABC& entity, std::function< bool( const kernel::Entity_ABC& ) > fun )
    {
        auto it = entity.Get< TacticalHierarchies >().CreateSubordinateIterator();
        while( it.HasMoreElements() )
            return fun( it.NextElement() );
        return false;
    }

    // -----------------------------------------------------------------------------
    // Name: IsAgent
    // Created: LGY 2011-07-18
    // -----------------------------------------------------------------------------
    bool IsAgent( const kernel::Entity_ABC& entity )
    {
        if( entity.GetTypeName() == kernel::Agent_ABC::typeName_ )
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
            const kernel::Entity_ABC& child = children.NextElement();
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
// Name: AggregatedPositions::Draw
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void AggregatedPositions::Draw( const Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    if( viewport.IsHotpointVisible() && !entity_.IsAggregated() && HasSubordinate( entity_, &::IsAggregated ) )
        tools.DrawCross( where, GL_CROSSSIZE, gui::GlTools_ABC::pixels );
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

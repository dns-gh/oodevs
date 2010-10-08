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
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/Viewport_ABC.h"

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

// -----------------------------------------------------------------------------
// Name: AggregatedPositions::GetPosition
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Point2f AggregatedPositions::GetPosition( bool ) const
{
    Point2f aggregatedPosition;
    unsigned count = 0;
    geometry::Point2f fallback;
    tools::Iterator< const Entity_ABC& > children = entity_.Get< TacticalHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const Positions& childPositions = children.NextElement().Get< Positions >();
        fallback = childPositions.GetPosition( false );
        if( childPositions.CanAggregate() )
        {
            const Point2f childPosition = fallback;
            aggregatedPosition.Set( aggregatedPosition.X() + childPosition.X(), aggregatedPosition.Y() + childPosition.Y() );
            ++count;
        }
    }
    return count ? Point2f( aggregatedPosition.X() / count, aggregatedPosition.Y() / count ) : fallback;
}

// -----------------------------------------------------------------------------
// Name: AggregatedPositions::GetHeight
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
float AggregatedPositions::GetHeight( bool ) const
{
    float height = 0;
    unsigned count = 0;
    tools::Iterator< const Entity_ABC& > children = entity_.Get< TacticalHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const Positions& childPositions = children.NextElement().Get< Positions >();
        height+=childPositions.GetHeight( false );
        ++count;
    }
    return count ? height / count : height;
}

// -----------------------------------------------------------------------------
// Name: AggregatedPositions::IsAt
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
bool AggregatedPositions::IsAt( const Point2f& pos, float precision /*= 100.f*/, float /*adaptiveFactor*/ /*= 1.f*/ ) const
{
    // $$$$ AGE 2006-10-06: CP de AgentPositions...
    const float halfSizeX = 500.f * 0.5f * 2.f; // $$$$ SBO 2006-03-21: use font size?
    const float sizeY     = 400.f * 2.f;
    const Point2f position = GetPosition( true );
    const Rectangle2f agentBBox( position.X() - halfSizeX - precision, position.Y() - precision,
                                 position.X() + halfSizeX + precision, position.Y() + sizeY + precision);
    return agentBBox.IsInside( pos );
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
    tools::Iterator< const Entity_ABC& > children = entity_.Get< TacticalHierarchies >().CreateSubordinateIterator();
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
void AggregatedPositions::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    visitor.VisitPoint( GetPosition( true ) );
}

// -----------------------------------------------------------------------------
// Name: AggregatedPositions::Draw
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void AggregatedPositions::Draw( const Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsHotpointVisible() )
        tools.DrawCross( where, GL_CROSSSIZE );
}

// -----------------------------------------------------------------------------
// Name: AggregatedPositions::CanAggregate
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool AggregatedPositions::CanAggregate() const
{
    tools::Iterator< const Entity_ABC& > children = entity_.Get< TacticalHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const Positions& childPositions = children.NextElement().Get< Positions >();
        if( childPositions.CanAggregate() )
            return true;
    }
    return false;
}

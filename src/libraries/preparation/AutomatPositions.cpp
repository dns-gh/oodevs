// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AutomatPositions.h"
#include "AgentPositions.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/Viewport_ABC.h"

using namespace kernel;
using namespace geometry;

// -----------------------------------------------------------------------------
// Name: AutomatPositions constructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
AutomatPositions::AutomatPositions( const Entity_ABC& automat )
    : automat_( automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatPositions destructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
AutomatPositions::~AutomatPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatPositions::GetPosition
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Point2f AutomatPositions::GetPosition() const
{
    Point2f aggregatedPosition;
    unsigned count = 0;
    tools::Iterator< const Entity_ABC& > children = automat_.Get< CommunicationHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const Positions& childPositions = children.NextElement().Get< Positions >();
        const Point2f childPosition = ((const AgentPositions&)( childPositions )).position_;
        aggregatedPosition.Set( aggregatedPosition.X() + childPosition.X(), aggregatedPosition.Y() + childPosition.Y() );
        ++count;
    }
    return count ? Point2f( aggregatedPosition.X() / count, aggregatedPosition.Y() / count ) : aggregatedPosition;
}

// -----------------------------------------------------------------------------
// Name: AutomatPositions::GetHeight
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
float AutomatPositions::GetHeight() const
{
    float height = 0;
    unsigned count = 0;
    tools::Iterator< const Entity_ABC& > children = automat_.Get< CommunicationHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const Positions& childPositions = children.NextElement().Get< Positions >();
        const float childHeight = ((const AgentPositions&)( childPositions )).height_;
        height+=childHeight;
        ++count;
    }
    return count ? height / count : height;
}

// -----------------------------------------------------------------------------
// Name: AutomatPositions::IsAt
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
bool AutomatPositions::IsAt( const Point2f& pos, float precision /*= 100.f*/ ) const
{
    // $$$$ AGE 2006-10-06: CP de AgentPositions...
    const float halfSizeX = 500.f * 0.5f * 2.f; // $$$$ SBO 2006-03-21: use font size?
    const float sizeY     = 400.f * 2.f;
    const Point2f position = GetPosition();
    const Rectangle2f agentBBox( position.X() - halfSizeX - precision, position.Y() - precision,
                                 position.X() + halfSizeX + precision, position.Y() + sizeY + precision);
    return agentBBox.IsInside( pos );
}

// -----------------------------------------------------------------------------
// Name: AutomatPositions::IsIn
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
bool AutomatPositions::IsIn( const Rectangle2f& rectangle ) const
{
    return rectangle.IsInside( GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: AutomatPositions::GetBoundingBox
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Rectangle2f AutomatPositions::GetBoundingBox() const
{
    const Point2f center = GetPosition();
    return Rectangle2f( center.X() - 500, center.Y(), center.X() + 500, center.Y() + 800 );
}

// -----------------------------------------------------------------------------
// Name: AutomatPositions::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void AutomatPositions::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    visitor.VisitPoint( GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: AutomatPositions::Draw
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void AutomatPositions::Draw( const Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsVisible( where ) )
        tools.DrawCross( where, GL_CROSSSIZE );
}

// -----------------------------------------------------------------------------
// Name: AutomatPositions::Move
// Created: SBO 2007-11-12
// -----------------------------------------------------------------------------
void AutomatPositions::Move( const geometry::Point2f& point )
{
    const geometry::Vector2f vect( GetPosition(), point );
    tools::Iterator< const Entity_ABC& > children = automat_.Get< CommunicationHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const Positions* positions = children.NextElement().Retrieve< Positions >();
        if( const kernel::Moveable_ABC* childPositions = dynamic_cast< const kernel::Moveable_ABC* >( positions ) )
            const_cast< kernel::Moveable_ABC& >( *childPositions ).Move( positions->GetPosition() + vect );
    }
}

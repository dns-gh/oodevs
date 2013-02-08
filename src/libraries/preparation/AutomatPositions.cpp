// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AutomatPositions.h"
#include "AgentPositions.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace kernel;
using namespace geometry;

// -----------------------------------------------------------------------------
// Name: AutomatPositions constructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
AutomatPositions::AutomatPositions( const Entity_ABC& automat )
    : automat_   ( automat )
    , aggregated_( false )
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
Point2f AutomatPositions::GetPosition( bool aggregated ) const
{
    if( !aggregated || !aggregated_ )
    {
        Point2f aggregatedPosition;
        unsigned count = 0;
        tools::Iterator< const Entity_ABC& > children = automat_.Get< TacticalHierarchies >().CreateSubordinateIterator();
        while( children.HasMoreElements() )
        {
            const Positions& childPositions = children.NextElement().Get< Positions >();
            const Point2f childPosition = childPositions.GetPosition( false );
            aggregatedPosition.Set( aggregatedPosition.X() + childPosition.X(), aggregatedPosition.Y() + childPosition.Y() );
            ++count;
        }
        return count ? Point2f( aggregatedPosition.X() / count, aggregatedPosition.Y() / count ) : aggregatedPosition;
    }
    const kernel::Entity_ABC* superior = automat_.Get< TacticalHierarchies >().GetSuperior();
    return superior->Get< Positions >().GetPosition( aggregated );
}

// -----------------------------------------------------------------------------
// Name: AutomatPositions::GetHeight
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
float AutomatPositions::GetHeight( bool aggregated ) const
{
    if( !aggregated || !aggregated_ )
    {
        float height = 0;
        unsigned count = 0;
        tools::Iterator< const Entity_ABC& > children = automat_.Get< TacticalHierarchies >().CreateSubordinateIterator();
        while( children.HasMoreElements() )
        {
            const Positions& childPositions = children.NextElement().Get< Positions >();
            const float childHeight = childPositions.GetHeight( false );
            height+=childHeight;
            ++count;
        }
        return count ? height / count : height;
    }
    const kernel::Entity_ABC* superior = automat_.Get< TacticalHierarchies >().GetSuperior();
    return superior->Get< Positions >().GetHeight( aggregated );
}

// -----------------------------------------------------------------------------
// Name: AutomatPositions::IsAt
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
bool AutomatPositions::IsAt( const Point2f& pos, float precision /* = 100.f*/, float /*adaptiveFactor = 1.f*/ ) const
{
    // $$$$ AGE 2006-10-06: CP de AgentPositions...
    const float halfSizeX = 500.f * 0.5f * ( aggregated_ ? 4.f : 2.f ); // $$$$ SBO 2006-03-21: use font size?
    const float sizeY     = 400.f * ( aggregated_ ? 4.f : 2.f );
    const Point2f position = GetPosition( true );
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
    return rectangle.IsInside( GetPosition( true ) );
}

// -----------------------------------------------------------------------------
// Name: AutomatPositions::GetBoundingBox
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Rectangle2f AutomatPositions::GetBoundingBox() const
{
    const Point2f center = GetPosition( true );
    return Rectangle2f( center.X() - 500, center.Y(), center.X() + 500, center.Y() + 800 );
}

// -----------------------------------------------------------------------------
// Name: AutomatPositions::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void AutomatPositions::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    visitor.VisitPoint( GetPosition( true ) );
}

// -----------------------------------------------------------------------------
// Name: AutomatPositions::Draw
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void AutomatPositions::Draw( const Point2f& where, const gui::Viewport_ABC& viewport, const gui::GlTools_ABC& tools ) const
{
    if( viewport.IsVisible( where ) )
        tools.DrawCross( where, GL_CROSSSIZE, gui::GlTools_ABC::pixels );
}

// -----------------------------------------------------------------------------
// Name: AutomatPositions::Move
// Created: SBO 2007-11-12
// -----------------------------------------------------------------------------
void AutomatPositions::Move( const geometry::Point2f& point )
{
    const geometry::Vector2f vect( GetPosition( false ), point );
    tools::Iterator< const Entity_ABC& > children = automat_.Get< TacticalHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const Positions* positions = children.NextElement().Retrieve< Positions >();
        if( const kernel::Moveable_ABC* childPositions = dynamic_cast< const kernel::Moveable_ABC* >( positions ) )
            const_cast< kernel::Moveable_ABC& >( *childPositions ).Move( positions->GetPosition( false ) + vect );
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatPositions::Aggregate
// Created: LGY 2011-03-03
// -----------------------------------------------------------------------------
void AutomatPositions::Aggregate( const bool& bDummy )
{
    aggregated_ = bDummy;
}

// -----------------------------------------------------------------------------
// Name: AutomatPositions::CanAggregate
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool AutomatPositions::CanAggregate() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: AutomatPositions::IsAggregated
// Created: LGY 2011-03-04
// -----------------------------------------------------------------------------
bool AutomatPositions::IsAggregated() const
{
    return aggregated_;
}

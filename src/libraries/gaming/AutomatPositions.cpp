// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AutomatPositions.h"
#include "AgentPositions.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include <numeric>

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
    UpdateChildrenPositions();
    return position_;
}

// -----------------------------------------------------------------------------
// Name: AutomatPositions::UpdateChildrenPositions
// Created: AGE 2007-05-23
// -----------------------------------------------------------------------------
void AutomatPositions::UpdateChildrenPositions() const
{
    Iterator< const Entity_ABC& > children = automat_.Get< CommunicationHierarchies >().CreateSubordinateIterator();
    unsigned count = 0;
    geometry::Point2f current;
    bool dirty = false;
    while( children.HasMoreElements() )
    {
        const AgentPositions& childPositions = (const AgentPositions&)children.NextElement().Get< Positions >();
        current = childPositions.position_;
        if( ! childPositions.dead_ )
        {
            if( dirty || children_.size() <= count )
            {
                dirty = true;
                children_.resize( count + 1 );
            }
            dirty = dirty || children_[ count ] != current;
            children_[ count ] = current;
            ++count;
        }
    }
    dirty = dirty || children_.size() > count;
    children_.resize( count );
    
    if( dirty )
    {
        position_ .Set( 0, 0 );
        leftMost_ .Set( std::numeric_limits< float >::infinity(), std::numeric_limits< float >::infinity() );
        rightMost_.Set(-std::numeric_limits< float >::infinity(),-std::numeric_limits< float >::infinity() );
        for( CIT_PointVector it = children_.begin(); it != children_.end(); ++it )
        {
            if( it->X() < leftMost_.X() )
                leftMost_ = *it;
            if( it->X() > rightMost_.X() )
                rightMost_ = *it;
            position_.Set( position_.X() + it->X(), position_.Y() + it->Y() );
        }
        if( children_.size() )
            position_.Set( position_.X() / count, position_.Y() / count );
        else
            position_ = current;
        ComputeHull();
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatPositions::GetHeight
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
float AutomatPositions::GetHeight() const
{
    float height = 0;
    unsigned count = 0;
    Iterator< const Entity_ABC& > children = automat_.Get< CommunicationHierarchies >().CreateSubordinateIterator();
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
    Rectangle2f result;
    Iterator< const Entity_ABC& > children = automat_.Get< CommunicationHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const AgentPositions& childPositions = (const AgentPositions&)children.NextElement().Get< Positions >();
        if( !childPositions.dead_ )
            result.Incorporate( childPositions.position_ );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: AutomatPositions::Draw
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void AutomatPositions::Draw( const Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsHotpointVisible() )
    {
        tools.DrawCross( where, GL_CROSSSIZE );
        tools.DrawConvexPolygon( hull_ );
    }
}


namespace
{
    bool FindOuterPoint( const T_PointVector& points, const Point2f& from, const Vector2f& direction, Point2f& worst )
    {
        bool bFound = false;
        float rMaxProjection = 0;
        for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
        {
            const Vector2f v( from, *it );
            const float rProjection = direction.CrossProduct( v );
            if( rProjection < -1 ) // epsilon
            {
                bFound = true;
                if( rMaxProjection > rProjection )
                {
                    rMaxProjection = rProjection;
                    worst = *it;
                }
            }
        }
        return bFound;
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatPositions::ComputeHull
// Created: AGE 2007-05-23
// -----------------------------------------------------------------------------
void AutomatPositions::ComputeHull() const
{
    hull_.resize( 0 );
    hull_.push_back( leftMost_ );
    hull_.push_back( rightMost_ );
    unsigned int nPoint = 0;
    while( nPoint != hull_.size() )
    {
        unsigned int nFollowingPoint = ( nPoint + 1 ) % hull_.size();
        Vector2f direction( hull_[ nPoint ], hull_[ nFollowingPoint ] );
        direction.Normalize();
        Point2f worst;
        if( FindOuterPoint( children_, hull_[ nPoint ], direction, worst ) )
        {
            hull_.insert( hull_.begin() + nFollowingPoint, worst );
            nPoint = 0;
        }
        else
            ++nPoint;
    }
}


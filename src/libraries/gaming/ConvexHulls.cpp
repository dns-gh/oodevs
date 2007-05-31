// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ConvexHulls.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include <numeric>

using namespace kernel;
using namespace geometry;

// -----------------------------------------------------------------------------
// Name: ConvexHulls constructor
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
ConvexHulls::ConvexHulls( const kernel::Entity_ABC& holder )
    : holder_( holder )
    , dead_  ( false )
    , dirty_ ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConvexHulls destructor
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
ConvexHulls::~ConvexHulls()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConvexHulls::Draw
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void ConvexHulls::Draw( const Point2f& , const Viewport_ABC& , const GlTools_ABC& tools ) const
{
    if( tools.ShouldDisplay( "ConvexHulls" ) )
    {
        Update();
        tools.DrawConvexPolygon( hull_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ConvexHulls::IsDirty
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
bool ConvexHulls::IsDirty() const
{
    if( ! dirty_ )
    {
        Iterator< const Entity_ABC& > children = holder_.Get< TacticalHierarchies >().CreateSubordinateIterator();
        while( ! dirty_ && children.HasMoreElements() )
            if( const ConvexHulls* childHulls = children.NextElement().Retrieve< ConvexHulls >() )
                dirty_ = childHulls->IsDirty();
    }
    return dirty_;
}

// -----------------------------------------------------------------------------
// Name: ConvexHulls::Update
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void ConvexHulls::Update() const
{
    dirty_ = true;
    T_PointVector children;
    Point2f leftMost, rightMost;
    Accumulate( children, leftMost, rightMost );
}

// -----------------------------------------------------------------------------
// Name: ConvexHulls::Accumulate
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void ConvexHulls::Accumulate( T_PointVector& points, Point2f& leftMost, Point2f& rightMost ) const
{
    if( IsDirty() )
        Update( points, leftMost, rightMost );
    else
        AccumulateHull( points, leftMost, rightMost );
}

// -----------------------------------------------------------------------------
// Name: ConvexHulls::Update
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void ConvexHulls::Update( T_PointVector& points, Point2f& leftMost, Point2f& rightMost ) const
{
    leftMost_ .Set( std::numeric_limits< float >::infinity(), std::numeric_limits< float >::infinity() );
    rightMost_.Set(-std::numeric_limits< float >::infinity(),-std::numeric_limits< float >::infinity() );
    unsigned currentSize = points.size();
    bool hasChild = false;
    Iterator< const Entity_ABC& > children = holder_.Get< TacticalHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
        if( const ConvexHulls* childHulls = children.NextElement().Retrieve< ConvexHulls >() )
        {
            if( ! childHulls->IsDead() )
            {
                childHulls->Accumulate( points, leftMost_, rightMost_ );
                hasChild = true;
            }
            position_ = childHulls->position_;
        }

    if( !hasChild )
    {
        leftMost_ = rightMost_ = position_;
        points.push_back( position_ );
    }

    if( leftMost.X() > leftMost_.X() )
        leftMost = leftMost_;
    if( rightMost.X() < rightMost_.X() )
        rightMost = rightMost_;

    ComputeHull( points.begin() + currentSize, points.end() );
    dirty_ = false;
}

// -----------------------------------------------------------------------------
// Name: ConvexHulls::AccumulateHull
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void ConvexHulls::AccumulateHull( T_PointVector& points, Point2f& leftMost, Point2f& rightMost ) const
{
    for( CIT_PointVector it = hull_.begin(); it != hull_.end(); ++it )
    {
        const Point2f& position = *it;
        points.push_back( position );
        if( leftMost.X() > position.X() )
            leftMost = position;
        if( rightMost.X() < position.X() )
            rightMost = position;
    }
}


namespace
{
    bool FindOuterPoint( CIT_PointVector begin, CIT_PointVector end, const Point2f& from, const Vector2f& direction, Point2f& worst )
    {
        bool bFound = false;
        float rMaxProjection = 0;
        for( CIT_PointVector it = begin; it != end; ++it )
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
// Name: ConvexHulls::ComputeHull
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void ConvexHulls::ComputeHull( CIT_PointVector from, CIT_PointVector to ) const
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
        if( FindOuterPoint( from, to, hull_[ nPoint ], direction, worst ) )
        {
            hull_.insert( hull_.begin() + nFollowingPoint, worst );
            nPoint = 0;
        }
        else
            ++nPoint;
    }
}

// -----------------------------------------------------------------------------
// Name: ConvexHulls::SetDead
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void ConvexHulls::SetDead( bool dead )
{
    dirty_ = dead_ != dead;
    dead_ = dead;
}

// -----------------------------------------------------------------------------
// Name: ConvexHulls::IsDead
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
bool ConvexHulls::IsDead() const
{
    return dead_;
}

// -----------------------------------------------------------------------------
// Name: ConvexHulls::SetPosition
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void ConvexHulls::SetPosition( const geometry::Point2f& point )
{
    if( position_ != point )
    {
        position_ = point;
        dirty_ = true;
    }
}

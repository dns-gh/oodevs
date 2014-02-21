// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "DetectionMapIterator.h"
#include "DetectionMap.h"

using namespace kernel;
using namespace geometry;

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator constructor
// Created: AGE 2006-04-06
// -----------------------------------------------------------------------------
DetectionMapIterator::DetectionMapIterator( const DetectionMap& map, const Point2f& from, const Point2f& to )
    : map_( map )
    , cellSize_( map_.GetCellSize() )
    , endCell_( map.Unmap( to ) )
    , xEnd_( to.X() )
    , yEnd_( to.Y() )
    , currentCell_( map_.Unmap( from ) )
    , xCurrent_( from.X() )
    , yCurrent_( from.Y() )
    , xOffset_( 0 )
    , yOffset_( 0 )
    , horizontalLine_( false )
{
    const Point2f cellCenter = map_.Map( currentCell_.first, currentCell_.second );
    xOffset_ = xCurrent_ - cellCenter.X();
    yOffset_ = yCurrent_ - cellCenter.Y();

    const float xDelta = xEnd_ - xCurrent_;
    const float yDelta = yEnd_ - yCurrent_;
    if( std::fabs( xDelta ) > std::fabs( yDelta ) )
    {
        if( xDelta > 0 )
            xIncrease_ = cellSize_;
        else
            xIncrease_ = - cellSize_;
        yIncrease_ = yDelta / xDelta * xIncrease_;
    }
    else
    {
        if( yDelta > 0 )
            yIncrease_ = cellSize_;
        else
            yIncrease_ = - cellSize_;
        xIncrease_ = xDelta / yDelta * yIncrease_;
    }
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::Increment
// Created: AGE 2006-04-06
// -----------------------------------------------------------------------------
void DetectionMapIterator::Increment()
{
    Vector2f increment = ComputeIncrement();

    xCurrent_ += increment.X();
    xOffset_  += increment.X();
    if( xOffset_ >= cellSize_ * .5f )
    {
        ++currentCell_.first;
        xOffset_ -= cellSize_;
    }
    else if( xOffset_ <= - cellSize_ * .5f )
    {
        --currentCell_.first;
        xOffset_ += cellSize_;
    }

    yCurrent_ += increment.Y();
    yOffset_  += increment.Y();
    if( yOffset_ >= cellSize_ * .5f )
    {
        ++currentCell_.second;
        yOffset_ -= cellSize_;
    }
    else if( yOffset_ <= -cellSize_ * .5f )
    {
        --currentCell_.second;
        yOffset_ += cellSize_;
    }
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::ComputeIncrement
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
Vector2f DetectionMapIterator::ComputeIncrement()
{
    float xDelta = ( ( xIncrease_ > 0 ) ? cellSize_*.51f : -cellSize_*.51f ) - xOffset_;
    float yDelta = ( ( yIncrease_ > 0 ) ? cellSize_*.51f : -cellSize_*.51f ) - yOffset_;
    horizontalLine_ = ( xIncrease_ == 0 )
        || ( ( yIncrease_ != 0 ) && ( yDelta / yIncrease_ < xDelta / xIncrease_ ) );
    return horizontalLine_ ?
                Vector2f( yDelta / yIncrease_ * xIncrease_, yDelta ) :
                Vector2f( xDelta, xDelta / xIncrease_ * yIncrease_ );
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::IsDone
// Created: AGE 2006-04-06
// -----------------------------------------------------------------------------
bool DetectionMapIterator::IsDone() const
{
    return CurrentPoint().SquareDistance( Point2f( xEnd_, yEnd_ ) ) < cellSize_ * cellSize_;
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::Altitude
// Created: AGE 2006-04-06
// -----------------------------------------------------------------------------
short DetectionMapIterator::Altitude() const
{
    short base = *map_.Data( currentCell_.first, currentCell_.second );
    short next;
    float alpha;
    if( horizontalLine_ )
    {
        //assert( yOffset_ ~= cellSize_/2 );
        unsigned nextX = currentCell_.first;
        if( currentCell_.first < map_.Width() - 1 )
            ++nextX;
        alpha = ( cellSize_ *.5f - xOffset_ ) / cellSize_;
        next = *map_.Data( nextX, currentCell_.second );
    }
    else
    {
        //assert( xOffset_ ~= cellSize_/2 );
        unsigned nextY = currentCell_.second;
        if( currentCell_.first < map_.Height() - 1 )
            ++nextY;
        alpha = ( cellSize_ *.5f - yOffset_ ) / cellSize_;
        next = *map_.Data( currentCell_.first, nextY );
    }
    return short( alpha *  base + ( 1 - alpha ) * next );
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::Elevation
// Created: AGE 2006-04-06
// -----------------------------------------------------------------------------
short DetectionMapIterator::Elevation() const
{
    return Altitude() + map_.EnvironmentData( currentCell_.first, currentCell_.second )->ElevationDelta();
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::IsInTown
// Created: AGE 2006-04-06
// -----------------------------------------------------------------------------
bool DetectionMapIterator::IsInTown() const
{
    return map_.EnvironmentData( currentCell_.first, currentCell_.second )->IsInTown();
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::IsInForest
// Created: AGE 2006-04-06
// -----------------------------------------------------------------------------
bool DetectionMapIterator::IsInForest() const
{
    return map_.EnvironmentData( currentCell_.first, currentCell_.second )->IsInForest();
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::CurrentPoint
// Created: AGE 2006-04-06
// -----------------------------------------------------------------------------
Point2f DetectionMapIterator::CurrentPoint() const
{
    return Point2f( xCurrent_, yCurrent_ );
}

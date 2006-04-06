// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "DetectionMapIterator.h"
#include "DetectionMap.h"

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator constructor
// Created: AGE 2006-04-06
// -----------------------------------------------------------------------------
DetectionMapIterator::DetectionMapIterator( const DetectionMap& map, const geometry::Point2f& from, const geometry::Point2f& to )
    : map_( map )
    , cellSize_( map_.GetCellSize() )
    , endCell_( map.Unmap( to ) )
    , xEnd_( to.X() )
    , yEnd_( to.Y() )
    , currentCell_( map.Unmap( from ) )
    , xCurrent_( from.X() )
    , yCurrent_( from.Y() )
    , xOffset_( 0 )
    , yOffset_( 0 )
{
    const float xDelta = xEnd_ - xCurrent_;
    const float yDelta = yEnd_ - yCurrent_;
    if( std::fabs( xDelta ) > std::fabs( yDelta ) )
    {
        xIncrease_ = cellSize_;
        yIncrease_ = ( yEnd_ - yCurrent_ ) / xDelta * cellSize_;
    }
    else
    {
        yIncrease_ = cellSize_;
        xIncrease_ = ( xEnd_ - xCurrent_ ) / yDelta * cellSize_;
    }
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator destructor
// Created: AGE 2006-04-06
// -----------------------------------------------------------------------------
DetectionMapIterator::~DetectionMapIterator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::Increment
// Created: AGE 2006-04-06
// -----------------------------------------------------------------------------
void DetectionMapIterator::Increment()
{
    xCurrent_ += xIncrease_;
    xOffset_  += xIncrease_;
    if( xOffset_ >= cellSize_ )
    {
        ++currentCell_.first;
        xOffset_ -= cellSize_;
    }
    else if( xOffset_ <= -cellSize_ )
    {
        --currentCell_.first;
        xOffset_ += cellSize_;
    } 

    yCurrent_ += yIncrease_;
    yOffset_  += yIncrease_;
    if( yOffset_ >= cellSize_ )
    {
        ++currentCell_.second;
        yOffset_ -= cellSize_;
    }
    else if( yOffset_ <= -cellSize_ )
    {
        --currentCell_.second;
        yOffset_ += cellSize_;
    } 
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::IsDone
// Created: AGE 2006-04-06
// -----------------------------------------------------------------------------
bool DetectionMapIterator::IsDone() const
{
    return currentCell_ == endCell_;
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::Altitude
// Created: AGE 2006-04-06
// -----------------------------------------------------------------------------
short DetectionMapIterator::Altitude() const
{
    return *map_.Data( currentCell_.first, currentCell_.second );
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
geometry::Point2f DetectionMapIterator::CurrentPoint() const
{
    return geometry::Point2f( xCurrent_, yCurrent_ );
}

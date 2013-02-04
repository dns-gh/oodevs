// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ElevationGrid.h"

ElevationGrid::sCell ElevationGrid::emptyCell_;

// -----------------------------------------------------------------------------
// Name: ElevationGrid constructor
// Created: LGY 2013-02-04
// -----------------------------------------------------------------------------
ElevationGrid::ElevationGrid( double cellSize, unsigned int width, unsigned int height,
                              sCell** ppCells )
    : ElevationBaseGrid( cellSize, width, height )
    , ppCells_( ppCells )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ElevationGrid destructor
// Created: LGY 2013-02-04
// -----------------------------------------------------------------------------
ElevationGrid::~ElevationGrid()
{
    for( unsigned int i = width_; i; )
        delete [] ppCells_[ --i ];
    delete [] ppCells_;
    ppCells_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ElevationGrid::GetCellAltitude
// Created: LGY 2013-02-04
// -----------------------------------------------------------------------------
short ElevationGrid::GetCellAltitude( unsigned int col, unsigned int row ) const
{
    return GetCell( col, row ).GetAltitude();
}

// -----------------------------------------------------------------------------
// Name: ElevationGrid::GetCell
// Created: LGY 2013-02-04
// -----------------------------------------------------------------------------
ElevationGrid::sCell& ElevationGrid::GetCell( unsigned int x, unsigned int y ) const
{
    return ( x < width_ && y < height_ ) ? ppCells_[ x ][ y ] : emptyCell_;
}

// -----------------------------------------------------------------------------
// Name: ElevationGrid::GetEmptyCell
// Created: LGY 2013-02-04
// -----------------------------------------------------------------------------
ElevationGrid::sCell& ElevationGrid::GetEmptyCell() const
{
    return emptyCell_;
}

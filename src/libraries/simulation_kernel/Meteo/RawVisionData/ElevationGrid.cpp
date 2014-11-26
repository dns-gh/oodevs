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
#include <tools/InputBinaryStream.h>
#include <tools/Path.h>

ElevationCell ElevationGrid::emptyCell_;

// -----------------------------------------------------------------------------
// Name: ElevationGrid constructor
// Created: LGY 2013-02-04
// -----------------------------------------------------------------------------
ElevationGrid::ElevationGrid( double cellSize, unsigned int width, unsigned int height, ElevationCell** ppCells )
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
ElevationCell& ElevationGrid::GetCell( unsigned int x, unsigned int y ) const
{
    return ( x < width_ && y < height_ ) ? ppCells_[ x ][ y ] : emptyCell_;
}

// -----------------------------------------------------------------------------
// Name: ElevationGrid::GetEmptyCell
// Created: LGY 2013-02-04
// -----------------------------------------------------------------------------
ElevationCell& ElevationGrid::GetEmptyCell() const
{
    return emptyCell_;
}

std::unique_ptr< ElevationGrid > LoadElevationGrid( const tools::Path& path )
{
    tools::InputBinaryStream archive( path );
    if( !archive )
        throw MASA_EXCEPTION( "Cannot open file " + path.ToUTF8() );
    double cellSize = 0;
    unsigned int rows = 0, columns = 0;
    if( !( archive >> cellSize >> rows >> columns ) )
       throw MASA_EXCEPTION( "Error reading file " + path.ToUTF8() );

    ElevationCell** cells = new ElevationCell*[ columns ];

    uint16_t elevation;
    uint8_t env, delta;
    for( unsigned int x = 0; x < columns; ++x )
    {
        ElevationCell* cell = new ElevationCell[ rows ];
        cells[ x ] = cell;

        for( unsigned int i = 0; i < rows; ++i )
        {
            archive >> elevation >> delta >> env;
            if( !archive )
                throw MASA_EXCEPTION( "Error reading file " + path.ToUTF8() );
            cell->h = elevation;
            cell->dh = delta;
            cell->e = env;
            cell->weatherId = 0;
            cell->pEffects = 0;
            cell++;
        }
    }
    return std::unique_ptr< ElevationGrid >(
            new ElevationGrid( cellSize, columns, rows, cells ) );
}

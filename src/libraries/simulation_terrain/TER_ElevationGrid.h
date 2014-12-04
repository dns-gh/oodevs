// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef TER_ELEVATIONGRID_H
#define TER_ELEVATIONGRID_H

#include <graphics/ElevationBaseGrid.h>
#include <memory>

namespace tools
{
    class Path;
}

class TER_ElevationGrid;

typedef unsigned char envBits;  // bit field

std::unique_ptr< TER_ElevationGrid > LoadElevationGrid( const tools::Path& path );

struct ElevationCell
{
public:
    ElevationCell()
        : h       ( 0 )
        , dh      ( 0 )
        , e       ( 0 )
    {
        // NOTHING
    }
    short GetAltitude() const          { return h; }
    unsigned char GetEnvHeight() const { return dh; }
    envBits GetEnv() const             { return e; }

    bool operator == ( const ElevationCell& rhs ) const
    {
        return h == rhs.h && dh == rhs.dh && e == rhs.e;
    }
private:
    friend std::unique_ptr< TER_ElevationGrid > LoadElevationGrid( const tools::Path& );

    // Take care to pack the following fields to save memory when
    // loading large elevation maps.
    uint16_t h;     // elevation
    uint8_t  dh;    // elevation delta caused by environment
    envBits  e;     // static environment bits
};

class TER_ElevationGrid : public ElevationBaseGrid
{
public:
    //! @name Constructors/Destructor
    //@{
             TER_ElevationGrid( double cellSize, unsigned int width, unsigned int height, ElevationCell** ppCells );
    virtual ~TER_ElevationGrid();
    //@}

    //! @name Operations
    //@{
    virtual short GetCellAltitude( unsigned int col, unsigned int row ) const;
    ElevationCell& GetCell( unsigned int x, unsigned int y ) const;
    ElevationCell& GetEmptyCell() const;
    //@}

private:
    //! @name Member data
    //@{
    ElevationCell** ppCells_;
    static ElevationCell emptyCell_;
    //@}
};

#endif // TER_ELEVATIONGRID_H

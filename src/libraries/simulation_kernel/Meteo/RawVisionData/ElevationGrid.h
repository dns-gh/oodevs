// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef ElevationGrid_h
#define ElevationGrid_h

#include <graphics/ElevationBaseGrid.h>

class PHY_AmmoEffect;

typedef unsigned char envBits;  // bit field

struct ElevationCell
{
public:
    ElevationCell()
        : h       ( 0 )
        , dh      ( 0 )
        , e       ( 0 )
        , weatherId( 0 )
        , pEffects( 0 )
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
    friend class PHY_RawVisionData;

    // Take care to pack the following fields to save memory when
    // loading large elevation maps.
    PHY_AmmoEffect* pEffects;       // ammunitions effects
    uint16_t h;     // elevation
    uint8_t  dh;    // elevation delta caused by environment
    envBits  e;     // static environment bits
    uint32_t weatherId; // local weather identifier, 0 if unset
};
//@}

// =============================================================================
/** @class  ElevationGrid
    @brief  ElevationGrid
*/
// Created: LGY 2013-02-04
// =============================================================================
class ElevationGrid : public ElevationBaseGrid
{
public:
    //! @name Types
    //@{

public:
    //! @name Constructors/Destructor
    //@{
             ElevationGrid( double cellSize, unsigned int width, unsigned int height, ElevationCell** ppCells );
    virtual ~ElevationGrid();
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

#endif // ElevationGrid_h

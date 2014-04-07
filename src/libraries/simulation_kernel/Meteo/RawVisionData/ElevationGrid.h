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

namespace weather
{
    class Meteo;
}

typedef unsigned char envBits;  // bit field

struct ElevationCell
{
public:
    ElevationCell()
        : h       ( 0 )
        , dh      ( 0 )
        , e       ( 0 )
        , pMeteo  ( 0 )
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
    // $$$$ _RC_ JSR 2011-05-19: TODO à cleaner (virer le friend, rajouter des underscores...)
    friend class PHY_RawVisionData;

    unsigned short h  : 16;                         // hauteur du sol
    unsigned char  dh : 8;                          // hauteur de la planimétrie
    envBits        e  : 8;                          // champ de bit représentant l'environnement visuel statique
    const weather::Meteo* pMeteo;                   // local weather
    PHY_AmmoEffect* pEffects;                       // effets météo provoqués par des munitions ( fumigènes, obus eclairants )
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

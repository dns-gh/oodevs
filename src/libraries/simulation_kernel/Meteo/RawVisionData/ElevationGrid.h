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
    struct WindData;
}

typedef unsigned char envBits;  // champ de bit

struct ElevationCell
{
public:
    ElevationCell()
        : h       ( 0 )
        , dh      ( 0 )
        , e       ( 0 )
        , pEffects( 0 )
    {
        // NOTHING
    }
    short GetAltitude() const          { return h; }
    unsigned char GetEnvHeight() const { return dh; }
    envBits GetEnv() const             { return e; }
    const weather::WindData&             GetWind         () const;

    bool operator == ( const ElevationCell& rhs ) const
    {
        return h == rhs.h && dh == rhs.dh && e == rhs.e;
    }
private:
    // $$$$ _RC_ JSR 2011-05-19: TODO � cleaner (virer le friend, rajouter des underscores...)
    friend class PHY_RawVisionData;

    unsigned short h  : 16;                         // hauteur du sol
    unsigned char  dh : 8;                          // hauteur de la planim�trie
    envBits        e  : 8;                          // champ de bit repr�sentant l'environnement visuel statique
    boost::shared_ptr< weather::Meteo > pMeteo;     // m�t�o locale
    PHY_AmmoEffect* pEffects;                       // effets m�t�o provoqu�s par des munitions ( fumig�nes, obus eclairants )
    static const weather::Meteo* pGlobalMeteo_;
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

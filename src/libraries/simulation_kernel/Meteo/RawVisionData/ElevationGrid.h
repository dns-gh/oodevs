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

#include "meteo/Meteo.h"
#include <graphics/ElevationBaseGrid.h>

class PHY_AmmoEffect;

// =============================================================================
/** @class  ElevationGrid
    @brief  ElevationGrid
*/
// Created: LGY 2013-02-04
// =============================================================================
class ElevationGrid : public ElevationBaseGrid
{
public:
    //! @name types
    //@{
    typedef unsigned char envBits;  // champ de bit

    // cellule de la matrice de vision
    struct sCell
    {
    public:
        sCell()
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
        const weather::PHY_Precipitation&    GetPrecipitation() const;
        const weather::PHY_Lighting&         GetLighting     () const;
        const weather::Meteo::sWindData& GetWind         () const;

        bool operator == ( const sCell& rhs ) const
        {
            return h == rhs.h && dh == rhs.dh && e == rhs.e;
        }
    private:
        // $$$$ _RC_ JSR 2011-05-19: TODO à cleaner (virer le friend, rajouter des underscores...)
        friend class PHY_RawVisionData;

        unsigned short h  : 16;                         // hauteur du sol
        unsigned char  dh : 8;                          // hauteur de la planimétrie
        envBits        e  : 8;                          // champ de bit représentant l'environnement visuel statique
        boost::shared_ptr< weather::Meteo > pMeteo;     // météo locale
        PHY_AmmoEffect* pEffects;                       // effets météo provoqués par des munitions ( fumigènes, obus eclairants )

        static const weather::Meteo* pGlobalMeteo_;

    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ElevationGrid( double cellSize, unsigned int width, unsigned int height,
                            sCell** ppCells );
    virtual ~ElevationGrid();
    //@}

    //! @name Operations
    //@{
    virtual short GetCellAltitude( unsigned int col, unsigned int row ) const;
    sCell& GetCell( unsigned int x, unsigned int y ) const;
    sCell& GetEmptyCell() const;
    //@}

private:
    //! @name Member data
    //@{
    sCell** ppCells_;
    static sCell emptyCell_;
    //@}
};

#endif // ElevationGrid_h

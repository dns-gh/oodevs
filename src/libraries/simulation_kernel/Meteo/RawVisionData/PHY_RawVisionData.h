// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

#ifndef __PHY_RawVisionData_h_
#define __PHY_RawVisionData_h_

#include "meteo/PHY_Meteo.h"
#include "meteo/PHY_RawVisionData_ABC.h"

namespace tools
{
    class WorldParameters;
}

class PHY_AmmoEffect;
class MT_Ellipse;
class PHY_IndirectFireDotationClass;
class TER_Localisation;

//*****************************************************************************
// Created: JVT 02-11-05
// Last modified: JVT 04-03-24
//*****************************************************************************
class PHY_RawVisionData : public weather::PHY_RawVisionData_ABC
                        , private boost::noncopyable
{
public:
    // environnement visuel statique
    enum E_VisionObject
    {
        eVisionEmpty      = 0x0,
        eVisionForest     = 0x1,
        eVisionUrban      = 0x2,
        eVisionGround     = 0x4,
        eNbrVisionObjects = 4
    };

    typedef unsigned char envBits;  // champ de bit

    // cellule de la matrice de vision
    struct sCell
    {
    public:
        sCell()
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
        const weather::PHY_Precipitation&    GetPrecipitation() const;
        const weather::PHY_Lighting&         GetLighting     () const;
        const weather::PHY_Meteo::sWindData& GetWind         () const;

        bool operator == ( const sCell& rhs ) const
        {
            return h == rhs.h && dh == rhs.dh && e == rhs.e;
        }
    private:
        // $$$$ _RC_ JSR 2011-05-19: TODO à cleaner (virer le friend, rajouter des underscores...)
        friend class PHY_RawVisionData;

        unsigned short h  : 16;     // hauteur du sol
        unsigned char  dh : 8;      // hauteur de la planimétrie
        envBits        e  : 8;      // champ de bit représentant l'environnement visuel statique
        weather::PHY_Meteo* pMeteo; // météo locale
        PHY_AmmoEffect* pEffects;   // effets météo provoqués par des munitions ( fumigènes, obus eclairants )

        static const weather::PHY_Meteo* pGlobalMeteo_;

    };

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RawVisionData( weather::PHY_Meteo& globalMeteo, tools::WorldParameters& config );
    virtual ~PHY_RawVisionData();
    //@}

    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{
    const sCell& operator () ( const MT_Vector2D& ) const;
    const sCell& operator () ( double, double ) const;

    double GetCellSize() const;

    const weather::PHY_Precipitation& GetPrecipitation( const MT_Vector2D& ) const;

    double GetAltitude( const MT_Vector2D& pos ) const;
    double GetAltitude( double rX_, double rY_ ) const;

    envBits GetVisionObject( const MT_Vector2D& pos ) const;
    envBits GetVisionObject( double rX_, double rY_ ) const;

    const weather::PHY_Meteo::sWindData& GetWind( const MT_Vector2D& vPos ) const;

    void ModifyAltitude( const TER_Localisation& localisation, short heightOffset );
    double GetMinAltitude() const;
    double GetMaxAltitude() const;
    void CalcMinMaxAltitude();

    static unsigned int ConvertEnvironementToObjectIdx( E_VisionObject obj );
    static E_VisionObject ConvertObjectIdxToEnvironnement( unsigned int );

    template< typename T >
    void GetVisionObjectsInSurface( const T& localisation, unsigned int& rEmptySurface, unsigned int& rForestSurface, unsigned int& rUrbanSurface ) const;

    void RegisterMeteoPatch  ( const geometry::Point2d&, const geometry::Point2d&, weather::PHY_Meteo* );
    void UnregisterMeteoPatch( const geometry::Point2d&, const geometry::Point2d&, weather::PHY_Meteo* );

    void RegisterWeatherEffect  ( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& weaponCategory );
    void UnregisterWeatherEffect( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& weaponCategory );
    //@}

    //-------------------------------------------------------------------------
    /** @name IN/OUT */
    //-------------------------------------------------------------------------
    //@{
    bool Read( const std::string& strFile );
    //@}

private:
    friend class PHY_RawVisionDataIterator;

    // Convertisseurs de coordonnées SIM en coordonnées du tableau
    unsigned int GetCol( double ) const;
    unsigned int GetRow( double ) const;
    const sCell& operator () ( unsigned int col, unsigned int row ) const;
    sCell& operator () ( double, double );

    double rCellSize_; // taille (en metre) du côté de la cellule

    unsigned int nNbrCol_;
    unsigned int nNbrRow_;

    sCell** ppCells_;

    double rMinAltitude_;
    double rMaxAltitude_;

    static sCell emptyCell_;
};

#include "PHY_RawVisionData.inl"

#endif // __PHY_RawVisionData_h_

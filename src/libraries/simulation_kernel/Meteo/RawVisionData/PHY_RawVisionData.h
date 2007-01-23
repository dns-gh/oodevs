//*****************************************************************************
//
// $Created: JVT 02-11-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Meteo/RawVisionData/PHY_RawVisionData.h $
// $Author: Nld $
// $Modtime: 17/03/05 14:53 $
// $Revision: 5 $
// $Workfile: PHY_RawVisionData.h $
//
//*****************************************************************************

#ifndef __PHY_RawVisionData_h_
#define __PHY_RawVisionData_h_

#include "MIL.h"
#include "Meteo/PHY_Meteo.h"

class MIL_Config;
class PHY_AmmoEffect;
class MT_Ellipse;
class PHY_Precipitation;
class PHY_Lighting;
class PHY_IndirectFireDotationClass;

//*****************************************************************************
// Created: JVT 02-11-05
// Last modified: JVT 04-03-24
//*****************************************************************************
class PHY_RawVisionData
{
    MT_COPYNOTALLOWED( PHY_RawVisionData )

public:
    enum E_AlignType
    {
        eTopLeft,
        eTopRight,
        eBottomLeft,
        eBottomRight,
        eCenter
    };

    // environnement visuel statique
    enum E_VisionObject
    {
        eVisionEmpty    = 0x0,
        eVisionForest   = 0x1,
        eVisionUrban    = 0x2,
        eVisionGround   = 0x4,

        eNbrVisionObjects = 4
    };

    typedef uint8 envBits;  // champ de bit

    // cellule de la matrice de vision
    struct sCell
    {
    private:
        friend class PHY_RawVisionData;

        int16           h     : 16;  // hauteur du sol
        uint8           dh    : 8;   // hauteur de la planimétrie
        envBits         e     : 8;   // champ de bit représentant l'environnement visuel statique
        PHY_Meteo*      pMeteo;      // météo locale
        PHY_AmmoEffect* pEffects;    // effets météo provoqués par des munitions ( fumigènes, obus eclairants )

        static const PHY_Meteo* pGlobalMeteo_;

    public:
        sCell() : h(0), dh(0), e(0), pMeteo(0), pEffects(0) {}
        int16                       GetAltitude     () const { return h; }
        uint8                       GetEnvHeight    () const { return dh; }
        envBits                     GetEnv          () const { return e; }
        const PHY_Precipitation&    GetPrecipitation() const;
        const PHY_Lighting&         GetLighting     () const;
        const PHY_Meteo::sWindData& GetWind         () const;

        bool operator == ( const sCell& rhs ) const
        {
            return h == rhs.h && dh == rhs.dh && e == rhs.e; 
        }
    };

public:
     PHY_RawVisionData( PHY_Meteo& globalMeteo, MIL_Config& config );
    ~PHY_RawVisionData();

    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{   
    const sCell& operator () ( const MT_Vector2D& ) const;
    const sCell& operator () ( double, double )     const;

    double   GetCellSize() const;

    MT_Float GetAltitude( const MT_Vector2D& pos )     const;
    MT_Float GetAltitude( MT_Float rX_, MT_Float rY_ ) const;
    
    envBits  GetVisionObject( const MT_Vector2D& pos )     const;
    envBits  GetVisionObject( MT_Float rX_, MT_Float rY_ ) const;
    
    const PHY_Meteo::sWindData& GetWind( const MT_Vector2D& vPos ) const;

    MT_Float GetMinAltitude() const;
    MT_Float GetMaxAltitude() const;
    void     CalcMinMaxAltitude();

    static uint           ConvertEnvironementToObjectIdx( E_VisionObject obj );
    static E_VisionObject ConvertObjectIdxToEnvironnement( uint );

    void GetVisionObjectsInSurface( const TER_Localisation& localisation, uint& rEmptySurface, uint& rForestSurface, uint& rUrbanSurface ) const;

    void RegisterMeteoPatch   ( const MT_Vector2D&, const MT_Vector2D&, PHY_Meteo* );

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
    uint            GetCol( double ) const;
    uint            GetRow( double ) const;
    const sCell&    operator () ( uint col, uint row ) const;
		  sCell&    operator () ( double, double );



    MT_Float rCellSize_; // taille (en metre) du côté de la cellule
    
    uint     nNbrCol_;
    uint     nNbrRow_;

    sCell**  ppCells_;

    MT_Float rMinAltitude_;
    MT_Float rMaxAltitude_;

    static sCell emptyCell_;
};

#include "PHY_RawVisionData.inl"

#endif // __PHY_RawVisionData_h_
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
#include "meteo/PHY_Meteo.h"
#include "meteo/PHY_RawVisionData_ABC.h"

class MIL_Config;
class PHY_AmmoEffect;
class MT_Ellipse;
class PHY_IndirectFireDotationClass;

//*****************************************************************************
// Created: JVT 02-11-05
// Last modified: JVT 04-03-24
//*****************************************************************************
class PHY_RawVisionData : public weather::PHY_RawVisionData_ABC
                        , private boost::noncopyable
{

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

    typedef unsigned char envBits;  // champ de bit

    // cellule de la matrice de vision
    struct sCell
    {
    private:
        friend class PHY_RawVisionData;

        unsigned short  h     : 16;  // hauteur du sol
        unsigned char   dh    : 8;   // hauteur de la planim�trie
        envBits         e     : 8;   // champ de bit repr�sentant l'environnement visuel statique
        weather::PHY_Meteo*      pMeteo;      // m�t�o locale
        PHY_AmmoEffect* pEffects;    // effets m�t�o provoqu�s par des munitions ( fumig�nes, obus eclairants )
        
        static const weather::PHY_Meteo* pGlobalMeteo_;

    public:
        sCell() : h(0), dh(0), e(0), pMeteo(0), pEffects(0) {}
        short                       GetAltitude     () const { return h; }
        unsigned char               GetEnvHeight    () const { return dh; }
        envBits                     GetEnv          () const { return e; }
        const weather::PHY_Precipitation&    GetPrecipitation() const;
        const weather::PHY_Lighting&         GetLighting     () const;
        const weather::PHY_Meteo::sWindData& GetWind         () const;

        bool operator == ( const sCell& rhs ) const
        {
            return h == rhs.h && dh == rhs.dh && e == rhs.e; 
        }
    };

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RawVisionData( weather::PHY_Meteo& globalMeteo, MIL_Config& config );
    virtual ~PHY_RawVisionData();
    //@}

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
    
    const weather::PHY_Meteo::sWindData& GetWind( const MT_Vector2D& vPos ) const;

    MT_Float GetMinAltitude() const;
    MT_Float GetMaxAltitude() const;
    void     CalcMinMaxAltitude();

    static unsigned int           ConvertEnvironementToObjectIdx( E_VisionObject obj );
    static E_VisionObject ConvertObjectIdxToEnvironnement( unsigned int );

    template< typename T > void GetVisionObjectsInSurface( const T& localisation, unsigned int& rEmptySurface, unsigned int& rForestSurface, unsigned int& rUrbanSurface ) const;

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

    // Convertisseurs de coordonn�es SIM en coordonn�es du tableau
    unsigned int            GetCol( double ) const;
    unsigned int            GetRow( double ) const;
    const sCell&    operator () ( unsigned int col, unsigned int row ) const;
          sCell&    operator () ( double, double );



    MT_Float rCellSize_; // taille (en metre) du c�t� de la cellule
    
    unsigned int     nNbrCol_;
    unsigned int     nNbrRow_;

    sCell**  ppCells_;

    MT_Float rMinAltitude_;
    MT_Float rMaxAltitude_;

    static sCell emptyCell_;
};

#include "PHY_RawVisionData.inl"

#endif // __PHY_RawVisionData_h_

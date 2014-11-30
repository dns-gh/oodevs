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

#include <memory>
#include <set>
#include <unordered_map>

typedef unsigned char envBits;
struct ElevationCell;
class ElevationGrid;
class MT_Ellipse;
class MT_Vector2D;
class PHY_IndirectFireDotationClass;
class PHY_LocalMeteo;
class TER_Localisation;
class TER_Localisation_ABC;

namespace tools
{
    class Path;
}

namespace weather
{
    class Meteo;
    class PHY_Precipitation;
    class PHY_Lighting;
    struct WindData;
}

//*****************************************************************************
// Created: JVT 02-11-05
// Last modified: JVT 04-03-24
//*****************************************************************************
class PHY_RawVisionData : private boost::noncopyable
{
public:
    // environnement visuel statique
    enum E_VisionObject
    {
        eVisionEmpty      = 0x0,
        eVisionForest     = 0x1,
        eVisionUrban      = 0x2,
        eVisionGround     = 0x4,
        eNbrVisionObject = 4
    };
    static std::map< std::string, E_VisionObject > environmentAssociation_;

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RawVisionData( const weather::Meteo& globalMeteo,
                     const tools::Path& detection );
    virtual ~PHY_RawVisionData();
    //@}

    // @name Operations
    //@{
    const ElevationCell& operator () ( const MT_Vector2D& ) const;
    const ElevationCell& operator () ( double, double ) const;

    double GetCellSize() const;

    const weather::Meteo& GetWeather( const ElevationCell& cell ) const;
    const weather::Meteo& GetWeather( const MT_Vector2D& pos ) const;
    bool IsWeatherPatched( const boost::shared_ptr< const PHY_LocalMeteo >& weather ) const;
    const weather::PHY_Precipitation& GetPrecipitation( const MT_Vector2D& ) const;
    const weather::PHY_Precipitation& GetPrecipitation( const ElevationCell& ) const;
    const weather::PHY_Lighting& GetLighting( const ElevationCell& ) const;

    double GetAltitude( const MT_Vector2D& pos, bool applyOnCell = false ) const;
    double GetAltitude( double rX_, double rY_, bool applyOnCell = false ) const;

    envBits GetVisionObject( const MT_Vector2D& pos ) const;
    envBits GetVisionObject( double rX_, double rY_ ) const;

    const weather::WindData& GetWind( const MT_Vector2D& vPos ) const;

    void ModifyAltitude( const TER_Localisation& localisation, short heightOffset, unsigned int objectId );
    double GetMinAltitude() const;
    double GetMaxAltitude() const;
    void CalcMinMaxAltitude();

    static unsigned int ConvertEnvironmentToObjectIdx( E_VisionObject obj );
    static E_VisionObject ConvertObjectIdxToEnvironnement( unsigned int );

    void GetVisionObjectsInSurface( const TER_Localisation_ABC& localisation, unsigned int& rEmptySurface, unsigned int& rForestSurface, unsigned int& rUrbanSurface ) const;

    void RegisterMeteoPatch( const boost::shared_ptr< const PHY_LocalMeteo >& pMeteo );
    void UnregisterMeteoPatch( const boost::shared_ptr< const PHY_LocalMeteo >& pMeteo );

    void RegisterWeatherEffect  ( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& weaponCategory );
    void UnregisterWeatherEffect( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& weaponCategory );

    //@}

private:
    friend class PHY_RawVisionDataIterator;

    // Convertisseurs de coordonn�es SIM en coordonn�es du tableau
    unsigned int GetCol( double ) const;
    unsigned int GetRow( double ) const;
    const ElevationCell& operator () ( unsigned int col, unsigned int row ) const;
    ElevationCell& operator () ( double, double );

    double rCellSize_; // taille (en metre) du c�t� de la cellule

    unsigned int nNbrCol_;
    unsigned int nNbrRow_;

    double rMinAltitude_;
    double rMaxAltitude_;
    std::unique_ptr< ElevationGrid > pElevationGrid_;

    const weather::Meteo& globalMeteo_;
    // Patched weather instances. It also ensures that weather pointers
    // in the elevation map cells remain valid as long as they remain
    // patched.
    std::set< boost::shared_ptr< const PHY_LocalMeteo > > meteos_;
    std::unordered_map< uint32_t, const PHY_LocalMeteo* > meteoIds_;
};

#endif // __PHY_RawVisionData_h_

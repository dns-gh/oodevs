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

typedef unsigned char envBits;
struct ElevationCell;
class ElevationGrid;
class MT_Ellipse;
class MT_Vector2D;
class PHY_IndirectFireDotationClass;
class PHY_MeteoDataManager;
class TER_Localisation;
class TER_Localisation_ABC;

namespace geometry
{
    template< typename T > class Point2;
    typedef Point2< float > Point2f;
    typedef Point2< double > Point2d;
}

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

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RawVisionData( const weather::Meteo& globalMeteo,
                     const tools::Path& detection, PHY_MeteoDataManager* manager );
    virtual ~PHY_RawVisionData();
    //@}

    // @name Operations
    //@{
    const ElevationCell& operator () ( const MT_Vector2D& ) const;
    const ElevationCell& operator () ( double, double ) const;

    double GetCellSize() const;

    const weather::Meteo& GetWeather( const MT_Vector2D& pos ) const;
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

    void RegisterMeteoPatch  ( const geometry::Point2d&, const geometry::Point2d&,
            const boost::shared_ptr< const weather::Meteo >& pMeteo );
    void UnregisterMeteoPatch( const geometry::Point2d&, const geometry::Point2d&,
            const boost::shared_ptr< const weather::Meteo >& pMeteo );

    void RegisterWeatherEffect  ( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& weaponCategory );
    void UnregisterWeatherEffect( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& weaponCategory );

    bool Read( const tools::Path& file );
    //@}

private:
    friend class PHY_RawVisionDataIterator;

    // Convertisseurs de coordonnées SIM en coordonnées du tableau
    unsigned int GetCol( double ) const;
    unsigned int GetRow( double ) const;
    const ElevationCell& operator () ( unsigned int col, unsigned int row ) const;
    ElevationCell& operator () ( double, double );

    double rCellSize_; // taille (en metre) du côté de la cellule

    unsigned int nNbrCol_;
    unsigned int nNbrRow_;

    double rMinAltitude_;
    double rMaxAltitude_;
    std::auto_ptr< ElevationGrid > pElevationGrid_;

    PHY_MeteoDataManager* meteoManager_;
    const weather::Meteo& globalMeteo_;
};

#endif // __PHY_RawVisionData_h_

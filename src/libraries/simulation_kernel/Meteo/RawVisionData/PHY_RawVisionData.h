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

#include "ElevationGrid.h"
#include "meteo/PHY_RawVisionData_ABC.h"
#include "simulation_terrain/TER_Localisation.h"

class PHY_AmmoEffect;
class MT_Ellipse;
class PHY_IndirectFireDotationClass;
class PHY_MeteoDataManager;
class ElevationGrid;

namespace tools
{
    class Path;
}

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

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RawVisionData( weather::Meteo& globalMeteo, const tools::Path& detection, PHY_MeteoDataManager* manager );
    virtual ~PHY_RawVisionData();
    //@}

    // @name Operations
    //@{
    const ElevationGrid::sCell& operator () ( const MT_Vector2D& ) const;
    const ElevationGrid::sCell& operator () ( double, double ) const;

    double GetCellSize() const;

    const weather::PHY_Precipitation& GetPrecipitation( const MT_Vector2D& ) const;

    double GetAltitude( const MT_Vector2D& pos, bool applyOnCell = false ) const;
    double GetAltitude( double rX_, double rY_, bool applyOnCell = false ) const;

    ElevationGrid::envBits GetVisionObject( const MT_Vector2D& pos ) const;
    ElevationGrid::envBits GetVisionObject( double rX_, double rY_ ) const;

    const weather::Meteo::sWindData& GetWind( const MT_Vector2D& vPos ) const;

    void ModifyAltitude( const TER_Localisation& localisation, short heightOffset, unsigned int objectId );
    double GetMinAltitude() const;
    double GetMaxAltitude() const;
    void CalcMinMaxAltitude();

    static unsigned int ConvertEnvironmentToObjectIdx( E_VisionObject obj );
    static E_VisionObject ConvertObjectIdxToEnvironnement( unsigned int );

    void GetVisionObjectsInSurface( const TER_Localisation_ABC& localisation, unsigned int& rEmptySurface, unsigned int& rForestSurface, unsigned int& rUrbanSurface ) const;

    void RegisterMeteoPatch  ( const geometry::Point2d&, const geometry::Point2d&, boost::shared_ptr< weather::Meteo > pMeteo );
    void UnregisterMeteoPatch( const geometry::Point2d&, const geometry::Point2d&, boost::shared_ptr< weather::Meteo > pMeteo );

    void RegisterWeatherEffect  ( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& weaponCategory );
    void UnregisterWeatherEffect( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& weaponCategory );

    bool Read( const tools::Path& file );
    //@}

private:
    friend class PHY_RawVisionDataIterator;

    // Convertisseurs de coordonnées SIM en coordonnées du tableau
    unsigned int GetCol( double ) const;
    unsigned int GetRow( double ) const;
    const ElevationGrid::sCell& operator () ( unsigned int col, unsigned int row ) const;
    ElevationGrid::sCell& operator () ( double, double );

    double rCellSize_; // taille (en metre) du côté de la cellule

    unsigned int nNbrCol_;
    unsigned int nNbrRow_;

    double rMinAltitude_;
    double rMaxAltitude_;
    std::auto_ptr< ElevationGrid > pElevationGrid_;

    PHY_MeteoDataManager* meteoManager_;

};

#endif // __PHY_RawVisionData_h_

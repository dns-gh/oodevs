// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __kernel_CoordinateConverter_h_
#define __kernel_CoordinateConverter_h_

#include "CoordinateConverter_ABC.h"
#include <geocoord/PlanarCartesian.h>
#include <geocoord/MGRS.h>
#include <geocoord/Geodetic.h>
#include <geocoord/UTM.h>
#include <boost/noncopyable.hpp>

namespace tools
{
    class ExerciseConfig;
}

namespace kernel
{

// =============================================================================
// Created: APE 2004-07-19
// $$$$ AGE 2006-04-28: warning : not thread safe
// =============================================================================
class CoordinateConverter : public CoordinateConverter_ABC
                          , private boost::noncopyable
{
public:
    //! @name Constructor / Destructor
    //@{
             CoordinateConverter();
    explicit CoordinateConverter( E_CoordinateSystem coordSystems );
    explicit CoordinateConverter( const tools::ExerciseConfig& config );
    virtual ~CoordinateConverter();
    //@}

    //! @name Operations
    //@{
    virtual void Load( const tools::ExerciseConfig& config );

    virtual E_CoordinateSystem GetDefaultCoordinateSystem() const;
    virtual void SetDefaultCoordinateSystem( E_CoordinateSystem );

    virtual std::string GetStringPosition( const geometry::Point2f& position ) const;
    virtual std::string GetStringPosition( const geometry::Point2f& position, E_CoordinateSystem projection ) const;

    virtual bool IsInBoundaries( const geometry::Point2f& point ) const;
    virtual const geometry::Rectangle2f& GetExtent() const;
    virtual std::string       ConvertToMgrs     ( const geometry::Point2f& pos ) const;
    virtual geometry::Point2d ConvertToGeo      ( const geometry::Point2f& pos ) const;
    virtual geometry::Point2d ConvertToGeo      ( const geometry::Point2d& pos ) const;
    virtual geometry::Point2f ConvertFromGeo    ( const geometry::Point2d& pos ) const;
    virtual geometry::Point2f ConvertToXY       ( const std::string& mgrs ) const;
    virtual std::string       ConvertToGeoDms   ( const geometry::Point2f& pos ) const;
    virtual geometry::Point2f ConvertFromGeoDms ( const std::string& longitude, const std::string& latitude ) const;
    virtual std::string       ConvertToUtm      ( const geometry::Point2f& p, const std::string& code ) const;
    virtual geometry::Point2f ConvertFromUtm    ( const std::string& pos, const std::string& code ) const;
    //@}

private:
    //! @name Initialization
    //@{
    void ReadTerrain( const std::string& terrain );
    void ReadWorld( const std::string& strArchive );
    //@}

     //! @name Helpers
    //@{
    void SetGeodeticCoordinates( const geometry::Point2f& pos ) const;
    //@}

private:
    //! @name Member data
    //@{
    geometry::Rectangle2f extent_;
    geometry::Vector2f translation_;

    geocoord::PlanarCartesian::Parameters parameters_;
    mutable geocoord::PlanarCartesian planar_;
    mutable geocoord::MGRS mgrs_;
    mutable geocoord::Geodetic geodetic_;
    mutable geocoord::UTM utm_;
    E_CoordinateSystem defaultCoordSystem_;
    //@}
};

}

#endif // __kernel_CoordinateConverter_h_
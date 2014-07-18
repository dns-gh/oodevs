// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __kernel_CoordinateConverter_ABC_h_
#define __kernel_CoordinateConverter_ABC_h_

#include "ENT/ENT_Enums.h"
#include <geometry/Types.h>
#include <boost/noncopyable.hpp>

enum E_CoordinateSystem;

namespace tools
{
    class ExerciseConfig;
}

namespace kernel
{

// =============================================================================
/** @class  CoordinateConverter_ABC
    @brief  CoordinateConverter_ABC
*/
// Created: AGE 2006-05-17
// =============================================================================
class CoordinateConverter_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             CoordinateConverter_ABC() {}
    virtual ~CoordinateConverter_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Load( const tools::ExerciseConfig& config ) = 0;

    virtual const geometry::Rectangle2f& GetExtent() const = 0;
    virtual bool IsInBoundaries( const geometry::Point2f& point ) const = 0;
    virtual std::string       ConvertToMgrs         ( const geometry::Point2f& pos ) const = 0;
    virtual std::string       ConvertToMgrs         ( const geometry::Point2d& pos ) const = 0;
    virtual geometry::Point2d ConvertToGeo          ( const geometry::Point2f& pos ) const = 0;
    virtual geometry::Point2d ConvertToGeo          ( const std::string& mgrs ) const = 0;
    virtual geometry::Point2f ConvertFromGeo        ( const geometry::Point2d& pos ) const = 0;
    virtual geometry::Point2f ConvertToXY           ( const std::string& mgrs ) const = 0;
    virtual std::string       ConvertToGeoDms       ( const geometry::Point2f& pos ) const = 0;
    virtual geometry::Point2f ConvertFromGeoDms     ( const std::string& longitude, const std::string& latitude ) const = 0;
    virtual std::string       ConvertToUtm          ( const geometry::Point2f& p, E_CoordinateSystem projection ) const = 0;
    virtual geometry::Point2f ConvertFromUtm        ( const std::string& pos, E_CoordinateSystem projection ) const = 0;

    virtual std::string GetStringPosition( const geometry::Point2f& position ) const = 0;
    virtual std::string GetStringPosition( const geometry::Point2f& position, E_CoordinateSystem projection ) const = 0;

    virtual E_CoordinateSystem GetDefaultCoordinateSystem() const = 0;
    virtual void SetDefaultCoordinateSystem( E_CoordinateSystem ) = 0;

    template< typename T >
    geometry::Point2f ConvertToXY( const T& latlong ) const
    {
        return ConvertFromGeo( geometry::Point2d( latlong.longitude() , latlong.latitude() ) );
    }

    template< typename T >
    void ConvertToGeo( const geometry::Point2f& pos, T& latlong ) const
    {
        const geometry::Point2d geo = ConvertToGeo( pos );
        latlong.set_longitude( geo.X() );
        latlong.set_latitude ( geo.Y() );
    }
    //@}
};

}

#endif // __kernel_CoordinateConverter_ABC_h_

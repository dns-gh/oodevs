// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Coordinates.h"
#include <geocoord/MGRS.h>
#include <geocoord/geodetic.h>
#include <geocoord/geocentric.h>
#include <geometry/types.h>
#include <cmath>

using namespace rpr;

namespace
{
    const double rPiOver180 = std::acos( -1. ) / 180.;
}

// -----------------------------------------------------------------------------
// Name: WorldLocation::WorldLocation
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
WorldLocation::WorldLocation()
    : x_( 0 )
    , y_( 0 )
    , z_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WorldLocation::WorldLocation
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
WorldLocation::WorldLocation( const std::string& mgrs, float altitude )
{
    const geocoord::MGRS base( mgrs );
    geocoord::Geodetic geodetic( base );
    geodetic.SetHeight( altitude );
    const geocoord::Geocentric centric( geodetic );
    x_ = centric.GetX();
    y_ = centric.GetY();
    z_ = centric.GetZ();
}

// -----------------------------------------------------------------------------
// Name: WorldLocation::WorldLocation
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
WorldLocation::WorldLocation( double latitude, double longitude, float altitude )
{
    const geocoord::Geodetic geodetic( latitude * rPiOver180, longitude * rPiOver180, altitude );
    const geocoord::Geocentric centric( geodetic );
    x_ = centric.GetX();
    y_ = centric.GetY();
    z_ = centric.GetZ();
}

// -----------------------------------------------------------------------------
// Name: WorldLocation::~WorldLocation
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
WorldLocation::~WorldLocation()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: WorldLocation::Latitude
// Created: SLI 2011-09-02
// -----------------------------------------------------------------------------
double WorldLocation::Latitude() const
{
    const geocoord::Geocentric centric( x_, y_, z_ );
    const geocoord::Geodetic geodetic( centric );
    return geodetic.GetLatitude() / rPiOver180;
}

// -----------------------------------------------------------------------------
// Name: WorldLocation::Longitude
// Created: SLI 2011-09-02
// -----------------------------------------------------------------------------
double WorldLocation::Longitude() const
{
    const geocoord::Geocentric centric( x_, y_, z_ );
    const geocoord::Geodetic geodetic( centric );
    return geodetic.GetLongitude() / rPiOver180;
}

// -----------------------------------------------------------------------------
// Name: WorldLocation::Altitude
// Created: SLI 2011-09-02
// -----------------------------------------------------------------------------
double WorldLocation::Altitude() const
{
    const geocoord::Geocentric centric( x_, y_, z_ );
    const geocoord::Geodetic geodetic( centric );
    return geodetic.GetHeight();
}

// -----------------------------------------------------------------------------
// Name: VelocityVector::VelocityVector
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
VelocityVector::VelocityVector()
    : xn_( 0 ), yn_( 0 ), zn_( 0 )
    , xv_( 0 ), yv_( 0 ), zv_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: VelocityVector::VelocityVector
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
VelocityVector::VelocityVector( float x, float y, float z )
    : xv_( x )
    , yv_( y )
    , zv_( z )
{
    geometry::Vector3f speed( x, y, z );
    speed.Normalize();
    xn_ = speed.X();
    yn_ = speed.Y();
    zn_ = speed.Z();
}

// -----------------------------------------------------------------------------
// Name: VelocityVector::VelocityVector
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
VelocityVector::VelocityVector( const WorldLocation& location, float rSpeed, float heading )
{
    static const double pi = std::acos( -1. );
    geometry::Vector3d base( location.X(), location.Y(), location.Z() );
    geometry::Vector3d yaxis( base.Y(), -base.X(), 0 );
    if( yaxis.SquareLength() < 1 )
        yaxis = geometry::Vector3d( 0, 1, 0 );
    yaxis.Normalize();
    geometry::Vector3d speed( base );
    speed.Rotate( yaxis, pi / 2 );
    geometry::Vector3d zaxis( base );
    zaxis.Normalize();
    const double angle = - static_cast< double >( heading ) * pi / 180.;
    speed.Rotate( zaxis, angle );
    speed.Normalize();
    xn_ = static_cast< float >( speed.X() );
    yn_ = static_cast< float >( speed.Y() );
    zn_ = static_cast< float >( speed.Z() );
    speed *= rSpeed; // * 3.6f; // km/h => m/s // $$$$ AGE 2008-04-02: marche mieux....
    xv_ = static_cast< float >( speed.X() );
    yv_ = static_cast< float >( speed.Y() );
    zv_ = static_cast< float >( speed.Z() );
}

// -----------------------------------------------------------------------------
// Name: VelocityVector::~VelocityVector
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
VelocityVector::~VelocityVector()
{
    // NOTHING
}

// $$$$ AGE 2008-03-11: zxz
//    if( std::fabs( z.Z() ) < 1 - 1e-6 )
//    {
//        beta = std::acos( z.Z() );
//        const double sinBeta = std::sin( beta );
//        alpha = std::atan2( z.Y() / sinBeta, z.X() / sinBeta );
//        gamma = std::atan2( -y.Z() / sinBeta, x.Z() / sinBeta );
//    }
//    else
//    {
//        beta = 0;
//        gamma = 0;
//        alpha = std::acos( x.X() );
//        if( x.Y() < 0 )
//            alpha = -alpha;
//    }

// $$$$ AGE 2008-03-11: zyx
//    if( std::fabs( x.Z() ) < 1 - 1e-6 )
//    {
//        const double cosBeta = std::cos( beta );
//        alpha = std::atan2( x.X() / cosBeta, -x.Y() / cosBeta );
//        gamma = std::atan2( z.Z() / cosBeta, -y.Z() / cosBeta );
//    }

namespace
{
    // $$$$ AGE 2008-03-11: zxy...
    geometry::Vector3d ComputeEulerAngles( const geometry::Vector3d& x, const geometry::Vector3d& y, const geometry::Vector3d& z )
    {
        double alpha = 0, beta = 0, gamma = 0;
        if( std::fabs( y.Z() ) < 1 - 1e-6 )
        {
            beta = - std::asin( y.Z() );
            const double cosBeta = std::cos( beta );
            alpha = std::atan2( y.Y() / cosBeta, y.X() / cosBeta );
            gamma = std::atan2( z.Z() / cosBeta, x.Z() / cosBeta );
        }
        else
        {
            beta = std::acos( 1. ) / 2;
            alpha = std::atan2( x.X(), y.Y() );
            gamma = 0;
        }
        return geometry::Vector3d( alpha, beta, gamma );
    }
}

// -----------------------------------------------------------------------------
// Name: Orientation::Orientation
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
Orientation::Orientation()
    : psi_  ( 0 )
    , theta_( 0 )
    , phi_  ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Orientation::Orientation
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
Orientation::Orientation( const WorldLocation& location, const VelocityVector& velocity )
{
    geometry::Vector3d up( location.X(), location.Y(), location.Z() );
    up.Normalize();
    geometry::Vector3d forward( velocity.X(), velocity.Y(), velocity.Z() );
    forward.Normalize();
    geometry::Vector3d left = up.CrossProduct( forward );
    const geometry::Vector3d x = -up;
    const geometry::Vector3d y = forward;
    const geometry::Vector3d z = -left;
    geometry::Vector3d euler = ComputeEulerAngles( x, y, z );
    psi_   = static_cast< float >( euler.X() );
    theta_ = static_cast< float >( euler.Y() );
    phi_   = static_cast< float >( euler.Z() );
}

// -----------------------------------------------------------------------------
// Name: Orientation::~Orientation
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
Orientation::~Orientation()
{
    // NOTHING
}

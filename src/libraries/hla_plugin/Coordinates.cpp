// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "Coordinates.h"

#include <geocoord/MGRS.h>
#include <geocoord/geodetic.h>
#include <geocoord/geocentric.h>
#include <geometry/types.h>

#include <cmath>

// -----------------------------------------------------------------------------
// Name: WorldLocation::WorldLocation
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
WorldLocation::WorldLocation( const std::string mgrs, float altitude )
{
    geocoord::MGRS base( mgrs );
    geocoord::Geodetic geodetic( base );
    geodetic.SetHeight( altitude );
    geocoord::Geocentric centric( geodetic );

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
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
VelocityVector::VelocityVector( const WorldLocation& location, float rSpeed, unsigned short heading )
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
    const double angle = - double( heading ) * pi / 180.;
    speed.Rotate( zaxis, angle );

    speed.Normalize();

    xn_ = float( speed.X() );
    yn_ = float( speed.Y() );
    zn_ = float( speed.Z() );

    speed *= rSpeed / 3.6f; // km/h => m/s

    xv_ = float( speed.X() );
    yv_ = float( speed.Y() );
    zv_ = float( speed.Z() );
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

    psi_   = float( euler.X() );
    theta_ = float( euler.Y() );
    phi_   = float( euler.Z() );
}

// -----------------------------------------------------------------------------
// Name: Orientation::~Orientation
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
Orientation::~Orientation()
{
    // NOTHING
}

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
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
VelocityVector::VelocityVector( const WorldLocation& location, float rSpeed, unsigned short heading )
{
    static const double pi = std::acos( -1. );
    geometry::Vector3d base( location.GetX(),  location.GetY(), location.GetZ() );

    geometry::Vector3d yaxis( base.Y(), -base.X(), 0 );
    if( yaxis.SquareLength() < 1 )
        yaxis = geometry::Vector3d( 0, 1, 0 );
    yaxis.Normalize();

    geometry::Vector3d speed( base );
    speed.Rotate( yaxis, pi / 2 );

    geometry::Vector3d zaxis( base );
    zaxis.Normalize();
    const double angle = - double( heading ) / ( 2 * pi );
    speed.Rotate( zaxis, angle );

    speed.Normalize();

    xn_ = float( speed.X() );
    yn_ = float( speed.Y() );
    zn_ = float( speed.Z() );

    speed *= rSpeed;

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

namespace
{
    geometry::Vector3d ComputeEulerAngles( const geometry::Vector3d& x, const geometry::Vector3d& y, const geometry::Vector3d& z )
    {
        double psi, theta, phi;
        if( std::fabs( x.Z() ) < 1 - 1e-6 )
        {
            theta = std::asin( x.Z() );
            const double cosTheta = std::cos( theta );
            phi   = std::atan2( y.Z() / cosTheta, z.Z() / cosTheta );
            psi   = std::atan2( x.Y() / cosTheta, x.X() / cosTheta );
        }
        else
        {
            psi = 0;
            phi = std::atan2( y.X(), z.X() );
            theta = std::acos( -1. ) / 2;
            if( x.Z() < 0 )
                theta = -theta;
        }
        return geometry::Vector3d( psi, theta, phi );
    }
}

// -----------------------------------------------------------------------------
// Name: Orientation::Orientation
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
Orientation::Orientation( const WorldLocation& location, const VelocityVector& velocity )
{
    geometry::Vector3d zAxis( location.GetX(),  location.GetY(), location.GetZ() );
    zAxis.Normalize();

    geometry::Vector3d xAxis( velocity.GetX(),  velocity.GetY(), velocity.GetZ() );
    xAxis.Normalize();

    geometry::Vector3d yAxis = zAxis.CrossProduct( xAxis );

    geometry::Vector3d euler = ComputeEulerAngles( xAxis, yAxis, zAxis );

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
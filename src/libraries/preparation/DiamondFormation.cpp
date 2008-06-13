// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "DiamondFormation.h"

// -----------------------------------------------------------------------------
// Name: DiamondFormation constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
DiamondFormation::DiamondFormation( const geometry::Point2f& center, float spacing )
    : center_( geometry::Vector2f( center.X(), center.Y() ) )
    , spacing_( spacing )
    , radius_( 1 )
    , angle_ ( 0 )
    , onLevel_( 0 )
    , totalOnLevel_( 4 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DiamondFormation destructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
DiamondFormation::~DiamondFormation()
{
    // NOTHING
}

namespace
{
    geometry::Point2f CartesianFromPolar( float radius, float angle )
    {
        static const float pi = std::acos( -1.f );
        const float radianAngle = pi * angle / 180.f;;
        return geometry::Point2f( radius * std::sin( radianAngle ), radius * std::cos( radianAngle ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DiamondFormation::NextPosition
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
geometry::Point2f DiamondFormation::NextPosition()
{
    geometry::Point2f position = CartesianFromPolar( radius_ * spacing_, angle_ ) + center_;
    ++onLevel_;

    // if level is full, start a new one
    if( onLevel_ == totalOnLevel_ )
    {
        radius_ *= 1.414f; // sqrt(2)
        angle_  = 45.f;
        totalOnLevel_ += 4;
        onLevel_ = 0;
    }
    else
        angle_ += 360.f / totalOnLevel_;
    return position;
}

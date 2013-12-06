// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "CircleFormation.h"

namespace
{
    static const float pi = std::acos( -1.f );
}

// -----------------------------------------------------------------------------
// Name: CircleFormation constructor
// Created: JSR 2012-07-02
// -----------------------------------------------------------------------------
CircleFormation::CircleFormation( const geometry::Point2f& center, unsigned int numberOfAgents, float radius )
    : center_( geometry::Vector2f( center.X(), center.Y() ) )
    , radius_( radius )
    , current_( 0 )
    , angle_  ( numberOfAgents > 1 ? 2 * pi / ( numberOfAgents - 1 ) : 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CircleFormation destructor
// Created: JSR 2012-07-02
// -----------------------------------------------------------------------------
CircleFormation::~CircleFormation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CircleFormation::NextPosition
// Created: JSR 2012-07-02
// -----------------------------------------------------------------------------
geometry::Point2f CircleFormation::NextPosition( bool isPc )
{
    if( isPc )
        return center_.ToPoint();
    return geometry::Point2f( radius_ * std::sin( current_++ * angle_ ), radius_ * std::cos( current_ * angle_ ) ) + center_;
}

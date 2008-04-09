// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tic_plugin_pch.h"
#include "DiamondFormation.h"
#include "Movable_ABC.h"

using namespace tic;

// -----------------------------------------------------------------------------
// Name: DiamondFormation constructor
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
DiamondFormation::DiamondFormation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DiamondFormation destructor
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
DiamondFormation::~DiamondFormation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DiamondFormation::Start
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void DiamondFormation::Start( const geometry::Point2f& at, const geometry::Vector2f& towards, unsigned count )
{
    points_.resize( 0 );
    const float baseAngle = std::atan2( towards.Y(), towards.X() );
    unsigned level = 0;
    unsigned platformCount = 0;
    while( count-- )
        AddPlatform( at, baseAngle, level, platformCount );
//    Sort( points_, at, towards );
    Sort( points_, at, geometry::Vector2f( 1, 0 ) );
}

namespace
{
    void NextPlatform( unsigned& level, unsigned& platformCount )
    {
        ++platformCount;
        if( platformCount >= 4 * level )
        {
            platformCount = 0;
            ++level;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DiamondFormation::AddPlatform
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void DiamondFormation::AddPlatform( const geometry::Point2f& center, float baseAngle, unsigned& level, unsigned& platformCount )
{
    const float radius = level ? std::pow( 1.414f, int( level ) ) : 0;
    const float angle  = level ? baseAngle + 2 * std::acos( -1.f ) * float( platformCount ) / float( 4 * level ) : 0;
    const float distanceBetweenPlatforms = 20.f;

    const geometry::Vector2f direction( std::cos( angle ), std::sin( angle ) );
    points_.push_back( center + distanceBetweenPlatforms * radius * direction );

    NextPlatform( level, platformCount );
}

// -----------------------------------------------------------------------------
// Name: DiamondFormation::Apply
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void DiamondFormation::Apply( Movable_ABC& movable )
{
    if( ! points_.empty() )
    {
        movable.Move( points_.back() );
        points_.pop_back();
    }
    else 
        movable.Stop();
}



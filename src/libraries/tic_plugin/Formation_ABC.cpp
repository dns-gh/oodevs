// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tic_plugin_pch.h"
#include "Formation_ABC.h"
#include <boost/bind.hpp>

using namespace plugins::tic;

// -----------------------------------------------------------------------------
// Name: Formation_ABC::Compare
// Created: AGE 2008-05-05
// -----------------------------------------------------------------------------
bool Formation_ABC::Compare( const geometry::Point2f& lhs, const geometry::Point2f& rhs, const geometry::Point2f& center, const geometry::Vector2f& towards )
{
    const geometry::Vector2f left( center, lhs );
    const geometry::Vector2f right( center, rhs );
    const float ldot = towards.DotProduct( left );
    const float rdot = towards.DotProduct( right );
    if( ldot > rdot ) return true;
    if( ldot < rdot ) return false;

    const float lcross = towards.CrossProduct( left );
    const float rcross = towards.CrossProduct( right );

    return lcross < rcross;
}

// -----------------------------------------------------------------------------
// Name: Formation_ABC::Sort
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void Formation_ABC::Sort( std::vector< geometry::Point2f >& points, const geometry::Point2f& center, const geometry::Vector2f& towards )
{
    std::sort( points.begin(), points.end(), boost::bind( &Formation_ABC::Compare, _1, _2, center, towards ) );
}

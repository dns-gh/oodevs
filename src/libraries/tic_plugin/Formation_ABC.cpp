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

using namespace tic;

namespace
{
    struct Sorter
    {
        Sorter( const geometry::Point2f& from, const geometry::Vector2f& towards )
            : from_( from )
            , towards_( towards ) {}
        bool operator()( const geometry::Point2f& lhs, const geometry::Point2f& rhs ) const
        {
            const geometry::Vector2f left( from_, lhs );
            const geometry::Vector2f right( from_, rhs );
            const float ldot = towards_.DotProduct( left );
            const float rdot = towards_.DotProduct( right );
            if( ldot < rdot ) return true;
            if( ldot > rdot ) return false;

            const float lcross = towards_.CrossProduct( left );
            const float rcross = towards_.CrossProduct( right );

            return lcross < rcross;
        }
        geometry::Point2f from_;
        geometry::Vector2f towards_;
    };
}

// -----------------------------------------------------------------------------
// Name: Formation_ABC::Sort
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void Formation_ABC::Sort( std::vector< geometry::Point2f >& points, const geometry::Point2f& center, const geometry::Vector2f& towards )
{
    std::sort( points.begin(), points.end(), Sorter( center, towards ) );
}

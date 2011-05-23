// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_Vector2D.h"
#include "MT_Vector2DTypes.h"
#include "MT_Droite.h"
#include <cassert>

// -----------------------------------------------------------------------------
// Name: MT_Vector2D::MT_ComputeBarycenter
// Created: NLD 2003-08-20
// -----------------------------------------------------------------------------
MT_Vector2D MT_ComputeBarycenter( const T_PointVector& points )
{
    MT_Vector2D vBarycenter;

    if( points.empty() )
        return vBarycenter;
    if( points.size() == 1 )
        return points.front();

    CIT_PointVector itEnd      = points.end();
    int nNbrPoints = static_cast< int >( points.size() );
    if( points.front() == points.back() )
    {
        itEnd --;
        nNbrPoints--;
    }
    assert( nNbrPoints > 0 );

    for( CIT_PointVector itPoint = points.begin(); itPoint != itEnd; ++itPoint )
        vBarycenter += *itPoint;
    vBarycenter /= nNbrPoints;
    return vBarycenter;
}

//*****************************************************************************
// Name   : MT_Vector2D.cpp
// Created: FBD 02-03-01
//*****************************************************************************

#include "MT_Tools_pch.h"
#include "MT_Vector2D.h"
#include "MT_Vector2DTypes.h"
#include "MT_Droite.h"

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
    unsigned int            nNbrPoints = points.size();
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

//*****************************************************************************
// Name   : MT_Vector2D.cpp
// Created: FBD 02-03-01
//*****************************************************************************

#include "MT_Tools_pch.h"
#include "MT_Vector2D.h"

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
    uint            nNbrPoints = points.size();
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

// -----------------------------------------------------------------------------
// Name: MT_IsPointBetweenTwoLines
// Created: NLD 2003-08-21
// -----------------------------------------------------------------------------
bool MT_IsPointBetweenTwoLines( const MT_Droite& leftDroite, const MT_Droite& rightDroite, const MT_Vector2D& vPoint )
{
    MT_Droite::E_Side nLeftSide  = leftDroite .GetSide( vPoint );
    MT_Droite::E_Side nRightSide = rightDroite.GetSide( vPoint );

    return      ( nLeftSide  == MT_Droite::eOnPositiveSide || nLeftSide == MT_Droite::eOnBoundary )
            &&  ( nRightSide == MT_Droite::eOnNegativeSide || nRightSide == MT_Droite::eOnBoundary );
}

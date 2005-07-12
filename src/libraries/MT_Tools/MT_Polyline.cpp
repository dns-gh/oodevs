//*****************************************************************************
//
// $Created: NLD 2003-07-25 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Polyline.cpp $
// $Author: Jvt $
// $Modtime: 16/05/05 13:26 $
// $Revision: 6 $
// $Workfile: MT_Polyline.cpp $
//
//*****************************************************************************

#include "MT_Tools_pch.h"

#include "MT_Polyline.h"

#include "MT_Line.h"

//-----------------------------------------------------------------------------
// Name: MT_Polyline constructor
// Created: NLD 2003-07-25
//-----------------------------------------------------------------------------
MT_Polyline::MT_Polyline()
{

}

//-----------------------------------------------------------------------------
// Name: MT_Polyline constructor
// Created: NLD 2003-07-25
//-----------------------------------------------------------------------------
MT_Polyline::MT_Polyline( const T_PointVector& points )
: points_( points )
{
}


//-----------------------------------------------------------------------------
// Name: MT_Polyline destructor
// Created: NLD 2003-07-25
//-----------------------------------------------------------------------------
MT_Polyline::~MT_Polyline()
{

}

//-----------------------------------------------------------------------------
// Name: MT_Polyline::operator=
// Created: NLD 2003-07-25
//-----------------------------------------------------------------------------
MT_Polyline& MT_Polyline::operator=( const T_PointVector& points )
{
    points_ = points;
    return *this;
}


//=============================================================================
// OPERATIONS
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MT_Polyline::Translate
// Created: JVT 2005-05-16
// -----------------------------------------------------------------------------
void MT_Polyline::Translate( const MT_Vector2D& vTranslation )
{
    for ( IT_PointVector it = points_.begin(); it != points_.end(); ++it )
        *it += vTranslation;
}

//-----------------------------------------------------------------------------
// Name: MT_Polyline::Intersect2DWithCircle
// Created: NLD 2003-07-25
//-----------------------------------------------------------------------------
bool MT_Polyline::Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, MT_Float rRadius ) const
{
    CIT_PointVector itFirstPoint = points_.begin();
    const MT_Vector2D* pPos1 = &*itFirstPoint;
    for( CIT_PointVector itPoint = ++itFirstPoint; itPoint != points_.end(); ++itPoint )
    {
        const MT_Vector2D* pPos2 = &*itPoint;
        MT_Line lineTmp( *pPos1, *pPos2 );

        if( lineTmp.ClosestPointOnLine( vCircleCenter ).Distance( vCircleCenter ) <= rRadius )
            return true;
        pPos1 = pPos2;
    }
    return false;   
}


//-----------------------------------------------------------------------------
// Name: MT_Polyline::IsInside
// Created: NLD 2003-07-25
//-----------------------------------------------------------------------------
bool MT_Polyline::IsInside( const MT_Vector2D& vPos, MT_Float rPrecision ) const
{
    CIT_PointVector itFirstPoint = points_.begin();
    const MT_Vector2D* pPos1 = &*itFirstPoint;
    for( CIT_PointVector itPoint = ++itFirstPoint; itPoint != points_.end(); ++itPoint )
    {
        const MT_Vector2D* pPos2 = &*itPoint;
        MT_Line lineTmp( *pPos1, *pPos2 );

        if( lineTmp.IsInside( vPos, rPrecision ) )
            return true;
        pPos1 = pPos2;
    }
    return false;
}


//-----------------------------------------------------------------------------
// Name: MT_Polyline::Intersect2D
// Created: NLD 2003-07-25
//-----------------------------------------------------------------------------
bool MT_Polyline::Intersect2D( const MT_Line& orientedLine, T_PointSet& collisions, MT_Float rPrecision ) const
{
    return orientedLine.Intersect2D( points_, collisions, rPrecision );
}


//-----------------------------------------------------------------------------
// Name: MT_Polyline::Intersect2D
// Created: NLD 2003-07-25
//-----------------------------------------------------------------------------
bool MT_Polyline::Intersect2D( const MT_Line& line, MT_Float rPrecision ) const
{
    return line.Intersect2D( points_, rPrecision );
}

// -----------------------------------------------------------------------------
// Name: MT_Polyline::Intersect2D
// Created: NLD 2003-10-20
// -----------------------------------------------------------------------------
bool MT_Polyline::Intersect2D( const MT_Droite& droite, T_PointSet& collisions ) const
{
    CIT_PointVector itFirstPoint = points_.begin();
    const MT_Vector2D* pPos1 = &*itFirstPoint;
    for( CIT_PointVector itPoint = ++itFirstPoint; itPoint != points_.end(); ++itPoint )
    {
        const MT_Vector2D* pPos2 = &*itPoint;
        MT_Line lineTmp( *pPos1, *pPos2 );

        MT_Vector2D vIntersect;
        if( droite.Intersect2D( lineTmp, vIntersect ) == eDoIntersect )
            collisions.insert( vIntersect );
        
        pPos1 = pPos2;
    }
    return false;
}


// -----------------------------------------------------------------------------
// Name: MT_Polyline::Magnitude
// Created: NLD 2003-09-01
// -----------------------------------------------------------------------------
MT_Float MT_Polyline::Magnitude() const
{
    CIT_PointVector itFirstPoint = points_.begin();
    const MT_Vector2D* pPos1 = &*itFirstPoint;
    MT_Float rMagnitude = 0.;
    for( CIT_PointVector itPoint = ++itFirstPoint; itPoint != points_.end(); ++itPoint )
    {
        const MT_Vector2D* pPos2 = &*itPoint;

        rMagnitude += (*pPos2 - *pPos1).Magnitude();
        pPos1 = pPos2;
    }
    return rMagnitude;    
}

// -----------------------------------------------------------------------------
// Name: MT_Polyline::GetPointAt
// Created: NLD 2003-09-01
// -----------------------------------------------------------------------------
MT_Vector2D MT_Polyline::GetPointAt( MT_Float rDist ) const
{
    assert( points_.size() > 1 );

    MT_Vector2D vCur  = *points_.begin();

    CIT_PointVector itNext = points_.begin();
    ++itNext;

    MT_Vector2D vNext = *itNext;
    MT_Vector2D vDir = vNext - vCur;

    MT_Float rDirLength = vDir.Magnitude();
    if( rDirLength )
        vDir /= rDirLength;

    while( 1 )
    {
        if( rDist < rDirLength )
        {
            vCur = vCur + ( vDir * rDist );
            return vCur;
        }
        else
        {
            rDist -= rDirLength;
            vCur = vNext;
            ++itNext;
            if( itNext == points_.end() )
                return vCur;
            vNext = *itNext;
            vDir = vNext - vCur;
            rDirLength = vDir.Magnitude();
            if( rDirLength )
                vDir /= rDirLength;
        }
    }     
}


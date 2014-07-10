// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_Polyline.h"
#include "MT_Line.h"
#include "MT_Droite.h"
#include "MT_Circle.h"
#include <iterator>
#include <cassert>

//-----------------------------------------------------------------------------
// Name: MT_Polyline constructor
// Created: NLD 2003-07-25
//-----------------------------------------------------------------------------
MT_Polyline::MT_Polyline()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_Polyline constructor
// Created: NLD 2003-07-25
//-----------------------------------------------------------------------------
MT_Polyline::MT_Polyline( const T_PointVector& points )
    : points_( points )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_Polyline destructor
// Created: NLD 2003-07-25
//-----------------------------------------------------------------------------
MT_Polyline::~MT_Polyline()
{
    // NOTHING
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
bool MT_Polyline::Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius ) const
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

// -----------------------------------------------------------------------------
// Name: MT_Polyline::Intersect2DWithCircle
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool MT_Polyline::Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius, T_PointVector& shape ) const
{
    shape.clear();

    CIT_PointVector itCur  = points_.begin();
    CIT_PointVector itPrev = points_.begin();
    ++ itCur;

    MT_Circle circle( vCircleCenter, rRadius );

    MT_Vector2D     vEntryPoint;
    CIT_PointVector itAfterEntryPoint = points_.end();
    MT_Vector2D     vExitPoint;
    CIT_PointVector itAfterExitPoint = points_.end();

    while( itCur != points_.end()  )
    {
        T_PointVector res = circle.Intersection( *itPrev, *itCur );

        if( !res.empty() )
        {
            //$$$$
            if( res.size() == 2 )
            {
                if( (*itPrev).SquareDistance( res.back () ) < (*itPrev).SquareDistance( res.front() ) )
                    std::swap( res.back(), res.front() );
            }

            if( itAfterEntryPoint == points_.end() )
            {
                itAfterEntryPoint = itCur;
                vEntryPoint = res.front();
            }

            itAfterExitPoint = itCur;
            vExitPoint       = res.back();
        }
        itPrev = itCur;
        ++ itCur;
    }


    if( itAfterEntryPoint == points_.end() )
    {
        assert( itAfterExitPoint == points_.end() );
        return false;
    }

    if( itAfterExitPoint == points_.end() )
    {
        shape.push_back( vEntryPoint );
        std::copy( itAfterEntryPoint, points_.end(), std::back_inserter( shape ) );
        return true;
    }

    shape.push_back( vEntryPoint );
    std::copy( itAfterEntryPoint, itAfterExitPoint, std::back_inserter( shape ) );
    shape.push_back( vExitPoint );
    return true;
}

//-----------------------------------------------------------------------------
// Name: MT_Polyline::IsInside
// Created: NLD 2003-07-25
//-----------------------------------------------------------------------------
bool MT_Polyline::IsInside( const MT_Vector2D& vPos, double rPrecision ) const
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
bool MT_Polyline::Intersect2D( const MT_Line& orientedLine, T_PointSet& collisions, double rPrecision ) const
{
    return orientedLine.Intersect2D( points_, collisions, rPrecision );
}


//-----------------------------------------------------------------------------
// Name: MT_Polyline::Intersect2D
// Created: NLD 2003-07-25
//-----------------------------------------------------------------------------
bool MT_Polyline::Intersect2D( const MT_Line& line, double rPrecision ) const
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
double MT_Polyline::Magnitude() const
{
    if( points_.empty() )
        return 0.;

    CIT_PointVector itFirstPoint = points_.begin();
    const MT_Vector2D* pPos1 = &*itFirstPoint;
    double rMagnitude = 0.;
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
MT_Vector2D MT_Polyline::GetPointAt( double rDist ) const
{
    assert( points_.size() > 1 );

    MT_Vector2D vCur  = *points_.begin();

    CIT_PointVector itNext = points_.begin();
    ++itNext;

    MT_Vector2D vNext = *itNext;
    MT_Vector2D vDir = vNext - vCur;

    double rDirLength = vDir.Magnitude();
    if( rDirLength )
        vDir /= rDirLength;

    while( rDist >= rDirLength )
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
    vCur = vCur + ( vDir * rDist );
    return vCur;
}

// -----------------------------------------------------------------------------
// Name: MT_Polyline::GetPoints
// Created: NLD 2003-07-31
// -----------------------------------------------------------------------------
const T_PointVector& MT_Polyline::GetPoints() const
{
    return points_;
}

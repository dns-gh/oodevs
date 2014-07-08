//*****************************************************************************
//
// $Created: JDY 03-05-19 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_Polygon.cpp $
// $Author: Age $
// $Modtime: 16/06/05 9:57 $
// $Revision: 4 $
// $Workfile: TER_Polygon.cpp $
//
//*****************************************************************************
#include "simulation_terrain_pch.h"
#include "TER_Polygon.h"
#include "MT_Tools/MT_Polyline.h"
#include "MT_Tools/MT_Droite.h"
#include <boost/make_shared.hpp>

struct TER_Polygon::PolygonData
{
    T_PointVector    borderVector_;
    MT_Rect          boundingBox_;
};

namespace
{

const MT_Rect emptyRect(
        std::numeric_limits<double>::max(), std::numeric_limits<double>::max(),
        std::numeric_limits<double>::min(), std::numeric_limits<double>::min() );

MT_Rect ComputeBoundingBox( const T_PointVector& points )
{
    // bounding box
    MT_Vector2D vDownLeft( std::numeric_limits<double>::max(), std::numeric_limits<double>::max() );
    MT_Vector2D vUpRight ( std::numeric_limits<double>::min(), std::numeric_limits<double>::min() );

    for( auto it = points.begin(); it != points.end(); ++it )
    {
        const MT_Vector2D& vPos = *it;
        if( vPos.rX_ < vDownLeft.rX_ )
            vDownLeft.rX_ = vPos.rX_;
        if( vPos.rY_ < vDownLeft.rY_ )
            vDownLeft.rY_ = vPos.rY_;
        if( vPos.rX_ > vUpRight.rX_ )
            vUpRight.rX_ = vPos.rX_;
        if( vPos.rY_ > vUpRight.rY_ )
            vUpRight.rY_ = vPos.rY_;
    }
    return MT_Rect( vDownLeft.rX_, vDownLeft.rY_, vUpRight.rX_, vUpRight.rY_ );
}

// -----------------------------------------------------------------------------
// Name: InitializeHull
// Created: AGE 2005-06-14
// -----------------------------------------------------------------------------
void InitializeHull( const T_PointVector& polygon, T_PointVector& hull )
{
    MT_Vector2D min, max;
    for( auto it = polygon.begin(); it != polygon.end(); ++it )
    {
        const MT_Vector2D& p = *it;
        if( min.IsZero() || min.rX_ > p.rX_ )
            min = p;
        if( max.IsZero() || max.rX_ < p.rX_ )
            max = p;
    }
    hull.push_back( min );
    hull.push_back( max );
}

double CrossProductTmp( const MT_Vector2D& v1, const MT_Vector2D& v2 )
{
    return v1.rX_ * v2.rY_ - v1.rY_ * v2.rX_;
}

// -----------------------------------------------------------------------------
// Name: FindOuterPoint
// Created: AGE 2005-03-29
// -----------------------------------------------------------------------------
bool FindOuterPoint( const T_PointVector& polygon, const MT_Vector2D& from, const MT_Vector2D& direction, MT_Vector2D& worst )
{
    bool bFound = false;
    double rMaxProjection = 0;
    for( auto it = polygon.begin(); it != polygon.end(); ++it )
    {
        const MT_Vector2D v = from - *it;
        const double rProjection = CrossProductTmp( direction, v );
        if( rProjection < -0.001 ) // epsilon
        {
            bFound = true;
            if( rMaxProjection > rProjection )
            {
                rMaxProjection = rProjection;
                worst = *it;
            }
        }
    }
    return bFound;
}

// -----------------------------------------------------------------------------
// Name: Convexify
// Created: AGE 2005-06-14
// -----------------------------------------------------------------------------
void Convexify( T_PointVector& points )
{
    if( points.size() < 3 )
        return;

    T_PointVector convexHull;
    InitializeHull( points, convexHull );

    unsigned int nPoint = 0;
    while( nPoint != convexHull.size() )
    {
        unsigned int nFollowingPoint = nPoint + 1;
        if( nFollowingPoint == convexHull.size() )
            nFollowingPoint = 0;

        MT_Vector2D direction = convexHull[ nFollowingPoint ] - convexHull[ nPoint ];
        direction.Normalize();
        MT_Vector2D worst;
        if( FindOuterPoint( points, convexHull[ nPoint ], direction, worst ) )
        {
            convexHull.insert( convexHull.begin() + nFollowingPoint, worst );
            nPoint = 0;
        }
        else
            ++nPoint;
    }
    // Ensure the polygon is closed
    if( convexHull.back() != convexHull.front() )
        convexHull.push_back( convexHull.front() );

    points.swap( convexHull );
}

}  // namespace

//-----------------------------------------------------------------------------
// Name: TER_Polygon constructor
// Created: JDY 03-05-19
//-----------------------------------------------------------------------------
TER_Polygon::TER_Polygon()
{
    Reset();
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon constructor
// Created: JDY 03-05-26
//-----------------------------------------------------------------------------
TER_Polygon::TER_Polygon( const T_PointVector& points, bool bConvexHull )
{
    if( points.empty() )
        return;

    auto data = boost::make_shared< PolygonData >();
    data->borderVector_ = points;
    if( bConvexHull )
        Convexify( data->borderVector_ );
    data->boundingBox_ = ComputeBoundingBox( data->borderVector_ );
    if( data->borderVector_.empty() )
        data.reset();

    pData_ = data;
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon constructor
// Created: JDY 03-05-26
//-----------------------------------------------------------------------------
TER_Polygon::TER_Polygon( const TER_Polygon& poly )
{
    *this = poly;
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon destructor
// Created: JDY 03-05-19
//-----------------------------------------------------------------------------
TER_Polygon::~TER_Polygon()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon operator =
// Created: JDY 03-05-26
//-----------------------------------------------------------------------------
TER_Polygon& TER_Polygon::operator=( const TER_Polygon& rhs )
{
    // Fine because pData_ is immutable
    pData_ = rhs.pData_;
    return *this;
}

//=============================================================================
// GEO
//=============================================================================

//#define PRECISION   0.1 //$$$ NLD CRADE POUR LE MOMENT

//-----------------------------------------------------------------------------
// Name: TER_Polygon::IsInside
// Created: JDY 03-05-19
//-----------------------------------------------------------------------------
bool TER_Polygon::IsInside( const MT_Vector2D& vPos, double rPrecision ) const
{
    if( !pData_ )
        return true;

    if( ! IsInBoundingBox( vPos, rPrecision ) )
        return false;

    return BoundedSide( vPos ) != eOnUnboundedSide
        || IsOnBorder(vPos, rPrecision);
}

// -----------------------------------------------------------------------------
// Name: TER_Polygon::IsInsidish
// Created: AGE 2005-05-12
// -----------------------------------------------------------------------------
bool TER_Polygon::IsInsidish( const MT_Vector2D& vPos ) const
{
    if( !pData_ || ! IsInBoundingBox( vPos ) )
        return false;
    return BoundedSide( vPos ) != eOnUnboundedSide;
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon::IsOnBorder
// Created: JDY 03-05-20
//-----------------------------------------------------------------------------
bool TER_Polygon::IsOnBorder( const MT_Vector2D& vPos, double rPrecision ) const
{
    if( !pData_ )
        return false;

    // Test des bords
    const MT_Vector2D* pLastPos = 0;
    for( CIT_PointVector itPoint = pData_->borderVector_.begin(); itPoint != pData_->borderVector_.end(); ++itPoint )
    {
        const MT_Vector2D* pPos = &*itPoint;
        if( pLastPos )
        {
            MT_Line lineTmp( *pLastPos, *pPos );
            if( lineTmp.IsInside( vPos, rPrecision ) ) //$$$$ DEVRAIT TESTER AVEC BOUNDING BOX AVANT
                return true;
        }
        pLastPos = pPos;
    }
    return false;
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon::IntersectWithBorder
// Created: JDY 03-05-19
//-----------------------------------------------------------------------------
bool TER_Polygon::IntersectWithBorder( const MT_Line& line, double rPrecision ) const
{
    if( !pData_ )
        return false;

    if( pData_->borderVector_.size() > 8 //$$$$ Optim, il semblerait
   && ! pData_->boundingBox_.Intersect2D( line )
   && ! pData_->boundingBox_.IsInside( line.GetPosStart() )
   && ! pData_->boundingBox_.IsInside( line.GetPosEnd() ) )
        return false;

    const MT_Vector2D* pLastPos = 0;
    for( CIT_PointVector itPoint = pData_->borderVector_.begin(); itPoint != pData_->borderVector_.end(); ++itPoint )
    {
        const MT_Vector2D* pPos = &*itPoint;

        if( pLastPos )
        {
            MT_Line lineTmp( *pLastPos, *pPos );
            if( lineTmp.IsClipped( line ) && lineTmp.Intersect2D( line ) == eDoIntersect )
            {
                if( !lineTmp.IsInside( line.GetPosStart(), rPrecision ) && !lineTmp.IsInside( line.GetPosEnd(), rPrecision ) )
                    return true;
            }
        }

        pLastPos = pPos;
    }
    return false;
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon::IntersectWithBorder
// Created: JDY 03-05-28
//-----------------------------------------------------------------------------
bool TER_Polygon::IntersectWithBorder( const MT_Line& line,T_PointSet& res, double rPrecision ) const
{
    if( !pData_ )
        return false;

    const MT_Vector2D* pLastPos = 0;
    for( CIT_PointVector itPoint = pData_->borderVector_.begin(); itPoint != pData_->borderVector_.end(); ++itPoint )
    {
        const MT_Vector2D* pPos = &*itPoint;

        if( pLastPos )
        {
            MT_Vector2D vPosIntersect;
            MT_Line lineTmp( *pLastPos, *pPos );
            if( lineTmp.IsClipped( line ) && lineTmp.Intersect2D( line ,vPosIntersect ) == eDoIntersect )
            {
                assert( lineTmp.IsInside( vPosIntersect, rPrecision ) );
                if( !lineTmp.IsInside( line.GetPosStart(), rPrecision ) && !lineTmp.IsInside( line.GetPosEnd(), rPrecision ) ) //$$$ test colinéraité ?
                    res.insert( vPosIntersect );
            }
        }
        pLastPos = pPos;
    }

    return !res.empty();
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon::IntersectWithBorder
// Created: NLD 2003-08-20
//-----------------------------------------------------------------------------
bool TER_Polygon::IntersectWithBorder( const MT_Droite& line, T_PointSet& res ) const
{
    if( pData_ )
        return false;

    const MT_Vector2D* pLastPos = 0;
    for( CIT_PointVector itPoint = pData_->borderVector_.begin(); itPoint != pData_->borderVector_.end(); ++itPoint )
    {
        const MT_Vector2D* pPos = &*itPoint;

        if( pLastPos )
        {
            MT_Vector2D vPosIntersect;
            MT_Line segTmp( *pLastPos, *pPos );

            if( line.Intersect2D( segTmp, vPosIntersect ) )
                res.insert( vPosIntersect );
        }
        pLastPos = pPos;
    }
    return !res.empty();
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon::Intersect2D
// Created: NLD 2003-07-25
//-----------------------------------------------------------------------------
bool TER_Polygon::Intersect2D( const MT_Line& line, double rPrecision ) const
{
    if( !pData_ )
        return false;
    return IsInside( line.GetPosStart(), rPrecision )
        || IsInside( line.GetPosEnd(), rPrecision )
        || IntersectWithBorder( line, rPrecision );
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon::Intersect2D
// Created: NLD 2003-07-25
//-----------------------------------------------------------------------------
bool TER_Polygon::Intersect2D( const MT_Line& line, T_PointSet& collisions, double rPrecision  ) const
{
    if( !pData_ )
        return false;
    collisions.clear();
    IntersectWithBorder( line, collisions, rPrecision );
    if( IsInside( line.GetPosStart(), rPrecision ) )
        collisions.insert( line.GetPosStart() );
    if( IsInside( line.GetPosEnd(), rPrecision ) )
        collisions.insert( line.GetPosEnd() );
    return !collisions.empty();
}

// -----------------------------------------------------------------------------
// Name: TER_Polygon::Intersect2D
// Created: NLD 2003-08-20
// -----------------------------------------------------------------------------
bool TER_Polygon::Intersect2D( const MT_Droite& line, T_PointSet& res, double /*rPrecision*/ ) const
{
    return IntersectWithBorder( line, res );
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon::Intersect2DWithCircle
// Created: NLD 2003-07-25
//-----------------------------------------------------------------------------
bool TER_Polygon::Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius ) const
{
    if( !pData_ )
        return false;

    if( IsInside( vCircleCenter ) )
        return true;

    const MT_Vector2D* pLastPos = 0;
    for( auto it = pData_->borderVector_.begin(); it != pData_->borderVector_.end(); ++it )
    {
        const MT_Vector2D& pos = *it;
        if( pLastPos )
        {
            const MT_Line line( *pLastPos, pos );
            if( line.ClosestPointOnLine( vCircleCenter ).Distance( vCircleCenter ) <= rRadius )
                return true;
        };
        pLastPos = &pos;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: TER_Polygon::Intersection
// Created: JVT 2005-05-16
// -----------------------------------------------------------------------------
void TER_Polygon::Intersection( MT_Polyline& polyline, double rPrecision ) const
{
    T_PointVector res;

    CIT_PointVector itSegEnd = polyline.GetPoints().begin();
    if( !pData_ || itSegEnd == polyline.GetPoints().end() )
    {
        polyline = res;
        return;
    }

    if( IsInside( *itSegEnd ) )
        res.push_back( *itSegEnd );

    for( CIT_PointVector itSegBegin = itSegEnd++; itSegEnd != polyline.GetPoints().end(); ++itSegBegin, ++itSegEnd )
    {
        TER_DistanceLess cmp( *itSegBegin );
        T_PointSet       collisionSet( cmp );

        IntersectWithBorder( MT_Line( *itSegBegin, *itSegEnd ), collisionSet, rPrecision );

        for( auto it = collisionSet.begin(); it != collisionSet.end(); ++it )
            res.push_back( *it );

        if( IsInside( *itSegEnd ) )
            res.push_back( *itSegEnd );
    }

    polyline = res;
}

// -----------------------------------------------------------------------------
// Name: TER_Polygon::ComputeExtremities
// Created: NLD 2003-08-20
// -----------------------------------------------------------------------------
void TER_Polygon::ComputeExtremities( const MT_Droite& line, MT_Vector2D& vLeftPoint, MT_Vector2D& vRightPoint ) const
{
    vLeftPoint = MT_Vector2D();
    vRightPoint = MT_Vector2D();
    if( !pData_ )
        return;

    // Retourne les deux extrémités du polygone sur la droite
    TER_DistanceLess cmp( MT_Vector2D( 0, 0 ) );
    T_PointSet collisionSet( cmp );
    IntersectWithBorder( line, collisionSet );
    if( collisionSet.size() <= 1 )
        return;

    vLeftPoint  = *collisionSet.begin();
    vRightPoint = *(--collisionSet.end());
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon::GetBoundingBox
// Created: JVT 04-06-22
//-----------------------------------------------------------------------------
MT_Rect TER_Polygon::GetBoundingBox() const
{
    if( !pData_ )
        return emptyRect;
    return pData_->boundingBox_;
}

namespace
{

    // -----------------------------------------------------------------------------
    // Name: ComputeOrientation
    // Created: AGE 2005-06-14
    // -----------------------------------------------------------------------------
    double ComputeOrientation( const MT_Vector2D& first, const MT_Vector2D& second, const MT_Vector2D& third )
    {
        const double r1 = second.rX_ - first.rX_;
        const double r2 = second.rY_ - first.rY_;
        const double r3 = third.rX_  - first.rX_;
        const double r4 = third.rY_  - first.rY_ ;
        return r1 * r4 - r2 * r3;
    }

    // -----------------------------------------------------------------------------
    // Name: TriangleArea
    // Created: AGE 2005-06-14
    // -----------------------------------------------------------------------------
    double TriangleArea( const MT_Vector2D& first, const MT_Vector2D& second, const MT_Vector2D& third )
    {
        return ComputeOrientation( first, second, third ) * 0.5;
    }
};

// -----------------------------------------------------------------------------
// Name: TER_Polygon::GetArea
// Created: AGE 2005-06-14
// -----------------------------------------------------------------------------
double TER_Polygon::GetArea() const
{
    double result = 0;
    if( ! pData_ || pData_->borderVector_.empty() )
        return 0;

    CIT_PointVector second = pData_->borderVector_.begin();
    ++second;

    // check if the polygon has only one point
    if (second == pData_->borderVector_.end())
        return 0;

    const MT_Vector2D firstPoint = pData_->borderVector_.front();
    CIT_PointVector third = second;
    while( ++third != pData_->borderVector_.end())
    {
        result = result + TriangleArea( firstPoint, *second, *third );
        second = third;
    }
    return std::fabs( result );
}

// -----------------------------------------------------------------------------
// Name: TER_Polygon::BoundedSide
// Created: AGE 2005-06-14
// -----------------------------------------------------------------------------
TER_Polygon::E_BoundedSize TER_Polygon::BoundedSide( const MT_Vector2D& point ) const
{
    bool bIsInside = false;
    CIT_PointVector current = pData_->borderVector_.begin();
    while( current != pData_->borderVector_.end() )
    {
        CIT_PointVector next = current + 1;
        if( next == pData_->borderVector_.end() )
            next = pData_->borderVector_.begin();

        // $$$$ MCO 2005-02-02: might be simplified further
        if( current->rY_ < point.rY_ )
        {
            if( next->rY_ == point.rY_ && point.rX_ == next->rX_ )
                return eOnBoundary;
            if( next->rY_ > point.rY_ )
            {
                const double orientation = ComputeOrientation( *current, point, *next );
                if( orientation == 0 )
                    return eOnBoundary;
                if( orientation < 0 )
                    bIsInside = ! bIsInside;
            }
        }
        else if( current->rY_ > point.rY_ )
        {
            if( next->rY_ == point.rY_ && point.rX_ == next->rX_ )
                return eOnBoundary;
            if( next->rY_ < point.rY_ )
            {
                const double orientation = ComputeOrientation( *next, point, *current );
                if( orientation == 0 )
                    return eOnBoundary;
                if( orientation < 0 )
                    bIsInside = ! bIsInside;
            }
        }
        else
        {
            if( point.rX_ == current->rX_ )
                return eOnBoundary;
            if( next->rY_ == point.rY_ )
            {
                if( point.rX_ > current->rX_ && point.rX_ <= next->rX_
                 || point.rX_ < current->rX_ && point.rX_ >= next->rX_ )
                    return eOnBoundary;
            }
            else if( point.rX_ < current->rX_ )
                bIsInside = ! bIsInside;
        }
        ++current;
    }
    return bIsInside ? eOnBoundedSide : eOnUnboundedSide;
}

// -----------------------------------------------------------------------------
// Name: TER_Polygon::IsInBoundingBox
// Created: AGE 2005-06-16
// -----------------------------------------------------------------------------
bool TER_Polygon::IsInBoundingBox( const MT_Vector2D& p, double rPrecision /*=0*/) const
{
    return pData_->boundingBox_.IsInside( p, rPrecision );
}

bool TER_Polygon::IsInside( const MT_Vector2D& vPos ) const
{
    return IsInside( vPos, 0.1 );
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon::GetBorderPoints
// Created: JDY 03-05-21
//-----------------------------------------------------------------------------
const T_PointVector& TER_Polygon::GetBorderPoints() const
{
    return pData_->borderVector_;
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon::Reset
// Created: NLD 2003-07-25
//-----------------------------------------------------------------------------
void TER_Polygon::Reset( const T_PointVector& points, bool bConvexHull )
{
    *this=TER_Polygon( points, bConvexHull ); //$$ c'est de la merde !
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon::Reset
// Created: JDY 03-05-19
//-----------------------------------------------------------------------------
void TER_Polygon::Reset()
{
    pData_.reset();
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon::IsNull
// Created: JDY 03-05-19
//-----------------------------------------------------------------------------
bool TER_Polygon::IsNull() const
{
    return !pData_;
}

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
#include "TER_Polygon.inl"
#include "MT_Tools/MT_Polyline.h"
#include "MT_Tools/MT_Droite.h"

TER_Polygon* TER_Polygon::empty_polygon = 0;

//-----------------------------------------------------------------------------
// Name: TER_Polygon constructor
// Created: JDY 03-05-19
//-----------------------------------------------------------------------------
TER_Polygon::TER_Polygon()
{
    if (!empty_polygon)
    {
        empty_polygon=new TER_Polygon(true);
        assert(empty_polygon);
    }
    pData_ = empty_polygon->pData_;
    pData_->ref();
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon constructor
// Created: JDY 03-05-26
//-----------------------------------------------------------------------------
TER_Polygon::TER_Polygon(bool bEmpty)
{
    pData_ = new PolygonData();
    assert( pData_ );
    pData_->bIsNull_=bEmpty;
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon constructor
// Created: JDY 03-05-26
//-----------------------------------------------------------------------------
TER_Polygon::TER_Polygon( const T_PointPtrVector& points,bool bConvexHull )
{
    pData_ = new PolygonData();
    pData_->bIsNull_=points.empty();

    // no points in polygon
    if (pData_->bIsNull_)
        return;

    // set ring
    pData_->borderVector_.reserve( points.size() );
    for( CIT_PointPtrVector itPoint = points.begin(); itPoint != points.end(); ++itPoint )
        pData_->borderVector_.push_back( **itPoint );

    if( bConvexHull )
        Convexify();

    ComputeBoundingBox();
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon constructor
// Created: JDY 03-05-26
//-----------------------------------------------------------------------------
TER_Polygon::TER_Polygon( const T_PointVector& points, bool bConvexHull )
{
    pData_ = new PolygonData();
    pData_->bIsNull_=points.empty();

    // no points in polygon
    if (pData_->bIsNull_)
        return;

    pData_->borderVector_ = points;

    if( bConvexHull )
        Convexify();

    ComputeBoundingBox();
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon constructor
// Created: JDY 03-05-26
//-----------------------------------------------------------------------------
TER_Polygon::TER_Polygon(const TER_Polygon& poly)
{
    pData_ = poly.pData_;
    pData_->ref();
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon destructor
// Created: JDY 03-05-19
//-----------------------------------------------------------------------------
TER_Polygon::~TER_Polygon()
{
    Detach();
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon operator =
// Created: JDY 03-05-26
//-----------------------------------------------------------------------------
TER_Polygon& TER_Polygon::operator=( const TER_Polygon& rhs )
{
    rhs.pData_->ref();                // beware of r = r
    Detach();
    pData_ = rhs.pData_;
    return *this;
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon::Detach
// Created: JDY 03-05-26
//-----------------------------------------------------------------------------
void TER_Polygon::Detach()
{
    assert(pData_);
    if( pData_->deref() )
        delete pData_;
    pData_ = 0;
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
    if (IsNull())
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
    if( IsNull() || ! IsInBoundingBox( vPos ) )
        return false;
    return BoundedSide( vPos ) != eOnUnboundedSide;
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon::IsOnBorder
// Created: JDY 03-05-20
//-----------------------------------------------------------------------------
bool TER_Polygon::IsOnBorder( const MT_Vector2D& vPos, double rPrecision ) const
{
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
    if (IsNull())
        return false;

    if( pData_->borderVector_.size() > 8 //$$$$ Optim, il semblerait
   && ! pData_->boundingBox_.Intersect2D( line )
   && ! pData_->boundingBox_.IsInside( line.GetPosStart() ) )
        return false;

    const MT_Vector2D* pLastPos = 0;
    for( CIT_PointVector itPoint = pData_->borderVector_.begin(); itPoint != pData_->borderVector_.end(); ++itPoint )
    {
        const MT_Vector2D* pPos = &*itPoint;

        if( pLastPos )
        {
            MT_Line lineTmp( *pLastPos, *pPos );
            if( lineTmp.IsClipped( line ) && lineTmp.Intersect2D( line ) )
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
    if (IsNull())
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
    if( IsNull() )
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
    if( IsInside( vCircleCenter ) )
        return true;

    const MT_Vector2D* pLastPos = 0;
    for( CIT_PointVector it = pData_->borderVector_.begin(); it != pData_->borderVector_.end(); ++it )
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

    if( itSegEnd == polyline.GetPoints().end() )
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

        for( CIT_PointSet it = collisionSet.begin(); it != collisionSet.end(); ++it )
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
    // Retourne les deux extrémités du polygone sur la droite

    TER_DistanceLess cmp( MT_Vector2D( 0, 0 ) );
    T_PointSet collisionSet( cmp );

    IntersectWithBorder( line, collisionSet );
    assert( collisionSet.size() > 1 );

    vLeftPoint  = *collisionSet.begin();
    vRightPoint = *(--collisionSet.end());
}

// -----------------------------------------------------------------------------
// Name: TER_Polygon::ComputeBoundingBox
// Created: AGE 2005-06-16
// -----------------------------------------------------------------------------
void TER_Polygon::ComputeBoundingBox()
{
    // bounding box
    MT_Vector2D vDownLeft( std::numeric_limits<double>::max(), std::numeric_limits<double>::max() );
    MT_Vector2D vUpRight ( std::numeric_limits<double>::min(), std::numeric_limits<double>::min() );

    for( CIT_PointVector itPoint = pData_->borderVector_.begin(); itPoint != pData_->borderVector_.end(); ++itPoint )
    {
        const MT_Vector2D& vPos = *itPoint;
        if( vPos.rX_ < vDownLeft.rX_ )
            vDownLeft.rX_ = vPos.rX_;
        if( vPos.rY_ < vDownLeft.rY_ )
            vDownLeft.rY_ = vPos.rY_;
        if( vPos.rX_ > vUpRight.rX_ )
            vUpRight.rX_ = vPos.rX_;
        if( vPos.rY_ > vUpRight.rY_ )
            vUpRight.rY_ = vPos.rY_;
    }
    pData_->boundingBox_ = MT_Rect( vDownLeft.rX_, vDownLeft.rY_, vUpRight.rX_, vUpRight.rY_ );
}

//-----------------------------------------------------------------------------
// Name: TER_Polygon::GetBoundingBox
// Created: JVT 04-06-22
//-----------------------------------------------------------------------------
MT_Rect TER_Polygon::GetBoundingBox() const
{
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
// Name: TER_Polygon::ComputeArea
// Created: AGE 2005-06-14
// -----------------------------------------------------------------------------
double TER_Polygon::ComputeArea() const
{
    double result = 0;
    if( ! pData_ || pData_->bIsNull_ || pData_->borderVector_.empty() )
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

namespace
{
    // -----------------------------------------------------------------------------
    // Name: InitializeHull
    // Created: AGE 2005-06-14
    // -----------------------------------------------------------------------------
    void InitializeHull( const T_PointVector& polygon, T_PointVector& hull )
    {
        MT_Vector2D min, max;
        for( CIT_PointVector it = polygon.begin(); it != polygon.end(); ++it )
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
        for( CIT_PointVector it = polygon.begin(); it != polygon.end(); ++it )
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
}

// -----------------------------------------------------------------------------
// Name: TER_Polygon::Convexify
// Created: AGE 2005-06-14
// -----------------------------------------------------------------------------
void TER_Polygon::Convexify()
{
    if( ! pData_ || pData_->borderVector_.size() < 3 )
        return;

    T_PointVector convexHull;
    InitializeHull( pData_->borderVector_, convexHull );

    unsigned int nPoint = 0;
    while( nPoint != convexHull.size() )
    {
        unsigned int nFollowingPoint = nPoint + 1;
        if( nFollowingPoint == convexHull.size() )
            nFollowingPoint = 0;

        MT_Vector2D direction = convexHull[ nFollowingPoint ] - convexHull[ nPoint ];
        direction.Normalize();
        MT_Vector2D worst;
        if( FindOuterPoint( pData_->borderVector_, convexHull[ nPoint ], direction, worst ) )
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

    pData_->borderVector_ = convexHull;
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

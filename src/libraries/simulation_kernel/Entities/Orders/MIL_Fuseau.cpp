//*****************************************************************************
//
// $Created: NLD 2003-01-14 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/MIL_Fuseau.cpp $
// $Author: Jvt $
// $Modtime: 16/05/05 14:55 $
// $Revision: 17 $
// $Workfile: MIL_Fuseau.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Fuseau.h"
#include "MIL_LimaOrder.h"
#include "Decision/DEC_Objective.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "MIL_AgentServer.h"
#include "Tools/MIL_Tools.h"
#include "MT_Tools/MT_Polyline.h"
#include "MT_Tools/MT_Droite.h"
#include "simulation_terrain/TER_LimitData.h"
#include "simulation_terrain/TER_LimitDataManager.h"
#include "simulation_terrain/TER_World.h"

namespace
{
    boost::shared_ptr< TER_LimitData > CreateLimit( const T_PointVector& points )
    {
        return TER_World::GetWorld().GetLimitManager().CreateLimit( points );
    }
}

unsigned int MIL_Fuseau::nNbrMeterPerSample_ = 400; //$$$ A GICLER

#define PRECISION 0.1 //$$ CRADE

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_Fuseau )

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau constructor
// Created: NLD 2004-05-21
// -----------------------------------------------------------------------------
MIL_Fuseau::MIL_Fuseau( const MT_Vector2D& vOrientationRefPos, const T_PointVector& leftLimit, const T_PointVector& rightLimit, const MIL_LimaOrder* pBeginMissionLima, const MIL_LimaOrder* pEndMissionLima )
    : globalDirectionLine_  ( vStartGlobalDirection_, vEndGlobalDirection_ )
{
    Reset( &vOrientationRefPos, leftLimit, rightLimit, pBeginMissionLima, pEndMissionLima );
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau constructor
// Created: LDC 2012-09-19
// -----------------------------------------------------------------------------
MIL_Fuseau::MIL_Fuseau( const MIL_Fuseau& rhs )
    : TER_Polygon( rhs )
    , pLeftLimit_           ( rhs.pLeftLimit_ )
    , pRightLimit_          ( rhs.pRightLimit_ )
    , pMiddleLimit_         ( rhs.pMiddleLimit_ )
    , vStartGlobalDirection_( rhs.vStartGlobalDirection_ )
    , vEndGlobalDirection_  ( rhs.vEndGlobalDirection_ )
    , globalDirectionLine_  ( vStartGlobalDirection_, vEndGlobalDirection_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau constructor
// Created: LDC 2012-08-06
// -----------------------------------------------------------------------------
MIL_Fuseau::MIL_Fuseau( const T_PointVector& leftLimit, const T_PointVector& rightLimit )
    : globalDirectionLine_( vStartGlobalDirection_, vEndGlobalDirection_ )
{
    Reset( 0, leftLimit, rightLimit, 0, 0 );
}

//-----------------------------------------------------------------------------
// Name: MIL_Fuseau constructor
// Created: NLD 2003-01-14
//-----------------------------------------------------------------------------
MIL_Fuseau::MIL_Fuseau()
    : globalDirectionLine_( vStartGlobalDirection_, vEndGlobalDirection_ )
{
    Reset();
}

//-----------------------------------------------------------------------------
// Name: MIL_Fuseau destructor
// Created: NLD 2003-01-14
//-----------------------------------------------------------------------------
MIL_Fuseau::~MIL_Fuseau()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================

//$$$ FONCTIONS DE MERDE !!

namespace
{
void InsertClosestIntersectionWithDroite( T_PointVector& pointVector, const MT_Droite& droite, const MT_Vector2D& vPointSrc )
{
    MT_Vector2D     vIntersect;
    IT_PointVector itWhereToInsertIntersection = pointVector.end();

    IT_PointVector itFirstPoint = pointVector.begin();
    const MT_Vector2D* pPos1 = &*itFirstPoint;
    for( IT_PointVector itPoint = ++itFirstPoint; itPoint != pointVector.end(); ++itPoint )
    {
        const MT_Vector2D* pPos2 = &*itPoint;
        MT_Line lineTmp( *pPos1, *pPos2 );

        MT_Vector2D vIntersectTmp;
        if( droite.Intersect2D( lineTmp, vIntersectTmp ) == eDoIntersect
            &&  ( vIntersect.IsZero() || vPointSrc.SquareDistance( vIntersect ) < vPointSrc.SquareDistance( vIntersectTmp ) ) )
        {
            vIntersect                  = vIntersectTmp;
            itWhereToInsertIntersection = itPoint;
        }
        pPos1 = pPos2;
    }

    if( itWhereToInsertIntersection == pointVector.end() )
    {
        if( vPointSrc.SquareDistance( pointVector.front() ) < vPointSrc.SquareDistance( pointVector.back() ) )
        {
            MT_Vector2D tmp = pointVector.front();
            pointVector.insert( pointVector.begin(), tmp );
        }
        else
        {
            MT_Vector2D tmp = pointVector.back();
            pointVector.insert( pointVector.end(), tmp );
        }
    }
    else
        pointVector.insert( itWhereToInsertIntersection, vIntersect );
}

// NB : vIntersection is on the line between itIntersectionIterator - 1 and itIntersectionIterator
bool ComputeIntersectionWithLima( const T_PointVector& limit, const MIL_LimaOrder& lima, MT_Vector2D& vIntersection, CIT_PointVector& itIntersectionIterator )
{
    itIntersectionIterator = limit.end();

    CIT_PointVector itPrevPoint = limit.begin();
    CIT_PointVector itCurPoint  = itPrevPoint;
    for( ++itCurPoint; itCurPoint != limit.end(); ++itCurPoint )
    {
        MT_Line lineTmp( *itPrevPoint, *itCurPoint );

        if( lima.Intersect2D( lineTmp, vIntersection ) )
        {
            itIntersectionIterator = itCurPoint;
            return true;
        }
        itPrevPoint = itCurPoint;
    }
    return false;
}

void InsertPointProjection( const T_PointVector& source, T_PointVector& target )
{
    MT_Vector2D vNormDirCurLine;
    for( CIT_PointVector itPoint = source.begin(); itPoint != source.end(); ++ itPoint )
    {
        const MT_Vector2D& vCurPoint = *itPoint;

        MT_Vector2D vNormDirNextLine;
        if( (itPoint+1) != source.end() )
        {
            vNormDirNextLine = MT_Vector2D( *(itPoint+1) - *itPoint );
            vNormDirNextLine.Normalize();
            vNormDirNextLine.Rotate90ClockWise();
        }

        MT_Vector2D vDir = vNormDirNextLine + vNormDirCurLine;
        vNormDirCurLine.Normalize();

        InsertClosestIntersectionWithDroite( target, MT_Droite( vCurPoint, vCurPoint + vDir ), vCurPoint );
        vNormDirCurLine = vNormDirNextLine;
    }
}
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::SplitLimit
// Created: NLD 2004-06-16
// -----------------------------------------------------------------------------
void MIL_Fuseau::SplitLimit( const MIL_LimaOrder* pBeginMissionLima, const MIL_LimaOrder* pEndMissionLima, const T_PointVector& limit, T_PointVectorVector& parts ) const
{
    CIT_PointVector itBeginLimaIntersection = limit.end();
    CIT_PointVector itEndLimaIntersection   = limit.end();
    MT_Vector2D     vBeginLimaIntersection;
    MT_Vector2D     vEndLimaIntersection;

    if( pBeginMissionLima )
        ComputeIntersectionWithLima( limit, *pBeginMissionLima, vBeginLimaIntersection, itBeginLimaIntersection );

    if( pEndMissionLima )
        ComputeIntersectionWithLima( limit, *pEndMissionLima, vEndLimaIntersection, itEndLimaIntersection );

    bool bReverse = false;
    if( itEndLimaIntersection != limit.end() && itBeginLimaIntersection != limit.end() && itBeginLimaIntersection > itEndLimaIntersection )
    {
        std::swap( itEndLimaIntersection, itBeginLimaIntersection );
        std::swap( vBeginLimaIntersection, vEndLimaIntersection );
        bReverse = true;
    }

    CIT_PointVector itPrev = limit.begin();
    parts.reserve( 4 );
    parts.push_back( T_PointVector() );

    // 1st part
    if( itBeginLimaIntersection != limit.end() )
    {
        assert( itPrev <= itBeginLimaIntersection );
        parts.back().reserve( itBeginLimaIntersection - itPrev + 1 );
        std::copy( itPrev, itBeginLimaIntersection, std::back_inserter( parts.back() ) );
        parts.back().push_back( vBeginLimaIntersection );
        parts.push_back( T_PointVector() );
        parts.back().push_back( vBeginLimaIntersection );
        itPrev = itBeginLimaIntersection;
    }
    else if( pBeginMissionLima )
    {
        parts.back().push_back( limit.front() );
        parts.push_back( T_PointVector() );
        parts.back().push_back( limit.front() );
        itPrev = limit.begin() + 1;
    }

    // 2nd part
    if( itEndLimaIntersection != limit.end() )
    {
        assert( itPrev <= itEndLimaIntersection );
        parts.back().reserve( itEndLimaIntersection - itPrev + 2 );
        std::copy( itPrev, itEndLimaIntersection, std::back_inserter( parts.back() ) );
        parts.back().push_back( vEndLimaIntersection );
        parts.push_back( T_PointVector() );
        parts.back().push_back( vEndLimaIntersection );
        itPrev = itEndLimaIntersection;
    }
    else if( pEndMissionLima )
    {
        assert( itPrev <= limit.end() );
        std::copy( itPrev, limit.end(), std::back_inserter( parts.back() ) );
        parts.push_back( T_PointVector() );
        parts.back().push_back( limit.back() );
        itPrev = limit.end();
    }

    // 3rd part
    parts.back().reserve( limit.end() - itPrev + 1 );
    std::copy( itPrev, limit.end(), std::back_inserter( parts.back() ) );

    if( parts.size() == 3 )
    {
        // Just keep the middle part
        parts.pop_back();
        parts.erase( parts.begin() );
        if( bReverse )
            std::reverse( parts.front().begin(), parts.front().end() );
    }
    else if( parts.size() == 2 )
    {
        if( itEndLimaIntersection != limit.end() )
            std::reverse( parts[1].begin(), parts[1].end() ); // The limit must be ordered : the last point MUST be itEndLimaIntersection

        else if( itBeginLimaIntersection != limit.end() )
        {
            std::reverse( parts[0].begin(), parts[0].end() ); // The limit must be ordered : the first point MUST be itBeginLimaIntersection
            std::reverse( parts.begin(), parts.end() );       // The prefered limit (parts.front()) is the one going to itBegin to end (not begin to itBegin), regarding the initial limit ordering
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::CreatePolygonFromLimits
// Created: NLD 2004-06-17
// -----------------------------------------------------------------------------
bool MIL_Fuseau::IsPointInsidePolygon( T_PointVector& leftPoints, T_PointVector& rightPoints, const MT_Vector2D& vPoint )
{
    T_PointVector pointsVector( leftPoints.size() + rightPoints.size() + 1 );
    std::copy( leftPoints.begin(), leftPoints.end(), pointsVector.end() );
    std::copy( rightPoints.rbegin(), rightPoints.rend(), pointsVector.end() );
    pointsVector.back() = leftPoints.front();

    TER_Polygon polygon;
    polygon.Reset( pointsVector );
    return polygon.IsInside( vPoint, 0.1 ); ///$$$$
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::TruncateAndOrienteLimits
// Created: NLD 2004-05-19
// -----------------------------------------------------------------------------
void MIL_Fuseau::TruncateAndReorientLimits( const MT_Vector2D* vOrientationRefPos, T_PointVector& leftLimit, T_PointVector& rightLimit, const MIL_LimaOrder* pBeginMissionLima, const MIL_LimaOrder* pEndMissionLima )
{
    if( leftLimit.empty() || rightLimit.empty() )
        throw MASA_EXCEPTION( "Null MIL_Fuseau" );
    // The two limits must have the same orientation
    MT_Vector2D u = *leftLimit.rbegin() - *leftLimit.begin();
    MT_Vector2D v = *rightLimit.rbegin() - *rightLimit.begin();
    const double rProjection = DotProduct( u, v );
    if( rProjection < 0. )
        std::reverse( rightLimit.begin(), rightLimit.end() );

    // The orientation cannot be determined from the 'LDM' and the 'LFM'
    //  => orient it regarding the 'orientation reference position' (usually the
    //     position of unit owning the fuseau

    // Orientation = From the closest border to the furthest
    MT_Line lineBorder1( *leftLimit.begin (), *rightLimit.begin() );
    MT_Line lineBorder2( *leftLimit.rbegin(), *rightLimit.rbegin() );
    if( vOrientationRefPos && vOrientationRefPos->SquareDistance( lineBorder1.GetCenter() ) > vOrientationRefPos->SquareDistance( lineBorder2.GetCenter() ) )
    {
        std::reverse( rightLimit.begin(), rightLimit.end() );
        std::reverse( leftLimit.begin(), leftLimit.end() );
    }

    T_PointVectorVector leftParts;
    T_PointVectorVector rightParts;

    SplitLimit( pBeginMissionLima, pEndMissionLima, leftLimit , leftParts  );
    SplitLimit( pBeginMissionLima, pEndMissionLima, rightLimit, rightParts );

    if( leftParts.size() != rightParts.size() || leftParts.empty() || leftParts.size() > 2 )
        throw MASA_EXCEPTION( "Invalid parts number when splitting fuseau" );

    std::size_t nNbParts = leftParts.size();
    if( nNbParts == 1 )
    {
        leftLimit  = leftParts [0];
        rightLimit = rightParts[0];
    }
    else if( nNbParts == 3 )
    {
        leftLimit  = leftParts [1];
        rightLimit = rightParts[1];
    }
    else if( nNbParts == 2 )
    {
        // Si 2 parties, on prend en priorité celle dans laquelle le point de réference se trouve
        if( vOrientationRefPos && IsPointInsidePolygon( leftParts[1], rightParts[1], *vOrientationRefPos ) )
        {
            leftLimit  = leftParts [1];
            rightLimit = rightParts[1];
        }
        // Sinon, on prend la première partie, qui tient compte de l'orientation initiale
        else
        {
            leftLimit  = leftParts [0];
            rightLimit = rightParts[0];
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::InitializePolygon
// Created: NLD 2004-05-19
// -----------------------------------------------------------------------------
void MIL_Fuseau::InitializePolygon()
{
    if( IsNull() )
        throw MASA_EXCEPTION( "Null fuseau" );
    const T_PointVector& leftLimitTmp  = pLeftLimit_ ->GetPoints();
    const T_PointVector& rightLimitTmp = pRightLimit_->GetPoints();

    // Create the ring
    T_PointVector pointsVector;
    pointsVector.reserve( leftLimitTmp.size() + rightLimitTmp.size() + 1 );
    for( CIT_PointVector itPoint = leftLimitTmp.begin(); itPoint != leftLimitTmp.end(); ++itPoint )
        pointsVector.push_back( *itPoint );
    for( CRIT_PointVector ritPoint = rightLimitTmp.rbegin(); ritPoint != rightLimitTmp.rend(); ++ritPoint )
        pointsVector.push_back( *ritPoint );
    pointsVector.push_back( *leftLimitTmp.begin() );

    // create new polygon with previous points
    TER_Polygon::Reset( pointsVector );
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::InitializeMiddleLimit
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
void MIL_Fuseau::InitializeMiddleLimit()
{
    if( IsNull() )
        throw MASA_EXCEPTION( "Null fuseau" );

    T_PointVector leftPointVectorTmp  = pLeftLimit_ ->GetPoints();
    T_PointVector rightPointVectorTmp = pRightLimit_->GetPoints();

    InsertPointProjection( pLeftLimit_ ->GetPoints(), rightPointVectorTmp );
    InsertPointProjection( pRightLimit_->GetPoints(), leftPointVectorTmp  );
    if( leftPointVectorTmp.size() != rightPointVectorTmp.size() )
        throw MASA_EXCEPTION( "Error InitializeMiddleLimit" );
    assert( !leftPointVectorTmp.empty() );
    ///
    T_PointVector middle;
    middle.reserve( leftPointVectorTmp.size() );
    for( unsigned int j = 0; j < leftPointVectorTmp.size(); ++j )
        middle.push_back( leftPointVectorTmp[j] + ( rightPointVectorTmp[j] - leftPointVectorTmp[j] ) / 2 );

    assert( !pMiddleLimit_ );
    pMiddleLimit_ = CreateLimit( middle );
}

//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::Reset
// Created: NLD 2003-01-14
//-----------------------------------------------------------------------------
void MIL_Fuseau::Reset( const MT_Vector2D* vOrientationRefPos, const T_PointVector& leftLimit, const T_PointVector& rightLimit, const MIL_LimaOrder* pBeginMissionLima, const MIL_LimaOrder* pEndMissionLima )
{
    if( leftLimit .empty() || rightLimit.empty() )
        throw MASA_EXCEPTION( "Error MILFuseau::Reset" );

    MIL_Fuseau::Reset();

    if( !pBeginMissionLima || !pEndMissionLima )
        pBeginMissionLima = pEndMissionLima = 0;

    T_PointVector leftLimitTmp  = leftLimit;
    T_PointVector rightLimitTmp = rightLimit;

    TruncateAndReorientLimits( vOrientationRefPos, leftLimitTmp, rightLimitTmp, pBeginMissionLima, pEndMissionLima  );

    pLeftLimit_ = CreateLimit( leftLimitTmp );
    pRightLimit_ = CreateLimit( rightLimitTmp );

    InitializeMiddleLimit();
    InitializePolygon    ();

    // Fuseau global orientation
    vStartGlobalDirection_ = MT_Line( *pLeftLimit_->GetPoints().begin (), *pRightLimit_->GetPoints().begin () ).GetCenter();
    vEndGlobalDirection_   = MT_Line( *pLeftLimit_->GetPoints().rbegin(), *pRightLimit_->GetPoints().rbegin() ).GetCenter();
}

//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::Reset
// Created: NLD 2003-01-14
//-----------------------------------------------------------------------------
void MIL_Fuseau::Reset()
{
    TER_Polygon::Reset();

    vStartGlobalDirection_.Reset();
    vEndGlobalDirection_  .Reset();

    pLeftLimit_.reset();
    pRightLimit_.reset();
    pMiddleLimit_.reset();
}

//=============================================================================
// TOOLS
//=============================================================================

// =============================================================================
// £$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ NLD - A NETTOYER
// =============================================================================

//-----------------------------------------------------------------------------
// Name: GetNextPoint
// Created: NLD 2003-04-22
//-----------------------------------------------------------------------------
template< typename Iterator >
void GetNextPoint( MT_Vector2D& vCur, Iterator itEnd, Iterator& itNext, double rStep ) // const
{
    if( itNext == itEnd )
        return;

    MT_Vector2D vNext = *itNext;

    MT_Vector2D vDir = vNext - vCur;
    double rDirLength = vDir.Magnitude();
    if( rDirLength )
        vDir /= rDirLength;

    for( ;; )
    {
        if( rStep < rDirLength )
        {
            vCur = vCur + ( vDir * rStep );
            return;
        }
        else
        {
            rStep -= rDirLength;
            vCur = vNext;
            ++itNext;
            if( itNext == itEnd )
                return;
            vNext = *itNext;
            vDir = vNext - vCur;
            rDirLength = vDir.Magnitude();
            if( rDirLength )
                vDir /= rDirLength;
        }
    }
}

//-----------------------------------------------------------------------------
// Name: GetPointOnLimitAfterIntersection
// Created: NLD 2003-08-22
//-----------------------------------------------------------------------------
MT_Vector2D GetPointOnLimitAfterIntersection( const T_PointVector& points, const MT_Vector2D& vIntersect, double rDistBefore ) // const
{
    if( points.empty() )
        throw MASA_EXCEPTION( "Error in GetPointOnLimitAfterIntersection" );
    CIT_PointVector itBegin = points.begin();
    CIT_PointVector itPoint = itBegin;

    const MT_Vector2D* pPrevPoint = &*itBegin;
    double rDist = 0;
    for( ++itPoint; itPoint != points.end(); ++itPoint )
    {
        const MT_Vector2D* pCurPoint = &*itPoint;
        MT_Line lineTmp( *pPrevPoint, *pCurPoint );

        if( lineTmp.IsInside( vIntersect, PRECISION ) )
        {
            rDist += pPrevPoint->Distance( vIntersect );
            break;
        }
        else
            rDist += pPrevPoint->Distance( *pCurPoint );

        pPrevPoint = pCurPoint;
    }

    ( rDist > rDistBefore ) ? rDist -= rDistBefore : rDist = 0.;

    MT_Vector2D vResult;
    vResult = *itBegin;
    GetNextPoint( vResult, points.end(), ++itBegin, rDist );
    return vResult;
}

// =============================================================================
// GEOMETRY
// =============================================================================

////////////////////////////////////////////////////////////////////////////////////////
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  A NETTOYER
////////////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::ComputeClosedTerrainRatio
// Created: NLD 2007-04-13
// -----------------------------------------------------------------------------
float MIL_Fuseau::ComputeClosedTerrainRatio() const
{
    unsigned int nForestSurface = 0, nEmptySurface  = 0, nUrbanSurface  = 0;
    MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetVisionObjectsInSurface( *this, nEmptySurface, nForestSurface, nUrbanSurface );
    return float( nForestSurface + nUrbanSurface ) / float( nForestSurface + nUrbanSurface + nEmptySurface );
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::ComputeOpenTerrainRatio
// Created: NLD 2007-04-13
// -----------------------------------------------------------------------------
float MIL_Fuseau::ComputeOpenTerrainRatio() const
{
    return 1.f - ComputeClosedTerrainRatio();
}

//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::Split
// Created: NLD 2003-04-22
//-----------------------------------------------------------------------------
bool MIL_Fuseau::Split( unsigned int nNbrSubFuseau, T_PointVectorVector& intermediateLimits ) const
{
    if( !pLeftLimit_ || !pRightLimit_ || nNbrSubFuseau < 1 )
        return false;

    // Create the new limits
    T_PointVector leftPointVectorTmp  = pLeftLimit_ ->GetPoints();
    T_PointVector rightPointVectorTmp = pRightLimit_->GetPoints();

    InsertPointProjection( pLeftLimit_ ->GetPoints(), rightPointVectorTmp );
    InsertPointProjection( pRightLimit_->GetPoints(), leftPointVectorTmp  );
    assert( leftPointVectorTmp.size() == rightPointVectorTmp.size() );

    intermediateLimits.clear();
    intermediateLimits.assign( nNbrSubFuseau - 1, T_PointVector() );
    for( unsigned int j = 0; j < leftPointVectorTmp.size(); ++j )
    {
        MT_Vector2D vTmp = rightPointVectorTmp[j] - leftPointVectorTmp[j];
        vTmp /= nNbrSubFuseau;

        unsigned int i = 1;
        for( IT_PointVectorVector it = intermediateLimits.begin(); it != intermediateLimits.end(); ++it, ++i )
        {
            (*it).reserve( leftPointVectorTmp.size() );
            (*it).push_back( leftPointVectorTmp[j] + vTmp * (double)(i) );
        }
    }

    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::SplitIntoSubFuseaux
// Created: NLD 2004-05-21
// -----------------------------------------------------------------------------
bool MIL_Fuseau::SplitIntoSubFuseaux( unsigned int nNbrSubFuseau, T_FuseauPtrList& container ) const
{
    container.clear();

    // Découpage
    T_PointVectorVector intermediateLimits;
    if( !Split( nNbrSubFuseau, intermediateLimits ) )
    {
        assert( intermediateLimits.empty() );
        return false;
    }

    // Creation des fuseaux
    assert( pLeftLimit_ && pRightLimit_ );
    if( !pLeftLimit_ || !pRightLimit_ )
        return false;
    const T_PointVector* pPrevLimit = &pLeftLimit_->GetPoints();
    for( auto it = intermediateLimits.begin(); it != intermediateLimits.end(); ++it )
    {
        const T_PointVector* pCurLimit = &*it;
        MIL_Fuseau* pNewSubFuseau = new MIL_Fuseau( *pPrevLimit, *pCurLimit );
        container.push_back( pNewSubFuseau );
        pPrevLimit = pCurLimit;
    }
    MIL_Fuseau* pNewSubFuseau = new MIL_Fuseau( *pPrevLimit, pRightLimit_->GetPoints() );
    container.push_back( pNewSubFuseau );

    assert( container.size() == nNbrSubFuseau );
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::ComputeFurthestExtremityPoint
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
bool MIL_Fuseau::ComputeFurthestExtremityPoint( MT_Vector2D& vResult ) const
{
    if( !pLeftLimit_ || !pRightLimit_ )
        return false;

    MT_Line lineFurthestBorder( *pLeftLimit_->GetPoints().rbegin(), *pRightLimit_->GetPoints().rbegin() );
    vResult = lineFurthestBorder.GetCenter();
    assert( IsInside( vResult ) );
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::ComputeClosestExtremityPoint
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
bool MIL_Fuseau::ComputeClosestExtremityPoint( MT_Vector2D& vResult ) const
{
    if( !pLeftLimit_ || !pRightLimit_ )
        return false;

    MT_Line lineClosestBorder( *pLeftLimit_->GetPoints().begin(), *pRightLimit_->GetPoints().begin() );
    vResult = lineClosestBorder.GetCenter();
    assert( IsInside( vResult ) );
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::ComputeNearestEntryPoint
// Created: NLD 2003-04-24
//-----------------------------------------------------------------------------
void MIL_Fuseau::ComputeNearestEntryPoint( const MT_Vector2D& vStartPos, MT_Vector2D& vResult ) const
{
    if( IsInside( vStartPos ) )
    {
        vResult = vStartPos;
        return;
    }

    assert( pLeftLimit_ && pRightLimit_ );
    CT_PointVector& borderVector = GetBorderPoints();

    double rSquareDist = std::numeric_limits<double>::max();

    const MT_Vector2D* pLastPos = 0;
    for( CIT_PointVector itPoint = borderVector.begin(); itPoint != borderVector.end(); ++itPoint )
    {
        const MT_Vector2D* pPos = &*itPoint;

        if( pLastPos )
        {
            MT_Line lineTmp( *pLastPos, *pPos );

            MT_Vector2D vTmp        = lineTmp.ClosestPointOnLine( vStartPos );
            double rSquareDistTmp = vStartPos.SquareDistance( vTmp );
            if( rSquareDistTmp < rSquareDist )
            {
                rSquareDist = rSquareDistTmp;
                vResult     = vTmp;
            }
        }
        pLastPos = pPos;
    }
    assert( IsInside( vResult ) );
}

namespace
{
//$$$ NLD - PUTAIN DE FONCTIONS !
bool GetPolyLineClosestPoint( const MT_Vector2D& vPos, const T_PointVector& vPoly, MT_Vector2D& vNearest, CIT_PointVector& itNext )
{
    double rDist = std::numeric_limits<double>::max();
    bool  bRes                      = false;
    const MT_Vector2D* pLastPos     = 0;
    for( CIT_PointVector itPoint = vPoly.begin(); itPoint != vPoly.end(); ++itPoint )
    {
        const MT_Vector2D* pPos = &*itPoint;
        if( pLastPos )
        {
            MT_Line lineTmp( *pLastPos, *pPos );
            MT_Vector2D vTmp        = lineTmp.ClosestPointOnLine( vPos );
            double rSquareDistTmp = vPos.SquareDistance( vTmp );
            if( rSquareDistTmp < rDist )
            {
                bRes        = true;
                rDist       = rSquareDistTmp;
                vNearest    = vTmp;
                itNext      = itPoint;
            }
        }
        pLastPos = pPos;
    }
    return bRes;
}
}

//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::ComputeEntryPoint
// Created: JDY 03-05-28
//-----------------------------------------------------------------------------
void MIL_Fuseau::ComputeEntryPoint(const MT_Vector2D& vPos, MT_Vector2D& vResult ) const
{
    if( IsInside( vPos ) )
    {
        // agent already in fuseau
        // return immediately
        vResult = vPos;
        return;
    }

    assert( pLeftLimit_ && pRightLimit_ );
    if( !pLeftLimit_ || !pRightLimit_ )
        throw MASA_EXCEPTION( "Null fuseau in ComputeEntryPoint" );

    const T_PointVector* pOwnLimit   = 0;
    const T_PointVector* pOtherLimit = 0;
    MT_Line*       pExtremity  = 0;
    MT_Vector2D    vNearest;
    static const double rNbSteps = MIL_Tools::ConvertMeterToSim( nNbrMeterPerSample_ );
    double rOwnStep   = 0.0;
    double rOtherStep = 0.0;

    // check nearest point with left border
    IT_PointVector itAfterRefPointProjection;
    if( GetPolyLineClosestPoint( vPos, pLeftLimit_->GetPoints(), vNearest, itAfterRefPointProjection ) )
    {
        pOwnLimit    = &pLeftLimit_ ->GetPoints();
        pOtherLimit  = &pRightLimit_->GetPoints();
        rOwnStep     = pLeftLimit_ ->GetLength() / rNbSteps;
        rOtherStep   = pRightLimit_->GetLength() / rNbSteps;
    }

    // check nearest point with right border
    if( GetPolyLineClosestPoint( vPos, pRightLimit_->GetPoints(), vNearest, itAfterRefPointProjection ) )
    {
        pOwnLimit    = &pRightLimit_->GetPoints();
        pOtherLimit  = &pLeftLimit_ ->GetPoints();
        rOwnStep     = pRightLimit_->GetLength() / rNbSteps;
        rOtherStep   = pLeftLimit_ ->GetLength() / rNbSteps;
    }

    double rSquareDist = vPos.SquareDistance( vNearest );

    // check extremity segments

    // left
    MT_Line vLeftSeg( *pLeftLimit_->GetPoints().begin(), *pRightLimit_->GetPoints().begin() );
    MT_Vector2D vTmp        = vLeftSeg.ClosestPointOnLine( vPos );
    double rSquareDistTmp = vPos.SquareDistance( vTmp );
    if( rSquareDistTmp <= rSquareDist)
    {
        rSquareDist = rSquareDistTmp;
        vNearest    = vTmp;
        pExtremity  = &vLeftSeg;
    }

    // right
    MT_Line vRightSeg( *pLeftLimit_->GetPoints().rbegin(), *pRightLimit_->GetPoints().rbegin() );
    vTmp           = vRightSeg.ClosestPointOnLine( vPos );
    rSquareDistTmp = vPos.SquareDistance( vTmp );
    if( rSquareDistTmp <= rSquareDist)
    {
        rSquareDist = rSquareDistTmp;
        vNearest    = vTmp;
        pExtremity  = &vRightSeg;
    }

    // nearest point is on an extremity
    if (pExtremity)
    {
        // return middle of this extremity
        vResult=pExtremity->GetCenter();
        return;
    }

    // general case - nearest point is on a limit
    if( !pOwnLimit || !pOtherLimit )
        throw MASA_EXCEPTION( "Invalid own or other limit" );
    CIT_PointVector itOwnPointNext   = pOwnLimit->begin();
    CIT_PointVector itOtherPointNext = pOtherLimit->begin();
    MT_Vector2D     vCurOwnPoint     = *itOwnPointNext;
    MT_Vector2D     vCurOtherPoint   = *itOtherPointNext;

    ++itOwnPointNext;
    ++itOtherPointNext;
    for( unsigned int j = 0; j < rNbSteps; ++j )
    {
        MT_Vector2D vLastOwnPoint  =vCurOwnPoint;
        MT_Vector2D vLastOtherPoint=vCurOtherPoint;
        GetNextPoint( vCurOwnPoint  , pOwnLimit  ->end(), itOwnPointNext , rOwnStep  );
        GetNextPoint( vCurOtherPoint, pOtherLimit->end(), itOtherPointNext, rOtherStep );
        MT_Vector2D vDir=((vCurOwnPoint-vLastOwnPoint).Normalize()+(vCurOtherPoint-vLastOtherPoint).Normalize())/2;
        if( MT_Line(vLastOwnPoint,vCurOwnPoint).IsInside(vNearest,PRECISION) )
        {
            vResult=(vLastOwnPoint+vLastOtherPoint)/2+vDir*vNearest.Distance(vLastOwnPoint);
            return;
        }
    }
    vResult=vNearest;
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::ComputePointBeforeLima
// Created: NLD 2003-08-22
// -----------------------------------------------------------------------------
bool MIL_Fuseau::ComputePointBeforeLima( const MIL_LimaOrder& lima, double rDistBefore, MT_Vector2D& vResult ) const
{
    T_PointVector result;
    ComputePointsBeforeLima( lima, rDistBefore, 1, result );

    if( result.size() == 1 )
    {
        vResult = result.front();
        return true;
    }

    assert( result.empty() );
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::ComputePointsBeforeLima
// Created: NLD 2003-08-22
// -----------------------------------------------------------------------------
bool MIL_Fuseau::ComputePointsBeforeLima( const MIL_LimaOrder& lima, double rDistBefore, unsigned int nNbPoints, T_PointVector& results ) const
{
    results.clear();

    if( !pLeftLimit_ || !pRightLimit_ || lima.GetPoints().empty() )
        return false;

    results.reserve( nNbPoints );

    // Calcul de la direction
    TER_DistanceLess cmpLeft ( *pLeftLimit_ ->GetPoints().begin () );
    TER_DistanceLess cmpRight( *pRightLimit_->GetPoints().begin() );

    T_PointSet leftIntersectionSet ( cmpLeft  );
    T_PointSet rightIntersectionSet( cmpRight );

    lima.Intersect2D( pLeftLimit_ ->GetPoints(), leftIntersectionSet  );
    lima.Intersect2D( pRightLimit_->GetPoints(), rightIntersectionSet );

    if( rightIntersectionSet.empty() || leftIntersectionSet.empty() )
        return false;

    const MT_Vector2D& vLeftIntersection  = *leftIntersectionSet .begin();
    const MT_Vector2D& vRightIntersection = *rightIntersectionSet.begin();

    MT_Vector2D vLeftPointBefore  = GetPointOnLimitAfterIntersection( pLeftLimit_ ->GetPoints(), vLeftIntersection , rDistBefore );
    MT_Vector2D vRightPointBefore = GetPointOnLimitAfterIntersection( pRightLimit_->GetPoints(), vRightIntersection, rDistBefore );

    MT_Vector2D vTranslation = ( ( vLeftPointBefore - vLeftIntersection ) + ( vRightPointBefore - vRightIntersection ) ) / 2.;

    // calcul du support
    MT_Polyline supportLine( lima.GetPoints() );
    supportLine.Translate( vTranslation );

    TER_Polygon::Intersection( supportLine, PRECISION );

    // positionnement des points
    const double rDist = supportLine.Magnitude() / nNbPoints;
    if( rDist == 0. )
        return false;

    for( double rCurrentDist = rDist * 0.5; nNbPoints--; rCurrentDist += rDist )
        results.push_back( supportLine.GetPointAt( rCurrentDist ) );

    lima.ReplacePointsByNearestObjectPositions( results );

    return true;
}

namespace
{
    double ComputeDistance( const T_PointVector& points, const CIT_PointVector& itNextRefPos, const MT_Vector2D& refPos )
    {
        assert( itNextRefPos != points.begin() );
        double rDist = 0.;
        CIT_PointVector itPrev = points.begin();
        CIT_PointVector itCur = itPrev;
        ++itCur;
        for( ; itCur != itNextRefPos; ++itCur )
        {
            rDist += itPrev->Distance( *itCur );
            itPrev = itCur;
        }
        rDist += itPrev->Distance( refPos );
        return rDist;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::ComputeAverageDistanceFromLima
// Created: NLD 2007-04-29
// -----------------------------------------------------------------------------
double MIL_Fuseau::ComputeAverageDistanceFromLima( const MIL_LimaOrder& lima, const MT_Vector2D& refPoint ) const
{
    if( !pMiddleLimit_ )
        throw MASA_EXCEPTION( "No middle limit in ComputeAverageDistanceFromLima" );

    // projection de refPoint sur middle limit
    MT_Vector2D    refPointProjection;
    IT_PointVector itAfterRefPointProjection;
    if( !GetPolyLineClosestPoint( refPoint, pMiddleLimit_->GetPoints(), refPointProjection, itAfterRefPointProjection ) )
        return 0.;

    // point intersection entre lima et middle limit
    MT_Vector2D    limaPoint;
    IT_PointVector itAfterLimaPoint;
    if( !ComputeIntersectionWithLima( pMiddleLimit_->GetPoints(), lima, limaPoint, itAfterLimaPoint ) )
        return 0.;

    // Calcul distance
    const double rDist1 = ComputeDistance( pMiddleLimit_->GetPoints(), itAfterRefPointProjection, refPointProjection );
    const double rDist2 = ComputeDistance( pMiddleLimit_->GetPoints(), itAfterLimaPoint         , limaPoint          );

    if( rDist1 > rDist2 )
        return 0;
    return rDist2 - rDist1;
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::ComputeAverageDistanceFromObjective
// Created: NLD 2007-04-29
// -----------------------------------------------------------------------------
double MIL_Fuseau::ComputeAverageDistanceFromObjective( const DEC_Objective& objective, const MT_Vector2D& refPoint ) const
{
    if( !pMiddleLimit_ )
        throw MASA_EXCEPTION( "No middle limit in ComputeAverageDistanceFromObjective" );

    // projection de refPoint sur middle limit
    MT_Vector2D    refPointProjection;
    IT_PointVector itAfterRefPointProjection;
    if( !GetPolyLineClosestPoint( refPoint, pMiddleLimit_->GetPoints(), refPointProjection, itAfterRefPointProjection ) )
        return 0.;

    // projection de barycentre de l'objectif sur middle limit
    MT_Vector2D    objectiveProjection;
    IT_PointVector itAfterObjectiveProjection;
    if( !GetPolyLineClosestPoint( objective.ComputerBarycenter(), pMiddleLimit_->GetPoints(), objectiveProjection, itAfterObjectiveProjection ) )
        return 0.;

    // Calcul distance
    const double rDist1 = ComputeDistance( pMiddleLimit_->GetPoints(), itAfterRefPointProjection , refPointProjection  );
    const double rDist2 = ComputeDistance( pMiddleLimit_->GetPoints(), itAfterObjectiveProjection, objectiveProjection );

    if( rDist1 > rDist2 )
        return 0;
    return rDist2 - rDist1;
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::ComputeAdvance
// Created: LDC 2010-11-26
// -----------------------------------------------------------------------------
double MIL_Fuseau::ComputeAdvance( const MT_Vector2D& position ) const
{
    double result = 0.;
    if( pMiddleLimit_ )
    {
        const T_PointVector& points = pMiddleLimit_->GetPoints();

        double currentAdvance  = 0.;
        double totalDistances = 0.;
        MT_Vector2D startPoint = points.front();
        MT_Vector2D nearestPoint;
        std::vector< double > advances;
        std::vector< double > distances;
        for( auto it = points.begin(); ++it != points.end(); )
        {
            MT_Vector2D firstPoint = startPoint;
            MT_Line line( firstPoint, *it );
            startPoint = *it;
            double advance = line.ProjectPointOnLine( position, nearestPoint );
            double distance = position.Distance( nearestPoint );
            advance *= line.Magnitude();
            advance += currentAdvance;
            advances.push_back( advance );
            distances.push_back( distance );
            totalDistances += distance;
            currentAdvance += line.Magnitude();
        }
        std::size_t length = advances.size();
        if( totalDistances == 0. )
            totalDistances = 1.;
        bool projectionMustBeWeighted = false;
        for( std::size_t i = 0; i < length; ++i )
            if( totalDistances != distances[i] * length )
            {
                projectionMustBeWeighted = true;
                break;
            }
        if( projectionMustBeWeighted )
        {
            for( unsigned int i = 0; i < length; ++i )
                result += advances[i] * (totalDistances - distances[i])/totalDistances;
            result /= (length-1);
        }
        else if( length )
            result = advances[0];
    }
    return result;
}

MT_Vector2D MIL_Fuseau::GetPositionAtAdvance( double advance ) const
{
    MT_Vector2D result;
    if( !pMiddleLimit_ )
        throw MASA_EXCEPTION( "Unable to compute point on middle fuseau" );
    const T_PointVector& points = pMiddleLimit_->GetPoints();
    MT_Vector2D firstPoint = points.front();
    result = firstPoint;
    if( advance <= 0 )
        return result;
    CIT_PointVector it = points.begin();
    ++it;
    for( ; ++it != points.end(); )
    {
        advance -= firstPoint.Distance( *it );
        if( advance <= 0 )
        {
            return *it + ( *it - firstPoint ).Normalize() * advance ;
        }
        firstPoint = *it;
    }
    return firstPoint;
}

// =============================================================================
// PATH FINDER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::operator=
// Created: NLD 2003-10-24
// -----------------------------------------------------------------------------
MIL_Fuseau& MIL_Fuseau::operator=( const MIL_Fuseau& fuseau )
{
    TER_Polygon::operator=( fuseau );

    //globalDirectionLine_    = fuseau.globalDirectionLine_; $$ NE PAS COPIER - MT_LINE SUCKS
    vStartGlobalDirection_  = fuseau.vStartGlobalDirection_;
    vEndGlobalDirection_    = fuseau.vEndGlobalDirection_;

    pLeftLimit_   = fuseau.pLeftLimit_;
    pRightLimit_  = fuseau.pRightLimit_;
    pMiddleLimit_ = fuseau.pMiddleLimit_;

    return *this;
}

// =============================================================================
// PATH FIND
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::Distance
// Created: AGE 2005-05-13
// -----------------------------------------------------------------------------
double MIL_Fuseau::Distance( const MT_Vector2D& p, bool bLimitsOnly ) const
{
    if( ! pLeftLimit_ || ! pRightLimit_
       || ( !bLimitsOnly && IsInside( p ) ) )
        return 0;

    MT_Line first( pLeftLimit_->GetPoints().front(), pRightLimit_->GetPoints().front() );
    const double rFirstDistance = first.ClosestPointOnLine( p ).Distance( p );
    MT_Line last( pLeftLimit_->GetPoints().back(), pRightLimit_->GetPoints().back() );
    const double rLastDistance = last.ClosestPointOnLine( p ).Distance( p );

    return std::min( std::min( rFirstDistance, rLastDistance ),
                     std::min( pLeftLimit_->Distance( p ), pRightLimit_->Distance( p ) ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::GetCost
// Created: AGE 2005-03-23
// -----------------------------------------------------------------------------
double MIL_Fuseau::GetCost( const MT_Vector2D&, const MT_Vector2D& to, double rMaxDistanceOut, double rCostPerMeterOut, double rComfortDistanceIn, double rCostPerMeterIn ) const
{
    if( !pLeftLimit_ )
        return 0;

    const bool     bInside      = IsInsidish( to );
    const double rMinDistance = Distance( to, true );

    if( bInside )
    {
        if( rMinDistance > rComfortDistanceIn )
            return 0;
        return ( rComfortDistanceIn - rMinDistance ) * rCostPerMeterIn;
    }
    else
    {
        if( rMinDistance > rMaxDistanceOut )
            return -1;
        return rMinDistance * rCostPerMeterOut;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::IsLeftFlank
// Created: SBO 2007-12-06
// -----------------------------------------------------------------------------
bool MIL_Fuseau::IsLeftFlank( const MIL_Fuseau& fuseau ) const
{
    return pLeftLimit_ && fuseau.pLeftLimit_ == pLeftLimit_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::IsRightFlank
// Created: SBO 2007-12-06
// -----------------------------------------------------------------------------
bool MIL_Fuseau::IsRightFlank( const MIL_Fuseau& fuseau ) const
{
    return pRightLimit_ && fuseau.pRightLimit_ == pRightLimit_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::IsOnFlank
// Created: SBO 2007-12-06
// -----------------------------------------------------------------------------
bool MIL_Fuseau::IsOnFlank( const MT_Vector2D& position, bool left, bool right ) const
{
    if( !pLeftLimit_ || !pRightLimit_ )
        return false;
    if( left && pLeftLimit_->Distance( position ) < pRightLimit_->Distance( position ) )
        return true;
    return right && pRightLimit_->Distance( position ) < pLeftLimit_->Distance( position );
}

//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::GetLeftLimit
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
const T_PointVector& MIL_Fuseau::GetLeftLimit() const
{
    return pLeftLimit_->GetPoints();
}

//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::GetRightLimit
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
const T_PointVector& MIL_Fuseau::GetRightLimit() const
{
    return pRightLimit_->GetPoints();
}

//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::GetMiddleLimit
// Created: NMI 2013-11-14
//-----------------------------------------------------------------------------
const T_PointVector& MIL_Fuseau::GetMiddleLimit() const
{
    return pMiddleLimit_->GetPoints();
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::GetGlobalDirection
// Created: NLD 2003-09-29
// -----------------------------------------------------------------------------
const MT_Line& MIL_Fuseau::GetGlobalDirection() const
{
    return globalDirectionLine_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::GetWidth
// Created: DDA 2010-12-03
// -----------------------------------------------------------------------------
double MIL_Fuseau::GetWidth() const
{
    if( IsNull() )
        throw MASA_EXCEPTION( "Null fuseau in GetWidth" );
     const T_PointVector& leftLimitTmp  = pLeftLimit_ ->GetPoints();
     const T_PointVector& rightLimitTmp = pRightLimit_->GetPoints();
     return MT_Line( leftLimitTmp[0], rightLimitTmp[0] ).Magnitude();
}

//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::IsNull
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
bool MIL_Fuseau::IsNull() const
{
    return !pLeftLimit_ || !pRightLimit_;
}

//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::IsInside
// Created: NLD 2003-07-27
//-----------------------------------------------------------------------------
bool MIL_Fuseau::IsInside( const MT_Vector2D& vPos  ) const
{
    return TER_Polygon::IsInside( vPos, 0.1 ); //$$$
}

//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::IntersectWithBorder
// Created: NLD 2003-07-27
//-----------------------------------------------------------------------------
bool MIL_Fuseau::IntersectWithBorder( const MT_Line& line ) const
{
    return TER_Polygon::IntersectWithBorder( line, 0.1 ); //$$$$$
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::IntersectWithBorder
// Created: NLD 2003-10-15
// -----------------------------------------------------------------------------
bool MIL_Fuseau::IntersectWithBorder( const MT_Line& line,T_PointSet& res ) const
{
    return TER_Polygon::IntersectWithBorder( line, res, 0.1 ); //$$$$
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::operator==
// Created: NLD 2005-07-08
// -----------------------------------------------------------------------------
bool MIL_Fuseau::operator==( const MIL_Fuseau& rhs ) const
{
    return (   pLeftLimit_ == rhs.pLeftLimit_
            && pRightLimit_ == rhs.pRightLimit_
            && pMiddleLimit_ == rhs.pMiddleLimit_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::operator!=
// Created: NLD 2005-07-08
// -----------------------------------------------------------------------------
bool MIL_Fuseau::operator!=( const MIL_Fuseau& rhs ) const
{
    return !this->operator==( rhs );
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::load
// Created: LGY 2011-06-07
// -----------------------------------------------------------------------------
void MIL_Fuseau::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    T_PointVector pLeftLimit;
    T_PointVector pRightLimit;
    {
        std::size_t nNbr;
        file >> nNbr;
        while( nNbr-- )
        {
            double x;
            double y;
            file >> x;
            file >> y;
            pLeftLimit.push_back( MT_Vector2D( x, y ) );
        }
    }
    {
        std::size_t nNbr2;
        file >> nNbr2;
        while( nNbr2-- )
        {
            double x;
            double y;
            file >> x;
            file >> y;
            pRightLimit.push_back( MT_Vector2D( x, y ) );
        }
    }
    file >> vStartGlobalDirection_;
    file >> vEndGlobalDirection_;
    globalDirectionLine_ = MT_Line( vStartGlobalDirection_, vEndGlobalDirection_ );
    if( !pLeftLimit.empty() && !pRightLimit.empty() )
    {
        pLeftLimit_ = CreateLimit( pLeftLimit );
        pRightLimit_ = CreateLimit( pRightLimit );
        InitializeMiddleLimit();
        InitializePolygon();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::save
// Created: LGY 2011-06-07
// -----------------------------------------------------------------------------
void MIL_Fuseau::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    T_PointVector pLeftLimit;
    T_PointVector pRightLimit;
    if( pLeftLimit_ )
        pLeftLimit = const_cast< T_PointVector& >( pLeftLimit_->GetPoints() );
    if( pRightLimit_ )
        pRightLimit = const_cast< T_PointVector& >( pRightLimit_->GetPoints() );

    std::size_t size = pLeftLimit.size();
    file << size;
    for( IT_PointVector it = pLeftLimit.begin(); it != pLeftLimit.end(); ++it )
    {
        file << it->rX_;
        file << it->rY_;
    }

    std::size_t size2 = pRightLimit.size();
    file << size2;
    for( IT_PointVector it = pRightLimit.begin(); it != pRightLimit.end(); ++it )
    {
        file << it->rX_;
        file << it->rY_;
    }

    file << vStartGlobalDirection_;
    file << vEndGlobalDirection_;
}

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
#include "TER_LimitData.h"
#include "MIL_TacticalLineManager.h"

#include "MIL_AgentServer.h"
#include "Tools/MIL_Tools.h"

#include "simulation_terrain/TER_PathFindManager.h"
#include "simulation_terrain/TER_DynamicData.h"
#include "MT_Tools/MT_Polyline.h"

uint MIL_Fuseau::nNbrMeterPerSample_ = 400; //$$$ A GICLER

#define PRECISION 0.1 //$$ CRADE

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau constructor
// Created: NLD 2004-05-21
// -----------------------------------------------------------------------------
MIL_Fuseau::MIL_Fuseau( const MT_Vector2D& vOrientationRefPos, const T_PointVector& leftLimit, const T_PointVector& rightLimit, const MIL_LimaOrder* pBeginMissionLima, const MIL_LimaOrder* pEndMissionLima )
    : TER_Polygon           ()
    , pLeftLimit_           ( 0 )
    , pRightLimit_          ( 0 )
    , pMiddleLimit_         ( 0 )
    , vOrientationRefPos_   ()
    , vStartGlobalDirection_()
    , vEndGlobalDirection_  ()
    , globalDirectionLine_  ( vStartGlobalDirection_, vEndGlobalDirection_ )
{
    Reset( vOrientationRefPos, leftLimit, rightLimit, pBeginMissionLima, pEndMissionLima );
}
   
//-----------------------------------------------------------------------------
// Name: MIL_Fuseau constructor
// Created: NLD 2003-01-14
//-----------------------------------------------------------------------------
MIL_Fuseau::MIL_Fuseau()
    : TER_Polygon           ()
    , pLeftLimit_           ( 0 )
    , pRightLimit_          ( 0 )
    , pMiddleLimit_         ( 0 )
    , vOrientationRefPos_   ()
    , vStartGlobalDirection_()
    , vEndGlobalDirection_  ()
    , globalDirectionLine_( vStartGlobalDirection_, vEndGlobalDirection_ )
{
    Reset();
}

//-----------------------------------------------------------------------------
// Name: MIL_Fuseau destructor
// Created: NLD 2003-01-14
//-----------------------------------------------------------------------------
MIL_Fuseau::~MIL_Fuseau()
{
    Reset();
}

//=============================================================================
// INIT
//=============================================================================

//$$$ FONCTIONS DE MERDE !!

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
        if(     droite.Intersect2D( lineTmp, vIntersectTmp ) == eDoIntersect 
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
            pointVector.insert( pointVector.begin(), pointVector.front() );
        else
            pointVector.insert( pointVector.end(), pointVector.back() );
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
    T_PointPtrVector pointsVector;
    pointsVector.reserve( leftPoints.size() + rightPoints.size() + 1 );
    for( IT_PointVector itPoint = leftPoints.begin(); itPoint != leftPoints.end(); ++itPoint )
        pointsVector.push_back( &*itPoint );
    for( RIT_PointVector ritPoint = rightPoints.rbegin(); ritPoint != rightPoints.rend(); ++ritPoint )
        pointsVector.push_back( &*ritPoint );
    pointsVector.push_back( &*leftPoints.begin() );

    TER_Polygon polygon;
    polygon.Reset( pointsVector );
    return polygon.IsInside( vPoint, 0.1 ); ///$$$$
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::TruncateAndOrienteLimits
// Created: NLD 2004-05-19
// -----------------------------------------------------------------------------
void MIL_Fuseau::TruncateAndReorientLimits( T_PointVector& leftLimit, T_PointVector& rightLimit, const MIL_LimaOrder* pBeginMissionLima, const MIL_LimaOrder* pEndMissionLima )
{
    // The two limits must have the same orientation
    MT_Line lineBorder1( *leftLimit.begin (), *rightLimit.begin() );
    MT_Line lineBorder2( *leftLimit.rbegin(), *rightLimit.rbegin() );
    if( lineBorder1.Intersect2D( lineBorder2 ) )
        std::reverse( rightLimit.begin(), rightLimit.end() );

    // The orientation cannot be determined from the 'LDM' and the 'LFM'
    //  => orient it regarding the 'orientation reference position' (usually the 
    //     position of unit owning the fuseau

    // Orientation = From the closest border to the furthest
    if( vOrientationRefPos_.SquareDistance( lineBorder1.GetCenter() ) > vOrientationRefPos_.SquareDistance( lineBorder2.GetCenter() ) )
    {
        std::reverse( rightLimit.begin(), rightLimit.end() );
        std::reverse( leftLimit.begin(), leftLimit.end() );
    }    

    T_PointVectorVector leftParts;
    T_PointVectorVector rightParts;

    SplitLimit( pBeginMissionLima, pEndMissionLima, leftLimit , leftParts  );
    SplitLimit( pBeginMissionLima, pEndMissionLima, rightLimit, rightParts );
 
    assert( leftParts.size() == rightParts.size() );
    assert( !leftParts.empty() );
    assert( leftParts.size() <= 2 );

    uint nNbParts = leftParts.size();
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
        if( IsPointInsidePolygon( leftParts[1], rightParts[1], vOrientationRefPos_ ) )
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
    assert( pLeftLimit_ && pRightLimit_ );
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
    assert( pLeftLimit_ );
    assert( pRightLimit_ );

    T_PointVector leftPointVectorTmp  = pLeftLimit_ ->GetPoints();
    T_PointVector rightPointVectorTmp = pRightLimit_->GetPoints();

    InsertPointProjection( pLeftLimit_ ->GetPoints(), rightPointVectorTmp );
    InsertPointProjection( pRightLimit_->GetPoints(), leftPointVectorTmp  );
    assert( leftPointVectorTmp.size() == rightPointVectorTmp.size() );
    assert( !leftPointVectorTmp.empty() );
    ///
    T_PointVector middle; 
    middle.reserve( leftPointVectorTmp.size() );
    for( uint j = 0; j < leftPointVectorTmp.size(); ++j )
        middle.push_back( leftPointVectorTmp[j] + ( rightPointVectorTmp[j] - leftPointVectorTmp[j] ) / 2 );

    assert( !pMiddleLimit_ );
    pMiddleLimit_ = &MIL_AgentServer::GetWorkspace().GetTacticalLineManager().CreateLimitData( middle );
    pMiddleLimit_ ->AddRef( *this );
}

//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::Reset
// Created: NLD 2003-01-14
//-----------------------------------------------------------------------------
void MIL_Fuseau::Reset( const MT_Vector2D& vOrientationRefPos, const T_PointVector& leftLimit, const T_PointVector& rightLimit, const MIL_LimaOrder* pBeginMissionLima, const MIL_LimaOrder* pEndMissionLima )
{
    assert( !leftLimit .empty() );
    assert( !rightLimit.empty() );

    MIL_Fuseau::Reset();

    vOrientationRefPos_ = vOrientationRefPos;
    if( !pBeginMissionLima || !pEndMissionLima )
        pBeginMissionLima = pEndMissionLima = 0;

    T_PointVector leftLimitTmp  = leftLimit;
    T_PointVector rightLimitTmp = rightLimit;

    TruncateAndReorientLimits( leftLimitTmp, rightLimitTmp, pBeginMissionLima, pEndMissionLima  );

    pLeftLimit_  = &MIL_AgentServer::GetWorkspace().GetTacticalLineManager().CreateLimitData( leftLimitTmp  );
    pRightLimit_ = &MIL_AgentServer::GetWorkspace().GetTacticalLineManager().CreateLimitData( rightLimitTmp );
    pLeftLimit_ ->AddRef( *this );   
    pRightLimit_->AddRef( *this );

    InitializeMiddleLimit();
    InitializePolygon    ();
    
    // Fuseau global orientation
    vStartGlobalDirection_ = MT_Line( *pLeftLimit_->GetPoints().begin (), *pRightLimit_->GetPoints().begin () ).GetCenter();
    vEndGlobalDirection_   = MT_Line( *pLeftLimit_->GetPoints().rbegin(), *pRightLimit_->GetPoints().rbegin() ).GetCenter();
    
    // DEBUG
    /*printf( "MIL_Fuseau::Reset BEGIN\n" );
    printf("\tLEFT\n");
    for( IT_PointVector itPoint = leftPointVector_.begin(); itPoint != leftPointVector_.end(); ++itPoint )
        printf("\t\t %.2f\n", itPoint->rX_, itPoint->rY_ );

    printf("\tRIGHT\n");
    for( itPoint = rightPointVector_.begin(); itPoint != rightPointVector_.end(); ++itPoint )
        printf("\t\t %.2f\n", itPoint->rX_, itPoint->rY_ );
    printf( "MIL_Fuseau::Reset END\n" );*/
    // DEBUG 
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
    vOrientationRefPos_   .Reset();

    if( pLeftLimit_ )
        pLeftLimit_->DecRef( *this );
    if( pRightLimit_ )
        pRightLimit_->DecRef( *this );
    if( pMiddleLimit_ )
        pMiddleLimit_->DecRef( *this );

    pLeftLimit_   = 0;
    pRightLimit_  = 0;
    pMiddleLimit_ = 0;
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
void GetNextPoint( MT_Vector2D& vCur, Iterator itEnd, Iterator& itNext, MT_Float rStep ) // const
{
    if( itNext == itEnd )
        return;

    MT_Vector2D vNext = *itNext;

    MT_Vector2D vDir = vNext - vCur;
    MT_Float rDirLength = vDir.Magnitude();
    if( rDirLength )
        vDir /= rDirLength;

    while( 1 )
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
MT_Vector2D GetPointOnLimitAfterIntersection( const T_PointVector& points, const MT_Vector2D& vIntersect, MT_Float rDistBefore ) // const
{
    CIT_PointVector itBegin = points.begin();
    CIT_PointVector itPoint = itBegin;

    const MT_Vector2D* pPrevPoint = &*itBegin;
    MT_Float rDist = 0;
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

//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::Split
// Created: NLD 2003-04-22
//-----------------------------------------------------------------------------
bool MIL_Fuseau::Split( uint nNbrSubFuseau, T_PointVectorVector& intermediateLimits ) const
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
    for( uint j = 0; j < leftPointVectorTmp.size(); ++j )
    {
        MT_Vector2D vTmp = rightPointVectorTmp[j] - leftPointVectorTmp[j];
        vTmp /= nNbrSubFuseau;
    
        uint i = 1;
        for( IT_PointVectorVector it = intermediateLimits.begin(); it != intermediateLimits.end(); ++it, ++i )
        {
            (*it).reserve( leftPointVectorTmp.size() );
            (*it).push_back( leftPointVectorTmp[j] + vTmp * (MT_Float)(i) );
        }
    }

    // Limits creation
//    limitVector.reserve( 2 + limitsPoints.size() );
//    limitVector.push_back( pLeftLimit_ );
//    for( CIT_PointVectorVector it = limitsPoints.begin(); it != limitsPoints.end(); ++it )
//        limitVector.push_back( &MIL_AgentServer::GetWorkspace().GetTacticalLineManager().CreateLimitData( *it ) );
//    limitVector.push_back( pRightLimit_ );
    return true;         
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::SplitIntoSubFuseaux
// Created: NLD 2004-05-21
// -----------------------------------------------------------------------------
bool MIL_Fuseau::SplitIntoSubFuseaux( uint nNbrSubFuseau, T_FuseauPtrList& container ) const
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
    const T_PointVector* pPrevLimit = &pLeftLimit_->GetPoints();
    for( CIT_PointVectorVector it = intermediateLimits.begin(); it != intermediateLimits.end(); ++it )
    {
        const T_PointVector* pCurLimit = &*it;
        MIL_Fuseau* pNewSubFuseau = new MIL_Fuseau( vOrientationRefPos_, *pPrevLimit, *pCurLimit );
        container.push_back( pNewSubFuseau );
        pPrevLimit = pCurLimit;
    }
    MIL_Fuseau* pNewSubFuseau = new MIL_Fuseau( vOrientationRefPos_, *pPrevLimit, pRightLimit_->GetPoints() );
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
    if( !pLeftLimit_ || pRightLimit_ )
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
    if( !pLeftLimit_ || pRightLimit_ )
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

    MT_Float rSquareDist = std::numeric_limits<MT_Float>::max();  

    const MT_Vector2D* pLastPos = 0;
    for( CIT_PointVector itPoint = borderVector.begin(); itPoint != borderVector.end(); ++itPoint )
    {
        const MT_Vector2D* pPos = &*itPoint;

        if( pLastPos )
        {
            MT_Line lineTmp( *pLastPos, *pPos );
            
            MT_Vector2D vTmp        = lineTmp.ClosestPointOnLine( vStartPos );
            MT_Float rSquareDistTmp = vStartPos.SquareDistance( vTmp );
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



//$$$ NLD - PUTAIN DE FONCTIONS !
inline bool GetPolyLineClosestPoint(const MT_Vector2D& vPos,const T_PointVector& vPoly,MT_Vector2D& vNearest,MT_Float& rDist)
{
    bool  bRes                      = false;
    const MT_Vector2D* pLastPos     = 0;
    for( CIT_PointVector itPoint = vPoly.begin(); itPoint != vPoly.end(); ++itPoint )
    {
        const MT_Vector2D* pPos = &*itPoint;
        if( pLastPos )
        {
            MT_Line lineTmp( *pLastPos, *pPos );
            MT_Vector2D vTmp        = lineTmp.ClosestPointOnLine( vPos );
            MT_Float rSquareDistTmp = vPos.SquareDistance( vTmp );
            if( rSquareDistTmp < rDist )
            {
                bRes        = true;
                rDist       = rSquareDistTmp;
                vNearest    = vTmp;
            }
        }
        pLastPos = pPos;
    }
    return bRes;
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
    
    const T_PointVector* pOwnLimit   = 0;
    const T_PointVector* pOtherLimit = 0;
    MT_Line*       pExtremity  = 0;
    MT_Vector2D    vNearest;
    MT_Float       rSquareDist = std::numeric_limits<MT_Float>::max();
    static const MT_Float rNbSteps = MIL_Tools::ConvertMeterToSim( nNbrMeterPerSample_ );
    MT_Float rOwnStep   = 0.0;
    MT_Float rOtherStep = 0.0;

    // check nearest point with left border
    if ( GetPolyLineClosestPoint( vPos, pLeftLimit_->GetPoints(), vNearest, rSquareDist ) )
    {
        pOwnLimit    = &pLeftLimit_ ->GetPoints();
        pOtherLimit  = &pRightLimit_->GetPoints();
        rOwnStep     = pLeftLimit_ ->GetLength() / rNbSteps;
        rOtherStep   = pRightLimit_->GetLength() / rNbSteps;
    }

    // check nearest point with right border
    if ( GetPolyLineClosestPoint( vPos, pRightLimit_->GetPoints(), vNearest, rSquareDist ) )
    {
        pOwnLimit    = &pRightLimit_->GetPoints();
        pOtherLimit  = &pLeftLimit_ ->GetPoints();
        rOwnStep     = pRightLimit_->GetLength() / rNbSteps;
        rOtherStep   = pLeftLimit_ ->GetLength() / rNbSteps;
    }

    // check extremity segments
    
    // left
    MT_Line vLeftSeg( *pLeftLimit_->GetPoints().begin(), *pRightLimit_->GetPoints().begin() );
    MT_Vector2D vTmp        = vLeftSeg.ClosestPointOnLine( vPos );
    MT_Float rSquareDistTmp = vPos.SquareDistance( vTmp );
    if ( rSquareDistTmp <= rSquareDist)
    {
        rSquareDist = rSquareDistTmp;
        vNearest    = vTmp;
        pExtremity  = &vLeftSeg;
    }

    // right
    MT_Line vRightSeg( *pLeftLimit_->GetPoints().rbegin(), *pRightLimit_->GetPoints().rbegin() );
    vTmp           = vRightSeg.ClosestPointOnLine( vPos );
    rSquareDistTmp = vPos.SquareDistance( vTmp );
    if ( rSquareDistTmp <= rSquareDist)
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
    CIT_PointVector itOwnPointNext   = pOwnLimit->begin();
    CIT_PointVector itOtherPointNext = pOtherLimit->begin();
    MT_Vector2D     vCurOwnPoint     = *itOwnPointNext;
    MT_Vector2D     vCurOtherPoint   = *itOtherPointNext;
    
    ++itOwnPointNext;
    ++itOtherPointNext;
    for( uint j = 0; j < rNbSteps; ++j )
    {
        MT_Vector2D vLastOwnPoint  =vCurOwnPoint;
        MT_Vector2D vLastOtherPoint=vCurOtherPoint;
        GetNextPoint( vCurOwnPoint  , pOwnLimit  ->end(), itOwnPointNext , rOwnStep  );
        GetNextPoint( vCurOtherPoint, pOtherLimit->end(), itOtherPointNext, rOtherStep );
        MT_Vector2D vDir=((vCurOwnPoint-vLastOwnPoint).Normalize()+(vCurOtherPoint-vLastOtherPoint).Normalize())/2;
        if ( MT_Line(vLastOwnPoint,vCurOwnPoint).IsInside(vNearest,PRECISION) )
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
bool MIL_Fuseau::ComputePointBeforeLima( const MIL_LimaOrder& lima, MT_Float rDistBefore, MT_Vector2D& vResult ) const
{
    T_PointVector result;
    ComputePointsBeforeLima( lima, rDistBefore, 1, result );

    if ( result.size() == 1 )
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
bool MIL_Fuseau::ComputePointsBeforeLima( const MIL_LimaOrder& lima, MT_Float rDistBefore, uint nNbPoints, T_PointVector& results ) const
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
    const MT_Float rDist = supportLine.Magnitude() / nNbPoints;
    if( rDist == 0. )
        return false;
          
    for ( MT_Float rCurrentDist = rDist * 0.5; nNbPoints--; rCurrentDist += rDist )
        results.push_back( supportLine.GetPointAt( rCurrentDist ) );
    return true;
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

    if( pLeftLimit_ )
        pLeftLimit_->DecRef( *this );
    if( pRightLimit_ )
        pRightLimit_->DecRef( *this );
    if( pMiddleLimit_ )
        pMiddleLimit_->DecRef( *this );

    pLeftLimit_   = fuseau.pLeftLimit_;
    pRightLimit_  = fuseau.pRightLimit_;
    pMiddleLimit_ = fuseau.pMiddleLimit_;
    
    if( pLeftLimit_ )
        pLeftLimit_->AddRef( *this );
    if( pRightLimit_ )
        pRightLimit_->AddRef( *this );
    if( pMiddleLimit_ )
        pMiddleLimit_->AddRef( *this );
    
    return *this;
}

// =============================================================================
// PATH FIND
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::Distance
// Created: AGE 2005-05-13
// -----------------------------------------------------------------------------
MT_Float MIL_Fuseau::Distance( const MT_Vector2D& p, bool bLimitsOnly ) const
{
    if( ! pLeftLimit_ || ! pRightLimit_ 
       || ( !bLimitsOnly && IsInside( p ) ) )
        return 0;

    MT_Line first( pLeftLimit_->GetPoints().front(), pRightLimit_->GetPoints().front() );
    const MT_Float rFirstDistance = first.ClosestPointOnLine( p ).Distance( p );
    MT_Line last( pLeftLimit_->GetPoints().back(), pRightLimit_->GetPoints().back() );
    const MT_Float rLastDistance = last.ClosestPointOnLine( p ).Distance( p );
    

    return std::min( std::min( rFirstDistance, rLastDistance ),
                     std::min( pLeftLimit_->Distance( p ), pRightLimit_->Distance( p ) ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::GetCost
// Created: AGE 2005-03-23
// -----------------------------------------------------------------------------
MT_Float MIL_Fuseau::GetCost( const MT_Vector2D&, const MT_Vector2D& to, MT_Float rMaxDistanceOut, MT_Float rCostPerMeterOut, MT_Float rComfortDistanceIn, MT_Float rCostPerMeterIn ) const
{
    if( !pLeftLimit_ )
        return 0;

    const bool     bInside      = IsInsidish( to );
    const MT_Float rMinDistance = Distance( to, true );

    if( bInside )
    {
        if( rMinDistance > rComfortDistanceIn )
            return 0;
        return ( rComfortDistanceIn - rMinDistance ) * rCostPerMeterIn;
    } else {
        if( rMinDistance > rMaxDistanceOut )
            return -1;
        return rMinDistance * rCostPerMeterOut;
    }
}

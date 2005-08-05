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

#include "MIL_Pch.h"

#include "MIL_Fuseau.h"

#include "Limit/MIL_Limit.h"
#include "Limit/MIL_LimitManager.h"
#include "Lima/MIL_Lima.h"
#include "MIL_AgentServer.h"
#include "Tools/MIL_Tools.h"

#include "TER/TER_PathFindManager.h"
#include "TER/TER_DynamicData.h"
#include "MT_Tools/MT_Polyline.h"

uint MIL_Fuseau::nNbrMeterPerSample_ = 400; //$$$ A GICLER

#define PRECISION 0.1 //$$ CRADE

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau constructor
// Created: NLD 2004-05-21
// -----------------------------------------------------------------------------
MIL_Fuseau::MIL_Fuseau( const MT_Vector2D& vOrientationRefPos, const MIL_Limit& leftLimit, const MIL_Limit& rightLimit, const MIL_Lima* pBeginMissionLima, const MIL_Lima* pEndMissionLima )
    : TER_Polygon         ()
    , pLeftLimit_         ( 0 )
    , pRightLimit_        ( 0 )
    , pBeginMissionLima_  ( 0 )
    , pEndMissionLima_    ( 0 )
    , globalDirectionLine_( vStartGlobalDirection_, vEndGlobalDirection_ )
    , rLeftLimitLength_   ( 0. )
    , rRightLimitLength_  ( 0. )
{
    Reset( vOrientationRefPos, leftLimit, rightLimit, pBeginMissionLima, pEndMissionLima );
}
   
//-----------------------------------------------------------------------------
// Name: MIL_Fuseau constructor
// Created: NLD 2003-01-14
//-----------------------------------------------------------------------------
MIL_Fuseau::MIL_Fuseau()
    : TER_Polygon         ()
    , pLeftLimit_         ( 0 )
    , pRightLimit_        ( 0 )
    , pBeginMissionLima_  ( 0 )
    , pEndMissionLima_    ( 0 )
    , globalDirectionLine_( vStartGlobalDirection_, vEndGlobalDirection_ )
    , rLeftLimitLength_   ( 0. )
    , rRightLimitLength_  ( 0. )
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

// NB : vIntersection is on the line between itIntersectionIterator - 1 and itIntersectionIterator
bool ComputeIntersectionWithLima( const T_PointVector& limit, const MIL_Lima& lima, MT_Vector2D& vIntersection, CIT_PointVector& itIntersectionIterator )
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


// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::SplitLimit
// Created: NLD 2004-06-16
// -----------------------------------------------------------------------------
void MIL_Fuseau::SplitLimit( const T_PointVector& limit, T_PointVectorVector& parts )
{
    CIT_PointVector itBeginLimaIntersection = limit.end();
    CIT_PointVector itEndLimaIntersection   = limit.end();
    MT_Vector2D     vBeginLimaIntersection;
    MT_Vector2D     vEndLimaIntersection;

    if( pBeginMissionLima_ )
        ComputeIntersectionWithLima( limit, *pBeginMissionLima_, vBeginLimaIntersection, itBeginLimaIntersection );

    if( pEndMissionLima_ )
        ComputeIntersectionWithLima( limit, *pEndMissionLima_, vEndLimaIntersection, itEndLimaIntersection );

    bool bReverse = false;
    if( itEndLimaIntersection != limit.end() && itBeginLimaIntersection != limit.end() && itBeginLimaIntersection > itEndLimaIntersection )
    {
        std::swap( itEndLimaIntersection, itBeginLimaIntersection );
        std::swap( vBeginLimaIntersection, vEndLimaIntersection );
        bReverse = true;
    }


    CIT_PointVector itPrev = limit.begin();
    parts.push_back( T_PointVector() );

    // 1st part
    if( itBeginLimaIntersection != limit.end() )
    {
        assert( itPrev <= itBeginLimaIntersection );
        std::copy( itPrev, itBeginLimaIntersection, std::back_inserter( parts.back() ) );
        parts.back().push_back( vBeginLimaIntersection );
        parts.push_back( T_PointVector() );
        parts.back().push_back( vBeginLimaIntersection );
        itPrev = itBeginLimaIntersection;
    } 
    else if( pBeginMissionLima_ )
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
        std::copy( itPrev, itEndLimaIntersection, std::back_inserter( parts.back() ) );
        parts.back().push_back( vEndLimaIntersection );
        parts.push_back( T_PointVector() );
        parts.back().push_back( vEndLimaIntersection );
        itPrev = itEndLimaIntersection;
    }
    else if( pEndMissionLima_ )
    {
        assert( itPrev <= limit.end() );
        std::copy( itPrev, limit.end(), std::back_inserter( parts.back() ) );
        parts.push_back( T_PointVector() );
        parts.back().push_back( limit.back() );
        itPrev = limit.end();
    }

    // 3rd part
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
void MIL_Fuseau::TruncateAndReorientLimits()
{
    // The two limits must have the same orientation
    MT_Line lineBorder1( *leftPointVector_.begin (), *rightPointVector_.begin() );
    MT_Line lineBorder2( *leftPointVector_.rbegin(), *rightPointVector_.rbegin() );
    if( lineBorder1.Intersect2D( lineBorder2 ) )
        std::reverse( rightPointVector_.begin(), rightPointVector_.end() );

    // The orientation cannot be determined from the 'LDM' and the 'LFM'
    //  => orient it regarding the 'orientation reference position' (usually the 
    //     position of unit owning the fuseau

    // Orientation = From the closest border to the furthest
    if( vOrientationRefPos_.SquareDistance( lineBorder1.GetCenter() ) > vOrientationRefPos_.SquareDistance( lineBorder2.GetCenter() ) )
    {
        std::reverse( rightPointVector_.begin(), rightPointVector_.end() );
        std::reverse( leftPointVector_.begin(), leftPointVector_.end() );
    }    

    T_PointVectorVector leftParts;
    T_PointVectorVector rightParts;

    SplitLimit( leftPointVector_ , leftParts  );
    SplitLimit( rightPointVector_, rightParts );
 
    assert( leftParts.size() == rightParts.size() );
    assert( !leftParts.empty() );
    assert( leftParts.size() <= 2 );

    uint nNbParts = leftParts.size();
    if( nNbParts == 1 )
    {
        leftPointVector_  = leftParts [0];
        rightPointVector_ = rightParts[0];
    }
    else if( nNbParts == 3 )
    {
        leftPointVector_  = leftParts [1];
        rightPointVector_ = rightParts[1];
    }
    else if( nNbParts == 2 )
    {
        // Si 2 parties, on prend en priorité celle dans laquelle le point de réference se trouve
        if( IsPointInsidePolygon( leftParts[1], rightParts[1], vOrientationRefPos_ ) )
        {
            leftPointVector_  = leftParts [1];
            rightPointVector_ = rightParts[1];
        }
        // Sinon, on prend la première partie, qui tient compte de l'orientation initiale
        else
        {
            leftPointVector_  = leftParts [0];
            rightPointVector_ = rightParts[0];
        }
    }  
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::InitializePolygon
// Created: NLD 2004-05-19
// -----------------------------------------------------------------------------
void MIL_Fuseau::InitializePolygon()
{
    // Create the ring
    T_PointPtrVector pointsVector;
    for( IT_PointVector itPoint = leftPointVector_.begin(); itPoint != leftPointVector_.end(); ++itPoint )
        pointsVector.push_back( &*itPoint );
    for( RIT_PointVector ritPoint = rightPointVector_.rbegin(); ritPoint != rightPointVector_.rend(); ++ritPoint )
        pointsVector.push_back( &*ritPoint );
    pointsVector.push_back( &*leftPointVector_.begin() );

    // create new polygon with previous points
    TER_Polygon::Reset(pointsVector);
}

//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::Reset
// Created: NLD 2003-01-14
//-----------------------------------------------------------------------------
void MIL_Fuseau::Reset( const MT_Vector2D& vOrientationRefPos, const MIL_Limit& leftLimit, const MIL_Limit& rightLimit, const MIL_Lima* pBeginMissionLima, const MIL_Lima* pEndMissionLima )
{
    MIL_Fuseau::Reset();

    vOrientationRefPos_ = vOrientationRefPos;
    pLeftLimit_         = &leftLimit;
    pRightLimit_        = &rightLimit;
    pBeginMissionLima_  = pBeginMissionLima;
    pEndMissionLima_    = pEndMissionLima;

    if( !pBeginMissionLima_ || !pEndMissionLima_ )
        pBeginMissionLima_ = pEndMissionLima_ = 0;

    leftPointVector_  = leftLimit .GetPointVector();
    rightPointVector_ = rightLimit.GetPointVector();
    assert( !leftPointVector_ .empty() );
    assert( !rightPointVector_.empty() );

    rLeftLimitLength_  = leftLimit .GetLength();
    rRightLimitLength_ = rightLimit.GetLength();

    TruncateAndReorientLimits();
    InitializePolygon        ();

    // Fuseau global orientation
    vStartGlobalDirection_ = MT_Line( *leftPointVector_.begin (), *rightPointVector_.begin () ).GetCenter();
    vEndGlobalDirection_   = MT_Line( *leftPointVector_.rbegin(), *rightPointVector_.rbegin() ).GetCenter();
    
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

    pLeftLimit_->RegisterFuseau( *this );
    pRightLimit_->RegisterFuseau( *this );
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

    leftPointVector_ .clear();
    rightPointVector_.clear();

    rLeftLimitLength_  = 0;
    rRightLimitLength_ = 0;

    if( pLeftLimit_ )
        pLeftLimit_->RemoveFuseau( *this );
    if( pRightLimit_ )
        pRightLimit_->RemoveFuseau( *this );

    pLeftLimit_  = 0;
    pRightLimit_ = 0;
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

//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::Split
// Created: NLD 2003-04-22
//-----------------------------------------------------------------------------
bool MIL_Fuseau::Split( uint nNbrSubFuseau, T_LimitConstPtrVector& limitVector ) const
{
    if( !pLeftLimit_ || !pRightLimit_ || nNbrSubFuseau < 1 )
        return false;

    // Create the new limits
    limitVector.push_back( pLeftLimit_ );
    for( uint i = 1; i < nNbrSubFuseau; ++i )
        limitVector.push_back( &MIL_AgentServer::GetWorkspace().GetLimitManager().CreateLimit() );
    limitVector.push_back( pRightLimit_ );

    T_PointVector leftPointVectorTmp  = leftPointVector_;
    T_PointVector rightPointVectorTmp = rightPointVector_;

    InsertPointProjection( leftPointVector_ , rightPointVectorTmp );
    InsertPointProjection( rightPointVector_, leftPointVectorTmp  );
    assert( leftPointVectorTmp.size() == rightPointVectorTmp.size() );

    for( uint j = 0; j < leftPointVectorTmp.size(); ++j )
    {
        MT_Vector2D vTmp = rightPointVectorTmp[j] - leftPointVectorTmp[j];
        vTmp /= nNbrSubFuseau;
        for( i = 1; i < nNbrSubFuseau; ++i )
            const_cast< MIL_Limit&>( *limitVector[i] ).AddPoint( leftPointVectorTmp[j] + vTmp * (MT_Float)(i) );
    }

    // Limit validation (MOS sending)
    for( i = 1; i < nNbrSubFuseau; ++i )
        limitVector[i]->SendFullState();
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
    T_LimitConstPtrVector limitVector;
    if( !Split( nNbrSubFuseau, limitVector) )
        return false;
    
    // Creation des fuseaux
    const MIL_Limit* pPrevLimit = 0;
    for( CIT_LimitConstPtrVector itLimit = limitVector.begin(); itLimit != limitVector.end(); ++itLimit )
    {
        const MIL_Limit* pCurLimit = *itLimit;
        if( pPrevLimit )
        {
            MIL_Fuseau* pNewSubFuseau = new MIL_Fuseau( vOrientationRefPos_, *pPrevLimit, *pCurLimit, pBeginMissionLima_, pEndMissionLima_ ); ///$$$$ RAM
            container.push_back( pNewSubFuseau );
        }
        pPrevLimit = pCurLimit;
    }
    assert( container.size() == nNbrSubFuseau );
    return true;
}

           
//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::ComputeFurthestExtremityPoint
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
bool MIL_Fuseau::ComputeFurthestExtremityPoint( MT_Vector2D& vResult ) const
{
    if( leftPointVector_.empty() || rightPointVector_.empty() )
        return false;

    MT_Line lineFurthestBorder( *leftPointVector_.rbegin(), *rightPointVector_.rbegin() );
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
    if( leftPointVector_.empty() || rightPointVector_.empty() )
        return false;

    MT_Line lineClosestBorder( *leftPointVector_.begin(), *rightPointVector_.begin() );
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
    
    T_PointVector* pOwnLimit  =0;
    T_PointVector* pOtherLimit=0;
    MT_Line*       pExtremity =0;
    MT_Vector2D    vNearest;
    MT_Float       rSquareDist = std::numeric_limits<MT_Float>::max();
    static const MT_Float rNbSteps = MIL_Tools::ConvertMeterToSim( nNbrMeterPerSample_ );
    MT_Float rOwnStep   = 0.0;
    MT_Float rOtherStep = 0.0;

    // check nearest point with left border
    if ( GetPolyLineClosestPoint(vPos,leftPointVector_,vNearest,rSquareDist) )
    {
        pOwnLimit    = (T_PointVector*)&leftPointVector_;
        pOtherLimit  = (T_PointVector*)&rightPointVector_;
        rOwnStep     = rLeftLimitLength_  / rNbSteps;
        rOtherStep   = rRightLimitLength_ / rNbSteps;
    }

    // check nearest point with right border
    if ( GetPolyLineClosestPoint(vPos,rightPointVector_,vNearest,rSquareDist) )
    {
        pOwnLimit    =(T_PointVector*)&rightPointVector_;
        pOtherLimit  =(T_PointVector*)&leftPointVector_;
        rOwnStep     = rRightLimitLength_ / rNbSteps;
        rOtherStep   = rLeftLimitLength_  / rNbSteps;
    }

    // check extremity segments
    
    // left
    MT_Line vLeftSeg(*leftPointVector_.begin(),*rightPointVector_.begin());
    MT_Vector2D vTmp        = vLeftSeg.ClosestPointOnLine( vPos );
    MT_Float rSquareDistTmp = vPos.SquareDistance( vTmp );
    if ( rSquareDistTmp <= rSquareDist)
    {
        rSquareDist = rSquareDistTmp;
        vNearest    = vTmp;
        pExtremity  = &vLeftSeg;
    }

    // right
    MT_Line vRightSeg(*leftPointVector_.rbegin(),*rightPointVector_.rbegin());
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
    IT_PointVector itOwnPointNext   = pOwnLimit->begin();
    IT_PointVector itOtherPointNext = pOtherLimit->begin();
    MT_Vector2D    vCurOwnPoint     = *itOwnPointNext;
    MT_Vector2D    vCurOtherPoint   = *itOtherPointNext;
    
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
bool MIL_Fuseau::ComputePointBeforeLima( const MIL_Lima& lima, MT_Float rDistBefore, MT_Vector2D& vResult ) const
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
bool MIL_Fuseau::ComputePointsBeforeLima( const MIL_Lima& lima, MT_Float rDistBefore, uint nNbPoints, T_PointVector& results ) const
{
    results.clear();

    if ( leftPointVector_.empty() || rightPointVector_.empty() || lima.GetPointVector().empty() )
        return false;

    results.reserve( nNbPoints );

    // Calcul de la direction
    TER_DistanceLess cmpLeft ( *leftPointVector_.begin () );
    TER_DistanceLess cmpRight( *rightPointVector_.begin() );

    T_PointSet leftIntersectionSet ( cmpLeft  );
    T_PointSet rightIntersectionSet( cmpRight );

    lima.Intersect2D( leftPointVector_ , leftIntersectionSet  );
    lima.Intersect2D( rightPointVector_, rightIntersectionSet );

    MT_Vector2D vTranslation;

    if ( leftIntersectionSet.empty() )
    {
        if ( rightIntersectionSet.empty() )
            vTranslation = ( vStartGlobalDirection_ - vEndGlobalDirection_ ).Normalized() * rDistBefore;
        else
            vTranslation = GetPointOnLimitAfterIntersection( rightPointVector_, *rightIntersectionSet.begin(), rDistBefore ) - *rightIntersectionSet.begin();
    }
    else
    {
        vTranslation = GetPointOnLimitAfterIntersection( leftPointVector_, *leftIntersectionSet.begin(), rDistBefore ) - *leftIntersectionSet.begin();
        if ( !rightIntersectionSet.empty() )
        {
            vTranslation += GetPointOnLimitAfterIntersection( rightPointVector_, *rightIntersectionSet.begin(), rDistBefore ) - *rightIntersectionSet.begin();
            vTranslation *= 0.5;
        }
    }

    // calcul du support
    MT_Polyline supportLine( lima.GetPointVector() );
    
    supportLine.Translate( vTranslation );
    TER_Polygon::Intersection( supportLine, PRECISION );
    
    // positionnement des points
    const MT_Float rDist = supportLine.Magnitude() / nNbPoints;
          
    for ( MT_Float rCurrentDist = rDist * 0.5; nNbPoints--; rCurrentDist += rDist )
        results.push_back( supportLine.GetPointAt( rCurrentDist ) );

    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::Distance
// Created: AGE 2005-05-13
// -----------------------------------------------------------------------------
MT_Float MIL_Fuseau::Distance( const MT_Vector2D& p, bool bLimitsOnly ) const
{
    if( ! pLeftLimit_ || ! pRightLimit_ 
       || leftPointVector_.empty() || rightPointVector_.empty()
       || ( !bLimitsOnly && IsInside( p ) ) )
        return 0;

    MT_Line first( leftPointVector_.front(), rightPointVector_.front() );
    const MT_Float rFirstDistance = first.ClosestPointOnLine( p ).Distance( p );
    MT_Line last( leftPointVector_.back(), rightPointVector_.back() );
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
    if( ! pLeftLimit_ )
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

// =============================================================================
// PATH FINDER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::CreateDynamicData
// Created: AGE 2005-03-24
// -----------------------------------------------------------------------------
TER_DynamicData* MIL_Fuseau::CreateDynamicData() const
{
    TER_DynamicData* pPathfindData = & TER_PathFindManager::GetPathFindManager().CreateDynamicData();

    T_PointVector leftPointVectorTmp  = leftPointVector_;
    T_PointVector rightPointVectorTmp = rightPointVector_;

    InsertPointProjection( leftPointVector_ , rightPointVectorTmp );
    InsertPointProjection( rightPointVector_, leftPointVectorTmp  );
    assert( leftPointVectorTmp.size() == rightPointVectorTmp.size() );
    assert( !leftPointVectorTmp.empty() );

    MT_Vector2D vLastMiddlePoint = leftPointVectorTmp[0] + (rightPointVectorTmp[0] - leftPointVectorTmp[0]) / 2;
    for( uint j = 1; j < leftPointVectorTmp.size(); ++j )
    {
        MT_Vector2D vMiddlePoint = leftPointVectorTmp[j] + (rightPointVectorTmp[j] - leftPointVectorTmp[j]) / 2;
        TER_PathFindManager::GetPathFindManager().CreateLineTree( vLastMiddlePoint, vMiddlePoint, *pPathfindData );
        vLastMiddlePoint = vMiddlePoint;
    }
    return pPathfindData;
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::operator==
// Created: NLD 2005-07-08
// -----------------------------------------------------------------------------
bool MIL_Fuseau::operator==( const MIL_Fuseau& rhs ) const
{
    if( leftPointVector_ != rhs.leftPointVector_ )
        return false;

    if( rightPointVector_ != rhs.rightPointVector_ )
        return false;

    if( pLeftLimit_ != rhs.pLeftLimit_ )
        return false;

    if( pRightLimit_ != rhs.pRightLimit_ )
        return false;

    return true;
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
// Name: MIL_Fuseau::operator=
// Created: NLD 2003-10-24
// -----------------------------------------------------------------------------
MIL_Fuseau& MIL_Fuseau::operator=( const MIL_Fuseau& fuseau )
{
    TER_Polygon::operator=( fuseau );
    
    leftPointVector_    = fuseau.leftPointVector_;
    rightPointVector_   = fuseau.rightPointVector_;

    rLeftLimitLength_   = fuseau.rLeftLimitLength_;
    rRightLimitLength_  = fuseau.rRightLimitLength_;

    //globalDirectionLine_    = fuseau.globalDirectionLine_; $$ NE PAS COPIER - MT_LINE SUCKS
    vStartGlobalDirection_  = fuseau.vStartGlobalDirection_;
    vEndGlobalDirection_    = fuseau.vEndGlobalDirection_;

    if( pLeftLimit_ )
        pLeftLimit_->RemoveFuseau( *this );
    if( pRightLimit_ )
        pRightLimit_->RemoveFuseau( *this );

    pLeftLimit_  = fuseau.pLeftLimit_;
    pRightLimit_ = fuseau.pRightLimit_;

    if( pLeftLimit_ )
        pLeftLimit_->RegisterFuseau( *this );
    if( pRightLimit_ )
        pRightLimit_->RegisterFuseau( *this );

    return *this;
}

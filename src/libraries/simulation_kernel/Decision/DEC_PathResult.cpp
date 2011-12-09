// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_PathResult.h"
#include "DEC_PathPoint.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/Protocol.h"
#include "simulation_terrain/TER_World.h"

// -----------------------------------------------------------------------------
// Name: DEC_PathResult constructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
DEC_PathResult::DEC_PathResult()
    : DEC_Path_ABC()
    , precision_( 0.0001 )
    , bSectionJustEnded_( false )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: DEC_PathResult destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
DEC_PathResult::~DEC_PathResult()
{
    resultList_.clear();
}

//-----------------------------------------------------------------------------
// Name: DEC_PathResult::GetPointOnPathCloseTo
// Created: CMA 2011-12-06
//-----------------------------------------------------------------------------
MT_Vector2D DEC_PathResult::GetPointOnPathCloseTo( const MT_Vector2D& posToTest, T_FollowingPathList& pathPoints, const MT_Vector2D& lastJoiningPoint, bool forceNextPoint ) const
{
    if( resultList_.empty() )
        throw std::runtime_error( "List of path points resulting from pathfind is empty" );
    if( pathPoints.size() <= 1 )
        throw std::runtime_error( "List of path points requested has not at least 2 elements" );
    
    CIT_FollowingPathList itCurrentRequest = pathPoints.begin();
    CIT_FollowingPathList itNextRequest    = itCurrentRequest;
    ++itNextRequest;

    CIT_PathPointList itResultStart = itCurrentRequest->second;
    CIT_PathPointList itResultEnd   = itResultStart;
    ++itResultEnd;

    bool bNewMin = false;
    bool hasMoreThanTwoPoints = pathPoints.size() > 2;
    double rSquareDistance = std::numeric_limits< double >::max();
    for( ; itResultEnd != resultList_.end(); ++itResultStart, ++itResultEnd )
    {
        if( itNextRequest == pathPoints.end() )
            break;

        MT_Line vLine( (*itResultStart)->GetPos(), (*itResultEnd)->GetPos() );
        MT_Vector2D vClosest = vLine.ClosestPointOnLine( posToTest );
        double rCurrentSquareDistance = vClosest.SquareDistance( posToTest );
        if( rCurrentSquareDistance < rSquareDistance )
        {
            rSquareDistance = rCurrentSquareDistance;
            bNewMin = true;
        }

        if( itNextRequest->second == itResultEnd )
        {
            if( bNewMin || rSquareDistance > 1000 )
            {
                if( itCurrentRequest != pathPoints.begin() )
                {
                    pathPoints.pop_front();
                    itCurrentRequest = pathPoints.begin();
                    itNextRequest    = itCurrentRequest;
                    ++itNextRequest;
                }
                itCurrentRequest = itNextRequest;
                ++itNextRequest;
                bNewMin = false;
                if( lastJoiningPoint != MT_Vector2D( 0, 0 ) && itCurrentRequest->first.SquareDistance( lastJoiningPoint ) < precision_ )
                    break;
            }
            else
                break;
        }
    }

    if( _isnan( itCurrentRequest->first.rX_ ) || _isnan( itCurrentRequest->first.rY_ ) )
        throw std::runtime_error( "Result point is invalid" );

    if( forceNextPoint )
    {
        if( itNextRequest != pathPoints.end() )
            return itNextRequest->first;
        else if( !hasMoreThanTwoPoints )
            return posToTest;
    }

    return itCurrentRequest->first;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::GetNextKeyOnPath
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
DEC_PathResult::CIT_PathPointList DEC_PathResult::GetCurrentKeyOnPath( const MT_Vector2D& vPos ) const
{
    if( resultList_.empty() )
        return resultList_.end();
    static const double rWeldValue = TER_World::GetWorld().GetWeldValue();
    if( resultList_.size() == 1 )
    {
        if( vPos.Distance( resultList_.front()->GetPos() ) <= rWeldValue )
            return resultList_.begin();
        return resultList_.end();
    }
    CIT_PathPointList itEnd = resultList_.begin();
    ++itEnd;
    for( CIT_PathPointList itStart = resultList_.begin(); itEnd != resultList_.end(); ++itStart, ++itEnd )
    {
        MT_Line vLine( (*itStart)->GetPos(), (*itEnd)->GetPos() );
        if( vLine.IsInside( vPos, rWeldValue ) ) //$$$ DE LA MERDE EN BOITE
            return itStart;
    }
    return resultList_.end();
}

//-----------------------------------------------------------------------------
// Name: DEC_PathResult::InternalGetFuturePosition
// Created: JVT 03-09-25
//-----------------------------------------------------------------------------
MT_Vector2D DEC_PathResult::InternalGetFuturePosition( const CIT_PathPointList& itCurrentPos, double rDist, bool bBoundOnPath ) const
{
    if( itCurrentPos == resultList_.end() )
        throw std::runtime_error( "Current position is invalid" );

    // recherche du prochain point sur le path
    // on passe tous les points spéciaux, car il n'y a des changement de direction que sur les PathPoint_Point
    CIT_PathPointList itNextPos = itCurrentPos;
    while ( ++itNextPos != resultList_.end() && (*itNextPos)->GetType() != DEC_PathPoint::eTypePointPath )
        ;
    const MT_Vector2D& vCurrentPos = (*itCurrentPos)->GetPos();
    if( itNextPos == resultList_.end() )
    {
        const MT_Vector2D& vStartPos = resultList_.front()->GetPos();
        const MT_Vector2D& vEndPos   = resultList_.back ()->GetPos();
        if( bBoundOnPath )
            return vEndPos;
        return vEndPos + ( vEndPos - vStartPos ).Normalize() * rDist; // on suit la direction générale du déplacement
    }
    double rLength = vCurrentPos.Distance( (*itNextPos)->GetPos() );
    if( rLength >= rDist )
        return vCurrentPos + ( (*itNextPos)->GetPos() - vCurrentPos ).Normalize() * rDist;
    // parcours sur le segment suivant
    return InternalGetFuturePosition( itNextPos, rDist - rLength, bBoundOnPath );
}

//-----------------------------------------------------------------------------
// Name: DEC_PathResult::GetFuturePosition
// Created: JVT 03-09-25
//-----------------------------------------------------------------------------
MT_Vector2D DEC_PathResult::GetFuturePosition( const MT_Vector2D& vStartPos, double rDist, bool bBoundOnPath ) const
{
    CIT_PathPointList itCurrentPathPoint = GetCurrentKeyOnPath( vStartPos );
    if( itCurrentPathPoint == resultList_.end() )
        return vStartPos;
    CIT_PathPointList itNextPathPoint = itCurrentPathPoint;
    ++itNextPathPoint;
    // Recherche du premier pathpoint
    if( itNextPathPoint == resultList_.end() )
    {
        const MT_Vector2D& vEndPos   = resultList_.back ()->GetPos();
        return vEndPos == vStartPos ? vEndPos : vStartPos + ( vEndPos - vStartPos ).Normalize() * rDist;
    }
    double rLength = vStartPos.Distance( (*itNextPathPoint)->GetPos() );
    if( rLength >= rDist )
        return vStartPos + ( (*itNextPathPoint)->GetPos() - vStartPos ).Normalize() * rDist;
    // parcours sur les points
    return InternalGetFuturePosition( itNextPathPoint, rDist - rLength, bBoundOnPath );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::ComputeFutureObjectCollision
// Created: NLD 2003-10-08
// -----------------------------------------------------------------------------
bool DEC_PathResult::ComputeFutureObjectCollision( const MT_Vector2D& vStartPos, const T_KnowledgeObjectVector& objectsToTest, double& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject ) const
{
    rDistance = std::numeric_limits< double >::max();
    pObject.reset();
    E_State nPathState = GetState();
    if( nPathState != eValid && nPathState != ePartial )
        return false;
    if( resultList_.empty() )
        throw std::runtime_error( "List of path points resulting from pathfind is empty" );
    CIT_PathPointList itCurrentPathPoint = GetCurrentKeyOnPath( vStartPos );
    if( itCurrentPathPoint == resultList_.end() )
        return false;
    CIT_PathPointList itNextPathPoint = itCurrentPathPoint;
    ++itNextPathPoint;
    std::multimap< double, boost::shared_ptr< DEC_Knowledge_Object > > objectsOnPathMap;
    // Determination de tous les objets connus avec lesquels il va y avoir collision dans le déplacement en cours
    for( CIT_KnowledgeObjectVector itKnowledge = objectsToTest.begin(); itKnowledge != objectsToTest.end(); ++itKnowledge )
    {
        boost::shared_ptr< DEC_Knowledge_Object > pKnowledge = *itKnowledge;
        const MT_Vector2D* pPrevPos = &vStartPos;
        for( CIT_PathPointList itPathPoint = itNextPathPoint; itPathPoint != resultList_.end(); ++itPathPoint )
        {
            MT_Line lineTmp( *pPrevPos, (*itPathPoint)->GetPos() );
            TER_DistanceLess colCmp( *pPrevPos );
            T_PointSet collisions( colCmp );
            if( pKnowledge->GetLocalisation().Intersect2D( lineTmp, collisions ) )
            {
                if( collisions.empty() )
                    throw std::runtime_error( "List of collision points is empty" );
                //$$$ Distance fausse (distance en ligne droite)
                const double rColDist = vStartPos.Distance( *collisions.begin() );
                objectsOnPathMap.insert( std::make_pair( rColDist, pKnowledge ) );
                break;
            }
            pPrevPos = &(*itPathPoint)->GetPos();
        }
    }
    if( objectsOnPathMap.empty() )
        return false;
    rDistance = objectsOnPathMap.begin()->first;
    pObject   = objectsOnPathMap.begin()->second;
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::Serialize
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void DEC_PathResult::Serialize( sword::Path& asn ) const
{
    if( resultList_.empty() )
        throw std::runtime_error( "List of path points resulting from pathfind is empty" );
    asn.mutable_location()->set_type( sword::Location::line );
    for( CIT_PathPointList it = resultList_.begin(); it != resultList_.end(); ++it )
        NET_ASN_Tools::WritePoint( (*it)->GetPos(), *asn.mutable_location()->mutable_coordinates()->add_elem() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::AddResultPoint
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void DEC_PathResult::AddResultPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint )
{
    if( bSectionJustEnded_ )
    {
        // Pop last point
        resultList_.pop_back();
        bSectionJustEnded_ = false;
    }
    boost::shared_ptr< DEC_PathPoint > point( new DEC_PathPoint( vPos, nObjectTypes, nObjectTypesToNextPoint ) );
    resultList_.push_back( point );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::NotifySectionEnded
// Created: AGE 2005-09-01
// -----------------------------------------------------------------------------
void DEC_PathResult::NotifySectionEnded()
{
    bSectionJustEnded_ = true;
}

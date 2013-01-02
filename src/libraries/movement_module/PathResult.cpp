// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "PathResult.h"
#include "PathPoint.h"
#include "PathType.h"
#include "MT_Tools/MT_Line.h"
#include <tools/Exception.h>
#include "wrapper/Hook.h"
#include "wrapper/View.h"
#include <module_api/Log.h>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <iterator>
#include <limits>

using namespace sword;
using namespace sword::movement;

DECLARE_HOOK( ComputeObjectCollision, void,
    ( const SWORD_Model* entity, const KnowledgeCache& objectsToTest,
      double& rDistance,
      boost::shared_ptr< DEC_Knowledge_Object >& pObject,
      MT_Vector2D* start, std::size_t size, bool blockedByObject, bool applyScale ) )

// -----------------------------------------------------------------------------
// Name: PathResult constructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
PathResult::PathResult( std::size_t identifier, const PathType& pathType )
    : Path_ABC( identifier )
    , pathType_         ( pathType )
    , bSectionJustEnded_( false )
{
    itCurrentPathPoint_ = resultList_.end();
}

//-----------------------------------------------------------------------------
// Name: PathResult destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
PathResult::~PathResult()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PathResult::GetCurrentKeyOnPath
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
PathResult::CIT_PathPointList PathResult::GetCurrentKeyOnPath() const
{
    if( itCurrentPathPoint_ == resultList_.end() )
        return itCurrentPathPoint_;
    auto itStart = itCurrentPathPoint_;
    auto itEnd = itCurrentPathPoint_;
    ++itEnd;
    for( ; itEnd != resultList_.end(); ++itStart, ++itEnd )
        if( (*itStart)->GetPos() != (*itEnd)->GetPos() )
            return itStart;
    return itStart;
}

//-----------------------------------------------------------------------------
// Name: PathResult::InternalGetFuturePosition
// Created: JVT 03-09-25
//-----------------------------------------------------------------------------
MT_Vector2D PathResult::InternalGetFuturePosition( const CIT_PathPointList& itCurrentPos, double rDist, bool bBoundOnPath ) const
{
    if( itCurrentPos == resultList_.end() )
        throw MASA_EXCEPTION( "Current position is invalid" );

    // recherche du prochain point sur le path
    // on passe tous les points spéciaux, car il n'y a des changement de direction que sur les PathPoint_Point
    CIT_PathPointList itNextPos = itCurrentPos;
    while ( ++itNextPos != resultList_.end() && (*itNextPos)->GetType() != PathPoint::eTypePointPath )
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
// Name: PathResult::GetFuturePosition
// Created: JVT 03-09-25
//-----------------------------------------------------------------------------
MT_Vector2D PathResult::GetFuturePosition( const MT_Vector2D& vStartPos, double rDist, bool bBoundOnPath ) const
{
    CIT_PathPointList itCurrentPathPoint = GetCurrentKeyOnPath();
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
// Name: PathResult::ComputeFutureObjectCollision
// Created: NLD 2003-10-08
// -----------------------------------------------------------------------------
bool PathResult::ComputeFutureObjectCollision( const wrapper::View& entity, const KnowledgeCache& objectsToTest, double& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject, bool blockedByObject, bool applyScale ) const
{
    rDistance = std::numeric_limits< double >::max();
    pObject.reset();
    E_State nPathState = GetState();
    if( nPathState != eValid && nPathState != ePartial )
        return false;
    if( resultList_.empty() )
        return false;
    CIT_PathPointList itCurrentPathPoint = GetCurrentKeyOnPath();
    if( itCurrentPathPoint == resultList_.end() )
        return false;
    CIT_PathPointList itNextPathPoint = itCurrentPathPoint;
    ++itNextPathPoint;
    // Determination de tous les objets connus avec lesquels il va y avoir collision dans le déplacement en cours
    std::vector< MT_Vector2D > path;
    std::transform( itCurrentPathPoint, resultList_.end(), std::back_inserter( path ),
        boost::lambda::bind( &PathPoint::GetPos, *boost::lambda::_1 ) );
    if( path.empty() )
        return false;
    GET_HOOK( ComputeObjectCollision )( entity, objectsToTest, rDistance, pObject, &path.front(), path.size(), blockedByObject, applyScale );
    if( !pObject )
        return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: PathResult::AddResultPoint
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void PathResult::AddResultPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint )
{
    if( bSectionJustEnded_ )
    {
        // Pop last point
        resultList_.pop_back();
        bSectionJustEnded_ = false;
    }
    boost::shared_ptr< PathPoint > point( new PathPoint( vPos, nObjectTypes, nObjectTypesToNextPoint ) );
    resultList_.push_back( point );
    if( resultList_.size() == 1 )
        itCurrentPathPoint_ = resultList_.begin();
}

// -----------------------------------------------------------------------------
// Name: PathResult::NotifySectionEnded
// Created: AGE 2005-09-01
// -----------------------------------------------------------------------------
void PathResult::NotifySectionEnded()
{
    bSectionJustEnded_ = true;
}

// -----------------------------------------------------------------------------
// Name: PathResult::NotifyPointReached
// Created: LDC 2012-01-18
// -----------------------------------------------------------------------------
void PathResult::NotifyPointReached( const CIT_PathPointList& itCurrentPathPoint )
{
    itCurrentPathPoint_ = itCurrentPathPoint;
}

// -----------------------------------------------------------------------------
// Name: PathResult::IsWaypoint
// Created: LDC 2012-01-18
// -----------------------------------------------------------------------------
bool PathResult::IsWaypoint( const MT_Vector2D& ) const
{
    return true;
}

//-----------------------------------------------------------------------------
// Name: PathResult::GetResult
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
const PathResult::T_PathPointList& PathResult::GetResult( bool useCheck ) const
{
    if( useCheck && GetState() == eComputing )
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Path is being computed" );
    return resultList_;
}

// -----------------------------------------------------------------------------
// Name: PathResult::GetPathType
// Created: CMA 2012-03-29
// -----------------------------------------------------------------------------
const PathType& PathResult::GetPathType() const
{
    return pathType_;
}

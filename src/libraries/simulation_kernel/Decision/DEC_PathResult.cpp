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
#include "MIL_AgentServer.h"
#include "Decision/DEC_PathType.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Network/NET_ASN_Tools.h"
#include "MIL_AgentServer.h"
#include "MT_Tools/MT_Logger.h"
#include "SlopeSpeedModifier.h"
#include "meteo/PHY_MeteoDataManager.h"
#include "meteo/RawVisionData/Elevation.h"
#include "meteo/RawVisionData/PHY_RawVisionData.h"
#include "simulation_terrain/TER_PathPoint.h"
#include "simulation_terrain/TER_Pathfinder.h"
#include "simulation_terrain/TER_Pathfinder_ABC.h"
#include "simulation_terrain/TER_World.h"
#include <boost/make_shared.hpp>

std::list< boost::shared_ptr< TER_PathPoint > > SplitEdgesOnElevationGrid(
    const std::list< boost::shared_ptr< TER_PathPoint > >& points )
{
    if( points.size() < 2 || !MIL_AgentServer::IsInitialized() )
        return points;
    std::list< boost::shared_ptr< TER_PathPoint > > output;
    const auto& elevation = MIL_AgentServer::GetWorkspace()
        .GetMeteoDataManager().GetRawVisionData();

    for( auto it = points.begin(); it != points.end(); ++it )
    {
        if( output.empty() || std::next( it ) == points.end() )
        {
            output.push_back( *it );
            continue;
        }
        const auto p1 = *it;
        const auto p2 = *std::next( it );
        SlopeSpeedModifier slopeSpeedModifier;
        SplitOnMajorGridLines( static_cast< int32_t >( elevation.GetCellSize() ),
            p1->GetPos(), p2->GetPos(), [&]( MT_Vector2D from, MT_Vector2D to )
            {
                return slopeSpeedModifier.ComputeLocalSlope( elevation, from, to );
            });
        const auto& slopes = slopeSpeedModifier.GetSlopes();
        for( auto is = slopes.begin(); is != slopes.end(); ++is )
        {
            auto ip = output.rbegin();
            while( ip != output.rend() && !(*ip)->IsSlopeValid() )
               (*ip++)->SetSlope( is->second ); 
            if( is->second > 0 && ( is + 1 ) != slopes.end() )
            {
                const MT_Line segment( p1->GetPos(), p2->GetPos() );
                const MT_Vector2D projected = segment.ProjectPointOnLine(
                        ( is + 1 )->first );
                const auto point = boost::make_shared< TER_PathPoint >(
                    projected, p1->GetObjectTypes(),
                    p1->GetObjectTypesToNextPoint(), false );
                output.push_back( point );
            }
        }
        output.push_back( p1 );
    }
    return output;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult constructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
DEC_PathResult::DEC_PathResult( const DEC_PathType& pathType, unsigned int callerId )
    : pathType_( pathType )
    , bSectionJustStarted_( false )
    , state_( TER_Path_ABC::eComputing )
    , finalized_( false )
    , callerId_( callerId )
{
    itCurrentPathPoint_ = resultList_.end();
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

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::IsOnPath
// Created: CMA 2012-04-16
// -----------------------------------------------------------------------------
bool DEC_PathResult::IsOnPath( const MT_Vector2D& vPos ) const
{
    if( resultList_.empty() )
        return false;
    static const double rWeldValue = TER_World::GetWorld().GetWeldValue();
    if( resultList_.size() == 1 )
        return ( vPos.Distance( resultList_.front()->GetPos() ) <= rWeldValue );
    auto itStart = resultList_.begin();
    auto itEnd = std::next( resultList_.begin() );
    for( ; itEnd != resultList_.end(); ++itStart, ++itEnd )
    {
        MT_Line vLine( (*itStart)->GetPos(), (*itEnd)->GetPos() );
        if( vLine.IsInside( vPos, rWeldValue ) )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::GetCurrentKeyOnPath
// Created: NLD 2004-09-22
// Last modified: CMA 2012-04-13
// -----------------------------------------------------------------------------
DEC_PathResult::T_PathPoints::const_iterator DEC_PathResult::GetCurrentKeyOnPath() const
{
    if( itCurrentPathPoint_ == resultList_.end() )
        return itCurrentPathPoint_;
    auto itStart = itCurrentPathPoint_;
    auto itEnd = std::next( itStart );
    for( ; itEnd != resultList_.end(); ++itStart, ++itEnd )
        if( (*itStart)->GetPos() != (*itEnd)->GetPos() )
            return itStart;
    return itStart;
}

//-----------------------------------------------------------------------------
// Name: DEC_PathResult::InternalGetFuturePosition
// Created: JVT 03-09-25
//-----------------------------------------------------------------------------
MT_Vector2D DEC_PathResult::InternalGetFuturePosition( const T_PathPoints::const_iterator& itCurrentPos, double rDist, bool bBoundOnPath ) const
{
    if( itCurrentPos == resultList_.end() )
        throw MASA_EXCEPTION( "Current position is invalid" );

    // recherche du prochain point sur le path
    // on passe tous les points spéciaux, car il n'y a des changement de direction que sur les PathPoint_Point
    auto itNextPos = itCurrentPos;
    while ( ++itNextPos != resultList_.end() && (*itNextPos)->GetType() != TER_PathPoint::eTypePointPath )
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
    auto itCurrentPathPoint = GetCurrentKeyOnPath();
    if( itCurrentPathPoint == resultList_.end() )
        return vStartPos;
    const auto itNextPathPoint = std::next( itCurrentPathPoint );
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

std::pair< TER_Polygon, std::size_t > DEC_PathResult::ComputePathHull( const T_PathPoints::const_iterator& itCurrentPathPoint ) const
{
    TER_Polygon pathHull;
    T_PointVector hullPoints;
    hullPoints.reserve( 1 + resultList_.size() );
    for( auto itPathPoint = itCurrentPathPoint; itPathPoint != resultList_.end(); ++itPathPoint )
        hullPoints.push_back( (*itPathPoint)->GetPos() );
    if( itCurrentPathPoint != resultList_.end() )
        hullPoints.push_back( (*itCurrentPathPoint)->GetPos() );
    pathHull.Reset( hullPoints, true );
    return std::make_pair( pathHull, hullPoints.size() );
}

namespace
{
    static const double epsilon = 1e-8;

    bool HullIntersects( const TER_Polygon& hull, const TER_Localisation& location )
    {
        const T_PointVector& points = hull.GetBorderPoints();
        if( points.empty() )
            return false;
        auto it = points.begin();
        auto itNext = std::next( it );
        for( ; itNext != points.end(); ++it, ++itNext )
        {
            const MT_Line lineTmp( *it, *itNext );
            const TER_DistanceLess colCmp( *itNext );
            T_PointSet collisions( colCmp );
            if( location.Intersect2D( lineTmp, collisions, epsilon ) )
                return true;
        }
        const TER_Localisation localisationHull( hull );
        return location.Contains( localisationHull, epsilon )
            || localisationHull.Contains( location, epsilon );
    }
}

void DEC_PathResult::ComputeFutureObjectCollision( const MT_Vector2D& vStartPos, double& rDistance,
    boost::shared_ptr< DEC_Knowledge_Object >& pObject, const boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge,
    const T_PathPoints::const_iterator& itCurrentPathPoint,
    const TER_Localisation& location ) const
{
    double rDistanceSum = 0;
    const MT_Vector2D* pPrevPos = &vStartPos;
    for( auto itPathPoint = std::next( itCurrentPathPoint ); itPathPoint != resultList_.end(); ++itPathPoint )
    {
        const MT_Line lineTmp( *pPrevPos, (*itPathPoint)->GetPos() );
        const TER_DistanceLess colCmp( *pPrevPos );
        T_PointSet collisions( colCmp );
        if( location.Intersect2D( lineTmp, collisions, epsilon ) )
        {
            if( collisions.empty() ) // should never happen
                continue;
            rDistanceSum += pPrevPos->Distance( *collisions.begin() );
            if( !pObject || rDistanceSum < rDistance )
            {
                rDistance = rDistanceSum;
                pObject = pKnowledge;
            }
            return;
        }
        rDistanceSum += pPrevPos->Distance( (*itPathPoint)->GetPos() );
        pPrevPos = &(*itPathPoint)->GetPos();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::ComputeFutureObjectCollision
// Created: NLD 2003-10-08
// -----------------------------------------------------------------------------
bool DEC_PathResult::ComputeFutureObjectCollision( const MT_Vector2D& vStartPos, const T_KnowledgeObjectVector& objects,
    double& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject,
    const MIL_Agent_ABC& agent, bool blockedByObject, bool applyScale ) const
{
    rDistance = std::numeric_limits< double >::max();
    pObject.reset();
    if( objects.empty() )
        return false;
    if( GetState() != eValid && GetState() != ePartial )
        return false;
    if( resultList_.empty() )
        return false;
    const auto itCurrentPathPoint = GetCurrentKeyOnPath();
    if( itCurrentPathPoint == resultList_.end() )
        return false;
    const auto hull = ComputePathHull( itCurrentPathPoint );
    const MT_Rect bbox = hull.first.GetBoundingBox();
    // Optimisation: Check bounding box instead of doing 345 intersection checks in case we have 345 points in the path (which happens)
    // Check intersection with convex hull of path if there are less segments.
    // Worst case: Bounding hull has 1 more segment than the actual path, or Bounding hull has 2 segments (degenerate).
    // Best case: Bounding hull has 3 segments.
    const std::size_t hullSize = hull.first.GetBorderPoints().size();
    const bool hullIntersectionIsFaster = hullSize > 2 && hullSize < hull.second;
    // Determination de tous les objets connus avec lesquels il va y avoir collision dans le déplacement en cours
    for( auto itKnowledge = objects.begin(); itKnowledge != objects.end(); ++itKnowledge )
    {
        const auto pKnowledge = *itKnowledge;
        if( blockedByObject && applyScale ) // $$$$ MCO 2014-08-26: not sure why it's only if applyScale
        {
            T_PointVector firstPointVector;
            firstPointVector.push_back( (*resultList_.begin())->GetPos() );
            if( pKnowledge->IsObjectInsidePathPoint( firstPointVector, &agent ) )
                continue;
        }
        TER_Localisation location( pKnowledge->GetLocalisation() );
        if( applyScale && location.GetType() != TER_Localisation::eNone )
            location.Scale( 10 ); // $$$ CMA arbitrary 10m precision (useful for recomputing path when it is very close to obstacle)
        const MT_Rect objectBBox = location.GetBoundingBox();
        if( !bbox.Intersect2D( objectBBox ) && !bbox.Contains( objectBBox ) && !objectBBox.Contains( bbox ) )
            continue;
        if( hullIntersectionIsFaster && !HullIntersects( hull.first, location ) )
            continue;
        ComputeFutureObjectCollision( vStartPos, rDistance, pObject, pKnowledge, itCurrentPathPoint, location );
    }
    return pObject != 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::Serialize
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void DEC_PathResult::Serialize( sword::Path& asn, int firstPoint, int pathSizeThreshold ) const
{
    assert( !resultList_.empty() );
    int index = 0;
    auto it = resultList_.begin();
    std::advance( it, firstPoint );
    for( firstPoint; it != resultList_.end(); ++it )
    {
        NET_ASN_Tools::WritePoint( (*it)->GetPos(), *asn.mutable_location()->mutable_coordinates()->add_elem() );
        if( ++index > pathSizeThreshold )
            return;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::NotifyPointReached
// Created: LDC 2012-01-18
// -----------------------------------------------------------------------------
void DEC_PathResult::NotifyPointReached( const T_PathPoints::const_iterator& itCurrentPathPoint )
{
    itCurrentPathPoint_ = itCurrentPathPoint;
}

//-----------------------------------------------------------------------------
// Name: DEC_PathResult::GetResult
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
const DEC_PathResult::T_PathPoints& DEC_PathResult::GetResult() const
{
    if( GetState() == eComputing )
        MT_LOG_ERROR_MSG( "Path is being computed" );
    return resultList_;
}

void DEC_PathResult::SetResult( T_PathPoints points )
{
    resultList_ = points;
    itCurrentPathPoint_ = resultList_.begin();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::GetPathType
// Created: CMA 2012-03-29
// -----------------------------------------------------------------------------
const DEC_PathType& DEC_PathResult::GetPathType() const
{
    return pathType_;
}

void DEC_PathResult::StartCompute( const sword::Pathfind& pathfind )
{
    auto& pathfinder = MIL_AgentServer::GetWorkspace().GetPathFindManager();
    future_ = pathfinder.StartCompute( callerId_, sections_, pathfind );
}

void DEC_PathResult::Cancel()
{
    if( !future_ )
        future_ = boost::make_shared< TER_PathFuture >();
    future_->Cancel();
}

TER_Path_ABC::E_State DEC_PathResult::GetState() const
{
    if( state_ != TER_Path_ABC::eComputing )
        return state_;
    boost::shared_ptr< TER_PathResult > result;
    if( future_ )
        result = future_->Get();
    if( !result )
        return TER_Path_ABC::eComputing;
    return result->state;
}

const MT_Vector2D& DEC_PathResult::GetLastWaypoint() const
{
    return lastWaypoint_;
}

void DEC_PathResult::Finalize()
{
    if( finalized_ )
        return;
    finalized_ = true;
    auto result = future_->Get();
    if( !result )
        throw MASA_EXCEPTION( "unexpected undefined pathfind result" );
    state_ = result->state;
    lastWaypoint_ = result->lastWaypoint;
    computedWaypoints_ = result->computedWaypoints;
    resultList_ = SplitEdgesOnElevationGrid( result->points );
    itCurrentPathPoint_ = resultList_.begin();
    DoFinalize();
}

void DEC_PathResult::DoFinalize()
{
    // Nothing
}

const std::vector< MT_Vector2D >& DEC_PathResult::GetComputedWaypoints() const
{
    return computedWaypoints_;
}

void DEC_PathResult::RemoveComputedWaypoint()
{
    if( !computedWaypoints_.empty() )
        computedWaypoints_.erase( computedWaypoints_.begin() );
}

void DEC_PathResult::AddSection( const boost::shared_ptr< TER_PathSection >& section )
{
    sections_.push_back( section );
}

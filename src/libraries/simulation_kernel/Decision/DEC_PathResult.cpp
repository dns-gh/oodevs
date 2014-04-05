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
#include "MIL_AgentServer.h"
#include "SlopeSpeedModifier.h"
#include "Decision/DEC_PathType.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "meteo/PHY_MeteoDataManager.h"
#include "meteo/RawVisionData/Elevation.h"
#include "meteo/RawVisionData/PHY_RawVisionData.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/Authentication.h"
#include "simulation_terrain/TER_World.h"
#include <boost/make_shared.hpp>

// -----------------------------------------------------------------------------
// Name: DEC_PathResult constructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
DEC_PathResult::DEC_PathResult( const DEC_PathType& pathType )
    : pathType_( pathType )
    , bSectionJustStarted_( false )
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
    auto itEnd = resultList_.begin();
    ++itEnd;
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
    auto itEnd = itCurrentPathPoint_;
    ++itEnd;
    for( ; itEnd != resultList_.end(); ++itStart, ++itEnd )
    {
        if( (*itStart)->GetPos() == (*itEnd)->GetPos() )
            continue;
        else
            return itStart;
    }
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
    auto itCurrentPathPoint = GetCurrentKeyOnPath();
    if( itCurrentPathPoint == resultList_.end() )
        return vStartPos;
    auto itNextPathPoint = itCurrentPathPoint;
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
bool DEC_PathResult::ComputeFutureObjectCollision( const T_KnowledgeObjectVector& objectsToTest, double& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject, const MIL_Agent_ABC& agent, bool blockedByObject, bool applyScale ) const
{
    static const double epsilon = 1e-8;
    rDistance = std::numeric_limits< double >::max();
    pObject.reset();
    E_State nPathState = GetState();
    if( objectsToTest.empty() )
        return false;
    if( nPathState != eValid && nPathState != ePartial )
        return false;
    if( resultList_.empty() )
        return false;
    auto itCurrentPathPoint = GetCurrentKeyOnPath();
    if( itCurrentPathPoint == resultList_.end() )
        return false;
    auto itNextPathPoint = itCurrentPathPoint;
    ++itNextPathPoint;
    // Path hull
    TER_Polygon pathHull;
    T_PointVector hullPoints;
    hullPoints.reserve( 1 + resultList_.size() );
    for( auto itPathPoint = itCurrentPathPoint; itPathPoint != resultList_.end(); ++itPathPoint )
        hullPoints.push_back( (*itPathPoint)->GetPos() );
    if( itCurrentPathPoint != resultList_.end() )
        hullPoints.push_back( (*itCurrentPathPoint)->GetPos() );
    pathHull.Reset( hullPoints, true );
    MT_Rect bbox = pathHull.GetBoundingBox();
    // Optimisation: Check bounding box instead of doing 345 intersection checks in case we have 345 points in the path (which happens)
    // Check intersection with convex hull of path if there are less segments.
    // Worst case: Bounding hull has 1 more segment than the actual path, or Bounding hull has 2 segments (degenerate).
    // Best case: Bounding hull has 3 segments.
    std::size_t hullSize = pathHull.GetBorderPoints().size();
    bool hullIntersectionIsFaster = hullSize > 2 && hullSize < hullPoints.size();
    // Determination de tous les objets connus avec lesquels il va y avoir collision dans le déplacement en cours
    for( auto itKnowledge = objectsToTest.begin(); itKnowledge != objectsToTest.end(); ++itKnowledge )
    {
        boost::shared_ptr< DEC_Knowledge_Object > pKnowledge = *itKnowledge;
        const TER_Localisation* pObjectLocation = 0;
        if( applyScale )
        {
            pObjectLocation = new TER_Localisation( pKnowledge->GetLocalisation() );
            if( !resultList_.empty() && blockedByObject )
            {
                T_PointVector firstPointVector;
                firstPointVector.push_back( (*resultList_.begin())->GetPos() );
                if( pKnowledge->IsObjectInsidePathPoint( firstPointVector, &agent ) )
                    continue;
            }
            if( pObjectLocation->GetType() != TER_Localisation::eNone )
                const_cast< TER_Localisation* >( pObjectLocation )->Scale( 10 ); // $$$ CMA arbitrary 10m precision (useful for recomputing path when it is very close to obstacle)
        }
        else
            pObjectLocation = &pKnowledge->GetLocalisation();

        MT_Rect objectBBox = pObjectLocation->GetBoundingBox();
        if( !bbox.Intersect2D( objectBBox ) && !bbox.Contains( objectBBox ) && !objectBBox.Contains( bbox ) )
            continue;
        if( hullIntersectionIsFaster )
        {
            const T_PointVector& borderPoints = pathHull.GetBorderPoints();
            if( borderPoints.empty() )
                continue;
            auto itPathHullPoint = borderPoints.begin();
            const MT_Vector2D* pPrevPathHullPos = &(*itPathHullPoint);
            ++itPathHullPoint;
            bool hullIntersected = false;
            for( ; !hullIntersected && itPathHullPoint != borderPoints.end(); ++itPathHullPoint )
            {
                MT_Line lineTmp( *pPrevPathHullPos, *itPathHullPoint );
                TER_DistanceLess colCmp( *pPrevPathHullPos );
                T_PointSet collisions( colCmp );

                hullIntersected |= pObjectLocation->Intersect2D( lineTmp, collisions, epsilon );
                pPrevPathHullPos = &(*itPathHullPoint);
            }
            if( !hullIntersected )
            {
                TER_Localisation localisationHull( pathHull );
                if( !pObjectLocation->Contains( localisationHull, epsilon ) && !localisationHull.Contains( *pObjectLocation, epsilon ) )
                    continue;
            }
        }
        double rDistanceSum = 0.;
        const MT_Vector2D* pPrevPos = &(*itCurrentPathPoint)->GetPos();
        for( auto itPathPoint = itNextPathPoint; itPathPoint != resultList_.end(); ++itPathPoint )
        {
            MT_Line lineTmp( *pPrevPos, (*itPathPoint)->GetPos() );
            TER_DistanceLess colCmp( *pPrevPos );
            T_PointSet collisions( colCmp );
            if( pObjectLocation->Intersect2D( lineTmp, collisions, epsilon ) )
            {
                if( collisions.empty() ) // should never happen
                    continue;
                rDistanceSum += pPrevPos->Distance( *collisions.begin() );
                if( !pObject || rDistanceSum < rDistance )
                {
                    rDistance = rDistanceSum;
                    pObject = pKnowledge;
                }
                break;
            }
            else
                rDistanceSum += pPrevPos->Distance( (*itPathPoint)->GetPos() );
            pPrevPos = &(*itPathPoint)->GetPos();
        }
    }
    if( !pObject )
        return false;
    return true;
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
// Name: DEC_PathResult::Serialize
// Created: LGY 2014-04-02
// -----------------------------------------------------------------------------
void DEC_PathResult::Serialize( sword::PathResult& msg ) const
{
    unsigned int index = 0;
    for( auto it = resultList_.begin(); it != resultList_.end(); ++it )
    {
        auto point = msg.add_points();
        const MT_Vector2D& position = (*it)->GetPos();
        const bool partial = (*it)->IsPartial();
        if( (*it)->IsWaypoint() || partial )
        {
            point->set_waypoint( index++ );
            point->set_reached( !partial );
        }
        NET_ASN_Tools::WritePoint( position, *point->mutable_coordinate() );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::AddResultPoint
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void DEC_PathResult::AddResultPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint, bool beginPoint )
{
    if( bSectionJustStarted_ )
    {
        bSectionJustStarted_ = false;
        // skip the first next point of the new section
        if( !resultList_.empty() )
            return;
    }
    if( !resultList_.empty() )
    {
        SlopeSpeedModifier slopeSpeedModifier;
        const PHY_RawVisionData& elevation = MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData();
        auto decelerationFunc = boost::bind( &SlopeSpeedModifier::ComputeLocalSlope, &slopeSpeedModifier, boost::cref( elevation ), _1, _2 );
        const MT_Vector2D& startPoint = resultList_.back()->GetPos();
        SplitOnMajorGridLines( static_cast< int32_t >( elevation.GetCellSize() ),
                startPoint, vPos, decelerationFunc );
        const SlopeSpeedModifier::T_Slopes& slopes = slopeSpeedModifier.GetSlopes();
        for( auto itSlope = slopes.begin(); itSlope != slopes.end(); ++itSlope )
        {
            auto itPoint = resultList_.rbegin();
            while( itPoint != resultList_.rend() && !( *itPoint )->IsSlopeValid() )
                ( *itPoint++ )->SetSlope( itSlope->second );
            if( itSlope->second > 0 && itSlope + 1 != slopes.end() )
            {
                const MT_Line segment( startPoint, vPos );
                const MT_Vector2D projected = segment.ProjectPointOnLine( ( itSlope + 1 )->first );
                auto point = boost::make_shared< DEC_PathPoint >( projected, nObjectTypes, nObjectTypesToNextPoint, beginPoint );
                resultList_.push_back( point );
            }
        }
    }
    auto point = boost::make_shared< DEC_PathPoint >( vPos, nObjectTypes, nObjectTypesToNextPoint, beginPoint );
    resultList_.push_back( point );
    if( resultList_.size() == 1 )
        itCurrentPathPoint_ = resultList_.begin();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::NotifySectionStarted
// Created: AGE 2005-09-01
// -----------------------------------------------------------------------------
void DEC_PathResult::NotifySectionStarted()
{
    bSectionJustStarted_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::NotifyPointReached
// Created: LDC 2012-01-18
// -----------------------------------------------------------------------------
void DEC_PathResult::NotifyPointReached( const T_PathPoints::const_iterator& itCurrentPathPoint )
{
    itCurrentPathPoint_ = itCurrentPathPoint;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::IsWaypoint
// Created: LDC 2012-01-18
// -----------------------------------------------------------------------------
bool DEC_PathResult::IsWaypoint( const MT_Vector2D& ) const
{
    return true;
}

//-----------------------------------------------------------------------------
// Name: DEC_PathResult::GetResult
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
const DEC_PathResult::T_PathPoints& DEC_PathResult::GetResult( bool useCheck ) const
{
    if( useCheck && GetState() == eComputing )
        MT_LOG_ERROR_MSG( "Path is being computed" );
    return resultList_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::GetPathType
// Created: CMA 2012-03-29
// -----------------------------------------------------------------------------
const DEC_PathType& DEC_PathResult::GetPathType() const
{
    return pathType_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::NotifyPartialSection
// Created: LGY 2014-04-03
// -----------------------------------------------------------------------------
void DEC_PathResult::NotifyPartialSection()
{
    if( !resultList_.empty() )
        resultList_.back()->NotifyPartial();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::NotifyCompletedSection
// Created: LGY 2014-04-03
// -----------------------------------------------------------------------------
void DEC_PathResult::NotifyCompletedSection()
{
    if( !resultList_.empty() )
        resultList_.back()->NotifyWaypoint();
}

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
#include "Entities/Objects/MIL_Object_ABC.h"
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
    , lastWaypoint_( 0 )
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
MT_Vector2D DEC_PathResult::GetPointOnPathCloseTo( const MT_Vector2D& posToTest ) const
{
    assert( !resultList_.empty() );
    CIT_PathPointList itStart = resultList_.begin();
    CIT_PathPointList itEnd   = resultList_.begin();
    ++itEnd;
    MT_Vector2D result( (*itStart)->GetPos() );
    double rDistance = std::numeric_limits< double >::max();
    for( itStart = resultList_.begin(); itEnd != resultList_.end(); ++itStart, ++itEnd )
    {
        MT_Line vLine( (*itStart)->GetPos(), (*itEnd)->GetPos() );
        MT_Vector2D vClosest = vLine.ClosestPointOnLine( posToTest );
        double rCurrentDistance = vClosest.SquareDistance( posToTest );
        if( rCurrentDistance < rDistance )
        {
            rDistance = rCurrentDistance;
            result = vClosest;
        }
    }
    assert( !_isnan( result.rX_ ) );
    assert( !_isnan( result.rY_ ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::GetNextPointOutsideObstacle
// Created: LDC 2011-01-12
// -----------------------------------------------------------------------------
MT_Vector2D DEC_PathResult::GetNextPointOutsideObstacle( const MT_Vector2D& posToTest, MIL_Object_ABC* obstacle, const MT_Vector2D* const lastWaypoint, bool forceNextPoint ) const
{
    if( resultList_.size() <= 1 )
        return posToTest;
    static const double rWeldValue = TER_World::GetWorld().GetWeldValue();
    CIT_PathPointList itEnd = resultList_.begin();
    ++itEnd;
    bool currentPositionReached = false;
    bool obstacleCrossed = false;
    boost::shared_ptr< TER_Localisation > obstacleLocalisation;
    if( obstacle )
    {
        TER_Localisation location = obstacle->GetLocalisation();
        obstacleLocalisation.reset( new TER_Localisation( location ) );
    }
    MT_Vector2D firstPositionAfterCurrent;
    for( CIT_PathPointList itStart = resultList_.begin(); itEnd != resultList_.end(); ++itStart, ++itEnd )
    {
        MT_Vector2D endPos = (*itEnd)->GetPos();
        if( !currentPositionReached )
        {
            MT_Line vLine( (*itStart)->GetPos(), endPos );
            if( vLine.IsInside( posToTest, rWeldValue ) )
            {
                currentPositionReached = true;
                firstPositionAfterCurrent = endPos;
                if( !obstacle )
                    return endPos;
            }
        }
        if( currentPositionReached )
        {
            MT_Line line( endPos, posToTest );
            obstacleCrossed |= ( obstacleLocalisation && obstacleLocalisation->Intersect2D( line ) );
            if( obstacleCrossed && ( !obstacleLocalisation || !obstacleLocalisation->IsInside( endPos ) ) )
                return endPos;
        }
    }
    // If we are already on a joining path or there is only one position crossing the obstacle:
    if( currentPositionReached )
        return forceNextPoint ? posToTest : firstPositionAfterCurrent;
    else if( !lastWaypoint )// jump to the last point after the obstacle. We may skip waypoints this way but at least we are not stuck and don't move backwards.
    {
        T_PathPointList::const_reverse_iterator ritEnd = resultList_.rbegin();
        ++ritEnd;
        for( T_PathPointList::const_reverse_iterator ritStart = resultList_.rbegin(); ritEnd != resultList_.rend(); ++ritStart, ++ritEnd )
        {
            MT_Vector2D endPos = (*ritEnd)->GetPos();
            MT_Line line( endPos, posToTest );
            obstacleCrossed |= ( obstacleLocalisation && obstacleLocalisation->Intersect2D( line ) );
            if( obstacleCrossed && ( !obstacleLocalisation || !obstacleLocalisation->IsInside( endPos )  ) )
                return endPos;
        }
    }
    else
    {
        itEnd = resultList_.begin();
        CIT_PathPointList itStart = itEnd;
        ++itEnd;
        while( itEnd != resultList_.end() )
        {
            ++itEnd;
            ++itStart;
            MT_Line vLine( (*itStart)->GetPos(), (*itEnd)->GetPos() );
            if( vLine.IsInside( *lastWaypoint, rWeldValue ) )
                break;
        }
        if( itEnd != resultList_.end() )
            ++itEnd; // Force to go after the way point we reached.
        for( ; itEnd != resultList_.end(); ++itEnd )
        {
            MT_Vector2D endPos = (*itEnd)->GetPos();
            if( !IsWaypoint( endPos ) )
                continue;
            MT_Line line( endPos, posToTest );
            obstacleCrossed |= ( obstacleLocalisation && obstacleLocalisation->Intersect2D( line ) );
            if( obstacleCrossed && ( !obstacleLocalisation || !obstacleLocalisation->IsInside( endPos ) ) )
                return endPos;
        }
    }
    return posToTest;
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
    MT_Vector2D startPos = (*itEnd)->GetPos();
    ++itEnd;
    CIT_PathPointList result = resultList_.end();
    int size = resultList_.size();
    int i = 0;
    for( CIT_PathPointList itStart = resultList_.begin(); itEnd != resultList_.end(); ++itStart, ++itEnd, ++i )
    {
        MT_Line vLine( (*itStart)->GetPos(), (*itEnd)->GetPos() );
        if( vLine.IsInside( vPos, rWeldValue ) ) //$$$ DE LA MERDE EN BOITE
        {
            if( i != size - 2 || result == resultList_.end() ) // $$$$ LDC Case you want to repeat and loop the movement.
                result = itStart;
        }
    }
    return result;
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
bool DEC_PathResult::ComputeFutureObjectCollision( const MT_Vector2D& vStartPos, const T_KnowledgeObjectVector& objectsToTest, double& rDistanceBefore, double& rDistanceAfter, boost::shared_ptr< DEC_Knowledge_Object >& pObject ) const
{
    rDistanceBefore = std::numeric_limits< double >::max();
    rDistanceAfter = std::numeric_limits< double >::max(); 
    pObject.reset();
    E_State nPathState = GetState();
    if( nPathState != eValid && nPathState != ePartial )
        return false;
    if( resultList_.empty() )
        return false;
    CIT_PathPointList itCurrentPathPoint = GetCurrentKeyOnPath( vStartPos );
    if( itCurrentPathPoint == resultList_.end() )
        return false;
    CIT_PathPointList itNextPathPoint = itCurrentPathPoint;
    ++itNextPathPoint;
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
                if( collisions.empty() ) // should never happen
                    continue;
                //$$$ Distance fausse (distance en ligne droite)
                const double rColDist = vStartPos.Distance( *collisions.begin() );
                if( !pObject )
                {
                    rDistanceBefore = rColDist;
                    pObject = pKnowledge;
                    if( collisions.size() > 1 )
                        rDistanceAfter = rColDist;
                }
                else
                    rDistanceAfter = rColDist;
            }
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
void DEC_PathResult::Serialize( sword::Path& asn ) const
{
    assert( !resultList_.empty() );
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

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::NotifyPointReached
// Created: LDC 2012-01-18
// -----------------------------------------------------------------------------
void DEC_PathResult::NotifyPointReached( const MT_Vector2D& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::IsWaypoint
// Created: LDC 2012-01-18
// -----------------------------------------------------------------------------
bool DEC_PathResult::IsWaypoint( const MT_Vector2D& ) const
{
    return true;
}
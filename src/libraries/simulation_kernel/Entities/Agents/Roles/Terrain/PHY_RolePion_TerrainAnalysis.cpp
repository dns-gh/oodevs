// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_TerrainAnalysis.h"
#include "MIL_AgentServer.h"
#include "Decision/DEC_GeometryFunctions.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Agents/Actions/Flying/PHY_RoleAction_InterfaceFlying.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_InterfaceMoving.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Objects/FloodAttribute.h"
#include "Entities/Objects/MIL_BurningCells.h"
#include "Entities/Orders/MIL_PionOrderManager.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "simulation_terrain/TER_Analyzer.h"
#include <spatialcontainer/TerrainData.h>

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_TerrainAnalysis constructor
// Created: MGD 2010-04-20
// -----------------------------------------------------------------------------
PHY_RolePion_TerrainAnalysis::PHY_RolePion_TerrainAnalysis( MIL_Agent_ABC& pion )
    : owner_               ( pion )
    , cacheRadius_         ( 3000.f )
    , cacheSafety_         ( 0.f )
    , crossroadsCacheValid_( false )
    , safetyCacheValid_    ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_TerrainAnalysis destructor
// Created: MGD 2010-04-20
// -----------------------------------------------------------------------------
PHY_RolePion_TerrainAnalysis::~PHY_RolePion_TerrainAnalysis()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_TerrainAnalysis::GetCrossroads
// Created: MGD 2010-04-20
// -----------------------------------------------------------------------------
void PHY_RolePion_TerrainAnalysis::GetCrossroads( std::vector< boost::shared_ptr< MT_Vector2D > >& points )
{
    CheckFuseau();
    if( !crossroadsCacheValid_ )
        UpdateCrossroads();
    for( std::map< MT_Vector2D, boost::shared_ptr< MT_Vector2D > >::const_iterator it = crossroadsBuffer_.begin(); it != crossroadsBuffer_.end(); ++it )
        points.push_back( it->second );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_TerrainAnalysis::CheckFuseau
// Created: LDC 2010-10-28
// -----------------------------------------------------------------------------
void PHY_RolePion_TerrainAnalysis::CheckFuseau()
{
    const MIL_Fuseau& fuseau = owner_.GetOrderManager().GetFuseau();
    if( fuseau_ != fuseau  )
    {
        crossroadsCacheValid_ = false;
        safetyCacheValid_ = false;
        fuseau_ = fuseau;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_TerrainAnalysis::NotifyHasMoved
// Created: MGD 2010-04-20
// -----------------------------------------------------------------------------
void PHY_RolePion_TerrainAnalysis::NotifyHasMoved( const MT_Vector2D& newPos )
{
    const double range = std::min( std::max( owner_.GetRole< PHY_RoleInterface_Perceiver >().GetMaxAgentPerceptionDistance(), 3000. ), (double)cacheRadius_ );
    const double squareRange = range * range;
    if( lastPos_.SquareDistance( newPos ) > squareRange * 0.25 )
    {
        safetyCacheValid_ = false;
        crossroadsCacheValid_ = false;
        lastPos_ = newPos;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_TerrainAnalysis::UpdateCrossroads
// Created: LDC 2010-10-28
// -----------------------------------------------------------------------------
void PHY_RolePion_TerrainAnalysis::UpdateCrossroads()
{
    const double range = std::min( std::max( owner_.GetRole< PHY_RoleInterface_Perceiver >().GetMaxAgentPerceptionDistance(), 3000. ), (double)cacheRadius_ );
    const double squareRange = range * range;
    //Remove old points out of range
    //Note that it's better than clearing the buffer as it allows to keep the same shared ptr so directia won't spawn a new node
    //for a changed shared_ptr while the position remains the same.
    for( auto it = crossroadsBuffer_.begin(); it != crossroadsBuffer_.end(); )
    {
        if( it->second->SquareDistance( lastPos_ ) > squareRange )
            it = crossroadsBuffer_.erase( it );
        else
            ++it;
    }
    //Add new points
    std::vector< boost::shared_ptr< MT_Vector2D > > temp = TER_Analyzer::GetAnalyzer().FindCrossroadsWithinCircle( lastPos_, static_cast< float >( range ) );
    for( auto it = temp.begin(); it != temp.end(); ++it )
        crossroadsBuffer_.insert( std::pair< MT_Vector2D, boost::shared_ptr< MT_Vector2D > >( **it, *it ) );
    //Remove outside fuseau
    if( !fuseau_.IsNull() )
    {
        for( auto it = crossroadsBuffer_.begin(); it != crossroadsBuffer_.end(); )
        {
            if( !fuseau_.IsInside( it->first ) )
                it = crossroadsBuffer_.erase( it );
            else
                ++it;
        }
    }
    crossroadsCacheValid_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_TerrainAnalysis::UpdateSafety
// Created: LDC 2010-10-28
// -----------------------------------------------------------------------------
void PHY_RolePion_TerrainAnalysis::UpdateSafety( float radius, float safetyDistance )
{
    const double squareRange = radius * radius;
    //Remove old points out of range
    //Note that it's better than clearing the buffer as it allows to keep the same shared ptr so directia won't spawn a new node
    //for a changed shared_ptr while the position remains the same.
    for( auto it = safetyBuffer_.begin(); it != safetyBuffer_.end(); )
    {
        if( it->second->SquareDistance( lastPos_ ) > squareRange )
            it = safetyBuffer_.erase( it );
        else
            ++it;
    }
    //Add new points
    std::vector< boost::shared_ptr< MT_Vector2D > > positions;
    TER_Analyzer::GetAnalyzer().FindSafetyPositionsWithinCircle( lastPos_, radius, safetyDistance, positions );
    for( auto it = positions.begin(); it != positions.end(); ++it )
        safetyBuffer_.insert( std::pair< MT_Vector2D, boost::shared_ptr< MT_Vector2D > >( **it, *it ) );
    //Remove outside fuseau
    if( !fuseau_.IsNull() )
    {
        for( auto it = safetyBuffer_.begin(); it != safetyBuffer_.end(); )
        {
            if( !fuseau_.IsInside( it->first ) )
                it = safetyBuffer_.erase( it );
            else
                ++it;
        }
    }
    safetyCacheValid_ = true;
    cacheRadius_ = radius;
    cacheSafety_ = safetyDistance;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_TerrainAnalysis::FindSafetyPositionsWithinCircle
// Created: LDC 2010-10-28
// -----------------------------------------------------------------------------
void PHY_RolePion_TerrainAnalysis::FindSafetyPositionsWithinCircle( std::vector< boost::shared_ptr< MT_Vector2D > >& points, float radius, float safetyDistance )
{
    if( radius <= 0.f || safetyDistance <= 0.f )
        return;
    CheckFuseau();
    if( !safetyCacheValid_ || cacheRadius_ != radius || cacheSafety_ != safetyDistance )
        UpdateSafety( radius, safetyDistance );
    for( std::map< MT_Vector2D, boost::shared_ptr< MT_Vector2D > >::const_iterator it = safetyBuffer_.begin(); it != safetyBuffer_.end(); ++it )
        points.push_back( it->second );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_TerrainAnalysis::CanMoveOnTerrain
// Created: CMA 2011-08-30
// -----------------------------------------------------------------------------
bool PHY_RolePion_TerrainAnalysis::CanMoveOnTerrain( const std::vector< MT_Vector2D >& points ) const
{
    if( owner_.GetRole< PHY_RoleAction_InterfaceFlying >().IsFlying() || points.empty() )
        return true;
    for( std::vector< MT_Vector2D >::const_iterator it = points.begin(); it != points.end(); ++it )
    {
        const TerrainData data = TER_Analyzer::GetAnalyzer().Pick( *it );
        double maxSpeed = owner_.GetRole< moving::PHY_RoleAction_InterfaceMoving >().GetMaxSpeed( data );
        if( maxSpeed > 0 )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_TerrainAnalysis::CanMoveOnUrbanBlock
// Created: CMA 2011-08-30
// -----------------------------------------------------------------------------
bool PHY_RolePion_TerrainAnalysis::CanMoveOnUrbanBlock( const std::vector< MT_Vector2D >& points ) const
{
    if( owner_.GetType().GetUnitType().CanFly() || points.empty() )
        return true;
    double weight = owner_.GetRole< PHY_RoleInterface_Composantes >().GetMaxWeight();
    for( std::vector< MT_Vector2D >::const_iterator it = points.begin(); it != points.end(); ++it )
        if( DEC_GeometryFunctions::IsUrbanBlockTrafficable( *it, weight ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_TerrainAnalysis::CanMoveOnBurningCells
// Created: CMA 2011-08-30
// -----------------------------------------------------------------------------
bool PHY_RolePion_TerrainAnalysis::CanMoveOnBurningCells( const std::vector< MT_Vector2D >& points ) const
{
    if( owner_.GetRole< PHY_RoleAction_InterfaceFlying >().IsFlying() || points.size() <= 1 )
    return true;
    std::vector< MT_Vector2D >::const_iterator itStart = points.begin();
    std::vector< MT_Vector2D >::const_iterator itEnd = points.begin();
    ++itEnd;
    for( ; itEnd != points.end(); ++itStart, ++itEnd )
        if( MIL_AgentServer::GetWorkspace().GetBurningCells().IsTrafficable( *itStart, *itEnd ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_TerrainAnalysis::CanMoveOnKnowledgeObject
// Created: CMA 2011-08-30
// -----------------------------------------------------------------------------
bool PHY_RolePion_TerrainAnalysis::CanMoveOnKnowledgeObject( const std::vector< MT_Vector2D >& points ) const
{
    if( owner_.GetRole< PHY_RoleAction_InterfaceFlying >().IsFlying() || points.empty() )
        return true;

    T_KnowledgeObjectVector knowledgesObject;
    auto bbKg = owner_.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetKnowledgeObjectContainer().GetObjects( knowledgesObject );
    if( knowledgesObject.empty() )
        return true;

    for( auto itPoint = points.begin(); itPoint != points.end(); ++itPoint )
    {
        bool isInside = false;
        T_PointVector pathPoints;
        pathPoints.push_back( *itPoint );
        for( auto itKnowledge = knowledgesObject.begin(); itKnowledge != knowledgesObject.end(); ++itKnowledge )
        {
            const DEC_Knowledge_Object& knowledge = **itKnowledge;
            if( knowledge.CanCollideWith( owner_ ) && knowledge.RetrieveAttribute< FloodAttribute >() != 0 && knowledge.IsObjectInsidePathPoint( pathPoints, &owner_ ) )
            {
                isInside = true;
                break;
            }
        }
        if( !isInside )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_TerrainAnalysis::CanMoveOn
// Created: CMA 2011-08-30
// -----------------------------------------------------------------------------
bool PHY_RolePion_TerrainAnalysis::CanMoveOn( const MT_Vector2D& position ) const
{
    std::vector< MT_Vector2D > points;
    points.push_back( position );
    return CanMoveOnTerrain( points ) && CanMoveOnUrbanBlock( points ) && CanMoveOnBurningCells( points ) && CanMoveOnKnowledgeObject( points );
}

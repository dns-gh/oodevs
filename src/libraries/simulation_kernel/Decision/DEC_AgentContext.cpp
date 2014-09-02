// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_AgentContext.h"
#include "DEC_Agent_PathClass.h"
#include "DEC_Path_KnowledgeAgent.h"
#include "DEC_Path_KnowledgeObject.h"
#include "DEC_Path_KnowledgeObjectFlood.h"
#include "DEC_Path_KnowledgeObjectBurnSurface.h"
#include "DEC_Path_KnowledgeObjectDisaster.h"
#include "DEC_Path_KnowledgePopulation.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RoleInterface_Reinforcement.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Objects/BurnSurfaceCapacity.h"
#include "Entities/Objects/DisasterCapacity.h"
#include "Entities/Objects/FloodCapacity.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/TrafficabilityCapacity.h"
#include "Entities/Orders/MIL_PionOrderManager.h"
#include "Entities/Orders/MIL_AutomateOrderManager.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include <boost/make_shared.hpp>

namespace
{
    double GetWeight( const MIL_Agent_ABC& queryMaker )
    {
        double weight = queryMaker.GetRole< PHY_RoleInterface_Composantes >().GetMaxWeight();
        const auto& reinforcements = queryMaker.GetRole< PHY_RoleInterface_Reinforcement >().GetReinforcements();
        for( auto it = reinforcements.begin(); it != reinforcements.end(); ++it )
            weight = std::max( weight, (*it)->GetRole< PHY_RoleInterface_Composantes >().GetMaxWeight() );
        return weight;
    }
}

DEC_AgentContext::DEC_AgentContext( const MIL_Agent_ABC& agent, const DEC_Agent_PathClass& pathClass,
    const T_PointVector& points, bool ignoreDynamicObjects )
    : fuseau_( agent.GetOrderManager().GetFuseau() )
    , automataFuseau_( agent.GetAutomate().GetOrderManager().GetFuseau() )
    , dangerDirection_( agent.GetOrderManager().GetDirDanger() )
    , maxSlope_( agent.GetRole< moving::PHY_RoleAction_Moving >().GetMaxSlope() )
    , slopeDeceleration_( agent.GetRole< moving::PHY_RoleAction_Moving >().GetSlopeDeceleration() )
    , majorWeight_( GetWeight( agent ) )
    , costOutsideOfAllObjects_( 0 )
    , class_( pathClass )
    , speeds_( agent.GetRole< moving::PHY_RoleAction_Moving >() )
{
    if( !ignoreDynamicObjects )
    {
        InitializeAgentKnowledges( agent );
        InitializeObjectKnowledges( agent, points );
        InitializePopulationKnowledges( agent );
    }
}

void DEC_AgentContext::InitializeAgentKnowledges( const MIL_Agent_ABC& agent )
{
    if( !class_.AvoidEnemies() )
        return;
    if( auto bbKg = agent.GetKnowledgeGroup()->GetKnowledge() )
    {
        const auto& enemies = bbKg->GetEnemies();
        for( auto it = enemies.begin(); it != enemies.end(); ++it )
        {
            const DEC_Knowledge_Agent& knowledge = **it;
            if( knowledge.IsValid() && fuseau_.IsInside( knowledge.GetPosition() ) )
            {
                const double factor = class_.GetEnemyCostOnContact();
                if( factor > 0 )
                    pathKnowledgeAgents_.push_back( DEC_Path_KnowledgeAgent( knowledge.GetPosition(),
                        class_.GetEnemyCostAtSecurityRange(), factor, knowledge.GetMaxRangeToFireOn( agent, 0 ) ) );
            }
        }
    }
}

void DEC_AgentContext::InitializeObjectKnowledges( const MIL_Agent_ABC& agent, const T_PointVector& points )
{
    if( !class_.AvoidObjects() )
        return;
    T_KnowledgeObjectVector knowledgesObject;
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = agent.GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        container->GetObjectsAtInteractionHeight( knowledgesObject, agent, MIL_PathObjectFilter() );
    T_PointVector firstPointVector;
    if( !points.empty() )
        firstPointVector.push_back( *points.begin() );
    for( auto it = knowledgesObject.begin(); it != knowledgesObject.end(); ++it )
    {
        const DEC_Knowledge_Object& knowledge = **it;
        if( knowledge.CanCollideWith( agent ) )
        {
            if( knowledge.IsObjectInsidePathPoint( firstPointVector, &agent ) )
            {
                if( const MIL_Object_ABC* pObject = knowledge.GetObjectKnown() )
                {
                    TerrainData data;
                    double rMaxSpeed = agent.GetRole< moving::PHY_RoleAction_Moving >().GetSpeed( data, *pObject );
                    if( rMaxSpeed == 0. || rMaxSpeed == std::numeric_limits< double >::max() )
                        continue;
                }
            }

            if( pathKnowledgeObjects_.size() <= knowledge.GetType().GetID() )
                pathKnowledgeObjects_.resize( knowledge.GetType().GetID() + 1 );

            T_PathKnowledgeObjectVector& pathKnowledges = pathKnowledgeObjects_[ knowledge.GetType().GetID() ];
            bool empty = pathKnowledges.empty();
            if( knowledge.GetType().GetCapacity< FloodCapacity >() )
                pathKnowledges.push_back( boost::make_shared< DEC_Path_KnowledgeObjectFlood >( agent.GetType().GetUnitType().GetCrossingHeight(), knowledge ) );
            else if( knowledge.GetType().GetCapacity< BurnSurfaceCapacity >() )
                pathKnowledges.push_back( boost::make_shared< DEC_Path_KnowledgeObjectBurnSurface >( knowledge ) );
            else if( knowledge.GetType().GetCapacity< DisasterCapacity >() )
                pathKnowledges.push_back( boost::make_shared< DEC_Path_KnowledgeObjectDisaster >( agent, knowledge ) );
            else if( ( class_.GetObjectCost( knowledge.GetType() ) != 0 && knowledge.GetLocalisation().GetType() != TER_Localisation::eNone )
                    || knowledge.HasAgentMaxSpeedMultiplier()
                    || knowledge.GetType().GetCapacity< TrafficabilityCapacity >() )
                pathKnowledges.push_back( boost::make_shared< DEC_Path_KnowledgeObject >( knowledge, class_.GetObjectCost( knowledge.GetType() ), class_.GetThreshold() ) );
            if( empty && pathKnowledges.size() == 1 && pathKnowledges.front()->GetCostOut() > 0 )
                costOutsideOfAllObjects_ += pathKnowledges.front()->GetCostOut();
        }
    }
}

void DEC_AgentContext::InitializePopulationKnowledges( const MIL_Agent_ABC& agent )
{
    if( !class_.HandlePopulations() )
        return;
    if( auto bbKg = agent.GetKnowledgeGroup()->GetKnowledge() )
    {
        T_KnowledgePopulationVector knowledgesPopulation;
        bbKg->GetPopulations( knowledgesPopulation );
        pathKnowledgePopulations_.reserve( knowledgesPopulation.size() );
        for( auto it = knowledgesPopulation.begin(); it != knowledgesPopulation.end(); ++it )
            pathKnowledgePopulations_.push_back( boost::make_shared< DEC_Path_KnowledgePopulation >( **it, class_, !agent.GetType().IsTerrorist() ) );
    }
}

const MIL_Fuseau& DEC_AgentContext::GetFuseau() const
{
    return fuseau_;
}

const MIL_Fuseau& DEC_AgentContext::GetAutomataFuseau() const
{
    return automataFuseau_;
}

const PHY_Speeds& DEC_AgentContext::GetUnitSpeeds() const
{
    return speeds_;
}

const MT_Vector2D& DEC_AgentContext::GetDirDanger() const
{
    return dangerDirection_;
}

const DEC_Agent_PathClass& DEC_AgentContext::GetPathClass() const
{
    return class_;
}

double DEC_AgentContext::GetUnitMaxSlope() const
{
    return maxSlope_;
}

double DEC_AgentContext::GetUnitSlopeDeceleration() const
{
    return slopeDeceleration_;
}

double DEC_AgentContext::GetUnitMajorWeight() const
{
    return majorWeight_;
}

double DEC_AgentContext::GetCostOutsideOfAllObjects() const
{
    return costOutsideOfAllObjects_;
}

const DEC_AgentContext::T_PathKnowledgeAgentVector& DEC_AgentContext::GetPathKnowledgeAgents() const
{
    assert( class_.AvoidEnemies() || pathKnowledgeAgents_.empty() );
    return pathKnowledgeAgents_;
}

const DEC_AgentContext::T_PathKnowledgeObjectByTypesVector& DEC_AgentContext::GetPathKnowledgeObjects() const
{
    assert( class_.AvoidObjects() || pathKnowledgeObjects_.empty() );
    return pathKnowledgeObjects_;
}

const DEC_AgentContext::T_PathKnowledgePopulationVector& DEC_AgentContext::GetPathKnowledgePopulations() const
{
    assert( class_.HandlePopulations() || pathKnowledgePopulations_.empty() );
    return pathKnowledgePopulations_;
}

// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_KnowledgeFunctions.cpp $
// $Author: Nld $
// $Modtime: 23/03/05 18:22 $
// $Revision: 7 $
// $Workfile: DEC_KnowledgeFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KnowledgeFunctions.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/MIL_Army.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Urban.h"

#include "urban/TerrainObject_ABC.h"

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetDetectedAgentsInFuseau
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetDetectedAgentsInFuseau( const MIL_AgentPion& callerAgent )
{
    T_ConstKnowledgeAgentVector knowledges;
    callerAgent.GetKnowledgeGroup().GetKnowledge().GetDetectedAgentsInZone( knowledges, callerAgent.GetOrderManager().GetFuseau() );

    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetDetectedAgentsInZone
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetDetectedAgentsInZone( const MIL_AgentPion& callerAgent, const TER_Localisation* area )
{
    assert( area );
    T_ConstKnowledgeAgentVector knowledges;
    callerAgent.GetKnowledgeGroup().GetKnowledge().GetDetectedAgentsInZone( knowledges, *area );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetAgentsAttacking
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetAgentsAttacking( const MIL_AgentPion& callerAgent )
{
    T_ConstKnowledgeAgentVector attackers;
    callerAgent.GetKnowledge().GetAgentsAttacking( attackers );
    return attackers;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetAgentsAttackingAlly
// Created: DDA 2010-06-02
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector  DEC_KnowledgeFunctions::GetAgentsAttackingAlly( const DEC_Decision_ABC* agentAlly)
{
    T_ConstKnowledgeAgentVector attackers;
    (*agentAlly).GetPion().GetKnowledge().GetAgentsAttacking( attackers );
    return attackers;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetDangerousEnemies
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetDangerousEnemies( const MIL_AgentPion& callerAgent )
{
    return callerAgent.GetKnowledge().GetDangerousEnemies();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetLivingEnemiesPerceived
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetLivingEnemiesPerceived( const MIL_AgentPion& callerAgent )
{
    T_ConstKnowledgeAgentVector knowledges;
    callerAgent.GetKnowledge().GetLivingEnemiesPerceived( knowledges );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedInFuseau
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedInFuseau( const MIL_AgentPion& callerAgent )
{
    T_ConstKnowledgeAgentVector knowledges;
    callerAgent.GetKnowledge().GetLivingEnemiesPerceivedInZone( knowledges, callerAgent.GetOrderManager().GetFuseau() );

    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetLivingEnemiesInZone
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetLivingEnemiesInZone( const MIL_AgentPion& callerAgent, TER_Localisation* pZone )
{
    assert( pZone );

    T_ConstKnowledgeAgentVector knowledges;
    callerAgent.GetKnowledgeGroup().GetKnowledge().GetLivingEnemiesInZone( knowledges, *pZone );

    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetLivingEnemiesInFuseau
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetLivingEnemiesInFuseau( const MIL_AgentPion& callerAgent )
{
    T_ConstKnowledgeAgentVector knowledges;
    callerAgent.GetKnowledgeGroup().GetKnowledge().GetLivingEnemiesInZone( knowledges, callerAgent.GetOrderManager().GetFuseau() );

    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetLivingEnemiesInFuseau
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetLivingEnemiesInCircle( const MIL_AgentPion& callerAgent, const MT_Vector2D* pCenter, float radius )
{
    assert( pCenter );

    T_ConstKnowledgeAgentVector knowledges;
    callerAgent.GetKnowledgeGroup().GetKnowledge().GetLivingEnemiesInCircle( knowledges, *pCenter, radius );

    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObservableKnowledge
// Created: MGD 2010-02-09
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetObservableKnowledge( directia::brain::Brain& brain, const MIL_AgentPion& pion, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& table )
{
    //Agents
    knowledgeCreateFunction( table, brain[ "integration.ontology.types.agentKnowledge" ], pion.GetKnowledgeGroup().GetKnowledge().GetEnemies(), true );

    //Object
    T_KnowledgeObjectVector objectsKn;
    pion.GetArmy().GetKnowledge().GetObjects( objectsKn );

    knowledgeCreateFunction( table, brain[ "integration.ontology.types.object" ], objectsKn, true );

    //Urban
    T_KnowledgeUrbanVector urbansKn;
    pion.GetArmy().GetKnowledge().GetUrbanObjects( urbansKn );

    knowledgeCreateFunction( table, brain[ "integration.ontology.types.urbanBlock" ], urbansKn, true );

}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetUrbanBlockKnowledge
// Created: DDA 2010-03-15
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetUrbanBlockKnowledge( directia::brain::Brain& brain, const MIL_AgentPion& pion, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& table )
{
    //Urban
    T_KnowledgeUrbanVector urbansKn;
    pion.GetArmy().GetKnowledge().GetUrbanObjects( urbansKn );

    knowledgeCreateFunction( table, brain[ "integration.ontology.types.urbanBlock" ], urbansKn, true );

}
// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetUrbanBlockKnowledgeInCircle
// Created: DDA 2010-03-16
// -----------------------------------------------------------------------------
T_KnowledgeUrbanVector DEC_KnowledgeFunctions::GetUrbanBlockKnowledgeInCircle( const MIL_AgentPion& pion, boost::shared_ptr< MT_Vector2D >& center, float radius )
{
    //Urban
    T_KnowledgeUrbanVector urbansKn;
    T_KnowledgeUrbanVector result;
    pion.GetArmy().GetKnowledge().GetUrbanObjects( urbansKn );
    geometry::Point2f geoCenter( (float)center->rX_, (float)center->rY_ );

    for( T_KnowledgeUrbanVector::iterator it = urbansKn.begin(); it != urbansKn.end(); it++ )
    {
        if( (*it)->GetTerrainObjectKnown().GetFootprint()->Intersect( geoCenter, radius ) )
            result.push_back( (*it) );
    }

    return result;

}


// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetDestroyableKnowledge
// Created: MGD 2010-02-10
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetDestroyableKnowledge( directia::brain::Brain& brain, const MIL_AgentPion& pion, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& table )
{
    //Agents //@TODO Add private tools function
    knowledgeCreateFunction( table, brain[ "integration.ontology.types.agentKnowledge" ], pion.GetKnowledgeGroup().GetKnowledge().GetEnemies(), true );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetIdentifiableKnowledge
// Created: PSN & GGE 2010-03-25
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetIdentifiableKnowledge( directia::brain::Brain& brain, const MIL_AgentPion& pion, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& table )
{
    //Agents //@TODO Add private tools function
    knowledgeCreateFunction( table, brain[ "integration.ontology.types.agentKnowledge" ], pion.GetKnowledgeGroup().GetKnowledge().GetEnemies(), true );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetNearbyRefugees
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetNearbyRefugees( const MIL_AgentPion& callerAgent, double radius )
{
    T_ConstKnowledgeAgentVector knowledges;
    callerAgent.GetKnowledgeGroup().GetKnowledge().GetRefugeesInCircle( knowledges, callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition(), radius );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetNearbySurrenderedAgents
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetNearbySurrenderedAgents( const MIL_AgentPion& callerAgent, double radius )
{
    T_ConstKnowledgeAgentVector knowledges;
    callerAgent.GetKnowledgeGroup().GetKnowledge().GetSurrenderedAgentsInCircle( knowledges, callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition(), radius );

    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::EnemyPresenceInCircle
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
bool DEC_KnowledgeFunctions::EnemyPresenceInCircle( const MIL_AgentPion& callerAgent, const MT_Vector2D* center, double radius )
{
    assert( center );
    return callerAgent.GetKnowledgeGroup().GetKnowledge().EnemyPresenceInCircle( *center, radius );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetRapForLocal
// Created: NLD 2004-04-08
// -----------------------------------------------------------------------------
double DEC_KnowledgeFunctions::GetRapForLocal( const MIL_AgentPion& callerAgent )
{
    return callerAgent.GetKnowledge().GetRapForLocalValue();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetClosestObject
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeFunctions::GetClosestObject( const MIL_Agent_ABC& callerAgent, const std::string& type )
{
    std::vector< std::string > typeList;
    typeList.push_back( type );
    const MIL_ObjectFilter filter( typeList );
    return callerAgent.GetArmy().GetKnowledge().GetClosestObject( callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition(), filter );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetClosestFriendObject
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeFunctions::GetClosestFriendObject( const MIL_Agent_ABC& callerAgent, const std::string& type )
{
    std::vector< std::string > typeList;
    typeList.push_back( type );
    const MIL_ObjectFilter filter( typeList );
    return callerAgent.GetArmy().GetKnowledge().GetClosestFriendObject( callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition(), filter );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObjectsColliding
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
T_KnowledgeObjectDiaIDVector DEC_KnowledgeFunctions::GetObjectsColliding( const MIL_AgentPion& callerAgent )
{
    T_KnowledgeObjectDiaIDVector objectsColliding;
    callerAgent.GetKnowledge().GetObjectsColliding( objectsColliding );

    return objectsColliding;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetPopulationsColliding
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
T_KnowledgePopulationDiaIDVector DEC_KnowledgeFunctions::GetPopulationsColliding( const MIL_AgentPion& callerAgent )
{
    T_KnowledgePopulationDiaIDVector populationsColliding;
    callerAgent.GetKnowledge().GetPopulationsColliding( populationsColliding );

    return populationsColliding;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetPopulationsAttacking
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
T_KnowledgePopulationDiaIDVector DEC_KnowledgeFunctions::GetPopulationsAttacking( const MIL_AgentPion& callerAgent )
{
    T_KnowledgePopulationDiaIDVector attackers;
    callerAgent.GetKnowledge().GetPopulationsAttacking( attackers );

    return attackers;
}


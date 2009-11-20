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

#include "DEC_FunctionsTools.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_Army.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "Decision/DEC_Tools.h"

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
// Name: DEC_KnowledgeFunctions::GetNearbyRefugees
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetNearbyRefugees( const MIL_AgentPion& callerAgent, MT_Float radius )
{
    T_ConstKnowledgeAgentVector knowledges;
    callerAgent.GetKnowledgeGroup().GetKnowledge().GetRefugeesInCircle( knowledges, callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition(), radius );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetNearbySurrenderedAgents
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetNearbySurrenderedAgents( const MIL_AgentPion& callerAgent, MT_Float radius )
{
    T_ConstKnowledgeAgentVector knowledges;
    callerAgent.GetKnowledgeGroup().GetKnowledge().GetSurrenderedAgentsInCircle( knowledges, callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition(), radius );

    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::EnemyPresenceInCircle
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
bool DEC_KnowledgeFunctions::EnemyPresenceInCircle( const MIL_AgentPion& callerAgent, const MT_Vector2D* center, MT_Float radius )
{
    assert( center );
    return callerAgent.GetKnowledgeGroup().GetKnowledge().EnemyPresenceInCircle( *center, radius );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetRapForLocal
// Created: NLD 2004-04-08
// -----------------------------------------------------------------------------
MT_Float DEC_KnowledgeFunctions::GetRapForLocal( const MIL_AgentPion& callerAgent )
{
    return callerAgent.GetKnowledge().GetRapForLocalValue();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetClosestObject
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeFunctions::GetClosestObject( const MIL_AgentPion& callerAgent, const std::string& type )
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
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeFunctions::GetClosestFriendObject( const MIL_AgentPion& callerAgent, const std::string& type )
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


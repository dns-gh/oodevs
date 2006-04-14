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

#include "MIL_pch.h"
#include "DEC_KnowledgeFunctions.h"

#include "DEC_FunctionsTools.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_Army.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetAgentsAttacking
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetAgentsAttacking( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    T_KnowledgeAgentDiaIDVector attackers;
    callerAgent.GetKnowledge().GetAgentsAttacking( attackers );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( attackers, DEC_Tools::GetTypeConnaissanceAgent() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetDangerousEnemies
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetDangerousEnemies( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( callerAgent.GetKnowledge().GetDangerousEnemies(), DEC_Tools::GetTypeConnaissanceAgent() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetLivingEnemiesPerceived
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetLivingEnemiesPerceived( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    T_KnowledgeAgentDiaIDVector knowledges;
    callerAgent.GetKnowledge().GetLivingEnemiesPerceived( knowledges );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceAgent() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedInFuseau
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedInFuseau( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    T_KnowledgeAgentDiaIDVector knowledges;
    callerAgent.GetKnowledge().GetLivingEnemiesPerceivedInZone( knowledges, callerAgent.GetFuseau() );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceAgent() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetDetectedAgentsInFuseau
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetDetectedAgentsInFuseau( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    T_KnowledgeAgentDiaIDVector knowledges;
    callerAgent.GetKnowledgeGroup().GetKnowledge().GetDetectedAgentsInZone( knowledges, callerAgent.GetFuseau() );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceAgent() );


}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetEnemiesInZone
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetEnemiesInZone( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) );
    TER_Localisation* pZone = call.GetParameter( 0 ).ToUserPtr( pZone );
    assert( pZone );

    T_KnowledgeAgentDiaIDVector knowledges;
    callerAgent.GetKnowledgeGroup().GetKnowledge().GetEnemiesInZone( knowledges, *pZone );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceAgent() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetNearbyRefugees
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetNearbyRefugees( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    T_KnowledgeAgentDiaIDVector knowledges;
    callerAgent.GetKnowledgeGroup().GetKnowledge().GetRefugeesInCircle( knowledges, callerAgent.GetRole< PHY_RolePion_Location >().GetPosition(), call.GetParameter( 0 ).ToFloat() );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceAgent() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetNearbySurrenderedAgents
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetNearbySurrenderedAgents( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    T_KnowledgeAgentDiaIDVector knowledges;
    callerAgent.GetKnowledgeGroup().GetKnowledge().GetSurrenderedAgentsInCircle( knowledges, callerAgent.GetRole< PHY_RolePion_Location >().GetPosition(), call.GetParameter( 0 ).ToFloat() );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceAgent() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::EnemyPresenceInCircle
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::EnemyPresenceInCircle( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 0 ) ) );

    const MT_Vector2D* pCenter = call.GetParameter( 0 ).ToUserPtr( pCenter );
    assert( pCenter );

    call.GetResult().SetValue( callerAgent.GetKnowledgeGroup().GetKnowledge().EnemyPresenceInCircle( *pCenter, call.GetParameter( 1 ).ToFloat() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetRapForLocal
// Created: NLD 2004-04-08
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetRapForLocal( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( (float)callerAgent.GetKnowledge().GetRapForLocalValue() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetRapForGlobal
// Created: NLD 2004-04-08
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetRapForGlobal( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    call.GetResult().SetValue( (float)callerAutomate.GetKnowledge().GetRapForGlobalValue() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetClosestObject
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetClosestObject( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    MIL_RealObjectTypeFilter objectsFilter( call.GetParameters(), 0 );
    
    DEC_Knowledge_Object* pKnowledge = callerAgent.GetArmy().GetKnowledge().GetClosestObject( callerAgent.GetRole< PHY_RolePion_Location >().GetPosition(), objectsFilter );
    call.GetResult().SetValue( (void*)( pKnowledge ? pKnowledge->GetID() : 0 ), &DEC_Tools::GetTypeConnaissanceObjet() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetClosestFriendObject
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetClosestFriendObject( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    MIL_RealObjectTypeFilter objectsFilter( call.GetParameters(), 0 );
    
    DEC_Knowledge_Object* pKnowledge = callerAgent.GetArmy().GetKnowledge().GetClosestFriendObject( callerAgent.GetRole< PHY_RolePion_Location >().GetPosition(), objectsFilter );
    call.GetResult().SetValue( (void*)( pKnowledge ? pKnowledge->GetID() : 0 ), &DEC_Tools::GetTypeConnaissanceObjet() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObjectsColliding
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetObjectsColliding( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    T_KnowledgeObjectDiaIDVector objectsColliding;
    callerAgent.GetKnowledge().GetObjectsColliding( objectsColliding );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( objectsColliding, DEC_Tools::GetTypeConnaissanceObjet() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetPopulationsColliding
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetPopulationsColliding( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    T_KnowledgePopulationDiaIDVector populationsColliding;
    callerAgent.GetKnowledge().GetPopulationsColliding( populationsColliding );

    DIA_Variable_ObjectList& diaPopulationList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaPopulationList.SetValueUserType( populationsColliding, DEC_Tools::GetTypeConnaissancePopulation() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetPopulationsAttacking
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetPopulationsAttacking( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    T_KnowledgePopulationDiaIDVector attackers;
    callerAgent.GetKnowledge().GetPopulationsAttacking( attackers );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( attackers, DEC_Tools::GetTypeConnaissancePopulation() );
}


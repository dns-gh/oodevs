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
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetAgentsAttacking
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetAgentsAttacking( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    T_KnowledgeAgentDiaIDVector attackers;
    callerAgent.GetKSQuerier().GetAgentsAttacking( attackers );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( attackers, DEC_Tools::GetTypeConnaissanceAgent() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetPopulationsAttacking
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetPopulationsAttacking( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    T_KnowledgePopulationDiaIDVector attackers;
    callerAgent.GetKSQuerier().GetPopulationsAttacking( attackers );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( attackers, DEC_Tools::GetTypeConnaissancePopulation() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetPotentiallyDangerousAgents
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetPotentiallyDangerousAgents( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    T_KnowledgeAgentDiaIDVector dangerousKnowledges;
    callerAgent.GetKSQuerier().GetPotentiallyDangerousAgents( dangerousKnowledges );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( dangerousKnowledges, DEC_Tools::GetTypeConnaissanceAgent() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetMilitiaAgents
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetMilitiaAgents( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    T_KnowledgeAgentDiaIDVector knowledges;
    callerAgent.GetKSQuerier().GetMilitiaAgents( knowledges );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceAgent() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetRefugeeAgents
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetRefugeeAgents( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    T_KnowledgeAgentDiaIDVector knowledges;
    callerAgent.GetKSQuerier().GetRefugeeAgents( knowledges );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceAgent() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetSurrenderedAgents
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetSurrenderedAgents( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    T_KnowledgeAgentDiaIDVector knowledges;
    callerAgent.GetKSQuerier().GetSurrenderedAgents( knowledges );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceAgent() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetPerceivedAgents
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetPerceivedAgents( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    T_KnowledgeAgentDiaIDVector knowledges;
    callerAgent.GetKSQuerier().GetPerceivedAgents( knowledges );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceAgent() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetRapForLocal
// Created: NLD 2004-04-08
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetRapForLocal( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( (float)callerAgent.GetKSQuerier().GetRapForLocalValue() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetRapForGlobal
// Created: NLD 2004-04-08
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetRapForGlobal( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    call.GetResult().SetValue( (float)callerAutomate.GetKSQuerier().GetRapForGlobalValue() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetClosestObject
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetClosestObject( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    MIL_RealObjectTypeFilter objectsFilter( call.GetParameters(), 0 );
    
    DEC_Knowledge_Object* pKnowledge = callerAgent.GetKSQuerier().GetClosestObject( callerAgent.GetRole< PHY_RolePion_Location >().GetPosition(), objectsFilter );
    call.GetResult().SetValue( (void*)( pKnowledge ? pKnowledge->GetID() : 0 ), &DEC_Tools::GetTypeConnaissanceObjet() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObjectsColliding
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetObjectsColliding( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    T_KnowledgeObjectDiaIDVector objectsColliding;
    callerAgent.GetKSQuerier().GetObjectsColliding( objectsColliding );

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
    callerAgent.GetKSQuerier().GetPopulationsColliding( populationsColliding );

    DIA_Variable_ObjectList& diaPopulationList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaPopulationList.SetValueUserType( populationsColliding, DEC_Tools::GetTypeConnaissancePopulation() );

}

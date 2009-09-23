//*****************************************************************************
//
// $Created: JVT 04-05-18 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_ActionFunctions.cpp $
// $Author: Nld $
// $Modtime: 12/05/05 17:40 $
// $Revision: 13 $
// $Workfile: DEC_ActionFunctions.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "DEC_ActionFunctions.h"
#include "DEC_FunctionsTools.h"
#include "Entities/Actions/PHY_Action_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/Roles/Refugee/PHY_RoleInterface_Refugee.h"
#include "Entities/Objects/MIL_Object_ABC.h"
//#include "Entities/Objects/LogisticCapacity.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"

namespace
{
    bool IsNotCampKnowledgeOrHasLogisticCapacity( DEC_Knowledge_Agent* pKnowledge, DEC_Knowledge_Object* pCampKnowledge )
    {
        return( !pKnowledge || !pCampKnowledge || !pCampKnowledge->GetObjectKnown() ); //|| pCampKnowledge->GetObjectKnown()->Retrieve< LogisticCapacity >() != 0 )
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Prisoners_CaptureAndLoad
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Prisoners_CaptureAndLoad( MIL_AgentPion& callerAgent, DEC_Knowledge_Agent* pKnowledge )
{
    if( pKnowledge && pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Surrender >().Capture( callerAgent ) )
        callerAgent.GetRole< transport::PHY_RoleAction_Transport >().MagicLoadPion( pKnowledge->GetAgentKnown(), false /*bTransportOnlyLoadable*/ );
}
   
// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Prisoners_Unload
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Prisoners_Unload( MIL_AgentPion& callerAgent, DEC_Knowledge_Agent* pKnowledge )
{
    if( pKnowledge )
    {
        pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Surrender >().Release();
        callerAgent.GetRole< transport::PHY_RoleAction_Transport >().MagicUnloadPion( pKnowledge->GetAgentKnown() );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Prisoners_UnloadInCamp
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Prisoners_UnloadInCamp( MIL_AgentPion& callerAgent, DEC_Knowledge_Agent* pKnowledge, unsigned int campKnowledgeID  )
{
	DEC_Knowledge_Object* pCampKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( campKnowledgeID, callerAgent.GetArmy() );
    if( IsNotCampKnowledgeOrHasLogisticCapacity( pKnowledge, pCampKnowledge ) )
        return;

    callerAgent.GetRole< transport::PHY_RoleAction_Transport >().MagicUnloadPion( pKnowledge->GetAgentKnown() );
    pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Surrender >().Imprison( *pCampKnowledge->GetObjectKnown() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Prisoners_IsUnloadedInCamp
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::Prisoners_IsUnloadedInCamp( MIL_AgentPion& callerAgent, DEC_Knowledge_Agent* pKnowledge, unsigned int campKnowledgeID )
{
	DEC_Knowledge_Object* pCampKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( campKnowledgeID, callerAgent.GetArmy          () );
    if( IsNotCampKnowledgeOrHasLogisticCapacity( pKnowledge, pCampKnowledge ) )
        return false;
    else
        return pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Surrender >().IsImprisoned( *pCampKnowledge->GetObjectKnown() ) ;
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Refugees_OrientateAndLoad
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Refugees_OrientateAndLoad( MIL_AgentPion& callerAgent, DEC_Knowledge_Agent* pKnowledge )
{
	if( pKnowledge && pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Refugee >().Orientate( callerAgent ) )
        callerAgent.GetRole< transport::PHY_RoleAction_Transport >().MagicLoadPion( pKnowledge->GetAgentKnown(), false /*bTransportOnlyLoadable*/ );
}
   
// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Refugees_Unload
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Refugees_Unload( MIL_AgentPion& callerAgent, DEC_Knowledge_Agent* pKnowledge )
{
	if( !pKnowledge )
        return;
    pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Refugee >().Release();
    callerAgent.GetRole< transport::PHY_RoleAction_Transport >().MagicUnloadPion( pKnowledge->GetAgentKnown() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Refugees_UnloadInCamp
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Refugees_UnloadInCamp( MIL_AgentPion& callerAgent, DEC_Knowledge_Agent* pKnowledge, unsigned int campKnowledgeID )
{
	DEC_Knowledge_Object* pCampKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( campKnowledgeID, callerAgent.GetArmy          () );
    if( IsNotCampKnowledgeOrHasLogisticCapacity( pKnowledge, pCampKnowledge ) )
        return;
    callerAgent.GetRole< transport::PHY_RoleAction_Transport >().MagicUnloadPion( pKnowledge->GetAgentKnown() );
    pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Refugee >().Release( *pCampKnowledge->GetObjectKnown() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Refugees_IsLoaded
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::PrisonnersRefugees_IsLoaded( MIL_AgentPion& callerAgent, DEC_Knowledge_Agent* pKnowledge )
{
    if( !pKnowledge )
        return false;
    else
        return callerAgent.GetRole< transport::PHY_RoleAction_Transport >().IsLoaded( pKnowledge->GetAgentKnown() ) ;
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Refugees_IsUnloadedInCamp
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::Refugees_IsUnloadedInCamp( MIL_AgentPion& callerAgent, DEC_Knowledge_Agent* pKnowledge, unsigned int campKnowledgeID )
{
    DEC_Knowledge_Object* pCampKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( campKnowledgeID, callerAgent.GetArmy() );
    if( IsNotCampKnowledgeOrHasLogisticCapacity( pKnowledge, pCampKnowledge ) )
        return false;
    else
        return pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Refugee >().IsManaged( *pCampKnowledge->GetObjectKnown() );
}

// =============================================================================
// TRANSPORT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_AddPion
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_AddPion( MIL_AgentPion& callerAgent, DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable )
{
    callerAgent.GetRole< transport::PHY_RoleAction_Transport >().AddPion( pPion->GetPion(), bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_AddPions
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_AddPions( MIL_AgentPion& callerAgent, const std::vector< DEC_Decision_ABC* >& pions, bool bTransportOnlyLoadable )
{
    for( std::vector< DEC_Decision_ABC* >::const_iterator itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {   
        MIL_AgentPion& pion = ( *itPion )->GetPion();
        callerAgent.GetRole< transport::PHY_RoleAction_Transport >().AddPion( pion, bTransportOnlyLoadable );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicLoadPion
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicLoadPion( MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable  )
{
    assert( pPion );
    callerAgent.GetRole< transport::PHY_RoleAction_Transport >().MagicLoadPion( pPion->GetPion(), bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicLoadPions
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicLoadPions( MIL_AgentPion& callerAgent, const std::vector< DEC_Decision_ABC* >& pions, bool bTransportOnlyLoadable )
{
    for( std::vector< DEC_Decision_ABC* >::const_iterator itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {   
        MIL_AgentPion& pion = ( *itPion )->GetPion();
        callerAgent.GetRole< transport::PHY_RoleAction_Transport >().MagicLoadPion( pion, bTransportOnlyLoadable );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicUnloadPion
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicUnloadPion( MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pPion )
{
	assert( pPion );
    callerAgent.GetRole< transport::PHY_RoleAction_Transport >().MagicUnloadPion( pPion->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_IsFinished
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::Transport_IsFinished( const MIL_AgentPion& callerAgent )
{
    return callerAgent.GetRole< transport::PHY_RoleAction_Transport >().IsFinished() ;
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_Cancel
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_Cancel( MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< transport::PHY_RoleAction_Transport >().Cancel();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::CanTransportPion
// Created: JVT 2005-01-18
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::CanTransportPion( const MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable )
{
    assert( pPion );
    return callerAgent.GetRole< transport::PHY_RoleAction_Transport >().CanTransportPion( pPion->GetPion(), bTransportOnlyLoadable ) ;
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_IsTransporting
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::Transport_IsTransporting( const MIL_AgentPion& callerAgent )
{
    return callerAgent.GetRole< transport::PHY_RoleAction_Transport >().IsTransporting();
}

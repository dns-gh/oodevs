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
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/Roles/Refugee/PHY_RoleInterface_Refugee.h"
#include "Entities/Objects/MIL_Object_ABC.h"
//#include "Entities/Objects/LogisticCapacity.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"

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
void DEC_ActionFunctions::Prisoners_CaptureAndLoad( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( pKnowledge && pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Surrender >().Capture( callerAgent ) )
        callerAgent.GetRole< PHY_RoleAction_Transport >().MagicLoadPion( pKnowledge->GetAgentKnown(), false /*bTransportOnlyLoadable*/ );
}
   
// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Prisoners_Unload
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Prisoners_Unload( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( pKnowledge )
    {
        pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Surrender >().Release();
        callerAgent.GetRole< PHY_RoleAction_Transport >().MagicUnloadPion( pKnowledge->GetAgentKnown() );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Prisoners_UnloadInCamp
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Prisoners_UnloadInCamp( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    DEC_Knowledge_Object* pCampKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 1 ), callerAgent.GetArmy() );
    if( IsNotCampKnowledgeOrHasLogisticCapacity( pKnowledge, pCampKnowledge ) )
        return;

    callerAgent.GetRole< PHY_RoleAction_Transport >().MagicUnloadPion( pKnowledge->GetAgentKnown() );
    pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Surrender >().Imprison( *pCampKnowledge->GetObjectKnown() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Prisoners_IsLoaded
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Prisoners_IsLoaded( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( pKnowledge )
        call.GetResult().SetValue( callerAgent.GetRole< PHY_RoleAction_Transport >().IsLoaded( pKnowledge->GetAgentKnown() ) );
    else
        call.GetResult().SetValue( false );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Prisoners_IsUnloadedInCamp
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Prisoners_IsUnloadedInCamp( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    DEC_Knowledge_Object* pCampKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 1 ), callerAgent.GetArmy          () );
    if( IsNotCampKnowledgeOrHasLogisticCapacity( pKnowledge, pCampKnowledge ) )
        call.GetResult().SetValue( false );
    else
        call.GetResult().SetValue( pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Surrender >().IsImprisoned( *pCampKnowledge->GetObjectKnown() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Refugees_OrientateAndLoad
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Refugees_OrientateAndLoad( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( pKnowledge && pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Refugee >().Orientate( callerAgent ) )
        callerAgent.GetRole< PHY_RoleAction_Transport >().MagicLoadPion( pKnowledge->GetAgentKnown(), false /*bTransportOnlyLoadable*/ );
}
   
// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Refugees_Unload
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Refugees_Unload( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
        return;
    pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Refugee >().Release();
    callerAgent.GetRole< PHY_RoleAction_Transport >().MagicUnloadPion( pKnowledge->GetAgentKnown() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Refugees_UnloadInCamp
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Refugees_UnloadInCamp( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    DEC_Knowledge_Object* pCampKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 1 ), callerAgent.GetArmy          () );
    if( IsNotCampKnowledgeOrHasLogisticCapacity( pKnowledge, pCampKnowledge ) )
        return;
    callerAgent.GetRole< PHY_RoleAction_Transport >().MagicUnloadPion( pKnowledge->GetAgentKnown() );
    pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Refugee >().Release( *pCampKnowledge->GetObjectKnown() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Refugees_IsLoaded
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Refugees_IsLoaded( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
        call.GetResult().SetValue( false );
    else
        call.GetResult().SetValue( callerAgent.GetRole< PHY_RoleAction_Transport >().IsLoaded( pKnowledge->GetAgentKnown() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Refugees_IsUnloadedInCamp
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Refugees_IsUnloadedInCamp( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    DEC_Knowledge_Object* pCampKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 1 ), callerAgent.GetArmy          () );
    if( IsNotCampKnowledgeOrHasLogisticCapacity( pKnowledge, pCampKnowledge ) )
        call.GetResult().SetValue( false );
    else
        call.GetResult().SetValue( pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Refugee >().IsManaged( *pCampKnowledge->GetObjectKnown() ) );
}

// =============================================================================
// TRANSPORT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_AddPion
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_AddPion( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );
    const bool bTransportOnlyLoadable = call.GetParameter( 1 ).ToBool();
    const DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    callerAgent.GetRole< PHY_RoleAction_Transport >().AddPion( pPion->GetPion(), bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_AddPions
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_AddPions( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeListePions( call.GetParameter( 0 ) ) );
    const bool bTransportOnlyLoadable = call.GetParameter( 1 ).ToBool();

    T_ObjectVector pions = call.GetParameter( 0 ).ToSelection();
    for( CIT_ObjectVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {   
        MIL_AgentPion& pion = static_cast< DEC_RolePion_Decision* >( *itPion )->GetPion();
        callerAgent.GetRole< PHY_RoleAction_Transport >().AddPion( pion, bTransportOnlyLoadable );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicLoadPion
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicLoadPion( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );
    const bool bTransportOnlyLoadable = call.GetParameter( 1 ).ToBool();
    const DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    callerAgent.GetRole< PHY_RoleAction_Transport >().MagicLoadPion( pPion->GetPion(), bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicLoadPions
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicLoadPions( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeListePions( call.GetParameter( 0 ) ) );
    const bool bTransportOnlyLoadable = call.GetParameter( 1 ).ToBool();

    T_ObjectVector pions = call.GetParameter( 0 ).ToSelection();
    for( CIT_ObjectVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {   
        MIL_AgentPion& pion = static_cast< DEC_RolePion_Decision* >( *itPion )->GetPion();
        callerAgent.GetRole< PHY_RoleAction_Transport >().MagicLoadPion( pion, bTransportOnlyLoadable );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicUnloadPion
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicUnloadPion( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );
    const DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    callerAgent.GetRole< PHY_RoleAction_Transport >().MagicUnloadPion( pPion->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_IsFinished
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_IsFinished( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( callerAgent.GetRole< PHY_RoleAction_Transport >().IsFinished() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_Cancel
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_Cancel( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RoleAction_Transport >().Cancel();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::CanTransportPion
// Created: JVT 2005-01-18
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::CanTransportPion( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );
    
    const DEC_RolePion_Decision* pTransported           = call.GetParameter( 0 ).ToUserObject( pTransported );
    const bool                   bTransportOnlyLoadable = call.GetParameter( 1 ).ToBool();
    
    assert( pTransported );
    call.GetResult().SetValue( callerAgent.GetRole< PHY_RoleAction_Transport >().CanTransportPion( pTransported->GetPion(), bTransportOnlyLoadable ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_IsTransporting
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_IsTransporting( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( callerAgent.GetRole< PHY_RoleAction_Transport >().IsTransporting() );
}

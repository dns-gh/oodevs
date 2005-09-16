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

#include "MIL_pch.h"

#include "DEC_ActionFunctions.h"
#include "DEC_FunctionsTools.h"
#include "Entities/Agents/Actions/PHY_Action_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/Roles/Refugee/PHY_RoleInterface_Refugee.h"
#include "Entities/Objects/MIL_CampRefugies.h"
#include "Entities/Objects/MIL_CampPrisonniers.h"
#include "Entities/Objects/MIL_RealObjectType.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"


// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::StopAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::StopAction( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeAction( call.GetParameter( 0 ) ) );

    PHY_Action_ABC* pAction = call.GetParameter( 0 ).ToUserPtr( pAction );
    if( !pAction )
        return;
    assert( callerAgent.HasAction( *pAction ) );
    MIL_AgentServer::GetWorkspace().GetWorkspaceDIA().GetDebug().NotifyActionStopped( call, callerAgent, *pAction );
    delete pAction;
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::SuspendAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::SuspendAction( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeAction( call.GetParameter( 0 ) ) );

    PHY_Action_ABC* pAction = call.GetParameter( 0 ).ToUserPtr( pAction );
    if( !pAction )
        return;
    assert( callerAgent.HasAction( *pAction ) );
    pAction->Suspend();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::ResumeAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::ResumeAction( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeAction( call.GetParameter( 0 ) ) );

    PHY_Action_ABC* pAction = call.GetParameter( 0 ).ToUserPtr( pAction );
    if( !pAction )
        return;
    assert( callerAgent.HasAction( *pAction ) );
    pAction->Resume();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::TakePrisoner
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::TakePrisoner( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    DEC_Knowledge_Object* pCampKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 1 ), callerAgent.GetArmy          () );
    if( !pKnowledge || !pCampKnowledge || !pCampKnowledge->GetObjectKnown() || pCampKnowledge->GetObjectKnown()->GetType() != MIL_RealObjectType::campPrisonniers_ )
    {
        call.GetParameter( 2 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( false );
        return;
    }
    
    call.GetParameter( 2 ).SetValue( eQueryValid );

    const MIL_CampPrisonniers& camp = static_cast< const MIL_CampPrisonniers& >( *pCampKnowledge->GetObjectKnown() );
    bool bOut = pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Surrender >().TakePrisoner( callerAgent, camp );
    call.GetResult().SetValue( bOut );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::OrientateRefugee
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::OrientateRefugee( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Agent*  pKnowledge     = DEC_FunctionsTools::GetKnowledgeAgentFromDia ( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    DEC_Knowledge_Object* pCampKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 1 ), callerAgent.GetArmy          () );
    if( !pKnowledge || !pCampKnowledge || !pCampKnowledge->GetObjectKnown() || pCampKnowledge->GetObjectKnown()->GetType() != MIL_RealObjectType::campRefugies_ )
    {
        call.GetParameter( 2 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( false );
        return;
    }   
    call.GetParameter( 2 ).SetValue( eQueryValid );

    const MIL_CampRefugies& camp = static_cast< const MIL_CampRefugies& >( *pCampKnowledge->GetObjectKnown() );
    bool bOut = pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Refugee >().OrientateRefugee( camp );
    call.GetResult().SetValue( bOut );
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
// Name: DEC_ActionFunctions::Transport_MagicUnloadPions
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicUnloadPions( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeListePions( call.GetParameter( 0 ) ) );
    
    T_ObjectVector pions = call.GetParameter( 0 ).ToSelection();
    for( CIT_ObjectVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {   
        MIL_AgentPion& pion = static_cast< DEC_RolePion_Decision* >( *itPion )->GetPion();
        callerAgent.GetRole< PHY_RoleAction_Transport >().MagicUnloadPion( pion );
    }
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

//*****************************************************************************
//
// $Created: JVT 04-05-18 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_ActionFunctions.inl $
// $Author: Nld $
// $Modtime: 21/10/04 10:17 $
// $Revision: 2 $
// $Workfile: DEC_ActionFunctions.inl $
//
//*****************************************************************************

#include "MIL_AgentServer.h"
#include "Decision/DEC_Workspace.h"
#include "Decision/DEC_Debug.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::StartAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
template< typename ActionType >
void DEC_ActionFunctions::StartAction( DIA_Call_ABC& diaCall, typename ActionType::ActorType& caller )
{
    ActionType* pAction = new ActionType( caller, diaCall );
    MIL_AgentServer::GetWorkspace().GetWorkspaceDIA().GetDebug().NotifyActionStarted( diaCall, caller, *pAction );
    diaCall.GetResult().SetValue( pAction, &DEC_Tools::GetTypeAction() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::StopAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
template< typename T >
void DEC_ActionFunctions::StopAction( DIA_Call_ABC& call, const T& caller )
{
    assert( DEC_Tools::CheckTypeAction( call.GetParameter( 0 ) ) );

    PHY_Action_ABC* pAction = call.GetParameter( 0 ).ToUserPtr( pAction );
    if( !pAction )
        return;
    assert( caller.HasAction( *pAction ) );
    MIL_AgentServer::GetWorkspace().GetWorkspaceDIA().GetDebug().NotifyActionStopped( call, caller, *pAction );
    delete pAction;
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::SuspendAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
template< typename T >
void DEC_ActionFunctions::SuspendAction( DIA_Call_ABC& call, const T& caller )
{
    assert( DEC_Tools::CheckTypeAction( call.GetParameter( 0 ) ) );

    PHY_Action_ABC* pAction = call.GetParameter( 0 ).ToUserPtr( pAction );
    if( !pAction )
        return;
    assert( caller.HasAction( *pAction ) );
    pAction->Suspend();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::ResumeAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
template< typename T >
void DEC_ActionFunctions::ResumeAction( DIA_Call_ABC& call, const T& caller )
{
    assert( DEC_Tools::CheckTypeAction( call.GetParameter( 0 ) ) );

    PHY_Action_ABC* pAction = call.GetParameter( 0 ).ToUserPtr( pAction );
    if( !pAction )
        return;
    assert( caller.HasAction( *pAction ) );
    pAction->Resume();
}

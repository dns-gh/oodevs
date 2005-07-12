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
template< class ActionType >
void DEC_ActionFunctions::StartAction( DIA_Call_ABC& diaCall, MIL_AgentPion& callerAgent )
{
    ActionType* pAction = new ActionType( callerAgent, diaCall );
    MIL_AgentServer::GetWorkspace().GetWorkspaceDIA().GetDebug().NotifyActionStarted( diaCall, callerAgent, *pAction );
    diaCall.GetResult().SetValue( pAction, &DEC_Tools::GetTypeAction() );
}

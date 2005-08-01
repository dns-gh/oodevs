//*****************************************************************************
//
// $Created: JVT 04-05-18 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_ActionFunctions.h $
// $Author: Nld $
// $Modtime: 19/04/05 16:31 $
// $Revision: 9 $
// $Workfile: DEC_ActionFunctions.h $
//
//*****************************************************************************

#ifndef __DEC_ActionFunctions_h_
#define __DEC_ActionFunctions_h_

#include "MIL.h"

class MIL_AgentPion;

// =============================================================================
// Created: JVT 04-05-18
// =============================================================================
class DEC_ActionFunctions
{
public:
    //! @name Functions
    //@{
    template< class ActionType >
    static void StartAction  ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void StopAction   ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void SuspendAction( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void ResumeAction ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );

    static void Transport_AddPion         ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );        
    static void Transport_AddPions        ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );        
    static void Transport_MagicLoadPion   ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );        
    static void Transport_MagicLoadPions  ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );        
    static void Transport_MagicUnloadPion ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );         
    static void Transport_MagicUnloadPions( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );        
    static void Transport_IsFinished      ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void Transport_IsTransporting  ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void Transport_Cancel          ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );        
    static void CanTransportPion          ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );

    static void TakePrisoner    ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void OrientateRefugee( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    //@}
};

#include "DEC_ActionFunctions.inl"

#endif // __DEC_ActionFunctions_h_
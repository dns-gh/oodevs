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
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: template< typename ActionType > static ActionType* DEC_ActionFunctions::StartAction
// Created: LDC 2009-06-26
// -----------------------------------------------------------------------------
template< typename ActionType >
PHY_Action_ABC* DEC_ActionFunctions::StartAction( typename ActionType::ActorType& caller )
{
    return new ActionType( caller );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::StartAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
template< typename ActionType, typename T >
PHY_Action_ABC* DEC_ActionFunctions::StartAction( typename ActionType::ActorType& caller, T arg )
{
    return new ActionType( caller, arg );
}

// -----------------------------------------------------------------------------
// Name: template< typename ActionType, T1, T2 > static ActionType* DEC_ActionFunctions::StartAction
// Created: LDC 2009-06-26
// -----------------------------------------------------------------------------
template< typename ActionType, typename T1, typename T2 >
PHY_Action_ABC* DEC_ActionFunctions::StartAction( typename ActionType::ActorType& caller, T1 arg1, T2 arg2 )
{
    return new ActionType( caller, arg1, arg2 );
}

// -----------------------------------------------------------------------------
// Name: template< typename ActionType, T1, T2, T3 > static ActionType* DEC_ActionFunctions::StartAction
// Created: LDC 2009-06-26
// -----------------------------------------------------------------------------
template< typename ActionType, typename T1, typename T2, typename T3 >
PHY_Action_ABC* DEC_ActionFunctions::StartAction( typename ActionType::ActorType& caller, T1 arg1, T2 arg2, T3 arg3 )
{
    return new ActionType( caller, arg1, arg2, arg3 );
}

// -----------------------------------------------------------------------------
// Name: template< typename ActionType, T1, T2, T3, T4 > static ActionType* DEC_ActionFunctions::StartAction
// Created: LDC 2009-06-26
// -----------------------------------------------------------------------------
template< typename ActionType, typename T1, typename T2, typename T3, typename T4 >
PHY_Action_ABC* DEC_ActionFunctions::StartAction( typename ActionType::ActorType& caller, T1 arg1, T2 arg2, T3 arg3, T4 arg4 )
{
    return new ActionType( caller, arg1, arg2, arg3, arg4 );
}

// -----------------------------------------------------------------------------
// Name: template< typename ActionType, typename T1, typename T2, typename T3, typename T4, typename T5 > static PHY_Action_ABC* DEC_ActionFunctions::StartAction
// Created: LDC 2009-12-30
// -----------------------------------------------------------------------------
template< typename ActionType, typename T1, typename T2, typename T3, typename T4, typename T5 >
PHY_Action_ABC* DEC_ActionFunctions::StartAction( typename ActionType::ActorType& caller, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5 )
{
    return new ActionType( caller, arg1, arg2, arg3, arg4, arg5 );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::StopAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
template< typename T >
PHY_Action_ABC* DEC_ActionFunctions::StopAction( const T& caller, PHY_Action_ABC* pAction )
{
    if( pAction )
    {
        assert( caller.HasAction( *pAction ) );
        delete pAction;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::SuspendAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
template< typename T >
void DEC_ActionFunctions::SuspendAction( const T& caller, PHY_Action_ABC* pAction )
{
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
void DEC_ActionFunctions::ResumeAction( const T& caller, PHY_Action_ABC* pAction )
{
    if( !pAction )
        return;
    assert( caller.HasAction( *pAction ) );
    pAction->Resume();
}

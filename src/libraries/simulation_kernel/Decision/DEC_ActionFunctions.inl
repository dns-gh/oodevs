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
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Populations/MIL_Population.h"

// -----------------------------------------------------------------------------
// Name: template< typename ActionType > static ActionType* DEC_ActionFunctions::StartAction
// Created: LDC 2009-06-26
// -----------------------------------------------------------------------------
template< typename ActionType >
unsigned int DEC_ActionFunctions::StartAction( typename ActionType::ActorType& caller )
{
    boost::shared_ptr< PHY_Action_ABC > action( new ActionType( caller ) );
    caller.RegisterAction( action );
    return action->GetId();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::StartAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
template< typename ActionType, typename T >
unsigned int DEC_ActionFunctions::StartAction( typename ActionType::ActorType& caller, T arg )
{
    boost::shared_ptr< PHY_Action_ABC > action( new ActionType( caller, arg ) );
    caller.RegisterAction( action );
    return action->GetId();
}

// -----------------------------------------------------------------------------
// Name: template< typename ActionType, T1, T2 > static ActionType* DEC_ActionFunctions::StartAction
// Created: LDC 2009-06-26
// -----------------------------------------------------------------------------
template< typename ActionType, typename T1, typename T2 >
unsigned int DEC_ActionFunctions::StartAction( typename ActionType::ActorType& caller, T1 arg1, T2 arg2 )
{
    boost::shared_ptr< PHY_Action_ABC > action( new ActionType( caller, arg1, arg2 ) );
    caller.RegisterAction( action );
    return action->GetId();
}

// -----------------------------------------------------------------------------
// Name: template< typename ActionType, T1, T2, T3 > static ActionType* DEC_ActionFunctions::StartAction
// Created: LDC 2009-06-26
// -----------------------------------------------------------------------------
template< typename ActionType, typename T1, typename T2, typename T3 >
unsigned int DEC_ActionFunctions::StartAction( typename ActionType::ActorType& caller, T1 arg1, T2 arg2, T3 arg3 )
{
    boost::shared_ptr< PHY_Action_ABC > action( new ActionType( caller, arg1, arg2, arg3 ) );
    caller.RegisterAction( action );
    return action->GetId();
}

// -----------------------------------------------------------------------------
// Name: template< typename ActionType, T1, T2, T3, T4 > static ActionType* DEC_ActionFunctions::StartAction
// Created: LDC 2009-06-26
// -----------------------------------------------------------------------------
template< typename ActionType, typename T1, typename T2, typename T3, typename T4 >
unsigned int DEC_ActionFunctions::StartAction( typename ActionType::ActorType& caller, T1 arg1, T2 arg2, T3 arg3, T4 arg4 )
{
    boost::shared_ptr< PHY_Action_ABC > action( new ActionType( caller, arg1, arg2, arg3, arg4 ) );
    caller.RegisterAction( action );
    return action->GetId();
}

// -----------------------------------------------------------------------------
// Name: template< typename ActionType, typename T1, typename T2, typename T3, typename T4, typename T5 > static PHY_Action_ABC* DEC_ActionFunctions::StartAction
// Created: LDC 2009-12-30
// -----------------------------------------------------------------------------
template< typename ActionType, typename T1, typename T2, typename T3, typename T4, typename T5 >
unsigned int DEC_ActionFunctions::StartAction( typename ActionType::ActorType& caller, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5 )
{
    boost::shared_ptr< PHY_Action_ABC > action( new ActionType( caller, arg1, arg2, arg3, arg4, arg5 ) );
    caller.RegisterAction( action );
    return action->GetId();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::StopAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
template< typename T >
unsigned int DEC_ActionFunctions::StopAction( T& caller, unsigned int actionId )
{
    if( actionId != 0 )
    {
        caller.UnregisterAction( actionId );
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::SuspendAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
template< typename T >
void DEC_ActionFunctions::SuspendAction( const T& caller, unsigned int actionId )
{
    boost::shared_ptr< PHY_Action_ABC > pAction = caller.GetAction( actionId );
    if( pAction.get() )
        pAction->Suspend();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::ResumeAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
template< typename T >
void DEC_ActionFunctions::ResumeAction( const T& caller, unsigned int actionId )
{
    boost::shared_ptr< PHY_Action_ABC > pAction = caller.GetAction( actionId );
    if( pAction.get() )
        pAction->Resume();
}
// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::GetActionState
// Created: LGY 2013-07-11
// -----------------------------------------------------------------------------
template< typename T >
unsigned int DEC_ActionFunctions::GetActionState( const T& caller, unsigned int actionId )
{
    boost::shared_ptr< PHY_Action_ABC > pAction = caller.GetAction( actionId );
    if( pAction.get() )
        return pAction->GetState();
    return PHY_Action_ABC::eError;
}
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

class DEC_Decision_ABC;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class MIL_AgentPion;
class PHY_Action_ABC;

// =============================================================================
// Created: JVT 04-05-18
// =============================================================================
class DEC_ActionFunctions
{
public:
    //! @name Functions
    //@{
    template< typename ActionType > static PHY_Action_ABC* StartAction  ( typename ActionType::ActorType& caller );
    template< typename ActionType, typename T > static PHY_Action_ABC* StartAction  ( typename ActionType::ActorType& caller, T arg );
    template< typename ActionType, typename T1, typename T2 > static PHY_Action_ABC* StartAction  ( typename ActionType::ActorType& caller, T1 arg1, T2 arg2 );
    template< typename ActionType, typename T1, typename T2, typename T3 > static PHY_Action_ABC* StartAction  ( typename ActionType::ActorType& caller, T1 arg1, T2 arg2, T3 arg3 );
    template< typename ActionType, typename T1, typename T2, typename T3, typename T4 > static PHY_Action_ABC* StartAction  ( typename ActionType::ActorType& caller, T1 arg1, T2 arg2, T3 arg3, T4 arg4 );
    template< typename ActionType, typename T1, typename T2, typename T3, typename T4, typename T5 > static PHY_Action_ABC* StartAction  ( typename ActionType::ActorType& caller, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5 );
    template< typename T >          static PHY_Action_ABC* StopAction   ( const T& caller, PHY_Action_ABC* pAction );
    template< typename T >          static void SuspendAction( const T& caller, PHY_Action_ABC* pAction );
    template< typename T >          static void ResumeAction ( const T& caller, PHY_Action_ABC* pAction );

    static void Transport_AddPion         ( MIL_AgentPion& callerAgent, DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable );        
    static void Transport_AddPions        ( MIL_AgentPion& callerAgent, const std::vector< DEC_Decision_ABC* >& pions, bool bTransportOnlyLoadable );        
    static void Transport_MagicLoadPion   ( MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable );
    static void Transport_MagicLoadPions  ( MIL_AgentPion& callerAgent, const std::vector< DEC_Decision_ABC* >& pions, bool bTransportOnlyLoadable );
    static void Transport_MagicUnloadPion ( MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pPion );
    static bool Transport_IsFinished      ( const MIL_AgentPion& callerAgent );
    static bool Transport_IsTransporting  ( const MIL_AgentPion& callerAgent );
    static void Transport_Cancel          ( MIL_AgentPion& callerAgent );
    static bool CanTransportPion          ( const MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable );

    static void Prisoners_CaptureAndLoad  ( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static void Prisoners_Unload          ( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static void Prisoners_UnloadInCamp    ( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object > campKnowledgeID );
    static bool Prisoners_IsUnloadedInCamp( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object > campKnowledgeID );

    static void Refugees_OrientateAndLoad( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static void Refugees_Unload          ( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static void Refugees_UnloadInCamp    ( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object > campKnowledgeID );
    static bool PrisonnersRefugees_IsLoaded( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool Refugees_IsUnloadedInCamp( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object > campKnowledgeID );
    //@}
};

#include "DEC_ActionFunctions.inl"

#endif // __DEC_ActionFunctions_h_

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

class DEC_Decision_ABC;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class DEC_Knowledge_Population;
class MIL_AgentPion;
class PHY_Action_ABC;
class MT_Vector2D;
class PHY_DotationCategory;

// =============================================================================
// Created: JVT 04-05-18
// =============================================================================
class DEC_ActionFunctions
{
public:
    //! @name Functions
    //@{
    template< typename ActionType > static unsigned int StartAction  ( typename ActionType::ActorType& caller );
    template< typename ActionType, typename T > static unsigned int StartAction  ( typename ActionType::ActorType& caller, T arg );
    template< typename ActionType, typename T1, typename T2 > static unsigned int StartAction  ( typename ActionType::ActorType& caller, T1 arg1, T2 arg2 );
    template< typename ActionType, typename T1, typename T2, typename T3 > static unsigned int StartAction  ( typename ActionType::ActorType& caller, T1 arg1, T2 arg2, T3 arg3 );
    template< typename ActionType, typename T1, typename T2, typename T3, typename T4 > static unsigned int StartAction  ( typename ActionType::ActorType& caller, T1 arg1, T2 arg2, T3 arg3, T4 arg4 );
    template< typename ActionType, typename T1, typename T2, typename T3, typename T4, typename T5 > static unsigned int StartAction  ( typename ActionType::ActorType& caller, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5 );
    template< typename T >          static unsigned int StopAction   ( T& caller, unsigned int actionId );
    template< typename T >          static void SuspendAction( const T& caller, unsigned int actionId  );
    template< typename T >          static void ResumeAction ( const T& caller, unsigned int actionId  );

    static void Orientate                 ( MIL_AgentPion& callerAgent, boost::shared_ptr< MT_Vector2D > dir );

    static void Transport_MagicLoadPionInCarrier    ( MIL_AgentPion& caller, const DEC_Decision_ABC* pCarrier );
    static void Transport_MagicUnloadPionFromCarrier( MIL_AgentPion& caller );
    static DEC_Decision_ABC* Transport_GetCarrier   ( const MIL_AgentPion& caller );

    static void Transport_AddPion         ( MIL_AgentPion& callerAgent, DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable );
    static void Transport_AddPions        ( MIL_AgentPion& callerAgent, const std::vector< DEC_Decision_ABC* >& pions, bool bTransportOnlyLoadable );
    static void TransportKnowledge_AddPion( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, bool bTransportOnlyLoadable );
    static void Transport_MagicLoadPion   ( MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable );
    static void Transport_MagicLoadPions  ( MIL_AgentPion& callerAgent, const std::vector< DEC_Decision_ABC* >& pions, bool bTransportOnlyLoadable );
    static void Transport_MagicUnloadPion ( MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pPion );
    static void Transport_MagicUnloadPions( MIL_AgentPion& callerAgent, const std::vector< DEC_Decision_ABC* >& pions );
    static bool Transport_IsFinished      ( const MIL_AgentPion& callerAgent );
    static bool Transport_IsTransporting  ( const MIL_AgentPion& callerAgent );
    static void Transport_Cancel          ( MIL_AgentPion& callerAgent );
    static bool CanTransportPion          ( const MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable );
    static bool CanTransportKnowledge     ( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, bool bTransportOnlyLoadable );
    static bool CanTransportCrowd         ( DEC_Decision_ABC& callerAgent );
    static bool IsTransportingCrowd       ( DEC_Decision_ABC& callerAgent );

    static void Prisoners_CaptureAndLoad  ( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static void Prisoners_Unload          ( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static void Prisoners_UnloadInCamp    ( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object > campKnowledgeID );
    static bool Prisoners_IsUnloadedInCamp( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object > campKnowledgeID );

    static void Refugees_OrientateAndLoad( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static void Refugees_Unload          ( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static void Refugees_UnloadInCamp    ( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object > campKnowledgeID );
    static bool PrisonnersRefugees_IsLoaded( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool Refugees_IsUnloadedInCamp( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object > campKnowledgeID );
    static void Knowledge_Load           ( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );

    static bool Stock_IsExtractPossible( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, const std::vector< const PHY_DotationCategory* >& dotationTypes );
    static bool Stock_IsSupplyPossible( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, const std::vector< const PHY_DotationCategory* >& dotationTypes );
    static bool Stock_IsDistributePossible( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, boost::shared_ptr< DEC_Knowledge_Population > population );
    //@}
};

#include "DEC_ActionFunctions.inl"

#endif // __DEC_ActionFunctions_h_

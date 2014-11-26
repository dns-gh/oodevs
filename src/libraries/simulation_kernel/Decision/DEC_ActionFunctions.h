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

#include <boost/make_shared.hpp>
#include "DEC_Decision_ABC.h"
#include "Entities/Populations/MIL_Population.h"

namespace sword
{
    class Brain;
}

class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class DEC_Knowledge_Population;
class MIL_AgentPion;
class MIL_Entity_ABC;
class PHY_Action_ABC;
class MT_Vector2D;
class PHY_DotationCategory;

// =============================================================================
// Created: JVT 04-05-18
// =============================================================================
class DEC_ActionFunctions
{
public:
    static void Register( sword::Brain& brain );

    //! @name Functions
    //@{
    template< typename ActionType > static unsigned int StartAction  ( DEC_Decision_ABC* caller );
    template< typename ActionType, typename T > static unsigned int StartAction  ( DEC_Decision_ABC* caller , T arg );
    template< typename ActionType, typename T1, typename T2 > static unsigned int StartAction  ( DEC_Decision_ABC* caller , T1 arg1, T2 arg2 );
    template< typename ActionType, typename T1, typename T2, typename T3 > static unsigned int StartAction  ( DEC_Decision_ABC* caller , T1 arg1, T2 arg2, T3 arg3 );
    template< typename ActionType, typename T1, typename T2, typename T3, typename T4 > static unsigned int StartAction  ( DEC_Decision_ABC* caller , T1 arg1, T2 arg2, T3 arg3, T4 arg4 );
    template< typename ActionType, typename T1, typename T2, typename T3, typename T4, typename T5 > static unsigned int StartAction  ( DEC_Decision_ABC* caller, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5 );
    static unsigned int StopAction( const DEC_Decision_ABC* caller, unsigned int actionId );
    static unsigned int StopAction( MIL_Entity_ABC& caller, unsigned int actionId );
    static void SuspendAction( const DEC_Decision_ABC* caller, unsigned int actionId );
    static void ResumeAction( const DEC_Decision_ABC* caller, unsigned int actionId );
    static unsigned int GetActionState( const DEC_Decision_ABC* caller, unsigned int actionId );

    static void Orientate                 ( DEC_Decision_ABC* callerAgent, boost::shared_ptr< MT_Vector2D > dir );

    static void Transport_MagicLoadPionInCarrier    ( DEC_Decision_ABC* caller, const DEC_Decision_ABC* pCarrier );
    static void Transport_MagicUnloadPionFromCarrier( DEC_Decision_ABC*caller );
    static DEC_Decision_ABC* Transport_GetCarrier   ( const DEC_Decision_ABC* caller );

    static void Transport_AddPion         ( DEC_Decision_ABC* callerAgent, DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable );
    static void Transport_AddPions        ( DEC_Decision_ABC* callerAgent, const std::vector< DEC_Decision_ABC* >& pions, bool bTransportOnlyLoadable );
    static void TransportKnowledge_AddPion( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, bool bTransportOnlyLoadable );
    static void Transport_MagicLoadPion   ( DEC_Decision_ABC* callerAgent, const DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable );
    static void Transport_MagicLoadPions  ( DEC_Decision_ABC* callerAgent, const std::vector< DEC_Decision_ABC* >& pions, bool bTransportOnlyLoadable );
    static void Transport_MagicUnloadPion ( DEC_Decision_ABC* callerAgent, const DEC_Decision_ABC* pPion );
    static void Transport_MagicUnloadPions( DEC_Decision_ABC* callerAgent, const std::vector< DEC_Decision_ABC* >& pions );
    static bool Transport_IsFinished      ( const DEC_Decision_ABC* callerAgent );
    static bool Transport_IsTransporting  ( const DEC_Decision_ABC* callerAgent );
    static void Transport_Cancel          ( DEC_Decision_ABC* callerAgent );
    static bool AgentCanTransportPion     ( const DEC_Decision_ABC* agent, const DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable );
    static bool CanLoad                   ( const DEC_Decision_ABC* pPion, const DEC_Decision_ABC* pTarget, bool bTransportOnlyLoadable );
    static double GetNumberOfRoundTripsLeftToTransportPion( const DEC_Decision_ABC* pPion, const DEC_Decision_ABC* pTarget, bool bTransportOnlyLoadable );
    static double GetNumberOfRoundTripToTransportPion( const DEC_Decision_ABC* callerAgent, const DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable );
    static bool CanTransportKnowledge     ( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, bool bTransportOnlyLoadable );
    static double GetNumberOfRoundTripToTransportKnowledge( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, bool bTransportOnlyLoadable );
    static bool CanTransportCrowd         ( DEC_Decision_ABC& callerAgent );
    static int GetCapacityToTransportCrowd( DEC_Decision_ABC& callerAgent );
    static bool IsTransportingCrowd       ( DEC_Decision_ABC& callerAgent );
    static bool IsSurrendered             ( DEC_Decision_ABC& callerAgent );
    static std::vector< DEC_Decision_ABC* > GetTransportedUnits( DEC_Decision_ABC& callerAgent );

    static void Prisoners_CaptureAndLoad  ( DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static void Prisoners_Unload          ( DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static void Prisoners_UnloadInCamp    ( DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object > campKnowledgeID );
    static bool Prisoners_IsUnloadedInCamp( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object > campKnowledgeID );

    static void Refugees_OrientateAndLoad( DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static void Refugees_Unload          ( DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static void Refugees_UnloadInCamp    ( DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object > campKnowledgeID );
    static bool PrisonnersRefugees_IsLoaded( DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool Refugees_IsUnloadedInCamp( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object > campKnowledgeID );
    static void Knowledge_Load           ( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );

    static void LoadAgentInCamp          ( DEC_Decision_ABC& callerAgent, DEC_Decision_ABC* pPion );
    static void UnLoadAgentInCamp        ( DEC_Decision_ABC& callerAgent, DEC_Decision_ABC* pPion, boost::shared_ptr< DEC_Knowledge_Object > pCampKnowledge );
    static bool IsAgentLoaded            ( DEC_Decision_ABC& callerAgent, DEC_Decision_ABC* pPion );
    static bool IsRefugee                ( DEC_Decision_ABC* pPion );

    static bool Stock_IsExtractPossible( DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, const std::vector< const PHY_DotationCategory* >& dotationTypes );
    static bool Stock_IsSupplyPossible( DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, const std::vector< const PHY_DotationCategory* >& dotationTypes );
    static bool Stock_IsDistributePossible( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, boost::shared_ptr< DEC_Knowledge_Population > population );
    //@}
};

namespace
{
    template< typename ActionType >
    unsigned int RegisterAction( typename ActionType::ActorType& caller, const boost::shared_ptr< ActionType >& action )
    {
        caller.RegisterAction( action );
        return action->GetId();
    }

    template< typename Entity >
    Entity& GetEntity( DEC_Decision_ABC* );

    template<>
    MIL_Entity_ABC& GetEntity( DEC_Decision_ABC* agent )
    {
        return agent->GetEntity();
    }

    template<>
    MIL_Population& GetEntity( DEC_Decision_ABC* agent )
    {
        return agent->GetPopulation();
    }

    template<>
    MIL_AgentPion& GetEntity( DEC_Decision_ABC* agent )
    {
        return agent->GetPion();
    }
}

// -----------------------------------------------------------------------------
// Name: template< typename ActionType > static ActionType* DEC_ActionFunctions::StartAction
// Created: LDC 2009-06-26
// -----------------------------------------------------------------------------
template< typename ActionType >
unsigned int DEC_ActionFunctions::StartAction( DEC_Decision_ABC* agent )
{
    auto& caller = GetEntity< typename ActionType::ActorType >( agent );
    return RegisterAction( caller, boost::make_shared< ActionType >( caller ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::StartAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
template< typename ActionType, typename T >
unsigned int DEC_ActionFunctions::StartAction( DEC_Decision_ABC* agent, T arg )
{
    auto& caller = GetEntity< typename ActionType::ActorType >( agent );
    return RegisterAction( caller, boost::make_shared< ActionType >( caller, arg ) );
}

// -----------------------------------------------------------------------------
// Name: template< typename ActionType, T1, T2 > static ActionType* DEC_ActionFunctions::StartAction
// Created: LDC 2009-06-26
// -----------------------------------------------------------------------------
template< typename ActionType, typename T1, typename T2 >
unsigned int DEC_ActionFunctions::StartAction( DEC_Decision_ABC* agent, T1 arg1, T2 arg2 )
{
    auto& caller = GetEntity< typename ActionType::ActorType >( agent );
    return RegisterAction( caller, boost::make_shared< ActionType >( caller, arg1, arg2 ) );
}

// -----------------------------------------------------------------------------
// Name: template< typename ActionType, T1, T2, T3 > static ActionType* DEC_ActionFunctions::StartAction
// Created: LDC 2009-06-26
// -----------------------------------------------------------------------------
template< typename ActionType, typename T1, typename T2, typename T3 >
unsigned int DEC_ActionFunctions::StartAction( DEC_Decision_ABC* agent, T1 arg1, T2 arg2, T3 arg3 )
{
    auto& caller = GetEntity< typename ActionType::ActorType >( agent );
    return RegisterAction( caller, boost::make_shared< ActionType >( caller, arg1, arg2, arg3 ) );
}

// -----------------------------------------------------------------------------
// Name: template< typename ActionType, T1, T2, T3, T4 > static ActionType* DEC_ActionFunctions::StartAction
// Created: LDC 2009-06-26
// -----------------------------------------------------------------------------
template< typename ActionType, typename T1, typename T2, typename T3, typename T4 >
unsigned int DEC_ActionFunctions::StartAction( DEC_Decision_ABC* agent, T1 arg1, T2 arg2, T3 arg3, T4 arg4 )
{
    auto& caller = GetEntity< typename ActionType::ActorType >( agent );
    return RegisterAction( caller, boost::make_shared< ActionType >( caller, arg1, arg2, arg3, arg4 ) );
}

// -----------------------------------------------------------------------------
// Name: template< typename ActionType, typename T1, typename T2, typename T3, typename T4, typename T5 > static PHY_Action_ABC* DEC_ActionFunctions::StartAction
// Created: LDC 2009-12-30
// -----------------------------------------------------------------------------
template< typename ActionType, typename T1, typename T2, typename T3, typename T4, typename T5 >
unsigned int DEC_ActionFunctions::StartAction( DEC_Decision_ABC* agent, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5 )
{
    auto& caller = GetEntity< typename ActionType::ActorType >( agent );
    return RegisterAction( caller, boost::make_shared< ActionType >( caller, arg1, arg2, arg3, arg4, arg5 ) );
}

#endif // __DEC_ActionFunctions_h_

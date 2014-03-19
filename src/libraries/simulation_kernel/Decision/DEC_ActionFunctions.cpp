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
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.h"
#include "Entities/Agents/Actions/CrowdTransport/PHY_RoleAction_CrowdTransport.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/Roles/Refugee/PHY_RoleInterface_Refugee.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationStock.h"
#include "Entities/Objects/StockAttribute.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "simulation_kernel/RefugeeActionsNotificationHandler_ABC.h"
#include <boost/foreach.hpp>

namespace
{
    bool IsNotCampKnowledgeOrHasLogisticCapacity( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object >& pCampKnowledge )
    {
        return( !pKnowledge || !pKnowledge->IsValid() || !pCampKnowledge || !pCampKnowledge->IsValid() || !pCampKnowledge->GetObjectKnown() ); //|| pCampKnowledge->GetObjectKnown()->Retrieve< LogisticCapacity >() != 0 )
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Prisoners_CaptureAndLoad
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Prisoners_CaptureAndLoad( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() && pKnowledge->GetAgentKnown().GetRole< surrender::PHY_RoleInterface_Surrender >().Capture( callerAgent ) )
    {
        bool transportOnlyLoadable = false;
        callerAgent.Apply( &transport::TransportNotificationHandler_ABC::MagicLoadPion, pKnowledge->GetAgentKnown(), transportOnlyLoadable );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Prisoners_Unload
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Prisoners_Unload( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent >  pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
    {
        pKnowledge->GetAgentKnown().GetRole< surrender::PHY_RoleInterface_Surrender >().Release();
        callerAgent.GetRole< transport::PHY_RoleAction_Transport >().MagicUnloadPion( pKnowledge->GetAgentKnown() );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Prisoners_UnloadInCamp
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Prisoners_UnloadInCamp( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object > pCampKnowledge  )
{
    if( IsNotCampKnowledgeOrHasLogisticCapacity( pKnowledge, pCampKnowledge ) )
        return;
    callerAgent.GetRole< transport::PHY_RoleAction_Transport >().MagicUnloadPion( pKnowledge->GetAgentKnown() );
    pKnowledge->GetAgentKnown().GetRole< surrender::PHY_RoleInterface_Surrender >().Imprison( *pCampKnowledge->GetObjectKnown(), callerAgent );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Prisoners_IsUnloadedInCamp
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::Prisoners_IsUnloadedInCamp( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object > pCampKnowledge )
{
    if( IsNotCampKnowledgeOrHasLogisticCapacity( pKnowledge, pCampKnowledge ) )
        return false;
    return pKnowledge->GetAgentKnown().GetRole< surrender::PHY_RoleInterface_Surrender >().IsImprisoned( *pCampKnowledge->GetObjectKnown() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Refugees_OrientateAndLoad
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Refugees_OrientateAndLoad( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        pKnowledge->GetAgentKnown().Apply( &refugee::RefugeeActionsNotificationHandler_ABC::Orientate, callerAgent );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Refugees_Unload
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Refugees_Unload( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return;
    pKnowledge->GetAgentKnown().Apply( &refugee::RefugeeActionsNotificationHandler_ABC::Release, callerAgent );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Refugees_UnloadInCamp
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Refugees_UnloadInCamp( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object > pCampKnowledge )
{
    if( IsNotCampKnowledgeOrHasLogisticCapacity( pKnowledge, pCampKnowledge ) )
        return;
    pKnowledge->GetAgentKnown().Apply( &refugee::RefugeeActionsNotificationHandler_ABC::ReleaseCamp, callerAgent, *pCampKnowledge->GetObjectKnown() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::LoadAgentInCamp
// Created: DDA 2012-03-05
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::LoadAgentInCamp( DEC_Decision_ABC& callerAgent, DEC_Decision_ABC* pPion )
{
    if( !pPion )
        return;
    pPion->GetPion().Apply( &refugee::RefugeeActionsNotificationHandler_ABC::Orientate, callerAgent.GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::UnLoadAgentInCamp
// Created: DDA 2012-03-05
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::UnLoadAgentInCamp( DEC_Decision_ABC& callerAgent, DEC_Decision_ABC* pPion, boost::shared_ptr< DEC_Knowledge_Object > pCampKnowledge )
{
    if( !pPion || !pCampKnowledge || !pCampKnowledge->IsValid() || !pCampKnowledge->GetObjectKnown() )
        return;
    pPion->GetPion().Apply( &refugee::RefugeeActionsNotificationHandler_ABC::ReleaseCamp, callerAgent.GetPion(), *pCampKnowledge->GetObjectKnown() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::IsAgentLoaded
// Created: DDA 2012-03-05
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::IsAgentLoaded( DEC_Decision_ABC& callerAgent, DEC_Decision_ABC* pPion )
{
    if( !pPion )
        return false;
    return callerAgent.GetPion().GetRole< transport::PHY_RoleAction_Transport >().IsLoaded( pPion->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::IsRefugee
// Created: DDA 2011-03-05
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::IsRefugee( DEC_Decision_ABC* pPion )
{
    if( !pPion )
        return false;
    return pPion->GetPion().GetType().IsRefugee();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Refugees_IsLoaded
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::PrisonnersRefugees_IsLoaded( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return false;
    return callerAgent.GetRole< transport::PHY_RoleAction_Transport >().IsLoaded( pKnowledge->GetAgentKnown() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Refugees_IsUnloadedInCamp
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::Refugees_IsUnloadedInCamp( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object > pCampKnowledge )
{
    if( IsNotCampKnowledgeOrHasLogisticCapacity( pKnowledge, pCampKnowledge ) )
        return false;
    return pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Refugee >().IsManaged( *pCampKnowledge->GetObjectKnown() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Knowledge_Load
// Created: EVH 2011-05-16
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Knowledge_Load( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
    {
        bool transportOnlyLoadable = false;
        callerAgent.GetPion().Apply( &transport::TransportNotificationHandler_ABC::MagicLoadPion, pKnowledge->GetAgentKnown(), transportOnlyLoadable );
        pKnowledge->GetAgentKnown().GetOrderManager().CancelMission();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_AddPion
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_AddPion( MIL_AgentPion& callerAgent, DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable )
{
    if( !pPion )
        throw MASA_EXCEPTION( "Null pion passed to DEC_ActionFunctions::Transport_AddPion" );
    callerAgent.GetRole< transport::PHY_RoleAction_Transport >().AddPion( pPion->GetPion(), bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_AddPions
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_AddPions( MIL_AgentPion& callerAgent, const std::vector< DEC_Decision_ABC* >& pions, bool bTransportOnlyLoadable )
{
    for( auto it = pions.begin(); it != pions.end(); ++it )
        Transport_AddPion( callerAgent, *it, bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::TransportKnowledge_AddPion
// Created: EVH 2011-05-23
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::TransportKnowledge_AddPion( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, bool bTransportOnlyLoadable )
{
    if( !pKnowledge )
        throw MASA_EXCEPTION( "Null knowledge passed to DEC_ActionFunctions::TransportKnowledge_AddPion" );
    MIL_Agent_ABC& pion = pKnowledge->GetAgentKnown();
    callerAgent.GetPion().GetRole< transport::PHY_RoleAction_Transport >().AddPion( pion, bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicLoadPionInCarrier
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicLoadPionInCarrier( MIL_AgentPion& caller, const DEC_Decision_ABC* pCarrier )
{
    if( !pCarrier )
        throw MASA_EXCEPTION( "Null carrier passed to DEC_ActionFunctions::Transport_MagicLoadPionInCarrier" );
    pCarrier->GetPion().Apply( &transport::TransportNotificationHandler_ABC::MagicLoadPion, caller, boost::cref( false ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicUnloadPionFromCarrier
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicUnloadPionFromCarrier( MIL_AgentPion& caller )
{
    //$$$ TMP
    const MIL_Agent_ABC* transporter = caller.GetRole< transport::PHY_RolePion_Transported >().GetTransporter();
    if( transporter )
        const_cast< MIL_Agent_ABC* >( transporter )->GetRole< transport::PHY_RoleAction_Transport >().MagicUnloadPion( caller );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_GetCarrier
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_ActionFunctions::Transport_GetCarrier( const MIL_AgentPion& caller )
{
    const MIL_Agent_ABC* transporter = caller.GetRole< transport::PHY_RolePion_Transported >().GetTransporter();
    if( transporter )
        return &const_cast< DEC_Decision_ABC& >( transporter->GetDecision() );
    return 0;
}
// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicLoadPion
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicLoadPion( MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable )
{
    if( !pPion )
        throw MASA_EXCEPTION( "Null pion passed to DEC_ActionFunctions::Transport_MagicLoadPion" );
    callerAgent.Apply( &transport::TransportNotificationHandler_ABC::MagicLoadPion, pPion->GetPion(), bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicLoadPions
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicLoadPions( MIL_AgentPion& callerAgent, const std::vector< DEC_Decision_ABC* >& pions, bool bTransportOnlyLoadable )
{
    for( auto it = pions.begin(); it != pions.end(); ++it )
        Transport_MagicLoadPion( callerAgent, *it, bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicUnloadPion
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicUnloadPion( MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pPion )
{
    if( !pPion )
        throw MASA_EXCEPTION( "Null pion passed to DEC_ActionFunctions::Transport_MagicUnloadPion" );
    callerAgent.GetRole< transport::PHY_RoleAction_Transport >().MagicUnloadPion( pPion->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicUnloadPions
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicUnloadPions( MIL_AgentPion& callerAgent, const std::vector< DEC_Decision_ABC* >& pions )
{
    for( auto it = pions.begin(); it != pions.end(); ++it )
        Transport_MagicUnloadPion( callerAgent, *it );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_IsFinished
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::Transport_IsFinished( const MIL_AgentPion& callerAgent )
{
    return callerAgent.GetRole< transport::PHY_RoleAction_Transport >().IsFinished();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_Cancel
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_Cancel( MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< transport::PHY_RoleAction_Transport >().Cancel();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::AgentCanTransportPion
// Created: GGE 2013-07-04
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::AgentCanTransportPion( const DEC_Decision_ABC* agent, const DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable )
{
    if( !agent || !pPion )
        return false;
    return agent->GetPion().GetRole< transport::PHY_RoleAction_Transport >().CanTransportPion( pPion->GetPion(), bTransportOnlyLoadable );
}

bool DEC_ActionFunctions::CanLoad( const DEC_Decision_ABC* pPion, const DEC_Decision_ABC* pTarget, bool bTransportOnlyLoadable )
{
    if( !pPion || !pTarget )
        return false;
    return pPion->GetPion().GetRole< transport::PHY_RoleAction_Transport >().CanLoad( pTarget->GetPion(), bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::GetNumberOfRoundTripToTransportPion
// Created: GGE 2013-04-04
// -----------------------------------------------------------------------------
double DEC_ActionFunctions::GetNumberOfRoundTripToTransportPion( const MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable )
{
    assert( pPion );
    return callerAgent.GetRole< transport::PHY_RoleAction_Transport >().GetNumberOfRoundTripToTransportPion( pPion->GetPion(), bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::CanTransportKnowledge
// Created: EVH 2011-05-30
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::CanTransportKnowledge( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, bool bTransportOnlyLoadable )
{
    if( !pKnowledge )
        throw MASA_EXCEPTION( "Null knowledge passed to DEC_ActionFunctions::CanTransportKnowledge" );
    MIL_Agent_ABC& pion = pKnowledge->GetAgentKnown();
    return callerAgent.GetPion().GetRole< transport::PHY_RoleAction_Transport >().CanTransportPion( pion, bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::GetNumberOfRoundTripToTransportKnowledge
// Created: GGE 2013-04-04
// -----------------------------------------------------------------------------
double DEC_ActionFunctions::GetNumberOfRoundTripToTransportKnowledge( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, bool bTransportOnlyLoadable )
{
    if( !pKnowledge )
        throw std::runtime_error( "Null knowledge passed to DEC_ActionFunctions::GetNumberOfRoundTripToTransportKnowledge" );
    MIL_Agent_ABC& pion = pKnowledge->GetAgentKnown();
    return callerAgent.GetPion().GetRole< transport::PHY_RoleAction_Transport >().GetNumberOfRoundTripToTransportPion( pion, bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::CanTransportCrowd
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::CanTransportCrowd( DEC_Decision_ABC& callerAgent )
{
    return callerAgent.GetPion().GetRole< PHY_RolePion_Composantes >().CanTransportCrowd();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::GetCapacityToTransportCrowd
// Created: GGE 2013-04-04
// -----------------------------------------------------------------------------
int DEC_ActionFunctions::GetCapacityToTransportCrowd( DEC_Decision_ABC& callerAgent )
{
    return callerAgent.GetPion().GetRole< PHY_RolePion_Composantes >().GetCrowdTransporterCapacity();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::IsTransportingCrowd
// Created: DDA 2011-08-31
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::IsTransportingCrowd( DEC_Decision_ABC& callerAgent )
{
    return callerAgent.GetPion().GetRole< crowdtransport::PHY_RoleAction_CrowdTransport >().IsTransportingCrowd();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_IsTransporting
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::Transport_IsTransporting( const MIL_AgentPion& callerAgent )
{
    return callerAgent.GetRole< transport::PHY_RoleAction_Transport >().IsTransporting();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Orientate
// Created: MGD 2010-07-22
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Orientate( MIL_AgentPion& callerAgent, boost::shared_ptr< MT_Vector2D > dir )
{
    callerAgent.GetRole< PHY_RoleInterface_Location >().Move( callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition(), *dir, 0. );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Stock_IsExtractPossible
// Created: BCI 2011-02-08
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::Stock_IsExtractPossible( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, const std::vector< const PHY_DotationCategory* >& dotationTypes )
{
    PHY_RoleInterface_Supply* supplyRole = callerAgent.RetrieveRole< PHY_RoleInterface_Supply >();
    if( !supplyRole )
        return false;

    PHY_RolePion_Composantes& composantes = callerAgent.GetRole< PHY_RolePion_Composantes >();

    if( !pKnowledge || !pKnowledge->GetObjectKnown() )
        throw MASA_EXCEPTION( "Null knowledge object in DEC_ActionFunctions::Stock_IsExtractPossible" );
    if( StockAttribute* attribute = pKnowledge->GetObjectKnown()->RetrieveAttribute< StockAttribute >() )
    {
        BOOST_FOREACH( const PHY_DotationCategory* pDotation, dotationTypes )
        {
            if( pDotation && attribute->CanDistribute( *pDotation ) && composantes.CanStockMoreOf( *supplyRole, *pDotation ) )
                return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Stock_IsSupplyPossible
// Created: BCI 2011-02-08
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::Stock_IsSupplyPossible( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, const std::vector< const PHY_DotationCategory* >& dotationTypes )
{
    PHY_RoleInterface_Supply* supplyRole = callerAgent.RetrieveRole< PHY_RoleInterface_Supply >();
    if( !supplyRole )
        return false;

    if( !pKnowledge || !pKnowledge->GetObjectKnown() )
        throw MASA_EXCEPTION( "Null knowledge object in DEC_ActionFunctions::Stock_IsSupplyPossible" );
    if( StockAttribute* attribute = pKnowledge->GetObjectKnown()->RetrieveAttribute< StockAttribute >() )
    {
        BOOST_FOREACH( const PHY_DotationCategory* pDotation, dotationTypes )
        {
            if( !pDotation )
                continue;
            PHY_DotationStock& stock = callerAgent.GetRole< PHY_RolePion_Composantes >().GetOrAddStock( *supplyRole, *pDotation );
            if( !stock.IsEmpty() && attribute->CanBeSuppliedWith( *pDotation ) )
                return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Stock_IsDistributePossible
// Created: BCI 2011-02-16
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::Stock_IsDistributePossible( MIL_AgentPion& /*callerAgent*/, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, boost::shared_ptr< DEC_Knowledge_Population > population )
{
    if( !pKnowledge || !pKnowledge->GetObjectKnown() )
        throw MASA_EXCEPTION( "Null knowledge object in DEC_ActionFunctions::Stock_IsDistributePossible" );
    if( StockAttribute* attribute = pKnowledge->GetObjectKnown()->RetrieveAttribute< StockAttribute >() )
        return !attribute->IsEmpty();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::IsSurrendered()
// Created: GGE 2012-02-17
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::IsSurrendered( DEC_Decision_ABC& callerAgent )
{
   if( callerAgent.GetPion().GetRole< surrender::PHY_RoleInterface_Surrender >().IsSurrendered() )
       return true;
   return false;
}

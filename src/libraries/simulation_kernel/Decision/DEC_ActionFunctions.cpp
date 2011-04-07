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
#include "Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.h"
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

//#include "Entities/Objects/LogisticCapacity.h"
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
    pKnowledge->GetAgentKnown().GetRole< surrender::PHY_RoleInterface_Surrender >().Imprison( *pCampKnowledge->GetObjectKnown() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Prisoners_IsUnloadedInCamp
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::Prisoners_IsUnloadedInCamp( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object > pCampKnowledge )
{
    if( IsNotCampKnowledgeOrHasLogisticCapacity( pKnowledge, pCampKnowledge ) )
        return false;
    return pKnowledge->GetAgentKnown().GetRole< surrender::PHY_RoleInterface_Surrender >().IsImprisoned( *pCampKnowledge->GetObjectKnown() ) ;
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
// Name: DEC_ActionFunctions::Refugees_IsLoaded
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::PrisonnersRefugees_IsLoaded( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return false;
    return callerAgent.GetRole< transport::PHY_RoleAction_Transport >().IsLoaded( pKnowledge->GetAgentKnown() ) ;
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
// Name: DEC_ActionFunctions::Transport_AddPion
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_AddPion( MIL_AgentPion& callerAgent, DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable )
{
    callerAgent.GetRole< transport::PHY_RoleAction_Transport >().AddPion( pPion->GetPion(), bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_AddPions
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_AddPions( MIL_AgentPion& callerAgent, const std::vector< DEC_Decision_ABC* >& pions, bool bTransportOnlyLoadable )
{
    for( std::vector< DEC_Decision_ABC* >::const_iterator itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        MIL_AgentPion& pion = ( *itPion )->GetPion();
        callerAgent.GetRole< transport::PHY_RoleAction_Transport >().AddPion( pion, bTransportOnlyLoadable );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicLoadPion
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicLoadPionInCarrier( MIL_AgentPion& caller, const DEC_Decision_ABC* pCarrier )
{
    assert( pCarrier );
    pCarrier->GetPion().Apply( &transport::TransportNotificationHandler_ABC::MagicLoadPion, caller, boost::cref( false ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicLoadPion
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicUnloadPionFromCarrier( MIL_AgentPion& caller )
{
    //$$$ TMP
    const MIL_Agent_ABC* transporter = caller.GetRole< transport::PHY_RolePion_Transported >().GetTransporter();
    if( transporter )
        const_cast< MIL_Agent_ABC* >( transporter )->GetRole< transport::PHY_RoleAction_Transport >().MagicUnloadPion( caller );;
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicLoadPion
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
void DEC_ActionFunctions::Transport_MagicLoadPion( MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable  )
{
    assert( pPion );
    callerAgent.Apply( &transport::TransportNotificationHandler_ABC::MagicLoadPion, pPion->GetPion(), bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicLoadPions
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicLoadPions( MIL_AgentPion& callerAgent, const std::vector< DEC_Decision_ABC* >& pions, bool bTransportOnlyLoadable )
{
    for( std::vector< DEC_Decision_ABC* >::const_iterator itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        MIL_AgentPion& pion = ( *itPion )->GetPion();
        callerAgent.Apply( &transport::TransportNotificationHandler_ABC::MagicLoadPion, pion, bTransportOnlyLoadable );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicUnloadPions
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicUnloadPions( MIL_AgentPion& callerAgent, const std::vector< DEC_Decision_ABC* >& pions )
{
    for( std::vector< DEC_Decision_ABC* >::const_iterator itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        MIL_AgentPion& pion = ( *itPion )->GetPion();
        callerAgent.GetRole< transport::PHY_RoleAction_Transport >().MagicUnloadPion( pion );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicUnloadPion
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicUnloadPion( MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pPion )
{
    assert( pPion );
    callerAgent.GetRole< transport::PHY_RoleAction_Transport >().MagicUnloadPion( pPion->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_IsFinished
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::Transport_IsFinished( const MIL_AgentPion& callerAgent )
{
    return callerAgent.GetRole< transport::PHY_RoleAction_Transport >().IsFinished() ;
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
// Name: DEC_ActionFunctions::CanTransportPion
// Created: JVT 2005-01-18
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::CanTransportPion( const MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable )
{
    assert( pPion );
    return callerAgent.GetRole< transport::PHY_RoleAction_Transport >().CanTransportPion( pPion->GetPion(), bTransportOnlyLoadable ) ;
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

    if( StockAttribute* attribute = pKnowledge->GetObjectKnown()->RetrieveAttribute< StockAttribute >() )
    {
        BOOST_FOREACH( const PHY_DotationCategory* pDotation, dotationTypes )
        {
            if( attribute->CanDistribute( *pDotation ) && composantes.CanStockMoreOf( *supplyRole, *pDotation ) )
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

    if( StockAttribute* attribute = pKnowledge->GetObjectKnown()->RetrieveAttribute< StockAttribute >() )
    {
        BOOST_FOREACH( const PHY_DotationCategory* pDotation, dotationTypes )
        {
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
bool DEC_ActionFunctions::Stock_IsDistributePossible( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, boost::shared_ptr< DEC_Knowledge_Population > population )
{
    if( StockAttribute* attribute = pKnowledge->GetObjectKnown()->RetrieveAttribute< StockAttribute >() )
        return !attribute->IsEmpty();
    return false;
}

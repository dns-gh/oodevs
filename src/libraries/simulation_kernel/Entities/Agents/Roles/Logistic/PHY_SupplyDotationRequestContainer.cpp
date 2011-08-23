// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Roles/Logistic/PHY_SupplyDotationRequestContainer.cpp $
// $Author: Nld $
// $Modtime: 3/03/05 16:04 $
// $Revision: 4 $
// $Workfile: PHY_SupplyDotationRequestContainer.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_SupplyDotationRequestContainer.h"
#include "PHY_SupplyDotationState.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Dotations/PHY_Dotation.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationRequestContainer constructor
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
PHY_SupplyDotationRequestContainer::PHY_SupplyDotationRequestContainer( MIL_Automate& suppliedAutomate, bool bForceRequestActivation )
    : suppliedAutomate_                  ( suppliedAutomate )
    , requests_                          ()
    , bForceRequestActivation_           ( bForceRequestActivation )
    , bAtLeastOneExplicitSupplySatisfied_( false )
    , bExplicitSupplyFullSatisfied_      ( false )
    , bAtLeastOneSupplySatisfied_        ( false )
{
    const MIL_Automate::T_PionVector& pions = suppliedAutomate.GetPions();
    for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
        (**itPion).GetRole< dotation::PHY_RoleInterface_Dotations >().FillSupplyRequest( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationRequestContainer destructor
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
PHY_SupplyDotationRequestContainer::~PHY_SupplyDotationRequestContainer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationRequestContainer::Execute
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
bool PHY_SupplyDotationRequestContainer::Execute( const logistic::LogisticHierarchy_ABC& logisticHierarchy, MIL_DotationSupplyManager::T_SupplyDotationStateMap& dotationSupplies )
{
    if( requests_.empty() )
        return true;

    tools::Iterator< MIL_AutomateLOG& > it = logisticHierarchy.CreateSuperiorsIterator();
    while( it.HasMoreElements() )
        AffectRequestsToAutomate( it.NextElement() );

    if( bAtLeastOneExplicitSupplySatisfied_ || ( bAtLeastOneSupplySatisfied_ && bForceRequestActivation_ ) )
        ActivateSupply( dotationSupplies );
    return bExplicitSupplyFullSatisfied_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationRequestContainer::AddDotation
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_SupplyDotationRequestContainer::AddDotation( PHY_Dotation& dotation )
{
    if( !dotation.NeedSupply() )
        return;

    PHY_SupplyDotationRequest& request = requests_[ &dotation.GetCategory() ];
    request.AddDotation( dotation );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationRequestContainer::AffectRequestsToAutomate
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
void PHY_SupplyDotationRequestContainer::AffectRequestsToAutomate( MIL_AutomateLOG& supplyingAutomate )
{
    bExplicitSupplyFullSatisfied_ = true;

    for( IT_RequestMap it = requests_.begin(); it != requests_.end(); ++it )
    {
        PHY_SupplyDotationRequest& request = it->second;

        if( request.AffectSupplier( supplyingAutomate ) )
        {
            bAtLeastOneSupplySatisfied_ = true;
            if( request.HasReachedSupplyThreshold() )
                bAtLeastOneExplicitSupplySatisfied_ = true;
        }
        else if( request.HasReachedSupplyThreshold() )
            bExplicitSupplyFullSatisfied_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationRequestContainer::ActivateSupply
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
void PHY_SupplyDotationRequestContainer::ActivateSupply( MIL_DotationSupplyManager::T_SupplyDotationStateMap& dotationSupplies )
{
    for( IT_RequestMap it = requests_.begin(); it != requests_.end(); ++it )
    {
        PHY_SupplyDotationRequest& request = it->second;
        if( !request.GetSupplier() )
            continue;

        PHY_SupplyDotationState*& pSupplyState = dotationSupplies[ request.GetSupplier() ];
        if( !pSupplyState )
        {
            pSupplyState = new PHY_SupplyDotationState( suppliedAutomate_, *request.GetSupplier() );
            request.GetSupplier()->SupplyHandleRequest( *pSupplyState, request.GetStockPion() );
        }
        request.ReserveStocks();
        pSupplyState->AddRequest( request );
    }

    for( MIL_Automate::CIT_SupplyDotationStateMap itState = dotationSupplies.begin(); itState != dotationSupplies.end(); ++itState )
        itState->second->SendMsgCreation();
}

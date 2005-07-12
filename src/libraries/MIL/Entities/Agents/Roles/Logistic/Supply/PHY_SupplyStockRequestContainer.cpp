// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyStockRequestContainer.cpp $
// $Author: Nld $
// $Modtime: 8/02/05 10:16 $
// $Revision: 2 $
// $Workfile: PHY_SupplyStockRequestContainer.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_SupplyStockRequestContainer.h"
#include "PHY_SupplyStockState.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_RolePion_Supply.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationStock.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequestContainer constructor
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
PHY_SupplyStockRequestContainer::PHY_SupplyStockRequestContainer( MIL_AutomateLOG& suppliedAutomate )
    : suppliedAutomate_                  ( suppliedAutomate )
    , requests_                          ()
    , bAtLeastOneExplicitSupplySatisfied_( false )
    , bAtLeastOneSupplySatisfied_        ( false )
    , bExplicitSupplyFullSatisfied_      ( false )
    , bPushedFlow_                       ( false )
{
    const MIL_Automate::T_PionVector& pions = suppliedAutomate.GetPions();
    for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
        (**itPion).GetRole< PHY_RolePion_Supply >().FillSupplyRequest( *this );    
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequestContainer constructor
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
PHY_SupplyStockRequestContainer::PHY_SupplyStockRequestContainer( MIL_AutomateLOG& suppliedAutomate, ASN1T__SeqOfDotationStock& asnStocks )
    : suppliedAutomate_                  ( suppliedAutomate )
    , requests_                          ()
    , bAtLeastOneExplicitSupplySatisfied_( false )
    , bAtLeastOneSupplySatisfied_        ( false )
    , bExplicitSupplyFullSatisfied_      ( false )
    , bPushedFlow_                       ( true )
{
    const MIL_Automate::T_PionVector& pions = suppliedAutomate.GetPions();
    for( uint i = 0; i < asnStocks.n; ++i )
    {
        const ASN1T_DotationStock& asnStock = asnStocks.elem[i];
        const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( asnStock.ressource_id );
        if( !pDotationCategory )
            continue;

        for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
        {
            PHY_DotationStock* pStock = (**itPion).GetRole< PHY_RolePion_Supply >().GetStock( *pDotationCategory );
            if( pStock )
                AddStockPushedFlow( *pStock );
        }
        IT_RequestMap itRequest = requests_.find( pDotationCategory );
        if( itRequest != requests_.end() )
            itRequest->second.SetStockPushedFlowValue( asnStock.quantite_disponible );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequestContainer destructor
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
PHY_SupplyStockRequestContainer::~PHY_SupplyStockRequestContainer()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequestContainer::Execute
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
bool PHY_SupplyStockRequestContainer::Execute( MIL_AutomateLOG& supplyingAutomate, PHY_SupplyStockState*& pStockSupplyState )
{
    if( requests_.empty() )
        return true;

    if( !bPushedFlow_ && !ApplyQuotas() )
        return false;

    AffectRequestsToAutomate( supplyingAutomate );

    if( ( bPushedFlow_ && bAtLeastOneSupplySatisfied_ ) || ( !bPushedFlow_ && bAtLeastOneExplicitSupplySatisfied_ ) )
        ActivateSupply( pStockSupplyState );

    return bExplicitSupplyFullSatisfied_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequestContainer::AddStock
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_SupplyStockRequestContainer::AddStock( PHY_DotationStock& stock )
{
    if( !stock.NeedSupply() )
        return;

    PHY_SupplyStockRequest& request = requests_[ &stock.GetCategory() ];
    request.AddStock( stock );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequestContainer::AddStockPushedFlow
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
void PHY_SupplyStockRequestContainer::AddStockPushedFlow( PHY_DotationStock& stock )
{
    PHY_SupplyStockRequest& request = requests_[ &stock.GetCategory() ];
    request.AddStockPushedFlow( stock );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequestContainer::AffectRequestsToAutomate
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_SupplyStockRequestContainer::AffectRequestsToAutomate( MIL_AutomateLOG& supplyingAutomate )
{ 
    bExplicitSupplyFullSatisfied_ = true;
    for( IT_RequestMap it = requests_.begin(); it != requests_.end(); ++it )
    {
        PHY_SupplyStockRequest& request = it->second;

        if( request.AffectAutomate( supplyingAutomate ) )
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
// Name: PHY_SupplyStockRequestContainer::ApplyQuotas
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
bool PHY_SupplyStockRequestContainer::ApplyQuotas()
{
    for( IT_RequestMap it = requests_.begin(); it != requests_.end(); )
    {
        it->second.ApplyQuota( suppliedAutomate_.GetQuota( *it->first ) );
        if( it->second.GetTotalRequestedValue() == 0. )
            it = requests_.erase( it );
        else
            ++it;
    }
    return !requests_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequestContainer::ActivateSupply
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
void PHY_SupplyStockRequestContainer::ActivateSupply( PHY_SupplyStockState*& pStockSupplyState )
{
    assert( !pStockSupplyState );
    for( IT_RequestMap it = requests_.begin(); it != requests_.end(); ++it )
    {
        PHY_SupplyStockRequest& request = it->second;
        if( !request.GetSupplyingAutomate() )
            continue;

        if( !pStockSupplyState )
        {
            pStockSupplyState = new PHY_SupplyStockState( suppliedAutomate_ );
            request.GetSupplyingAutomate()->SupplyHandleRequest( *pStockSupplyState );
        }

        request.ReserveStocks();
        if( !bPushedFlow_ )
            suppliedAutomate_.ConsumeQuota( request.GetDotationCategory(), request.GetTotalReservedValue() );
        pStockSupplyState->AddRequest( request );
    }

    if( pStockSupplyState )
        pStockSupplyState->SendMsgCreation();
}

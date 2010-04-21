// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Roles/Logistic/PHY_SupplyStockRequestContainer.cpp $
// $Author: Nld $
// $Modtime: 8/02/05 10:16 $
// $Revision: 2 $
// $Workfile: PHY_SupplyStockRequestContainer.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_SupplyStockRequestContainer.h"
#include "PHY_SupplyStockState.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationStock.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "protocol/protocol.h"

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
        if( PHY_RoleInterface_Supply* stockPion = (**itPion).RetrieveRole< PHY_RoleInterface_Supply >() )
            stockPion->FillSupplyRequest( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequestContainer constructor
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
PHY_SupplyStockRequestContainer::PHY_SupplyStockRequestContainer( MIL_AutomateLOG& suppliedAutomate, const Common::MsgMissionParameter& asnStocks )
    : suppliedAutomate_                  ( suppliedAutomate )
    , requests_                          ()
    , bAtLeastOneExplicitSupplySatisfied_( false )
    , bAtLeastOneSupplySatisfied_        ( false )
    , bExplicitSupplyFullSatisfied_      ( false )
    , bPushedFlow_                       ( true )
{
    if( !asnStocks.has_value() || asnStocks.value().list_size() == 0 )
        return;

    const MIL_Automate::T_PionVector& pions = suppliedAutomate.GetPions();
    for( int i = 0; i < asnStocks.value().list_size(); ++i )
    {
        unsigned int type = asnStocks.value().list( i ).list( 0 ).identifier();
        int number = asnStocks.value().list( i ).list( 1 ).quantity();

        const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( type );
        if( !pDotationCategory )
            continue;

        MT_Float rTotalValue = number;

        typedef std::vector< std::pair< PHY_DotationStock*, MT_Float > > T_PionStockVector;
        typedef T_PionStockVector::iterator                              IT_PionStockVector;
        typedef T_PionStockVector::const_iterator                        CIT_PionStockVector;

        T_PionStockVector pionStocks;
        for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
        {
            PHY_RoleInterface_Supply* stockPion = (**itPion).RetrieveRole< PHY_RoleInterface_Supply >();
            if( stockPion )
            {
                PHY_DotationStock* pStock = stockPion->GetStock( *pDotationCategory );
                if( pStock )
                    pionStocks.push_back( std::make_pair( pStock, 0. ) );
            }

        }

        if( pionStocks.empty() )
        {
            PHY_DotationStock* pNewStock = 0;
            for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end() && !pNewStock; ++itPion )
            {
                PHY_RoleInterface_Supply* stockPion = (**itPion).RetrieveRole< PHY_RoleInterface_Supply >();
                if( stockPion )
                {
                    pNewStock = stockPion->AddStock( *pDotationCategory );
                }
            }
                
            
            if( !pNewStock )
                continue;

            pionStocks.push_back( std::make_pair( pNewStock, 0. ) );
        }

        // Priority for pions needing supply
        for( IT_PionStockVector it = pionStocks.begin(); it != pionStocks.end() && rTotalValue > 0.; ++it )
        {
            const PHY_DotationStock& stock = *it->first;
            const MT_Float rAffectedValue = std::min( rTotalValue, std::max( 0., stock.GetCapacity() - stock.GetValue() ) );
            rTotalValue -= rAffectedValue;
            it->second += rAffectedValue;
        }

        // Overhead
        if( rTotalValue > 0. )
        {
            const MT_Float rAffectedValue = rTotalValue / pionStocks.size();
            for( IT_PionStockVector it = pionStocks.begin(); it != pionStocks.end(); ++it )
                it->second += rAffectedValue;
        }

        // Request creation
        PHY_SupplyStockRequest& request = requests_[ pDotationCategory ];
        for( CIT_PionStockVector it = pionStocks.begin(); it != pionStocks.end(); ++it )
            request.AddStock( *it->first, it->second );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequestContainer destructor
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
PHY_SupplyStockRequestContainer::~PHY_SupplyStockRequestContainer()
{
    // NOTHING
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
            pStockSupplyState = new PHY_SupplyStockState( suppliedAutomate_, bPushedFlow_ );
            request.GetSupplyingAutomate()->SupplyHandleRequest( *pStockSupplyState );
        }

        request.ReserveStocks();
        pStockSupplyState->AddRequest( request );
    }

    if( pStockSupplyState )
        pStockSupplyState->SendMsgCreation();
}

*****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyStockManualRequestBuilder_ABC.h"
#include "SupplyRequestContainer_ABC.h"
#include "SupplyResourceStock.h"
#include "Entities/Automates/MIL_Automate.h" //$$$ A GICLER
#include "Entities/Automates/MIL_StockSupplyManager.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationStock.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteTypePion.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h" //$$$$ A GICLER
#include "Entities/Agents/MIL_AgentPion.h" //$$$$ A GICLER
#include <boost/foreach.hpp>

using namespace logistic;

// -----------------------------------------------------------------------------
// Name: SupplyStockManualRequestBuilder_ABC::SupplyStockManualRequestBuilder_ABC
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyStockManualRequestBuilder_ABC::SupplyStockManualRequestBuilder_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: SupplyStockManualRequestBuilder_ABC::~SupplyStockManualRequestBuilder_ABC
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyStockManualRequestBuilder_ABC::~SupplyStockManualRequestBuilder_ABC()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyStockManualRequestBuilder_ABC::CreateRequest
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyStockManualRequestBuilder_ABC::CreateRequest( MIL_Automate& recipient, const sword::SupplyFlowResource& resource, SupplyRequestContainer_ABC& container ) const
{
    const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( resource.resourcetype().id() );
    if( !pDotationCategory )
        return;

    double rTotalValue = resource.quantity();

    typedef std::vector< std::pair< PHY_DotationStock*, double > > T_PionStockVector;

    T_PionStockVector pionStocks;
    BOOST_FOREACH( MIL_AgentPion* pion, recipient.GetPions() )
    {
        PHY_RoleInterface_Supply* stockPion = pion->RetrieveRole< PHY_RoleInterface_Supply >();
        if( stockPion )
        {
            PHY_DotationStock* pStock = stockPion->GetStock( *pDotationCategory );
            if( pStock )
                pionStocks.push_back( std::make_pair( pStock, 0. ) );
        }
    }
    // Add dynamically a slot for the dotation category, if no initial stock received can hold this kind of dotation
    if( pionStocks.empty() )
    {
        PHY_DotationStock* pNewStock = 0;
        BOOST_FOREACH( MIL_AgentPion* pion, recipient.GetPions() )
        {
            PHY_RoleInterface_Supply* stockPion = pion->RetrieveRole< PHY_RoleInterface_Supply >();
            if( stockPion )
                pNewStock = stockPion->AddStock( *pDotationCategory );
        }
        if( pNewStock )
            pionStocks.push_back( std::make_pair( pNewStock, 0. ) );
    }

    // Priority for pions needing supply
    for( T_PionStockVector::iterator it = pionStocks.begin(); it != pionStocks.end() && rTotalValue > 0.; ++it )
    {
        const PHY_DotationStock& stock = *it->first;
        const double rAffectedValue = std::min( rTotalValue, std::max( 0., stock.GetCapacity() - stock.GetValue() ) );
        rTotalValue -= rAffectedValue;
        it->second += rAffectedValue;
    }

    // Overhead
    if( rTotalValue > 0. && !pionStocks.empty() )
    {
        const double rAffectedValue = rTotalValue / pionStocks.size();
        for( T_PionStockVector::iterator it = pionStocks.begin(); it != pionStocks.end(); ++it )
            it->second += rAffectedValue;
    }

    BOOST_FOREACH( T_PionStockVector::value_type& data, pionStocks )
        container.AddResource( recipient.GetStockSupplyManager(), boost::shared_ptr< SupplyResource_ABC >( new SupplyResourceStock( *data.first ) ), data.second );
}


// -----------------------------------------------------------------------------
// Name: SupplyStockManualRequestBuilder_ABC::SetTransporters
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyStockManualRequestBuilder_ABC::SetTransporters( const google::protobuf::RepeatedPtrField< ::sword::SupplyFlowTransporter >& transporters, SupplyRequestContainer_ABC& container ) const
{
    if( transporters.size() == 0 )
        return;

    std::vector< std::pair< const PHY_ComposanteTypePion* , unsigned > > newTransporters;
    BOOST_FOREACH( const sword::SupplyFlowTransporter& transporter, transporters )
    {
        const PHY_ComposanteTypePion* type = PHY_ComposanteTypePion::Find( transporter.equipmenttype() );
        if( type )
            newTransporters.push_back( std::make_pair( type, transporter.quantity() ) );
    }
    container.SetTransporters( newTransporters );
}

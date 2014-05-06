//*****************************************************************************
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
#include "Entities/Specialisations/LOG/MIL_LogisticVisitors.h"
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

using namespace logistic;

BOOST_CLASS_EXPORT_IMPLEMENT( logistic::SupplyStockManualRequestBuilder_ABC )

// -----------------------------------------------------------------------------
// Name: SupplyStockManualRequestBuilder_ABC::SupplyStockManualRequestBuilder_ABC
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyStockManualRequestBuilder_ABC::SupplyStockManualRequestBuilder_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyStockManualRequestBuilder_ABC::~SupplyStockManualRequestBuilder_ABC
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyStockManualRequestBuilder_ABC::~SupplyStockManualRequestBuilder_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyStockManualRequestBuilder_ABC::CreateRequest
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyStockManualRequestBuilder_ABC::CreateRequest( MIL_Automate& recipient, const sword::SupplyFlowResource& resource, SupplyRequestContainer_ABC& container ) const
{
    const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( resource.resourcetype().id() );
    if( !pDotationCategory )
        return;

    const auto& pions = recipient.GetPions();
    std::vector< SupplyStockQuantity > stocks;
    for( auto it = pions.begin(); it != pions.end(); ++it )
        if( PHY_RoleInterface_Supply* pion = (*it)->RetrieveRole< PHY_RoleInterface_Supply >() )
            if( PHY_DotationStock* stock = pion->GetStock( *pDotationCategory ) )
                stocks.push_back( SupplyStockQuantity( *it, stock ) );
    // Add dynamically a slot for the dotation category, if no initial stock received can hold this kind of dotation
    if( stocks.empty() )
        for( auto it = pions.begin(); it != pions.end(); ++it )
            if( PHY_RoleInterface_Supply* pion = (*it)->RetrieveRole< PHY_RoleInterface_Supply >() )
                if( PHY_DotationStock* stock = pion->AddStock( *pDotationCategory ) )
                {
                    stocks.push_back( SupplyStockQuantity( *it, stock ) );
                    break;
                }

    double rTotalValue = resource.quantity();
    // Priority for pions needing supply
    for( auto it = stocks.begin(); it != stocks.end() && rTotalValue > 0.; ++it )
    {
        const PHY_DotationStock& stock = *it->stock_;
        const double rAffectedValue = std::min( rTotalValue, std::max( 0., stock.GetCapacity() - stock.GetValue() ) );
        rTotalValue -= rAffectedValue;
        it->quantity_ += rAffectedValue;
    }

    // Overhead
    if( rTotalValue > 0 && !stocks.empty() )
    {
        const double rAffectedValue = rTotalValue / stocks.size();
        for( auto it = stocks.begin(); it != stocks.end(); ++it )
            it->quantity_ += rAffectedValue;
    }

    for( auto it = stocks.begin(); it != stocks.end(); ++it )
        container.AddResource(
            recipient.GetStockSupplyManager(),
            *it->pion_,
            boost::make_shared< SupplyResourceStock >( *it->stock_ ),
            it->quantity_ );
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
        if( const PHY_ComposanteTypePion* type = PHY_ComposanteTypePion::Find( transporter.equipmenttype().id() ) )
            newTransporters.push_back( std::make_pair( type, transporter.quantity() ) );
    container.SetTransporters( newTransporters );
}

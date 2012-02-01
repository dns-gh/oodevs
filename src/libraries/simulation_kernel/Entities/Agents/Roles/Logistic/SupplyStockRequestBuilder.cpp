//*****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyStockRequestBuilder.h"
#include "SupplyRequestContainer_ABC.h"
#include "SupplyResourceStock.h"
#include "SupplyConvoyRealFactory.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Automates/MIL_Automate.h" //$$$ A GICLER
#include "Entities/Agents/Units/Dotations/PHY_DotationStock.h"

using namespace logistic;

// -----------------------------------------------------------------------------
// Name: SupplyDotationRequestBuilder::SupplyDotationRequestBuilder
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyStockRequestBuilder::SupplyStockRequestBuilder( MIL_Automate& automate, SupplyRecipient_ABC& recipient )
    : automate_ ( automate )
    , recipient_( recipient )
{
}

// -----------------------------------------------------------------------------
// Name: SupplyStockRequestBuilder::SupplyStockRequestBuilder
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyStockRequestBuilder::~SupplyStockRequestBuilder()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyStockRequestBuilder::CreateRequest
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyStockRequestBuilder::Process( SupplyRequestContainer_ABC& container )
{
    automate_.Apply2( (boost::function< void( PHY_DotationStock& ) >)boost::bind( &SupplyStockRequestBuilder::VisitStock, this, _1, boost::ref( container ) ) );
    container.SetTransportersProvider( automate_.FindLogisticManager() );
    container.SetConvoyFactory( SupplyConvoyRealFactory::Instance() );
}

// -----------------------------------------------------------------------------
// Name: SupplyStockRequestBuilder::VisitStock
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void SupplyStockRequestBuilder::VisitStock( PHY_DotationStock& stock, SupplyRequestContainer_ABC& container ) const
{
    if( stock.NeedSupply() )
    {
        double quantityToRequest = std::max( 0., stock.GetCapacity() - stock.GetValue() ); //$$ devrait être le retour de NeedSupply() ?
        container.AddResource( recipient_, boost::shared_ptr< SupplyResource_ABC >( new SupplyResourceStock( stock ) ), quantityToRequest );
    }
}


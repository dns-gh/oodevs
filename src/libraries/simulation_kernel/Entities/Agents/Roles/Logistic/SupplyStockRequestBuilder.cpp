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
#include "SupplyConvoyRealFactory.h"
#include "SupplyRecipient_ABC.h"
#include "SupplyRequestContainer_ABC.h"
#include "SupplyResourceStock.h"
#include "SupplyConvoyConfig.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationStock.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include <boost/make_shared.hpp>

using namespace logistic;

BOOST_CLASS_EXPORT_IMPLEMENT( logistic::SupplyStockRequestBuilder )

// -----------------------------------------------------------------------------
// Name: SupplyDotationRequestBuilder::SupplyDotationRequestBuilder
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyStockRequestBuilder::SupplyStockRequestBuilder( MIL_Automate& automate, SupplyRecipient_ABC& recipient )
    : automate_ ( &automate )
    , recipient_( &recipient )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyStockRequestBuilder constructor
// Created: JSR 2013-01-17
// -----------------------------------------------------------------------------
SupplyStockRequestBuilder::SupplyStockRequestBuilder()
    : automate_ ( 0 )
    , recipient_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyStockRequestBuilder::SupplyStockRequestBuilder
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyStockRequestBuilder::~SupplyStockRequestBuilder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyStockRequestBuilder::CreateRequest
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyStockRequestBuilder::Process( SupplyRequestContainer_ABC& container )
{
    std::function< void( const MIL_AgentPion& pion, PHY_DotationStock& ) > visitor =
        boost::bind( &SupplyStockRequestBuilder::VisitStock, this, _1, _2, boost::ref( container ) );
    automate_->Apply2( visitor );
    MIL_AutomateLOG* logisticManager = automate_->FindLogisticManager();
    // If TC2: pulled flow. If BL: Pushed flow. cf. http://jira.masagroup.net/browse/SWBUG-9331
    if( logisticManager && logisticManager->GetLogisticHierarchy().HasSuperior()&& !automate_->GetBrainLogistic() )
        logisticManager = logisticManager->GetLogisticHierarchy().GetPrimarySuperior();
    container.SetTransportersProvider( logisticManager );
    container.SetConvoyFactory( SupplyConvoyConfig::GetStockSupplyConvoyFactory() );
}

// -----------------------------------------------------------------------------
// Name: SupplyStockRequestBuilder::VisitStock
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void SupplyStockRequestBuilder::VisitStock( const MIL_AgentPion& pion, PHY_DotationStock& stock, SupplyRequestContainer_ABC& container ) const
{
    if( stock.NeedSupply() )
    {
        const double quantityToRequest = std::max( 0., stock.GetCapacity() - stock.GetValue() ); //$$ devrait être le retour de NeedSupply() ?
        container.AddResource( *recipient_, pion, boost::make_shared< SupplyResourceStock >( stock ), quantityToRequest );
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyStockRequestBuilder::serialize
// Created: LDC 2013-01-17
// -----------------------------------------------------------------------------
template< typename Archive > void SupplyStockRequestBuilder::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< SupplyRequestBuilder_ABC >( *this );
    archive & automate_;
    archive & recipient_;
}

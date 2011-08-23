*****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyStockPullFlowRequestBuilder.h"
#include "SupplyRequestContainer_ABC.h"
#include "SupplyConvoyRealFactory.h"
#include "Entities/Automates/MIL_Automate.h" //$$$ A GICLER
#include "Entities/Automates/MIL_StockSupplyManager.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h" //$$$ A GICLER
#include "Network/NET_ASN_Tools.h"
#include "protocol/ClientSenders.h"
#include <boost/foreach.hpp>

using namespace logistic;

// -----------------------------------------------------------------------------
// Name: SupplyStockPullFlowRequestBuilder::SupplyDotationRequestBuilder
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyStockPullFlowRequestBuilder::SupplyStockPullFlowRequestBuilder( const sword::PullFlowParameters& parameters, MIL_Automate& recipient, SupplySupplier_ABC& supplier )
    : pullFlowParameters_( parameters )
    , recipient_         ( recipient )
    , supplier_          ( supplier )
{
}

// -----------------------------------------------------------------------------
// Name: SupplyStockPullFlowRequestBuilder::~SupplyStockPullFlowRequestBuilder
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyStockPullFlowRequestBuilder::~SupplyStockPullFlowRequestBuilder()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyStockPullFlowRequestBuilder::CreateRequest
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyStockPullFlowRequestBuilder::Process( SupplyRequestContainer_ABC& container )
{
    BOOST_FOREACH( const sword::SupplyFlowResource& resource, pullFlowParameters_.resources() )
        CreateRequest( recipient_, resource, container );
    
    if( pullFlowParameters_.has_wayoutpath() )
    {
        T_PointVector wayPoints;
        if( NET_ASN_Tools::ReadPointList( pullFlowParameters_.wayoutpath(), wayPoints ) )
            container.SetPathToSupplier( wayPoints );
    }

    if( pullFlowParameters_.has_waybackpath() )
    {
        T_PointVector wayPoints;
        if( NET_ASN_Tools::ReadPointList( pullFlowParameters_.waybackpath(), wayPoints ) )
            container.SetPathToRecipient( recipient_.GetStockSupplyManager(), wayPoints );
    }
    container.SetTransportersProvider( recipient_.FindLogisticManager() );
    SetTransporters( pullFlowParameters_.transporters(), container );
    container.SetConvoyFactory( SupplyConvoyRealFactory::Instance() );
}

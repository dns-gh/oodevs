//*****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyStockPushFlowRequestBuilder.h"
#include "SupplyRequestContainer_ABC.h"
#include "SupplyConvoyRealFactory.h"
#include "Entities/Automates/MIL_Automate.h" //$$$ A GICLER
#include "Entities/Automates/MIL_StockSupplyManager.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/ClientSenders.h"
#include <boost/foreach.hpp>

using namespace logistic;

// -----------------------------------------------------------------------------
// Name: SupplyStockPushFlowRequestBuilder::SupplyDotationRequestBuilder
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyStockPushFlowRequestBuilder::SupplyStockPushFlowRequestBuilder( const sword::PushFlowParameters& parameters, SupplySupplier_ABC& supplier, const tools::Resolver_ABC< MIL_Automate >& recipientResolver )
    : pushFlowParameters_( parameters )
    , supplier_          ( supplier )
    , recipientResolver_ ( recipientResolver )
{
}

// -----------------------------------------------------------------------------
// Name: SupplyStockPushFlowRequestBuilder::~SupplyStockPushFlowRequestBuilder
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyStockPushFlowRequestBuilder::~SupplyStockPushFlowRequestBuilder()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyStockPushFlowRequestBuilder::CreateRequest
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyStockPushFlowRequestBuilder::Process( SupplyRequestContainer_ABC& container )
{
    BOOST_FOREACH( const sword::SupplyFlowRecipient& data, pushFlowParameters_.recipients() )
    {
        MIL_Automate* recipient = recipientResolver_.Find( data.receiver().id() );
        if( recipient )
        {
            BOOST_FOREACH( const sword::SupplyFlowResource& resource, data.resources() )
                CreateRequest( *recipient, resource, container );

            if( data.has_path() )
            {
                T_PointVector wayPoints;
                if( NET_ASN_Tools::ReadPointList( data.path(), wayPoints ) )
                    container.SetPathToRecipient( recipient->GetStockSupplyManager(), wayPoints );
            }
        }
    }
    if( pushFlowParameters_.has_waybackpath() )
    {
        T_PointVector wayPoints;
        if( NET_ASN_Tools::ReadPointList( pushFlowParameters_.waybackpath(), wayPoints ) )
            container.SetPathToTransportersProvider( wayPoints );
    }
    container.SetTransportersProvider( &supplier_ );
    SetTransporters( pushFlowParameters_.transporters(), container );
    container.SetConvoyFactory( SupplyConvoyRealFactory::Instance() );
}

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
#include "AutomateFactory_ABC.h"
#include "SupplyConvoyRealFactory.h"
#include "SupplyRequestContainer_ABC.h"
#include "SupplyConvoyConfig.h"
#include "SupplySupplier_ABC.h"
#include "Entities/Automates/MIL_Automate.h" //$$$ A GICLER
#include "Entities/Automates/MIL_StockSupplyManager.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/ClientSenders.h"
#include <boost/foreach.hpp>

using namespace logistic;

BOOST_CLASS_EXPORT_IMPLEMENT( logistic::SupplyStockPushFlowRequestBuilder )

// -----------------------------------------------------------------------------
// Name: SupplyStockPushFlowRequestBuilder::SupplyDotationRequestBuilder
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyStockPushFlowRequestBuilder::SupplyStockPushFlowRequestBuilder( const sword::PushFlowParameters& parameters, SupplySupplier_ABC& supplier, const AutomateFactory_ABC& recipientResolver )
    : pushFlowParameters_( parameters )
    , supplier_          ( &supplier )
    , recipientResolver_ ( &recipientResolver )
{
}

// -----------------------------------------------------------------------------
// Name: SupplyStockPushFlowRequestBuilder constructor
// Created: LDC 2013-01-17
// -----------------------------------------------------------------------------
SupplyStockPushFlowRequestBuilder::SupplyStockPushFlowRequestBuilder()
    : supplier_          ( 0 )
    , recipientResolver_ ( 0 )
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
// Name: SupplyStockPushFlowRequestBuilder::Process
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyStockPushFlowRequestBuilder::Process( SupplyRequestContainer_ABC& container )
{
    MT_Vector2D position = supplier_->GetPosition();
    BOOST_FOREACH( const sword::SupplyFlowRecipient& data, pushFlowParameters_.recipients() )
    {
        MIL_Automate* recipient = recipientResolver_->Find( data.receiver().id() );
        if( recipient )
        {
            BOOST_FOREACH( const sword::SupplyFlowResource& resource, data.resources() )
                CreateRequest( *recipient, resource, container );

            if( data.has_pathfind() )
                container.SetPathToRecipient( recipient->GetStockSupplyManager(), data.pathfind(),
                                              position, recipient->GetPosition() );
            position = recipient->GetPosition();
        }
    }

    if( pushFlowParameters_.has_waybackpathfind() )
        container.SetPathToTransportersProvider( pushFlowParameters_.waybackpathfind(), position, supplier_->GetPosition() );

    container.SetTransportersProvider( supplier_ );
    SetTransporters( pushFlowParameters_.transporters(), container );
    container.SetConvoyFactory( SupplyConvoyConfig::GetStockSupplyConvoyFactory() );
}


// -----------------------------------------------------------------------------
// Name: SupplyStockPushFlowRequestBuilder::serialize
// Created: LDC 2013-01-17
// -----------------------------------------------------------------------------
void SupplyStockPushFlowRequestBuilder::serialize( MIL_CheckPointInArchive& archive, const unsigned int )
{
    archive >> boost::serialization::base_object< SupplyStockManualRequestBuilder_ABC >( *this );
    std::string pushFlow;
    archive >> pushFlow;
    const_cast< sword::PushFlowParameters& >( pushFlowParameters_ ).ParseFromString( pushFlow );
    archive >> supplier_;
    archive >> const_cast< AutomateFactory_ABC*& >( recipientResolver_ );
}

// -----------------------------------------------------------------------------
// Name: SupplyStockPushFlowRequestBuilder::serialize
// Created: LDC 2013-01-17
// -----------------------------------------------------------------------------
void SupplyStockPushFlowRequestBuilder::serialize( MIL_CheckPointOutArchive& archive, const unsigned int )
{
    archive << boost::serialization::base_object< SupplyStockManualRequestBuilder_ABC >( *this );
    std::string pushFlow = pushFlowParameters_.SerializeAsString();
    archive << pushFlow;
    archive << supplier_;
    archive << recipientResolver_;
}

//*****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyDotationManualRequestBuilder.h"
#include "AutomateFactory_ABC.h"
#include "SupplyRequestContainer_ABC.h"
#include "Entities/Specialisations/LOG/MIL_LogisticVisitors.h"
#include "Entities/Agents/Roles/Logistic/SupplyConvoyConfig.h"
#include "Entities/Agents/Roles/Logistic/SupplyResourceDotation.h"
#include "Entities/Agents/Roles/Logistic/SupplySupplier_ABC.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteTypePion.h"
#include "Entities/Agents/Units/Dotations/PHY_Dotation.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Automates/MIL_DotationSupplyManager.h"
#include "Network/NET_ASN_Tools.h"
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

using namespace logistic;

BOOST_CLASS_EXPORT_IMPLEMENT( logistic::SupplyDotationManualRequestBuilder )

// -----------------------------------------------------------------------------
// Name: SupplyDotationManualRequestBuilder constructor
// Created: JSR 2013-04-16
// -----------------------------------------------------------------------------
SupplyDotationManualRequestBuilder::SupplyDotationManualRequestBuilder()
    : supplier_          ( 0 )
    , recipientResolver_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyDotationManualRequestBuilder constructor
// Created: JSR 2013-04-16
// -----------------------------------------------------------------------------
SupplyDotationManualRequestBuilder::SupplyDotationManualRequestBuilder( const sword::PushFlowParameters& parameters, SupplySupplier_ABC& supplier, const AutomateFactory_ABC& recipientResolver )
    : pushFlowParameters_( parameters )
    , supplier_( &supplier )
    , recipientResolver_( &recipientResolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyDotationManualRequestBuilder destructor
// Created: JSR 2013-04-16
// -----------------------------------------------------------------------------
SupplyDotationManualRequestBuilder::~SupplyDotationManualRequestBuilder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyDotationManualRequestBuilder::Process
// Created: JSR 2013-04-16
// -----------------------------------------------------------------------------
void SupplyDotationManualRequestBuilder::Process( SupplyRequestContainer_ABC& container )
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
                container.SetPathToRecipient( recipient->GetDotationSupplyManager(), data.pathfind(), position, recipient->GetPosition() );

            position = recipient->GetPosition();
        }
    }

    if( pushFlowParameters_.has_waybackpathfind() )
        container.SetPathToTransportersProvider( pushFlowParameters_.waybackpathfind(), position, supplier_->GetPosition() );

    container.SetTransportersProvider( supplier_ );
    SetTransporters( pushFlowParameters_.transporters(), container );
    container.SetConvoyFactory( SupplyConvoyConfig::GetDotationSupplyConvoyFactory() );
}

// -----------------------------------------------------------------------------
// Name: SupplyDotationManualRequestBuilder::CreateRequest
// Created: JSR 2013-04-16
// -----------------------------------------------------------------------------
void SupplyDotationManualRequestBuilder::CreateRequest( MIL_Automate& recipient, const sword::SupplyFlowResource& resource, SupplyRequestContainer_ABC& container ) const
{
    const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( resource.resourcetype().id() );
    if( !pDotationCategory )
        return;

    double rTotalValue = resource.quantity();
    std::vector< SupplyDotationQuantity > pionDotations;
    recipient.Apply2( (std::function< void( const MIL_AgentPion&, PHY_Dotation& ) >)
        [&]( const MIL_AgentPion& pion, PHY_Dotation& dotation ) {
            if( &dotation.GetCategory() == pDotationCategory )
                pionDotations.push_back( SupplyDotationQuantity( &pion, &dotation ) );
    } );

    for( auto it = pionDotations.begin(); it != pionDotations.end() && rTotalValue > 0.; ++it )
    {
        const PHY_Dotation& dotation = *it->dotation_;
        const double rAffectedValue = std::min( rTotalValue, std::max( 0., dotation.GetCapacity() - dotation.GetValue() ) );
        rTotalValue -= rAffectedValue;
        it->quantity_ += rAffectedValue;
    }

    // Overhead
    if( rTotalValue > 0. && !pionDotations.empty() )
    {
        const double rAffectedValue = rTotalValue / pionDotations.size();
        for( auto it = pionDotations.begin(); it != pionDotations.end(); ++it )
            it->quantity_ += rAffectedValue;
    }

    BOOST_FOREACH( SupplyDotationQuantity& request, pionDotations )
        if( request.quantity_ > 0 )
            container.AddResource( recipient.GetDotationSupplyManager(), *request.pion_, boost::make_shared< SupplyResourceDotation >( *request.dotation_ ), request.quantity_ );
}

// -----------------------------------------------------------------------------
// Name: SupplyDotationManualRequestBuilder::SetTransporters
// Created: JSR 2013-04-16
// -----------------------------------------------------------------------------
void SupplyDotationManualRequestBuilder::SetTransporters( const google::protobuf::RepeatedPtrField< ::sword::SupplyFlowTransporter >& transporters, SupplyRequestContainer_ABC& container ) const
{
    if( transporters.size() == 0 )
        return;

    std::vector< std::pair< const PHY_ComposanteTypePion* , unsigned > > newTransporters;
    BOOST_FOREACH( const sword::SupplyFlowTransporter& transporter, transporters )
    {
        const PHY_ComposanteTypePion* type = PHY_ComposanteTypePion::Find( transporter.equipmenttype().id() );
        if( type )
            newTransporters.push_back( std::make_pair( type, transporter.quantity() ) );
    }
    container.SetTransporters( newTransporters );
}

// -----------------------------------------------------------------------------
// Name: SupplyDotationManualRequestBuilder::serialize
// Created: JSR 2013-04-16
// -----------------------------------------------------------------------------
void SupplyDotationManualRequestBuilder::serialize( MIL_CheckPointInArchive& archive, const unsigned int )
{
    archive >> boost::serialization::base_object< SupplyRequestBuilder_ABC >( *this );
    std::string pushFlow;
    archive >> pushFlow;
    const_cast< sword::PushFlowParameters& >( pushFlowParameters_ ).ParseFromString( pushFlow );
    archive >> supplier_;
    archive >> const_cast< AutomateFactory_ABC*& >( recipientResolver_ );
}

// -----------------------------------------------------------------------------
// Name: SupplyDotationManualRequestBuilder::serialize
// Created: JSR 2013-04-16
// -----------------------------------------------------------------------------
void SupplyDotationManualRequestBuilder::serialize( MIL_CheckPointOutArchive& archive, const unsigned int )
{
    archive << boost::serialization::base_object< SupplyRequestBuilder_ABC >( *this );
    std::string pushFlow = pushFlowParameters_.SerializeAsString();
    archive << pushFlow;
    archive << supplier_;
    archive << recipientResolver_;
}

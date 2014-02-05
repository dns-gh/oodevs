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
#include "Entities/Agents/Roles/Logistic/SupplyConvoyConfig.h"
#include "Entities/Agents/Roles/Logistic/SupplyResourceDotation.h"
#include "Entities/Agents/Roles/Logistic/SupplySupplier_ABC.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteTypePion.h"
#include "Entities/Agents/Units/Dotations/PHY_Dotation.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Automates/MIL_DotationSupplyManager.h"
#include "Network/NET_ASN_Tools.h"
#include <boost/foreach.hpp>

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
    BOOST_FOREACH( const sword::SupplyFlowRecipient& data, pushFlowParameters_.recipients() )
    {
        MIL_Automate* recipient = recipientResolver_->Find( data.receiver().id() );
        if( recipient )
        {
            BOOST_FOREACH( const sword::SupplyFlowResource& resource, data.resources() )
                CreateRequest( *recipient, resource, container );

            if( data.has_path() )
            {
                T_PointVector wayPoints;
                if( NET_ASN_Tools::ReadPointList( data.path(), wayPoints ) )
                    container.SetPathToRecipient( recipient->GetDotationSupplyManager(), wayPoints );
            }
        }
    }
    if( pushFlowParameters_.has_waybackpath() )
    {
        T_PointVector wayPoints;
        if( NET_ASN_Tools::ReadPointList( pushFlowParameters_.waybackpath(), wayPoints ) )
            container.SetPathToTransportersProvider( wayPoints );
    }
    container.SetTransportersProvider( supplier_ );
    SetTransporters( pushFlowParameters_.transporters(), container );
    container.SetConvoyFactory( SupplyConvoyConfig::GetDotationSupplyConvoyFactory() );
}

namespace
{
    struct SupplyDotationQuantity
    {
        SupplyDotationQuantity( const MIL_AgentPion* pion, PHY_Dotation* dotation )
            : pion_( pion )
            , dotation_( dotation )
            , quantity_( 0 )
        {}
        const MIL_AgentPion* pion_;
        PHY_Dotation* dotation_;
        double quantity_;
    };

    typedef std::vector< SupplyDotationQuantity > T_PionDotationVector;

    class DotationVisitor : private boost::noncopyable
    {
    public:
        DotationVisitor( T_PionDotationVector& pionDotations, const PHY_DotationCategory* category )
             : pionDotations_( pionDotations )
             , category_( category )
        {
            // NOTHING
        }

        ~DotationVisitor()
        {
            // NOTHING
        }

        void VisitDotation( const MIL_AgentPion& pion, PHY_Dotation& dotation ) const
        {
            if( &dotation.GetCategory() == category_ )
                pionDotations_.push_back( SupplyDotationQuantity( &pion, &dotation ) );
        }
    private:
        T_PionDotationVector& pionDotations_;
        const PHY_DotationCategory* category_;
    };
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

    T_PionDotationVector pionDotations;
    DotationVisitor visitor( pionDotations, pDotationCategory );
    recipient.Apply2( ( boost::function< void( const MIL_AgentPion&, PHY_Dotation& ) > )boost::bind( &DotationVisitor::VisitDotation, &visitor, _1, _2 ) );

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
            container.AddResource( recipient.GetDotationSupplyManager(), *request.pion_, boost::shared_ptr< SupplyResource_ABC >( new SupplyResourceDotation( *request.dotation_ ) ), request.quantity_ );
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

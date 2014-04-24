//*****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyConvoyRealFactory.h"
#include "SupplyDotationRequestBuilder.h"
#include "SupplyRecipient_ABC.h"
#include "SupplyRequestBuilder_ABC.h"
#include "SupplyRequestContainer_ABC.h"
#include "SupplyResourceDotation.h"
#include "SupplyConvoyConfig.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Dotations/PHY_Dotation.h"
#include "Entities/Automates/MIL_Automate.h"

using namespace logistic;

BOOST_CLASS_EXPORT_IMPLEMENT( logistic::SupplyDotationRequestBuilder )

// -----------------------------------------------------------------------------
// Name: SupplyDotationRequestBuilder::SupplyDotationRequestBuilder
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplyDotationRequestBuilder::SupplyDotationRequestBuilder( MIL_Automate& automate, SupplyRecipient_ABC& recipient )
    : automate_ ( &automate )
    , recipient_( &recipient )
{
}

// -----------------------------------------------------------------------------
// Name: SupplyDotationRequestBuilder constructor
// Created: LDC 2013-01-17
// -----------------------------------------------------------------------------
SupplyDotationRequestBuilder::SupplyDotationRequestBuilder()
    : automate_ ( 0 )
    , recipient_( 0 )
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyDotationRequestBuilder::SupplyDotationRequestBuilder
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplyDotationRequestBuilder::~SupplyDotationRequestBuilder()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyDotationRequestBuilder::CreateRequest
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyDotationRequestBuilder::Process( SupplyRequestContainer_ABC& container )
{
    automate_->Apply2( (boost::function< void( const MIL_AgentPion&, PHY_Dotation& ) >)boost::bind( &SupplyDotationRequestBuilder::VisitDotation, this, _1, _2, boost::ref( container ) ) );
    container.SetConvoyFactory( SupplyConvoyConfig::GetDotationSupplyConvoyFactory() );
}

// -----------------------------------------------------------------------------
// Name: SupplyDotationRequestBuilder::VisitDotation
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void SupplyDotationRequestBuilder::VisitDotation( const MIL_AgentPion& pion, PHY_Dotation& dotation, SupplyRequestContainer_ABC& container ) const
{
    if( dotation.NeedSupply() )
        if( !pion.Retrieve< PHY_RoleInterface_Supply >() || !pion.Retrieve< PHY_RoleInterface_Supply >()->IsConvoy() )
        {
            double quantityToRequest = std::max( 0., dotation.GetCapacity() - dotation.GetValue() ); //$$ devrait être le retour de NeedSupply() ?
            container.AddResource( *recipient_, pion, boost::shared_ptr< SupplyResource_ABC >( new SupplyResourceDotation( dotation ) ), quantityToRequest );
        }
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void SupplyDotationRequestBuilder::serialize
// Created: LDC 2013-01-17
// -----------------------------------------------------------------------------
template< typename Archive > void SupplyDotationRequestBuilder::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< SupplyRequestBuilder_ABC >( *this );
    archive & automate_;
    archive & recipient_;
}

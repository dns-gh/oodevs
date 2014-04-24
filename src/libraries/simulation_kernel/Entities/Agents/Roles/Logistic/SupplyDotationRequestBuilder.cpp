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
#include <boost/make_shared.hpp>

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
    // NOTHING
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyDotationRequestBuilder::CreateRequest
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyDotationRequestBuilder::Process( SupplyRequestContainer_ABC& container )
{
    const boost::function< void( const MIL_AgentPion&, PHY_Dotation& ) > f =
        [&]( const MIL_AgentPion& pion, PHY_Dotation& dotation )
        {
            if( !dotation.NeedSupply() )
                return;
            const auto supply = pion.Retrieve< PHY_RoleInterface_Supply >();
            if( supply && supply->IsConvoy() )
                return;
            const double quantity = std::max( 0., dotation.GetCapacity() - dotation.GetValue() ); //$$ devrait être le retour de NeedSupply() ?
            container.AddResource( *recipient_, pion, boost::make_shared< SupplyResourceDotation >( dotation ), quantity );
        };
    automate_->Apply2( f );
    container.SetConvoyFactory( SupplyConvoyConfig::GetDotationSupplyConvoyFactory() );
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

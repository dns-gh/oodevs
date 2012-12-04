//*****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyDotationRequestBuilder.h"
#include "SupplyRequestContainer_ABC.h"
#include "SupplyResourceDotation.h"
#include "SupplyConvoyRealFactory.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Agents/Units/Dotations/PHY_Dotation.h"

using namespace logistic;

// -----------------------------------------------------------------------------
// Name: SupplyDotationRequestBuilder::SupplyDotationRequestBuilder
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplyDotationRequestBuilder::SupplyDotationRequestBuilder( MIL_Automate& automate, SupplyRecipient_ABC& recipient )
    : automate_ ( automate )
    , recipient_( recipient )
{
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
    automate_.Apply2( (boost::function< void( const MIL_AgentPion&, PHY_Dotation& ) >)boost::bind( &SupplyDotationRequestBuilder::VisitDotation, this, _1, _2, boost::ref( container ) ) );
    //container.SetConvoyFactory( SupplyConvoyRealFactory::Instance() );
}

// -----------------------------------------------------------------------------
// Name: SupplyDotationRequestBuilder::VisitDotation
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void SupplyDotationRequestBuilder::VisitDotation( const MIL_AgentPion& pion, PHY_Dotation& dotation, SupplyRequestContainer_ABC& container ) const
{
    if( dotation.NeedSupply() )
    {
        double quantityToRequest = std::max( 0., dotation.GetCapacity() - dotation.GetValue() ); //$$ devrait �tre le retour de NeedSupply() ?
        container.AddResource( recipient_, pion, boost::shared_ptr< SupplyResource_ABC >( new SupplyResourceDotation( dotation ) ), quantityToRequest );
    }
}

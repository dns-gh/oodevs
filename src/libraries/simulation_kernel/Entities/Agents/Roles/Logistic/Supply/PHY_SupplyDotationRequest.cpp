// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyDotationRequest.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 13:39 $
// $Revision: 4 $
// $Workfile: PHY_SupplyDotationRequest.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_SupplyDotationRequest.h"

BOOST_CLASS_EXPORT_GUID( PHY_SupplyDotationRequest, "PHY_SupplyDotationRequest" )

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationRequest constructor
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
PHY_SupplyDotationRequest::PHY_SupplyDotationRequest()
    : PHY_SupplyRequest_ABC< PHY_Dotation >()
{

}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationRequest destructor
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
PHY_SupplyDotationRequest::~PHY_SupplyDotationRequest()
{

}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationRequest::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_SupplyDotationRequest::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_SupplyRequest_ABC< PHY_Dotation > >( *this );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationRequest::AddDotation
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_SupplyDotationRequest::AddDotation( PHY_Dotation& dotation )
{
    sIndividualRequest request;
    request.pRequest_       = &dotation;
    request.rRequestedValue_ = dotation.GetCapacity() - dotation.GetValue();

    assert( request.rRequestedValue_ > 0. );
    rTotalRequestedValue_ += request.rRequestedValue_;   

    requests_.push_back( request );
}

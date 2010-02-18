// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyStockRequest.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 13:44 $
// $Revision: 4 $
// $Workfile: PHY_SupplyStockRequest.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_SupplyStockRequest.h"

BOOST_CLASS_EXPORT_GUID( PHY_SupplyStockRequest, "PHY_SupplyStockRequest" )

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest constructor
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
PHY_SupplyStockRequest::PHY_SupplyStockRequest()
    : PHY_SupplyRequest_ABC< PHY_DotationStock >()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest destructor
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
PHY_SupplyStockRequest::~PHY_SupplyStockRequest()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest::AddStock
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_SupplyStockRequest::AddStock( PHY_DotationStock& stock )
{
    sIndividualRequest request;
    request.pRequest_        = &stock;
    request.rRequestedValue_ = std::max( 0., stock.GetCapacity() - stock.GetValue() );
    if( request.rRequestedValue_ > 0 )
    {
        rTotalRequestedValue_ += request.rRequestedValue_;   
        requests_.push_back( request );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest::AddStock
// Created: NLD 2005-12-15
// -----------------------------------------------------------------------------
void PHY_SupplyStockRequest::AddStock( PHY_DotationStock& stock, MT_Float rValue )
{
    assert( rValue >= 0. );
    sIndividualRequest request;
    request.pRequest_        = &stock;
    request.rRequestedValue_ = rValue;
    if( request.rRequestedValue_ > 0 )
    {
        rTotalRequestedValue_ += request.rRequestedValue_;   
        requests_.push_back( request );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest::ApplyQuota
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
void PHY_SupplyStockRequest::ApplyQuota( MT_Float rQuota )
{
    const MT_Float rNewTotalRequestedValue_ = std::min( rTotalRequestedValue_, rQuota );
    const MT_Float rTmp                     = rNewTotalRequestedValue_ / rTotalRequestedValue_;

    for( IT_RequestVector itRequest = requests_.begin(); itRequest != requests_.end(); ++itRequest )
    {
        sIndividualRequest& request = *itRequest;
        request.rRequestedValue_ *= rTmp;
    }        
    rTotalRequestedValue_ = rNewTotalRequestedValue_;
}

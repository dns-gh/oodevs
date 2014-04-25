//*****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyRequestHierarchyDispatcher.h"
#include "SupplyRecipient_ABC.h"
#include "SupplyRequest_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include "Entities/Specialisations/LOG/LogisticLink_ABC.h"
#include <tools/Iterator.h>

using namespace logistic;

// -----------------------------------------------------------------------------
// Name: SupplyDotationRequestBuilder constructor
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplyRequestHierarchyDispatcher::SupplyRequestHierarchyDispatcher( const LogisticHierarchy_ABC& logisticHierarchy, bool forceSupply )
    : logisticHierarchy_               ( logisticHierarchy )
    , forceSupply_                     ( forceSupply )
    , nbMandatoryRequestsSatisfied_    ( 0 )
    , nbComplementaryRequests_         ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyDotationRequestBuilder destructor
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplyRequestHierarchyDispatcher::~SupplyRequestHierarchyDispatcher()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestHierarchyDispatcher::Dispatch
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void SupplyRequestHierarchyDispatcher::Dispatch( SupplyRecipient_ABC& recipient, SupplyRequest_ABC& request )
{
    // Filter out too small requests (0.xyz as quantity)
    if( request.GetRequestedQuantity() < 1 )
        return;
    auto it = logisticHierarchy_.CreateSuperiorLinksIterator();
    if( ! it.HasMoreElements() )
    {
        request.NotifySuperiorNotAvailable( recipient );
        return;
    }
    if( request.IsComplementary() )
        ++nbComplementaryRequests_;
    while( it.HasMoreElements() )
    {
        if( request.AffectSupplier( recipient, it.NextElement() ) )
        {
            if( ! request.IsComplementary() )
                ++nbMandatoryRequestsSatisfied_;
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestHierarchyDispatcher::AllowSupply
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
bool SupplyRequestHierarchyDispatcher::AllowSupply()
{
    return nbMandatoryRequestsSatisfied_ > 0 || ( forceSupply_ && nbComplementaryRequests_ > 0 );
}

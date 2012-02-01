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
#include "SupplyRequest_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include "Entities/Specialisations/LOG/LogisticLink_ABC.h"
#include "tools/Iterator.h"

using namespace logistic;

// -----------------------------------------------------------------------------
// Name: SupplyDotationRequestBuilder constructor
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplyRequestHierarchyDispatcher::SupplyRequestHierarchyDispatcher( const LogisticHierarchy_ABC& logisticHierarchy )
    : logisticHierarchy_                ( logisticHierarchy )
    , nbMandatoryRequests_              ( 0 )
    , nbMandatoryRequestsSatisfied_     ( 0 )
    , nbComplementaryRequests_          ( 0 )
    , nbComplementaryRequestsSatisfied_ ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: SupplyDotationRequestBuilder destructor
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplyRequestHierarchyDispatcher::~SupplyRequestHierarchyDispatcher()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyRequestHierarchyDispatcher::Dispatch
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void SupplyRequestHierarchyDispatcher::Dispatch( SupplyRequest_ABC& request )
{
    tools::Iterator< boost::shared_ptr< LogisticLink_ABC > > it = logisticHierarchy_.CreateSuperiorLinksIterator();
    while( it.HasMoreElements() )
    {
        if( request.IsComplementary() )
            ++ nbComplementaryRequests_;
        else
            ++ nbMandatoryRequests_;

        if( request.AffectSupplier( it.NextElement() ) )
        {
            if( request.IsComplementary() )
                ++ nbComplementaryRequestsSatisfied_;
            else
                ++ nbMandatoryRequestsSatisfied_;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestHierarchyDispatcher::AllowSupply
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
bool SupplyRequestHierarchyDispatcher::AllowSupply()
{
    return nbMandatoryRequestsSatisfied_ > 0;// || ( nbComplementaryRequests_ > 0 && forceSupply_;
}

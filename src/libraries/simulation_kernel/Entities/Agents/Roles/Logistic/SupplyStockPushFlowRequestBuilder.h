// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyStockPushFlowRequestBuilder_h_
#define __SupplyStockPushFlowRequestBuilder_h_

#include "SupplyStockManualRequestBuilder_ABC.h"
#include "protocol/ClientSenders.h"
#include "tools/Resolver.h"

class MIL_Automate;
class PHY_DotationStock;

namespace logistic {
    class SupplySupplier_ABC;

// =============================================================================
/** @class  SupplyStockPushFlowRequestBuilder
    @brief  SupplyStockPushFlowRequestBuilder
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyStockPushFlowRequestBuilder : public SupplyStockManualRequestBuilder_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyStockPushFlowRequestBuilder( const sword::PushFlowParameters& parameters, SupplySupplier_ABC& supplier, const tools::Resolver_ABC< MIL_Automate >& recipientResolver );
    virtual ~SupplyStockPushFlowRequestBuilder();
    //@}

    //! @name Operations
    //@{
    virtual void Process( SupplyRequestContainer_ABC& container );
    //@}

private:
    const sword::PushFlowParameters pushFlowParameters_;
    SupplySupplier_ABC& supplier_;
    const tools::Resolver_ABC< MIL_Automate >& recipientResolver_;
};

} // end namespace logistic

#endif // __SupplyStockPushFlowRequestBuilder_h_

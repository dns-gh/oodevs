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
#include <boost/serialization/export.hpp>
#include <boost/serialization/export.hpp>

class AutomateFactory_ABC;
class MIL_Automate;
class PHY_DotationStock;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

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
             SupplyStockPushFlowRequestBuilder();
             SupplyStockPushFlowRequestBuilder( const sword::PushFlowParameters& parameters, SupplySupplier_ABC& supplier, const AutomateFactory_ABC& recipientResolver );
    virtual ~SupplyStockPushFlowRequestBuilder();
    //@}

    //! @name Operations
    //@{
    virtual void Process( SupplyRequestContainer_ABC& container );
    void serialize( MIL_CheckPointInArchive&, const unsigned int );
    void serialize( MIL_CheckPointOutArchive&, const unsigned int );
    //@}

private:
    const sword::PushFlowParameters pushFlowParameters_;
    SupplySupplier_ABC* supplier_;
    const AutomateFactory_ABC* recipientResolver_;
};

} // end namespace logistic

BOOST_CLASS_EXPORT_KEY( logistic::SupplyStockPushFlowRequestBuilder )

#endif // __SupplyStockPushFlowRequestBuilder_h_

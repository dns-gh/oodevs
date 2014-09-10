// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyStockPullFlowRequestBuilder_h_
#define __SupplyStockPullFlowRequestBuilder_h_

#include "SupplyStockManualRequestBuilder_ABC.h"
#include "protocol/ClientSenders.h"
#include <boost/serialization/export.hpp>

class MIL_Automate;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

namespace logistic {
    class SupplySupplier_ABC;

// =============================================================================
/** @class  SupplyStockPullFlowRequestBuilder
    @brief  SupplyStockPullFlowRequestBuilder
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyStockPullFlowRequestBuilder : public SupplyStockManualRequestBuilder_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyStockPullFlowRequestBuilder();
             SupplyStockPullFlowRequestBuilder( const sword::PullFlowParameters& parameters, MIL_Automate& recipient, SupplySupplier_ABC& supplier );
    virtual ~SupplyStockPullFlowRequestBuilder();
    //@}

    //! @name Operations
    //@{
    virtual void Process( SupplyRequestContainer_ABC& container );
    void serialize( MIL_CheckPointInArchive&, const unsigned int );
    void serialize( MIL_CheckPointOutArchive&, const unsigned int );
    //@}

private:
    const sword::PullFlowParameters pullFlowParameters_;
    MIL_Automate* recipient_;
    SupplySupplier_ABC* supplier_;
};

} // end namespace logistic

BOOST_CLASS_EXPORT_KEY( logistic::SupplyStockPullFlowRequestBuilder )

#endif // __SupplyStockPullFlowRequestBuilder_h_

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyRequestManualDispatcher_h_
#define __SupplyRequestManualDispatcher_h_

#include "SupplyRequestDispatcher_ABC.h"

namespace logistic {
   class SupplySupplier_ABC;

// =============================================================================
/** @class  SupplyRequestManualDispatcher
    @brief  SupplyRequestManualDispatcher
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyRequestManualDispatcher : public SupplyRequestDispatcher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyRequestManualDispatcher( SupplySupplier_ABC& supplier );
    virtual ~SupplyRequestManualDispatcher();
    //@}

    //! @name Operations
    //@{
    virtual void Dispatch   ( SupplyRequest_ABC& request );
    virtual bool AllowSupply();
    //@}

private:
    SupplySupplier_ABC& supplier_;
    unsigned nbRequests_;
    unsigned nbRequestsSatisfied_;
};

} // end namespace logistic

#endif // __SupplyRequestManualDispatcher_h_

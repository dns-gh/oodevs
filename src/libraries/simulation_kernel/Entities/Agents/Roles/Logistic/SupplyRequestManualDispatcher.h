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

namespace logistic
{
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
    explicit SupplyRequestManualDispatcher( SupplySupplier_ABC& supplier );
    virtual ~SupplyRequestManualDispatcher();
    //@}

    //! @name Operations
    //@{
    virtual void Dispatch( SupplyRecipient_ABC& recipient, SupplyRequest_ABC& request );
    virtual bool AllowSupply();
    //@}

private:
    //! @name Member data
    //@{
    SupplySupplier_ABC& supplier_;
    bool supply_;
    //@}
};

}

#endif // __SupplyRequestManualDispatcher_h_

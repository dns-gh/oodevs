// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyRequestHierarchyDispatcher_h_
#define __SupplyRequestHierarchyDispatcher_h_

#include "SupplyRequestDispatcher_ABC.h"

namespace logistic
{
   class LogisticHierarchy_ABC;

// =============================================================================
/** @class  SupplyRequestHierarchyDispatcher
    @brief  SupplyRequestHierarchyDispatcher
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyRequestHierarchyDispatcher : public SupplyRequestDispatcher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SupplyRequestHierarchyDispatcher( const LogisticHierarchy_ABC& logisticHierarchy, bool forceSupply = false );
    virtual ~SupplyRequestHierarchyDispatcher();
    //@}

    //! @name Operations
    //@{
    virtual void Dispatch( SupplyRecipient_ABC& recipient, SupplyRequest_ABC& request );
    virtual bool AllowSupply();
    //@}

private:
    //! @name Member data
    //@{
    const LogisticHierarchy_ABC& logisticHierarchy_;
    const bool forceSupply_;
    unsigned nbMandatoryRequestsSatisfied_;
    unsigned nbComplementaryRequests_;
    //@}
};

}

#endif // __SupplyRequestHierarchyDispatcher_h_

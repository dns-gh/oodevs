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

namespace logistic {
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
             SupplyRequestHierarchyDispatcher( const LogisticHierarchy_ABC& logisticHierarchy );
    virtual ~SupplyRequestHierarchyDispatcher();
    //@}

    //! @name Operations
    //@{
    virtual void Dispatch   ( SupplyRequest_ABC& request );
    virtual bool AllowSupply();
    //@}

private:
    const LogisticHierarchy_ABC& logisticHierarchy_;
    //const bool forceSupply_;
    unsigned nbMandatoryRequests_;
    unsigned nbMandatoryRequestsSatisfied_;
    unsigned nbComplementaryRequests_;
    unsigned nbComplementaryRequestsSatisfied_;
};

} // end namespace logistic

#endif // __SupplyRequestHierarchyDispatcher_h_

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyRecipientResourcesRequest_h_
#define __SupplyRecipientResourcesRequest_h_

#include "tools/Resolver.h"

class SupplyResourceRequest;

namespace kernel
{
    class Automat_ABC;
    class DotationType;
}

namespace sword
{
    class SupplyRecipientResourcesRequest;
}

// =============================================================================
/** @class  DotationRequest
    @brief  DotationRequest
*/
// Created: AGE 2006-02-28
// =============================================================================
class SupplyRecipientResourcesRequest : public tools::Resolver< SupplyResourceRequest >
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyRecipientResourcesRequest( const tools::Resolver_ABC< kernel::DotationType >& dotationResolver, const tools::Resolver_ABC< kernel::Automat_ABC >& resolver, const sword::SupplyRecipientResourcesRequest& msg );
    virtual ~SupplyRecipientResourcesRequest();
    //@}

    //! @name Accessors
    //@{
    void Update( const sword::SupplyRecipientResourcesRequest& msg );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Automat_ABC& recipient_;
    const tools::Resolver_ABC< kernel::DotationType >& dotationResolver_;
    //@}
};

#endif // __SupplyRecipientResourcesRequest_h_

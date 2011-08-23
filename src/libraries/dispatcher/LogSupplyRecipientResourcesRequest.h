 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogSupplyRecipientResourcesRequest_h_
#define __LogSupplyRecipientResourcesRequest_h_

#include "tools/Resolver.h"
#include <boost/noncopyable.hpp>

namespace sword
{
    class SupplyRecipientResourcesRequest;
}

namespace kernel
{
    class Automat_ABC;
}

namespace dispatcher
{
    class Model;
    class LogSupplyResourceRequest;

// =============================================================================
/** @class  LogSupplyRecipientResourcesRequest
    @brief  LogSupplyRecipientResourcesRequest
*/
// Created: NLD 2006-09-19
// =============================================================================
class LogSupplyRecipientResourcesRequest : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             LogSupplyRecipientResourcesRequest( const Model& model, const sword::SupplyRecipientResourcesRequest& msg );
    virtual ~LogSupplyRecipientResourcesRequest();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::SupplyRecipientResourcesRequest& msg );
    void Send  ( sword::SupplyRecipientResourcesRequest& msg ) const ;
    //@}

private:
    const Model&               model_;
    const kernel::Automat_ABC& automat_;
    tools::Resolver< LogSupplyResourceRequest > resources_;
};

}

#endif // __LogSupplyDotation_h_

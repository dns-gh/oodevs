// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Services_h_
#define __Services_h_

#include "clients_kernel/Services.h"

namespace dispatcher
{
    class ClientPublisher_ABC;

// =============================================================================
/** @class  Services
    @brief  Services
*/
// Created: AGE 2008-08-13
// =============================================================================
class Services : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Services();
    virtual ~Services();
    //@}

    //! @name Operations
    //@{
    void Send( ClientPublisher_ABC& publisher ) const;
    void Declare( sword::Service id );
    bool HasService( sword::Service id ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Services services_;
    //@}
};

}

#endif // __Services_h_

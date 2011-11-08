// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NullClientPublisher_h_
#define __NullClientPublisher_h_

#include "protocol/ClientPublisher_ABC.h"

namespace dispatcher
{
// =============================================================================
/** @class  NullClientPublisher
    @brief  NullClientPublisher
*/
// Created: AGE 2007-08-24
// =============================================================================
class NullClientPublisher : public ClientPublisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NullClientPublisher() {}
    virtual ~NullClientPublisher() {}
    //@}

    //! @name Operations
    //@{
    virtual void Send( const sword::SimToClient& ) {}
    virtual void Send( const sword::AuthenticationToClient& ) {}
    virtual void Send( const sword::ReplayToClient& ) {}
    virtual void Send( const sword::AarToClient& ) {}
    virtual void Send( const sword::MessengerToClient& ) {}
    virtual void Send( const sword::DispatcherToClient& ) {}
    //@}

    //! @name Accesors
    //@{
    virtual std::string GetEndpoint() const { return ""; }
    //@}
};

}

#endif // __NullClientPublisher_h_

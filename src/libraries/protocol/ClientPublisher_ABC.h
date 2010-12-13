// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ClientPublisher_ABC_h_
#define __ClientPublisher_ABC_h_

#include <string>

namespace sword
{
    class AarToClient;
    class SimToClient;
    class AuthenticationToClient;
    class ReplayToClient;
    class MessengerToClient;
    class DispatcherToClient;
    class ClientToAar;
    class ClientToSim;
    class ClientToReplay;
    class ClientToAuthentication;
}

namespace dispatcher
{
// =============================================================================
/** @class  ClientPublisher_ABC
    @brief  ClientPublisher_ABC
*/
// Created: AGE 2007-08-24
// =============================================================================
class ClientPublisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ClientPublisher_ABC() {}
    virtual ~ClientPublisher_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Send( const sword::SimToClient&                      message ) = 0;
    virtual void Send( const sword::AuthenticationToClient& message ) = 0;
    virtual void Send( const sword::ReplayToClient&                message ) = 0;
    virtual void Send( const sword::AarToClient&                       message ) = 0;
    virtual void Send( const sword::MessengerToClient&          message ) = 0;
    virtual void Send( const sword::DispatcherToClient&        message ) = 0;
    //@}

    //! @name Accessors
    //@{
    virtual std::string GetEndpoint() const = 0;
    //@}
};

}

#endif // __ClientPublisher_ABC_h_

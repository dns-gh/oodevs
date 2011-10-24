// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __SwordMessageHandler_ABC_h_
#define __SwordMessageHandler_ABC_h_

namespace sword
{
    class AuthenticationToClient;
    class DispatcherToClient;
    class MessengerToClient;
    class SimToClient;
}

// =============================================================================
/** @class  SwordMessageHandler_ABC
    @brief  SwordMessageHandler_ABC
*/
// Created: SEB 2010-10-12
// =============================================================================
class SwordMessageHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SwordMessageHandler_ABC() {}
    virtual ~SwordMessageHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void OnReceiveMessage( const sword::SimToClient& message ) = 0;
    virtual void OnReceiveMessage( const sword::MessengerToClient& message ) = 0;
    virtual void OnReceiveMessage( const sword::AuthenticationToClient& message ) = 0;
    virtual void OnReceiveMessage( const sword::DispatcherToClient& message ) = 0;
    //@}
};

#endif // __SwordMessageHandler_ABC_h_

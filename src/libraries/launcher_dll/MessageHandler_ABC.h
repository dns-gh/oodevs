// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef launcher_MessageHandler_ABC_h
#define launcher_MessageHandler_ABC_h

#include <boost/noncopyable.hpp>

namespace sword
{
    class SimToClient;
    class MessengerToClient;
    class AuthenticationToClient;
    class DispatcherToClient;
}

namespace launcher
{
// =============================================================================
/** @class  MessageHandler_ABC
    @brief  Message handler declaration
*/
// Created: LGY 2011-05-18
// =============================================================================
class MessageHandler_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MessageHandler_ABC() {}
    virtual ~MessageHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool OnReceiveMessage( const sword::SimToClient& message ) = 0;
    virtual bool OnReceiveMessage( const sword::MessengerToClient& message ) = 0;
    virtual bool OnReceiveMessage( const sword::AuthenticationToClient& message ) = 0;
    virtual bool OnReceiveMessage( const sword::DispatcherToClient& message ) = 0;
    // TODO gérer le nouveau message unhandled DIspatcherToClient
    //@}
};

}

#endif // launcher_MessageHandler_ABC_h

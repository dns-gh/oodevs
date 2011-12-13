// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SocketEventCallback_ABC_h_
#define __SocketEventCallback_ABC_h_

#include <string>

namespace tools
{
    class Message;

// =============================================================================
/** @class  SocketEventCallback_ABC
    @brief  Socket event callback declaration
*/
// Created: AGE 2007-09-06
// =============================================================================
class SocketEventCallback_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SocketEventCallback_ABC() {}
    virtual ~SocketEventCallback_ABC() {}
    //@}

    //! @name Operations
    //@{
    //$$$$ A RENOMMER EN QQCHOSE DE COHERENT
    virtual void ConnectionSucceeded( const std::string& endpoint ) = 0;
    virtual void ConnectionFailed   ( const std::string& address, const std::string& error ) = 0;
    virtual void ConnectionError    ( const std::string& endpoint, const std::string& error ) = 0;
    virtual void ConnectionWarning  ( const std::string& endpoint, const std::string& warning ) = 0;
    virtual void OnMessage( const std::string& endpoint, Message& message ) = 0;
    //@}

};

}

#endif // __SocketEventCallback_ABC_h_

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SocketManager_h_
#define __SocketManager_h_

#include "MessageSender_ABC.h"
#include <map>

namespace tools
{
    class MessageCallback_ABC;
    class ConnectionCallback_ABC;
    class Socket;
    class Message;

// =============================================================================
/** @class  SocketManager
    @brief  SocketManager
*/
// Created: AGE 2007-09-05
// =============================================================================
class SocketManager : public MessageSender_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SocketManager( MessageCallback_ABC& message, ConnectionCallback_ABC& connection );
    virtual ~SocketManager();
    //@}

    //! @name Operations
    //@{
    void Add( const boost::shared_ptr< boost::asio::ip::tcp::socket >& socket );
    virtual void Send( const std::string& endpoint, unsigned long tag, const Message& message );

    void Disconnect();
    void Disconnect( const std::string& endpoint );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SocketManager( const SocketManager& );            //!< Copy constructor
    SocketManager& operator=( const SocketManager& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, boost::shared_ptr< Socket > >   T_Sockets;
    typedef T_Sockets::iterator                                   IT_Sockets;
    typedef T_Sockets::const_iterator                            CIT_Sockets;
    //@}

private:
    //! @name Member data
    //@{
    MessageCallback_ABC& message_;
    ConnectionCallback_ABC& connection_;
    T_Sockets sockets_;
    //@}
};

}

#endif // __SocketManager_h_
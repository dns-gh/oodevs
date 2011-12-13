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
#include "asio.h"
#include <map>

namespace tools
{
    class SocketEventCallback_ABC;
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
             SocketManager( boost::shared_ptr< SocketEventCallback_ABC > callback, DWORD timeOut );
    virtual ~SocketManager();
    //@}

    //! @name Operations
    //@{
    void Add( const boost::shared_ptr< boost::asio::ip::tcp::socket >& socket );
    virtual void Send( const std::string& endpoint, unsigned long tag, Message& message );

    void Disconnect();
    void Disconnect( const std::string& endpoint );
    //@}

    //! @name Statistics
    //@{
    virtual unsigned long GetNbMessagesSent() const { return nbMessagesSent_; }
    virtual bool HasAnsweredSinceLastTick( const std::string& endpoint );
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
    boost::shared_ptr< SocketEventCallback_ABC > callback_;
    const DWORD timeOut_;
    T_Sockets sockets_;
    unsigned long nbMessagesSent_;
    //@}
};

}

#endif // __SocketManager_h_
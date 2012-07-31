// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Socket_h_
#define __Socket_h_

#include "asio.h"
#include "Message.h"
#include <deque>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/mutex.hpp>

namespace tools
{
    class SocketEventCallback_ABC;
    class Message;

// =============================================================================
/** @class  Socket
    @brief  Socket
*/
// Created: AGE 2007-09-05
// =============================================================================
class Socket : public boost::enable_shared_from_this< Socket >
{
public:
    //! @name Constructors/Destructor
    //@{
             Socket( boost::shared_ptr< boost::asio::ip::tcp::socket > socket,
                     boost::shared_ptr< SocketEventCallback_ABC > callback, const std::string& endpoint, int queueMaxSize = 200000 );
    virtual ~Socket();
    //@}

    //! @name Operations
    //@{
    int Send( unsigned long tag, Message& message );
    void StartReading();
    void Close();
    const std::string& Endpoint() const;

    bool HasAnsweredSinceLastTick();
    bool IsQueueFlooded();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Socket( const Socket& );            //!< Copy constructor
    Socket& operator=( const Socket& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Sent      ( const Message& message, const boost::system::error_code& error );
    void HeaderRead( Message& message, const boost::system::error_code& error );
    void Read      ( Message& message, const boost::system::error_code& error );
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< boost::asio::ip::tcp::socket > socket_;
    boost::shared_ptr< SocketEventCallback_ABC > callback_;
    std::string endpoint_;
    boost::system::error_code previous_;
    std::deque< std::pair< unsigned long, Message > > queue_;
    boost::mutex mutex_;
    int queueMaxSize_;
    bool needCleanup_;
    bool answered_;
    //@}
};

}

#endif // __Socket_h_

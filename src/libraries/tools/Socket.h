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
#include <boost/enable_shared_from_this.hpp>

namespace tools
{
    class MessageCallback_ABC;
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
             Socket( const boost::shared_ptr< boost::asio::ip::tcp::socket >& socket, const std::string& endpoint, MessageCallback_ABC& message );
    virtual ~Socket();
    //@}

    //! @name Operations
    //@{
    void Send( unsigned long tag, const Message& message );
    void StartReading();
    void Close();
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
    std::string endpoint_;
    MessageCallback_ABC& message_;
    boost::system::error_code previous_;
    //@}
};

}

#endif // __Socket_h_

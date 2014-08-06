// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UdpNetwork_h_
#define __UdpNetwork_h_

#include "Serializer.h"
#include "tools/asio.h"
#include <tools/WaitEvent.h>
#pragma warning( push, 0 )
#include <boost/thread.hpp>
#pragma warning( pop )
#include <boost/bind.hpp>

namespace dispatcher
{
    class Logger_ABC;
}

namespace plugins
{
namespace dis
{
    class Message;

// =============================================================================
/** @class  UdpNetwork
    @brief  Udp network implementation
*/
// Created: ZEBRE 2007-05-10
// =============================================================================
class UdpNetwork
{
public:
    //! @name Constructors/Destructor
    //@{
             UdpNetwork( const std::string& target, unsigned short port, dispatcher::Logger_ABC& logger );
    virtual ~UdpNetwork();
    //@}

    //! @name Operations
    //@{
    template< typename Message >
    void Send( const Message& message )
    {
        char buffer[ Message::size ];
        Serializer serializer( buffer );
        message.Serialize( serializer );
        boost::shared_ptr< std::string > messageString( new std::string( buffer, Message::size ) );
        socket_.async_send_to( boost::asio::buffer( *messageString ), target_,
                           boost::bind( &UdpNetwork::Sent, this, messageString, boost::asio::placeholders::error ) );
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UdpNetwork( const UdpNetwork& );            //!< Copy constructor
    UdpNetwork& operator=( const UdpNetwork& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Start();
    void Sent( boost::shared_ptr< std::string >, const boost::system::error_code& );
    //@}

protected:
    //! @name Member data
    //@{
    dispatcher::Logger_ABC& logger_;
    boost::asio::ip::udp::endpoint target_;
    boost::asio::io_service service_;
    boost::asio::ip::udp::socket socket_;
    tools::WaitEvent quit_;
    boost::thread thread_;
    //@}
};

}
}

#endif // __UdpNetwork_h_

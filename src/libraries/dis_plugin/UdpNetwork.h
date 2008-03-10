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
#include <boost/thread.hpp>
#include <boost/bind.hpp>

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
             UdpNetwork( const std::string& target, unsigned short port );
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
        socket_.async_send_to( boost::asio::buffer( (const char*)buffer, Message::size ), target_,
                           boost::bind( &UdpNetwork::Sent, this, boost::asio::placeholders::error ) );
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
    void Stop();
    void Sent( const boost::system::error_code& ) {};
    //@}

protected:
    //! @name Member data
    //@{
    boost::asio::ip::udp::endpoint target_;
    boost::asio::io_service service_;
    boost::asio::ip::udp::socket socket_;

    boost::thread thread_;
    //@}
};

}

#endif // __UdpNetwork_h_

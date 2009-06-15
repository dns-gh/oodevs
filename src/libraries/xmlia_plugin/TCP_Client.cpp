// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "TCP_Client.h"

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: TCP_Client constructor
// Created: SBO 2008-04-02
// -----------------------------------------------------------------------------
TCP_Client::TCP_Client( boost::asio::io_service& io_service, const std::string parameter )
: resolver_(io_service),
  socket_(io_service),
  parameter_ (parameter)
{
    // NOTHING


// Start an asynchronous resolve to translate the server and service names
// into a list of endpoints.
    tcp::resolver::query query("http://sbo-02", "8080");
    resolver_.async_resolve(query,
                            boost::bind(&TCP_Client::handle_resolve, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::iterator));
}


// -----------------------------------------------------------------------------
// Name: handle_resolve 
// Created: SLG 2009-06-02
// -----------------------------------------------------------------------------
void TCP_Client::handle_resolve( const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator)
{
    if (!err)
    {
        // Attempt a connection to the first endpoint in the list. Each endpoint
        // will be tried until we successfully establish a connection.
        tcp::endpoint endpoint = *endpoint_iterator;
        socket_.async_connect(endpoint,
            boost::bind(&TCP_Client::handle_connect, this,
            boost::asio::placeholders::error, ++endpoint_iterator));
    }
    else
    {
        std::cout << "Error: " << err.message() << "\n";
    }
}


// -----------------------------------------------------------------------------
// Name: handle_connect 
// Created: SLG 2009-06-02
// -----------------------------------------------------------------------------
void TCP_Client::handle_connect( const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator)
{
    if (!err)
    {
        // The connection was successful. Send the information.
        if (parameter_ == "sending")
        {
            std::string messageTest = "test envoi message!";

            boost::asio::async_write( socket_, boost::asio::buffer(messageTest),
                                              boost::bind(&TCP_Client::handle_write, this,
                                              boost::asio::placeholders::error ));
        }
        else if ( parameter_ == "reception" )
        {
            boost::asio::async_read( socket_, boost::asio::buffer( ReceptionbufferMessage ),
                boost::asio::transfer_at_least(20), // (2)
                boost::bind(&TCP_Client::handle_read, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred ));
        }
       else
       {
           std::cout << "wrong parameters function in connection to webServices NCCO"  << "\n";
       }
    
    }
    else if (endpoint_iterator != tcp::resolver::iterator())
    {
        // The connection failed. Try the next endpoint in the list.
        socket_.close();
        tcp::endpoint endpoint = *endpoint_iterator;
        socket_.async_connect(endpoint,
            boost::bind(&TCP_Client::handle_connect, this,
            boost::asio::placeholders::error, ++endpoint_iterator));
    }
    else
    {
        std::cout << "Error: " << err.message() << "\n";
    }

}

// -----------------------------------------------------------------------------
// Name: handle_write 
// Created: SLG 2009-06-02
// -----------------------------------------------------------------------------
void TCP_Client::handle_write(const boost::system::error_code& err)
{
    if (!err)
    {
        // Hum!!!
    }
    else
    {
        std::cout << "Error: " << err.message() << "\n";
    }
}

// -----------------------------------------------------------------------------
// Name: handle_write 
// Created: SLG 2009-06-02
// -----------------------------------------------------------------------------
void TCP_Client::handle_read(const boost::system::error_code& err, size_t number_bytes_read )
{
    if (!err)
    {
        // Hum!!!
    }
    else
    {
        std::cout << "Error: " << err.message() << "\n";
    }
}
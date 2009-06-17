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
TCP_Client::TCP_Client( boost::asio::io_service& io_service, const std::string parameter, const std::string webServicePath, const std::string webServiceHost )
: resolver_(io_service),
  socket_(io_service),
  parameter_ (parameter)
{
    if ( parameter_ == "reception" )
    {
        std::ostream request_stream(&request_);
        request_stream << "GET " << webServicePath << " HTTP/1.0\r\n";
        request_stream << "Host: " << webServiceHost << "\r\n";
        request_stream << "Accept: */*\r\n";
        request_stream << "Connection: close\r\n\r\n";
        request_stream << "Content-Length: 28" << "\r\n";
    }
    else if  ( parameter_ == "sending" )
    {
        std::ostream request_stream(&request_);
        request_stream << "HEAD " << "/ServiceXmlIa/xmlia/type/create" << " HTTP/1.0\r\n";
        request_stream << "Host: " << webServiceHost << "\r\n";
        request_stream << "Accept: */*\r\n";
        request_stream << "Connection: close\r\n\r\n";
        request_stream << "poe: 1\r\n\r\n";
        request_stream << "Authorization: Basic\r\n\r\n";

        //request_stream << "Content-Length: 28" << "\r\n";
    }
    else 
    {
        std::cout << "wrong parameters function in connection to webServices NCCO"  << "\n";
    }



// Start an asynchronous resolve to translate the server and service names
// into a list of endpoints.
    tcp::resolver::query query(webServiceHost, "12100");
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


/// -----------------------------------------------------------------------------
// Name: handle_connect 
// Created: SLG 2009-06-02
// -----------------------------------------------------------------------------
void TCP_Client::handle_connect( const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator)
{
    if (!err)
    {
        // The connection was successful.
        boost::asio::async_write( socket_, request_,
            boost::bind(&TCP_Client::handle_write_request, this,
            boost::asio::placeholders::error ));


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
/*
// -----------------------------------------------------------------------------
// Name: handle_write 
// Created: SLG 2009-06-02
// -----------------------------------------------------------------------------
void TCP_Client::handle_write(const boost::system::error_code& err)
{
    if (!err)
    {
        // lecture de la réponse!!!
    }
    else
    {
        std::cout << "Error: " << err.message() << "\n";
    }
}
*/
// -----------------------------------------------------------------------------
// Name: handle_write 
// Created: SLG 2009-06-02
// -----------------------------------------------------------------------------
void TCP_Client::handle_write_request(const boost::system::error_code& err)
{
    if (!err)
    {
        // Read the response status line.
        boost::asio::async_read_until(socket_, response_, "\r\n",
            boost::bind(&TCP_Client::handle_read_status_line, this,
            boost::asio::placeholders::error));
    }
    else
    {
        std::cout << "Error: " << err.message() << "\n";
    }
}

// -----------------------------------------------------------------------------
// Name: handle_read_status_line 
// Created: SLG 2009-06-02
// -----------------------------------------------------------------------------
void TCP_Client::handle_read_status_line(const boost::system::error_code& err)
{
    if (!err)
    {
        // Check that response is OK.
        std::istream response_stream(&response_);
        std::string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;
        std::string status_message;
        std::getline(response_stream, status_message);
        if (!response_stream || http_version.substr(0, 5) != "HTTP/")
        {
            std::cout << "Invalid response\n";
            return;
        }
        if (status_code != 200 && status_code!= 201 )
        {
            std::cout << "Response returned with status code ";
            std::cout << status_code << "\n";
            return;
        }

        // Read the response headers, which are terminated by a blank line.
        boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
            boost::bind(&TCP_Client::handle_read_headers, this,
            boost::asio::placeholders::error));
    }
    else
    {
        std::cout << "Error: " << err << "\n";
    }
}

// -----------------------------------------------------------------------------
// Name: handle_read_status_line 
// Created: SLG 2009-06-02
// -----------------------------------------------------------------------------
void TCP_Client::handle_read_headers(const boost::system::error_code& err)
{
    if (!err)
    {
        std::ostringstream ss;
        ss << &response_;
        strContent_ += ss.str();
        // Process the response headers.
        /*std::istream response_stream(&response_);
        std::string header;
        while (std::getline(response_stream, header) && header != "\r")
        std::cout << header << "\n";
        std::cout << "\n";

        // Write whatever content we already have to output.
        if (response_.size() > 0)
        {
        //std::cout << &response_;
        std::ostringstream ss;
        ss << &response_;
        strContent_ += ss.str();
        }*/

        // Start reading remaining data until EOF.
        boost::asio::async_read(socket_, response_,
            boost::asio::transfer_at_least(1),
            boost::bind(&TCP_Client::handle_read_content, this,
            boost::asio::placeholders::error));
    }
    else
    {
        std::cout << "Error: " << err << "\n";
    }
}


// -----------------------------------------------------------------------------
// Name: handle_read_status_line 
// Created: SLG 2009-06-02
// -----------------------------------------------------------------------------
void TCP_Client::handle_read_content(const boost::system::error_code& err)
{
    if (!err)
    {
        if ( parameter_ == "reception" )
        {

            // Write all of the data that has been read so far.
            std::ostringstream ss;
            ss << &response_;
            strContent_ += ss.str();

            // Continue reading remaining data until EOF.
            boost::asio::async_read(socket_, response_,
                boost::asio::transfer_at_least(1),
                boost::bind(&TCP_Client::handle_read_content, this,
                boost::asio::placeholders::error));
        }
        else if ( parameter_ == "sending" )
        {
            // Write all of the data that has been read so far.
            std::ostringstream ss;
            ss << &response_;
            strContent_ += ss.str();

            // Continue reading remaining data until EOF.
            boost::asio::async_read(socket_, response_,
                boost::asio::transfer_at_least(1),
                boost::bind(&TCP_Client::handle_read_content, this,
                boost::asio::placeholders::error));
        }
    }
    else if (err != boost::asio::error::eof)
    {
        std::cout << "Error: " << err << "\n";
    }
}

// -----------------------------------------------------------------------------
// Name: GetContent 
// Created: SLG 2009-06-02
// -----------------------------------------------------------------------------
std::string TCP_Client::GetContent()
{
    return strContent_;
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
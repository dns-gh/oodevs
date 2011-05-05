// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "edxlhave_app_pch.h"
#include "RestClient.h"
#include "UrlEncoder.h"

#include <iostream>
using namespace edxl;



// -----------------------------------------------------------------------------
// Name: RestClient SSL constructor
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
RestClient::RestClient( const std::string& host, const std::string& path, bool useSsl )
    : host_ ( host )
    , path_ ( path )
    , status_ ( 400 )
    , useChunk_( false )
    , socket_ ( host_, useSsl )
{
}

// -----------------------------------------------------------------------------
// Name: RestClient destructor
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
RestClient::~RestClient()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RestClient::DoPost
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
void RestClient::DoPost( const std::string& message, std::string& response )
{
    boost::asio::streambuf request;
    std::ostream request_stream( &request );
    request_stream << "POST " << path_ << " HTTP/1.0\r\n";
    request_stream << "Host: " << host_ << "\r\n";
    request_stream << "Authorization: Basic\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n";
    request_stream << UrlEncoder( message );

    ProceedRequest( request, response );
}

// -----------------------------------------------------------------------------
// Name: RestClient::DoGet
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
void RestClient::DoGet( const std::string& /*message*/, std::string& response )
{
    boost::asio::streambuf request;
    std::ostream request_stream( &request );
    request_stream << "GET " << path_ << " HTTP/1.0\r\n";
    request_stream << "Host: " << host_ << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";
    ProceedRequest( request, response );
}


// -----------------------------------------------------------------------------
// Name: RestClient::ProceedRequest
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
void RestClient::ProceedRequest( boost::asio::streambuf& request, std::string& content )
{
    boost::asio::streambuf response;

    // Send the request.
    socket_.Write( request );

    ReadHeader( response );
    if( GetStatus() == 200 )
        ReadContent( response, content );
}

// -----------------------------------------------------------------------------
// Name: RestClient::WriteRequestHandler
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
void RestClient::ReadHeader( boost::asio::streambuf& response )
{
    socket_.ReadUntil( response, "\r\n" );

    // Check that response is OK.
    std::istream response_stream( &response );
    std::string http_version, status_message;
    response_stream >> http_version >> status_;

    std::getline( response_stream, status_message );
    if( !response_stream || http_version.substr( 0, 5 ) != "HTTP/" )
        status_ = 204; // NO CONTENT
    // else
    // LOG( status_message );

    // Read the response headers, which are terminated by a blank line.
    socket_.ReadUntil( response, "\r\n\r\n" );

    // Proceed header
    std::string header;
    std::string encoding( "Transfer-Encoding:" );
    while ( std::getline( response_stream, header ) && header != "\r" )
    {
        std::string::size_type pos = header.find( encoding );
        if( pos != std::string::npos )
        {
            std::string::size_type size = pos + encoding.size() + 1;
            useChunk_ = header.substr( size, header.size() - size - 1 ) == "chunked";
        }
    }
}

// -----------------------------------------------------------------------------
// Name: RestClient::ReadResponseContent
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
void RestClient::ReadContent( boost::asio::streambuf& response, std::string& content )
{
    boost::system::error_code error = boost::asio::error::host_not_found;

    // Read until EOF, writing data to output as we go.

    while( socket_.Read( response, error ) )
    {
        std::ostringstream ss;
        ss << &response;
        content += ss.str();
    }
    if( error != boost::asio::error::eof && error != boost::asio::error::shut_down )
        throw boost::system::system_error( error );
}


// -----------------------------------------------------------------------------
// Name: RestClient::GetStatus
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
unsigned RestClient::GetStatus() const
{
    return status_;
}

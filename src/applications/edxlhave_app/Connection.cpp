// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "edxlhave_app_pch.h"
#include "Connection.h"
#include <iostream>
using namespace edxl;


Connection::Connection( const std::string& host, bool useSsl)
    : io_service_ ()
    , resolver_ ( io_service_ )
    , socket_ ( io_service_ )
    , ctx_( io_service_, boost::asio::ssl::context::sslv23 )
    , useSsl_( useSsl )
    , sslSocket_ ( socket_, ctx_ )
{
    // Start an asynchronous resolve to translate the server and service names
    // into a list of endpoints.
    tcp::resolver::query query( host, useSsl_ ? "https" : "http" );

    boost::system::error_code error = boost::asio::error::host_not_found;
    ResolveHandler( query, error );
    if( error )
      throw boost::system::system_error( error );

    if( useSsl_ )
    {
        ctx_.set_verify_mode( boost::asio::ssl::context::verify_none );
        sslSocket_.handshake( boost::asio::ssl::stream_base::client );
    }
}

Connection::~Connection( )
{
    socket_.close();
}

// -----------------------------------------------------------------------------
// Name: Connection::ResolveHandler
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
void Connection::ResolveHandler( tcp::resolver::query& query, boost::system::error_code& error )
{
    tcp::resolver::iterator endpoint_iterator = resolver_.resolve( query );
    tcp::resolver::iterator end;

    // Try each endpoint until we successfully establish a connection.
    while ( error && endpoint_iterator != end )
    {
        socket_.close();
        socket_.connect( *endpoint_iterator++, error );
    }
}

size_t Connection::Write( boost::asio::streambuf& request )
{
    size_t result;
    if( useSsl_ )
        result = boost::asio::write( sslSocket_, request );
    else
        result = boost::asio::write( socket_, request );
    return result;
}

size_t Connection::ReadUntil( boost::asio::streambuf& response, const std::string& delimiter )
{
    size_t result;
    if( useSsl_ )
        result = boost::asio::read_until( sslSocket_, response, delimiter);
    else
        result = boost::asio::read_until( socket_, response, delimiter);
    return result;
}

size_t Connection::Read( boost::asio::streambuf& response, boost::system::error_code& error )
{
    size_t result;
    if( useSsl_ )
        result = boost::asio::read( sslSocket_, response, boost::asio::transfer_at_least( 1 ), error );
    else
        result = boost::asio::read( socket_, response, boost::asio::transfer_at_least( 1 ), error );
    return result;
}

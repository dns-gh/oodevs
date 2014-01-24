// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "timeline_plugin_pch.h"
#include "Connection.h"

using namespace plugins::timeline;

namespace
{
    std::pair< std::string, std::string > ExtractHost( const std::string& host )
    {
        std::string::size_type pos = host.find_last_of( ":" );
        return std::make_pair( std::string( host, 0, pos ), ( pos != std::string::npos ) ? std::string( host, pos + 1, host.length() - pos ) : "" );
    }
}

// -----------------------------------------------------------------------------
// Name: Connection constructor
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
Connection::Connection( const std::string& host, bool useSsl )
    : resolver_ ( io_service_ )
    , socket_   ( io_service_ )
    , ctx_      ( io_service_, boost::asio::ssl::context::sslv23 )
    , useSsl_   ( useSsl )
    , sslSocket_( socket_, ctx_ )
{
    std::pair< std::string, std::string > hostinfo( ExtractHost( host ) );
    tcp::resolver::query query( hostinfo.first, hostinfo.second.empty() ? (useSsl_ ? "https" : "http") : hostinfo.second );
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

// -----------------------------------------------------------------------------
// Name: Connection destructor
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
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
    tcp::resolver::iterator it = resolver_.resolve( query );
    while( error && it != tcp::resolver::iterator() )
    {
        socket_.close();
        socket_.connect( *it++, error );
    }
}

// -----------------------------------------------------------------------------
// Name: Connection::Write
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
size_t Connection::Write( boost::asio::streambuf& request )
{
    if( useSsl_ )
        return boost::asio::write( sslSocket_, request );
    return boost::asio::write( socket_, request );
}

// -----------------------------------------------------------------------------
// Name: Connection::ReadUntil
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
size_t Connection::ReadUntil( boost::asio::streambuf& response, const std::string& delimiter )
{
    if( useSsl_ )
        return boost::asio::read_until( sslSocket_, response, delimiter);
    return boost::asio::read_until( socket_, response, delimiter);
}

// -----------------------------------------------------------------------------
// Name: Connection::Read
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
size_t Connection::Read( boost::asio::streambuf& response, boost::system::error_code& error )
{
    if( useSsl_ )
        return boost::asio::read( sslSocket_, response, boost::asio::transfer_at_least( 1 ), error );
    return boost::asio::read( socket_, response, boost::asio::transfer_at_least( 1 ), error );
}

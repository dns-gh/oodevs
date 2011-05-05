// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "edxlhave_app_pch.h"
#include "SwordClient.h"

// -----------------------------------------------------------------------------
// Name: SwordClient constructor
// Created: BCI 2011-04-29
// -----------------------------------------------------------------------------
SwordClient::SwordClient()
: resolver_ ( io_service_ )
, socket_ ( io_service_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SwordClient destructor
// Created: BCI 2011-04-29
// -----------------------------------------------------------------------------
SwordClient::~SwordClient()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SwordClient::Connect
// Created: BCI 2011-04-29
// -----------------------------------------------------------------------------
void SwordClient::Connect( const std::string& hostname, const std::string& port, bool waitconnection )
{
    boost::system::error_code error = boost::asio::error::host_not_found;
    do
    {
        boost::asio::ip::tcp::resolver resolver( io_service_ );
        boost::asio::ip::tcp::resolver::query query( boost::asio::ip::tcp::v4(), hostname, port );
        boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve( query );
        boost::asio::ip::tcp::resolver::iterator end;

        // Try each endpoint until we successfully establish a connection.
        while ( error && endpoint_iterator != end )
        {
            socket_.close();
            socket_.connect( *endpoint_iterator++, error );
        }
    } while( error && waitconnection );
}

// -----------------------------------------------------------------------------
// Name: SwordClient::Send
// Created: BCI 2011-04-29
// -----------------------------------------------------------------------------
    template< typename Message >
void SwordClient::Send( const Message& message, unsigned long tag )
{
    // if message is not properly filled, fail
    if( !message.IsInitialized() )
        throw std::runtime_error( "Invalid message (missing required fields)" );

    // allocate encoding buffer
    std::vector< google::protobuf::uint8 > buffer( message.ByteSize() );

    // encode message to destination buffer
    if( !message.SerializeWithCachedSizesToArray( &buffer.front() ) )
        throw std::runtime_error( "Serialization failed" );

    // create header with size and message tag
    unsigned long header[2];
    header[0] = htonl( message.GetCachedSize() + sizeof( unsigned long ) );
    header[1] = htonl( tag );

    // send header
    boost::asio::write( socket_, boost::asio::buffer( &header[0], 2 * sizeof( unsigned long ) ) );

    // send content
    boost::asio::write( socket_, boost::asio::buffer( &buffer.front(), message.GetCachedSize() ) );
}

// -----------------------------------------------------------------------------
// Name: SwordClient::Send
// Created: BCI 2011-04-29
// -----------------------------------------------------------------------------
void SwordClient::Send( const sword::ClientToAuthentication& message )
{
    Send( message, sword::tag::client_authentication );
}

// -----------------------------------------------------------------------------
// Name: SwordClient::Send
// Created: BCI 2011-04-29
// -----------------------------------------------------------------------------
void SwordClient::Send( const sword::ClientToSim& message )
{
    Send( message, sword::tag::client_simulation );
}

// -----------------------------------------------------------------------------
// Name: SwordClient::Receive
// Created: BCI 2011-04-29
// -----------------------------------------------------------------------------
bool SwordClient::Receive()
{
    // read message header
    unsigned long size = 0;
    unsigned long tag = 0;
    boost::asio::read( socket_, boost::asio::buffer( &size, sizeof( size ) ), boost::asio::transfer_at_least( sizeof( size ) ) );
    boost::asio::read( socket_, boost::asio::buffer( &tag, sizeof( tag ) ), boost::asio::transfer_at_least( sizeof( tag ) ) );
    size = ntohl( size );
    tag  = ntohl( tag );
    const unsigned long body_size = size - sizeof( unsigned long );

    // read message content
    std::vector< google::protobuf::uint8 > buffer( body_size );
    boost::asio::read( socket_, boost::asio::buffer( buffer, body_size ), boost::asio::transfer_at_least( body_size ) );

    // process message body
    if( tag == sword::tag::authentication_client )
        ReadMessage< sword::AuthenticationToClient >( buffer );
    else if( tag == sword::tag::aar_client )
        ReadMessage< sword::AarToClient >( buffer );
    else if( tag == sword::tag::dispatcher_client )
        ReadMessage< sword::DispatcherToClient >( buffer );
    else if( tag == sword::tag::messenger_client )
        ReadMessage< sword::MessengerToClient >( buffer );
    else if( tag == sword::tag::replay_client )
        ReadMessage< sword::ReplayToClient >( buffer );
    else if( tag == sword::tag::simulation_client )
        ReadMessage< sword::SimToClient >( buffer );
    else
    {
        std::cerr << "Received unhandled message: "
            << std::dec << "size=" << size
            << std::hex << " tag=" << tag << std::endl;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: SwordClient::ReadMessage
// Created: BCI 2011-04-29
// -----------------------------------------------------------------------------
template< typename Message >
void SwordClient::ReadMessage( const std::vector< google::protobuf::uint8 >& buffer )
{
    // decode message
    Message message;
    if( ! message.ParseFromArray( &buffer.front(), buffer.size() ) )
        throw std::runtime_error( "Deserialization failed" );
    Handle( message );
}

// -----------------------------------------------------------------------------
// Name: SwordClient::Handle
// Created: BCI 2011-05-02
// -----------------------------------------------------------------------------
template< typename Message >
void SwordClient::Handle( const Message& )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: SwordClient::Handle
// Created: BCI 2011-05-02
// -----------------------------------------------------------------------------
template<>
void SwordClient::Handle( const sword::SimToClient& message )
{
    simToClientHandler_( message );
}

// -----------------------------------------------------------------------------
// Name: SwordClient::function< void
// Created: BCI 2011-05-02
// -----------------------------------------------------------------------------
void SwordClient::SetListener( boost::function< void( const sword::SimToClient& ) > listener )
{
    simToClientHandler_ = listener;
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "PortFactory.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/thread.hpp>

using namespace host;
namespace aio = boost::asio;
using boost::asio::ip::tcp;

namespace
{
class PortLink : public Port_ABC
{
public:
    explicit PortLink( int port, std::function< void( int ) > deleter )
        : port_   ( port )
        , deleter_( deleter )
    {
        // NOTHING
    }
    ~PortLink()
    {
        deleter_( port_ );
    }
    virtual int Get() const
    {
        return port_;
    }
    int port_;
    std::function< void( int ) > deleter_;
};
}

// -----------------------------------------------------------------------------
// Name: PortFactory::PortFactory
// Created: BAX 2012-03-20
// -----------------------------------------------------------------------------
PortFactory::PortFactory( int period, int min, int max )
    : period_( period )
    , min_   ( min )
    , max_   ( max )
{
    if( min_ >= max_ )
        throw std::runtime_error( ( boost::format( "%1% >= %2%" ) % min_ % max_ ).str() );
}

// -----------------------------------------------------------------------------
// Name: PortFactory::~PortFactory
// Created: BAX 2012-03-20
// -----------------------------------------------------------------------------
PortFactory::~PortFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PortFactory::Acquire
// Created: BAX 2012-03-20
// -----------------------------------------------------------------------------
Port PortFactory::Acquire( int port )
{
    ports_.insert( port );
    return boost::make_shared< PortLink >( port, boost::bind( &PortFactory::Release, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: PortFactory::Create
// Created: BAX 2012-03-20
// -----------------------------------------------------------------------------
Port PortFactory::Create()
{
    boost::lock_guard< boost::mutex > lock( access_ );
    // fast-check last value
    if( !ports_.empty() )
    {
        const int last = *ports_.rbegin() + period_;
        if( last < max_ )
            return Acquire( last );
    }
    // revert to linear search
    int idx = min_;
    for( T_Ports::const_iterator it = ports_.find( idx ); it != ports_.end(); ++it, idx += period_ )
        if( idx != *it )
            return Acquire( idx );
    if( idx >= max_ )
        throw std::runtime_error( "unable to find any available port" );
    return Acquire( idx );
}

// -----------------------------------------------------------------------------
// Name: PortFactory::Create
// Created: BAX 2012-03-21
// -----------------------------------------------------------------------------
Port PortFactory::Create( int port )
{
    if( port < min_ || max_ <= port || ( ( port - min_ ) % period_ ) )
        throw std::runtime_error( ( boost::format( "invalid port value %1%" ) % port ).str() );

    boost::lock_guard< boost::mutex > lock( access_ );
    if( ports_.count( port ) )
        throw std::runtime_error( ( boost::format( "unable to acquire port %1%" ) % port ).str() );
    return Acquire( port );
}

// -----------------------------------------------------------------------------
// Name: PortFactory::Release
// Created: BAX 2012-03-20
// -----------------------------------------------------------------------------
void PortFactory::Release( int port )
{
    boost::lock_guard< boost::mutex > lock( access_ );
    ports_.erase( port );
}

bool PortFactory::WaitConnected( int port, const std::function< bool() >& alive )
{
    aio::io_service service;
    const tcp::endpoint endpoint( aio::ip::address::from_string( "127.0.0.1" ), static_cast< uint16_t >( port ) );
    tcp::socket socket( service );
    boost::system::error_code ec;
    auto deadline = boost::posix_time::microsec_clock::local_time()
                  + boost::posix_time::minutes( 1 );
    while( boost::posix_time::microsec_clock::local_time() < deadline )
    {
        socket.connect( endpoint, ec );
        if( !ec )
            return true;
        if( !alive() )
            return false;
        boost::this_thread::sleep( boost::posix_time::milliseconds( 100 ) );
    }
    return false;
}

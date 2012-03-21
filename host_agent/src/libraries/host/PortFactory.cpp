// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "PortFactory.h"
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/function.hpp>

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4244 )
#endif
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

using namespace host;

namespace
{
// -----------------------------------------------------------------------------
// Name: Port class definition
// Created: BAX 2012-03-20
// -----------------------------------------------------------------------------
class Port : public Port_ABC
{
public:
    explicit Port( int port, boost::function< void( int ) > deleter )
        : port_   ( port )
        , deleter_( deleter )
    {
        // NOTHING
    }
    ~Port()
    {
        deleter_( port_ );
    }
    virtual int Get() const
    {
        return port_;
    }
    int port_;
    boost::function< void( int ) > deleter_;
};

}

// -----------------------------------------------------------------------------
// Name: PortFactory::PortFactory
// Created: BAX 2012-03-20
// -----------------------------------------------------------------------------
PortFactory::PortFactory( int period, int min, int max )
    : access_( new boost::shared_mutex() )
    , period_( period )
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
std::auto_ptr< Port_ABC > PortFactory::Acquire( int port )
{
    ports_.insert( port );
    return std::auto_ptr< Port_ABC >( new Port( port, boost::bind( &PortFactory::Release, this, _1 ) ) );
}

// -----------------------------------------------------------------------------
// Name: PortFactory::Create
// Created: BAX 2012-03-20
// -----------------------------------------------------------------------------
std::auto_ptr< Port_ABC > PortFactory::Create()
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    // fast-check last value
    if( !ports_.empty() )
        for( const int last = *ports_.rbegin() + period_; last < max_; /**/ )
            return Acquire( last );
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
std::auto_ptr< Port_ABC > PortFactory::Create( int port )
{
    if( port < min_ || max_ <= port || ( ( port - min_ ) % period_ ) )
        throw std::runtime_error( ( boost::format( "invalid port value %1%" ) % port ).str() );

    boost::lock_guard< boost::shared_mutex > lock( *access_ );
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
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    ports_.erase( port );
}

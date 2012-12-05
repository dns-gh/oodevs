// *****************************************************************************
//
// $Created: NLD 2004-02-11 $
// $Archive: /MVW_v10/Build/SDK/SIM/src/SIM_NetworkLogger.cpp $
// $Author: Nld $
// $Modtime: 8/11/04 10:05 $
// $Revision: 2 $
// $Workfile: SIM_NetworkLogger.cpp $
//
// *****************************************************************************

#include "simulation_app_pch.h"
#include "SIM_NetworkLogger.h"
#ifdef _MSC_VER
#   pragma warning( push, 0 )
#endif
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#   pragma warning( disable: 4503 )
#endif

// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger constructor
// Created: NLD 2004-02-11
// -----------------------------------------------------------------------------
SIM_NetworkLogger::SIM_NetworkLogger( unsigned short nPort, int nLogLevels )
    : MT_Logger_ABC( nLogLevels )
    , acceptor_( service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), nPort ) )
    , mutex_   ( new boost::mutex() )
{
    Accept();
    thread_.reset( new boost::thread( boost::bind( &boost::asio::io_service::run, &service_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger destructor
// Created: NLD 2004-02-11
// -----------------------------------------------------------------------------
SIM_NetworkLogger::~SIM_NetworkLogger()
{
    sockets_.clear();
    service_.stop();
    thread_->join();
}

// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger::Accept
// Created: RDS 2008-07-11
// -----------------------------------------------------------------------------
void SIM_NetworkLogger::Accept()
{
    T_Socket socket( new boost::asio::ip::tcp::socket( service_ ) );
    acceptor_.async_accept( *socket, boost::bind( &SIM_NetworkLogger::OnAccept, this, socket, boost::asio::placeholders::error ) );
}

// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger::OnAccept
// Created: RDS 2008-07-11
// -----------------------------------------------------------------------------
void SIM_NetworkLogger::OnAccept( T_Socket socket, const boost::system::error_code& error )
{
    if( !error )
    {
        boost::lock_guard< boost::mutex > locker( *mutex_ );
        sockets_.insert( socket ) ;
    }
    Accept();
}

// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger::OnWrite
// Created: RDS 2008-07-09
// -----------------------------------------------------------------------------
void SIM_NetworkLogger::OnWrite( T_Socket socket, const boost::system::error_code& error )
{
    if( error )
    {
        boost::lock_guard< boost::mutex > locker( *mutex_ );
        sockets_.erase( socket ) ;
    }
}

void SIM_NetworkLogger::WriteString( const std::string& s )
{
    boost::lock_guard< boost::mutex > locker( *mutex_ );
    for( auto it = sockets_.begin(); it != sockets_.end(); ++it )
        boost::asio::async_write( **it, boost::asio::buffer( s.data(), s.size() ),
                                  boost::bind( &SIM_NetworkLogger::OnWrite, this, *it, boost::asio::placeholders::error ) ) ;
}

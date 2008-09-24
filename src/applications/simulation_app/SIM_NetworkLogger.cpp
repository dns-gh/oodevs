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
#endif

// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger constructor
// Created: NLD 2004-02-11
// -----------------------------------------------------------------------------
SIM_NetworkLogger::SIM_NetworkLogger( uint nPort, uint nLogLevels, uint nLogLayers )
    : MT_Logger_ABC( nLogLevels, nLogLayers )
    , sockets_   ()
    , io_service_() 
    , acceptor_  (io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), nPort ) )
    , criticalSection_ ( new boost::mutex ) 
{
    WaitForClient(); 
    thread_.reset( new boost::thread( boost::bind( &boost::asio::io_service::run, &io_service_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger destructor
// Created: NLD 2004-02-11
// -----------------------------------------------------------------------------
SIM_NetworkLogger::~SIM_NetworkLogger()
{
    sockets_.clear(); 
    io_service_.stop();
    thread_->join();
}


// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger::WaitForClient
// Created: RDS 2008-07-11
// -----------------------------------------------------------------------------
void SIM_NetworkLogger::WaitForClient()
{
    boost::shared_ptr< boost::asio::ip::tcp::socket > socket( new boost::asio::ip::tcp::socket( io_service_ ) );
    acceptor_.async_accept( *socket, boost::bind( &SIM_NetworkLogger::StartConnection, this, socket, boost::asio::placeholders::error ) );
}

// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger::StartConnection
// Created: RDS 2008-07-11
// -----------------------------------------------------------------------------
void SIM_NetworkLogger::StartConnection( boost::shared_ptr< boost::asio::ip::tcp::socket > newClientSocket, const boost::system::error_code& error )
{
    if ( !error )
    {
        boost::lock_guard< boost::mutex>  locker( *criticalSection_ );
        sockets_.insert( newClientSocket ) ; 
        WaitForClient();
    }
}

// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger::StopConnection
// Created: RDS 2008-07-11
// -----------------------------------------------------------------------------
void SIM_NetworkLogger::StopConnection( boost::shared_ptr< boost::asio::ip::tcp::socket > socket )
{
    boost::lock_guard< boost::mutex>  locker( *criticalSection_ );
    sockets_.erase( socket ) ; 
}

// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger::AsyncWrite
// Created: RDS 2008-07-09
// -----------------------------------------------------------------------------
void SIM_NetworkLogger::AsyncWrite( boost::shared_ptr< boost::asio::ip::tcp::socket > socket, const boost::system::error_code& error )
{
    if (error) 
        StopConnection( socket );  
}

// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger::LogString
// Created: NLD 2004-02-11
// -----------------------------------------------------------------------------
void SIM_NetworkLogger::LogString( const char* strLayerName, E_LogLevel nLevel, const char* szMsg, const char* strContext, int nCode )
{
    boost::lock_guard< boost::mutex>  locker( *criticalSection_ );
    
    std::stringstream strTmp;

    strTmp << "[" << GetTimestampAsString() << "]";

    if( strLayerName != NULL )
        strTmp << " " << strLayerName << " -";

    if( nLevel != eLogLevel_None )
        strTmp << " " << GetLogLevelAsString( nLevel ) << " -";

    strTmp << " " << szMsg;

    if( nCode != -1 )
        strTmp << " (" << nCode << ")";

    if( strContext != NULL )
        strTmp << " [Context: " << strContext << "]";

    strTmp << "\r\n";
        
    for ( IT_SocketSet it = sockets_.begin(); it != sockets_.end(); it++ ) 
    {
        boost::asio::async_write( **it,  boost::asio::buffer( strTmp.str().data() , strTmp.str().size() ) ,
                                  boost::bind(&SIM_NetworkLogger::AsyncWrite, this, *it, boost::asio::placeholders::error) ) ;
    }
}


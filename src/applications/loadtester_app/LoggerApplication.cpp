// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "LoggerApplication.h"
#include "game_asn/AarSenders.h"
#include "game_asn/AuthenticationSenders.h"
#include "game_asn/ClientSenders.h"
#include "game_asn/DispatcherSenders.h"
#include "game_asn/MessengerSenders.h"
#include "game_asn/ReplaySenders.h"
#include <ctime>

// -----------------------------------------------------------------------------
// Name: LoggerApplication constructor
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
LoggerApplication::LoggerApplication( const std::string& hostname, const std::string& logFile, const std::string& login, const std::string& password, bool verbose )
: login_          ( login )
, password_       ( password )
, bConnectionLost_( false )
, bVerbose_       ( verbose )
{
    file_.open( logFile.c_str(), std::ios::out | std::ios::trunc );
    RegisterMessage( *this, &LoggerApplication::OnReceiveMsgSimToClient );
    RegisterMessage( *this, &LoggerApplication::OnReceiveMsgAuthenticationToClient );
    RegisterMessage( *this, &LoggerApplication::OnReceiveMsgDispatcherToClient );
    RegisterMessage( *this, &LoggerApplication::OnReceiveMsgMessengerToClient );
    
    RegisterMessage( *this, &LoggerApplication::OnReceiveMsgReplayToClient );
    RegisterMessage( *this, &LoggerApplication::OnReceiveMsgAarToClient );

    Connect( hostname, false );
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication destructor
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
LoggerApplication::~LoggerApplication()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::Run
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
int LoggerApplication::Run()
{
    try
    {
        while ( !bConnectionLost_ )
        {
            Update();
            boost::this_thread::sleep( boost::posix_time::milliseconds( 50 ) ) ;
        }
    }
    catch (std::runtime_error& err )
    {
        file_ << "Error: " << err.what() << std::endl;
        return 2;
    }
    catch (...)
    {
        file_ << "Unexpected exception caught" << std::endl;
        return 3;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::ConnectionSucceeded
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::ConnectionSucceeded( const std::string& endpoint )
{
    endpoint_ = endpoint;
    authentication::AuthenticationRequest message;
    message().login    = login_.c_str();
    message().password = password_.c_str();
	message.Send( *this );
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::ConnectionFailed
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::ConnectionFailed( const std::string& address, const std::string& error )
{
    DumpTime();
    file_ << "Connection failed: " << address << " : " << error << std::endl << std::flush;
    bConnectionLost_ = true;
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::ConnectionError
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::ConnectionError( const std::string& address, const std::string& error )
{
    DumpTime();
    file_ << "Connection error: " << address << " : " << error << std::endl << std::flush;
    bConnectionLost_ = true;
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::OnReceiveMsgSimToClient
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::OnReceiveMsgSimToClient( const std::string& /*from*/, const ASN1T_MsgsSimToClient& message )
{
    switch( message.msg.t )
    {
        case T_MsgsSimToClient_msg_msg_control_begin_tick:
        {
            OnReceiveMsgControlBeginTick( message.msg.u.msg_control_begin_tick->current_tick );
            break;
        }
        default:
            LogMessage( message.msg.t );
    };
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::OnReceiveMsgAuthenticationToClient
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::OnReceiveMsgAuthenticationToClient( const std::string& /*from*/, const ASN1T_MsgsAuthenticationToClient& /*message*/ )
{
    DumpTime();
    file_ << "Authentication received" << std::endl << std::flush;
}
    
// -----------------------------------------------------------------------------
// Name: LoggerApplication::OnReceiveMsgDispatcherToClient
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::OnReceiveMsgDispatcherToClient( const std::string& /*from*/, const ASN1T_MsgsDispatcherToClient& /*message*/ )
{
    DumpTime();
    file_ << "Dispatcher message received" << std::endl << std::flush;
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::OnReceiveMsgMessengerToClient
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::OnReceiveMsgMessengerToClient( const std::string& /*from*/, const ASN1T_MsgsMessengerToClient& /*message*/ )
{
    DumpTime();
    file_ << "Messenger message received" << std::endl << std::flush;
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::OnReceiveMsgReplayToClient
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::OnReceiveMsgReplayToClient( const std::string& /*from*/, const ASN1T_MsgsReplayToClient& /*message*/ )
{
    DumpTime();
    file_ << "Replay message received" << std::endl << std::flush;
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::OnReceiveMsgAarToClient
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::OnReceiveMsgAarToClient( const std::string& /*from*/, const ASN1T_MsgsAarToClient& /*message*/ )
{
    DumpTime();
    file_ << "Aar message received" << std::endl << std::flush;
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::OnReceiveMsgControlBeginTick
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::OnReceiveMsgControlBeginTick( int tick )
{
    DumpTime();
    file_ << "New tick " << tick << std::endl << std::flush;
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::LogMessage
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::LogMessage( int type )
{
    if( bVerbose_ )
    {
        DumpTime();
        file_ << "Received message type " << type << std::endl << std::flush;
    }
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::DumpTime
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::DumpTime()
{
    static char buffer[256];
    time_t nTime = time( NULL );
#pragma warning( disable : 4996 )
    strftime(buffer, 256, "%H:%M:%S", localtime( &nTime ) );
#pragma warning( pop )
    file_ << buffer<< " : ";
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::Send
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::Send( const ASN1T_MsgsClientToAuthentication& message )
{
    MessageSender_ABC::Send( endpoint_, message );
}
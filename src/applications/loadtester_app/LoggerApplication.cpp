// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "LoggerApplication.h"
#include "protocol/authenticationsenders.h"
#include "protocol/clientsenders.h"
#include "protocol/simulationsenders.h"
#include "protocol/simulation.h"
#include "protocol/aarsenders.h"
#include "protocol/replaysenders.h"
#include "protocol/dispatchersenders.h"
#include "protocol/messengersenders.h"
#include "protocol/ProtocolVersionChecker.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>
#include <google/protobuf/Descriptor.h>

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
    message().mutable_version()->set_value( ProtocolVersionChecker::GetCurrentProtocolVersion() );
    message().set_login( login_.c_str() );
    message().set_password( password_.c_str() );
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
void LoggerApplication::OnReceiveMsgSimToClient( const std::string& /*from*/, const MsgsSimToClient::MsgSimToClient& wrapper )
{
    if( wrapper.message().has_control_begin_tick() )
        OnReceiveMsgControlBeginTick( wrapper.message().control_begin_tick().current_tick() );
    else
        LogMessage( wrapper );
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::OnReceiveMsgAuthenticationToClient
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::OnReceiveMsgAuthenticationToClient( const std::string& /*from*/, const MsgsAuthenticationToClient::MsgAuthenticationToClient& /*wrapper*/ )
{
    DumpTime();
    file_ << "Authentication received" << std::endl << std::flush;
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::OnReceiveMsgDispatcherToClient
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::OnReceiveMsgDispatcherToClient( const std::string& /*from*/, const MsgsDispatcherToClient::MsgDispatcherToClient& wrapper )
{
    DumpTime();
    file_ << "Dispatcher message received" << std::endl << std::flush;
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::OnReceiveMsgMessengerToClient
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::OnReceiveMsgMessengerToClient( const std::string& /*from*/, const MsgsMessengerToClient::MsgMessengerToClient& /*wrapper*/ )
{
    DumpTime();
    file_ << "Messenger message received" << std::endl << std::flush;
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::OnReceiveMsgReplayToClient
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::OnReceiveMsgReplayToClient( const std::string& /*from*/, const MsgsReplayToClient::MsgReplayToClient& /*wrapper*/ )
{
    DumpTime();
    file_ << "Replay message received" << std::endl << std::flush;
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::OnReceiveMsgAarToClient
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::OnReceiveMsgAarToClient( const std::string& /*from*/, const MsgsAarToClient::MsgAarToClient& /*wrapper*/ )
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
void LoggerApplication::LogMessage( const MsgsSimToClient::MsgSimToClient& wrapper )
{
    if( bVerbose_ )
    {
        DumpTime();
        file_ << "Received " << wrapper.message().DebugString() << std::endl;
        
        /*typedef std::vector< const google::protobuf::FieldDescriptor* > T_Fields;
        T_Fields fields;
        const google::protobuf::Reflection* reflect = wrapper.message().GetReflection();
        reflect->ListFields( wrapper.message(), &fields );
        BOOST_FOREACH( const T_Fields::value_type& field, fields )
            if( reflect->HasField( wrapper.message(), field ) )
                file_ << "Received " << wrapper.GetDescriptor()->full_name() << " of type: " << field->name() << std::endl;
                */
    }
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::DumpTime
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::DumpTime()
{
    file_ << boost::posix_time::microsec_clock::universal_time() << ": ";
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::Send
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::Send( const MsgsClientToAuthentication::MsgClientToAuthentication& wrapper )
{
    MessageSender_ABC::Send( endpoint_, wrapper );
}
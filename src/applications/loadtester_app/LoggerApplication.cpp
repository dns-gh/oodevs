// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "LoggerApplication.h"
#include "protocol/AuthenticationSenders.h"
#include "protocol/ClientSenders.h"
#include "protocol/SimulationSenders.h"
#include "protocol/AarSenders.h"
#include "protocol/ReplaySenders.h"
#include "protocol/DispatcherSenders.h"
#include "protocol/MessengerSenders.h"
#pragma warning( push, 0 )
#include <google/protobuf/descriptor.h>
#pragma warning( pop )
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>

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
    RegisterMessage( *this, &LoggerApplication::OnReceiveSimToClient );
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
        while( !bConnectionLost_ )
        {
            Update();
            boost::this_thread::sleep( boost::posix_time::milliseconds( 50 ) ) ;
        }
    }
    catch( std::runtime_error& err )
    {
        file_ << "Error: " << err.what() << std::endl;
        return 2;
    }
    catch(...)
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
    message().mutable_version()->set_value( sword::ProtocolVersion().value() );
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
    file_ << "Connection failed: " << address << " : " << error << std::endl;
    bConnectionLost_ = true;
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::ConnectionError
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::ConnectionError( const std::string& address, const std::string& error )
{
    DumpTime();
    file_ << "Connection error: " << address << " : " << error << std::endl;
    bConnectionLost_ = true;
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::ConnectionWarning
// Created: MCO 2011-09-28
// -----------------------------------------------------------------------------
void LoggerApplication::ConnectionWarning( const std::string& address, const std::string& warning )
{
    DumpTime();
    file_ << "Connection warning: " << address << " : " << warning << std::endl;
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::OnReceiveSimToClient
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::OnReceiveSimToClient( const std::string& /*from*/, const sword::SimToClient& wrapper )
{
    if( wrapper.message().has_control_begin_tick() )
        OnReceiveControlBeginTick( wrapper.message().control_begin_tick().current_tick() );
    else
    {
        if(     wrapper.message().has_log_supply_handling_creation()
            ||  wrapper.message().has_log_supply_handling_destruction()
            ||  wrapper.message().has_log_supply_handling_update() )
        LogMessage( wrapper );
    }
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::OnReceiveMsgAuthenticationToClient
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::OnReceiveMsgAuthenticationToClient( const std::string& /*from*/, const sword::AuthenticationToClient& /*wrapper*/ )
{
    DumpTime();
    file_ << "Authentication received" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::OnReceiveMsgDispatcherToClient
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::OnReceiveMsgDispatcherToClient( const std::string& /*from*/, const sword::DispatcherToClient& /*wrapper*/ )
{
    DumpTime();
    file_ << "Dispatcher message received" << std::endl;
}

// ----------------------------------   -------------------------------------------
// Name: LoggerApplication::OnReceiveMsgMessengerToClient
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::OnReceiveMsgMessengerToClient( const std::string& /*from*/, const sword::MessengerToClient& /*wrapper*/ )
{
    DumpTime();
    file_ << "Messenger message received" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::OnReceiveMsgReplayToClient
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::OnReceiveMsgReplayToClient( const std::string& /*from*/, const sword::ReplayToClient& /*wrapper*/ )
{
    DumpTime();
    file_ << "Replay message received" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::OnReceiveMsgAarToClient
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::OnReceiveMsgAarToClient( const std::string& /*from*/, const sword::AarToClient& /*wrapper*/ )
{
    DumpTime();
    file_ << "Aar message received" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::OnReceiveControlBeginTick
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::OnReceiveControlBeginTick( int tick )
{
    DumpTime();
    file_ << "New tick " << tick << std::endl;
}

// -----------------------------------------------------------------------------
// Name: LoggerApplication::LogMessage
// Created: LDC 2009-09-02
// -----------------------------------------------------------------------------
void LoggerApplication::LogMessage( const sword::SimToClient& wrapper )
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
void LoggerApplication::Send( const sword::ClientToAuthentication& wrapper )
{
    MessageSender_ABC::Send( endpoint_, wrapper );
}

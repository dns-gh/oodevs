// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "LauncherClient.h"
#include "Exercises.h"
#include "LauncherPublisher.h"
#include "clients_kernel/Controller.h"
#include "protocol/LauncherSenders.h"
#include <boost/lexical_cast.hpp>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: LauncherClient constructor
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
LauncherClient::LauncherClient( kernel::Controller& controller )
    : tools::ClientNetworker( "", false )
    , exercises_( new Exercises( controller ) )
    , connected_( false )
{
    RegisterMessage( *this, &LauncherClient::HandleLauncherToAdmin );
}

// -----------------------------------------------------------------------------
// Name: LauncherClient destructor
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
LauncherClient::~LauncherClient()
{
    if( connected_ )
        Disconnect();
}

// -----------------------------------------------------------------------------
// Name: LauncherClient::Connect
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
void LauncherClient::Connect( const std::string& host, unsigned int port )
{
    tools::ClientNetworker::Connect( host + ":" + boost::lexical_cast< std::string >( port ), true );
}

// -----------------------------------------------------------------------------
// Name: LauncherClient::ConnectionSucceeded
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
void LauncherClient::ConnectionSucceeded( const std::string& endpoint )
{
    publisher_.reset( new LauncherPublisher( *this, endpoint ) );
    connected_ = true;
    launcher::ConnectionRequest message;
    message().mutable_client_version()->set_value( Version::ProtocolVersion().value() );
    message.Send( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: LauncherClient::ConnectionError
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
void LauncherClient::ConnectionError( const std::string& address, const std::string& error )
{
    connected_ = false;
    tools::ClientNetworker::ConnectionError( address, error );
}

// -----------------------------------------------------------------------------
// Name: LauncherClient::Connected
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
bool LauncherClient::Connected() const
{
    return connected_;
}

// -----------------------------------------------------------------------------
// Name: LauncherClient::HandleLauncherToAdmin
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
void LauncherClient::HandleLauncherToAdmin( const std::string& /*endpoint*/, const MsgsLauncherToAdmin::MsgLauncherToAdmin& message )
{
    if( message.message().has_connection_ack() )
    {
        // $$$$ SBO 2010-09-30: log error
        if( message.message().connection_ack().error_code() != MsgsLauncherToAdmin::MsgConnectionAck::success )
            Disconnect();
    }
    else if( message.message().has_exercise_list_response() )
        exercises_->Update( message.message().exercise_list_response() );
}

// -----------------------------------------------------------------------------
// Name: LauncherClient::QueryExerciseList
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
void LauncherClient::QueryExerciseList()
{
    if( Connected() )
    {
        launcher::ExercicesListRequest message;
        message.Send( *publisher_ );
    }
}

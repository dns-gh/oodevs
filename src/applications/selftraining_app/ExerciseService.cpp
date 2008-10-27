// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ExerciseService.h"
#include "Config.h"
#include "Multiplayer.h"
#include "clients_kernel/Controllers.h"
#include "frontend/ConfigurationManipulator.h"
#include "frontend/Process_ABC.h"
#include <boost/bind.hpp>
#include <sstream>

using namespace boost::asio;

// -----------------------------------------------------------------------------
// Name: ExerciseService constructor
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
ExerciseService::ExerciseService( kernel::Controllers& controllers, const Config& config )
    : controllers_( controllers )
    , config_     ( config )
    , port_       ( config.GetListServerPort() )
    , socket_     ( network_, ip::udp::endpoint( ip::udp::v4(), port_ ) )
    , thread_     ( boost::bind( &ExerciseService::RunNetwork, this ) )
{
    controllers_.Register( *this );
    SetupNetwork();
}

// -----------------------------------------------------------------------------
// Name: ExerciseService destructor
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
ExerciseService::~ExerciseService()
{
    controllers_.Unregister( *this );
    thread_.join();
}

// -----------------------------------------------------------------------------
// Name: ExerciseService::NotifyCreated
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
void ExerciseService::NotifyCreated( const frontend::Process_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExerciseService::NotifyUpdated
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
void ExerciseService::NotifyUpdated( const frontend::Process_ABC& process )
{
    std::string exerciseName = process.GetStartedExercise();
    if( !exerciseName.empty() && exerciseList_.find( exerciseName ) == exerciseList_.end() )
    {
        frontend::ConfigurationManipulator manipulator( config_, exerciseName, MULTIPLAYER_SESSION );
        exerciseList_[ exerciseName ] = manipulator.GetValue< unsigned int >( "session/config/dispatcher/network/@server" );
    }
    exerciseMessage_.clear();
}
    
// -----------------------------------------------------------------------------
// Name: ExerciseService::NotifyDeleted
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
void ExerciseService::NotifyDeleted( const frontend::Process_ABC& process )
{
    std::string exerciseName = process.GetStartedExercise();
    if( !exerciseName.empty() )
        exerciseList_.erase( exerciseName );
    exerciseMessage_.clear();
}

// -----------------------------------------------------------------------------
// Name: ExerciseService::SetupNetwork
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
void ExerciseService::SetupNetwork()
{
    socket_.async_receive_from( buffer( answer_, 32 ), remoteEndPoint_,
						         boost::bind( &ExerciseService::OnReceive, this, 
									          placeholders::error,
									          placeholders::bytes_transferred ) );
}

// -----------------------------------------------------------------------------
// Name: ExerciseService::RunNetwork
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
void ExerciseService::RunNetwork()
{
    network_.run();
}

// -----------------------------------------------------------------------------
// Name: ExerciseService::OnReceive
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
void ExerciseService::OnReceive( const boost::system::error_code& error, size_t bytes_received )
{
    if( !error )
    {
        if( exerciseMessage_.empty() )
        {
            std::stringstream stream;
            for( CIT_ExercicePortList it = exerciseList_.begin(); it != exerciseList_.end(); ++it )
                stream << it->first << ":" << it->second<<"/";
            exerciseMessage_ = stream.str();
        }
        socket_.async_send_to( buffer( exerciseMessage_ ), remoteEndPoint_, boost::bind( &ExerciseService::OnSendExercisesRequest, this, placeholders::error ) );
    }
    socket_.async_receive_from( buffer( answer_, 32 ), remoteEndPoint_,
					         boost::bind( &ExerciseService::OnReceive, this, 
								          placeholders::error,
								          placeholders::bytes_transferred ) );
}

// -----------------------------------------------------------------------------
// Name: ExerciseService::OnSendExercisesRequest
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
void ExerciseService::OnSendExercisesRequest( const boost::system::error_code& error )
{
    // NOTHING
}

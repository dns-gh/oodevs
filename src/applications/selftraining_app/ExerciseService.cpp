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
#pragma warning( push )
#pragma warning( disable: 4512 )
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#pragma warning( pop )
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
    , socket_     ( 0 )
    , thread_     ( 0 )
{
    try
    {
        socket_.reset( new boost::asio::ip::udp::socket( network_, ip::udp::endpoint( ip::udp::v4(), port_ ) ) );
        thread_.reset( new boost::thread( boost::bind( &ExerciseService::RunNetwork, this ) ) );
        SetupNetwork();
    }
    catch( ... )
    {
        // $$$$ SBO 2008-10-31: throw something to warn user
    }
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ExerciseService destructor
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
ExerciseService::~ExerciseService()
{
    controllers_.Unregister( *this );
    network_.stop();
    if( thread_.get() )
        thread_->join();
}

// -----------------------------------------------------------------------------
// Name: ExerciseService::NotifyCreated
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
void ExerciseService::NotifyCreated( const boost::shared_ptr< frontend::Process_ABC >& process )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExerciseService::NotifyUpdated
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
void ExerciseService::NotifyUpdated( const boost::shared_ptr< frontend::Process_ABC >& process )
{
    const std::string exerciseName = process->GetStartedExercise();
    if( !exerciseName.empty() && exerciseList_.find( exerciseName ) == exerciseList_.end() )
    {
        try
        {
            frontend::ConfigurationManipulator manipulator( config_, exerciseName, MULTIPLAYER_SESSION );
            exerciseList_[ exerciseName ] = manipulator.GetValue< unsigned int >( "session/config/dispatcher/network/@server" );
            runningProcesses_[ exerciseName ] = process;
        }
        catch( ... )
        {
            // $$$$ SBO 2008-10-30: multiplayer session does not exists
        }
    }
    exerciseMessage_.clear();
}
    
// -----------------------------------------------------------------------------
// Name: ExerciseService::NotifyDeleted
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
void ExerciseService::NotifyDeleted( const boost::shared_ptr< frontend::Process_ABC >& process )
{
    const std::string exerciseName = process->GetStartedExercise();
    if( !exerciseName.empty() )
    {
        exerciseList_.erase( exerciseName );
        runningProcesses_.erase( exerciseName );
    }
    exerciseMessage_.clear();
}

// -----------------------------------------------------------------------------
// Name: ExerciseService::SetupNetwork
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
void ExerciseService::SetupNetwork()
{
    socket_->async_receive_from( buffer( answer_, 32 ), remoteEndPoint_,
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
                stream << it->first << ":" << it->second << "/";
            exerciseMessage_ = stream.str();
        }
        socket_->async_send_to( buffer( exerciseMessage_ ), remoteEndPoint_, boost::bind( &ExerciseService::OnSendExercisesRequest, this, placeholders::error ) );
    }
    socket_->async_receive_from( buffer( answer_, 32 ), remoteEndPoint_,
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

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 MASA Group
//
// *****************************************************************************

#include "simulation_app_pch.h"
#include "SIM_App.h"
#include "resource.h"
#include "dispatcher/DispatcherLoader.h"
#include "FileLoaderObserver.h"
#include "MT_Tools/MT_FileLogger.h"
#include "MT_Tools/MT_Logger.h"
#include "simulation_kernel/MIL_AgentServer.h"
#include "simulation_kernel/MIL_Random.h"
#include "simulation_kernel/Tools/MIL_Config.h"
#include "tools/WinArguments.h"
#include <tools/WaitEvent.h>
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>

SIM_App::SIM_App( int maxConnections )
    : tools::Application( IDI_ICON1, IDI_ICON2 )
    , maxConnections_( maxConnections )
    , config_( new MIL_Config( boost::make_shared< FileLoaderObserver >() ) )
{
    config_->Parse( arguments_->Argc(), const_cast< char** >( arguments_->Argv() ) );
    logger_.reset( new MT_FileLogger( config_->BuildSessionChildFile( "Sim.log" ),
        config_->GetSimLogFiles(), config_->GetSimLogSize(),
        config_->GetSimLogLevel(), !config_->HasCheckpoint(),
        MT_Logger_ABC::eSimulation, config_->IsSimLogInBytes() ) );
    MT_LOG_REGISTER_LOGGER( *logger_ );
}

SIM_App::~SIM_App()
{
    quit_->Signal();
    if( dispatcher_ )
        dispatcher_->join();
    MIL_AgentServer::DestroyWorkspace();
    MT_LOG_UNREGISTER_LOGGER( *logger_ );
}

void SIM_App::RunDispatcher()
{
    try
    {
        dispatcher::DispatcherLoader loader( arguments_->Argc(),
            const_cast< char** >( arguments_->Argv() ), maxConnections_ );
        while( !quit_->IsSignaled() )
        {
            if( !loader.Update() )
                throw MASA_EXCEPTION( "An error occured in dispatcher, see dispatcher.log for more information" );
            quit_->Wait( boost::posix_time::milliseconds( 25 ) );
        }
    }
    catch( const std::exception& e )
    {
        MT_LOG_FATAL_ERROR_MSG( "dispatcher: " << tools::GetStackTraceAndMessage( e ) );
        result_ = EXIT_FAILURE;
    }
    quit_->Signal();
}

void SIM_App::Initialize()
{
    Application::Initialize( "Simulation" );
    MT_LOG_INFO_MSG( "Session: " << config_->GetSessionDir() );
    if( config_->IsDispatcherEmbedded() )
    {
        MT_LOG_INFO_MSG( "Starting embedded dispatcher" );
        dispatcher_.reset( new boost::thread( [&]() { RunDispatcher(); } ) );
    }
    MIL_Random::Initialize(
        config_->GetRandomSeed(), config_->GetRandomGaussian(),
        config_->GetRandomDeviation(), config_->GetRandomMean() );
    MIL_AgentServer::CreateWorkspace( *config_ );
}

bool SIM_App::Update()
{
    return MIL_AgentServer::GetWorkspace().Update();
}

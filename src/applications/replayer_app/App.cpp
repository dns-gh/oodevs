// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 MASA Group
//
// *****************************************************************************

#include "App.h"
#include "resource.h"
#include "dispatcher/Config.h"
#include "dispatcher/Replayer.h"
#include "MT_Tools/MT_FileLogger.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/WinArguments.h"
#include <tools/WaitEvent.h>

using namespace dispatcher;

App::App( bool replayLog )
    : tools::Application( IDI_ICON1, IDI_ICON2, WM_QUIT )
    , config_( new dispatcher::Config() )
    , test_( arguments_->HasOption( "--test" ) )
{
    config_->Parse( arguments_->Argc(), arguments_->Argv() );
    if( replayLog )
    {
        logger_.reset( new MT_FileLogger(
            config_->BuildSessionChildFile( "Replayer.log" ),
            config_->GetSimLogFiles(), config_->GetSimLogSize(),
            config_->GetSimLogLevel(), true ) );
        MT_LOG_REGISTER_LOGGER( *logger_ );
    }
}

App::~App()
{
    MT_LOG_UNREGISTER_LOGGER( *logger_ );
}

void App::Initialize()
{
    Application::Initialize( "Replayer" );
    MT_LOG_INFO_MSG( "Session: " << config_->GetSessionDir() );
    replayer_.reset( new Replayer( *config_ ) );
}

bool App::Update()
{
    if( !replayer_->Update() || test_ )
        return false;
    quit_->Wait( boost::posix_time::milliseconds( 10 ) );
    return true;
}

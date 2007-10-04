// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "App.h"
#include "dispatcher/Replayer.h"
#include "MT/MT_Logger/MT_Logger_Lib.h"

#include "dispatcher/Loader.h"
#include "dispatcher/PluginConfig.h"

#include <windows.h> //$$$ A VIRER

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: App constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
App::App( int argc, char** argv )
    : replayer_( 0 )
{
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_STARTUP_MESSAGE( "CSword(tm) Replayer" );
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );

    config_.Parse( argc, argv );

    const std::string record = config_.GetRecord();
    MT_LOG_INFO_MSG( "Loading record " << record );
    replayer_ = new Replayer( config_, record );
}

// -----------------------------------------------------------------------------
// Name: App destructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
App::~App()
{
    delete replayer_;
}

// -----------------------------------------------------------------------------
// Name: App::Execute
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void App::Execute()
{
    try
    {
        while( 1 )
        {
            ::Sleep( 10 );
            replayer_->Update();
        }
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Replayer error : " << e.what() );
    }
}

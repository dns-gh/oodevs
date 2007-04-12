// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MyFirstApplication.h"
#include <string> // $$$$ AGE 2007-04-10: 
#include "dispatcher/Config.h"
#include "dispatcher/Replayer.h"
#include "MT/MT_Logger/MT_Logger_Lib.h"

#include <windows.h> //$$$ A VIRER

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MyFirstApplication constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
MyFirstApplication::MyFirstApplication( int argc, char** argv )
{
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_STARTUP_MESSAGE( "CSword(tm) Replayer" );
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );

    Config config;
    config.Parse( argc, argv );
    replayer_ = new Replayer( config );
}

// -----------------------------------------------------------------------------
// Name: MyFirstApplication destructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
MyFirstApplication::~MyFirstApplication()
{
    delete replayer_;
}

// -----------------------------------------------------------------------------
// Name: MyFirstApplication::Execute
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void MyFirstApplication::Execute()
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

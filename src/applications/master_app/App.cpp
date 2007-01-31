// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "App.h"

#include <cassert>
#include "master/Master.h"
#include "master/Config.h"
#include "MT/MT_Logger/MT_Logger_Lib.h"

#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/program_options.hpp>
#pragma warning( pop )

#include <windows.h> //$$$ A VIRER

using namespace master;
namespace po = boost::program_options;

// -----------------------------------------------------------------------------
// Name: App constructor
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
App::App( int argc, char** argv )
    : pMaster_ ( 0 )
{
    std::string strMsg = "CSword(tm) Master";
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_STARTUP_MESSAGE( strMsg.c_str() );
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );

    Config config;
    config.Parse( argc, argv );
    pMaster_ = new Master( config );
}

// -----------------------------------------------------------------------------
// Name: App destructor
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
App::~App()
{
    delete pMaster_;
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: App::Run
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
int App::Execute()
{
    assert( pMaster_ );
    try
    {
        while( 1 )
        {
            ::Sleep( 10 );
            pMaster_->Update();
        }
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Initialization error : " << e.what() );
    }
    return 0;
}


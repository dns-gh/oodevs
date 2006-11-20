// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "App.h"

#include <cassert>
#include "dispatcher/Dispatcher.h"
#include "MT/MT_Logger/MT_Logger_Lib.h"

#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/program_options.hpp>
#pragma warning( pop )

#include <windows.h> //$$$ A VIRER

using namespace dispatcher;
namespace po = boost::program_options;

// -----------------------------------------------------------------------------
// Name: App constructor
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
App::App( int argc, char** argv )
    : strConfFile_( )
    , pDispatcher_( 0 )
{
//    std::string strMsg = "Sword(tm) Dispatcher - " VERSION " - " MT_COMPILE_TYPE " - " __TIMESTAMP__;
    std::string strMsg = "Sword(tm) Dispatcher";
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_STARTUP_MESSAGE( strMsg.c_str() );
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );

    ParseCmdArgs( argc, argv );

    pDispatcher_ = new Dispatcher( strConfFile_ );
}

// -----------------------------------------------------------------------------
// Name: App destructor
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
App::~App()
{
    delete pDispatcher_;
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
    assert( pDispatcher_ );
    try
    {
        while( 1 )
        {
            ::Sleep( 10 );
            pDispatcher_->Update();
        }
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Initialization error : " << e.what() );
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: App::ParseCmdArgs
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
void App::ParseCmdArgs( int argc, char** argv )
{
    po::options_description desc( "Allowed options" );
    desc.add_options()
        ( "conffile,c", po::value< std::string >( &strConfFile_ )->default_value( "./scipio.xml" ), "specify main config file (scipio.xml)" )
    ;
    po::variables_map vm;
    po::store( po::parse_command_line( argc, argv, desc ), vm );
    po::notify( vm );
}

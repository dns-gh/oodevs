// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Application.h"
#include "MT_Tools/MT_Logger.h"
#include <windows.h>
#include <boost/preprocessor/stringize.hpp>

#ifndef PLATFORM
#error PLATFORM must be defined (for instance vc80 or vc100_x64) for dispatcher
#endif

#ifdef _DEBUG
# define EXTENSION "-gd"
#else
# define EXTENSION ""
#endif

// -----------------------------------------------------------------------------
// Name: Application constructor
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
Application::Application( int argc, char** argv, int maxConnections )
{
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_STARTUP_MESSAGE( "Sword Officer Training(tm) Dispatcher" );
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    HMODULE module = LoadLibrary( ( "dispatcher" + std::string(  "-" BOOST_PP_STRINGIZE( PLATFORM ) "-mt" EXTENSION ".dll" ) ).c_str() );
    facadeCreator_ = (T_FacadeCreator) GetProcAddress( module, "CreateDispatcherFacade" );
    facadeUpdator_ = (T_FacadeUpdator) GetProcAddress( module, "UpdateDispatcherFacade" );
    facadeDestructor_= (T_FacadeDestructor) GetProcAddress( module, "DestroyDispatcherFacade" );
    dispatcher_ = facadeCreator_( argc, argv, maxConnections );
}

// -----------------------------------------------------------------------------
// Name: App destructor
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
Application::~Application()
{
    facadeDestructor_( dispatcher_ );
}

#pragma warning( disable : 4127 ) //conditional expression is constant

// -----------------------------------------------------------------------------
// Name: Application::Run
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
int Application::Execute()
{
    try
    {
        while( 1 )
        {
            ::Sleep( 10 );
            facadeUpdator_( dispatcher_ );
        }
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Initialization error : " << e.what() );
    }
    return 0;
}

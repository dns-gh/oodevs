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
# define DEBUG_EXTENSION "-gd"
#else
# define DEBUG_EXTENSION ""
#endif

#define EXTENSION "-" BOOST_PP_STRINGIZE( PLATFORM ) "-mt" DEBUG_EXTENSION ".dll"

class Application::DispatcherFacade : private boost::noncopyable
{
public:
    DispatcherFacade( int argc, char** argv, int maxConnections )
        : module_          ( LoadLibrary( "dispatcher" EXTENSION ) )
        , facadeCreator_   ( LoadFunction< T_FacadeCreator >( "CreateDispatcherFacade" ) )
        , facadeDestructor_( LoadFunction< T_FacadeDestructor >( "DestroyDispatcherFacade" ) )
        , facadeUpdator_   ( LoadFunction< T_FacadeUpdator >( "UpdateDispatcherFacade" ) )
        , dispatcher_      ( facadeCreator_( argc, argv, maxConnections ) )
    {
        if( !dispatcher_ )
            throw std::runtime_error( "failed to create dispatcher" );
    }
    ~DispatcherFacade()
    {
        facadeDestructor_( dispatcher_ );
        FreeModule( module_ );
    }
    void Update()
    {
        facadeUpdator_( dispatcher_ );
    }
    template< typename Function >
    Function LoadFunction( const std::string& name ) const
    {
        if( !module_ )
            throw std::runtime_error( "failed to load dispatcher" EXTENSION );
        Function fun = reinterpret_cast< Function >( GetProcAddress( module_, name.c_str() ) );
        if( !fun )
            throw std::runtime_error( "failed to load function '" + name + "' in dispatcher" EXTENSION );
        return fun;
    }
private:
    typedef void* ( *T_FacadeCreator )( int argc, char** argv, int maxConnections );
    typedef void ( *T_FacadeDestructor )( void* facade );
    typedef void ( *T_FacadeUpdator )( void* facade );

    HMODULE module_;
    T_FacadeCreator facadeCreator_;
    T_FacadeDestructor facadeDestructor_;
    T_FacadeUpdator facadeUpdator_;
    void* dispatcher_;
};

// -----------------------------------------------------------------------------
// Name: Application constructor
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
Application::Application( int argc, char** argv, int maxConnections )
{
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_STARTUP_MESSAGE( "Sword Officer Training(tm) Dispatcher" );
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    dispatcher_.reset( new DispatcherFacade( argc, argv, maxConnections ) );
}

// -----------------------------------------------------------------------------
// Name: App destructor
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
Application::~Application()
{
    // NOTHING
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
            dispatcher_->Update();
        }
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Initialization error : " << e.what() );
    }
    return 0;
}

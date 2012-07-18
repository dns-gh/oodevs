// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_app_pch.h"
#include "SIM_Dispatcher.h"
#include "MT_Tools/MT_Logger.h"
#pragma warning( push, 0 )
#include <boost/thread.hpp>
#pragma warning( pop )
#include <windows.h>

#ifndef PLATFORM
#error PLATFORM must be defined (for instance vc80 or vc100_x64) for dispatcher
#endif

#ifdef _DEBUG
# define DEBUG_EXTENSION "-gd"
#else
# define DEBUG_EXTENSION ""
#endif

#define EXTENSION "-" BOOST_PP_STRINGIZE( PLATFORM ) "-mt" DEBUG_EXTENSION ".dll"

class SIM_Dispatcher::DispatcherFacade : private boost::noncopyable
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
// Name: SIM_Dispatcher constructor
// Created: NLD 2006-10-04
// -----------------------------------------------------------------------------
SIM_Dispatcher::SIM_Dispatcher( int argc, char** argv, int maxConnections )
    : running_   ( true )
    , dispatcher_( new DispatcherFacade( argc, argv, maxConnections ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SIM_Dispatcher destructor
// Created: NLD 2006-10-04
// -----------------------------------------------------------------------------
SIM_Dispatcher::~SIM_Dispatcher()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SIM_Dispatcher::Run
// Created: AGE 2004-02-05
// -----------------------------------------------------------------------------
void SIM_Dispatcher::Run()
{
    while( running_ ) // $$$$ MCO : should be protected with a mutex
    {
        dispatcher_->Update();
        boost::this_thread::sleep( boost::posix_time::milliseconds( 25 ) ) ;
    }
}

// -----------------------------------------------------------------------------
// Name: SIM_Dispatcher::Stop
// Created: RDS 2008-07-24
// -----------------------------------------------------------------------------
void SIM_Dispatcher::Stop()
{
    running_ = false ; // $$$$ MCO : should be protected with a mutex
}

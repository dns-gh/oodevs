// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#include "Runtime.h"
#include "Process.h"
#include "Handle.h"
#include "Api_ABC.h"
#include <runtime/Utf8.h>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/ref.hpp>
#include <boost/algorithm/string/join.hpp>

using namespace runtime;

namespace
{
    // -----------------------------------------------------------------------------
    // Name: Enumerate
    // Created: BAX 2012-03-07
    // -----------------------------------------------------------------------------
    template< typename T >
    bool Enumerate( const Api_ABC& api, Runtime::T_Processes& list, std::vector< T >& pids )
    {
        DWORD size = 0;
        bool done = api.EnumProcesses( &pids[0], static_cast< DWORD >( pids.size() * sizeof T ), &size );
        if( !done )
            return true;
        if( size == pids.size() * sizeof T )
            return false;
        pids.resize( size / sizeof size );
        BOOST_FOREACH( const T& id, pids )
            try
            {
                list.push_back( boost::make_shared< Process >( api, id ) );
            }
            catch( const std::exception& /*err*/ )
            {
                // LOG_ERROR
                continue;
            }
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: Runtime::Runtime
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
Runtime::Runtime( const Api_ABC& api )
    : api_( api )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Runtime::~Runtime
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
Runtime::~Runtime()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Runtime::Processes
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Runtime::T_Processes Runtime::GetProcesses() const
{
    T_Processes list;
    std::vector< DWORD > pids( 128, 0 );
    while( !Enumerate( api_, list, pids ) )
        pids.resize( pids.size() * 2 );
    return list;
}

// -----------------------------------------------------------------------------
// Name: Runtime::GetProcess
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
boost::shared_ptr< Process_ABC > Runtime::GetProcess( int pid ) const
{
    try
    {
        return boost::make_shared< Process >( api_, pid );
    }
    catch( const std::exception& /*err*/ )
    {
        // LOG_ERROR
        return boost::shared_ptr< Process_ABC >();
    }
}

namespace
{
    // -----------------------------------------------------------------------------
    // Name: CreateProcess
    // Created: BAX 2012-03-07
    // -----------------------------------------------------------------------------
    boost::shared_ptr< Process_ABC > CreateProcess( const Api_ABC& api,
                                                    const std::wstring& app,
                                                    std::vector< wchar_t >& args,
                                                    const std::wstring& run )
    {
        STARTUPINFOW startup = { sizeof startup, };
        PROCESS_INFORMATION info = {};

        bool done = api.CreateProcess( app.c_str(), &args[0], 0, 0, false,
                        NORMAL_PRIORITY_CLASS, 0, run.empty() ? 0 : run.c_str(),
                        &startup, &info );
        if( info.hThread )
            api.CloseHandle( info.hThread );
        std::auto_ptr< Handle > handle( new Handle( api, info.hProcess ) );
        if( !done )
            return boost::shared_ptr< Process_ABC >();

        return boost::make_shared< Process >( api, info.dwProcessId, boost::ref( handle ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Runtime::Start
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
boost::shared_ptr< Process_ABC > Runtime::Start( const std::string& cmd,
                                                 const std::vector< std::string >& args,
                                                 const std::string& run ) const
{
    std::wstring wapp, wrun;
    std::vector< wchar_t > wcmd;
    try
    {
        wapp = Utf8Convert( cmd );
        const std::wstring join = Utf8Convert( boost::algorithm::join( args, " " ) );
        std::copy( join.begin(), join.end(), std::back_inserter( wcmd ) );
        wcmd.push_back( 0 );
        wrun = Utf8Convert( run );
    }
    catch( const std::runtime_error& )
    {
        return boost::shared_ptr< Process_ABC >();
    }
    return CreateProcess( api_, wapp, wcmd, wrun );
}

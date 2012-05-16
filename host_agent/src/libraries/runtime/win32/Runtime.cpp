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
#include "Api_ABC.h"
#include <runtime/Utf8.h>
#include <cpplog/cpplog.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/ref.hpp>
#include <boost/algorithm/string/join.hpp>

#include <boost/bind.hpp>
#include <boost/function.hpp>


using namespace runtime;

namespace
{
template< typename T >
bool Enumerate( cpplog::BaseLogger& log, const Api_ABC& api, Runtime::T_Processes& list, std::vector< T >& pids )
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
        catch( const std::exception& err )
        {
            LOG_WARN( log ) << "[runtime] Unable to load process " << id << ", " << err.what();
            continue;
        }
    return true;
}
}

// -----------------------------------------------------------------------------
// Name: Runtime::Runtime
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
Runtime::Runtime( cpplog::BaseLogger& log, const Api_ABC& api )
    : log_( log )
    , api_( api )
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
    while( !Enumerate( log_, api_, list, pids ) )
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
    catch( const std::exception& err )
    {
        LOG_WARN( log_ ) << "[runtime] Unable to load process " << pid << ", " << err.what();
        return boost::shared_ptr< Process_ABC >();
    }
}

namespace
{
boost::shared_ptr< Process_ABC > MakeProcess( const Api_ABC& api,
                                              const std::wstring& app,
                                              std::vector< wchar_t >& args,
                                              const std::wstring& run,
                                              const std::wstring& log )
{
    int pid;
    if( !log.empty() )
        boost::filesystem::create_directories( boost::filesystem::path( log ).remove_filename() );
    HANDLE reply = api.MakeProcess( app.c_str(), &args[0], run.empty() ? 0 : run.c_str(), log.empty() ? 0 : log.c_str(), pid );
    if( !reply )
        throw std::runtime_error( "unable to create process" );
    Handle handle = MakeHandle( api, reply );
    return boost::make_shared< Process >( api, pid, handle );
}
}

// -----------------------------------------------------------------------------
// Name: Runtime::Start
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
boost::shared_ptr< Process_ABC > Runtime::Start( const std::string& cmd,
                                                 const std::vector< std::string >& args,
                                                 const std::string& run,
                                                 const std::string& log ) const
{
    try
    {
        std::vector< wchar_t > wcmd;
        const std::wstring join = Utf8Convert( boost::algorithm::join( args, " " ) );
        std::copy( join.begin(), join.end(), std::back_inserter( wcmd ) );
        wcmd.push_back( 0 );
        return MakeProcess( api_, Utf8Convert( cmd ), wcmd, Utf8Convert( run ), Utf8Convert( log ) );
    }
    catch( const std::runtime_error& err )
    {
        LOG_WARN( log_ ) << "[runtime] Unable to start process " << cmd << " " << boost::algorithm::join( args, " " ) << ", " << err.what();
        return boost::shared_ptr< Process_ABC >();
    }
}

// -----------------------------------------------------------------------------
// Name: Runtime::GetModuleFilename
// Created: BAX 2012-05-09
// -----------------------------------------------------------------------------
boost::filesystem::path Runtime::GetModuleFilename() const
{
    return api_.GetModuleFilename();
}

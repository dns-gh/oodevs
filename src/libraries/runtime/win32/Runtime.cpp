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
#include <tools/Helpers.h>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/ref.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

using namespace runtime;

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
    if( !log.empty() )
        boost::filesystem::create_directories( boost::filesystem::path( log ).remove_filename() );
    ProcessDescriptor proc = api.MakeProcess( app.c_str(), &args[0], run.empty() ? 0 : run.c_str(), log.empty() ? 0 : log.c_str() );
    if( !proc.handle )
        throw std::runtime_error( "unable to create process" );
    Handle handle = MakeHandle( api, proc.handle );
    return boost::make_shared< Process >( api, proc.pid, handle );
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
        std::vector< std::wstring > wargs;
        wargs.push_back( Utf8( cmd ) );
        for( auto it = args.begin(); it != args.end(); ++it )
            wargs.push_back( Utf8( *it ) );
        auto wcmd = tools::ArgsToCommandLine( wargs );
        return MakeProcess( api_, Utf8( cmd ), wcmd, Utf8( run ), Utf8( log ) );
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

// -----------------------------------------------------------------------------
// Name: Runtime::GetLastError
// Created: BAX 2014-02-12
// -----------------------------------------------------------------------------
std::string Runtime::GetLastError() const
{
    return api_.GetLastError();
}
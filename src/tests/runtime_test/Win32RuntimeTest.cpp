// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "runtime_test.h"

#include <runtime/Utf8.h>
#include <runtime/win32/Api.h>
#include <runtime/win32/Process.h>
#include <runtime/win32/Runtime.h>
#include <cpplog/cpplog.hpp>

#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <iostream>
#include <ctime>

using namespace runtime;

BOOST_AUTO_TEST_CASE( runtime_process_gets )
{
    cpplog::OstreamLogger log( std::cout );
    Api api( log );
    Runtime runtime( log, api );
    // Check itself
    int pid = api.GetCurrentProcessIdentifier();
    boost::shared_ptr< Process_ABC > ptr = runtime.GetProcess( pid );
    BOOST_CHECK_EQUAL( ptr->GetPid(), pid );
    BOOST_CHECK(ptr->GetName().find("runtime_test") != std::string::npos );
}

namespace
{

struct FileDeleter : boost::noncopyable
{
     FileDeleter( const boost::filesystem::path& p ): path( p ) {}
    ~FileDeleter()
    {
        try
        {
            boost::filesystem::remove(path);
        }
        catch( const std::exception& )
        {
        }
    }

    const boost::filesystem::path path;
};

}  //namespace

BOOST_AUTO_TEST_CASE( runtime_process_starts )
{
    cpplog::OstreamLogger log( std::cout );
    Api api( log );
    Runtime runtime( log, api );

    // Check the test executable is here
    const tools::Path app = testOptions.GetDataPath( "res/echowin32.exe" );
    boost::system::error_code err;
    if( !app.IsRegularFile() )
        BOOST_FAIL("Cannot find echowin32.exe, please set --data_directory to the project root: "
            + app.ToDebug() );

    // Call it with random arguments
    std::string randarg = boost::lexical_cast< std::string >( std::time( NULL ));
    const std::vector< std::string > args
        = boost::assign::list_of( "hello" )( randarg.c_str() );
    const std::string dir = boost::filesystem::path( runtime.GetModuleFilename() )
        .parent_path().string();
    FileDeleter logDeleter(dir / boost::filesystem::path( "runtime_process_starts" + randarg + ".log" ));
    const std::string logs = logDeleter.path.string();
    boost::shared_ptr< Process_ABC > p = runtime.Start( app.ToUTF8(), args, dir, logs );
    BOOST_REQUIRE( p );
    BOOST_CHECK_GT( p->GetPid(), 0 );
    p->Join( 5000 );

    // Check the redirection output
    std::fstream f( logs.c_str(), std::ios::in );
    std::string line;
    BOOST_CHECK( std::getline( f, line ));
    BOOST_CHECK_EQUAL( line, "hello" + randarg );
}

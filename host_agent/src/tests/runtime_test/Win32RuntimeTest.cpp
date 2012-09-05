// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "runtime_test.h"

#include <runtime/Utf8.h>
#include <runtime/win32/Api.h>
#include <runtime/win32/Process.h>
#include <runtime/win32/Runtime.h>
#include <cpplog/cpplog.hpp>

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
    unsigned long pid = api.GetCurrentProcessIdentifier();
    boost::shared_ptr< Process_ABC > ptr = runtime.GetProcess( pid );
    BOOST_CHECK_EQUAL( ptr->GetPid(), pid );
    BOOST_CHECK(ptr->GetName().find("runtime_test") != std::string::npos );
}

BOOST_AUTO_TEST_CASE( runtime_process_starts )
{
    cpplog::OstreamLogger log( std::cout );
    Api api( log );
    Runtime runtime( log, api );

    // Check the test executable is here
    const std::string app = BOOST_RESOLVE( "res/echowin32.exe" );
    boost::system::error_code err;
    if( !boost::filesystem::is_regular_file( app, err ))
    BOOST_FAIL("Cannot find echowin32.exe, please set --data_directory to the project root: "
        + app );

    // Call it with random arguments
    std::string randarg = boost::lexical_cast< std::string >( std::time( NULL ));
    const std::vector< std::string > args
        = boost::assign::list_of( "hello" )( randarg.c_str() );
    const std::string dir = boost::filesystem::path( runtime.GetModuleFilename() )
        .parent_path().string();
    const std::string logs =
        (dir / boost::filesystem::path( "runtime_process_starts.log" )).string();
    boost::shared_ptr< Process_ABC > p = runtime.Start( app, args, dir, logs );
    BOOST_CHECK( p );
    BOOST_CHECK_GT( p->GetPid(), 0 );
    p->Join( 5000 );

    // Check the redirection output
    std::fstream f( logs.c_str(), std::ios::in );
    std::string line;
    BOOST_CHECK( std::getline( f, line ));
    BOOST_CHECK_EQUAL( line, "hello" + randarg );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "runtime_test.h"
#include "Win32MockApi.h"

#include <runtime/Utf8.h>
#include <runtime/win32/Api.h>
#include <runtime/win32/Process.h>
#include <runtime/win32/Runtime.h>
#include <cpplog/cpplog.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <iostream>
#include <ctime>

using namespace runtime;
using test::MockApi;

namespace
{
    void* const dummy = reinterpret_cast< void* >( 0xCAFEBABE );
    const std::wstring wname = L"Zebulon";

    class StdoutLogger: public cpplog::BaseLogger
    {
        virtual bool sendLogMessage( cpplog::LogData* logData )
        {
            std::cout << logData->stream.str() << std::flush;
            return true;
        }
    };

    bool FakeEnumProcesses( unsigned long* ids, int cb, unsigned long* pBytesReturned, int size )
    {
        size = std::min< int >( size, cb / sizeof *ids );
        for( int i = 0; i < size; ++i )
            ids[i] = i + 1;
        BOOST_REQUIRE( pBytesReturned );
        *pBytesReturned = size * sizeof *ids;
        return true;
    }

    struct PtrPrinter
    {
        explicit PtrPrinter(void* ptr)
            : ptr_( ptr )
        {
            // NOTHING
        }
        friend std::ostream& operator<<( std::ostream& sink, PtrPrinter printer )
        {
            return sink << printer.ptr_;
        }
        private:
        void* ptr_;
    };

    int FakeGetProcessName( void* handle, wchar_t* dst, int size, void* dummy )
    {
        BOOST_CHECK_EQUAL( handle, dummy );
        std::string name = boost::lexical_cast< std::string >( PtrPrinter( dummy ) );
        std::wstring wname = Utf8Convert( name );
        int count = std::min( size, static_cast< int >( wname.size() ) );
        std::copy( wname.begin(), wname.begin() + count, dst );
        dst[count] = 0;
        return count + 1;
    }

    bool StringCompare( const wchar_t* actual, const std::string& expected )
    {
        return Utf8Convert( std::wstring( actual ) ) == expected;
    }

    void ExpectOpenProcess( MockApi& api, void* handle, int pid )
    {
        MOCK_EXPECT( api.OpenProcess ).once().with( mock::any, false, pid ).returns( handle );
        MOCK_EXPECT( api.GetProcessName ).once().calls( boost::bind( &FakeGetProcessName, _1, _2, _3, handle ) );
        MOCK_EXPECT( api.CloseHandle ).once().with( handle ).returns( true );
    }

    void CheckProcess( const Process_ABC& process, int pid, void* handle )
    {
        BOOST_CHECK_EQUAL( process.GetPid(),  pid );
        BOOST_CHECK_EQUAL( process.GetName(), boost::lexical_cast< std::string >( PtrPrinter( handle ) ) );
    }
}

BOOST_AUTO_TEST_CASE( runtime_process_lists )
{
    StdoutLogger log;
    MockApi api;
    Runtime runtime( log, api );
    int size = 64;
    MOCK_EXPECT( api.EnumProcesses ).once().calls( boost::bind( &FakeEnumProcesses, _1, _2, _3, size ) );
    for( int i = 0; i < size; ++i )
        ExpectOpenProcess( api, reinterpret_cast< void* >( 0xDEADBEEF + i ), i + 1 );
    Runtime::T_Processes list = runtime.GetProcesses();
    BOOST_CHECK_EQUAL( static_cast< size_t >( size ), list.size() );
    int idx = size >> 1;
    CheckProcess( *list[ idx ], idx + 1, reinterpret_cast< void* >( 0xDEADBEEF + idx ) );
}

BOOST_AUTO_TEST_CASE( runtime_process_gets )
{
    StdoutLogger log;
    MockApi api;
    Runtime runtime( log, api );
    const int pid = 1337;
    ExpectOpenProcess( api, dummy, pid );
    boost::shared_ptr< Process_ABC > ptr = runtime.GetProcess( pid );
    CheckProcess( *ptr, pid, dummy );
}


BOOST_AUTO_TEST_CASE( runtime_process_starts )
{
    StdoutLogger log;
    Api api( log );
    Runtime runtime( log, api );

    // Check the test executable is here
    const std::string app = BOOST_RESOLVE("res/echowin32.exe");
    boost::system::error_code err;
    if( !boost::filesystem::is_regular_file( boost::filesystem::path( app ), err ))
    BOOST_FAIL("Cannot find echowin32.exe, please set --data_directory to the project root: "
    	    + app );

    // Call it with random arguments
    std::string randarg = boost::lexical_cast<std::string>( std::time( NULL ));
    const std::vector< std::string > args
        = boost::assign::list_of( "hello" )( randarg.c_str() );
    const std::string dir = boost::filesystem::path( runtime.GetModuleFilename() )
        .parent_path().string();
    const std::string logs =
        (dir / boost::filesystem::path( "runtime_process_starts.log" )).string();
    boost::shared_ptr< Process_ABC > p = runtime.Start( app, args, dir, logs );
    BOOST_CHECK(p);
    BOOST_CHECK_GT(p->GetPid(), 0);
    p->Join(5000);

    // Check the redirection output
    std::fstream f(logs, std::ios::in);
    std::string line;
    BOOST_CHECK(std::getline(f, line));
    BOOST_CHECK_EQUAL(line, "hello" + randarg);
}

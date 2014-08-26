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
#include <runtime/win32/Api_ABC.h>
#include <runtime/win32/Process.h>
#include <boost/bind.hpp>

#define  NOMINMAX
#include <windows.h>

using namespace runtime;
using test::MockApi;

namespace
{
    HANDLE MakeHandle( intptr_t value )
    {
        return reinterpret_cast< HANDLE >( value );
    }

    const HANDLE dummy  = MakeHandle( 0xCAFEBABE );
    const HANDLE thread = MakeHandle( 0xBAADF00D );
    const std::wstring wname = L"Zebulon";

    int FakeGetProcessName( HANDLE handle, wchar_t* name, int size )
    {
        BOOST_CHECK_EQUAL( handle, dummy );
        int count = std::min( size, static_cast< int >( wname.size() ) );
        std::copy( wname.begin(), wname.begin() + count, name );
        name[count] = 0;
        return count + 1;
    }

    std::unique_ptr< Process > MakeProcess( const MockApi& api, int pid, const std::wstring& /*wname*/ )
    {
        MOCK_EXPECT( api.OpenProcess ).once().with( mock::any, false, pid ).returns( dummy );
        MOCK_EXPECT( api.GetProcessName ).once().calls( boost::bind( &FakeGetProcessName, _1, _2, _3 ) );
        MOCK_EXPECT( api.CloseHandle ).once().with( dummy ).returns( true );
        return std::unique_ptr< Process >( new Process( api, pid ) );
    }

    bool FakeGetExitCodeProcess( HANDLE handle, DWORD* code )
    {
        BOOST_CHECK_EQUAL( handle, dummy );
        BOOST_REQUIRE( code );
        *code = STILL_ACTIVE;
        return true;
    }
}

BOOST_AUTO_TEST_CASE( process_with_handle_constructs )
{
    MockApi api;
    int pid = 42;
    Handle ptr = MakeHandle( api, dummy );
    MOCK_EXPECT( api.GetProcessName ).once().with( dummy, mock::any, mock::any ).returns( 0 );
    MOCK_EXPECT( api.CloseHandle ).once().with( dummy ).returns( true );
    Process process( api, pid, ptr );
}

BOOST_AUTO_TEST_CASE( process_returns_correct_members )
{
    MockApi api;
    int pid = 42;
    std::unique_ptr< Process > process = MakeProcess( api, pid, wname );
    BOOST_CHECK_EQUAL( Utf8( wname ), process->GetName() );
    BOOST_CHECK_EQUAL( pid, process->GetPid() );
}

BOOST_AUTO_TEST_CASE( process_joins )
{
    MockApi api;
    std::unique_ptr< Process > process = MakeProcess( api, 42, wname );
    int msTimeout = 3*1000;
    MOCK_EXPECT( api.WaitForSingleObjectEx ).once().with( dummy, msTimeout, false ).returns( WAIT_OBJECT_0 );
    bool done = process->Join( msTimeout );
    BOOST_CHECK( done );
}

BOOST_AUTO_TEST_CASE( corrupted_process_terminates_unsafely )
{
    MockApi api;
    std::unique_ptr< Process > process = MakeProcess( api, 42, wname );
    MOCK_EXPECT( api.TerminateProcess ).once().with( dummy, mock::any ).returns( true );
    process->Kill();
}

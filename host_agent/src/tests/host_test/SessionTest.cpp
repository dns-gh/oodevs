// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifdef _MSC_VER
#   define _SCL_SECURE_NO_WARNINGS
#endif

#include "host_test.h"

#include <runtime/Process_ABC.h>
#include <runtime/Runtime_ABC.h>

#include <host/FileSystem_ABC.h>
#include <host/PortFactory_ABC.h>
#include <host/Session.h>
#include <host/UuidFactory_ABC.h>

#include <boost/make_shared.hpp>
#include <boost/uuid/uuid_generators.hpp>

using namespace host;

namespace
{
    const std::string default_tag_string = "12345678-90AB-CDEF-9876-543210123456";
    const boost::uuids::uuid default_tag = boost::uuids::string_generator()( default_tag_string );

    MOCK_BASE_CLASS( MockRuntime, runtime::Runtime_ABC )
    {
        MOCK_METHOD( GetProcesses, 0 );
        MOCK_METHOD( GetProcess, 1 );
        MOCK_METHOD( Start, 3 );
    };

    MOCK_BASE_CLASS( MockProcess, runtime::Process_ABC )
    {
        MOCK_METHOD( GetPid, 0 );
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( Join, 1 );
        MOCK_METHOD( Kill, 1 );
    };

    MOCK_BASE_CLASS( MockUuidFactory, UuidFactory_ABC )
    {
        MOCK_METHOD( Create, 0 );
    };

    MOCK_BASE_CLASS( MockFileSystem, FileSystem_ABC )
    {
        MOCK_METHOD( IsFile, 1 );
        MOCK_METHOD( IsDirectory, 1 );
        MOCK_METHOD( Exists, 1 );
        MOCK_METHOD( Copy, 2 );
        MOCK_METHOD( CreateDirectory, 1 );
        MOCK_METHOD( Remove, 1 );
        MOCK_METHOD( WriteFile, 2 );
        MOCK_METHOD( ReadFile, 1 );
        MOCK_METHOD( Glob, 2 );
    };

    MOCK_BASE_CLASS( MockPort, Port_ABC )
    {
        MOCK_METHOD( Get, 0 );
    };

    struct Fixture
    {
        Fixture()
        {
            MOCK_EXPECT( system.IsDirectory ).with( mock::any ).returns( true );
            MOCK_EXPECT( system.IsFile ).with( mock::any ).returns( true );
            MOCK_EXPECT( system.Exists ).with( mock::any ).returns( true );
            MOCK_EXPECT( system.CreateDirectory ).with( mock::any );
            MOCK_EXPECT( system.Copy ).with( mock::any, mock::any );
        }
        MockRuntime runtime;
        MockUuidFactory uuids;
        MockFileSystem system;
    };
}

BOOST_FIXTURE_TEST_CASE( session_starts_and_stops, Fixture )
{
    MOCK_EXPECT( uuids.Create ).once().returns( default_tag );
    MockPort* ptr = new MockPort();
    MOCK_EXPECT( ptr->Get ).returns( 10000 );
    Session session( runtime, uuids, system, L"data", L"apps", "exercise_name", "session_name", std::auto_ptr< Port_ABC >( ptr ) );
    boost::shared_ptr< MockProcess > process = boost::make_shared< MockProcess >();
    MOCK_EXPECT( runtime.Start ).once().with( mock::any, mock::any, mock::any ).returns( process );
    MOCK_EXPECT( system.WriteFile ).once().with( mock::any, mock::any );
    session.Start();
    MOCK_EXPECT( process->Kill ).once().with( mock::any ).returns( true );
    session.Stop();
}

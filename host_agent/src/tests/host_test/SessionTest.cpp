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

#include <xeumeuleu/xml.hpp>

#include <boost/make_shared.hpp>
#include <boost/uuid/uuid_generators.hpp>

using namespace host;

namespace
{
    const std::string default_tag_string = "12345678-90AB-CDEF-9876-543210123456";

    MOCK_BASE_CLASS( MockRuntime, runtime::Runtime_ABC )
    {
        MOCK_METHOD( GetProcesses, 0 );
        MOCK_METHOD( GetProcess, 1 );
        MOCK_METHOD( Start, 3 );
    };

    MOCK_BASE_CLASS( MockProcess, runtime::Process_ABC )
    {
        MockProcess( int pid, const std::string& name )
            : pid_( pid ), name_( name )
        {
            MOCK_EXPECT( this->GetPid ).returns( pid_ );
            MOCK_EXPECT( this->GetName ).returns( name_ );
        }
        MOCK_METHOD( GetPid, 0 );
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( Join, 1 );
        MOCK_METHOD( Kill, 1 );
    private:
        int pid_;
        const std::string name_;
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
        MockPort( int port )
        {
            MOCK_EXPECT( this->Get ).returns( port );
        }
    };

    MOCK_BASE_CLASS( MockPortFactory, PortFactory_ABC )
    {
        MOCK_METHOD_EXT( Create, 0, std::auto_ptr< Port_ABC >(), Create0 );
        MOCK_METHOD_EXT( Create, 1, std::auto_ptr< Port_ABC >( int ), Create1 );
    };

    struct Fixture
    {
        MockRuntime     runtime;
        MockUuidFactory uuids;
        MockFileSystem  system;
        MockPortFactory ports;
        const boost::uuids::uuid tag;
        const int port;
        const boost::filesystem::wpath data;
        const boost::filesystem::wpath apps;
        const std::string exercise;
        const std::string name;
        const int processPid;
        const std::string processName;
        Fixture()
            : tag        ( boost::uuids::string_generator()( default_tag_string ) )
            , port       ( 10000 )
            , data       ( L"data" )
            , apps       ( L"apps" )
            , exercise   ( "my_exercise" )
            , name       ( "my_name" )
            , processPid ( 1337 )
            , processName( "bidule.exe" )
        {
            MOCK_EXPECT( system.CreateDirectory );
            MOCK_EXPECT( system.IsDirectory ).with( data ).returns( true );
            MOCK_EXPECT( system.IsDirectory ).with( apps ).returns( true );
            MOCK_EXPECT( system.Exists ).with( apps / L"/simulation_app.exe" ).returns( true );
            MOCK_EXPECT( system.IsFile ).with( apps / L"/simulation_app.exe" ).returns( true );
        }

        boost::shared_ptr< Session > MakeSession()
        {
            MOCK_EXPECT( uuids.Create ).once().returns( tag );
            MOCK_EXPECT( ports.Create0 ).once().returns( std::auto_ptr< Port_ABC >( new MockPort( port ) ) );
            return boost::shared_ptr< Session >( new Session( runtime, uuids, system, data, apps, exercise, name, ports ) );
        }

        boost::shared_ptr< Session > MakeSession( const std::string& sessionTag, boost::shared_ptr< MockProcess > process )
        {
            MOCK_EXPECT( runtime.GetProcess ).once().with( processPid ).returns( process );
            MOCK_EXPECT( ports.Create1 ).once().returns( std::auto_ptr< Port_ABC >( new MockPort( port ) ) );
            return boost::shared_ptr< Session >( new Session( runtime, system, data, apps, xml::xistringstream( sessionTag ), ports ) );
        }

        boost::shared_ptr< MockProcess > StartSession( Session& session, std::string* sessionTag = 0 )
        {
            boost::shared_ptr< MockProcess > reply = boost::make_shared< MockProcess >( processPid, processName );
            MOCK_EXPECT( runtime.Start ).once().returns( reply );
            MOCK_EXPECT( system.WriteFile ).once();
            if( sessionTag  )
                MOCK_EXPECT( system.WriteFile ).once().with( mock::any, mock::retrieve( *sessionTag ) );
            else
                MOCK_EXPECT( system.WriteFile ).once();
            session.Start();
            return reply;
        }

        void StopSession( Session& session, MockProcess& process )
        {
            MOCK_EXPECT( process.Kill ).once().returns( true );
            session.Stop();
        }
    };
}

BOOST_FIXTURE_TEST_CASE( session_starts_and_stops, Fixture )
{
    boost::shared_ptr< Session > session = MakeSession();
    boost::shared_ptr< MockProcess > process = StartSession( *session );
    StopSession( *session, *process );
}

BOOST_FIXTURE_TEST_CASE( session_reloads, Fixture )
{
    boost::shared_ptr< MockProcess > process;
    std::string sessionTag;
    {
        boost::shared_ptr< Session > session = MakeSession();
        process = StartSession( *session, &sessionTag );
    }
    {
        boost::shared_ptr< Session > session = MakeSession( sessionTag, process );
        StopSession( *session, *process );
    }
}

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

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/uuid/uuid_generators.hpp>

using namespace host;

namespace
{
    const std::string default_id_string = "12345678-90AB-CDEF-9876-543210123456";

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
        MOCK_METHOD( CopyDirectory, 2 );
        MOCK_METHOD( CopyFile, 2 );
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
        const boost::uuids::uuid id;
        const int port;
        const boost::filesystem::wpath data;
        const boost::filesystem::wpath apps;
        const std::string exercise;
        const std::string name;
        const int processPid;
        const std::string processName;
        Fixture()
            : id         ( boost::uuids::string_generator()( default_id_string ) )
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
            MOCK_EXPECT( system.Remove );
        }

        void SaveSessionTag( std::string* tag )
        {
            if( tag )
                MOCK_EXPECT( system.WriteFile ).once().with( mock::any, mock::retrieve( *tag ) );
            else
                MOCK_EXPECT( system.WriteFile ).once();
        }

        boost::shared_ptr< Session > MakeSession( std::string* tag = 0 )
        {
            MOCK_EXPECT( uuids.Create ).once().returns( id );
            MOCK_EXPECT( ports.Create0 ).once().returns( std::auto_ptr< Port_ABC >( new MockPort( port ) ) );
            SaveSessionTag( tag );
            return boost::shared_ptr< Session >( new Session( runtime, uuids, system, data, apps, exercise, name, ports ) );
        }

        boost::shared_ptr< Session > MakeSession( const std::string& tag, boost::shared_ptr< MockProcess > process )
        {
            if( process )
                MOCK_EXPECT( runtime.GetProcess ).once().with( processPid ).returns( process );
            MOCK_EXPECT( ports.Create1 ).once().returns( new MockPort( port ) );
            xml::xistringstream xis( tag );
            SaveSessionTag( 0 );
            return boost::shared_ptr< Session >( new Session( runtime, system, data, apps, xis, ports ) );
        }

        boost::shared_ptr< MockProcess > StartSession( Session& session, std::string* tag = 0, const std::string& name = std::string() )
        {
            boost::shared_ptr< MockProcess > reply = boost::make_shared< MockProcess >( processPid, name.empty() ? processName : name );
            MOCK_EXPECT( runtime.Start ).once().returns( reply );
            MOCK_EXPECT( system.WriteFile ).once();
            SaveSessionTag( tag );
            session.Start();
            MOCK_EXPECT( reply->Kill ).once().returns( true );
            return reply;
        }

        void StopSession( Session& session )
        {
            SaveSessionTag( 0 );
            session.Stop();
        }
    };
}

BOOST_FIXTURE_TEST_CASE( session_starts_and_stops, Fixture )
{
    boost::shared_ptr< Session > session = MakeSession();
    StartSession( *session );
    StopSession( *session );
}

BOOST_FIXTURE_TEST_CASE( session_reloads, Fixture )
{
    std::string tag;
    MakeSession( &tag );
    MakeSession( tag, boost::shared_ptr< MockProcess >() );
}

BOOST_FIXTURE_TEST_CASE( session_starts_and_reloads, Fixture )
{
    std::string tag;
    boost::shared_ptr< MockProcess > process = StartSession( *MakeSession( &tag ), &tag );
    MOCK_EXPECT( process->Kill ).once().returns( true );
    StopSession( *MakeSession( tag, process ) );
}

BOOST_FIXTURE_TEST_CASE( session_rejects_bind_to_another_process, Fixture )
{
    std::string tag;
    StartSession( *MakeSession( &tag ), &tag );
    MakeSession( tag, boost::make_shared< MockProcess >( processPid, "a" + processName ) );
}

BOOST_FIXTURE_TEST_CASE( session_converts_to_json, Fixture )
{
    boost::shared_ptr< Session > ptr = MakeSession();
    BOOST_CHECK_EQUAL( ptr->ToJson(), "{ "
        "\"id\" : \"12345678-90ab-cdef-9876-543210123456\", "
        "\"name\" : \"my_name\", "
        "\"port\" : 10000, "
        "\"exercise\" : \"my_exercise\", "
        "\"status\" : \"stopped\""
        " }"
    );
    StartSession( *ptr );
    BOOST_CHECK_EQUAL( ptr->ToJson(), "{ "
        "\"id\" : \"12345678-90ab-cdef-9876-543210123456\", "
        "\"name\" : \"my_name\", "
        "\"port\" : 10000, "
        "\"exercise\" : \"my_exercise\", "
        "\"status\" : \"playing\""
        " }"
    );
}

namespace
{
    bool IsQuoted( const std::string& arg )
    {
        return arg.empty() || ( arg.front() == '"' && arg.back() == '"' );
    }

    boost::shared_ptr< runtime::Process_ABC > CheckRuntimeStart( const std::string& cmd, const std::vector< std::string >& args, const std::string& run )
    {
        BOOST_CHECK( IsQuoted( cmd ) );
        BOOST_FOREACH( const std::string& arg, args )
        {
            size_t separator = arg.find_first_of( '=' );
            if( separator == std::string::npos )
                BOOST_CHECK( IsQuoted( arg ) );
            else
                BOOST_CHECK( IsQuoted( arg.substr( separator + 1, std::string::npos ) ) );
        }
        BOOST_CHECK( IsQuoted( run ) );
        boost::shared_ptr< MockProcess > ptr = boost::make_shared< MockProcess >( 1337, "noname" );
        MOCK_EXPECT( ptr->Kill ).once().returns( true );
        return ptr;
    }
}

BOOST_FIXTURE_TEST_CASE( session_start_with_quoted_arguments, Fixture )
{
    boost::shared_ptr< Session > session = MakeSession();
    MOCK_EXPECT( runtime.Start ).once().calls( &CheckRuntimeStart );
    MOCK_EXPECT( system.WriteFile ).once();
    SaveSessionTag( 0 );
    session->Start();
}

BOOST_FIXTURE_TEST_CASE( session_can_start_twice, Fixture )
{
    boost::shared_ptr< Session > session = MakeSession();
    StartSession( *session );
    session->Start();
}
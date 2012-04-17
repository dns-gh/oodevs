// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "host_test.h"

#include <cpplog/cpplog.hpp>

#include <runtime/Process_ABC.h>
#include <runtime/Runtime_ABC.h>

#include <host/FileSystem_ABC.h>
#include <host/Node.h>
#include <host/Pool_ABC.h>
#include <host/PortFactory_ABC.h>
#include <host/Proxy_ABC.h>
#include <host/UuidFactory_ABC.h>

#include <xeumeuleu/xml.hpp>

#include <boost/bind/apply.hpp>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/uuid/uuid_generators.hpp>

using namespace host;

namespace
{
    const std::string default_id_string = "12345678-90ab-cdef-9876-543210123456";
    const boost::uuids::uuid default_id = boost::uuids::string_generator()( default_id_string );

    MOCK_BASE_CLASS( MockLog, cpplog::BaseLogger )
    {
        MOCK_METHOD( sendLogMessage, 1 );
        MockLog()
        {
            MOCK_EXPECT( this->sendLogMessage ).returns( true );
        }
    };

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

    MOCK_BASE_CLASS( MockProxy, Proxy_ABC )
    {
        MockProxy( int port )
        {
            MOCK_EXPECT( this->GetPort ).returns( port );
        }
        MOCK_METHOD( GetPort, 0 );
        MOCK_METHOD( Register, 3 );
        MOCK_METHOD( Unregister, 1 );
    };

    MOCK_BASE_CLASS( MockPool, Pool_ABC )
    {
        MockPool()
        {
            MOCK_EXPECT( this->Post ).calls( boost::bind( boost::apply< void >(), _1 ) );
        }
        MOCK_METHOD( Post, 1 );
        MOCK_METHOD( Stop, 0 );
    };

    struct Fixture
    {
        MockLog         log;
        MockRuntime     runtime;
        MockUuidFactory uuids;
        MockFileSystem  system;
        MockPortFactory ports;
        MockProxy       proxy;
        MockPool        pool;
        const int port;
        const boost::filesystem::path java;
        const boost::filesystem::path jar;
        const boost::filesystem::path web;
        const std::string name;
        const int processPid;
        const std::string processName;
        Fixture()
            : port       ( 10000 )
            , proxy      ( 8080 )
            , java       ( L"java" )
            , jar        ( L"jar" )
            , web        ( L"web" )
            , name       ( "my_name" )
            , processPid ( 1337 )
            , processName( "java.exe" )
        {
            MOCK_EXPECT( system.CreateDirectory );
            MOCK_EXPECT( system.Exists ).with( jar ).returns( true );
            MOCK_EXPECT( system.IsFile ).with( jar ).returns( true );
            MOCK_EXPECT( system.IsDirectory ).with( web ).returns( true );
            MOCK_EXPECT( system.Remove );
        }

        void SaveNodeTag( std::string* tag )
        {
            if( tag )
                MOCK_EXPECT( system.WriteFile ).once().with( mock::any, mock::retrieve( *tag ) );
            else
                MOCK_EXPECT( system.WriteFile ).once();
        }

        boost::shared_ptr< Node > MakeNode( std::string* tag = 0 )
        {
            MOCK_EXPECT( uuids.Create ).once().returns( default_id );
            MOCK_EXPECT( ports.Create0 ).once().returns( std::auto_ptr< Port_ABC >( new MockPort( port ) ) );
            SaveNodeTag( tag );
            MOCK_EXPECT( proxy.Register ).once().with( default_id_string, "localhost", port );
            MOCK_EXPECT( proxy.Unregister ).once().with( default_id_string );
            return boost::shared_ptr< Node >( new Node( log, pool, runtime, uuids, system, proxy, java, jar, web, "node", name, ports ) );
        }

        boost::shared_ptr< Node > MakeNode( const std::string& tag, boost::shared_ptr< MockProcess > process )
        {
            if( process )
                MOCK_EXPECT( runtime.GetProcess ).once().with( processPid ).returns( process );
            MOCK_EXPECT( ports.Create1 ).once().returns( new MockPort( port ) );
            xml::xistringstream xis( tag );
            SaveNodeTag( 0 );
            MOCK_EXPECT( proxy.Register ).once().with( default_id_string, "localhost", port );
            MOCK_EXPECT( proxy.Unregister ).once().with( default_id_string );
            return boost::shared_ptr< Node >( new Node( log, pool, runtime, system, proxy, java, jar, web, xis, ports ) );
        }

        boost::shared_ptr< MockProcess > StartNode( Node& node, std::string* tag = 0, const std::string& name = std::string() )
        {
            boost::shared_ptr< MockProcess > reply = boost::make_shared< MockProcess >( processPid, name.empty() ? processName : name );
            MOCK_EXPECT( runtime.Start ).once().returns( reply );
            SaveNodeTag( tag );
            node.Start();
            MOCK_EXPECT( reply->Kill ).once().returns( true );
            return reply;
        }

        void StopNode( Node& node )
        {
            SaveNodeTag( 0 );
            node.Stop();
        }
    };
}

BOOST_FIXTURE_TEST_CASE( node_starts_and_stops, Fixture )
{
    boost::shared_ptr< Node > node = MakeNode();
    StartNode( *node );
    StopNode( *node );
}

BOOST_FIXTURE_TEST_CASE( node_reloads, Fixture )
{
    std::string tag;
    MakeNode( &tag );
    MakeNode( tag, boost::shared_ptr< MockProcess >() );
}

BOOST_FIXTURE_TEST_CASE( node_starts_and_reloads, Fixture )
{
    std::string tag;
    boost::shared_ptr< MockProcess > process = StartNode( *MakeNode( &tag ), &tag );
    MOCK_EXPECT( process->Kill ).once().returns( true );
    StopNode( *MakeNode( tag, process ) );
}

BOOST_FIXTURE_TEST_CASE( node_rejects_bind_to_another_process, Fixture )
{
    std::string tag;
    StartNode( *MakeNode( &tag ), &tag );
    MakeNode( tag, boost::make_shared< MockProcess >( processPid, "a" + processName ) );
}

BOOST_FIXTURE_TEST_CASE( node_converts_to_json, Fixture )
{
    boost::shared_ptr< Node > ptr = MakeNode();
    BOOST_CHECK_EQUAL( ptr->ToJson(), "{ "
        "\"id\" : \"12345678-90ab-cdef-9876-543210123456\", "
        "\"type\" : \"node\", "
        "\"name\" : \"my_name\", "
        "\"port\" : 10000, "
        "\"status\" : \"stopped\""
        " }"
    );
    StartNode( *ptr );
    BOOST_CHECK_EQUAL( ptr->ToJson(), "{ "
        "\"id\" : \"12345678-90ab-cdef-9876-543210123456\", "
        "\"type\" : \"node\", "
        "\"name\" : \"my_name\", "
        "\"port\" : 10000, "
        "\"status\" : \"running\""
        " }"
    );
}

namespace
{
    bool IsQuoted( const std::string& arg )
    {
        return arg.empty() || ( arg.front() == '"' && arg.back() == '"' );
    }

    boost::shared_ptr< runtime::Process_ABC > CheckRuntimeStart( const std::string& /*cmd*/, const std::vector< std::string >& args, const std::string& /*run*/ )
    {
        BOOST_FOREACH( const std::string& arg, args )
        {
            size_t separator = arg.find_last_of( ' ' );
            if( separator != std::string::npos )
                BOOST_CHECK( IsQuoted( arg.substr( separator + 1, std::string::npos ) ) );
        }
        boost::shared_ptr< MockProcess > ptr = boost::make_shared< MockProcess >( 1337, "noname" );
        MOCK_EXPECT( ptr->Kill ).once().returns( true );
        return ptr;
    }
}

BOOST_FIXTURE_TEST_CASE( node_start_with_quoted_arguments, Fixture )
{
    boost::shared_ptr< Node > node = MakeNode();
    MOCK_EXPECT( runtime.Start ).once().calls( &CheckRuntimeStart );
    SaveNodeTag( 0 );
    node->Start();
}

BOOST_FIXTURE_TEST_CASE( node_can_start_twice, Fixture )
{
    boost::shared_ptr< Node > node = MakeNode();
    StartNode( *node );
    node->Start();
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "host_test.h"

#include <host/Proxy.h>
#include <host/FileSystem_ABC.h>
#include <host/Pool_ABC.h>
#include <cpplog/cpplog.hpp>
#include <runtime/Runtime_ABC.h>
#include <runtime/Process_ABC.h>
#include <web/Client_ABC.h>

#include <boost/bind/apply.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>

using namespace host;

namespace
{
    MOCK_BASE_CLASS( MockLog, cpplog::BaseLogger )
    {
        MockLog()
        {
            MOCK_EXPECT( sendLogMessage ).returns( true );
        }
        MOCK_METHOD( sendLogMessage, 1 );
    };

    MOCK_BASE_CLASS( MockRuntime, runtime::Runtime_ABC )
    {
        MOCK_METHOD( GetProcesses, 0 );
        MOCK_METHOD( GetProcess, 1 );
        MOCK_METHOD( Start, 3 );
    };

    MOCK_BASE_CLASS( MockFileSystem, FileSystem_ABC )
    {
        MOCK_METHOD( IsFile, 1 );
        MOCK_METHOD( IsDirectory, 1 );
        MOCK_METHOD( Exists, 1 );
        MOCK_METHOD( CopyDirectory, 2 );
        MOCK_METHOD( CopyFile, 2 );
        MOCK_METHOD( MakeDirectory, 1 );
        MOCK_METHOD( Remove, 1 );
        MOCK_METHOD( WriteFile, 2 );
        MOCK_METHOD( ReadFile, 1 );
        MOCK_METHOD( Glob, 2 );
    };

    MOCK_BASE_CLASS( MockClient, web::Client_ABC )
    {
        MOCK_METHOD( Get, 4 );
    };

    MOCK_BASE_CLASS( MockProcess, runtime::Process_ABC )
    {
        MockProcess( int pid, const std::string& name )
        {
            MOCK_EXPECT( GetPid ).returns( pid );
            MOCK_EXPECT( GetName ).returns( name );
        }
        MOCK_METHOD( GetPid, 0 );
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( Join, 1 );
        MOCK_METHOD( Kill, 1 );
    };

    MOCK_BASE_CLASS( MockPool, Pool_ABC )
    {
        MockPool()
        {
            MOCK_EXPECT( Post ).calls( boost::bind( boost::apply< void >(), _1 ) );
        }
        MOCK_METHOD( Post, 1 );
        MOCK_METHOD( Stop, 0 );
    };

    struct Fixture
    {
        Fixture()
            : java   ( "java" )
            , jar    ( "jar" )
            , port   ( 1337 )
            , process( boost::make_shared< MockProcess >( 7331, "el_process_name" ) )
        {}

        const std::string java;
        const std::string jar;
        const int port;
        std::string tag;
        MockLog log;
        MockRuntime runtime;
        MockFileSystem system;
        MockClient client;
        MockPool pool;
        boost::shared_ptr< MockProcess > process;

        boost::shared_ptr< Proxy > MakeProxy()
        {
            MOCK_EXPECT( system.IsFile ).once().with( "proxy.id" ).returns( false );
            MOCK_EXPECT( system.MakeDirectory );
            MOCK_EXPECT( system.WriteFile ).once().with( mock::any, mock::retrieve( tag ) );
            MOCK_EXPECT( runtime.Start ).once().with( java, boost::assign::list_of
                ( " " "-jar \"" + jar + "\"" )
                ( "--port \"" + boost::lexical_cast< std::string >( port ) + "\"" ),
                "" ).returns( process );
            return boost::make_shared< Proxy >( log, runtime, system, java, jar, port, client, pool );
        }

        boost::shared_ptr< Proxy > ReloadProxy()
        {
            MOCK_EXPECT( runtime.GetProcess ).once().with( process->GetPid() ).returns( process );
            MOCK_EXPECT( system.IsFile ).once().with( "proxy.id" ).returns( true );
            MOCK_EXPECT( system.ReadFile ).once().with( "proxy.id" ).returns( tag );
            return boost::make_shared< Proxy >( log, runtime, system, java, jar, port, client, pool );
        }
    };

    bool Contains( const web::Client_ABC::T_Parameters& actual, const web::Client_ABC::T_Parameters& expected )
    {
        BOOST_FOREACH( const web::Client_ABC::T_Parameters::value_type& value, expected )
        {
            web::Client_ABC::T_Parameters::const_iterator it = actual.find( value.first );
            if( it == actual.end() || value.second != it->second )
                return false;
        }
        return true;
    }
}

BOOST_FIXTURE_TEST_CASE( proxy_registers, Fixture )
{
    boost::shared_ptr< Proxy > proxy = MakeProxy();
    const std::string dstPrefix = "some_prefix";
    const std::string dstHost = "some_host";
    const int dstPort = 15000;
    MOCK_EXPECT( client.Get ).once().with( "localhost", port, "/register_proxy",
        boost::bind( &Contains, _1, boost::assign::map_list_of
            ( "prefix", dstPrefix )
            ( "host",   dstHost )
            ( "port",   boost::lexical_cast< std::string >( dstPort ) ) ) );
    proxy->Register( dstPrefix, dstHost, dstPort );
    MOCK_EXPECT( process->Kill ).once().returns( true );
}

BOOST_FIXTURE_TEST_CASE( proxy_unregisters, Fixture )
{
    boost::shared_ptr< Proxy > proxy = MakeProxy();
    const std::string dstPrefix = "some_prefix";
    MOCK_EXPECT( client.Get ).once().with( "localhost", port, "/unregister_proxy",
        boost::bind( &Contains, _1, boost::assign::map_list_of( "prefix", dstPrefix ) ) );
    proxy->Unregister( dstPrefix );
    MOCK_EXPECT( process->Kill ).once().returns( true );
}

BOOST_FIXTURE_TEST_CASE( proxy_reloads, Fixture )
{
    MOCK_EXPECT( process->Kill ).once().returns( true );
    MakeProxy();
    MOCK_EXPECT( process->Kill ).once().returns( true );
    ReloadProxy();
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "host_test.h"

#include "host/Proxy.h"

#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>

#include "MockClient.h"
#include "MockFileSystem.h"
#include "MockLog.h"
#include "MockPool.h"
#include "MockProcess.h"
#include "MockRuntime.h"

using namespace host;
using namespace mocks;

namespace
{
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

    struct Fixture
    {
        Fixture()
            : ssl    ( false, "cert.pem", "key.pem" )
            , config ( "", "app", 1337, 1338, ssl )
            , process( boost::make_shared< MockProcess >( 7331, "el_process_name" ) )
        {
            MOCK_EXPECT( fs.Exists ).with( config.app ).returns( true );
            MOCK_EXPECT( fs.IsFile ).with( config.app ).returns( true );
        }

        const proxy::Ssl ssl;
        const proxy::Config config;
        std::string tag;
        MockLog log;
        MockRuntime runtime;
        MockFileSystem fs;
        MockClient client;
        MockPool pool;
        boost::shared_ptr< MockProcess > process;

        boost::shared_ptr< Proxy > MakeProxy()
        {
            MOCK_EXPECT( fs.IsFile ).once().with( "proxy.id" ).returns( false );
            MOCK_EXPECT( fs.MakePaths );
            MOCK_EXPECT( fs.WriteFile ).once().with( mock::any, mock::retrieve( tag ) ).returns( true );
            MOCK_EXPECT( runtime.Start ).once().with( config.app, mock::any, "", mock::any ).returns( process );
            MOCK_EXPECT( fs.Remove ).once().with( "proxy.id" ).returns( true );
            MOCK_EXPECT( process->Kill ).once().returns( true );
            MOCK_EXPECT( process->Join ).returns( true );
            return boost::make_shared< Proxy >( log, runtime, fs, config, client, pool );
        }

        boost::shared_ptr< Proxy > ReloadProxy()
        {
            MOCK_EXPECT( runtime.GetProcess ).once().with( process->GetPid() ).returns( process );
            MOCK_EXPECT( fs.IsFile ).once().with( "proxy.id" ).returns( true );
            MOCK_EXPECT( fs.ReadFile ).once().with( "proxy.id" ).returns( tag );
            MOCK_EXPECT( fs.Remove ).once().with( "proxy.id" ).returns( true );
            MOCK_EXPECT( process->Kill ).once().returns( true );
            MOCK_EXPECT( process->Join ).returns( true );
            return boost::make_shared< Proxy >( log, runtime, fs, config, client, pool );
        }

        void Register( boost::shared_ptr< Proxy > proxy, const std::string& prefix, const std::string& host, int next )
        {
            boost::shared_ptr< MockResponse > response = boost::make_shared< MockResponse >();
            MOCK_EXPECT( response->GetStatus ).returns( 200 );
            MOCK_EXPECT( client.Get5 ).once().with( mock::any, "localhost", config.http, "/register_proxy",
                boost::bind( &Contains, _1, boost::assign::map_list_of
                    ( "prefix", prefix )
                    ( "host",   host )
                    ( "port",   boost::lexical_cast< std::string >( next ) ) ) )
                .returns( response );
            proxy->Register( prefix, host, next );
        }

        void Unregister( boost::shared_ptr< Proxy > proxy, const std::string& prefix )
        {
            boost::shared_ptr< MockResponse > response = boost::make_shared< MockResponse >();
            MOCK_EXPECT( response->GetStatus ).returns( 200 );
            MOCK_EXPECT( client.Get5 ).once().with( mock::any, "localhost", config.http, "/unregister_proxy",
                boost::bind( &Contains, _1, boost::assign::map_list_of( "prefix", prefix ) ) )
                .returns( response );
            proxy->Unregister( prefix );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( proxy_registers, Fixture )
{
    boost::shared_ptr< Proxy > proxy = MakeProxy();
    Register( proxy, "some_prefix", "some_host", 15000 );
}

BOOST_FIXTURE_TEST_CASE( proxy_unregisters, Fixture )
{
    boost::shared_ptr< Proxy > proxy = MakeProxy();
    Register( proxy, "some_prefix", "some_host", 15000 );
    Unregister( proxy, "some_prefix" );
}

BOOST_FIXTURE_TEST_CASE( proxy_reloads, Fixture )
{
    MakeProxy();
    ReloadProxy();
}

BOOST_FIXTURE_TEST_CASE( proxy_with_externally_killed_process_resaves, Fixture )
{
    boost::shared_ptr< Proxy > proxy = MakeProxy();
    MOCK_RESET( process->IsAlive );
    MOCK_EXPECT( process->IsAlive ).once().returns( false );
    MOCK_EXPECT( runtime.Start ).once().returns( process );
    MOCK_EXPECT( fs.WriteFile ).once().returns( true );
    proxy->Update();
}

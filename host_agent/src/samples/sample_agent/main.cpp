// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include <runtime/Factory.h>
#include <host/Agent.h>
#include <host/FileSystem.h>
#include <host/Node_ABC.h>
#include <host/NodeController.h>
#include <host/Pool.h>
#include <host/PortFactory.h>
#include <host/Proxy.h>
#include <host/SecurePool.h>
#include <host/SessionController.h>
#include <host/UuidFactory.h>
#include <web/Client.h>
#include <web/Controller.h>
#include <web/Server.h>

#define  CPPLOG_THREADING
#include <cpplog/cpplog.hpp>

#include <boost/lexical_cast.hpp>

namespace
{

    template< typename T >
    bool ReadParameter( T& dst, const std::string& name, int& idx, int argc, const char* argv[] )
    {
        if( name != argv[idx] )
            return false;
        if( ++idx >= argc )
            throw std::runtime_error( "missing " + name + " parameter" );
        dst = boost::lexical_cast< T >( argv[idx] );
        return true;
    }

    struct Configuration
    {
        std::wstring java;
        struct
        {
            int host;
            int proxy;
            int period;
            int min;
            int max;
        } ports;
        struct
        {
            std::wstring jar;
        } proxy;
        struct
        {
            bool enabled;
        } cluster;
        struct
        {
            std::wstring jar;
            std::wstring root;
        } node;
        struct
        {
            std::wstring data;
            std::wstring applications;
        } session;

        void Parse( int argc, const char* argv[] )
        {
            for( int i = 0; i < argc; ++i )
            {
                bool found = false;
                found |= ReadParameter( ports.host, "--port_host", i, argc, argv );
                found |= ReadParameter( ports.period, "--port_period", i, argc, argv );
                found |= ReadParameter( ports.min, "--port_min", i, argc, argv );
                found |= ReadParameter( ports.max, "--port_max", i, argc, argv );
                found |= ReadParameter( ports.proxy, "--port_proxy", i, argc, argv );
                found |= ReadParameter( cluster.enabled, "--cluster", i, argc, argv );
                found |= ReadParameter( java, "--java", i, argc, argv );
                found |= ReadParameter( proxy.jar, "--proxy_jar", i, argc, argv );
                found |= ReadParameter( node.jar, "--node_jar", i, argc, argv );
                found |= ReadParameter( node.root, "--node_root", i, argc, argv );
                found |= ReadParameter( session.data, "--session_data", i, argc, argv );
                found |= ReadParameter( session.applications, "--session_applications", i, argc, argv );
                if( !found )
                    throw std::runtime_error( std::string( "unsupported parameter " ) + argv[i] );
            }
        }
    };

    void Start( cpplog::BaseLogger& log, Configuration& cfg )
    {
        host::Pool pool( 8 );
        runtime::Factory runtime( log );
        host::UuidFactory uuids;
        host::FileSystem system( log );
        web::Client client;
        host::Proxy proxy( log, runtime.GetRuntime(), system, cfg.java, cfg.proxy.jar, cfg.ports.proxy, client, pool );
        proxy.Register( "api", "localhost", cfg.ports.host );
        host::PortFactory ports( cfg.ports.period, cfg.ports.min, cfg.ports.max );
        host::NodeController nodes( log, runtime.GetRuntime(), system, uuids, proxy, cfg.java, cfg.node.jar, cfg.node.root, "node", pool, ports );
        host::NodeController cluster( log, runtime.GetRuntime(), system, uuids, proxy, cfg.java, cfg.node.jar, cfg.node.root, "cluster", pool, ports );
        host::SessionController sessions( log, runtime.GetRuntime(), system, uuids, cfg.session.data, cfg.session.applications, pool, ports );
        host::Agent agent( log, cfg.cluster.enabled ? &cluster : 0, nodes, sessions );
        web::Controller controller( log, agent );
        web::Server server( log, controller, cfg.ports.host );
        server.Listen();
        host::SecurePool run( log, "server", pool );
        run.Post( boost::bind( &web::Server::Run, &server ) );
        getc( stdin );
        server.Stop();
    }
}

int StartServer( int argc, const char* argv[] )
{
    cpplog::StdErrLogger base;
    cpplog::BackgroundLogger log( base );
    LOG_INFO( log ) << "Host Agent - (c) copyright MASA Group 2012";
    Configuration cfg;
    cfg.ports.host           = 15000;
    cfg.ports.period         = 40;
    cfg.ports.min            = 50000;
    cfg.ports.max            = 60000;
    cfg.ports.proxy          = 8080;
    cfg.java                 = L"C:/Program Files/Java/jdk1.6.0_31/bin/java.exe";
    cfg.proxy.jar            = L"e:/cloud_hg/proxy/out/jar/proxy.jar";
    cfg.node.jar             = L"e:/cloud_hg/node/out/jar/node.jar";
    cfg.node.root            = L"e:/cloud_hg/node/www";
    cfg.session.data         = L"d:/apps/sword_434_data";
    cfg.session.applications = L"d:/apps/sword_434/applications";
    try
    {
        cfg.Parse( argc-1, argv+1 );
    }
    catch( const std::runtime_error& err )
    {
        LOG_FATAL( log ) << "[cfg] Unable to parse configuration, " << err.what();
    }
    try
    {
        Start( log, cfg );
    }
    catch( const std::runtime_error& err )
    {
        LOG_ERROR( log ) << "[main] Unable to start, " << err.what();
    }
    LOG_INFO( log ) << "Host Agent - Exit";
    return 0;
}

int main( int argc, const char* argv[] )
{
    return StartServer( argc, argv );
}

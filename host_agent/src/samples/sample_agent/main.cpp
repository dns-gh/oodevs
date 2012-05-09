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
#include <host/Json.h>
#include <host/Node_ABC.h>
#include <host/NodeController.h>
#include <host/Pool.h>
#include <host/PortFactory.h>
#include <host/Proxy.h>
#include <host/SecurePool.h>
#include <host/SessionController.h>
#include <host/UuidFactory.h>
#include <runtime/Runtime_ABC.h>
#include <runtime/Utf8.h>
#include <web/Client.h>
#include <web/Controller.h>
#include <web/Server.h>

#define  CPPLOG_THREADING
#include <cpplog/cpplog.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>

using runtime::Utf8Convert;

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

    bool Parse( cpplog::BaseLogger& log, int argc, const char* argv[] )
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
            {
                LOG_ERROR( log ) << "[cfg] Unknown parameter " << argv[i];
                return false;
            }
        }
        return true;
    }
};

void Start( cpplog::BaseLogger& log, const runtime::Runtime_ABC& runtime, const host::FileSystem_ABC& system, Configuration& cfg )
{
    host::Pool pool( 8 );
    host::UuidFactory uuids;
    web::Client client;
    host::Proxy proxy( log, runtime, system, cfg.java, cfg.proxy.jar, cfg.ports.proxy, client, pool );
    proxy.Register( "api", "localhost", cfg.ports.host );
    host::PortFactory ports( cfg.ports.period, cfg.ports.min, cfg.ports.max );
    host::NodeController nodes( log, runtime, system, uuids, proxy, cfg.java, cfg.node.jar, cfg.node.root, "node", pool, ports );
    host::NodeController cluster( log, runtime, system, uuids, proxy, cfg.java, cfg.node.jar, cfg.node.root, "cluster", pool, ports );
    host::SessionController sessions( log, runtime, system, uuids, cfg.session.data, cfg.session.applications, pool, ports );
    host::Agent agent( log, cfg.cluster.enabled ? &cluster : 0, nodes, sessions );
    web::Controller controller( log, agent );
    web::Server server( log, pool, controller, cfg.ports.host );
    server.Listen();
    host::SecurePool run( log, "server", pool );
    run.Post( boost::bind( &web::Server::Run, &server ) );
    getc( stdin );
    server.Stop();
}

template< typename T >
T GetTree( boost::property_tree::ptree& tree, const std::string& key, const T& value )
{
    const boost::optional< T > opt = tree.get_optional< T >( key );
    if( opt != boost::none )
        return *opt;
    tree.put< T >( key, value );
    return value;
}

struct NullLogger : public cpplog::BaseLogger
{
    virtual bool sendLogMessage( cpplog::LogData* /*logData*/ )
    {
        return true;
    }
};

boost::filesystem::path GetLogDir( int argc, const char* argv[] )
{
    std::wstring dir;
    for( int i = 0; i < argc; ++i )
        if( ReadParameter( dir, "--log_dir", i, argc, argv ) )
            return dir;
    NullLogger nil;
    runtime::Factory factory( nil );
    boost::filesystem::path reply = factory.GetRuntime().GetModuleFilename();
    reply.remove_filename();
    reply.remove_filename();
    return reply / "log";
}

int StartServer( int argc, const char* argv[] )
{
    const boost::filesystem::path logs = GetLogDir( argc-1, argv+1 );
    cpplog::TeeLogger tee(
        new cpplog::FileLogger( ( logs / "host_agent.log" ).string(), true ), true,
        new cpplog::StdErrLogger(), true );
    cpplog::BackgroundLogger log( tee );
    LOG_INFO( log ) << "Host Agent - (c) copyright MASA Group 2012";

    try
    {
        runtime::Factory factory( log );
        const runtime::Runtime_ABC& runtime = factory.GetRuntime();
        host::FileSystem system( log );

        boost::property_tree::ptree tree;
        const boost::filesystem::path root = runtime.GetModuleFilename().remove_filename();
        const boost::filesystem::path config = root / "host_agent.config";
        if( system.IsFile( config ) )
            tree = host::FromJson( system.ReadFile( config ) );

        const char* jhome = getenv( "JAVA_HOME" );
        const std::string java = GetTree( tree, "java", jhome ? std::string( jhome ) + "/bin/java.exe" : "" );
        if( java.empty() )
            throw std::runtime_error( "Missing JAVA_HOME environment variable. Please install a Java Runtime Environment" );

        Configuration cfg;
        cfg.ports.host           = GetTree( tree, "ports.host", 15000 );
        cfg.ports.period         = GetTree( tree, "ports.period", 40 );
        cfg.ports.min            = GetTree( tree, "ports.min", 50000 );
        cfg.ports.max            = GetTree( tree, "ports.max", 60000 );
        cfg.ports.proxy          = GetTree( tree, "ports.proxy", 8080 );
        cfg.cluster.enabled      = GetTree( tree, "cluster.enabled", true );
        cfg.java                 = Utf8Convert( java );
        cfg.proxy.jar            = Utf8Convert( GetTree( tree, "proxy.jar",            Utf8Convert( root / "proxy.jar" ) ) );
        cfg.node.jar             = Utf8Convert( GetTree( tree, "node.jar",             Utf8Convert( root / "node.jar" ) ) );
        cfg.node.root            = Utf8Convert( GetTree( tree, "node.root",            Utf8Convert( root / "../www" ) ) );
        cfg.session.data         = Utf8Convert( GetTree( tree, "session.data",         Utf8Convert( root / "../data" ) ) );
        cfg.session.applications = Utf8Convert( GetTree( tree, "session.applications", Utf8Convert( root / "../bin" ) ) );

        system.WriteFile( root / "host_agent.config", host::ToJson( tree, true ) );

        bool valid = cfg.Parse( log, argc-1, argv+1 );
        if( !valid )
            return -1;
        Start( log, runtime, system, cfg );
    }
    catch( const std::runtime_error& err )
    {
        LOG_ERROR( log ) << "[main] Unable to start, " << err.what();
        return -1;
    }

    LOG_INFO( log ) << "Host Agent - Exit";
    return 0;
}
}

int main( int argc, const char* argv[] )
{
    return StartServer( argc, argv );
}
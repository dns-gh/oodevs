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
#include <host/Node.h>
#include <host/NodeController.h>
#include <host/Package.h>
#include <host/Pool.h>
#include <host/PortFactory.h>
#include <host/PropertyTree.h>
#include <host/Proxy.h>
#include <host/Session.h>
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
#include <boost/make_shared.hpp>
#include <boost/property_tree/ptree.hpp>

using namespace host;
using runtime::Utf8Convert;

namespace
{
typedef boost::filesystem::path Path;
typedef boost::property_tree::ptree Tree;

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
    std::wstring root;
    std::wstring java;
    struct
    {
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
            found |= ReadParameter( root, "--root", i, argc, argv );
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

struct NodeFactory : public NodeFactory_ABC
{
    NodeFactory( const PackageFactory_ABC& packages,
                 const FileSystem_ABC& system,
                 const runtime::Runtime_ABC& runtime,
                 const UuidFactory_ABC& uuids,
                 PortFactory_ABC& ports,
                 Pool_ABC& pool )
        : packages( packages )
        , system  ( system )
        , runtime ( runtime )
        , uuids   ( uuids )
        , ports   ( ports )
        , pool    ( pool )
    {
        // NOTHING
    }

    Ptr Make( const Path& root, const std::string& name ) const
    {
        return boost::make_shared< Node >( packages, system, uuids, pool, root, name, ports );
    }

    Ptr Make( const Path& tag ) const
    {
        return boost::make_shared< Node >( packages, system, uuids, pool, Path( tag ).remove_filename(), FromJson( system.ReadFile( tag ) ), runtime, ports );
    }

    const PackageFactory_ABC& packages;
    const FileSystem_ABC& system;
    const runtime::Runtime_ABC& runtime;
    const UuidFactory_ABC& uuids;
    PortFactory_ABC& ports;
    Pool_ABC& pool;
};

struct PackageFactory : public PackageFactory_ABC
{
    PackageFactory( Pool_ABC& pool, const FileSystem_ABC& system )
        : pool  ( pool )
        , system( system )
    {
        // NOTHING
    }

    boost::shared_ptr< Package_ABC > Make( const Path& path, bool reference ) const
    {
        return boost::make_shared< Package >( pool, system, path, reference );
    }

    Pool_ABC& pool;
    const FileSystem_ABC& system;
};

struct SessionFactory : public SessionFactory_ABC
{
    SessionFactory( const runtime::Runtime_ABC& runtime, const UuidFactory_ABC& uuids, PortFactory_ABC& ports )
        : runtime( runtime )
        , uuids  ( uuids )
        , ports  ( ports )
    {
        // NOTHING
    }

    Ptr Make( const Uuid& node, const std::string& name, const std::string& exercise ) const
    {
        return boost::make_shared< Session >( uuids.Create(), node, name, exercise, ports.Create() );
    }

    Ptr Make( const Tree& tree ) const
    {
        return boost::make_shared< Session >( tree, runtime, ports );
    }

    const runtime::Runtime_ABC& runtime;
    const UuidFactory_ABC& uuids;
    PortFactory_ABC& ports;
};

void Start( cpplog::BaseLogger& log, const runtime::Runtime_ABC& runtime, const FileSystem_ABC& system, Configuration& cfg )
{
    Pool pool( 8 );
    UuidFactory uuids;
    web::Client client;
    Proxy proxy( log, runtime, system, cfg.root + L"/log", cfg.java, cfg.proxy.jar, cfg.ports.proxy, client, pool );
    PortFactory ports( cfg.ports.period, cfg.ports.min, cfg.ports.max );
    PackageFactory packages( pool, system );
    NodeFactory fnodes( packages, system, runtime, uuids, ports, pool );
    NodeController nodes( log, runtime, system, proxy, fnodes, cfg.root, cfg.java, cfg.node.jar, cfg.node.root, "node", pool );
    NodeController cluster( log, runtime, system, proxy, fnodes, cfg.root, cfg.java, cfg.node.jar, cfg.node.root, "cluster", pool );
    SessionFactory fsessions( runtime, uuids, ports );
    SessionController sessions( log, runtime, system, fsessions, cfg.root + L"/log", cfg.session.data, cfg.session.applications, pool );
    Agent agent( log, cfg.cluster.enabled ? &cluster : 0, nodes, sessions );
    web::Controller controller( log, agent );
    const std::auto_ptr< Port_ABC > host = ports.Create();
    web::Server server( log, pool, controller, host->Get() );
    server.Listen();
    proxy.Register( "api", "localhost", host->Get() );
    getc( stdin );
}

template< typename T >
T GetTree( Tree& tree, const std::string& key, const T& value )
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

Path GetRootDir( int argc, const char* argv[] )
{
    std::wstring dir;
    for( int i = 0; i < argc; ++i )
        if( ReadParameter( dir, "--root", i, argc, argv ) )
            return dir;
    NullLogger nil;
    runtime::Factory factory( nil );
    Path reply = factory.GetRuntime().GetModuleFilename();
    reply.remove_filename();
    reply.remove_filename();
    return reply;
}

int StartServer( int argc, const char* argv[] )
{
    const Path root = GetRootDir( argc-1, argv+1 );
    cpplog::TeeLogger tee(
        new cpplog::FileLogger( ( root / "log" / "host_agent.log" ).string(), true ), true,
        new cpplog::StdErrLogger(), true );
    cpplog::BackgroundLogger log( tee );
    LOG_INFO( log ) << "Host Agent - (c) copyright MASA Group 2012";

    try
    {
        runtime::Factory factory( log );
        const runtime::Runtime_ABC& runtime = factory.GetRuntime();
        FileSystem system( log );

        Tree tree;
        const Path config = root / "host" / "host_agent.config";
        if( system.IsFile( config ) )
            tree = FromJson( system.ReadFile( config ) );

        const char* jhome = getenv( "JAVA_HOME" );
        const std::string java = GetTree( tree, "java", jhome ? std::string( jhome ) + "/bin/java.exe" : "" );
        if( java.empty() )
            throw std::runtime_error( "Missing JAVA_HOME environment variable. Please install a Java Runtime Environment" );

        const Path bin = runtime.GetModuleFilename().remove_filename();
        Configuration cfg;
        cfg.root                 = Utf8Convert( GetTree( tree, "root", Utf8Convert( root ) ) );
        cfg.ports.period         = GetTree( tree, "ports.period", 40 );
        cfg.ports.min            = GetTree( tree, "ports.min", 50000 );
        cfg.ports.max            = GetTree( tree, "ports.max", 60000 );
        cfg.ports.proxy          = GetTree( tree, "ports.proxy", 8080 );
        cfg.cluster.enabled      = GetTree( tree, "cluster.enabled", true );
        cfg.java                 = Utf8Convert( java );
        cfg.proxy.jar            = Utf8Convert( GetTree( tree, "proxy.jar",            Utf8Convert( bin / "proxy.jar" ) ) );
        cfg.node.jar             = Utf8Convert( GetTree( tree, "node.jar",             Utf8Convert( bin / "node.jar" ) ) );
        cfg.node.root            = Utf8Convert( GetTree( tree, "node.root",            Utf8Convert( bin / "../www" ) ) );
        cfg.session.data         = Utf8Convert( GetTree( tree, "session.data",         Utf8Convert( bin / "../data" ) ) );
        cfg.session.applications = Utf8Convert( GetTree( tree, "session.applications", Utf8Convert( bin ) ) );

        system.WriteFile( root / "host" / "host_agent.config", ToJson( tree, true ) );

        bool valid = cfg.Parse( log, argc-1, argv+1 );
        if( !valid )
            return -1;
        Start( log, runtime, system, cfg );
    }
    catch( const std::exception& err )
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

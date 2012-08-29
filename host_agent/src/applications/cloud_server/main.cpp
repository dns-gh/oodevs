// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include <host/Agent.h>
#include <host/Crypt.h>
#include <host/Node.h>
#include <host/NodeController.h>
#include <host/Package.h>
#include <host/PortFactory.h>
#include <host/Proxy.h>
#include <host/Session.h>
#include <host/SessionController.h>
#include <host/Sql.h>
#include <host/UserController.h>
#include <host/UuidFactory.h>
#include <runtime/CrashHandler.h>
#include <runtime/Daemon.h>
#include <runtime/Factory.h>
#include <runtime/FileSystem.h>
#include <runtime/Pool.h>
#include <runtime/PropertyTree.h>
#include <runtime/Runtime_ABC.h>
#include <runtime/Scoper.h>
#include <runtime/Utf8.h>
#include <web/Client.h>
#include <web/Controller.h>
#include <web/Plugins.h>
#include <web/Server.h>

#define  CPPLOG_THREADING
#include <cpplog/cpplog.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/nil_generator.hpp>

#include <sqlite/sqlite3.h>

using namespace host;
using namespace runtime;
using namespace property_tree;
typedef runtime::Daemon::T_Waiter Waiter;

#ifdef _WIN32
#define main main_
int main( int argc, const char* argv[] );
int wmain( int argc, wchar_t* argv[], wchar_t* /*env*/ )
{
    std::vector< std::string > args;
    for( int i = 0; i < argc; ++i )
        args.push_back( Utf8Convert( std::wstring( argv[i] ) ) );
    std::vector< const char* > ptrs;
    for( int i = 0; i < argc; ++i )
        ptrs.push_back( args.at( i ).c_str() );
    main( argc, &ptrs[0] );
}
#endif

namespace
{
typedef boost::filesystem::path Path;
typedef boost::property_tree::ptree Tree;

const std::string serviceName = "Sword Cloud";

template< typename T >
bool ReadSingle( T& cmd, const std::string& name, const char* argv, const T& value )
{
    if( name != argv )
        return false;
    cmd = value;
    return true;
}

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

template<>
bool ReadParameter( Path& dst, const std::string& name, int& idx, int argc, const char* argv[] )
{
    if( name != argv[idx] )
        return false;
    if( ++idx >= argc )
        throw std::runtime_error( "missing " + name + " parameter" );
    dst = Utf8Convert( std::string( argv[idx] ) );
    return true;
}

bool ReadToggle( bool& dst, const std::string& name, const std::string& value )
{
    if( name != value )
        return false;
    dst = true;
    return true;
}

enum Command
{
    CMD_REGISTER,
    CMD_UNREGISTER,
    CMD_DAEMON,
    CMD_EXECUTE,
};

struct Configuration
{
    Command command;
    Path root;
    struct
    {
        int proxy;
        int period;
        int min;
        int max;
    } ports;
    struct
    {
        Path app;
    } proxy;
    struct
    {
        bool enabled;
        Path certificate;
        Path key;
    } ssl;
    struct
    {
        bool enabled;
    } cluster;
    struct
    {
        Path app;
        Path root;
        int min_play_seconds;
    } node;
    struct
    {
        Path apps;
    } session;

    bool Parse( cpplog::BaseLogger& log, int argc, const char* argv[] )
    {
        command = CMD_EXECUTE;
        for( int i = 0; i < argc; ++i )
        {
            bool found = false;
            found |= ReadSingle( command, "--register", argv[i], CMD_REGISTER );
            found |= ReadSingle( command, "--unregister", argv[i], CMD_UNREGISTER );
            found |= ReadSingle( command, "--daemon", argv[i], CMD_DAEMON );
            found |= ReadParameter( root, "--root", i, argc, argv );
            found |= ReadParameter( ports.period, "--port_period", i, argc, argv );
            found |= ReadParameter( ports.min, "--port_min", i, argc, argv );
            found |= ReadParameter( ports.max, "--port_max", i, argc, argv );
            found |= ReadParameter( ports.proxy, "--port_proxy", i, argc, argv );
            found |= ReadParameter( cluster.enabled, "--cluster", i, argc, argv );
            found |= ReadParameter( proxy.app, "--proxy", i, argc, argv );
            found |= ReadParameter( node.app, "--node", i, argc, argv );
            found |= ReadParameter( node.root, "--node_root", i, argc, argv );
            found |= ReadParameter( node.min_play_seconds, "--node_min_play", i, argc, argv );
            found |= ReadParameter( session.apps, "--session_apps", i, argc, argv );
            found |= ReadToggle( ssl.enabled, "--ssl", argv[i] );
            found |= ReadParameter( ssl.certificate, "--ssl_certificate", i, argc, argv );
            found |= ReadParameter( ssl.key, "--ssl_key", i, argc, argv );
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
                 const Runtime_ABC& runtime,
                 const UuidFactory_ABC& uuids,
                 const web::Plugins& plugins,
                 PortFactory_ABC& ports,
                 int min_play,
                 Pool_ABC& pool )
        : packages( packages )
        , system  ( system )
        , runtime ( runtime )
        , uuids   ( uuids )
        , plugins ( plugins )
        , ports   ( ports )
        , pool    ( pool )
        , min_play( min_play )
        , observer( 0 )
    {
        // NOTHING
    }

    Ptr Make( const Path& root, const std::string& ident, const web::node::Config& cfg ) const
    {
        NodeDependencies deps( packages, system, uuids, *observer, runtime, plugins, pool, ports );
        return boost::make_shared< Node >( deps, root, min_play, ident, cfg );
    }

    Ptr Make( const Path& tag ) const
    {
        NodeDependencies deps( packages, system, uuids, *observer, runtime, plugins, pool, ports );
        return boost::make_shared< Node >( deps, Path( tag ).remove_filename(), min_play,
                                           FromJson( deps.system.ReadFile( tag ) ) );
    }

    const PackageFactory_ABC& packages;
    const FileSystem_ABC& system;
    const Runtime_ABC& runtime;
    const UuidFactory_ABC& uuids;
    const web::Plugins& plugins;
    const int min_play;
    PortFactory_ABC& ports;
    Pool_ABC& pool;
    NodeObserver_ABC* observer;
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
        return boost::make_shared< Package >( boost::ref( pool ), system, path, reference );
    }

    Pool_ABC& pool;
    const FileSystem_ABC& system;
};

struct SessionFactory : public SessionFactory_ABC
{
    SessionFactory( const FileSystem_ABC& system,
                    const Runtime_ABC& runtime,
                    const web::Plugins& plugins,
                    const UuidFactory_ABC& uuids,
                    const NodeController_ABC& nodes,
                    PortFactory_ABC& ports,
                    web::Client_ABC& client,
                    Pool_ABC& pool )
        : nodes  ( nodes )
        , deps   ( system, runtime, plugins, uuids, client, pool, ports )
    {
        // NOTHING
    }

    Session_ABC::T_Ptr Make( const Path& root, const Path& trash, const Uuid& id, const web::session::Config& cfg, const std::string& exercise ) const
    {
        NodeController_ABC::T_Node node = nodes.Get( id );
        if( !node )
            return Session_ABC::T_Ptr();
        SessionPaths paths( root, trash );
        return boost::make_shared< Session >( boost::cref( deps ), node, paths, boost::cref( cfg ), exercise, boost::uuids::nil_uuid() );
    }

    Session_ABC::T_Ptr Make( const Path& tag, const Path& trash ) const
    {
        const Tree tree = FromJson( deps.system.ReadFile( tag ) );
        const boost::optional< std::string > id = tree.get_optional< std::string >( "node" );
        if( id == boost::none )
            throw std::runtime_error( "missing node id in " + Utf8Convert( tag ) );
        NodeController_ABC::T_Node node = nodes.Get( boost::uuids::string_generator()( *id ) );
        if( !node )
            throw std::runtime_error( "unknown node " + *id );
        SessionPaths paths( Path( tag ).remove_filename(), trash );
        return boost::make_shared< Session >( boost::cref( deps ), node, paths, tree );
    }

    const NodeController_ABC& nodes;
    const SessionDependencies deps;
};

int Start( cpplog::BaseLogger& log, const runtime::Runtime_ABC& runtime, const FileSystem_ABC& system, const Configuration& cfg, const Waiter& waiter )
{
    sqlite3_config( SQLITE_CONFIG_SINGLETHREAD );
    sqlite3_initialize();
    Scoper sqliteShutdown( &sqlite3_shutdown );
    Pool pool( 8 );
    UuidFactory uuids;
    web::Client client;
    const proxy::Ssl ssl( cfg.ssl.enabled, cfg.ssl.certificate, cfg.ssl.key );
    const proxy::Config proxyConfig( cfg.root / "host", cfg.proxy.app, cfg.ports.proxy, ssl );
    Proxy proxy( log, runtime, system, proxyConfig, client, pool );
    PortFactory ports( cfg.ports.period, cfg.ports.min, cfg.ports.max );
    PackageFactory packages( pool, system );
    web::Plugins plugins( system, cfg.session.apps / "plugins" );
    NodeFactory fnodes( packages, system, runtime, uuids, plugins, ports, cfg.node.min_play_seconds, pool );
    const Port host = ports.Create();
    NodeController nodes( log, runtime, system, plugins, fnodes, cfg.root, cfg.node.app, cfg.node.root, "node", host->Get(), pool, proxy );
    fnodes.observer = &nodes;
    NodeController cluster( log, runtime, system, plugins, fnodes, cfg.root, cfg.node.app, cfg.node.root, "cluster", host->Get(), pool, proxy );
    SessionFactory fsessions( system, runtime, plugins, uuids, nodes, ports, client, pool );
    SessionController sessions( log, runtime, system, fsessions, nodes, cfg.root, cfg.session.apps, pool );
    Agent agent( log, cfg.cluster.enabled ? &cluster : 0, nodes, sessions );
    Crypt crypt;
    Sql db( cfg.root / "host" / "host_agent.db" );
    UserController users( log, crypt, uuids, db );
    web::Controller controller( plugins, log, agent, users, true );
    web::Server server( log, pool, controller, host->Get() );
    server.Listen();
    proxy.Register( "api", "localhost", host->Get() );
    waiter();
    return 0;
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

Path GetSinglePath( int argc, const char** argv, const std::string& name )
{
    Path reply;
    for( int i = 0; i < argc; ++i )
        if( ReadParameter( reply, name, i, argc, argv ) )
            return reply;
    return reply;
}

Path GetRootDir( int argc, const char* argv[] )
{
    Path reply = GetSinglePath( argc, argv, "--root" );
    if( !reply.empty() )
        return reply;
    NullLogger nil;
    return runtime::Factory( nil ).GetRuntime().GetModuleFilename().remove_filename().remove_filename();
}

void PrintConfiguration( cpplog::BaseLogger& log, const Configuration& cfg )
{
    LOG_INFO( log ) << "[cfg] root "                  << cfg.root;
    LOG_INFO( log ) << "[cfg] ports.period "          << cfg.ports.period;
    LOG_INFO( log ) << "[cfg] ports.min "             << cfg.ports.min;
    LOG_INFO( log ) << "[cfg] ports.max "             << cfg.ports.max;
    LOG_INFO( log ) << "[cfg] ports.proxy "           << cfg.ports.proxy;
    LOG_INFO( log ) << "[cfg] cluster.enabled "       << ( cfg.cluster.enabled ? "true" : "false" );
    LOG_INFO( log ) << "[cfg] proxy.app "             << cfg.proxy.app;
    LOG_INFO( log ) << "[cfg] node.app "              << cfg.node.app;
    LOG_INFO( log ) << "[cfg] node.root "             << cfg.node.root;
    LOG_INFO( log ) << "[cfg] node.min_play_seconds " << cfg.node.min_play_seconds;
    LOG_INFO( log ) << "[cfg] session.apps "          << cfg.session.apps;
    LOG_INFO( log ) << "[cfg] ssl "                   << ( cfg.ssl.enabled ? "true" : "false" );
    if( cfg.ssl.enabled )
    {
        LOG_INFO( log ) << "[cfg] ssl.certificate "       << cfg.ssl.certificate;
        LOG_INFO( log ) << "[cfg] ssl.key "               << cfg.ssl.key;
    }
}

Configuration ParseConfiguration( const runtime::Runtime_ABC& runtime, const FileSystem_ABC& system,
                                  const Path& root, cpplog::BaseLogger& log, int argc, const char* argv[] )
{
    const Path module = runtime.GetModuleFilename();
    const Path config = Path( module ).replace_extension( ".config" );

    Tree tree;
    if( system.IsFile( config ) )
        tree = FromJson( system.ReadFile( config ) );

    const Path bin = Path( module ).remove_filename();
    Configuration cfg;
    cfg.root                  = Utf8Convert( GetTree( tree, "root", Utf8Convert( root ) ) );
    cfg.ports.period          = GetTree( tree, "ports.period", 40 );
    cfg.ports.min             = GetTree( tree, "ports.min", 50000 );
    cfg.ports.max             = GetTree( tree, "ports.max", 60000 );
    cfg.ports.proxy           = GetTree( tree, "ports.proxy", 8080 );
    cfg.cluster.enabled       = GetTree( tree, "cluster.enabled", true );
    cfg.ssl.enabled           = GetTree( tree, "ssl.enabled", false );
    cfg.ssl.certificate       = Utf8Convert( GetTree( tree, "ssl.certificate", Utf8Convert( bin / "certificate.pem" ) ) );
    cfg.ssl.key               = Utf8Convert( GetTree( tree, "ssl.key", Utf8Convert( bin / "key.pem" ) ) );
    cfg.proxy.app             = Utf8Convert( GetTree( tree, "proxy.app",    Utf8Convert( bin / "proxy.exe" ) ) );
    cfg.node.app              = Utf8Convert( GetTree( tree, "node.app",     Utf8Convert( bin / "node.exe" ) ) );
    cfg.node.root             = Utf8Convert( GetTree( tree, "node.root",    Utf8Convert( bin / ".." / "www" ) ) );
    cfg.node.min_play_seconds = GetTree( tree, "node.min_play_s", 5*60 );
    cfg.session.apps          = Utf8Convert( GetTree( tree, "session.apps", Utf8Convert( bin ) ) );
    system.WriteFile( config, ToJson( tree, true ) );

    bool valid = cfg.Parse( log, argc, argv );
    if( !valid )
        throw std::runtime_error( "Invalid configuration" );

    return cfg;
}

bool IsCommand( const char* arg )
{
    if( !strcmp( arg, "--register" ) )
        return true;
    if( !strcmp( arg, "--unregister" ) )
        return true;
    if( !strcmp( arg, "--daemon" ) )
        return true;
    return false;
}

void ConsoleWaiter()
{
    getc( stdin );
}

int StartServer( int argc, const char* argv[], const Waiter& waiter )
{
    const Path root = GetRootDir( argc-1, argv+1 );
    boost::filesystem::create_directories( root / "host" );
    cpplog::TeeLogger tee(
        new cpplog::FileLogger( ( root / "host" / "host_agent.log" ).string(), true ), true,
        new cpplog::StdErrLogger(), true );
    cpplog::BackgroundLogger log( tee );
    LOG_INFO( log ) << "Host Agent - (c) copyright MASA Group 2012";

    try
    {
        runtime::Factory factory( log );
        const runtime::Runtime_ABC& runtime = factory.GetRuntime();
        FileSystem system( log );
        Configuration cfg = ParseConfiguration( runtime, system, root, log, argc-1, argv+1 );
        runtime::Daemon daemon( log, runtime );
        runtime::Daemon::T_Args args;
        switch( cfg.command )
        {
        case CMD_REGISTER:
            args.push_back( "--daemon" );
            for( int i = 1; i < argc; ++i )
                if( !IsCommand( argv[i] ) )
                    args.push_back( argv[i] );
            daemon.Register( serviceName, args, std::string(), std::string() );
            break;

        case CMD_UNREGISTER:
            daemon.Unregister( serviceName );
            break;

        case CMD_DAEMON:
            PrintConfiguration( log, cfg );
            daemon.Run( boost::bind( &Start, boost::ref( log ), boost::cref( runtime ), boost::cref( system ), boost::cref( cfg ), _1 ) );
            break;

        default:
        case CMD_EXECUTE:
            PrintConfiguration( log, cfg );
            Start( log, runtime, system, cfg, waiter );
            break;
        }
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log ) << "[main] " << err.what();
        LOG_ERROR( log ) << "[main] Unable to start";
        return -1;
    }

    LOG_INFO( log ) << "Host Agent - Exit";
    return 0;
}
}

int main( int argc, const char* argv[] )
{
    CrashHandlerInit();
    return StartServer( argc, argv, &ConsoleWaiter );
}

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
#include <runtime/Utf8.h>
#include <web/Client.h>
#include <web/Controller.h>
#include <web/Plugins.h>
#include <web/Server.h>

#define  CPPLOG_THREADING
#include <cpplog/cpplog.hpp>

#pragma warning( push, 0 )
#include <Qt/qsettings.h>
#include <Qt/qstring.h>
#pragma warning( pop )

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
        args.push_back( Utf8( std::wstring( argv[i] ) ) );
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
    dst = Utf8( std::string( argv[idx] ) );
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
        int http;
        int tcp;
        int period;
        int min;
        int max;
    } ports;
    struct
    {
        std::string service;
        std::string user;
        std::string display;
        std::string password;
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
        Path simulation;
        Path replayer;
        Path timeline;
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
            found |= ReadParameter( ports.http, "--port_http", i, argc, argv );
            found |= ReadParameter( ports.tcp, "--port_tcp", i, argc, argv );
            found |= ReadParameter( cluster.enabled, "--cluster", i, argc, argv );
            found |= ReadParameter( proxy.service, "--service", i, argc, argv );
            found |= ReadParameter( proxy.user, "--user", i, argc, argv );
            found |= ReadParameter( proxy.display, "--display", i, argc, argv );
            found |= ReadParameter( proxy.password, "--password", i, argc, argv );
            found |= ReadParameter( proxy.app, "--proxy", i, argc, argv );
            found |= ReadParameter( node.app, "--node", i, argc, argv );
            found |= ReadParameter( node.root, "--node_root", i, argc, argv );
            found |= ReadParameter( node.min_play_seconds, "--node_min_play", i, argc, argv );
            found |= ReadParameter( session.simulation, "--simulation", i, argc, argv );
            found |= ReadParameter( session.replayer, "--replayer", i, argc, argv );
            found |= ReadParameter( session.timeline, "--timeline", i, argc, argv );
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
                 const FileSystem_ABC& fs,
                 const Runtime_ABC& runtime,
                 const UuidFactory_ABC& uuids,
                 const web::Plugins& plugins,
                 PortFactory_ABC& ports,
                 int min_play,
                 Pool_ABC& pool )
        : packages( packages )
        , fs      ( fs )
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
        NodeDependencies deps( packages, fs, uuids, *observer, runtime, plugins, pool, ports );
        return boost::make_shared< Node >( deps, root, min_play, ident, cfg );
    }

    Ptr Make( const Path& tag ) const
    {
        NodeDependencies deps( packages, fs, uuids, *observer, runtime, plugins, pool, ports );
        return boost::make_shared< Node >( deps, Path( tag ).remove_filename(), min_play,
                                           FromJson( deps.fs.ReadFile( tag ) ) );
    }

    const PackageFactory_ABC& packages;
    const FileSystem_ABC& fs;
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
    PackageFactory( Pool_ABC& pool, const FileSystem_ABC& fs )
        : pool( pool )
        , fs  ( fs )
    {
        // NOTHING
    }

    boost::shared_ptr< Package_ABC > Make( const Path& path, bool reference ) const
    {
        return boost::make_shared< Package >( boost::ref( pool ), fs, path, reference );
    }

    Pool_ABC& pool;
    const FileSystem_ABC& fs;
};

struct SessionFactory : public SessionFactory_ABC
{
    SessionFactory( const FileSystem_ABC& fs,
                    const Runtime_ABC& runtime,
                    const web::Plugins& plugins,
                    const UuidFactory_ABC& uuids,
                    const NodeController_ABC& nodes,
                    cpplog::BaseLogger& log,
                    PortFactory_ABC& ports,
                    web::Client_ABC& client,
                    Pool_ABC& pool )
        : deps( fs, runtime, plugins, nodes, uuids, log, client, pool, ports )
    {
        // NOTHING
    }

    Session_ABC::T_Ptr Make( const Path& root, const Path& trash, const Uuid& id, const web::session::Config& cfg, const std::string& exercise, const web::User& owner ) const
    {
        NodeController_ABC::T_Node node = deps.nodes.Get( id );
        if( !node )
            return Session_ABC::T_Ptr();
        SessionPaths paths( root, trash );
        return boost::make_shared< Session >( boost::cref( deps ), node, paths, boost::cref( cfg ), exercise, boost::uuids::nil_uuid(), owner );
    }

    Session_ABC::T_Ptr Make( const Path& tag, const Path& trash ) const
    {
        const Tree tree = FromJson( deps.fs.ReadFile( tag ) );
        const boost::optional< std::string > id = tree.get_optional< std::string >( "node" );
        if( id == boost::none )
            throw std::runtime_error( "missing node id in " + Utf8( tag ) );
        NodeController_ABC::T_Node node = deps.nodes.Get( boost::uuids::string_generator()( *id ) );
        if( !node )
            throw std::runtime_error( "unknown node " + *id );
        SessionPaths paths( Path( tag ).remove_filename(), trash );
        return boost::make_shared< Session >( boost::cref( deps ), node, paths, tree );
    }

    const SessionDependencies deps;
};

struct SqlFacade
{
    SqlFacade()
    {
        sqlite3_config( SQLITE_CONFIG_SINGLETHREAD );
        sqlite3_initialize();
    }
    ~SqlFacade()
    {
        sqlite3_shutdown();
    }
};

struct Facade : SqlFacade
{
    Facade( cpplog::BaseLogger& log, const Configuration& cfg )
        : log  ( log )
        , cfg  ( cfg )
        , db   ( cfg.root / "host" / "host_agent.db" )
        , users( log, crypt, uuids, db )
    {
        if( users.CountUsers( boost::uuids::nil_uuid() ) == 0 )
        {
            if( cfg.proxy.user.empty() )
                throw std::runtime_error( "no administrator account, use --user to create one" );
            if( cfg.proxy.display.empty() )
                throw std::runtime_error( "user display name empty, use --display to set it" );
            if( cfg.proxy.password.empty() )
                throw std::runtime_error( "user password empty, use --password to set it" );
            users.CreateUser( boost::uuids::nil_uuid(), cfg.proxy.user, cfg.proxy.display, cfg.proxy.password, web::USER_TYPE_ADMINISTRATOR, false );
        }
    }

    void AddLicensePathToReg( const std::string& value )
    {
        QSettings settings( "HKEY_CURRENT_USER\\Software\\FLEXlm License Manager", QSettings::NativeFormat );
        QString currentValue = settings.value( "BKMASA_LICENSE_FILE" ).toString();
        if( !currentValue.contains( value.c_str() ) )
            settings.setValue( "BKMASA_LICENSE_FILE", currentValue + ";" + value.c_str() );
    }

    int Start( const runtime::Runtime_ABC& runtime, const FileSystem_ABC& fs, const Waiter& waiter )
    {
        Pool pool( 32, 256 );
        web::Client client;
        const proxy::Ssl ssl( cfg.ssl.enabled, cfg.ssl.certificate, cfg.ssl.key );
        const proxy::Config proxyConfig( cfg.root / "host", cfg.proxy.app, cfg.ports.http, cfg.ports.tcp, ssl );
        Proxy proxy( log, runtime, fs, proxyConfig, client, pool );
        PortFactory ports( cfg.ports.period, cfg.ports.min, cfg.ports.max );
        PackageFactory packages( pool, fs );
        web::Plugins plugins( fs, Path( cfg.session.simulation ).remove_filename() / "plugins" );
        NodeFactory fnodes( packages, fs, runtime, uuids, plugins, ports, cfg.node.min_play_seconds, pool );
        const Port host = ports.Create();
        const Path client_root = cfg.root / "client";
        NodeController nodes( log, runtime, fs, plugins, fnodes, cfg.root, cfg.node.app, cfg.node.root,
            client_root, cfg.session.simulation.parent_path(), "node", host->Get(), cfg.ports.tcp, pool, proxy );
        fnodes.observer = &nodes;
        NodeController cluster( log, runtime, fs, plugins, fnodes, cfg.root, cfg.node.app, cfg.node.root,
            Path(), cfg.session.simulation.parent_path(), "cluster", host->Get(), cfg.ports.tcp, pool, proxy );
        SessionFactory fsessions( fs, runtime, plugins, uuids, nodes, log, ports, client, pool );
        SessionController sessions( log, runtime, fs, fsessions, nodes, cfg.root, cfg.session.simulation, cfg.session.replayer, cfg.session.timeline, pool );
        Agent agent( log, cfg.cluster.enabled ? &cluster : 0, nodes, sessions );
        web::Controller controller( plugins, log, agent, users, true );
        web::Server server( runtime, log, pool, controller, host->Get() );
        server.Listen();
        proxy.Register( "api", "localhost", host->Get() );
        AddLicensePathToReg( cfg.session.simulation.parent_path().string() );
        waiter();
        return 0;
    }
    cpplog::BaseLogger& log;
    const Configuration& cfg;
    UuidFactory uuids;
    Crypt crypt;
    Sql db;
    UserController users;
};

template< typename T >
T GetTree( Tree& tree, const std::string& key, const T& value )
{
    const boost::optional< T > opt = tree.get_optional< T >( key );
    if( opt )
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
    Path path;
    for( int i = 0; i < argc; ++i )
        if( ReadParameter( path, name, i, argc, argv ) )
            return path;
    return path;
}

Path GetRootDir( int argc, const char* argv[] )
{
    Path path = GetSinglePath( argc, argv, "--root" );
    if( !path.empty() )
        return path;
    NullLogger nil;
    return runtime::Factory( nil ).GetRuntime().GetModuleFilename().remove_filename().remove_filename();
}

void PrintConfiguration( cpplog::BaseLogger& log, const Configuration& cfg )
{
    LOG_INFO( log ) << "[cfg] root "                  << cfg.root;
    LOG_INFO( log ) << "[cfg] ports.period "          << cfg.ports.period;
    LOG_INFO( log ) << "[cfg] ports.min "             << cfg.ports.min;
    LOG_INFO( log ) << "[cfg] ports.max "             << cfg.ports.max;
    LOG_INFO( log ) << "[cfg] ports.http "            << cfg.ports.http;
    LOG_INFO( log ) << "[cfg] ports.tcp "             << cfg.ports.tcp;
    LOG_INFO( log ) << "[cfg] cluster.enabled "       << ( cfg.cluster.enabled ? "true" : "false" );
    LOG_INFO( log ) << "[cfg] proxy.service "         << cfg.proxy.service;
    LOG_INFO( log ) << "[cfg] proxy.app "             << cfg.proxy.app;
    LOG_INFO( log ) << "[cfg] node.app "              << cfg.node.app;
    LOG_INFO( log ) << "[cfg] node.root "             << cfg.node.root;
    LOG_INFO( log ) << "[cfg] node.min_play_seconds " << cfg.node.min_play_seconds;
    LOG_INFO( log ) << "[cfg] session.simulation "    << cfg.session.simulation;
    LOG_INFO( log ) << "[cfg] session.replayer "      << cfg.session.replayer;
    LOG_INFO( log ) << "[cfg] session.timeline "      << cfg.session.timeline;
    LOG_INFO( log ) << "[cfg] ssl "                   << ( cfg.ssl.enabled ? "true" : "false" );
    if( cfg.ssl.enabled )
    {
        LOG_INFO( log ) << "[cfg] ssl.certificate "   << cfg.ssl.certificate;
        LOG_INFO( log ) << "[cfg] ssl.key "           << cfg.ssl.key;
    }
}

Configuration ParseConfiguration( const runtime::Runtime_ABC& runtime, const FileSystem_ABC& fs,
                                  const Path& root, cpplog::BaseLogger& log, int argc, const char* argv[] )
{
    const Path module = runtime.GetModuleFilename();
    const Path config = Path( module ).replace_extension( ".config" );

    Tree tree;
    if( fs.IsFile( config ) )
        tree = FromJson( fs.ReadFile( config ) );

    const Path bin = Path( module ).remove_filename();
    Configuration cfg;
    cfg.root                  = Utf8( GetTree( tree, "root", Utf8( root ) ) );
    cfg.ports.period          = GetTree( tree, "ports.period", 12 );
    cfg.ports.min             = GetTree( tree, "ports.min", 12000 );
    cfg.ports.max             = GetTree( tree, "ports.max", 15000 );
    cfg.ports.http            = GetTree( tree, "ports.http", 8080 );
    cfg.ports.tcp             = GetTree( tree, "ports.tcp",  cfg.ports.http+1 );
    cfg.cluster.enabled       = GetTree( tree, "cluster.enabled", true );
    cfg.ssl.enabled           = GetTree( tree, "ssl.enabled", false );
    cfg.ssl.certificate       = Utf8( GetTree( tree, "ssl.certificate", Utf8( bin / "certificate.pem" ) ) );
    cfg.ssl.key               = Utf8( GetTree( tree, "ssl.key", Utf8( bin / "key.pem" ) ) );
    cfg.proxy.service         = GetTree( tree, "proxy.service", std::string( "Sword Cloud" ) );
    cfg.proxy.app             = Utf8( GetTree( tree, "proxy.app", Utf8( bin / "proxy.exe" ) ) );
    cfg.node.app              = Utf8( GetTree( tree, "node.app", Utf8( bin / "node.exe" ) ) );
    cfg.node.root             = Utf8( GetTree( tree, "node.root", Utf8( bin / ".." / "www" ) ) );
    cfg.node.min_play_seconds = GetTree( tree, "node.min_play_s", 5 * 60 );
    cfg.session.simulation    = Utf8( GetTree( tree, "session.simulation", Utf8( bin / "simulation_app.exe" ) ) );
    cfg.session.replayer      = Utf8( GetTree( tree, "session.replayer", Utf8( bin / "replayer_app.exe" ) ) );
    cfg.session.timeline      = Utf8( GetTree( tree, "session.timeline", Utf8( bin / "timeline_server.exe" ) ) );
    fs.WriteFile( config, ToJson( tree ) );

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

int StartServer( int argc, const char* argv[] )
{
    const Path root = GetRootDir( argc - 1, argv + 1 );
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
        FileSystem fs( log );
        Configuration cfg = ParseConfiguration( runtime, fs, root, log, argc-1, argv+1 );
        runtime::Daemon daemon( log, runtime );
        runtime::Daemon::T_Args args;
        Facade facade( log, cfg );
        switch( cfg.command )
        {
        case CMD_REGISTER:
            args.push_back( "--daemon" );
            for( int i = 1; i < argc; ++i )
                if( !IsCommand( argv[i] ) )
                    args.push_back( argv[i] );
            daemon.Register( cfg.proxy.service, args, std::string(), std::string() );
            break;

        case CMD_UNREGISTER:
            daemon.Unregister( cfg.proxy.service );
            break;

        case CMD_DAEMON:
            PrintConfiguration( log, cfg );
            daemon.Run( boost::bind( &Facade::Start, boost::ref( facade ), boost::cref( runtime ), boost::cref( fs ), _1 ) );
            break;

        case CMD_EXECUTE:
        default:
            PrintConfiguration( log, cfg );
            facade.Start( runtime, fs, &std::getchar );
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
    return StartServer( argc, argv );
}

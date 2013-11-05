// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Session.h"

#include "Node_ABC.h"
#include "NodeController_ABC.h"
#include "PortFactory_ABC.h"
#include "UuidFactory_ABC.h"
#include "Error.h"
#include "runtime/Async.h"
#include "runtime/FileSystem_ABC.h"
#include "runtime/Process_ABC.h"
#include "runtime/PropertyTree.h"
#include "runtime/Runtime_ABC.h"
#include "runtime/Scoper.h"
#include "runtime/Utf8.h"
#include "web/Agent_ABC.h"
#include "web/Chunker_ABC.h"
#include "web/Client_ABC.h"
#include "web/HttpException.h"

#include <boost/assign/list_of.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/make_shared.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/algorithm.hpp>

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4702 )
#endif
#include <boost/xpressive/xpressive.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

using namespace host;
using namespace property_tree;
using namespace web::session;
using runtime::Async;
using runtime::FileSystem_ABC;
using runtime::Runtime_ABC;
using runtime::Utf8;
using web::Client_ABC;

namespace
{
enum SessionPort
{
    DISPATCHER_PORT,
    SIMULATION_PORT,
    WEB_CONTROL_PORT,
    DIA_DEBUGGER_PORT,
    NETWORK_LOGGER_PORT,
    TIMELINE_PORT,
    TIMELINE_WEB_PORT,
    SESSION_PORT_COUNT,
};

std::string ConvertStatus( Session::Status status )
{
    switch( status )
    {
        default:
        case Session::STATUS_STOPPED:   return "stopped";
        case Session::STATUS_PLAYING:   return "playing";
        case Session::STATUS_REPLAYING: return "replaying";
        case Session::STATUS_PAUSED:    return "paused";
        case Session::STATUS_ARCHIVED:  return "archived";
        case Session::STATUS_WAITING:   return "waiting";
    }
}

Session::Status ConvertStatus( const std::string& status )
{
    if( status == "playing" )   return Session::STATUS_PLAYING;
    if( status == "replaying" ) return Session::STATUS_REPLAYING;
    if( status == "paused" )    return Session::STATUS_PAUSED;
    if( status == "archived" )  return Session::STATUS_ARCHIVED;
    if( status == "waiting" )   return Session::STATUS_WAITING;
    return Session::STATUS_STOPPED;
}

Session::Status ConvertRemoteStatus( const std::string& status )
{
    if( status == "running" )   return Session::STATUS_PLAYING;
    if( status == "paused" )    return Session::STATUS_PAUSED;
    if( status == "loading" )   return Session::STATUS_PLAYING;
    //"stopped" is ignored as the session should quit anyway
    return Session::STATUS_COUNT;
}

std::string GetUrl( Session::Status status )
{
    if( status == Session::STATUS_PLAYING )
        return "/play";
    if( status == Session::STATUS_PAUSED )
        return "/pause";
    return std::string();
}

Port AcquirePort( int wanted, PortFactory_ABC& ports )
{
    try
    {
        return ports.Create( wanted );
    }
    catch( const std::exception& /*err*/ )
    {
        return ports.Create();
    }
}

Session::T_Process GetProcess( const Tree& tree, const std::string& key, const runtime::Runtime_ABC& runtime )
{
    const boost::optional< int > pid = tree.get_optional< int >( key + ".pid" );
    if( pid == boost::none )
        return Session::T_Process();
    return runtime.GetProcess( *pid );
}

Session::Status GetIdleStatus( bool replay )
{
    return replay ? Session::STATUS_WAITING : Session::STATUS_STOPPED;
}

Session::T_Process AcquireProcess( const Tree& tree, const std::string& key, const runtime::Runtime_ABC& runtime, int expected )
{
    Session::T_Process ptr = GetProcess( tree, key, runtime );
    if( !ptr  )
        return Session::T_Process();
    if( expected != Get< int >( tree, "port" ) || ptr->GetName() != Get< std::string >( tree, key + ".name" ) )
        return Session::T_Process();
    return ptr;
}

Path GetPathFrom( const Tree& src, const std::string& key )
{
    return Utf8( Get< std::string >( src, key ) );
}

template< typename T >
int GetPid( T& process )
{
    return process ? process->GetPid() : -1;
}

// -----------------------------------------------------------------------------
// Name: ReadConfig
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
Config ReadConfig( const web::Plugins& plugins, const Tree& src )
{
    Config cfg;
    ReadConfig( cfg, plugins, src );
    return cfg;
}

Session::Status AcquireStatus( Session::Status status, bool has_process, bool replay )
{
    if( has_process )
        return status;
    if( status == Session::STATUS_ARCHIVED )
        return status;
    return GetIdleStatus( replay );
}
}

// -----------------------------------------------------------------------------
// Name: Session::Session
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
Session::Session( const SessionDependencies& deps,
                  boost::shared_ptr< Node_ABC > node,
                  const SessionPaths& paths,
                  const Config& cfg,
                  const std::string& exercise,
                  const Uuid& replay )
    : deps_        ( deps )
    , async_       ( deps.pool )
    , node_        ( node )
    , id_          ( deps.uuids.Create() )
    , paths_       ( paths )
    , cfg_         ( cfg )
    , links_       ( deps.nodes.LinkExercise( *node_, exercise ) )
    , port_        ( deps.ports.Create() )
    , replay_      ( replay )
    , running_     ()
    , process_     ()
    , timeline_    ()
    , status_      ( GetIdleStatus( !replay_.is_nil() ) )
    , polling_     ( false )
    , counter_     ( 0 )
    , sizing_      ( false )
    , size_        ( 0 )
    , clients_     ()
    , start_time_  ()
    , current_time_()
    , checkpoints_ ()
    , first_time_  ( true )
    , replays_     ()
{
    NotifyNode();
    ParseExerciseProperties();
}

// -----------------------------------------------------------------------------
// Name: Session::Session
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
Session::Session( const SessionDependencies& deps,
                  boost::shared_ptr< Node_ABC > node,
                  const SessionPaths& paths,
                  const Tree& tree )
    : deps_        ( deps )
    , async_       ( deps.pool )
    , node_        ( node )
    , id_          ( Get< Uuid >( tree, "id" ) )
    , paths_       ( paths )
    , cfg_         ()
    , links_       ( deps.nodes.LinkExercise( *node_, tree.get_child( "links" ) ) )
    , port_        ( AcquirePort( Get< int >( tree, "port" ), deps.ports ) )
    , replay_      ( Get< Uuid >( tree, "replay.root" ) )
    , process_     ( AcquireProcess( tree, "process", deps_.runtime, port_->Get() ) )
    , timeline_    ( AcquireProcess( tree, "timeline", deps_.runtime, port_->Get() ) )
    , running_     ( process_ ? node->StartSession( boost::posix_time::not_a_date_time, true ) : Node_ABC::T_Token() )
    , status_      ( AcquireStatus( ConvertStatus( Get< std::string >( tree, "status" ) ), process_, !replay_.is_nil() ) )
    , polling_     ( false )
    , counter_     ( 0 )
    , sizing_      ( false )
    , size_        ( Get< size_t >( tree, "size" ) )
    , clients_     ()
    , start_time_  ()
    , current_time_()
    , last_error_  ( Get< std::string >( tree, "last_error" ) )
    , checkpoints_ ()
    , first_time_  ( Get< bool >( tree, "first_time" ) )
    , replays_     ()
{
    NotifyNode();
    // refill config with exercise properties,
    // they could have changed since last run
    ParseExerciseProperties();
    ReadConfig( cfg_, deps.plugins, tree );
    node_->UpdateSessionSize( id_, size_ );
    if( !process_ && !IsReplay() )
        ParseCheckpoints();
}

// -----------------------------------------------------------------------------
// Name: Session::~Session
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
Session::~Session()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Session::GetId
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
Uuid Session::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Session::GetRoot
// Created: BAX 2012-06-05
// -----------------------------------------------------------------------------
Path Session::GetRoot() const
{
    return paths_.root;
}

// -----------------------------------------------------------------------------
// Name: Session::GetExerciseSides
// Created: NPT 2013-04-05
// -----------------------------------------------------------------------------
void Session::ParseExerciseProperties()
{
    const Tree tree = node_->GetExerciseProperties( GetExercise().string() );
    if( const auto sides = tree.get_child_optional( "sides" ) )
        for( auto it = sides->begin(); it != sides->end(); ++it )
            cfg_.sides.list[ it->first ] = Side( Get< std::string >( it->second, "" ), true );
    if( const auto profiles = tree.get_child_optional( "profiles" ) )
        for( auto it = profiles->begin(); it != profiles->end(); ++it )
            cfg_.profiles.insert( Profile( it->first,
                Get< std::string >( it->second, ".password" ) ) );
}

// -----------------------------------------------------------------------------
// Name: Session::GetNode
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
Uuid Session::GetNode() const
{
    return node_->GetId();
}

// -----------------------------------------------------------------------------
// Name: Session::GetExercise
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
Path Session::GetExercise() const
{
    return ::GetPathFrom( links_, "exercise.name" );
}

// -----------------------------------------------------------------------------
// Name: Session::GetName
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
std::string Session::GetName() const
{
    return cfg_.name;
}

// -----------------------------------------------------------------------------
// Name: Session::GetPort
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
int Session::GetPort() const
{
    return port_->Get();
}

// -----------------------------------------------------------------------------
// Name: Session::IsReplay
// Created: BAX 2012-08-13
// -----------------------------------------------------------------------------
bool Session::IsReplay() const
{
    return !replay_.is_nil();
}

// -----------------------------------------------------------------------------
// Name: Session::GetReplayId
// Created: BAX 2012-08-13
// -----------------------------------------------------------------------------
Uuid Session::GetReplayId() const
{
    return replay_;
}

// -----------------------------------------------------------------------------
// Name: Session::HasReplays
// Created: BAX 2012-08-13
// -----------------------------------------------------------------------------
bool Session::HasReplays() const
{
    boost::shared_lock< boost::shared_mutex > lock( access_ );
    return !replays_.empty();
}

namespace
{
    bool AddLogFile( const FileSystem_ABC& fs, const runtime::Path& path, Tree& tree )
    {
        if( fs.IsFile( path ) &&
            path.extension() == ".log" && boost::count( path.string(), '.' ) == 1 )
            tree.put( runtime::Utf8( path.filename() ), true );
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: Session::AvailableLogs
// Created: NPT 2013-07-10
// -----------------------------------------------------------------------------
Tree Session::AvailableLogs() const
{
    Tree tree;
    deps_.fs.Walk( GetOutput(), false,
        boost::bind( &AddLogFile, boost::cref( deps_.fs ), _1, boost::ref( tree ) ) );
    return tree;
}

// -----------------------------------------------------------------------------
// Name: Session::GetProperties
// Created: BAX 2012-06-11
// -----------------------------------------------------------------------------
Tree Session::GetProperties( bool save ) const
{
    Tree tree;
    tree.put( "id", id_ );
    tree.put( "node", node_->GetId() );
    tree.put( "port", port_->Get() );
    tree.put_child( "logs", AvailableLogs() );
    WriteConfig( tree, cfg_ );
    tree.put( "status", ConvertStatus( status_ ) );
    tree.put( "first_time", first_time_ );
    tree.put( "last_error", last_error_ );
    if( IsReplay() )
        tree.put( "replay.root", replay_ );
    if( save )
        tree.put_child( "links", links_ );
    else
        BOOST_FOREACH( const Tree::value_type& it, links_ )
        {
            tree.put( it.first + ".name",     Get< std::string >( it.second, "name" ) );
            tree.put( it.first + ".checksum", Get< std::string >( it.second, "checksum" ) );
        }
    return tree;
}

// -----------------------------------------------------------------------------
// Name: Session::GetProperties
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
Tree Session::GetProperties() const
{
    boost::shared_lock< boost::shared_mutex > lock( access_ );
    Tree tree = GetProperties( false );
    tree.put( "start_time", start_time_ );
    tree.put( "current_time", current_time_ );
    tree.put( "timeline.port", port_->Get() + TIMELINE_WEB_PORT );
    PutList( tree, "clients", clients_ );
    PutList( tree, "checkpoints.list", checkpoints_ );
    PutList( tree, "replay.list", replays_ );
    return tree;
}

namespace
{
    void SaveProcess( Tree& dst, const std::string& key, const Session::T_Process& process )
    {
        if( !process )
            return;
        dst.put( key + ".pid", process->GetPid() );
        dst.put( key + ".name", process->GetName() );
    }
}

// -----------------------------------------------------------------------------
// Name: Session::Save
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
Tree Session::Save() const
{
    boost::shared_lock< boost::shared_mutex > lock( access_ );
    Tree tree = GetProperties( true );
    tree.put( "size", size_ );
    SaveProcess( tree, "process", process_ );
    SaveProcess( tree, "timeline", timeline_ );
    return tree;
}

namespace
{
std::string XpathToXml( std::string xpath )
{
    boost::algorithm::replace_all( xpath, "/", "." );
    boost::algorithm::replace_all( xpath, "@", "<xmlattr>." );
    return xpath;
}

std::string GetParties( const Config::T_Sides& sides )
{
    std::vector< std::string > list;
    for( auto it = sides.begin(); it != sides.end(); ++it )
        if( it->second.created )
            list.push_back( it->first );
    return boost::algorithm::join( list, ";" );
}

void WritePlugin( Tree& tree, const std::string& prefix, const web::session::PluginConfig& cfg )
{
    if( !cfg.enabled )
        return;
    tree.put( prefix + "<xmlattr>.library", Utf8( cfg.library ) );
    BOOST_FOREACH( const web::session::PluginConfig::T_Parameters::value_type& value, cfg.parameters )
        tree.put( prefix + XpathToXml( value.first ), value.second );
}

void WriteLogConfiguration( Tree& tree, const std::string& prefix, const Config& cfg )
{
    if( cfg.logs.rotate )
    {
        tree.put( prefix + "logfiles", cfg.logs.max_files + 1 );
        tree.put( prefix + "logsize", cfg.logs.max_size );
        tree.put( prefix + "sizeunit", cfg.logs.size_unit );
    }
}

void WriteDispatcherConfiguration( Tree& tree, int base, const Config& cfg )
{
    const std::string prefix = "session.config.dispatcher.";
    tree.put( prefix + "network.<xmlattr>.client", "localhost:" + boost::lexical_cast< std::string >( base + SIMULATION_PORT ) );
    tree.put( prefix + "network.<xmlattr>.server", "0.0.0.0:"   + boost::lexical_cast< std::string >( base + DISPATCHER_PORT ) );
    tree.put( prefix + "plugins.web_control.<xmlattr>.server", base + WEB_CONTROL_PORT );
    tree.put( prefix + "plugins.web_control.<xmlattr>.library", "web_control_plugin" );
    tree.put( prefix + "plugins.recorder.<xmlattr>.fragmentfreq", cfg.recorder.frequency );
    tree.put( prefix + "reports.<xmlattr>.frequency", cfg.reports.clean_frequency );
    WriteLogConfiguration( tree, prefix + "log.<xmlattr>.", cfg );
    WriteLogConfiguration( tree, prefix + "messages.<xmlattr>.", cfg );
    WriteLogConfiguration( tree, prefix + "debug.<xmlattr>.", cfg );
    BOOST_FOREACH( const Config::T_Plugins::value_type& value, cfg.plugins )
        WritePlugin( tree, prefix + "plugins." + value.first + ".", value.second );
}

void WriteRngConfiguration( Tree& tree, const std::string& prefix, const RngConfig& cfg )
{
    switch( cfg.distribution )
    {
        case RNG_DISTRIBUTION_GAUSSIAN:
            tree.put( prefix + "<xmlattr>.distribution", true );
            tree.put( prefix + "<xmlattr>.deviation", cfg.deviation );
            tree.put( prefix + "<xmlattr>.mean", cfg.mean );
            break;
    }
}

void WriteSimulationConfiguration( Tree& tree, int base, const Config& cfg )
{
    const std::string prefix = "session.config.simulation.";
    tree.put( prefix + "GarbageCollector.<xmlattr>.setpause", 100 );
    tree.put( prefix + "GarbageCollector.<xmlattr>.setstepmul", 100 );
    std::string frequency = "100000h";
    if( cfg.checkpoints.enabled )
        frequency = boost::lexical_cast< std::string >( cfg.checkpoints.frequency ) + "s";
    tree.put( prefix + "checkpoint.<xmlattr>.frequency", frequency );
    tree.put( prefix + "checkpoint.<xmlattr>.keep", cfg.checkpoints.keep );
    tree.put( prefix + "checkpoint.<xmlattr>.usecrc", true );
    WriteLogConfiguration( tree, prefix + "debug.<xmlattr>.", cfg );
    tree.put( prefix + "debug.<xmlattr>.decisional", false );
    tree.put( prefix + "debug.<xmlattr>.diadebugger", false );
    tree.put( prefix + "debug.<xmlattr>.diadebuggerport", base + DIA_DEBUGGER_PORT );
    tree.put( prefix + "debug.<xmlattr>.networklogger", false );
    tree.put( prefix + "debug.<xmlattr>.networkloggerport", base + NETWORK_LOGGER_PORT );
    tree.put( prefix + "debug.<xmlattr>.pathfind", false );
    tree.put( prefix + "decisional.<xmlattr>.useonlybinaries", false );
    tree.put( prefix + "dispatcher.<xmlattr>.embedded", true );
    tree.put( prefix + "network.<xmlattr>.port", base + SIMULATION_PORT );
    tree.put( prefix + "orbat.<xmlattr>.checkcomposition", false );
    tree.put( prefix + "pathfinder.<xmlattr>.threads", cfg.pathfind.threads );
    tree.put( prefix + "profiling.<xmlattr>.enabled", false );
    tree.put( prefix + "time.<xmlattr>.factor", cfg.time.factor );
    tree.put( prefix + "time.<xmlattr>.step", cfg.time.step );
    if( cfg.time.end_tick )
        tree.put( prefix + "time.<xmlattr>.end-tick", cfg.time.end_tick );
    tree.put( prefix + "time.<xmlattr>.paused", cfg.time.paused );
    tree.put( prefix + "random.<xmlattr>.seed", cfg.rng.seed );
    if( cfg.sides.no_side_objects )
            tree.put( prefix + "orbat.subset.<xmlattr>.no-party", cfg.sides.no_side_objects );
    tree.put( prefix + "orbat.subset.<xmlattr>.parties", GetParties( cfg.sides.list ) );
    WriteRngConfiguration( tree, prefix + "random0.", cfg.rng.fire );
    WriteRngConfiguration( tree, prefix + "random1.", cfg.rng.wound );
    WriteRngConfiguration( tree, prefix + "random2.", cfg.rng.perception );
    WriteRngConfiguration( tree, prefix + "random3.", cfg.rng.breakdown );
}

std::string GetConfiguration( const Config& cfg, int base )
{
    Tree tree;
    tree.put( "session.meta.comment", "Auto-generated by Cloud Host Agent" );
    tree.put( "session.meta.date", boost::posix_time::to_iso_string( boost::posix_time::second_clock::local_time() ) );
    tree.put( "session.meta.name", cfg.name );
    WriteDispatcherConfiguration( tree, base, cfg );
    WriteSimulationConfiguration( tree, base, cfg );
    return ToXml( tree );
}
}

// -----------------------------------------------------------------------------
// Name: Session::StopProcess
// Created: BAX 2012-06-20
// -----------------------------------------------------------------------------
bool Session::StopProcess( boost::upgrade_lock< boost::shared_mutex >& lock )
{
    T_Process copy;
    Node_ABC::T_Token token;
    {
        const std::string last_error = GetLastError( deps_.fs, GetOutput() );
        boost::upgrade_to_unique_lock< boost::shared_mutex> write( lock );
        last_error_ = last_error;
        status_ = GetIdleStatus( IsReplay() );
        copy.swap( process_ );
        if( timeline_ )
            timeline_->Kill();
        timeline_.reset();
        token.swap( running_ );
        clients_.clear();
    }
    if( !copy || !copy->IsAlive() )
        return true;

    const bool replay = IsReplay();
    bool done = false;
    if( !replay )
    {
        deps_.client.Get( "localhost", port_->Get() + WEB_CONTROL_PORT, "/stop", Client_ABC::T_Parameters() );
        done = copy->Join( 15 * 1000 );
    }
    if( !done )
        copy->Kill();
    copy->Join( 5 * 1000 );
    if( !replay )
        ParseCheckpoints();
    return true;
}

// -----------------------------------------------------------------------------
// Name: Session::Archive
// Created: BAX 2012-08-06
// -----------------------------------------------------------------------------
bool Session::Archive( boost::upgrade_lock< boost::shared_mutex >& lock )
{
    if( status_ != STATUS_STOPPED )
        throw web::HttpException( web::FORBIDDEN );
    boost::upgrade_to_unique_lock< boost::shared_mutex > write( lock );
    status_ = STATUS_ARCHIVED;
    return true;
}

// -----------------------------------------------------------------------------
// Name: Session::ModifyStatus
// Created: BAX 2012-06-20
// -----------------------------------------------------------------------------
bool Session::ModifyStatus( boost::upgrade_lock< boost::shared_mutex >& lock, Session::Status next )
{
    const bool replay = IsReplay();
    const Session::Status idle = GetIdleStatus( replay );

    if( process_ && !process_->IsAlive() )
        next = idle;

    if( next == status_ )
        return false;

    if( next == idle )
        return StopProcess( lock );

    if( replay )
        throw web::HttpException( web::FORBIDDEN );

    if( next == STATUS_ARCHIVED )
        if( !replays_.empty() )
            throw web::HttpException( web::FORBIDDEN );
        else
            return Archive( lock );

    const size_t counter = counter_++;
    const int pid = GetPid( process_ );
    lock.unlock();

    const std::string url = GetUrl( next );
    if( url.empty() )
        return false;

    Client_ABC::T_Response response = deps_.client.Get( "localhost", port_->Get() + WEB_CONTROL_PORT, url, Client_ABC::T_Parameters() );
    if( response->GetStatus() != 200 )
        return false;

    boost::upgrade_to_unique_lock< boost::shared_mutex > write( lock );
    if( counter + 1 != counter_ || GetPid( process_ ) != pid )
        return false;

    std::swap( status_, next );
    return status_ != next;
}

// -----------------------------------------------------------------------------
// Name: Session::Stop
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
bool Session::Stop()
{
    boost::upgrade_lock< boost::shared_mutex > lock( access_ );
    return ModifyStatus( lock, GetIdleStatus( IsReplay() ) );
}

// -----------------------------------------------------------------------------
// Name: Session::Pause
// Created: BAX 2012-06-19
// -----------------------------------------------------------------------------
bool Session::Pause()
{
    boost::upgrade_lock< boost::shared_mutex > lock( access_ );
    return ModifyStatus( lock, STATUS_PAUSED );
}

namespace
{
template< typename T >
std::string MakeOption( const std::string& option, const T& value )
{
    return "--" + option + " \"" + boost::lexical_cast< std::string >( value ) + "\"";
}
}

// -----------------------------------------------------------------------------
// Name: Session::StartSimulation
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
Session::T_Process Session::StartSimulation( boost::upgrade_lock< boost::shared_mutex >& lock,
                                             Status next,
                                             const std::string& checkpoint,
                                             bool replay,
                                             const Path& app )
{
    boost::upgrade_to_unique_lock< boost::shared_mutex > write( lock );
    status_ = next;
    start_time_.clear();
    current_time_.clear();
    const Path output = GetOutput();
    if( !replay && checkpoint.empty() )
        ClearOutput( output );
    deps_.fs.MakePaths( output );
    std::vector< std::string > options = boost::assign::list_of
        ( MakeOption( "debug-dir", Utf8( GetRoot() / "debug" ) ) )
        ( MakeOption( "exercises-dir", Utf8( GetPath( "exercise" ) ) ) )
        ( MakeOption( "terrains-dir", Utf8( GetPath( "terrain" ) ) ) )
        ( MakeOption( "models-dir", Utf8( GetPath( "model" ) ) ) )
        ( MakeOption( "exercise", Utf8( GetExercise() ) ) )
        ( MakeOption( "session",  output.filename() ) );
    std::string file = "session.xml";
    if( replay )
    {
        file = boost::lexical_cast< std::string >( id_ );
        options.push_back( MakeOption( "session-file", file ) );
        options.push_back( "--no-log" );
    }
    deps_.fs.WriteFile( output / file, GetConfiguration( cfg_, port_->Get() ) );
    if( !checkpoint.empty() )
        options.push_back( MakeOption( "checkpoint", checkpoint ) );
    return deps_.runtime.Start( Utf8( app ),
        options, Utf8( Path( app ).remove_filename() ), "" );
}

namespace
{
void WriteTimelineConfig( const UuidFactory_ABC& uuids,
                          const FileSystem_ABC& fs,
                          const Path& filename,
                          int port )
{
    const std::string uuid = boost::lexical_cast< std::string >( uuids.Create() );

    std::string output;
    output += "[";
    Tree login;
    login.put( "type", "USER_LOGIN" );
    login.put( "user.login.username", "" );
    login.put( "user.login.password", "" );
    output += ToJson( login ) + ",";

    Tree create;
    create.put( "type", "SESSION_CREATE" );
    create.put( "session.create.uuid", uuid );
    create.put( "session.create.name", "session" );
    output += ToJson( create ) + ",";

    // manual serialization for invalid bool support from boost::property_tree
    // todo use a real json library
    const std::string attach = ( boost::format(
    "{"
    "    \"type\": \"SESSION_ATTACH\","
    "    \"session\": {"
    "        \"attach\": {"
    "            \"uuid\": \"%1%\","
    "            \"service\": {"
    "                \"name\": \"simulation\","
    "                \"clock\": true,"
    "                \"sword\": {"
    "                    \"address\": \"localhost:%2%\""
    "                }"
    "            }"
    "        }"
    "    }"
    "}," ) % uuid % port ).str();
    output += attach;

    Tree start;
    start.put( "type", "SESSION_START" );
    start.put( "session.start.uuid", uuid );
    output += ToJson( start ) + "]";

    fs.WriteFile( filename, output );
}

Session::T_Process StartTimeline( const SessionDependencies& deps,
                                  const Path& app,
                                  const Path& root,
                                  int base )
{
    const Path config = root / "timeline.run";
    WriteTimelineConfig( deps.uuids, deps.fs, config, base + DISPATCHER_PORT );
    std::vector< std::string > options = boost::assign::list_of
        ( MakeOption( "port",  base + TIMELINE_PORT ) )
        ( MakeOption( "serve", base + TIMELINE_WEB_PORT ) )
        ( MakeOption( "run", Utf8( config ) ) );
    return deps.runtime.Start( Utf8( app ), options,
        Utf8( Path( app ).remove_filename() ), Utf8( root / "timeline.log" ) );
}

void Reset( boost::upgrade_lock< boost::shared_mutex >& lock,
            Session::Status& status, Session::Status next )
{
    boost::upgrade_to_unique_lock< boost::shared_mutex > write( lock );
    status = next;
}
}

// -----------------------------------------------------------------------------
// Name: Session::Start
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
bool Session::Start( const Path& app, const Path& timeline, const std::string& checkpoint )
{
    boost::upgrade_lock< boost::shared_mutex > lock( access_ );

    const bool replay = IsReplay();
    const Status next = replay ? STATUS_REPLAYING : STATUS_PLAYING;
    if( process_ )
        return ModifyStatus( lock, next );

    if( !replays_.empty() )
        throw web::HttpException( web::FORBIDDEN );

    Node_ABC::T_Token token;
    if( !replay )
    {
        const boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
        token = node_->StartSession( now, first_time_ || !checkpoint.empty() );
        if( !token )
            return false;
    }

    runtime::Scoper reset_status( boost::bind( &Reset, boost::ref( lock ), boost::ref( status_ ), status_ ) );
    auto ptr = StartSimulation( lock, next, checkpoint, replay, app );
    if( !ptr )
        return false;

    runtime::Scoper kill_orphaned_process( boost::bind( &runtime::Process_ABC::Kill, ptr ) );
    T_Process time;
    if( cfg_.timeline.enabled )
    {
        if( !deps_.ports.WaitConnected( lock, port_->Get() + DISPATCHER_PORT ) )
            return false;
        if( !( time = StartTimeline( deps_, timeline, GetRoot(), port_->Get() + DISPATCHER_PORT ) ) )
            return false;
    }

    boost::upgrade_to_unique_lock< boost::shared_mutex > write( lock );
    reset_status.Reset();
    kill_orphaned_process.Reset();
    process_    = ptr;
    timeline_   = time;
    running_    = token;
    first_time_ = false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: Session::GetPath
// Created: BAX 2012-06-06
// -----------------------------------------------------------------------------
Path Session::GetPath( const std::string& type ) const
{
    if( type == "exercise" )
        return ::GetPathFrom( links_, "exercise.root" ) / "exercises";
    if( type == "model" )
        return ::GetPathFrom( links_, "model.root" ) / "data" / "models";
    if( type == "terrain" )
        return ::GetPathFrom( links_, "terrain.root" ) / "data" / "terrains";
    return Path();
}

// -----------------------------------------------------------------------------
// Name: Session::GetOutput
// Created: BAX 2012-06-25
// -----------------------------------------------------------------------------
Path Session::GetOutput() const
{
    const Uuid& id = IsReplay() ? replay_ : id_;
    return GetPath( "exercise" ) / GetExercise() / "sessions" / boost::lexical_cast< std::string >( id );
}

// -----------------------------------------------------------------------------
// Name: Session::Refresh
// Created: BAX 2012-06-14
// -----------------------------------------------------------------------------
bool Session::Refresh()
{
    boost::upgrade_lock< boost::shared_mutex > lock( access_ );
    return ModifyStatus( lock, status_ );
}

namespace
{
void ResetBool( boost::upgrade_lock< boost::shared_mutex >& lock, bool& value, bool next )
{
    boost::upgrade_to_unique_lock< boost::shared_mutex > write( lock );
    value = next;
}
}

// -----------------------------------------------------------------------------
// Name: Session::RefreshSize
// Created: BAX 2012-07-19
// -----------------------------------------------------------------------------
bool Session::RefreshSize()
{
    boost::upgrade_lock< boost::shared_mutex > lock( access_ );
    if( sizing_ )
        return false;
    if( IsReplay() )
        return false;
    sizing_ = true;
    runtime::Scoper unsize( boost::bind( &ResetBool, boost::ref( lock ), boost::ref( sizing_ ), false ) );
    lock.unlock();

    const size_t next = deps_.fs.GetDirectorySize( paths_.root )
                      + deps_.fs.GetDirectorySize( GetOutput() );
    lock.lock();
    const bool modified = next != size_;
    size_ = next;
    sizing_ = false;
    lock.unlock();

    if( modified )
        node_->UpdateSessionSize( id_, next );
    return modified;
}

namespace
{
std::string RoundTripIsoTime( const std::string& time )
{
    const boost::posix_time::ptime next = boost::posix_time::from_iso_string( time );
    return boost::posix_time::to_iso_extended_string( next );
}
const boost::xpressive::sregex portRegex = boost::xpressive::sregex::compile( ":\\d+$" );
}

// -----------------------------------------------------------------------------
// Name: Session::Poll
// Created: BAX 2012-06-19
// -----------------------------------------------------------------------------
bool Session::Poll()
{
    boost::upgrade_lock< boost::shared_mutex > lock( access_ );
    if( polling_ )
        return false;
    if( !process_ || !process_->IsAlive() )
        return false;

    polling_ = true;
    const size_t counter = counter_++;
    const int pid = GetPid( process_ );
    runtime::Scoper unpoll( boost::bind( &ResetBool, boost::ref( lock ), boost::ref( polling_ ), false ) );
    lock.unlock();

    Client_ABC::T_Response response = deps_.client.Get( "localhost", port_->Get() + WEB_CONTROL_PORT, "/get", Client_ABC::T_Parameters() );
    if( response->GetStatus() != 200 )
        return false;

    const Tree data = FromJson( response->GetBody() );
    Session::Status state = ConvertRemoteStatus( Get< std::string >( data, "state" ) );
    if( state == STATUS_COUNT )
        return false;

    const std::string start = RoundTripIsoTime( Get< std::string >( data, "start_time" ) );
    const std::string current = RoundTripIsoTime( Get< std::string >( data, "current_time" ) );
    std::vector< std::string > clients;
    Tree::const_assoc_iterator cai = data.find( "clients" );
    if( cai != data.not_found() )
        for( Tree::const_iterator it = cai->second.begin(); it != cai->second.end(); ++it )
            clients.push_back( boost::xpressive::regex_replace( it->second.data(), portRegex, "" ) );

    lock.lock();
    if( counter + 1 != counter_ || GetPid( process_ ) != pid )
        return false;
    start_time_ = start;
    current_time_ = current;
    std::swap( clients_, clients );
    return ModifyStatus( lock, state );
}

// -----------------------------------------------------------------------------
// Name: Session::Remove
// Created: BAX 2012-06-22
// -----------------------------------------------------------------------------
void Session::Remove()
{
    boost::upgrade_lock< boost::shared_mutex > lock( access_ );
    const bool replay = IsReplay();
    ModifyStatus( lock, GetIdleStatus( replay ) );
    if( !replay )
        deps_.fs.Remove( GetOutput() );
    node_->UnlinkExercise( links_ );
    node_->RemoveSession( id_ );
    deps_.fs.Remove( GetRoot() );
}

// -----------------------------------------------------------------------------
// Name: Session::Update
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
bool Session::Update( const Tree& cfg )
{
    boost::upgrade_lock< boost::shared_mutex > lock( access_ );
    if( status_ != GetIdleStatus( IsReplay() ) )
        return false;
    web::session::Config next = cfg_;
    bool modified = ReadConfig( next, deps_.plugins, cfg );
    if( !modified )
        return false;
    node_->FilterConfig( next );
    boost::upgrade_to_unique_lock< boost::shared_mutex > write( lock );
    cfg_ = next;
    return true;
}

// -----------------------------------------------------------------------------
// Name: Session::Archive
// Created: BAX 2012-08-06
// -----------------------------------------------------------------------------
bool Session::Archive()
{
    boost::upgrade_lock< boost::shared_mutex > lock( access_ );
    if( status_ != STATUS_STOPPED )
        throw web::HttpException( web::FORBIDDEN );
    return ModifyStatus( lock, STATUS_ARCHIVED );
}

// -----------------------------------------------------------------------------
// Name: Session::Restore
// Created: BAX 2012-08-06
// -----------------------------------------------------------------------------
bool Session::Restore()
{
    boost::upgrade_lock< boost::shared_mutex > lock( access_ );
    if( status_ != STATUS_ARCHIVED )
        throw web::HttpException( web::FORBIDDEN );
    return ModifyStatus( lock, STATUS_STOPPED );
}

namespace
{
    void PackDir( const FileSystem_ABC& fs, runtime::Packer_ABC& packer, const Path& path )
    {
        if( fs.IsDirectory( path ) )
            packer.Pack( path, runtime::Packer_ABC::T_Predicate() );
    }
}

// -----------------------------------------------------------------------------
// Name: Session::Download
// Created: BAX 2012-08-06
// -----------------------------------------------------------------------------
bool Session::Download( web::Chunker_ABC& dst ) const
{
    boost::shared_lock< boost::shared_mutex > lock( access_ );
    dst.SetName( cfg_.name + ".zip" );
    dst.SetHeader( "Content-Type", "application/zip" );
    io::Writer_ABC& sink = dst.OpenWriter();
    FileSystem_ABC::T_Packer packer = deps_.fs.Pack( sink, runtime::ARCHIVE_FMT_ZIP );
    PackDir( deps_.fs, *packer, paths_.root );
    if( !IsReplay() )
        PackDir( deps_.fs, *packer, GetOutput() );
    packer->PackEntry( "signature", 0, 0 );
    return true;
}

// -----------------------------------------------------------------------------
// Name: Session::ClearOutput
// Created: BAX 2012-08-08
// -----------------------------------------------------------------------------
void Session::ClearOutput( const Path& path )
{
    checkpoints_.clear();
    if( !deps_.fs.IsDirectory( path ) )
        return;
    const Path output = deps_.fs.MakeAnyPath( paths_.trash );
    deps_.fs.Rename( path, output / "_" );
    async_.Post( boost::bind( &FileSystem_ABC::Remove, &deps_.fs, output ) );
}

namespace
{
template< typename T >
bool Attach( const FileSystem_ABC& fs, const Path& path, T& items )
{
    if( fs.IsFile( path / "data" ) )
        items.push_back( runtime::Utf8( path.filename() ) );
    return true;
}
}

// -----------------------------------------------------------------------------
// Name: Session::Replay
// Created: BAX 2012-08-10
// -----------------------------------------------------------------------------
Session::T_Ptr Session::Replay()
{
    boost::lock_guard< boost::shared_mutex > read( access_ );
    if( IsReplay() )
        throw web::HttpException( web::FORBIDDEN );
    if( first_time_ )
        throw web::HttpException( web::FORBIDDEN );
    web::session::Config cfg = cfg_;
    cfg.name += " replay " + boost::lexical_cast< std::string >( replays_.size() + 1 );
    SessionPaths paths = paths_;
    paths.root = deps_.fs.MakeAnyPath( paths.root.remove_filename() );
    T_Ptr next = boost::make_shared< Session >( deps_, node_, paths, cfg, Utf8( GetExercise() ), id_ );
    replays_.insert( next->GetId() );
    return next;
}

// -----------------------------------------------------------------------------
// Name: Session::AttachReplay
// Created: BAX 2012-08-13
// -----------------------------------------------------------------------------
void Session::AttachReplay( const Session_ABC& replay )
{
    boost::lock_guard< boost::shared_mutex > lock( access_ );
    replays_.insert( replay.GetId() );
}

// -----------------------------------------------------------------------------
// Name: Session::DetachReplay
// Created: BAX 2012-08-14
// -----------------------------------------------------------------------------
void Session::DetachReplay( const Session_ABC& replay )
{
    boost::lock_guard< boost::shared_mutex > lock( access_ );
    replays_.erase( replay.GetId() );
}

// -----------------------------------------------------------------------------
// Name: Session::ParseCheckpoints
// Created: BAX 2012-08-08
// -----------------------------------------------------------------------------
void Session::ParseCheckpoints()
{
    checkpoints_.clear();
    deps_.fs.Walk( GetOutput() / "checkpoints", false, boost::bind( &Attach< T_Checkpoints >,
                  boost::cref( deps_.fs ), _1, boost::ref( checkpoints_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: Session::NotifyNode
// Created: BAX 2012-08-29
// -----------------------------------------------------------------------------
void Session::NotifyNode()
{
    boost::lock_guard< boost::shared_mutex > lock( access_ );
    node_->FilterConfig( cfg_ );
}

// -----------------------------------------------------------------------------
// Name: Session::DownloadLog
// Created: NPT 2013-07-10
// -----------------------------------------------------------------------------
bool Session::DownloadLog( web::Chunker_ABC& dst, const std::string& logFile, int limitSize, bool deflate ) const
{
    boost::shared_lock< boost::shared_mutex > lock( access_ );
    dst.SetName( logFile );
    io::Writer_ABC& sink = dst.OpenWriter();
    if( !deps_.fs.Exists( GetOutput() / logFile ) )
        return false;

    deps_.fs.ReadFileWithLimitSize( deflate ? *deps_.fs.MakeGzipFilter( sink ) : sink, GetOutput() / logFile, limitSize );
    return true;
}

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
#include "PortFactory_ABC.h"
#include "runtime/Async.h"
#include "runtime/FileSystem_ABC.h"
#include "runtime/Process_ABC.h"
#include "runtime/PropertyTree.h"
#include "runtime/Runtime_ABC.h"
#include "runtime/Scoper.h"
#include "runtime/Utf8.h"
#include "web/Client_ABC.h"

#include <boost/assign/list_of.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace host;
using namespace property_tree;
using namespace web::session;
using runtime::Async;
using runtime::FileSystem_ABC;
using runtime::Runtime_ABC;
using runtime::Utf8Convert;
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
    }
}

Session::Status ConvertStatus( const std::string& status )
{
    if( status == "playing" )   return Session::STATUS_PLAYING;
    if( status == "replaying" ) return Session::STATUS_REPLAYING;
    if( status == "paused" )    return Session::STATUS_PAUSED;
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

Session::T_Process GetProcess( const Tree& tree, const runtime::Runtime_ABC& runtime )
{
    const boost::optional< int > pid = tree.get_optional< int >( "process.pid" );
    if( pid == boost::none )
        return Session::T_Process();
    return runtime.GetProcess( *pid );
}

Session::T_Process AcquireProcess( const Tree& tree, const runtime::Runtime_ABC& runtime, int expected )
{
    Session::T_Process ptr = GetProcess( tree, runtime );
    if( !ptr  )
        return Session::T_Process();
    if( expected != Get< int >( tree, "port" ) || ptr->GetName() != Get< std::string >( tree, "process.name" ) )
        return Session::T_Process();
    return ptr;
}

Path GetPath( const Tree& src, const std::string& key )
{
    return Utf8Convert( Get< std::string >( src, key ) );
}

template< typename T >
int GetPid( T& process )
{
    return process ? process->GetPid() : -1;
}

bool ReadRngConfig( RngConfig& dst, const Tree& src, const std::string& prefix )
{
    std::string dist;
    bool modified = TryRead( dist, src, prefix + ".distribution" );
    if( modified )
        dst.distribution = ConvertRngDistribution( dist );
    modified |= TryRead( dst.deviation, src, prefix + ".deviation" );
    modified |= TryRead( dst.mean, src, prefix + ".mean" );
    return modified;
}

void WriteRngConfig( Tree& dst, const std::string& prefix, const RngConfig& cfg )
{
    dst.put( prefix + ".distribution", ConvertRngDistribution( cfg.distribution ) );
    dst.put( prefix + ".deviation", cfg.deviation );
    dst.put( prefix + ".mean", cfg.mean );
}

bool ReadConfig( Config& dst, const Tree& src )
{
    bool modified = false;
    modified |= TryRead( dst.name, src, "name" );
    modified |= TryRead( dst.checkpoints.enabled, src, "checkpoints.enabled" );
    modified |= TryRead( dst.checkpoints.frequency, src, "checkpoints.frequency" );
    modified |= TryRead( dst.checkpoints.keep, src, "checkpoints.keep" );
    modified |= TryRead( dst.time.end_tick, src, "time.end_tick" );
    modified |= TryRead( dst.time.factor, src, "time.factor" );
    modified |= TryRead( dst.time.paused, src, "time.paused" );
    modified |= TryRead( dst.time.step, src, "time.step" );
    modified |= TryRead( dst.rng.seed, src, "rng.seed" );
    modified |= ReadRngConfig( dst.rng.breakdown, src, "rng.breakdown" );
    modified |= ReadRngConfig( dst.rng.fire, src, "rng.fire" );
    modified |= ReadRngConfig( dst.rng.perception, src, "rng.perception" );
    modified |= ReadRngConfig( dst.rng.wound, src, "rng.wound" );
    modified |= TryRead( dst.pathfind.threads, src, "pathfind.threads" );
    modified |= TryRead( dst.recorder.frequency, src, "recorder.frequency" );
    return modified;
}

Config ReadConfig( const Tree& src )
{
    Config cfg;
    ReadConfig( cfg, src );
    return cfg;
}

void WriteConfig( Tree& dst, const Config& cfg )
{
    dst.put( "name", cfg.name );
    dst.put( "checkpoints.enabled", cfg.checkpoints.enabled );
    dst.put( "checkpoints.frequency", cfg.checkpoints.frequency );
    dst.put( "checkpoints.keep", cfg.checkpoints.keep );
    dst.put( "time.end_tick", cfg.time.end_tick );
    dst.put( "time.factor", cfg.time.factor );
    dst.put( "time.paused", cfg.time.paused );
    dst.put( "time.step", cfg.time.step );
    dst.put( "rng.seed", cfg.rng.seed );
    WriteRngConfig( dst, "rng.breakdown", cfg.rng.breakdown );
    WriteRngConfig( dst, "rng.fire", cfg.rng.fire );
    WriteRngConfig( dst, "rng.perception", cfg.rng.perception );
    WriteRngConfig( dst, "rng.wound", cfg.rng.wound );
    dst.put( "pathfind.threads", cfg.pathfind.threads );
    dst.put( "recorder.frequency", cfg.recorder.frequency );
}
}

// -----------------------------------------------------------------------------
// Name: Session::Session
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
Session::Session( const FileSystem_ABC& system,
                  Client_ABC& client,
                  const boost::shared_ptr< Node_ABC > node,
                  const Path& root,
                  const Uuid& id,
                  const Config& cfg,
                  const std::string& exercise,
                  const Port& port )
    : system_      ( system )
    , client_      ( client )
    , node_        ( node )
    , id_          ( id )
    , root_        ( root )
    , cfg_         ( cfg )
    , links_       ( node->LinkExercise( exercise ) )
    , port_        ( port )
    , running_     ()
    , process_     ()
    , status_      ( STATUS_STOPPED )
    , polling_     ( false )
    , counter_     ( 0 )
    , sizing_      ( false )
    , size_        ( 0 )
    , clients_     ()
    , start_time_  ()
    , current_time_()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Session::Session
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
Session::Session( const FileSystem_ABC& system,
                  Client_ABC& client,
                  const boost::shared_ptr< Node_ABC > node,
                  const Path& root,
                  const Tree& tree,
                  const runtime::Runtime_ABC& runtime,
                  PortFactory_ABC& ports )
    : system_      ( system )
    , client_      ( client )
    , node_        ( node )
    , id_          ( Get< Uuid >( tree, "id" ) )
    , root_        ( root )
    , cfg_         ( ReadConfig( tree ) )
    , links_       ( node->LinkExercise( tree.get_child( "links" ) ) )
    , port_        ( AcquirePort( Get< int >( tree, "port" ), ports ) )
    , process_     ( AcquireProcess( tree, runtime, port_->Get() ) )
    , running_     ( process_ ? node->StartSession( boost::posix_time::not_a_date_time ) : Node_ABC::T_Token() )
    , status_      ( process_ ? ConvertStatus( Get< std::string >( tree, "status" ) ) : Session::STATUS_STOPPED )
    , polling_     ( false )
    , counter_     ( 0 )
    , sizing_      ( false )
    , size_        ( Get< size_t >( tree, "size" ) )
    , clients_     ()
    , start_time_  ()
    , current_time_()
{
    node_->UpdateSessionSize( id_, size_ );
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
    return root_;
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
    return ::GetPath( links_, "exercise.name" );
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
// Name: Session::GetProperties
// Created: BAX 2012-06-11
// -----------------------------------------------------------------------------
Tree Session::GetProperties( bool save ) const
{
    Tree tree;
    tree.put( "id", id_ );
    tree.put( "node", node_->GetId() );
    tree.put( "port", port_->Get() );
    WriteConfig( tree, cfg_ );
    tree.put( "status", ConvertStatus( status_ ) );
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
    Tree& sub = tree.put_child( "clients", Tree() );
    BOOST_FOREACH( const T_Clients::value_type& value, clients_ )
        sub.push_back( std::make_pair( "", value ) );
    return tree;
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
    if( !process_ )
        return tree;
    tree.put( "process.pid", process_->GetPid() );
    tree.put( "process.name", process_->GetName() );
    return tree;
}

namespace
{
void WriteDispatcherConfiguration( Tree& tree, int base, const Config& cfg )
{
    const std::string prefix = "session.config.dispatcher.";
    tree.put( prefix + "network.<xmlattr>.client", "localhost:" + boost::lexical_cast< std::string >( base + SIMULATION_PORT ) );
    tree.put( prefix + "network.<xmlattr>.server", base + DISPATCHER_PORT );
    tree.put( prefix + "plugins.web_control.<xmlattr>.server", base + WEB_CONTROL_PORT );
    tree.put( prefix + "plugins.web_control.<xmlattr>.library", "web_control_plugin" );
    tree.put( prefix + "plugins.recorder.<xmlattr>.fragmentfreq", cfg.recorder.frequency );
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
    if( cfg.checkpoints.enabled )
    {
        tree.put( prefix + "checkpoint.<xmlattr>.frequency", cfg.checkpoints.frequency );
        tree.put( prefix + "checkpoint.<xmlattr>.keep", cfg.checkpoints.keep );
        tree.put( prefix + "checkpoint.<xmlattr>.usecrc", true );
    }
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
        boost::upgrade_to_unique_lock< boost::shared_mutex> write( lock );
        status_ = STATUS_STOPPED;
        copy.swap( process_ );
        token.swap( running_ );
        clients_.clear();
        start_time_.clear();
        current_time_.clear();
    }
    if( !copy || !copy->IsAlive() )
        return true;

    client_.Get( "localhost", port_->Get() + WEB_CONTROL_PORT, "/stop", Client_ABC::T_Parameters() );
    if( copy->Join( 15 * 1000 ) )
        return true;

    copy->Kill();
    copy->Join( 5 * 1000 );
    return true;
}

// -----------------------------------------------------------------------------
// Name: Session::ModifyStatus
// Created: BAX 2012-06-20
// -----------------------------------------------------------------------------
bool Session::ModifyStatus( boost::upgrade_lock< boost::shared_mutex >& lock, Session::Status next )
{
    if( process_ && !process_->IsAlive() )
        next = STATUS_STOPPED;

    if( next == status_ )
        return false;

    if( next == STATUS_STOPPED )
        return StopProcess( lock );

    const size_t counter = counter_++;
    const int pid = GetPid( process_ );
    lock.unlock();

    const std::string url = GetUrl( next );
    if( url.empty() )
        return false;

    Client_ABC::T_Response response = client_.Get( "localhost", port_->Get() + WEB_CONTROL_PORT, url, Client_ABC::T_Parameters() );
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
    return ModifyStatus( lock, STATUS_STOPPED );
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
// Name: Session::Start
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
bool Session::Start( const Runtime_ABC& runtime, const Path& apps )
{
    boost::upgrade_lock< boost::shared_mutex > lock( access_ );
    if( process_ )
        return ModifyStatus( lock, STATUS_PLAYING );

    const boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    Node_ABC::T_Token token = node_->StartSession( now );
    if( !token )
        return false;

    boost::upgrade_to_unique_lock< boost::shared_mutex > write( lock );
    const Path output = GetOutput();
    system_.MakePaths( output );
    system_.WriteFile( output / "session.xml", GetConfiguration( cfg_, port_->Get() ) );
    T_Process ptr = runtime.Start( Utf8Convert( apps / "simulation_app.exe" ), boost::assign::list_of
        ( MakeOption( "debug-dir", Utf8Convert( GetRoot() / "debug" ) ) )
        ( MakeOption( "exercises-dir", Utf8Convert( GetPath( "exercise" ) ) ) )
        ( MakeOption( "terrains-dir", Utf8Convert( GetPath( "terrain" ) ) ) )
        ( MakeOption( "models-dir", Utf8Convert( GetPath( "model" ) ) ) )
        ( MakeOption( "exercise", Utf8Convert( GetExercise() ) ) )
        ( MakeOption( "session",  output.filename() ) )
        ( "--silent" ),
        Utf8Convert( apps ),
        Utf8Convert( GetRoot() / "session.log" ) );
    if( !ptr )
        return false;

    process_ = ptr;
    running_ = token;
    status_  = STATUS_PLAYING;
    return true;
}

// -----------------------------------------------------------------------------
// Name: Session::GetPath
// Created: BAX 2012-06-06
// -----------------------------------------------------------------------------
Path Session::GetPath( const std::string& type ) const
{
    if( type == "exercise" )
        return ::GetPath( links_, "exercise.root" ) / "exercises";
    if( type == "model" )
        return ::GetPath( links_, "model.root" ) / "data" / "models";
    if( type == "terrain" )
        return ::GetPath( links_, "terrain.root" ) / "data" / "terrains";
    return Path();
}

// -----------------------------------------------------------------------------
// Name: Session::GetOutput
// Created: BAX 2012-06-25
// -----------------------------------------------------------------------------
Path Session::GetOutput() const
{
    return GetPath( "exercise" ) / GetExercise() / "sessions" / boost::lexical_cast< std::string >( id_ );
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
    sizing_ = true;
    runtime::Scoper unsize( boost::bind( &ResetBool, boost::ref( lock ), boost::ref( sizing_ ), false ) );
    lock.unlock();

    bool modified = false;
    const size_t next = system_.GetDirectorySize( root_ );
    lock.lock();
    modified = next != size_;
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
    if( polling_ || !process_ || !process_->IsAlive() )
        return false;

    polling_ = true;
    const size_t counter = counter_++;
    const int pid = GetPid( process_ );
    runtime::Scoper unpoll( boost::bind( &ResetBool, boost::ref( lock ), boost::ref( polling_ ), false ) );
    lock.unlock();

    Client_ABC::T_Response response = client_.Get( "localhost", port_->Get() + WEB_CONTROL_PORT, "/get", Client_ABC::T_Parameters() );
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
    ModifyStatus( lock, STATUS_STOPPED );
    node_->UnlinkExercise( links_ );
    node_->RemoveSession( id_ );
    system_.Remove( GetRoot() );
    system_.Remove( GetOutput() );
}

// -----------------------------------------------------------------------------
// Name: Session::Update
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
bool Session::Update( const Tree& cfg )
{
    boost::upgrade_lock< boost::shared_mutex > lock( access_ );
    if( status_ != STATUS_STOPPED )
        return false;
    web::session::Config next = cfg_;
    bool modified = ReadConfig( next, cfg );
    if( !modified )
        return false;
    boost::upgrade_to_unique_lock< boost::shared_mutex > write( lock );
    cfg_ = next;
    return true;
}
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
#include "PropertyTree.h"
#include "runtime/FileSystem_ABC.h"
#include "runtime/Process_ABC.h"
#include "runtime/Runtime_ABC.h"
#include "runtime/Utf8.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4244 )
#endif
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

using namespace host;
using runtime::Utf8Convert;
using runtime::FileSystem_ABC;

namespace
{
enum SessionPort
{
    SIMULATION_PORT,
    DISPATCHER_PORT,
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

std::auto_ptr< Port_ABC > AcquirePort( int wanted, PortFactory_ABC& ports )
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
}

// -----------------------------------------------------------------------------
// Name: Session::Session
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
Session::Session( const Path& root, const Uuid& id, const Node_ABC& node, const std::string& name, const std::string& exercise, std::auto_ptr< Port_ABC > port )
    : id_     ( id )
    , root_   ( root )
    , node_   ( node )
    , name_   ( name )
    , links_  ( node.LinkExercise( exercise ) )
    , port_   ( port )
    , access_ ( new boost::shared_mutex )
    , process_()
    , status_ ( Session::STATUS_STOPPED )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Session::Session
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
Session::Session( const Path& root, const Tree& tree, const Node_ABC& node, const runtime::Runtime_ABC& runtime, PortFactory_ABC& ports )
    : id_     ( Get< Uuid >( tree, "id" ) )
    , root_   ( root )
    , node_   ( node )
    , name_   ( Get< std::string >( tree, "name" ) )
    , links_  ( node.LinkExercise( tree.get_child( "links" ) ) )
    , port_   ( AcquirePort( Get< int >( tree, "port" ), ports ) )
    , access_ ( new boost::shared_mutex )
    , process_( AcquireProcess( tree, runtime, port_->Get() ) )
    , status_ ( process_ ? ConvertStatus( Get< std::string >( tree, "status" ) ) : Session::STATUS_STOPPED )
{
    // NOTHING
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
    return node_.GetId();
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
    return name_;
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
    tree.put( "node", node_.GetId() );
    tree.put( "name", name_ );
    tree.put( "port", port_->Get() );
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
    return GetProperties( false );
}

// -----------------------------------------------------------------------------
// Name: Session::Save
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
Tree Session::Save() const
{
    Tree tree = GetProperties( true );
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    if( !process_ )
        return tree;
    tree.put( "process.pid", process_->GetPid() );
    tree.put( "process.name", process_->GetName() );
    return tree;
}

// -----------------------------------------------------------------------------
// Name: Session::Stop
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
bool Session::Stop()
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    if( !process_ )
        return true;

    process_->Kill( 0 );
    process_.reset();
    status_ = Session::STATUS_STOPPED;
    return true;
}

namespace
{
void GetDispatcherConfiguration( Tree& tree, int base )
{
    const std::string prefix = "session.config.dispatcher.";
    tree.put( prefix + "network.<xmlattr>.client", "localhost:" + boost::lexical_cast< std::string >( base + SIMULATION_PORT ) );
    tree.put( prefix + "network.<xmlattr>.server", base + DISPATCHER_PORT );
    tree.put( prefix + "plugins.web_control.<xmlattr>.server", base + WEB_CONTROL_PORT );
    tree.put( prefix + "plugins.web_control.<xmlattr>.library", "web_control_plugin_dll-vc100-mt" );
}

void GetSimulationConfiguration( Tree& tree, int base )
{
    const std::string prefix = "session.config.simulation.";
    tree.put( prefix + "GarbageCollector.<xmlattr>.setpause", 100 );
    tree.put( prefix + "GarbageCollector.<xmlattr>.setstepmul", 100 );
    tree.put( prefix + "checkpoint.<xmlattr>.frequency", "100000h" );
    tree.put( prefix + "checkpoint.<xmlattr>.keep", 1 );
    tree.put( prefix + "checkpoint.<xmlattr>.usecrc", true );
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
    tree.put( prefix + "pathfinder.<xmlattr>.threads", 1 );
    tree.put( prefix + "profiling.<xmlattr>.enabled", false );
    tree.put( prefix + "time.<xmlattr>.factor", 10 );
    tree.put( prefix + "time.<xmlattr>.step", 10 );
}

std::string GetConfiguration( const std::string& name, int base )
{
    Tree tree;
    tree.put( "session.meta.comment", "Auto-generated by Cloud Host Agent" );
    tree.put( "session.meta.date", boost::posix_time::to_iso_string( boost::posix_time::second_clock::local_time() ) );
    tree.put( "session.meta.name", name );
    GetDispatcherConfiguration( tree, base );
    GetSimulationConfiguration( tree, base );
    return ToXml( tree );
}

void WriteSettings( const FileSystem_ABC& system, const Path& file, const std::string& data )
{
    system.MakePaths( Path( file ).remove_filename() );
    system.WriteFile( file, data );
}
}

// -----------------------------------------------------------------------------
// Name: Session::Start
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
bool Session::Start( const FileSystem_ABC& system, const T_Starter& starter )
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    if( process_ )
        return true;

    const Path output = GetPath( "exercise" ) / GetExercise() / "sessions" / boost::lexical_cast< std::string >( id_ ) / "session.xml";
    WriteSettings( system, output, GetConfiguration( name_, port_->Get() ) );
    T_Process ptr = starter( *this );
    if( !ptr )
        return false;

    process_ = ptr;
    status_  = Session::STATUS_PLAYING;
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
// Name: Session::Unlink
// Created: BAX 2012-06-06
// -----------------------------------------------------------------------------
void Session::Unlink()
{
    node_.UnlinkExercise( links_ );
}

// -----------------------------------------------------------------------------
// Name: Session::Update
// Created: BAX 2012-06-14
// -----------------------------------------------------------------------------
void Session::Update()
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    if( !process_ || process_->IsAlive() )
        return;
    process_.reset();
    status_ = Session::STATUS_STOPPED;
}
// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Session.h"

#include "PortFactory_ABC.h"
#include "runtime/Process_ABC.h"
#include "runtime/Runtime_ABC.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
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
        case Session::STATUS_PLAYING:   return "running";
        case Session::STATUS_REPLAYING: return "replaying";
        case Session::STATUS_PAUSED:    return "paused";
    }
}

Session::Status ConvertStatus( const std::string& status )
{
    if( status == "running" )   return Session::STATUS_PLAYING;
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

Session::T_Process GetProcess( const boost::property_tree::ptree& tree, const runtime::Runtime_ABC& runtime )
{
    const boost::optional< int > pid = tree.get_optional< int >( "process.pid" );
    if( pid == boost::none )
        return Session::T_Process();
    return runtime.GetProcess( *pid );
}

Session::T_Process AcquireProcess( const boost::property_tree::ptree& tree, const runtime::Runtime_ABC& runtime, int expected )
{
    Session::T_Process ptr = GetProcess( tree, runtime );
    if( !ptr  )
        return Session::T_Process();
    if( expected != tree.get< int >( "port" ) || ptr->GetName() != tree.get< std::string >( "process.name" ) )
        return Session::T_Process();
    return ptr;
}
}

// -----------------------------------------------------------------------------
// Name: Session::Session
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
Session::Session( const boost::uuids::uuid& id, const boost::uuids::uuid& node, const std::string& name, const std::string& exercise, std::auto_ptr< Port_ABC > port )
    : id_      ( id )
    , node_    ( node )
    , name_    ( name )
    , exercise_( exercise )
    , port_    ( port )
    , access_  ( new boost::shared_mutex )
    , process_ ()
    , status_  ( Session::STATUS_STOPPED )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Session::Session
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
Session::Session( const boost::property_tree::ptree& tree, const runtime::Runtime_ABC& runtime, PortFactory_ABC& ports )
    : id_      ( boost::uuids::string_generator()( tree.get< std::string >( "id" ) ) )
    , node_    ( boost::uuids::string_generator()( tree.get< std::string >( "node" ) ) )
    , name_    ( tree.get< std::string >( "name" ) )
    , exercise_( tree.get< std::string >( "exercise" ) )
    , port_    ( AcquirePort( tree.get< int >( "port" ), ports ) )
    , access_  ( new boost::shared_mutex )
    , process_ ( AcquireProcess( tree, runtime, port_->Get() ) )
    , status_  ( process_ ? ConvertStatus( tree.get< std::string >( "status" ) ) : Session::STATUS_STOPPED )
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
boost::uuids::uuid Session::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Session::GetNode
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
boost::uuids::uuid Session::GetNode() const
{
    return node_;
}

// -----------------------------------------------------------------------------
// Name: Session::GetProperties
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
boost::property_tree::ptree Session::GetProperties() const
{
    boost::property_tree::ptree tree;
    tree.put( "id", id_ );
    tree.put( "node", node_ );
    tree.put( "name", name_ );
    tree.put( "exercise", exercise_ );
    tree.put( "port", port_->Get() );
    tree.put( "status", ConvertStatus( status_ ) );
    return tree;
}

// -----------------------------------------------------------------------------
// Name: Session::Save
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
boost::property_tree::ptree Session::Save() const
{
    boost::property_tree::ptree tree = GetProperties();
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    if( !process_ )
        return tree;
    tree.put( "process.pid", process_->GetPid() );
    tree.put( "process.name", process_->GetName() );
    return tree;
}

// -----------------------------------------------------------------------------
// Name: Session::Start
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
bool Session::Start( const T_Starter& starter )
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    if( process_ )
        return true;

    T_Process ptr = starter( *this );
    if( !ptr )
        return false;

    process_ = ptr;
    status_  = Session::STATUS_PLAYING;
    return true;
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
void GetDispatcherConfiguration( boost::property_tree::ptree& tree, int base )
{
    const std::string prefix = "session.config.dispatcher.";
    tree.put( prefix + "network.<xmlattr>.client", "localhost:" + boost::lexical_cast< std::string >( base + SIMULATION_PORT ) );
    tree.put( prefix + "network.<xmlattr>.server", base + DISPATCHER_PORT );
    tree.put( prefix + "plugins.web_control.<xmlattr>.server", base + WEB_CONTROL_PORT );
    tree.put( prefix + "plugins.web_control.<xmlattr>.library", "web_control_plugin_dll-vc100-mt" );
}

void GetSimulationConfiguration( boost::property_tree::ptree& tree, int base )
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
    boost::property_tree::ptree tree;
    tree.put( "session.meta.comment", "Auto-generated by Cloud Host Agent" );
    tree.put( "session.meta.date", boost::posix_time::to_iso_string( boost::posix_time::second_clock::local_time() ) );
    tree.put( "session.meta.name", name );
    GetDispatcherConfiguration( tree, base );
    GetSimulationConfiguration( tree, base );
    std::ostringstream output;
    boost::property_tree::write_xml( output, tree );
    return output.str();
}
}

// -----------------------------------------------------------------------------
// Name: Session::GetConfiguration
// Created: BAX 2012-04-19
// -----------------------------------------------------------------------------
std::string Session::GetConfiguration() const
{
    return ::GetConfiguration( name_, port_->Get() );
}

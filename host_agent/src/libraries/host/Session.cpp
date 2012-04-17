// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Session.h"
#include "FileSystem_ABC.h"
#include "PortFactory_ABC.h"
#include "SecurePool.h"
#include "UuidFactory_ABC.h"

#include "cpplog/cpplog.hpp"
#include "runtime/Process_ABC.h"
#include "runtime/Runtime_ABC.h"
#include "runtime/Utf8.h"

#include <xeumeuleu/xml.hpp>

#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_generators.hpp>
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
    const int MAX_KILL_TIMEOUT_MS = 3*1000;

    // -----------------------------------------------------------------------------
    // Name: Utf8Convert
    // Created: BAX 2012-03-19
    // -----------------------------------------------------------------------------
    std::string Utf8Convert( const boost::filesystem::path& path )
    {
        return runtime::Utf8Convert( path.wstring() );
    }

    // -----------------------------------------------------------------------------
    // Name: SessionPort Enums
    // Created: BAX 2012-03-20
    // -----------------------------------------------------------------------------
    enum SessionPort
    {
        SIMULATION_PORT,
        DISPATCHER_PORT,
        WEB_CONTROL_PORT,
        DIA_DEBUGGER_PORT,
        NETWORK_LOGGER_PORT,
        SESSION_PORT_COUNT,
    };

    template< typename T >
    T ParseItem( xml::xisubstream xis, const std::string& name )
    {
        xis >> xml::start( "session" );
        return xis.attribute< T >( name );
    }

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

    boost::shared_ptr< runtime::Process_ABC > GetProcess( const runtime::Runtime_ABC& runtime, xml::xisubstream xis )
    {
        boost::shared_ptr< runtime::Process_ABC > nil;
        xis >> xml::start( "session" );
        if( !xis.has_attribute( "process_pid" ) || !xis.has_attribute( "process_name" ) )
            return nil;
        boost::shared_ptr< runtime::Process_ABC > ptr = runtime.GetProcess( xis.attribute< int >( "process_pid" ) );
        if( !ptr || ptr->GetName() != xis.attribute< std::string >( "process_name" ) )
            return nil;
        return ptr;
    }
}

// -----------------------------------------------------------------------------
// Name: Session::Session
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Session::Session( cpplog::BaseLogger& log, Pool_ABC& pool,
                  const runtime::Runtime_ABC& runtime, const UuidFactory_ABC& uuids,
                  const FileSystem_ABC& system, const boost::filesystem::path& data,
                  const boost::filesystem::path& applications,
                  const boost::uuids::uuid& node, const std::string& exercise,
                  const std::string& name, PortFactory_ABC& ports )
    : log_         ( log )
    , runtime_     ( runtime )
    , system_      ( system )
    , data_        ( data )
    , applications_( applications )
    , id_          ( uuids.Create() )
    , node_        ( node )
    , exercise_    ( exercise )
    , name_        ( name )
    , pool_        ( new SecurePool( log, "session", pool ) )
    , access_      ( new boost::shared_mutex() )
    , port_        ( ports.Create() )
    , status_      ( STATUS_STOPPED )
{
    CheckPaths();
    LOG_INFO( log_ ) << "[session] + " << id_ << " " << name_;
    Save();
}

// -----------------------------------------------------------------------------
// Name: Session::Session
// Created: BAX 2012-03-21
// -----------------------------------------------------------------------------
Session::Session( cpplog::BaseLogger& log, Pool_ABC& pool,
                  const runtime::Runtime_ABC& runtime, const FileSystem_ABC& system,
                  const boost::filesystem::path& data, const boost::filesystem::path& applications,
                  xml::xistream& xis, PortFactory_ABC& ports )
    : log_         ( log )
    , runtime_     ( runtime )
    , system_      ( system )
    , data_        ( data )
    , applications_( applications )
    , id_          ( boost::uuids::string_generator()( ParseItem< std::string >( xis, "id" ) ) )
    , node_        ( boost::uuids::string_generator()( ParseItem< std::string >( xis, "node" ) ) )
    , exercise_    ( ParseItem< std::string >( xis, "exercise" ) )
    , name_        ( ParseItem< std::string >( xis, "name" ) )
    , pool_        ( new SecurePool( log, "session", pool ) )
    , access_      ( new boost::shared_mutex() )
    , process_     ( GetProcess( runtime_, xis ) )
    , port_        ( ports.Create( ParseItem< int >( xis, "port" ) ) )
    , status_      ( process_ ? ConvertStatus( ParseItem< std::string >( xis, "status" ) ) : STATUS_STOPPED )
{
    LOG_INFO( log_ ) << "[session] + " << id_ << " " << name_;
    if( !process_ )
        LOG_WARN( log_ ) << "[session] " << name_ << " Unable to reload process";
    CheckPaths();
    Save();
}

// -----------------------------------------------------------------------------
// Name: Session::CheckPaths
// Created: BAX 2012-03-21
// -----------------------------------------------------------------------------
void Session::CheckPaths() const
{
    if( !system_.IsDirectory( data_ ) )
        throw std::runtime_error( Utf8Convert( data_ ) + " is not a directory" );
    if( !system_.IsDirectory( applications_ ) )
        throw std::runtime_error( Utf8Convert( applications_ ) + " is not a directory " );
    const boost::filesystem::path app = applications_ / L"simulation_app.exe";
    if( !system_.Exists( app ) )
        throw std::runtime_error( Utf8Convert( app ) + " is missing" );
    if( !system_.IsFile( app ) )
        throw std::runtime_error( Utf8Convert( app ) + " is not a file" );
}

// -----------------------------------------------------------------------------
// Name: Session::~Session
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Session::~Session()
{
    if( process_ )
        process_->Kill( MAX_KILL_TIMEOUT_MS );
    system_.Remove( GetPath() );
    LOG_INFO( log_ ) << "[session] - " << id_ << " " << name_;
}

// -----------------------------------------------------------------------------
// Name: Session::GetTag
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
boost::uuids::uuid Session::GetTag() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Session::GetNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
boost::uuids::uuid Session::GetNode() const
{
    return node_;
}

// -----------------------------------------------------------------------------
// Name: Session::Save
// Created: BAX 2012-03-29
// -----------------------------------------------------------------------------
void Session::Save() const
{
    const boost::filesystem::path path = GetPath();
    system_.CreateDirectory( path );
    pool_->Post( boost::bind( &FileSystem_ABC::WriteFile, &system_, path / L"session.id", ToXml() ) );
}

// -----------------------------------------------------------------------------
// Name: Session::ToJson
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
std::string Session::ToJson() const
{
    boost::shared_lock< boost::shared_mutex > lock( *access_ );
    return (boost::format( "{ "
        "\"id\" : \"%1%\", "
        "\"node\" : \"%2%\", "
        "\"name\" : \"%3%\", "
        "\"port\" : %4%, "
        "\"exercise\" : \"%5%\", "
        "\"status\" : \"%6%\""
        " }" )
        % id_ % node_ % name_ % port_->Get() % exercise_
        % ConvertStatus( status_ )
        ).str();
}

// -----------------------------------------------------------------------------
// Name: Session::ToXml
// Created: BAX 2012-03-21
// -----------------------------------------------------------------------------
std::string Session::ToXml() const
{
    xml::xostringstream xos;
    xos << xml::start( "session" )
            << xml::attribute( "id", boost::lexical_cast< std::string >( id_ ) )
            << xml::attribute( "node", boost::lexical_cast< std::string >( node_ ) )
            << xml::attribute( "exercise", exercise_ )
            << xml::attribute( "name", name_ )
            << xml::attribute( "port", port_->Get() );
    if( process_ )
        xos << xml::attribute( "process_pid", process_->GetPid() )
            << xml::attribute( "process_name", process_->GetName() );
    xos << xml::attribute( "status", ConvertStatus( status_ ) );
    return xos.str();
}

namespace
{

// -----------------------------------------------------------------------------
// Name: WriteDispatchConfiguration
// Created: BAX 2012-03-20
// -----------------------------------------------------------------------------
void WriteDispatcherConfiguration( xml::xostream& xos, int base )
{
    xos << xml::start( "dispatcher" )
            << xml::start( "network" )
                << xml::attribute( "client", "localhost:" + boost::lexical_cast< std::string >( base + SIMULATION_PORT ) )
                << xml::attribute( "server", base + DISPATCHER_PORT )
            << xml::end
            << xml::start( "plugins" )
                << xml::start( "web_control" )
                    << xml::attribute( "server", base + WEB_CONTROL_PORT )
                    << xml::attribute( "library", "web_control_plugin_dll-vc100-mt" )
                << xml::end
            << xml::end
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: WriteSimulationConfiguration
// Created: BAX 2012-03-20
// -----------------------------------------------------------------------------
void WriteSimulationConfiguration( xml::xostream& xos, int base )
{
    xos << xml::start( "simulation" )
            << xml::start( "GarbageCollector" )
                << xml::attribute( "setpause", 100 )
                << xml::attribute( "setstepmul", 100 )
            << xml::end
            << xml::start( "checkpoint" )
                << xml::attribute( "frequency", "100000h" )
                << xml::attribute( "keep", 1 )
                << xml::attribute( "usecrc", true )
            << xml::end
            << xml::start( "debug" )
                << xml::attribute( "decisional", false )
                << xml::attribute( "diadebugger", false )
                << xml::attribute( "diadebuggerport", base + DIA_DEBUGGER_PORT )
                << xml::attribute( "networklogger", false )
                << xml::attribute( "networkloggerport", base + NETWORK_LOGGER_PORT )
                << xml::attribute( "pathfind", false )
            << xml::end
            << xml::start( "decisional" )
                << xml::attribute( "useonlybinaries", false )
            << xml::end
            << xml::start( "dispatcher" )
                << xml::attribute( "embedded", true )
            << xml::end
            << xml::start( "network" )
                << xml::attribute( "port", base + SIMULATION_PORT )
            << xml::end
            << xml::start( "orbat" )
                << xml::attribute( "checkcomposition", false )
            << xml::end
            << xml::start( "pathfinder" )
                << xml::attribute( "threads", 1 )
            << xml::end
            << xml::start( "profiling" )
                << xml::attribute( "enabled", false )
            << xml::end
            << xml::start( "time" )
                << xml::attribute( "factor", 10 )
                << xml::attribute( "step", 10 )
            << xml::end
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: WriteConfiguration
// Created: BAX 2012-03-20
// -----------------------------------------------------------------------------
std::string WriteConfiguration( const std::string& name, int base )
{
    xml::xostringstream xos;
    xos << xml::start( "session" )
            << xml::start( "meta" )
                << xml::content( "comment", "Auto-generated by Cloud Host Agent" )
                << xml::content( "date", boost::posix_time::to_iso_string( boost::posix_time::second_clock::local_time() ) )
                << xml::content( "name", name )
            << xml::end
            << xml::start( "config" );
    WriteDispatcherConfiguration( xos, base );
    WriteSimulationConfiguration( xos, base );
    return xos.str();
}

}

// -----------------------------------------------------------------------------
// Name: Session::GetPath
// Created: BAX 2012-03-29
// -----------------------------------------------------------------------------
boost::filesystem::path Session::GetPath() const
{
    return data_ / L"exercises" / runtime::Utf8Convert( exercise_ ) / L"sessions" / boost::lexical_cast< std::wstring >( id_ );
}

// -----------------------------------------------------------------------------
// Name: Session::Start
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
void Session::Start()
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    if( process_ ) return;
    const boost::filesystem::path path = GetPath();
    system_.WriteFile( path / L"session.xml", WriteConfiguration( name_, port_->Get() ) );
    process_ = runtime_.Start( Utf8Convert( applications_ / L"simulation_app.exe" ), boost::assign::list_of
            ( "--root-dir=\""      + Utf8Convert( data_ ) + "\"" )
            ( "--exercises-dir=\"" + Utf8Convert( data_ / L"exercises" ) + "\"" )
            ( "--terrains-dir=\""  + Utf8Convert( data_ / L"data/terrains" ) + "\"" )
            ( "--models-dir=\""    + Utf8Convert( data_ / L"data/models" ) + "\"" )
            ( "--exercise=\""      + exercise_ + "\"" )
            ( "--session=\""       + boost::lexical_cast< std::string >( id_ ) + "\"" ),
        Utf8Convert( applications_ )
    );
    if( !process_ ) return;

    status_ = STATUS_PLAYING;
    Save();
}

// -----------------------------------------------------------------------------
// Name: Session::Stop
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
void Session::Stop()
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    if( process_ )
        process_->Kill( MAX_KILL_TIMEOUT_MS );
    process_.reset();
    status_ = STATUS_STOPPED;
    Save();
}

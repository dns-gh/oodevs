// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifdef  _MSC_VER
#   define _SCL_SECURE_NO_WARNINGS
#endif

#include "Session.h"
#include "FileSystem_ABC.h"
#include "UuidFactory_ABC.h"

#include <runtime/Process_ABC.h>
#include <runtime/Runtime_ABC.h>
#include <runtime/Utf8.h>

#include <xeumeuleu/xml.h>

#include <boost/assign/list_of.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace host;

namespace
{
    const int MAX_KILL_TIMEOUT = 3*1000;

    // -----------------------------------------------------------------------------
    // Name: Utf8Convert
    // Created: BAX 2012-03-19
    // -----------------------------------------------------------------------------
    std::string Utf8Convert( const boost::filesystem::wpath& path )
    {
        return runtime::Utf8Convert( path.string() );
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
}

// -----------------------------------------------------------------------------
// Name: Session::Session
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Session::Session( const runtime::Runtime_ABC& runtime, const UuidFactory_ABC& uuids,
                  const FileSystem_ABC& system, const boost::filesystem::wpath& data,
                  const boost::filesystem::wpath& applications,
                  const std::string& exercise, const std::string& name, int port )
    : runtime_     ( runtime )
    , system_      ( system )
    , tag_         ( uuids.Create() )
    , data_        ( data )
    , applications_( applications )
    , exercise_    ( exercise )
    , name_        ( name )
    , port_        ( port )
{
    if( !system_.IsDirectory( data_ ) )
        throw std::runtime_error( Utf8Convert( data_ ) + " is not a directory" );
    if( !system_.IsDirectory( applications_ ) )
        throw std::runtime_error( Utf8Convert( applications_ ) + " is not a directory " );
    const boost::filesystem::wpath app = applications_ / L"simulation_app.exe";
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Session::GetTag
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
boost::uuids::uuid Session::GetTag() const
{
    return tag_;
}

namespace
{

// -----------------------------------------------------------------------------
// Name: ToJson
// Created: BAX 2012-03-20
// -----------------------------------------------------------------------------
std::string ToJson( const runtime::Process_ABC& process )
{
    return (boost::format( "{ \"pid\" : %1%, \"name\" : \"%2%\" }" ) % process.GetPid() % process.GetName() ).str();
}

}

// -----------------------------------------------------------------------------
// Name: Session::ToJson
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
std::string Session::ToJson() const
{
    const std::string process = process_ ? ::ToJson( *process_ ) : "{}";
    return (boost::format( "{ \"tag\" : \"%1%\", \"process\" : %2%, \"name\" : \"%3%\", \"port\" : %4%" )
        % tag_ % process % name_ % port_ ).str();
}

namespace
{

// -----------------------------------------------------------------------------
// Name: WriteDispatchConfiguration
// Created: BAX 2012-03-20
// -----------------------------------------------------------------------------
void WriteDispatcherConfiguration( xml::xostringstream& xos, int base )
{
    xos << xml::start( "dispatcher" )
            << xml::start( "network" )
                << xml::attribute( "client", "localhost:" + boost::lexical_cast< std::string >( base + SIMULATION_PORT ) )
                << xml::attribute( "server", base + DISPATCHER_PORT )
            << xml::end //  network
        << xml::end; // dispatcher
}

// -----------------------------------------------------------------------------
// Name: WriteSimulationConfiguration
// Created: BAX 2012-03-20
// -----------------------------------------------------------------------------
void WriteSimulationConfiguration( xml::xostringstream& xos, int base )
{
    xos << xml::start( "simulation" )
            << xml::start( "GarbageCollector" )
                << xml::attribute( "setpause", 100 )
                << xml::attribute( "setstepmul", 100 )
            << xml::end //  GarbageCollector
            << xml::start( "checkpoint" )
                << xml::attribute( "frequency", "100000h" )
                << xml::attribute( "keep", 1 )
                << xml::attribute( "usecrc", true )
            << xml::end //  checkpoint
            << xml::start( "debug" )
                << xml::attribute( "decisional", false )
                << xml::attribute( "diadebugger", false )
                << xml::attribute( "diadebuggerport", base + DIA_DEBUGGER_PORT )
                << xml::attribute( "networklogger", false )
                << xml::attribute( "networkloggerport", base + NETWORK_LOGGER_PORT )
                << xml::attribute( "pathfind", false )
            << xml::end //  debug
            << xml::start( "decisional" )
                << xml::attribute( "useonlybinaries", false )
            << xml::end //  decisional
            << xml::start( "dispatcher" )
                << xml::attribute( "embedded", true )
            << xml::end //  dispatcher
            << xml::start( "network" )
                << xml::attribute( "port", base + SIMULATION_PORT )
            << xml::end //  network
            << xml::start( "orbat" )
                << xml::attribute( "checkcomposition", false )
            << xml::end //  orbat
            << xml::start( "pathfinder" )
                << xml::attribute( "threads", 1 )
            << xml::end //  pathfinder
            << xml::start( "profiling" )
                << xml::attribute( "enabled", false )
            << xml::end //  profiling
            << xml::start( "time" )
                << xml::attribute( "factor", 10 )
                << xml::attribute( "step", 10 )
            << xml::end //  time
        << xml::end; // simulation
}

// -----------------------------------------------------------------------------
// Name: WriteConfiguration
// Created: BAX 2012-03-20
// -----------------------------------------------------------------------------
std::string WriteConfiguration( const std::string& name, int base )
{
    xml::xostringstream xos( xml::encoding( "UTF-8" ) );
    xos << xml::start( "session" )
            << xml::start( "meta" )
                << xml::content( "comment", "Auto-generated by Cloud Host Agent" )
                << xml::content( "date", boost::posix_time::to_iso_string( boost::posix_time::second_clock::local_time() ) )
                << xml::content( "name", name )
            << xml::end //  meta
            << xml::start( "config" );
    WriteDispatcherConfiguration( xos, base );
    WriteSimulationConfiguration( xos, base );
    return xos.str();
}

}

// -----------------------------------------------------------------------------
// Name: Session::Start
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
void Session::Start()
{
    const std::wstring exercise = runtime::Utf8Convert( exercise_ );
    const boost::filesystem::wpath sessionPath = data_ / L"exercises" / exercise / L"sessions" / boost::lexical_cast< std::wstring >( tag_ );
    system_.CreateDirectory( sessionPath );
    system_.WriteFile( sessionPath / L"session.xml", WriteConfiguration( name_, port_ ) );
    process_ = runtime_.Start( Utf8Convert( applications_ / L"simulation_app.exe" ),
        boost::assign::list_of
            ( "--root-dir="      + Utf8Convert( data_ ) )
            ( "--exercises-dir=" + Utf8Convert( data_ / L"exercises" ) )
            ( "--terrains-dir="  + Utf8Convert( data_ / L"data/terrains" ) )
            ( "--models-dir="    + Utf8Convert( data_ / L"data/models" ) )
            ( "--exercise="      + exercise_ )
            ( "--session="       + boost::lexical_cast< std::string >( tag_ ) ),
        Utf8Convert( applications_ )
    );
}

// -----------------------------------------------------------------------------
// Name: Session::Stop
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
void Session::Stop()
{
    if( process_ )
        process_->Kill( MAX_KILL_TIMEOUT );
}

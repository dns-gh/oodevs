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

// -----------------------------------------------------------------------------
// Name: Session::ToJson
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
std::string Session::ToJson() const
{
    const int pid = process_ ? process_->GetPid() : -1;
    const std::string& name = process_ ? process_->GetName() : "";
    return (boost::format( "{ \"tag\" : \"%1%\", \"pid\" : %2%, \"name\" : \"%3%\" }" )
        % boost::lexical_cast< std::string >( tag_ ) % pid % name ).str();
}

// -----------------------------------------------------------------------------
// Name: Session::Start
// Created: BAX 2012-03-20
// -----------------------------------------------------------------------------
void Session::WriteConfiguration( const boost::filesystem::wpath& filename )
{
    xml::xostringstream xos( xml::encoding( "UTF-8" ) );
    xos << xml::start( "session" )
            << xml::start( "config" )
                << xml::start( "dispatcher" )
                    << xml::start( "network" )
                        << xml::attribute( "client", "localhost:10000" )
                        << xml::attribute( "server", 10001 )
                    << xml::end //  network
                    << xml::start( "plugins" )
                        << xml::start( "recorder" )
                            << xml::attribute( "fragmentfreq", 200 )
                            << xml::attribute( "keyframesfreq", 100 )
                        << xml::end //  recorder
                    << xml::end //  plugins
                << xml::end //  dispatcher
                << xml::start( "gaming" )
                    << xml::start( "network" )
                        << xml::attribute( "server", "localhost:10001" )
                    << xml::end //  network
                << xml::end //  gaming
                << xml::start( "simulation" )
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
                        << xml::attribute( "diadebuggerport", 15000 )
                        << xml::attribute( "networklogger", true )
                        << xml::attribute( "networkloggerport", 20000 )
                        << xml::attribute( "pathfind", false )
                    << xml::end //  debug
                    << xml::start( "decisional" )
                        << xml::attribute( "useonlybinaries", false )
                    << xml::end //  decisional
                    << xml::start( "dispatcher" )
                        << xml::attribute( "embedded", true )
                    << xml::end //  dispatcher
                    << xml::start( "network" )
                        << xml::attribute( "port", 10000 )
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
                << xml::end //  simulation
            << xml::end //  config
            << xml::start( "meta" )
                << xml::content( "comment", "Auto-generated by Cloud Host Agent" )
                << xml::content( "date", "00000000T000000" ) // TODO use real date
                << xml::content( "name", "default" );
    system_.WriteFile( filename, xos.str() );
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
    WriteConfiguration( sessionPath / L"session.xml" );
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

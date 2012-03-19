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
                  const boost::filesystem::wpath& applications, const boost::filesystem::wpath& output,
                  const std::string& exercise, int port )
    : runtime_     ( runtime )
    , system_      ( system )
    , tag_         ( uuids.Create() )
    , data_        ( data )
    , applications_( applications )
    , output_      ( output )
    , exercise_    ( exercise )
    , port_        (  port )
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
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
void Session::Start()
{
    const boost::filesystem::wpath root = output_ / boost::lexical_cast< std::wstring >( tag_ );
    system_.CreateDirectory( root );
    system_.Copy( data_ / L"exercises" / runtime::Utf8Convert( exercise_ ), root );
    process_ = runtime_.Start( Utf8Convert( applications_ / L"simulation_app.exe" ),
        boost::assign::list_of
            (  "--root-dir="      + Utf8Convert( data_ ) )
            (  "--exercises-dir=" + Utf8Convert( root ) )
            (  "--terrains-dir="  + Utf8Convert( data_ / L"data/terrains" ) )
            (  "--models-dir="    + Utf8Convert( data_ / L"data/models" ) )
            (  "--exercise="      + exercise_ )
            (  "--session=default" ), Utf8Convert( applications_ )
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

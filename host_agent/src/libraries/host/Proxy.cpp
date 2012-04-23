// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************
#include "Proxy.h"

#include "cpplog/cpplog.hpp"
#include "FileSystem_ABC.h"
#include "Json.h"
#include "runtime/Process_ABC.h"
#include "runtime/Runtime_ABC.h"
#include "runtime/Utf8.h"
#include "SecurePool.h"
#include "web/Client_ABC.h"

#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4244 )
#endif
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

using namespace host;
using runtime::Utf8Convert;

// -----------------------------------------------------------------------------
// Name: Proxy::Proxy
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
Proxy::Proxy( cpplog::BaseLogger& log, const runtime::Runtime_ABC& runtime,
              const FileSystem_ABC& system, const boost::filesystem::path& java,
              const boost::filesystem::path& jar, int port, web::Client_ABC& client,
              Pool_ABC& pool )
    : log_    ( log )
    , runtime_( runtime )
    , system_ ( system )
    , java_   ( java )
    , jar_    ( jar )
    , port_   ( port )
    , pool_   ( new SecurePool( log, "proxy", pool ) )
    , client_ ( client )
    , access_ ( new boost::mutex() )
{
    system_.MakeDirectory( GetPath() );
    const boost::filesystem::path tag = GetPath() / L"proxy.id";
    LOG_INFO( log_ ) << "[proxy] Listening to localhost:" << port;
    bool hasProcess = system_.IsFile( tag );
    if( hasProcess )
    {
        hasProcess = Reload( tag );
        if( !hasProcess )
            LOG_WARN( log_ ) << "[proxy] Unable to reload proxy process";
    }
    if( !hasProcess )
        Start();
}

// -----------------------------------------------------------------------------
// Name: Proxy::~Proxy
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
Proxy::~Proxy()
{
    if( process_ )
        process_->Kill( 0 );
    pool_->Post( boost::bind( &FileSystem_ABC::Remove, &system_, GetPath() / "proxy.id" ) );
}

// -----------------------------------------------------------------------------
// Name: Proxy::GetPort
// Created: BAX 2012-04-13
// -----------------------------------------------------------------------------
int Proxy::GetPort() const
{
    return port_;
}

// -----------------------------------------------------------------------------
// Name: Proxy::GetPath
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
boost::filesystem::path Proxy::GetPath() const
{
    boost::filesystem::path path = jar_;
    return path.remove_filename();
}

// -----------------------------------------------------------------------------
// Name: Proxy::ToXml
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
boost::property_tree::ptree Proxy::GetProperties() const
{
    boost::property_tree::ptree tree;
    tree.put( "port", port_ );
    tree.put( "process.pid", process_->GetPid() );
    tree.put( "process.name", process_->GetName() );
    return tree;
}

// -----------------------------------------------------------------------------
// Name: Proxy::Reload
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
bool Proxy::Reload( const boost::filesystem::path& path )
{
    try
    {
        const boost::property_tree::ptree tree = FromJson( system_.ReadFile( path ) );
        const boost::optional< int > port = tree.get_optional< int >( "port" );
        if( port == boost::none || *port != port_ )
            return false;
        const boost::optional< int > pid  = tree.get_optional< int >( "process.pid" );
        const boost::optional< std::string > name = tree.get_optional< std::string >( "process.name" );
        if( pid == boost::none || name == boost::none )
            return false;
        boost::shared_ptr< runtime::Process_ABC > ptr = runtime_.GetProcess( *pid );
        if( !ptr || ptr->GetName() != *name )
            return false;
        process_ = ptr;
        return true;
    }
    catch( const std::exception& err )
    {
        LOG_DEBUG( log_ ) << "[proxy] Unable to reload proxy from " << path.string() << " - " << err.what();
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: Proxy::Start
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
void Proxy::Start()
{
    boost::lock_guard< boost::mutex > lock( *access_ );
    if( process_ ) return;
    const boost::filesystem::path path = GetPath();
    boost::filesystem::path jar_path = jar_;
    process_ = runtime_.Start( Utf8Convert( java_ ), boost::assign::list_of
            ( " " "-jar \""  + Utf8Convert( jar_.filename() ) + "\"" )
            ( "--port \"" + boost::lexical_cast< std::string >( port_ ) + "\"" ),
            Utf8Convert( jar_path.remove_filename() ) );
    if( !process_ )
        throw new std::runtime_error( "Unable to start proxy process" );
    Save();
}

// -----------------------------------------------------------------------------
// Name: Proxy::Stop
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
void Proxy::Stop()
{
    boost::lock_guard< boost::mutex > lock( *access_ );
    if( process_ )
        process_->Kill( 0 );
    process_.reset();
    Save();
}

// -----------------------------------------------------------------------------
// Name: Proxy::Save
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
void Proxy::Save() const
{
    const boost::filesystem::path path = GetPath();
    pool_->Post( boost::bind( &FileSystem_ABC::WriteFile, &system_, path / L"proxy.id", ToJson( GetProperties() ) ) );
}

// -----------------------------------------------------------------------------
// Name: Proxy::Register
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
void Proxy::Register( const std::string& prefix, const std::string& host, int port ) const
{
    pool_->Post( boost::bind( &web::Client_ABC::Get, &client_,
        "localhost", port_, "/register_proxy", boost::assign::map_list_of
        ( "prefix", prefix )
        ( "host", host )
        ( "port", boost::lexical_cast< std::string >( port ) ) ) );
    LOG_INFO( log_ ) << "[proxy] Added link from /" << prefix << " to " << host << ":" << port;
}

// -----------------------------------------------------------------------------
// Name: Proxy::Unregister
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
void Proxy::Unregister( const std::string& prefix ) const
{
    pool_->Post( boost::bind( &web::Client_ABC::Get, &client_,
        "localhost", port_, "/unregister_proxy", boost::assign::map_list_of( "prefix", prefix ) ) );
    LOG_INFO( log_ ) << "[proxy] Removed link to /" << prefix;
}

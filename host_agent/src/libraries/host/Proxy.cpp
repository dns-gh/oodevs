// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************
#include "Proxy.h"
#include "FileSystem_ABC.h"
#include "Pool_ABC.h"

#include "cpplog/cpplog.hpp"
#include "runtime/Process_ABC.h"
#include "runtime/Runtime_ABC.h"
#include "runtime/Utf8.h"
#include "web/Client_ABC.h"

#include <xeumeuleu/xml.hpp>

#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

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

namespace
{
const int MAX_KILL_TIMEOUT_MS = 3*1000;

// -----------------------------------------------------------------------------
// Name: Utf8Convert
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
std::string Utf8Convert( const boost::filesystem::path& path )
{
    return runtime::Utf8Convert( path.wstring() );
}
}

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
    , pool_   ( pool )
    , client_ ( client )
    , access_ ( new boost::mutex() )
{
    const boost::filesystem::path tag = GetPath() / L"proxy.id";
    bool hasProcess = system_.IsFile( tag );
    if( hasProcess )
        if( !( hasProcess = Reload( tag ) ) )
            LOG_WARN( log_ ) << "[proxy] Unable to reload proxy process";
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
        process_->Kill( MAX_KILL_TIMEOUT_MS );
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
std::string Proxy::ToXml() const
{
    xml::xostringstream xos;
    xos << xml::start( "proxy" )
        << xml::attribute( "port", port_ )
        << xml::attribute( "process_pid", process_->GetPid() )
        << xml::attribute( "process_name", process_->GetName() );
    return xos.str();
}

// -----------------------------------------------------------------------------
// Name: Proxy::Reload
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
bool Proxy::Reload( const boost::filesystem::path& path )
{
    try
    {
        xml::xistringstream xis( system_.ReadFile( path ) );
        xis >> xml::start( "proxy" );
        const int port = xis.attribute< int >( "port" );
        if( port_ != port )
            return false;
        if( !xis.has_attribute( "process_pid" ) || !xis.has_attribute( "process_name" ) )
            return false;
        boost::shared_ptr< runtime::Process_ABC > ptr = runtime_.GetProcess( xis.attribute< int >( "process_pid" ) );
        if( !ptr || ptr->GetName() != xis.attribute< std::string >( "process_name" ) )
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
        process_->Kill( MAX_KILL_TIMEOUT_MS );
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
    system_.CreateDirectory( path );
    system_.WriteFile( path / L"proxy.id", ToXml() );
}

// -----------------------------------------------------------------------------
// Name: Proxy::Register
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
void Proxy::Register( const std::string& prefix, const std::string& host, int port ) const
{
    LOG_INFO( log_ ) << "[proxy] Adding proxy from /" << prefix << " to " << host << ":" << port;
    pool_.Post( boost::bind( &web::Client_ABC::Get, &client_,
        "localhost", port_, "/register_proxy", boost::assign::map_list_of
        ( "prefix", prefix )
        ( "host", host )
        ( "port", boost::lexical_cast< std::string >( port ) ) ) );
}

// -----------------------------------------------------------------------------
// Name: Proxy::Unregister
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
void Proxy::Unregister( const std::string& prefix ) const
{
    LOG_INFO( log_ ) << "[proxy] Removing proxy from /" << prefix;
    pool_.Post( boost::bind( &web::Client_ABC::Get, &client_,
        "localhost", port_, "/unregister_proxy", boost::assign::map_list_of( "prefix", prefix ) ) );
}

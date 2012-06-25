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
#include "PropertyTree.h"
#include "runtime/FileSystem_ABC.h"
#include "runtime/Process_ABC.h"
#include "runtime/Runtime_ABC.h"
#include "runtime/Utf8.h"
#include "web/Client_ABC.h"

#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <set>

using namespace host;
using runtime::Utf8Convert;
using runtime::Async;
using runtime::FileSystem_ABC;
using runtime::Pool_ABC;

namespace
{
enum ProxyState
{
    PROXY_STATE_DISABLED,
    PROXY_STATE_ENABLED,
    PROXY_STATE_ZOMBIE,
    PROXY_STATE_COUNT,
};
}

struct host::ProxyLink
{
    ProxyLink( const std::string& host, int port )
        : host ( host )
        , port ( port )
        , state( PROXY_STATE_DISABLED )
    {
        // NOTHING
    }
    const std::string host;
    const int port;
    ProxyState state;
private:
    ProxyLink& operator=( const ProxyLink& );
};

// -----------------------------------------------------------------------------
// Name: Proxy::Proxy
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
Proxy::Proxy( cpplog::BaseLogger& log, const runtime::Runtime_ABC& runtime,
              const FileSystem_ABC& system, const Path& logs,
              const Path& java, const Path& jar,
              int port, web::Client_ABC& client, Pool_ABC& pool )
    : log_    ( log )
    , runtime_( runtime )
    , system_ ( system )
    , logs_   ( logs )
    , java_   ( java )
    , jar_    ( jar )
    , port_   ( port )
    , client_ ( client )
    , async_  ( pool )
{
    if( !system_.Exists( java_ ) )
        throw std::runtime_error( runtime::Utf8Convert( java_ ) + " is missing" );
    if( !system_.IsFile( java_ ) )
        throw std::runtime_error( runtime::Utf8Convert( java_ ) + " is not a file" );
    if( !system_.Exists( jar_ ) )
        throw std::runtime_error( runtime::Utf8Convert( jar_ ) + " is missing" );
    if( !system_.IsFile( jar_ ) )
        throw std::runtime_error( runtime::Utf8Convert( jar_ ) + " is not a file" );
    const Path tag = GetPath() / "proxy.id";
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
    timer_ = MakeTimer( pool, boost::posix_time::seconds( 5 ), boost::bind( &Proxy::Update, this ) );
}

// -----------------------------------------------------------------------------
// Name: Proxy::~Proxy
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
Proxy::~Proxy()
{
    timer_->Stop();
    if( process_ )
    {
        process_->Kill();
        process_->Join( 1000 );
    }
    async_.Post( boost::bind( &FileSystem_ABC::Remove, &system_, GetPath() / "proxy.id" ) );
}

namespace
{
bool KeepLink( ProxyState state )
{
    return state == PROXY_STATE_DISABLED
        || state == PROXY_STATE_ENABLED;
}
}

// -----------------------------------------------------------------------------
// Name: Proxy::Restart
// Created: BAX 2012-06-15
// -----------------------------------------------------------------------------
void Proxy::Restart()
{
    T_Process next = MakeProcess();
    if( !next )
        return;
    boost::lock_guard< boost::mutex > lock( access_ );
    process_ = next;
    std::set< std::string > cemetery;
    BOOST_FOREACH( T_Links::value_type& value, links_ )
        if( KeepLink( value.second.state ) )
            value.second.state = PROXY_STATE_DISABLED;
        else
            cemetery.insert( value.first );
    BOOST_FOREACH( const std::string& prefix, cemetery )
        links_.erase( prefix );
    Save();
}

// -----------------------------------------------------------------------------
// Name: Proxy::RegisterMissingLinks
// Created: BAX 2012-06-15
// -----------------------------------------------------------------------------
void Proxy::RegisterMissingLinks()
{
    boost::unique_lock< boost::mutex > lock( access_ );
    const T_Links next = links_;
    lock.unlock();
    BOOST_FOREACH( const T_Links::value_type& value, next )
        if( value.second.state == PROXY_STATE_DISABLED )
            async_.Post( boost::bind( &Proxy::HttpRegister, this, value.first, value.second ) );
        else if( value.second.state == PROXY_STATE_ZOMBIE )
            async_.Post( boost::bind( &Proxy::HttpUnregister, this, value.first ) );
}

// -----------------------------------------------------------------------------
// Name: Proxy::Update
// Created: BAX 2012-06-15
// -----------------------------------------------------------------------------
void Proxy::Update()
{
    if( process_ && !process_->IsAlive() )
        Restart();
    else
        RegisterMissingLinks();
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
Path Proxy::GetPath() const
{
    return Path( jar_ ).remove_filename();
}

// -----------------------------------------------------------------------------
// Name: Proxy::ToXml
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
Tree Proxy::GetProperties() const
{
    Tree tree;
    tree.put( "port", port_ );
    tree.put( "process.pid", process_->GetPid() );
    tree.put( "process.name", process_->GetName() );
    return tree;
}

// -----------------------------------------------------------------------------
// Name: Proxy::Reload
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
bool Proxy::Reload( const Path& path )
{
    try
    {
        const Tree tree = FromJson( system_.ReadFile( path ) );
        const boost::optional< int > port = tree.get_optional< int >( "port" );
        if( port == boost::none || *port != port_ )
            return false;
        const boost::optional< int > pid  = tree.get_optional< int >( "process.pid" );
        const boost::optional< std::string > name = tree.get_optional< std::string >( "process.name" );
        if( pid == boost::none || name == boost::none )
            return false;
        T_Process ptr = runtime_.GetProcess( *pid );
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
// Name: Proxy::MakeProcess
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
Proxy::T_Process Proxy::MakeProcess() const
{
    return runtime_.Start( Utf8Convert( java_ ), boost::assign::list_of
            ( "-jar \""  + Utf8Convert( jar_.filename() ) + "\"" )
            ( "--port \"" + boost::lexical_cast< std::string >( port_ ) + "\"" ),
            Utf8Convert( Path( jar_ ).remove_filename() ),
            Utf8Convert( logs_ / "proxy.log" ) );
}

// -----------------------------------------------------------------------------
// Name: Proxy::Start
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
void Proxy::Start()
{
    boost::lock_guard< boost::mutex > lock( access_ );
    if( process_ )
        return;
    process_ = MakeProcess();
    if( !process_ )
        throw std::runtime_error( "Unable to start proxy process" );
    Save();
}

// -----------------------------------------------------------------------------
// Name: Proxy::Stop
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
void Proxy::Stop()
{
    boost::lock_guard< boost::mutex > lock( access_ );
    if( process_ )
    {
        process_->Kill();
        process_->Join( 1000 );
    }
    process_.reset();
    Save();
}

// -----------------------------------------------------------------------------
// Name: Proxy::Save
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
void Proxy::Save() const
{
    async_.Post( boost::bind( &FileSystem_ABC::WriteFile, &system_, GetPath() / "proxy.id", ToJson( GetProperties() ) ) );
}

// -----------------------------------------------------------------------------
// Name: Proxy::Register
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
void Proxy::Register( const std::string& prefix, const std::string& host, int port )
{
    boost::lock_guard< boost::mutex > lock( access_ );
    std::pair< T_Links::iterator, bool > pair = links_.insert( std::make_pair( prefix, ProxyLink( host, port ) ) );
    pair.first->second.state = PROXY_STATE_DISABLED;
    async_.Post( boost::bind( &Proxy::HttpRegister, this, prefix, pair.first->second ) );
    LOG_INFO( log_ ) << "[proxy] Added link from /" << prefix << " to " << host << ":" << port;
}

// -----------------------------------------------------------------------------
// Name: Proxy::HttpRegister
// Created: BAX 2012-06-18
// -----------------------------------------------------------------------------
void Proxy::HttpRegister( const std::string& prefix, const ProxyLink& link )
{
    web::Client_ABC::T_Response response = client_.Get( "localhost", port_, "/register_proxy",
        boost::assign::map_list_of
        ( "prefix", prefix )
        ( "host", link.host )
        ( "port", boost::lexical_cast< std::string >( link.port ) ) );
    if( response->GetStatus() != 200 )
        return;

    boost::lock_guard< boost::mutex > lock( access_ );
    T_Links::iterator it = links_.find( prefix );
    if( it == links_.end() )
        return;
    it->second.state = PROXY_STATE_ENABLED;
}

// -----------------------------------------------------------------------------
// Name: Proxy::Unregister
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
void Proxy::Unregister( const std::string& prefix )
{
    boost::lock_guard< boost::mutex > lock( access_ );
    T_Links::iterator it = links_.find( prefix );
    if( it == links_.end() )
        return;
    it->second.state = PROXY_STATE_ZOMBIE;
    async_.Post( boost::bind( &Proxy::HttpUnregister, this, prefix ) );
    LOG_INFO( log_ ) << "[proxy] Removed link to /" << prefix;
}

// -----------------------------------------------------------------------------
// Name: Proxy::HttpUnregister
// Created: BAX 2012-06-18
// -----------------------------------------------------------------------------
void Proxy::HttpUnregister( const std::string& prefix )
{
    web::Client_ABC::T_Response response = client_.Get( "localhost", port_, "/unregister_proxy",
        boost::assign::map_list_of( "prefix", prefix ) );
    if( response->GetStatus() != 200 )
        return;

    boost::lock_guard< boost::mutex > lock( access_ );
    T_Links::iterator it = links_.find( prefix );
    if( it == links_.end() || it->second.state != PROXY_STATE_ZOMBIE )
        return;
    links_.erase( it );
}
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
#include "runtime/FileSystem_ABC.h"
#include "runtime/Process_ABC.h"
#include "runtime/PropertyTree.h"
#include "runtime/Runtime_ABC.h"
#include "runtime/Utf8.h"
#include "web/Client_ABC.h"

#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread/lock_guard.hpp>
#include <set>

using namespace host;
using namespace host::proxy;
using namespace property_tree;
using runtime::Utf8;
using runtime::Async;
using runtime::FileSystem_ABC;
using runtime::Pool_ABC;

namespace
{
enum State
{
    STATE_DISABLED,
    STATE_ENABLED,
    STATE_ZOMBIE,
    STATE_COUNT,
};
}

struct proxy::Link
{
    Link( const std::string& host, int port )
        : host ( host )
        , port ( port )
        , state( STATE_DISABLED )
    {
        // NOTHING
    }
    const std::string host;
    const int port;
    State state;
private:
    Link& operator=( const Link& );
};

// -----------------------------------------------------------------------------
// Name: Proxy::Proxy
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
Proxy::Proxy( cpplog::BaseLogger& log, const runtime::Runtime_ABC& runtime,
              const FileSystem_ABC& fs, const Config& config,
              web::Client_ABC& client, Pool_ABC& pool )
    : log_    ( log )
    , runtime_( runtime )
    , fs_     ( fs )
    , config_ ( config )
    , client_ ( client )
    , async_  ( pool )
{
    if( !fs_.Exists( config_.app ) )
        throw std::runtime_error( runtime::Utf8( config_.app ) + " is missing" );
    if( !fs_.IsFile( config_.app ) )
        throw std::runtime_error( runtime::Utf8( config_.app ) + " is not a file" );
    const Path tag = config_.root / "proxy.id";
    LOG_INFO( log_ ) << "[proxy] Listening to localhost:" << config_.http;
    bool hasProcess = fs_.IsFile( tag );
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
    async_.Post( boost::bind( &FileSystem_ABC::Remove, &fs_, config_.root / "proxy.id" ) );
}

namespace
{
bool KeepLink( State state )
{
    return state == STATE_DISABLED
        || state == STATE_ENABLED;
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
            value.second.state = STATE_DISABLED;
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
        if( value.second.state == STATE_DISABLED )
            async_.Post( boost::bind( &Proxy::HttpRegister, this, value.first, value.second ) );
        else if( value.second.state == STATE_ZOMBIE )
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
    return config_.http;
}

// -----------------------------------------------------------------------------
// Name: Proxy::GetProperties
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
Tree Proxy::GetProperties() const
{
    Tree tree;
    tree.put( "http", config_.http );
    tree.put( "tcp", config_.tcp );
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
        const Tree tree = FromJson( fs_.ReadFile( path ) );
        const boost::optional< int > http = tree.get_optional< int >( "http" );
        if( !http || *http != config_.http )
            return false;
        const boost::optional< int > tcp = tree.get_optional< int >( "tcp" );
        if( !tcp || *tcp != config_.tcp )
            return false;
        const boost::optional< int > pid  = tree.get_optional< int >( "process.pid" );
        const boost::optional< std::string > name = tree.get_optional< std::string >( "process.name" );
        if( !pid || !name )
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
    std::vector< std::string > args = boost::assign::list_of
        ( "--http \"" + boost::lexical_cast< std::string >( config_.http ) + "\"" )
        ( "--tcp \"" + boost::lexical_cast< std::string >( config_.tcp ) + "\"" );
    if( config_.ssl.enabled )
    {
        args.push_back( "--ssl" );
        args.push_back( "--ssl_certificate \"" + Utf8( config_.ssl.certificate ) + "\"" );
        args.push_back( "--ssl_key \"" + Utf8( config_.ssl.key ) + "\"" );
    }
    return runtime_.Start( Utf8( config_.app ), args,
            Utf8( Path( config_.app ).remove_filename() ),
            Utf8( config_.root / "proxy.log" ) );
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
    async_.Post( boost::bind( &FileSystem_ABC::WriteFile, &fs_, config_.root / "proxy.id", ToJson( GetProperties() ) ) );
}

// -----------------------------------------------------------------------------
// Name: Proxy::Register
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
void Proxy::Register( const std::string& prefix, const std::string& host, int port )
{
    boost::lock_guard< boost::mutex > lock( access_ );
    std::pair< T_Links::iterator, bool > pair = links_.insert( std::make_pair( prefix, Link( host, port ) ) );
    pair.first->second.state = STATE_DISABLED;
    async_.Post( boost::bind( &Proxy::HttpRegister, this, prefix, pair.first->second ) );
    LOG_INFO( log_ ) << "[proxy] Added link from /" << prefix << " to " << host << ":" << port;
}

// -----------------------------------------------------------------------------
// Name: Proxy::HttpRegister
// Created: BAX 2012-06-18
// -----------------------------------------------------------------------------
void Proxy::HttpRegister( const std::string& prefix, const Link& link )
{
    const std::string scheme = config_.ssl.enabled ? "https" : "http";
    web::Client_ABC::T_Response response = client_.Get( scheme, "localhost", config_.http, "/register_proxy",
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
    it->second.state = STATE_ENABLED;
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
    it->second.state = STATE_ZOMBIE;
    async_.Post( boost::bind( &Proxy::HttpUnregister, this, prefix ) );
    LOG_INFO( log_ ) << "[proxy] Removed link to /" << prefix;
}

// -----------------------------------------------------------------------------
// Name: Proxy::HttpUnregister
// Created: BAX 2012-06-18
// -----------------------------------------------------------------------------
void Proxy::HttpUnregister( const std::string& prefix )
{
    const std::string scheme = config_.ssl.enabled ? "https" : "http";
    web::Client_ABC::T_Response response = client_.Get( scheme, "localhost", config_.http, "/unregister_proxy",
        boost::assign::map_list_of( "prefix", prefix ) );
    if( response->GetStatus() != 200 )
        return;

    boost::lock_guard< boost::mutex > lock( access_ );
    T_Links::iterator it = links_.find( prefix );
    if( it == links_.end() || it->second.state != STATE_ZOMBIE )
        return;
    links_.erase( it );
}
// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Node.h"

#include "NodeObserver_ABC.h"
#include "Package_ABC.h"
#include "PortFactory_ABC.h"
#include "UuidFactory_ABC.h"
#include "runtime/FileSystem_ABC.h"
#include "runtime/Process_ABC.h"
#include "runtime/PropertyTree.h"
#include "runtime/Runtime_ABC.h"
#include "runtime/Scoper.h"
#include "runtime/Utf8.h"
#include "web/HttpException.h"
#include "web/Plugins.h"

#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace host;
using namespace property_tree;
using namespace web::node;
using runtime::Utf8;
using runtime::Async;
using runtime::FileSystem_ABC;
using runtime::Pool_ABC;
using runtime::Runtime_ABC;
using runtime::Scoper;

namespace
{
Port AcquirePort( int wanted, PortFactory_ABC& ports )
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

Node::T_Process GetProcess( const Tree& tree, const runtime::Runtime_ABC& runtime )
{
    const boost::optional< int > pid = tree.get_optional< int >( "process.pid" );
    if( pid == boost::none )
        return Node::T_Process();
    return runtime.GetProcess( *pid );
}

Node::T_Process AcquireProcess( const Tree& tree, const runtime::Runtime_ABC& runtime, int expected )
{
    Node::T_Process ptr = GetProcess( tree, runtime );
    if( !ptr  )
        return Node::T_Process();
    if( expected == Get< int >( tree, "port" ) && ptr->GetName() == Get< std::string >( tree, "process.name" ) )
        return ptr;
    return Node::T_Process();
}

// -----------------------------------------------------------------------------
// Name: ReadConfig
// Created: BAX 2012-08-09
// -----------------------------------------------------------------------------
Config ReadConfig( const Tree& src )
{
    Config cfg;
    ReadConfig( cfg, src );
    return cfg;
}

static const std::string install_directory = "_";
}

// -----------------------------------------------------------------------------
// Name: Node::Node
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
Node::Node( const NodeDependencies& deps,
            const Path& root,
            int min_play_seconds,
            const std::string& ident,
            const web::node::Config& cfg )
    : deps_            ( deps )
    , id_              ( deps.uuids.Create() )
    , ident_           ( ident )
    , root_            ( root )
    , port_            ( deps.ports.Create() )
    , min_play_seconds_( min_play_seconds )
    , stopped_         ( false )
    , async_           ( deps.pool )
    , cfg_             ( cfg )
    , num_play_        ( 0 )
    , num_parallel_    ( 0 )
    , num_exercises_   ( 0 )
    , install_size_    ( 0 )
    , cache_size_      ( 0 )
    , sessions_size_   ( 0 )
{
    install_ = deps_.packages.Make( root_ / install_directory, true );
}

// -----------------------------------------------------------------------------
// Name: Node::Node
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
Node::Node( const NodeDependencies& deps,
            const Path& root,
            int min_play_seconds,
            const Tree& tree )
    : deps_            ( deps )
    , id_              ( Get< Uuid >( tree, "id" ) )
    , ident_           ( Get< std::string >( tree, "ident" ) )
    , root_            ( root )
    , port_            ( AcquirePort( Get< int >( tree, "port" ), deps.ports ) )
    , min_play_seconds_( min_play_seconds )
    , process_         ( AcquireProcess( tree, deps_.runtime, port_->Get() ) )
    , stopped_         ( Get< bool >( tree, "stopped" ) )
    , async_           ( deps.pool )
    , cfg_             ( ReadConfig( tree ) )
    , num_play_        ( Get< size_t >( tree, "sessions.num_play" ) )
    , num_parallel_    ( 0 )
    , num_exercises_   ( 0 )
    , install_size_    ( 0 )
    , cache_size_      ( 0 )
    , sessions_size_   ( 0 )
{
    const boost::optional< std::string > cache = tree.get_optional< std::string >( "cache" );
    ParsePackages( cache == boost::none ? Path() : Utf8( *cache ) );
}

// -----------------------------------------------------------------------------
// Name: Node::~Node
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
Node::~Node()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Node::GetId
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
Uuid Node::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Node::GetRoot
// Created: BAX 2012-06-04
// -----------------------------------------------------------------------------
Path Node::GetRoot() const
{
    return root_;
}

// -----------------------------------------------------------------------------
// Name: Node::GetName
// Created: BAX 2012-05-23
// -----------------------------------------------------------------------------
std::string Node::GetIdent() const
{
    return ident_;
}

// -----------------------------------------------------------------------------
// Name: Node::GetPort
// Created: BAX 2012-05-23
// -----------------------------------------------------------------------------
int Node::GetPort() const
{
    return port_->Get();
}

// -----------------------------------------------------------------------------
// Name: Node::GetCommonProperties
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
Tree Node::GetCommonProperties() const
{
    Tree tree;
    tree.put( "id", id_ );
    tree.put( "ident", ident_ );
    tree.put( "port", port_->Get() );
    WriteConfig( tree, cfg_ );
    return tree;
}

// -----------------------------------------------------------------------------
// Name: Node::GetProperties
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
Tree Node::GetProperties() const
{
    boost::shared_lock< boost::shared_mutex > lock( access_ );
    Tree tree = GetCommonProperties();
    tree.put( "num_exercises", num_exercises_ );
    tree.put( "sessions.num_play", num_play_ );
    tree.put( "sessions.num_parallel", num_parallel_ );
    tree.put( "data_size", install_size_ + cache_size_ + sessions_size_ );
    tree.put( "status", process_ ? "running" : "stopped" );
    return tree;
}

// -----------------------------------------------------------------------------
// Name: Node::Save
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
Tree Node::Save() const
{
    boost::shared_lock< boost::shared_mutex > lock( access_ );
    Tree tree = GetCommonProperties();
    tree.put( "sessions.num_play", num_play_ );
    if( cache_ )
        tree.put( "cache", Utf8( cache_->GetPath().filename() ) );
    tree.put( "stopped", stopped_ );
    if( !process_ )
        return tree;
    tree.put( "process.pid", process_->GetPid() );
    tree.put( "process.name", process_->GetName() );
    return tree;
}

// -----------------------------------------------------------------------------
// Name: Node::Start
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
bool Node::Start( const Path& app, const Path& web, const std::string& type,
                  int host, int tcp, bool weak )
{
    // ensure we have a non-exclusive path if current node does not need
    // to restart
    boost::upgrade_lock< boost::shared_mutex > lock( access_ );
    if( stopped_ && weak )
        return false;

    const bool previous = stopped_;
    const bool alive = process_ && process_->IsAlive();
    if( alive && !stopped_ )
        return false;
    if( stopped_ )
    {
        boost::upgrade_to_unique_lock< boost::shared_mutex > write( lock );
        stopped_ = false;
    }
    if( alive )
        return previous;

    std::vector< std::string > args = boost::assign::list_of< std::string >
#ifdef _DEBUG
        ( "--debug" )
#endif
        ( "--www" )( Utf8( web ) )
        ( "--uuid" )( boost::lexical_cast< std::string >( id_ ) )
        ( "--type" )( type )
        ( "--name" )( cfg_.name )
        ( "--host" )( boost::lexical_cast< std::string >( host ) )
        ( "--tcp" )( boost::lexical_cast< std::string >( tcp ) )
        ( "--port" )( boost::lexical_cast< std::string >( port_->Get() ) );
    if( cfg_.sessions.reset )
        args.push_back( "--reset" );
    T_Process ptr = deps_.runtime.Start( Utf8( app ), args, std::string(),
                                         Utf8( root_ / ( type + ".log" ) ) );
    if( !ptr )
        return previous;

    boost::upgrade_to_unique_lock< boost::shared_mutex > write( lock );
    process_ = ptr;
    return true;
}

// -----------------------------------------------------------------------------
// Name: Node::StopProcess
// Created: BAX 2012-06-25
// -----------------------------------------------------------------------------
std::pair< Node::T_Process, bool > Node::StopProcess( bool weak )
{
    T_Process next;
    boost::lock_guard< boost::shared_mutex > lock( access_ );
    bool modified = !stopped_ && !weak;
    stopped_ |= !weak;
    if( !process_ )
        return std::make_pair( next, modified );
    next.swap( process_ );
    next->Kill();
    return std::make_pair( next, true );
}

// -----------------------------------------------------------------------------
// Name: Node::Stop
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
bool Node::Stop( bool weak )
{
    return StopProcess( weak ).second;
}

// -----------------------------------------------------------------------------
// Name: Node::SoftKill
// Created: BAX 2012-07-27
// -----------------------------------------------------------------------------
void Node::SoftKill()
{
    StopProcess( true ).first->Join( 10*1000 );
}

namespace
{
void Cleanup( Node::T_Process process, const FileSystem_ABC& fs, const Path& path )
{
    if( process )
        process->Join( 10*1000 );
    fs.Remove( path );
}
}

// -----------------------------------------------------------------------------
// Name: Node::Remove
// Created: BAX 2012-06-25
// -----------------------------------------------------------------------------
void Node::Remove( Async& async )
{
    std::pair< T_Process, bool > pair = StopProcess( true );
    async.Post( boost::bind( ::Cleanup, pair.first, boost::cref( deps_.fs ), GetRoot() ) );
}

// -----------------------------------------------------------------------------
// Name: Node::Update
// Created: BAX 2012-06-25
// -----------------------------------------------------------------------------
bool Node::Update( const Tree& cfg )
{
    boost::lock_guard< boost::shared_mutex > lock( access_ );
    const std::string name = cfg_.name;
    const bool reset = cfg_.sessions.reset;
    ReadConfig( cfg_, cfg );
    num_play_ = 0;
    if( process_ && process_->IsAlive() )
        return cfg_.name != name || cfg_.sessions.reset != reset;
    return false;
}

namespace
{
template< typename T, typename U >
void ParseInline( const T& packages, U& dst, const Path& path, U reference = U() )
{
    U next = packages.Make( path, !reference );
    if( !next->Parse() )
        return;
    next->Identify( reference ? *reference : *next );
    dst = next;
}
}

// -----------------------------------------------------------------------------
// Name: Node::UploadCache
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
void Node::UploadCache( io::Reader_ABC& src )
{
    const Path output = deps_.fs.MakeAnyPath( root_ );
    boost::shared_ptr< Package_ABC > next;
    try
    {
        FileSystem_ABC::T_Unpacker unpacker = deps_.fs.Unpack( output, src, 0 );
        unpacker->Unpack();
        next = deps_.packages.Make( output, false );
        if( !next->Parse() )
            throw std::runtime_error( "invalid package contents" );
    }
    catch( ... )
    {
        Post( async_, boost::bind( &FileSystem_ABC::Remove, &deps_.fs, output ) );
        throw;
    }

    boost::upgrade_lock< boost::shared_mutex > lock( access_ );
    next->Identify( *install_ );

    boost::upgrade_to_unique_lock< boost::shared_mutex > write( lock );
    next.swap( cache_ );
    cache_size_ = cache_->GetSize();
    if( next )
        Post( async_, boost::bind( &FileSystem_ABC::Remove, &deps_.fs, next->GetPath() ) );
}

// -----------------------------------------------------------------------------
// Name: Node::ParsePackages
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
void Node::ParsePackages( const Path& cache )
{
    boost::lock_guard< boost::shared_mutex > lock( access_ );
    ParseInline( deps_.packages, install_, root_ / install_directory );
    num_exercises_ = install_->CountExercises();
    install_size_ = install_->GetSize();
    if( cache.empty() )
        return;
    ParseInline( deps_.packages, cache_, root_ / cache, install_ );
    cache_size_ = cache_->GetSize();
}

// -----------------------------------------------------------------------------
// Name: Node::GetInstall
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
Tree Node::GetInstall() const
{
    boost::shared_lock< boost::shared_mutex > lock( access_ );
    return install_ ? install_->GetProperties() : Tree();
}

// -----------------------------------------------------------------------------
// Name: Node::GetCache
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
Tree Node::GetCache() const
{
    boost::shared_lock< boost::shared_mutex > lock( access_ );
    return cache_ ? cache_->GetProperties() : Tree();
}

// -----------------------------------------------------------------------------
// Name: Node::DeleteInstall
// Created: BAX 2012-05-22
// -----------------------------------------------------------------------------
Tree Node::DeleteInstall( const std::vector< size_t >& ids )
{
    boost::lock_guard< boost::shared_mutex > lock( access_ );
    install_->Uninstall( async_, root_, ids );
    num_exercises_ = install_->CountExercises();
    install_size_ = install_->GetSize();
    if( cache_ )
        cache_->Identify( *install_ );
    return install_->GetProperties();
}

// -----------------------------------------------------------------------------
// Name: Node::DeleteCache
// Created: BAX 2012-05-22
// -----------------------------------------------------------------------------
Tree Node::DeleteCache()
{
    boost::shared_ptr< Package_ABC > next;
    boost::upgrade_lock< boost::shared_mutex > lock( access_ );
    if( !cache_ )
        return Tree();

    boost::upgrade_to_unique_lock< boost::shared_mutex > write( lock );
    next.swap( cache_ );
    Post( async_, boost::bind( &FileSystem_ABC::Remove, &deps_.fs, next->GetPath() ) );
    cache_size_ = 0;
    return next->GetProperties();
}

// -----------------------------------------------------------------------------
// Name: Node::InstallFromCache
// Created: BAX 2012-05-22
// -----------------------------------------------------------------------------
Tree Node::InstallFromCache( const std::vector< size_t >& list )
{
    boost::upgrade_lock< boost::shared_mutex > lock( access_ );
    if( !cache_ )
        return Tree();

    boost::upgrade_to_unique_lock< boost::shared_mutex > write( lock );
    install_->Install( async_, root_, *cache_, list );
    num_exercises_ = install_->CountExercises();
    install_size_ = install_->GetSize();
    cache_->Identify( *install_ );
    return cache_->GetProperties();
}

// -----------------------------------------------------------------------------
// Name: Node::DownloadInstall
// Created: BAX 2012-09-11
// -----------------------------------------------------------------------------
void Node::DownloadInstall( web::Chunker_ABC& dst, size_t id )
{
    Package_ABC::T_Item item;
    Scoper unlink;
    {
        boost::upgrade_lock< boost::shared_mutex > lock( access_ );
        item = install_->FindId( id );
        if( !item )
            throw web::HttpException( web::NOT_FOUND );
        boost::upgrade_to_unique_lock< boost::shared_mutex > write( lock );
        unlink = boost::bind( &Node::UnlinkExercise, this, install_->LinkItem( *item ) );
    }
    install_->Download( dst, *item );
}

// -----------------------------------------------------------------------------
// Name: Node::DownloadInstall
// Created: BAX 2012-09-18
// -----------------------------------------------------------------------------
void Node::DownloadInstall( web::Chunker_ABC& dst, const std::string& type,
                            const std::string& name, const std::string& checksum )
{
    Package_ABC::T_Item item;
    Scoper unlink;
    {
        boost::upgrade_lock< boost::shared_mutex > lock( access_ );
        item = install_->Find( type, name, checksum );
        if( !item )
            throw web::HttpException( web::NOT_FOUND );
        boost::upgrade_to_unique_lock< boost::shared_mutex > write( lock );
        unlink = boost::bind( &Node::UnlinkExercise, this, install_->LinkItem( *item ) );
    }
    install_->Download( dst, *item );
}

// -----------------------------------------------------------------------------
// Name: Node::GetExercises
// Created: BAX 2012-05-31
// -----------------------------------------------------------------------------
Node::T_Exercises Node::GetExercises( int offset, int limit ) const
{
    boost::shared_lock< boost::shared_mutex > lock( access_ );
    return install_->GetExercises( offset, limit );
}

// -----------------------------------------------------------------------------
// Name: Node::GetExerciseProperties
// Created: NPT 2013-04-05
// -----------------------------------------------------------------------------
Tree Node::GetExerciseProperties( const std::string& exercise ) const
{
    return install_->GetExerciseProperties( exercise );
}

// -----------------------------------------------------------------------------
// Name: Node::CountExercises
// Created: BAX 2012-05-31
// -----------------------------------------------------------------------------
size_t Node::CountExercises() const
{
    boost::shared_lock< boost::shared_mutex > lock( access_ );
    return install_->CountExercises();
}

// -----------------------------------------------------------------------------
// Name: Node::LinkExercise
// Created: BAX 2012-06-06
// -----------------------------------------------------------------------------
Tree Node::LinkExercise( const std::string& name ) const
{
    boost::lock_guard< boost::shared_mutex > lock( access_ );
    return install_->LinkExercise( name );
}

// -----------------------------------------------------------------------------
// Name: Node::LinkExercise
// Created: BAX 2012-06-06
// -----------------------------------------------------------------------------
Tree Node::LinkExercise( const Tree& tree ) const
{
    boost::lock_guard< boost::shared_mutex > lock( access_ );
    return install_->LinkItem( tree );
}

// -----------------------------------------------------------------------------
// Name: Node::UnlinkExercise
// Created: BAX 2012-06-06
// -----------------------------------------------------------------------------
void Node::UnlinkExercise( const Tree& tree )
{
    boost::lock_guard< boost::shared_mutex > lock( access_ );
    const bool modified = install_->UnlinkItem( async_, tree );
    if( modified )
        install_size_ = install_->GetSize();
}

struct node::Token
{
    Token( Node* node, const boost::posix_time::ptime& start )
        : node ( node )
        , start( start )
    {
        // NOTHING
    }
    ~Token()
    {
        if( !node )
            return;
        node->StopSession( start );
    }
private:
    Node* node;
    boost::posix_time::ptime start;
};

namespace
{
template< typename T >
size_t accumulate( const T& map )
{
    size_t sum = 0;
    BOOST_FOREACH( const typename T::value_type& it, map )
        sum += it.second;
    return sum;
}
}

// -----------------------------------------------------------------------------
// Name: Node::StartSession
// Created: BAX 2012-07-18
// -----------------------------------------------------------------------------
Node_ABC::T_Token Node::StartSession( const boost::posix_time::ptime& start, bool first_time )
{
    const bool force = start == boost::posix_time::not_a_date_time;

    boost::upgrade_lock< boost::shared_mutex > lock( access_ );
    if( !first_time && !cfg_.sessions.reset )
        return T_Token();
    if( !force && cfg_.sessions.max_parallel )
        if( num_parallel_ >= cfg_.sessions.max_parallel )
            throw web::HttpException( web::FORBIDDEN );
    if( !force && cfg_.sessions.max_play )
        if( num_play_ >= cfg_.sessions.max_play )
            throw web::HttpException( web::FORBIDDEN );
    {
        boost::upgrade_to_unique_lock< boost::shared_mutex > write( lock );
        ++num_parallel_;
        if( !force )
            ++num_play_;
    }
    lock.unlock(); // release everything

    if( !force )
        deps_.observer.Notify( *this );
    return boost::make_shared< node::Token >( this, start );
}

// -----------------------------------------------------------------------------
// Name: Node::StopSession
// Created: BAX 2012-07-18
// -----------------------------------------------------------------------------
void Node::StopSession( const boost::posix_time::ptime& start )
{
    boost::unique_lock< boost::shared_mutex > lock( access_ );
    num_parallel_ = std::max( size_t( 0 ), num_parallel_ - 1 );
    if( start == boost::posix_time::not_a_date_time )
        return;
    if( start + boost::posix_time::seconds( min_play_seconds_ ) > boost::posix_time::second_clock::local_time() )
        return;
    if( num_play_ < 1 )
        return;
    --num_play_;
    lock.unlock();

    deps_.observer.Notify( *this );
}

// -----------------------------------------------------------------------------
// Name: Node::RemoveSession
// Created: BAX 2012-07-19
// ----------------------------------------------------------------------------
void Node::RemoveSession( const Uuid& id )
{
    boost::lock_guard< boost::shared_mutex > lock( access_ );
    sessions_.erase( id );
    sessions_size_ = accumulate( sessions_ );
}

// -----------------------------------------------------------------------------
// Name: Node::UpdateSessionSize
// Created: BAX 2012-07-19
// ----------------------------------------------------------------------------
void Node::UpdateSessionSize( const Uuid& id, size_t size )
{
    boost::lock_guard< boost::shared_mutex > lock( access_ );
    std::pair< T_Sessions::iterator, bool > it = sessions_.insert( std::make_pair( id, size ) );
    it.first->second = size;
    sessions_size_ = accumulate( sessions_ );
}

// -----------------------------------------------------------------------------
// Name: Node::FilterConfig
// Created: BAX 2012-08-29
// ----------------------------------------------------------------------------
void Node::FilterConfig( web::session::Config& cfg ) const
{
    boost::shared_lock< boost::shared_mutex > lock( access_ );
    std::set< std::string > bad;
    BOOST_FOREACH( const web::session::Config::T_Plugins::value_type& value, cfg.plugins )
        if( !cfg_.plugins.count( value.first ) )
            bad.insert( value.first );
    BOOST_FOREACH( const std::string& id, bad )
        cfg.plugins.erase( id );
    BOOST_FOREACH( const web::node::Config::T_Plugins::value_type& value, cfg_.plugins )
        if( !cfg.plugins.count( value ) )
            cfg.plugins.insert( std::make_pair( value, web::session::PluginConfig( deps_.plugins, value ) ) );
}

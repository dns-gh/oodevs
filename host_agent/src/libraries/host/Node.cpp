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
#include "runtime/Utf8.h"

#include <boost/assign/list_of.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace host;
using namespace property_tree;
using runtime::Utf8Convert;
using runtime::Async;
using runtime::FileSystem_ABC;
using runtime::Pool_ABC;
using runtime::Runtime_ABC;

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
}

// -----------------------------------------------------------------------------
// Name: Node::Node
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
Node::Node( const PackageFactory_ABC& packages,
            const runtime::FileSystem_ABC& system,
            const UuidFactory_ABC& uuids,
            const NodeObserver_ABC& observer,
            runtime::Pool_ABC& pool,
            PortFactory_ABC& ports,
            const NodeConfig& config )
    : packages_         ( packages )
    , system_           ( system )
    , uuids_            ( uuids )
    , observer_         ( observer )
    , id_               ( uuids.Create() )
    , name_             ( config.name )
    , root_             ( config.root )
    , port_             ( ports.Create() )
    , stopped_          ( false )
    , async_            ( pool )
    , num_sessions_     ( config.num_sessions )
    , num_counter_      ( 0 )
    , parallel_sessions_( config.parallel_sessions )
    , parallel_counter_ ( 0 )
    , min_play_seconds_ ( config.min_play_seconds )
{
    install_ = packages_.Make( root_ / "install", true );
}

// -----------------------------------------------------------------------------
// Name: Node::Node
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
Node::Node( const PackageFactory_ABC& packages,
            const runtime::FileSystem_ABC& system,
            const UuidFactory_ABC& uuids,
            const NodeObserver_ABC& observer,
            runtime::Pool_ABC& pool,
            PortFactory_ABC& ports,
            const NodeConfig& config,
            const Tree& tree,
            const runtime::Runtime_ABC& runtime )
    : packages_         ( packages )
    , system_           ( system )
    , uuids_            ( uuids )
    , observer_         ( observer )
    , id_               ( Get< Uuid >( tree, "id" ) )
    , name_             ( Get< std::string >( tree, "name" ) )
    , root_             ( config.root )
    , port_             ( AcquirePort( Get< int >( tree, "port" ), ports ) )
    , process_          ( AcquireProcess( tree, runtime, port_->Get() ) )
    , stopped_          ( Get< bool >( tree, "stopped" ) )
    , async_            ( pool )
    , num_sessions_     ( Get< size_t >( tree, "num_sessions" ) )
    , num_counter_      ( Get< size_t >( tree, "num_counter" ) )
    , parallel_sessions_( Get< size_t >( tree, "parallel_sessions" ) )
    , parallel_counter_ ( 0 )
    , min_play_seconds_ ( config.min_play_seconds )
{
    const boost::optional< std::string > cache = tree.get_optional< std::string >( "cache" );
    ParsePackages( cache == boost::none ? Path() : Utf8Convert( *cache ) );
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
std::string Node::GetName() const
{
    return name_;
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
    tree.put( "name", name_ );
    tree.put( "port", port_->Get() );
    tree.put( "num_sessions", num_sessions_ );
    tree.put( "parallel_sessions", parallel_sessions_ );
    return tree;
}

// -----------------------------------------------------------------------------
// Name: Node::GetProperties
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
Tree Node::GetProperties() const
{
    Tree tree = GetCommonProperties();
    tree.put( "status", process_ ? "running" : "stopped" );
    return tree;
}

// -----------------------------------------------------------------------------
// Name: Node::Save
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
Tree Node::Save() const
{
    Tree tree = GetCommonProperties();

    boost::shared_lock< boost::shared_mutex > lock( access_ );
    tree.put( "num_counter", num_counter_ );
    if( cache_ )
        tree.put( "cache", Utf8Convert( cache_->GetPath().filename() ) );
    tree.put( "stopped", stopped_ );
    if( !process_ )
        return tree;
    tree.put( "process.pid", process_->GetPid() );
    tree.put( "process.name", process_->GetName() );
    return tree;
}

namespace
{
template< typename T >
std::string MakeOption( const std::string& option, const T& value )
{
    return "--" + option + " \"" + boost::lexical_cast< std::string >( value ) + "\"";
}
}

// -----------------------------------------------------------------------------
// Name: Node::Start
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
bool Node::Start( const Runtime_ABC& runtime, const Path& java, const Path& jar,
                  const Path& web, const std::string& type, int host, bool weak )
{
    boost::lock_guard< boost::shared_mutex > lock( access_ );
    if( stopped_ && weak )
        return false;

    bool modified = stopped_;
    stopped_ = false;
    if( process_  && process_->IsAlive() )
        return modified;

    T_Process ptr = runtime.Start( Utf8Convert( java ), boost::assign::list_of
        ( "-jar \"" + Utf8Convert( jar.filename() ) + "\"" )
#ifdef _DEBUG
        ( "--debug" )
#endif
        ( MakeOption( "www",  Utf8Convert( web ) ) )
        ( MakeOption( "uuid", id_ ) )
        ( MakeOption( "type", type ) )
        ( MakeOption( "name", name_ ) )
        ( MakeOption( "host", host ) )
        ( MakeOption( "port", port_->Get() ) ),
        Utf8Convert( Path( jar ).remove_filename() ),
        Utf8Convert( root_ / ( type + ".log" ) ) );
    if( !ptr )
        return modified;

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

namespace
{
void Cleanup( Node::T_Process process, const FileSystem_ABC& system, const Path& path )
{
    if( process )
        process->Join( 10*1000 );
    system.Remove( path );
}
}

// -----------------------------------------------------------------------------
// Name: Node::Remove
// Created: BAX 2012-06-25
// -----------------------------------------------------------------------------
void Node::Remove( const FileSystem_ABC& system, Async& async )
{
    std::pair< T_Process, bool > pair = StopProcess( true );
    async.Go( boost::bind( ::Cleanup, pair.first, boost::cref( system ), GetRoot() ) );
}

// -----------------------------------------------------------------------------
// Name: Node::Update
// Created: BAX 2012-06-25
// -----------------------------------------------------------------------------
void Node::Update( size_t num_sessions, size_t parallel_sessions )
{
    boost::lock_guard< boost::shared_mutex > lock( access_ );
    num_sessions_ = num_sessions;
    num_counter_ = 0;
    parallel_sessions_ = parallel_sessions;
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
void Node::UploadCache( std::istream& src )
{
    const Path output = system_.MakeAnyPath( root_ );
    boost::shared_ptr< Package_ABC > next;
    try
    {
        FileSystem_ABC::T_Unpacker unpacker = system_.Unpack( output, src );
        unpacker->Unpack();
        next = packages_.Make( output, false );
        if( !next->Parse() )
            throw std::runtime_error( "invalid package contents" );
    }
    catch( ... )
    {
        async_.Go( boost::bind( &FileSystem_ABC::Remove, &system_, output ) );
        throw;
    }

    boost::lock_guard< boost::shared_mutex > lock( access_ );
    next->Identify( *install_ );
    next.swap( cache_ );
    if( next )
        async_.Go( boost::bind( &FileSystem_ABC::Remove, &system_, next->GetPath() ) );
}

// -----------------------------------------------------------------------------
// Name: Node::ParsePackages
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
void Node::ParsePackages( const Path& cache )
{
    boost::lock_guard< boost::shared_mutex > lock( access_ );
    ParseInline( packages_, install_, root_ / "install" );
    if( !cache.empty() )
        ParseInline( packages_, cache_,   root_ / cache, install_ );
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
    boost::lock_guard< boost::shared_mutex > lock( access_ );
    if( !cache_ )
        return Tree();
    next.swap( cache_ );
    async_.Go( boost::bind( &FileSystem_ABC::Remove, &system_, next->GetPath() ) );
    return next->GetProperties();
}

// -----------------------------------------------------------------------------
// Name: Node::InstallFromCache
// Created: BAX 2012-05-22
// -----------------------------------------------------------------------------
Tree Node::InstallFromCache( const std::vector< size_t >& list )
{
    boost::lock_guard< boost::shared_mutex > lock( access_ );
    if( !cache_ )
        return Tree();
    install_->Install( async_, root_, *cache_, list );
    cache_->Identify( *install_ );
    return cache_->GetProperties();
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
void Node::UnlinkExercise( const Tree& tree ) const
{
    boost::lock_guard< boost::shared_mutex > lock( access_ );
    install_->UnlinkItem( async_, tree );
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
        node->SessionStop( start );
    }
private:
    Node* node;
    boost::posix_time::ptime start;
};

// -----------------------------------------------------------------------------
// Name: Node::SessionStart
// Created: BAX 2012-07-18
// -----------------------------------------------------------------------------
Node_ABC::T_Token Node::SessionStart( const boost::posix_time::ptime& start )
{
    const bool force = start == boost::posix_time::not_a_date_time;
    boost::unique_lock< boost::shared_mutex > lock( access_ );
    if( !force && parallel_sessions_ )
        if( parallel_counter_ >= parallel_sessions_ )
            return T_Token();
    if( !force && num_sessions_ )
        if( num_counter_ >= num_sessions_ )
            return T_Token();
    ++parallel_counter_;
    if( !force )
        ++num_counter_;
    lock.unlock();

    if( !force )
        observer_.Notify( *this );
    return boost::make_shared< node::Token >( this, start );
}

// -----------------------------------------------------------------------------
// Name: Node::SessionStop
// Created: BAX 2012-07-18
// -----------------------------------------------------------------------------
void Node::SessionStop( const boost::posix_time::ptime& start )
{
    boost::unique_lock< boost::shared_mutex > lock( access_ );
    parallel_counter_ = std::max( size_t( 0 ), parallel_counter_ - 1 );
    if( start == boost::posix_time::not_a_date_time )
        return;
    if( start + boost::posix_time::seconds( min_play_seconds_ ) > boost::posix_time::second_clock::local_time() )
        return;
    if( num_counter_ < 1 )
        return;
    --num_counter_;
    lock.unlock();

    observer_.Notify( *this );
}
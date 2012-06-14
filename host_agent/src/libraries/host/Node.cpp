// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Node.h"

#include "Async.h"
#include "FileSystem_ABC.h"
#include "Package_ABC.h"
#include "PortFactory_ABC.h"
#include "PropertyTree.h"
#include "UuidFactory_ABC.h"
#include "runtime/Process_ABC.h"
#include "runtime/Runtime_ABC.h"
#include "runtime/Utf8.h"

#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4244 )
#endif
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/mutex.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

using namespace host;
using runtime::Utf8Convert;

namespace
{
std::auto_ptr< Port_ABC > AcquirePort( int wanted, PortFactory_ABC& ports )
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
Node::Node( const PackageFactory_ABC& packages, const FileSystem_ABC& system,
            const UuidFactory_ABC& uuids, Pool_ABC& pool, const Path& root,
            const std::string& name, PortFactory_ABC& ports )
    : packages_ ( packages )
    , system_   ( system )
    , uuids_    ( uuids )
    , id_       ( uuids.Create() )
    , name_     ( name )
    , root_     ( root )
    , port_     ( ports.Create() )
    , access_   ( new boost::shared_mutex() )
    , stopped_  ( false )
    , async_    ( new Async( pool ) )
{
    install_ = packages_.Make( root_ / "install", true );
}

// -----------------------------------------------------------------------------
// Name: Node::Node
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
Node::Node( const PackageFactory_ABC& packages, const FileSystem_ABC& system,
            const UuidFactory_ABC& uuids, Pool_ABC& pool, const Path& root,
            const Tree& tree, const runtime::Runtime_ABC& runtime, PortFactory_ABC& ports )
    : packages_ ( packages )
    , system_   ( system )
    , uuids_    ( uuids )
    , id_       ( Get< Uuid >( tree, "id" ) )
    , name_     ( Get< std::string >( tree, "name" ) )
    , root_     ( root )
    , port_     ( AcquirePort( Get< int >( tree, "port" ), ports ) )
    , access_   ( new boost::shared_mutex() )
    , process_  ( AcquireProcess( tree, runtime, port_->Get() ) )
    , stopped_  ( Get< bool >( tree, "stopped" ) )
    , async_    ( new Async( pool ) )
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

    boost::shared_lock< boost::shared_mutex > lock( *access_ );
    if( cache_ )
        tree.put( "cache", Utf8Convert( cache_->GetPath().filename() ) );
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
bool Node::Start( const T_Starter& starter, bool restart )
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    if( stopped_ && restart )
        return false;

    bool modified = stopped_;
    stopped_ = false;
    if( process_  && process_->IsAlive() )
        return modified;

    T_Process ptr = starter( *this );
    if( !ptr )
        return modified;

    process_ = ptr;
    return true;
}

// -----------------------------------------------------------------------------
// Name: Node::Stop
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
bool Node::Stop()
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    bool modified = !stopped_;
    stopped_ = true;
    if( !process_ )
        return modified;

    process_->Kill( 0 );
    process_.reset();
    return true;
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
        async_->Go( boost::bind( &FileSystem_ABC::Remove, &system_, output ) );
        throw;
    }

    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    next->Identify( *install_ );
    next.swap( cache_ );
    if( next )
        async_->Go( boost::bind( &FileSystem_ABC::Remove, &system_, next->GetPath() ) );
}

// -----------------------------------------------------------------------------
// Name: Node::ParsePackages
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
void Node::ParsePackages( const Path& cache )
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
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
    boost::shared_lock< boost::shared_mutex > lock( *access_ );
    return install_ ? install_->GetProperties() : Tree();
}

// -----------------------------------------------------------------------------
// Name: Node::GetCache
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
Tree Node::GetCache() const
{
    boost::shared_lock< boost::shared_mutex > lock( *access_ );
    return cache_ ? cache_->GetProperties() : Tree();
}

// -----------------------------------------------------------------------------
// Name: Node::DeleteInstall
// Created: BAX 2012-05-22
// -----------------------------------------------------------------------------
Tree Node::DeleteInstall( const std::vector< size_t >& ids )
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    install_->Uninstall( *async_, root_, ids );
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
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    if( !cache_ )
        return Tree();
    next.swap( cache_ );
    async_->Go( boost::bind( &FileSystem_ABC::Remove, &system_, next->GetPath() ) );
    return next->GetProperties();
}

// -----------------------------------------------------------------------------
// Name: Node::InstallFromCache
// Created: BAX 2012-05-22
// -----------------------------------------------------------------------------
Tree Node::InstallFromCache( const std::vector< size_t >& list )
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    if( !cache_ )
        return Tree();
    install_->Install( *async_, root_, *cache_, list );
    cache_->Identify( *install_ );
    return cache_->GetProperties();
}

// -----------------------------------------------------------------------------
// Name: Node::GetExercises
// Created: BAX 2012-05-31
// -----------------------------------------------------------------------------
Node::T_Exercises Node::GetExercises( int offset, int limit ) const
{
    boost::shared_lock< boost::shared_mutex > lock( *access_ );
    return install_->GetExercises( offset, limit );
}

// -----------------------------------------------------------------------------
// Name: Node::CountExercises
// Created: BAX 2012-05-31
// -----------------------------------------------------------------------------
size_t Node::CountExercises() const
{
    boost::shared_lock< boost::shared_mutex > lock( *access_ );
    return install_->CountExercises();
}

// -----------------------------------------------------------------------------
// Name: Node::LinkExercise
// Created: BAX 2012-06-06
// -----------------------------------------------------------------------------
Tree Node::LinkExercise( const std::string& name ) const
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    return install_->LinkExercise( name );
}

// -----------------------------------------------------------------------------
// Name: Node::LinkExercise
// Created: BAX 2012-06-06
// -----------------------------------------------------------------------------
Tree Node::LinkExercise( const Tree& tree ) const
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    return install_->LinkItem( tree );
}

// -----------------------------------------------------------------------------
// Name: Node::UnlinkExercise
// Created: BAX 2012-06-06
// -----------------------------------------------------------------------------
void Node::UnlinkExercise( const Tree& tree ) const
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    install_->UnlinkItem( *async_, tree );
}
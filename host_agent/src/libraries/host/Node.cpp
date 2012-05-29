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
    if( expected == tree.get< int >( "port" ) && ptr->GetName() == tree.get< std::string >( "process.name" ) )
        return ptr;
    return Node::T_Process();
}
}

// -----------------------------------------------------------------------------
// Name: Node::Node
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
Node::Node( const PackageFactory_ABC& packages, const FileSystem_ABC& system, Pool_ABC& pool,
            const Path& root, const Uuid& id, const std::string& name, std::auto_ptr< Port_ABC > port )
    : packages_ ( packages )
    , system_   ( system )
    , id_       ( id )
    , name_     ( name )
    , root_     ( root )
    , port_     ( port )
    , access_   ( new boost::shared_mutex() )
    , package_  ( new boost::mutex() )
    , async_    ( new Async( pool ) )
{
    install_ = packages_.Make( GetInstallPath(), true );
}

// -----------------------------------------------------------------------------
// Name: Node::Node
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
Node::Node( const PackageFactory_ABC& packages, const FileSystem_ABC& system, Pool_ABC& pool,
            const Tree& tree, const runtime::Runtime_ABC& runtime, PortFactory_ABC& ports )
    : packages_ ( packages )
    , system_   ( system )
    , id_       ( boost::uuids::string_generator()( tree.get< std::string >( "id" ) ) )
    , name_     ( tree.get< std::string >( "name" ) )
    , root_     ( Utf8Convert( tree.get< std::string >( "root" ) ) )
    , port_     ( AcquirePort( tree.get< int >( "port" ), ports ) )
    , access_   ( new boost::shared_mutex() )
    , package_  ( new boost::mutex() )
    , process_  ( AcquireProcess( tree, runtime, port_->Get() ) )
    , async_    ( new Async( pool ) )
{
    async_->Post( boost::bind( &Node::ParsePackages, this ) );
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
// Name: Node::GetCachePath
// Created: BAX 2012-05-23
// -----------------------------------------------------------------------------
Path Node::GetCachePath() const
{
    return root_ / boost::lexical_cast< std::string >( id_ ) / "cache";
}

// -----------------------------------------------------------------------------
// Name: Node::GetInstallPath
// Created: BAX 2012-05-23
// -----------------------------------------------------------------------------
Path Node::GetInstallPath() const
{
    return root_ / boost::lexical_cast< std::string >( id_ ) / "install";
}

// -----------------------------------------------------------------------------
// Name: Node::Save
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
Tree Node::Save() const
{
    Tree tree = GetCommonProperties();
    tree.put( "root", Utf8Convert( root_ ) );

    boost::shared_lock< boost::shared_mutex > lock( *access_ );
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
bool Node::Start( const T_Starter& starter )
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    if( process_ )
        return true;

    T_Process ptr = starter( *this );
    if( !ptr )
        return false;

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
    if( !process_ )
        return true;

    process_->Kill( 0 );
    process_.reset();
    return true;
}

namespace
{
template< typename T, typename U >
void Reset( T& access, U& dst )
{
    boost::lock_guard< T > lock( access );
    dst.reset();
}

template< typename T, typename U, typename V >
void Parse( T& access, const U& packages, V& dst, const Path& path, V reference )
{
    V next = packages.Make( path, !reference );
    bool valid = next->Parse();
    if( !valid )
        return;

    boost::lock_guard< T > write( access );
    if( reference )
        next->Identify( *reference );
    dst = next;
}

template< typename T, typename U >
U Steal( T& access, U& src )
{
    U reply;
    boost::lock_guard< T > lock( access );
    reply.swap( src );
    return reply;
}

template< typename T, typename U, typename V >
void ParsePackage( T& access, const U& packages, V& ptr, const Path& path, V ref = V() )
{
    Reset( access, ptr );
    Parse( access, packages, ptr, path, ref );
}
}

// -----------------------------------------------------------------------------
// Name: Node::UploadCache
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
void Node::UploadCache( std::istream& src )
{
    boost::mutex::scoped_try_lock lock( *package_ );
    if( !lock.owns_lock() )
        return;

    Reset( *access_, cache_ );
    const Path path = GetCachePath();
    system_.Remove( path );
    system_.MakeDirectory( path );
    FileSystem_ABC::T_Unpacker unpacker = system_.Unpack( path, src );
    unpacker->Unpack();
    ::Parse( *access_, packages_, cache_, path, install_ );
}

// -----------------------------------------------------------------------------
// Name: Node::ParsePackages
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
void Node::ParsePackages()
{
    boost::mutex::scoped_try_lock lock( *package_ );
    if( !lock.owns_lock() )
        return;
    ::ParsePackage( *access_, packages_, install_, GetInstallPath() );
    ::ParsePackage( *access_, packages_, cache_, GetCachePath(), install_ );
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
    install_->Remove( ids );
    return install_->GetProperties();
}

// -----------------------------------------------------------------------------
// Name: Node::DeleteCache
// Created: BAX 2012-05-22
// -----------------------------------------------------------------------------
Tree Node::DeleteCache()
{
    boost::shared_ptr< Package_ABC > next = Steal( *access_, cache_ );
    return next ? next->GetProperties() : Tree();
}

// -----------------------------------------------------------------------------
// Name: Node::InstallFromCache
// Created: BAX 2012-05-22
// -----------------------------------------------------------------------------
Tree Node::InstallFromCache( const std::vector< size_t >& list )
{
    boost::mutex::scoped_try_lock lock( *package_ );
    if( !lock.owns_lock() )
        return Tree();
    if( !cache_ )
        return Tree();
    install_->Install( *cache_, list );
    ::ParsePackage( *access_, packages_, install_, GetInstallPath() );
    ::ParsePackage( *access_, packages_, cache_, GetCachePath(), install_ );
    return GetCache();
}
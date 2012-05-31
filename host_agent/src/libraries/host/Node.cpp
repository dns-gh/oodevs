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
    if( expected == tree.get< int >( "port" ) && ptr->GetName() == tree.get< std::string >( "process.name" ) )
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
    , async_    ( new Async( pool ) )
{
    install_ = packages_.Make( GetInstallPath(), true );
}

// -----------------------------------------------------------------------------
// Name: Node::Node
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
Node::Node( const PackageFactory_ABC& packages, const FileSystem_ABC& system,
            const UuidFactory_ABC& uuids, Pool_ABC& pool, const Tree& tree,
            const runtime::Runtime_ABC& runtime, PortFactory_ABC& ports )
    : packages_ ( packages )
    , system_   ( system )
    , uuids_    ( uuids )
    , id_       ( boost::uuids::string_generator()( tree.get< std::string >( "id" ) ) )
    , name_     ( tree.get< std::string >( "name" ) )
    , root_     ( Utf8Convert( tree.get< std::string >( "root" ) ) )
    , port_     ( AcquirePort( tree.get< int >( "port" ), ports ) )
    , access_   ( new boost::shared_mutex() )
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
// Name: Node::GetTemporaryPath
// Created: BAX 2012-05-30
// -----------------------------------------------------------------------------
Path Node::GetTemporaryPath() const
{
    return root_ / boost::lexical_cast< std::string >( id_ )
                 / boost::lexical_cast< std::string >( uuids_.Create() ).substr( 0, 8 );
}

// -----------------------------------------------------------------------------
// Name: Node::UploadCache
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
void Node::UploadCache( std::istream& src )
{
    const Path output = GetTemporaryPath();
    system_.MakeDirectory( output );
    FileSystem_ABC::T_Unpacker unpacker = system_.Unpack( output, src );
    unpacker->Unpack();

    boost::shared_ptr< Package_ABC > next = packages_.Make( output, false );
    if( !next->Parse() )
        return;

    const Path& path = GetCachePath();
    const Path other = GetTemporaryPath();

    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    system_.Rename( path, other );
    cache_ = next;
    cache_->Move( path );
    cache_->Identify( *install_ );
    async_->Go( boost::bind( &FileSystem_ABC::Remove, &system_, other ) );
}

// -----------------------------------------------------------------------------
// Name: Node::ParsePackages
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
void Node::ParsePackages()
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    ParseInline( packages_, install_, GetInstallPath() );
    ParseInline( packages_, cache_, GetCachePath(), install_ );
}

// -----------------------------------------------------------------------------
// Name: Node::IdentifyPackages
// Created: BAX 2012-05-30
// -----------------------------------------------------------------------------
void Node::IdentifyPackages()
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    install_->Identify( *install_ );
    cache_->Identify( *install_ );
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
    const Path other = GetTemporaryPath();
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    install_->Move( other, ids );
    install_->Identify( *install_ );
    if( cache_ )
        cache_->Identify( *install_ );
    async_->Go( boost::bind( &FileSystem_ABC::Remove, &system_, other ) );
    return install_->GetProperties();
}

// -----------------------------------------------------------------------------
// Name: Node::DeleteCache
// Created: BAX 2012-05-22
// -----------------------------------------------------------------------------
Tree Node::DeleteCache()
{
    const Path other = GetTemporaryPath();
    boost::shared_ptr< Package_ABC > next;
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    if( !cache_ )
        return Tree();
    next.swap( cache_ );
    next->Move( other );
    async_->Go( boost::bind( &FileSystem_ABC::Remove, &system_, other ) );
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
    install_->Install( *cache_, list );
    ParseInline( packages_, install_, GetInstallPath() );
    cache_->Identify( *install_ );
    return cache_->GetProperties();
}
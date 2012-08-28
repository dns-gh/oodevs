// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "NodeController.h"

#include "cpplog/cpplog.hpp"
#include "Node_ABC.h"
#include "Proxy_ABC.h"
#include "runtime/FileSystem_ABC.h"
#include "runtime/PropertyTree.h"
#include "runtime/Utf8.h"
#include "web/Configs.h"
#include "web/Plugins.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace host;
using namespace property_tree;
using runtime::Utf8Convert;
using runtime::Async;
using runtime::FileSystem_ABC;
using runtime::Pool_ABC;

// -----------------------------------------------------------------------------
// Name: NodeController::NodeController
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::NodeController( cpplog::BaseLogger& log,
                                const runtime::Runtime_ABC& runtime,
                                const FileSystem_ABC& system,
                                const web::Plugins& plugins,
                                const NodeFactory_ABC& nodes,
                                const Path& root,
                                const Path& app,
                                const Path& web,
                                const std::string& type,
                                int host,
                                Pool_ABC& pool,
                                Proxy_ABC& proxy )
    : log_     ( log )
    , runtime_ ( runtime )
    , system_  ( system )
    , plugins_ ( plugins )
    , factory_ ( nodes )
    , root_    ( root / ( type == "cluster" ? type : "nodes" ) )
    , app_     ( app )
    , web_     ( web )
    , type_    ( type )
    , host_    ( host )
    , proxy_   ( proxy )
    , async_   ( pool )
{
    system.MakePaths( root_ );
    if( !system_.IsFile( app_ ) )
        throw std::runtime_error( "'" + runtime::Utf8Convert( app_ ) + "' is not a binary" );
    if( !system_.IsDirectory( web_ ) )
        throw std::runtime_error( "'" + runtime::Utf8Convert( web_ ) + "' is not a directory" );
    timer_ = runtime::MakeTimer( pool, boost::posix_time::seconds( 5 ), boost::bind( &NodeController::Refresh, this ) );
}

// -----------------------------------------------------------------------------
// Name: NodeController::NodeController
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::~NodeController()
{
    timer_->Stop();
    async_.Join();
    nodes_.ForeachRef( boost::bind( &NodeController::Stop, this, _1, false, true ) );
}

namespace
{
bool HasSameIdent( const std::string& ident, const Node_ABC& node )
{
    return ident == node.GetIdent();
}
}

// -----------------------------------------------------------------------------
// Name: NodeController::ReloadNode
// Created: BAX 2012-07-20
// -----------------------------------------------------------------------------
void NodeController::ReloadNode( const Path& path )
{
    try
    {
        T_Node node = factory_.Make( path );
        if( !node )
            return;

        bool valid = nodes_.AttachUnless( node, boost::bind( &HasSameIdent, node->GetIdent(), _1 ) );
        if( !valid )
            return;

        Create( *node, true );
    }
    catch( const std::exception& err )
    {
        LOG_WARN( log_ ) << "[" << type_ << "] " << err.what();
        LOG_WARN( log_ ) << "[" << type_ << "] Unable to reload " << Utf8Convert( path );
    }
}

// -----------------------------------------------------------------------------
// Name: NodeController::ReloadDirectory
// Created: BAX 2012-08-10
// -----------------------------------------------------------------------------
bool NodeController::ReloadDirectory( runtime::Async& reload, const Path& dir )
{
    const Path path = dir / ( type_ + ".id" );
    if( system_.IsFile( path ) )
        reload.Go( boost::bind( &NodeController::ReloadNode, this, path ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NodeController::Reload
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
void NodeController::Reload()
{
    Async reload( async_.GetPool() );
    system_.Walk( root_, false, boost::bind( &NodeController::ReloadDirectory, this, boost::ref( reload ), _1 ) );
}

// -----------------------------------------------------------------------------
// Name: NodeController::Refresh
// Created: BAX 2012-06-14
// -----------------------------------------------------------------------------
void NodeController::Refresh()
{
    nodes_.ForeachRef( boost::bind( &NodeController::Start, this, _1, false, true ) );
}

// -----------------------------------------------------------------------------
// Name: NodeController::List
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::T_Nodes NodeController::List( int offset, int limit ) const
{
    return nodes_.List< Node_ABC >( offset, limit );
}

// -----------------------------------------------------------------------------
// Name: NodeController::Count
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
size_t NodeController::Count() const
{
    return nodes_.Count();
}

// -----------------------------------------------------------------------------
// Name: NodeController::Has
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
bool NodeController::Has( const Uuid& id ) const
{
    return nodes_.Has( id );
}

// -----------------------------------------------------------------------------
// Name: NodeController::Get
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::T_Node NodeController::Get( const Uuid& id ) const
{
    return nodes_.Get( id );
}

// -----------------------------------------------------------------------------
// Name: NodeController::Create
// Created: BAX 2012-04-23
// -----------------------------------------------------------------------------
void NodeController::Create( Node_ABC& node, bool reload )
{
    const int port = node.GetPort();
    LOG_INFO( log_ ) << "[" << type_ << "] " << ( reload ? "Reloaded " : "Added " ) << node.GetId() << " " << node.GetIdent() << " :" << port;
    if( !reload )
        system_.MakePaths( node.GetRoot() );
    proxy_.Register( node.GetIdent(), "localhost", port );
    if( reload )
        Save( node );
    else
        Start( node, true, false );
}

// -----------------------------------------------------------------------------
// Name: NodeController::IsValid
// Created: BAX 2012-08-23
// -----------------------------------------------------------------------------
bool NodeController::IsValid( const web::node::Config& cfg ) const
{
    BOOST_FOREACH( const std::string& plugin, cfg.plugins )
        if( !plugins_.Has( plugin ) )
            return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: NodeController::Create
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::T_Node NodeController::Create( const std::string& ident, const web::node::Config& cfg )
{
    if( !IsValid( cfg ) )
        return T_Node();

    const Path output = system_.MakeAnyPath( root_ );
    T_Node node = factory_.Make( output, ident, cfg );
    bool valid = nodes_.AttachUnless( node, boost::bind( &HasSameIdent, ident, _1 ) );
    if( !valid )
    {
        async_.Go( boost::bind( &FileSystem_ABC::Remove, &system_, output ) );
        return T_Node();
    }

    Create( *node, false );
    return node;
}

// -----------------------------------------------------------------------------
// Name: NodeController::Save
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
void NodeController::Save( const Node_ABC& node ) const
{
    const Path path = node.GetRoot() / ( type_ + ".id" );
    async_.Post( boost::bind( &FileSystem_ABC::WriteFile, &system_, path, ToJson( node.Save() ) ) );
}

// -----------------------------------------------------------------------------
// Name: NodeController::Delete
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::T_Node NodeController::Delete( const Uuid& id )
{
    T_Node node = nodes_.Detach( id );
    if( !node )
        return node;
    LOG_INFO( log_ ) << "[" << type_ << "] Removed " << node->GetId() << " " << node->GetIdent() << " :" << node->GetPort();
    proxy_.Unregister( node->GetIdent() );
    node->Remove( async_ );
    return node;
}

// -----------------------------------------------------------------------------
// Name: NodeController::Start
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::T_Node NodeController::Start( const Uuid& id ) const
{
    T_Node node = nodes_.Get( id );
    if( node )
        Start( *node, false, false );
    return node;
}

// -----------------------------------------------------------------------------
// Name: NodeController::Stop
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::T_Node NodeController::Stop( const Uuid& id ) const
{
    T_Node node = nodes_.Get( id );
    if( node )
        Stop( *node, false, false );
    return node;
}

// -----------------------------------------------------------------------------
// Name: NodeController::Start
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
void NodeController::Start( Node_ABC& node, bool force, bool weak ) const
{
    bool modified = node.Start( app_, web_, type_, host_, weak );
    if( modified || force )
        Save( node );
}

// -----------------------------------------------------------------------------
// Name: NodeController::Stop
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
void NodeController::Stop( Node_ABC& node, bool skip, bool weak ) const
{
    bool modified = node.Stop( weak );
    if( modified && !skip )
        Save( node );
}

// -----------------------------------------------------------------------------
// Name: NodeController::Update
// Created: BAX 2012-07-27
// -----------------------------------------------------------------------------
NodeController::T_Node NodeController::Update( const Uuid& id, const Tree& cfg )
{
    T_Node node = nodes_.Get( id );
    if( !node )
        return node;
    bool restart = node->Update( cfg );
    if( restart )
    {
        node->SoftKill();
        Start( *node, false, true );
    }
    else
    {
        Save( *node );
    }
    return node;
}

// -----------------------------------------------------------------------------
// Name: NodeController::GetInstall
// Created: BAX 2012-05-25
// -----------------------------------------------------------------------------
Tree NodeController::GetInstall( const Uuid& id ) const
{
    T_Node node = nodes_.Get( id );
    return node ? node->GetInstall() : Tree();
}

// -----------------------------------------------------------------------------
// Name: NodeController::DeleteInstall
// Created: BAX 2012-05-25
// -----------------------------------------------------------------------------
Tree NodeController::DeleteInstall( const Uuid& id, const std::vector< size_t >& list )
{
    T_Node node = nodes_.Get( id );
    return node ? node->DeleteInstall( list ) : Tree();
}

// -----------------------------------------------------------------------------
// Name: NodeController::GetCache
// Created: BAX 2012-05-11
// -----------------------------------------------------------------------------
Tree NodeController::GetCache( const Uuid& id ) const
{
    T_Node node = nodes_.Get( id );
    return node ? node->GetCache() : Tree();
}

// -----------------------------------------------------------------------------
// Name: NodeController::UploadCache
// Created: BAX 2012-05-11
// -----------------------------------------------------------------------------
Tree NodeController::UploadCache( const Uuid& id, std::istream& src ) const
{
    T_Node node = nodes_.Get( id );
    if( !node )
        return Tree();
    try
    {
        node->UploadCache( src );
        Save( *node );
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << "[" << type_ << "] " << err.what();
        LOG_ERROR( log_ ) << "[" << type_ << "] Unable to unpack cache";
    }
    return node->GetCache();
}

// -----------------------------------------------------------------------------
// Name: NodeController::DeleteCache
// Created: BAX 2012-05-23
// -----------------------------------------------------------------------------
Tree NodeController::DeleteCache( const Uuid& id )
{
    T_Node node = nodes_.Get( id );
    if( !node )
        return Tree();
    const Tree tree = node->DeleteCache();
    Save( *node );
    return tree;
}

// -----------------------------------------------------------------------------
// Name: NodeController::InstallFromCache
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
Tree NodeController::InstallFromCache( const Uuid& id, const std::vector< size_t >& list )
{
    T_Node node = nodes_.Get( id );
    return node ? node->InstallFromCache( list ) : Tree();
}

// -----------------------------------------------------------------------------
// Name: NodeController::GetExercises
// Created: BAX 2012-05-31
// -----------------------------------------------------------------------------
NodeController::T_Exercises NodeController::GetExercises( const Uuid& id, int offset, int limit ) const
{
    T_Node node = nodes_.Get( id );
    return node ? node->GetExercises( offset, limit ) : T_Exercises();
}

// -----------------------------------------------------------------------------
// Name: NodeController::CountExercises
// Created: BAX 2012-05-31
// -----------------------------------------------------------------------------
size_t NodeController::CountExercises( const Uuid& id ) const
{
    T_Node node = nodes_.Get( id );
    return node ? node->CountExercises() : 0;
}

// -----------------------------------------------------------------------------
// Name: NodeController::Notify
// Created: BAX 2012-07-18
// -----------------------------------------------------------------------------
void NodeController::Notify( const Node_ABC& node ) const
{
    Save( node );
}

// -----------------------------------------------------------------------------
// Name: NodeController::GetPlugins
// Created: BAX 2012-08-23
// -----------------------------------------------------------------------------
NodeController_ABC::T_Plugins NodeController::GetPlugins( int offset, int limit ) const
{
    return plugins_.GetNames( offset, limit );
}

// -----------------------------------------------------------------------------
// Name: NodeController::CountPlugins
// Created: BAX 2012-08-23
// -----------------------------------------------------------------------------
size_t NodeController::CountPlugins() const
{
    return plugins_.Count();
}

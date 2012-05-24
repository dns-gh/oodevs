// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "NodeController.h"

#include "Container.h"
#include "cpplog/cpplog.hpp"
#include "FileSystem_ABC.h"
#include "Node_ABC.h"
#include "Pool_ABC.h"
#include "PortFactory_ABC.h"
#include "PropertyTree.h"
#include "Proxy_ABC.h"
#include "runtime/Process_ABC.h"
#include "runtime/Runtime_ABC.h"
#include "runtime/Utf8.h"
#include "SecurePool.h"
#include "UuidFactory_ABC.h"

#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace host;
using runtime::Utf8Convert;

namespace
{
template< typename T >
std::string MakeOption( const std::string& option, const T& value )
{
    return "--" + option + " \"" + boost::lexical_cast< std::string >( value ) + "\"";
}

Path GetPath( const Path& root, const Node_ABC& node )
{
    return root / boost::lexical_cast< std::wstring >( node.GetId() );
}

std::string GetPrefix( const std::string& type, const Node_ABC& node )
{
    return type == "cluster" ? type : boost::lexical_cast< std::string >( node.GetId() );
}
}

// -----------------------------------------------------------------------------
// Name: NodeController::NodeController
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::NodeController( cpplog::BaseLogger& log,
                                const runtime::Runtime_ABC& runtime,
                                const FileSystem_ABC& system,
                                const Proxy_ABC& proxy,
                                const NodeFactory_ABC& nodes,
                                const Path& root,
                                const Path& java,
                                const Path& jar,
                                const Path& web,
                                const std::string& type,
                                Pool_ABC& pool )
    : log_     ( log )
    , runtime_ ( runtime )
    , system_  ( system )
    , proxy_   ( proxy )
    , factory_ ( nodes )
    , root_    ( root / type )
    , java_    ( java )
    , jar_     ( jar )
    , web_     ( web )
    , type_    ( type )
    , pool_    ( new SecurePool( log, type, pool ) )
    , nodes_   ( new Container< Node_ABC >() )
{
    system.MakeDirectory( root_ );
    if( !system_.Exists( java_ ) )
        throw std::runtime_error( runtime::Utf8Convert( java_ ) + " is missing" );
    if( !system_.IsFile( java_ ) )
        throw std::runtime_error( runtime::Utf8Convert( java_ ) + " is not a file" );
    if( !system_.Exists( jar_ ) )
        throw std::runtime_error( runtime::Utf8Convert( jar_ ) + " is missing" );
    if( !system_.IsFile( jar_ ) )
        throw std::runtime_error( runtime::Utf8Convert( jar_ ) + " is not a file" );
    if( !system_.IsDirectory( web_ ) )
        throw std::runtime_error( runtime::Utf8Convert( web_ ) + " is not a directory" );
}

// -----------------------------------------------------------------------------
// Name: NodeController::NodeController
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::~NodeController()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NodeController::Reload
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
void NodeController::Reload()
{
    BOOST_FOREACH( const Path& path, system_.Glob( root_, Utf8Convert( type_ ) + L".id" ) )
        try
        {
            boost::shared_ptr< Node_ABC > node = factory_.Make( FromJson( system_.ReadFile( path ) ) );
            if( !node )
                continue;
            nodes_->Attach( node );
            Create( *node, true );
        }
        catch( const std::exception& err )
        {
            LOG_WARN( log_ ) << "[" << type_ << "] Unable to reload " << Utf8Convert( path ) << " - " << err.what();
            continue; // skip invalid entry
        }
}

// -----------------------------------------------------------------------------
// Name: NodeController::List
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::T_Nodes NodeController::List( int offset, int limit ) const
{
    return nodes_->List< Node_ABC >( boost::function< bool( const Node_ABC& ) >(), offset, limit );
}

// -----------------------------------------------------------------------------
// Name: NodeController::Count
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
size_t NodeController::Count() const
{
    return nodes_->Count();
}

// -----------------------------------------------------------------------------
// Name: NodeController::Has
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
bool NodeController::Has( const Uuid& id ) const
{
    return nodes_->Has( id );
}

// -----------------------------------------------------------------------------
// Name: NodeController::Get
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::T_Node NodeController::Get( const Uuid& id ) const
{
    return nodes_->Get( id );
}

// -----------------------------------------------------------------------------
// Name: NodeController::Create
// Created: BAX 2012-04-23
// -----------------------------------------------------------------------------
void NodeController::Create( Node_ABC& node, bool isReload )
{
    const int port = node.GetPort();
    LOG_INFO( log_ ) << "[" << type_ << "] " << ( isReload ? "Reloaded " : "Added " ) << node.GetId() << " " << node.GetName() << " :" << port;
    if( !isReload )
        system_.MakeDirectory( GetPath( root_, node ) );
    proxy_.Register( GetPrefix( type_, node ), "localhost", port );
    if( isReload )
        Save( node );
    else
        Start( node, true );
}

// -----------------------------------------------------------------------------
// Name: NodeController::Create
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::T_Node NodeController::Create( const std::string& name )
{
    boost::shared_ptr< Node_ABC > node = factory_.Make( root_, name );
    nodes_->Attach( node );
    Create( *node, false );
    return node;
}

// -----------------------------------------------------------------------------
// Name: NodeController::Save
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
void NodeController::Save( const Node_ABC& node ) const
{
    const Path path = GetPath( root_, node ) / ( type_ + ".id" );
    pool_->Post( boost::bind( &FileSystem_ABC::WriteFile, &system_, path, ToJson( node.Save() ) ) );
}

// -----------------------------------------------------------------------------
// Name: NodeController::Delete
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::T_Node NodeController::Delete( const Uuid& id )
{
    boost::shared_ptr< Node_ABC > node = nodes_->Detach( id );
    if( !node )
        return node;
    LOG_INFO( log_ ) << "[" << type_ << "] Removed " << node->GetId() << " " << node->GetName() << " :" << node->GetPort();
    proxy_.Unregister( GetPrefix( type_, *node ) );
    Stop( *node, true );
    pool_->Post( boost::bind( &FileSystem_ABC::Remove, &system_, GetPath( root_, *node ) ) );
    return node;
}

// -----------------------------------------------------------------------------
// Name: NodeController::Start
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
boost::shared_ptr< runtime::Process_ABC > NodeController::StartWith( const Node_ABC& node ) const
{
    Path jar_path = jar_;
    return runtime_.Start( Utf8Convert( java_ ), boost::assign::list_of
        ( " " "-jar \"" + Utf8Convert( jar_.filename() ) + "\"" )
        ( MakeOption( "root",  Utf8Convert( web_ ) ) )
        ( MakeOption( "proxy", proxy_.GetPort() ) )
        ( MakeOption( "uuid", node.GetId() ) )
        ( MakeOption( "type", type_ ) )
        ( MakeOption( "name", node.GetName() ) )
        ( MakeOption( "port", node.GetPort() ) ),
        Utf8Convert( jar_path.remove_filename() ),
        Utf8Convert( GetPath( root_, node ) / ( type_ + ".log" ) ) );
}

// -----------------------------------------------------------------------------
// Name: NodeController::Start
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::T_Node NodeController::Start( const Uuid& id ) const
{
    boost::shared_ptr< Node_ABC > node = nodes_->Get( id );
    if( !node )
        return T_Node();
    Start( *node, false );
    return node;
}

// -----------------------------------------------------------------------------
// Name: NodeController::Stop
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::T_Node NodeController::Stop( const Uuid& id ) const
{
    boost::shared_ptr< Node_ABC > node = nodes_->Get( id );
    if( !node )
        return T_Node();
    Stop( *node, false );
    return node;
}

// -----------------------------------------------------------------------------
// Name: NodeController::Start
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
void NodeController::Start( Node_ABC& node, bool mustSave ) const
{
    bool valid = node.Start( boost::bind( &NodeController::StartWith, this, _1 ) );
    if( valid || mustSave )
        Save( node );
}

// -----------------------------------------------------------------------------
// Name: NodeController::Stop
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
void NodeController::Stop( Node_ABC& node, bool skipSave ) const
{
    bool valid = node.Stop();
    if( valid && !skipSave )
        Save( node );
}

// -----------------------------------------------------------------------------
// Name: NodeController::GetStash
// Created: BAX 2012-05-11
// -----------------------------------------------------------------------------
Tree NodeController::GetPack( const Uuid& id ) const
{
    boost::shared_ptr< Node_ABC > node = nodes_->Get( id );
    return node ? node->GetPack() : Tree();
}

// -----------------------------------------------------------------------------
// Name: NodeController::UploadPack
// Created: BAX 2012-05-11
// -----------------------------------------------------------------------------
Tree NodeController::UploadPack( const Uuid& id, std::istream& src ) const
{
    boost::shared_ptr< Node_ABC > node = nodes_->Get( id );
    if( !node )
        return Tree();
    try
    {
        node->ReadPack( src );
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << "[" << type_ << "] " << " Unable to unpack - " << err.what();
    }
    return node->GetPack();
}

// -----------------------------------------------------------------------------
// Name: NodeController::DeletePack
// Created: BAX 2012-05-23
// -----------------------------------------------------------------------------
Tree NodeController::DeletePack( const Uuid& id )
{
    boost::shared_ptr< Node_ABC > node = nodes_->Get( id );
    return node ? node->DeletePack() : Tree();
}

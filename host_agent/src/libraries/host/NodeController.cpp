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
#include "Json.h"
#include "Node.h"
#include "Pool_ABC.h"
#include "PortFactory_ABC.h"
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

boost::filesystem::path GetPath( const boost::filesystem::path& path, const Node& node )
{
    boost::filesystem::path copy = path;
    return copy.remove_filename() / boost::lexical_cast< std::wstring >( node.id_ );
}

std::string GetPrefix( const std::string& type, const Node& node )
{
    return type == "cluster" ? type : boost::lexical_cast< std::string >( node.id_ );
}
}

// -----------------------------------------------------------------------------
// Name: NodeController::NodeController
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::NodeController( cpplog::BaseLogger& log,
                                const runtime::Runtime_ABC& runtime,
                                const FileSystem_ABC& system,
                                const UuidFactory_ABC& uuids,
                                const Proxy_ABC& proxy,
                                const boost::filesystem::path& java,
                                const boost::filesystem::path& jar,
                                const boost::filesystem::path& web,
                                const std::string& type,
                                Pool_ABC& pool,
                                PortFactory_ABC& ports )
    : log_    ( log )
    , runtime_( runtime )
    , system_ ( system )
    , uuids_  ( uuids )
    , proxy_  ( proxy )
    , java_   ( java )
    , jar_    ( jar )
    , web_    ( web )
    , type_   ( type )
    , pool_   ( new SecurePool( log, type, pool ) )
    , ports_  ( ports )
    , nodes_  ( new Container< Node >() )
{
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
    boost::filesystem::path dir = jar_;
    const std::wstring wtype = Utf8Convert( type_ );
    BOOST_FOREACH( const boost::filesystem::path& path, system_.Glob( dir.remove_filename(), wtype + L".id" ) )
        try
        {
            boost::shared_ptr< Node > ptr = boost::make_shared< Node >( FromJson( system_.ReadFile( path ) ), runtime_, ports_ );
            if( ptr )
                nodes_->Attach( ptr );
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
    return nodes_->List< Node_ABC >( boost::function< bool( const Node& ) >(), offset, limit );
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
bool NodeController::Has( const boost::uuids::uuid& id ) const
{
    return nodes_->Has( id );
}

// -----------------------------------------------------------------------------
// Name: NodeController::Get
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::T_Node NodeController::Get( const boost::uuids::uuid& id ) const
{
    return nodes_->Get( id );
}

// -----------------------------------------------------------------------------
// Name: NodeController::Create
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::T_Node NodeController::Create( const std::string& name )
{
    std::auto_ptr< Port_ABC > ptrPort = ports_.Create();
    const int port = ptrPort->Get();
    boost::shared_ptr< Node > node = boost::make_shared< Node >( uuids_.Create(), name, ptrPort );
    bool valid = nodes_->Attach( node );
    if( !valid )
        return T_Node();
    LOG_INFO( log_ ) << "[" << type_ << "] Added " << type_ << " " << node->id_ << " " << node->name_;
    proxy_.Register( GetPrefix( type_, *node ), "localhost", port );
    Start( *node, true );
    return node;
}

// -----------------------------------------------------------------------------
// Name: NodeController::Save
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
void NodeController::Save( const Node& node ) const
{
    const boost::filesystem::path path = GetPath( jar_, node ) / ( Utf8Convert( type_ ) + L".id" );
    pool_->Post( boost::bind( &FileSystem_ABC::WriteFile, &system_, path, ToJson( node.Save() ) ) );
}

// -----------------------------------------------------------------------------
// Name: NodeController::Delete
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::T_Node NodeController::Delete( const boost::uuids::uuid& id )
{
    boost::shared_ptr< Node > node = nodes_->Detach( id );
    if( !node )
        return node;
    LOG_INFO( log_ ) << "[" << type_ << "] Removed " << type_ << " " << node->id_ << " " << node->name_;
    proxy_.Unregister( GetPrefix( type_, *node ) );
    Stop( *node, true );
    pool_->Post( boost::bind( &FileSystem_ABC::Remove, &system_, GetPath( jar_, *node ) ) );
    return node;
}

// -----------------------------------------------------------------------------
// Name: NodeController::Start
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
boost::shared_ptr< runtime::Process_ABC > NodeController::StartWith( const Node& node ) const
{
    boost::filesystem::path jar_path = jar_;
    return runtime_.Start( Utf8Convert( java_ ), boost::assign::list_of
        ( " " "-jar \"" + Utf8Convert( jar_.filename() ) + "\"" )
        ( MakeOption( "root",  Utf8Convert( web_ ) ) )
        ( MakeOption( "proxy", proxy_.GetPort() ) )
        ( MakeOption( "uuid", node.GetId() ) )
        ( MakeOption( "type", type_ ) )
        ( MakeOption( "name", node.name_ ) )
        ( MakeOption( "port", node.port_->Get() ) ),
        Utf8Convert( jar_path.remove_filename() ) );
}

// -----------------------------------------------------------------------------
// Name: NodeController::Start
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::T_Node NodeController::Start( const boost::uuids::uuid& id ) const
{
    boost::shared_ptr< Node > node = nodes_->Get( id );
    if( !node )
        return T_Node();
    Start( *node, false );
    return node;
}

// -----------------------------------------------------------------------------
// Name: NodeController::Stop
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::T_Node NodeController::Stop( const boost::uuids::uuid& id ) const
{
    boost::shared_ptr< Node > node = nodes_->Get( id );
    if( !node )
        return T_Node();
    Stop( *node, false );
    return node;
}

// -----------------------------------------------------------------------------
// Name: NodeController::Start
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
void NodeController::Start( Node& node, bool mustSave ) const
{
    bool valid = node.Start( boost::bind( &NodeController::StartWith, this, _1 ) );
    if( valid || mustSave )
        Save( node );
}

// -----------------------------------------------------------------------------
// Name: NodeController::Stop
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
void NodeController::Stop( Node& node, bool skipSave ) const
{
    bool valid = node.Stop();
    if( valid && !skipSave )
        Save( node );
}


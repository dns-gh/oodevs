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
#include "Package.h"
#include "Proxy_ABC.h"
#include "runtime/FileSystem_ABC.h"
#include "runtime/PropertyTree.h"
#include "runtime/Utf8.h"
#include "web/Configs.h"
#include "web/HttpException.h"
#include "web/Plugins.h"

#include <tools/win32/FlexLmLicense.h>
#include <tools/win32/FlexLm.h>

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/date_time/posix_time/time_parsers.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/filesystem/operations.hpp>

using namespace host;
using namespace property_tree;
using runtime::Utf8;
using runtime::Async;
using runtime::FileSystem_ABC;
using runtime::Pool_ABC;

// -----------------------------------------------------------------------------
// Name: NodeController::NodeController
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
NodeController::NodeController( cpplog::BaseLogger& log,
                                const runtime::Runtime_ABC& runtime,
                                const FileSystem_ABC& fs,
                                const web::Plugins& plugins,
                                const NodeFactory_ABC& nodes,
                                const Path& root,
                                const Path& app,
                                const Path& web,
                                const Path& client,
                                const Path& licenses,
                                const std::string& type,
                                int host,
                                int tcp,
                                Pool_ABC& pool,
                                Proxy_ABC& proxy )
    : log_        ( log )
    , runtime_    ( runtime )
    , fs_         ( fs )
    , plugins_    ( plugins )
    , factory_    ( nodes )
    , root_       ( root / ( type == "cluster" ? type : "nodes" ) )
    , app_        ( app )
    , web_        ( web )
    , licensesDir_( licenses )
    , type_       ( type )
    , host_       ( host )
    , tcp_        ( tcp )
    , proxy_      ( proxy )
    , client_     ()
    , async_      ( pool )
{
    fs.MakePaths( root_ );
    if( !fs_.IsFile( app_ ) )
        throw std::runtime_error( "'" + runtime::Utf8( app_ ) + "' is not a binary" );
    if( !fs_.IsDirectory( web_ ) )
        throw std::runtime_error( "'" + runtime::Utf8( web_ ) + "' is not a directory" );
    if( !client.empty() )
    {
        if( !fs_.IsDirectory( client ) )
            throw std::runtime_error( "'" + runtime::Utf8( client ) + "' is not a directory" );
        client_ = boost::make_shared< Package >( pool, fs, client, true, true );
        client_->Parse();
    }
    if( !fs_.IsDirectory( licensesDir_ ) )
        throw std::runtime_error( "'" + runtime::Utf8( licensesDir_ ) + "' is not a directory" );
    timer_ = runtime::MakeTimer( pool, boost::posix_time::seconds( 5 ), boost::bind( &NodeController::Refresh, this ) );
    GetAvailableLicences();
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
    const std::vector< std::string > licenses = boost::assign::list_of< std::string >
        ( "sword" )
        ( "sword-runtime" )
        ( "sword-dispatcher" )
        ( "sword-replayer" );
}

// -----------------------------------------------------------------------------
// Name: NodeController::GetAvailableLicences
// Created: NPT 2013-06-24
// -----------------------------------------------------------------------------
void NodeController::GetAvailableLicences()
{
    Tree tree;
    for( auto it = ::licenses.begin(); it != ::licenses.end(); ++it )
    {
        Tree subTree;
        try
        {
            FlexLmLicense license( *it );
            subTree.put( "date", license.GetExpirationDate() );
            subTree.put( "validity", "valid" );
#if !defined(_DEBUG)
            if( *it == "sword-dispatcher" )
                subTree.put( "connections", license.GetAuthorisedUsers() );
#endif
        }
        catch( const FlexLmLicense::LicenseError& )
        {
            subTree.put( "date", "" );
            subTree.put( "validity", "none" );
        }
        tree.add_child( *it, subTree );
    }
    licenses_ = tree;
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
        LOG_WARN( log_ ) << "[" << type_ << "] Unable to reload " << Utf8( path );
    }
}

// -----------------------------------------------------------------------------
// Name: NodeController::ReloadDirectory
// Created: BAX 2012-08-10
// -----------------------------------------------------------------------------
bool NodeController::ReloadDirectory( runtime::Async& reload, const Path& dir )
{
    const Path path = dir / ( type_ + ".id" );
    if( fs_.IsFile( path ) )
        reload.Post( boost::bind( &NodeController::ReloadNode, this, path ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NodeController::Reload
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
void NodeController::Reload()
{
    Async reload( async_.GetPool() );
    fs_.Walk( root_, false, boost::bind( &NodeController::ReloadDirectory, this, boost::ref( reload ), _1 ) );
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
        fs_.MakePaths( node.GetRoot() );
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

    const Path output = fs_.MakeAnyPath( root_ );
    T_Node node = factory_.Make( output, ident, cfg );
    bool valid = nodes_.AttachUnless( node, boost::bind( &HasSameIdent, ident, _1 ) );
    if( !valid )
    {
        Post( async_, boost::bind( &FileSystem_ABC::Remove, &fs_, output ) );
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
    Post( async_, boost::bind( &FileSystem_ABC::WriteFile, &fs_, path, ToJson( node.Save() ) ) );
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
    bool modified = node.Start( app_, web_, type_, host_, tcp_, weak );
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

namespace
{
    std::string GetClient( bool x64 )
    {
        return x64 ? "client64" : "client32";
    }
}

// -----------------------------------------------------------------------------
// Name: NodeController::GetClient
// Created: BAX 2012-10-02
// -----------------------------------------------------------------------------
Tree NodeController::GetClient( bool x64 ) const
{
    if( !client_ )
        throw web::HttpException( web::NOT_FOUND );
    const auto target = Package::MakeDependency( ::GetClient( x64 ), "gaming" );
    const auto item = client_->FindAlive( *target );
    if( !item )
        throw web::HttpException( web::NOT_FOUND );
    return client_->GetPropertiesFrom( *item );
}

// -----------------------------------------------------------------------------
// Name: NodeController::DownloadClient
// Created: BAX 2012-10-02
// -----------------------------------------------------------------------------
void NodeController::DownloadClient( web::Chunker_ABC& dst, bool x64 ) const
{
    if( !client_ )
        throw web::HttpException( web::NOT_FOUND );
    const auto target = Package::MakeDependency( ::GetClient( x64 ), "gaming" );
    const auto item = client_->FindAlive( *target );
    if( !item )
        throw web::HttpException( web::NOT_FOUND );
    try
    {
        client_->Download( dst, *item );
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << "[" << type_ << "] " << err.what();
        LOG_ERROR( log_ ) << "[" << type_ << "] Unable to download client";
    }
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
// Name: NodeController::DownloadInstall
// Created: BAX 2012-09-11
// -----------------------------------------------------------------------------
void NodeController::DownloadInstall( const Uuid& id, web::Chunker_ABC& dst, size_t item )
{
    T_Node node = nodes_.Get( id );
    if( !node )
        throw web::HttpException( web::NOT_FOUND );
    try
    {
        node->DownloadInstall( dst, item );
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << "[" << type_ << "] " << err.what();
        LOG_ERROR( log_ ) << "[" << type_ << "] Unable to download item " << item;
    }
}

// -----------------------------------------------------------------------------
// Name: NodeController::DownloadInstall
// Created: BAX 2012-09-18
// -----------------------------------------------------------------------------
void NodeController::DownloadInstall( const Uuid& id, web::Chunker_ABC& dst,
                                      const std::string& type, const std::string& name,
                                      const std::string& checksum )
{
    T_Node node = nodes_.Get( id );
    if( !node )
        throw web::HttpException( web::NOT_FOUND );
    try
    {
        node->DownloadInstall( dst, type, name, checksum );
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << "[" << type_ << "] " << err.what();
        LOG_ERROR( log_ ) << "[" << type_ << "] Unable to download item "
                          << type << " " << name << " 0x" << checksum;
    }
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
Tree NodeController::UploadCache( const Uuid& id, io::Reader_ABC& src ) const
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

namespace
{
void AppendClient( Tree& dst, const std::string& name, const Tree& data )
{
    dst.put( name + ".name",     Get< std::string >( data, "name" ) );
    dst.put( name + ".checksum", Get< std::string >( data, "checksum" ) );
}

Tree AppendClients( Tree dst, const Tree& bin32, const Tree& bin64 )
{
    AppendClient( dst, "client32", bin32 );
    AppendClient( dst, "client64", bin64 );
    return dst;
}
}

// -----------------------------------------------------------------------------
// Name: NodeController::LinkExercise
// Created: BAX 2012-10-02
// -----------------------------------------------------------------------------
Tree NodeController::LinkExercise( const Node_ABC& node, const std::string& name ) const
{
    return AppendClients( node.LinkExercise( name ), GetClient( false ), GetClient( true ) );
}

// -----------------------------------------------------------------------------
// Name: NodeController::LinkExercise
// Created: BAX 2012-10-02
// -----------------------------------------------------------------------------
Tree NodeController::LinkExercise( const Node_ABC& node, const Tree& tree ) const
{
    return AppendClients( node.LinkExercise( tree ), GetClient( false ), GetClient( true ) );
}

// -----------------------------------------------------------------------------
// Name: UserController::ListLicenses
// Created: NPT 2013-06-24
// For the moment license are global and are not dependent of the nodes
// -----------------------------------------------------------------------------
Tree NodeController::ListLicenses( const Uuid& /*id*/ ) const
{
    return licenses_;
}

// -----------------------------------------------------------------------------
// Name: NodeController::UploadLicenses
// Created: NPT 2013-06-28
// -----------------------------------------------------------------------------
Tree NodeController::UploadLicenses( io::Reader_ABC& src )
{
    boost::system::error_code ec;
    const Path output = boost::filesystem::unique_path( Path( licensesDir_ ) / "%%%%%%%%.lic", ec );
    if( ec )
        return Tree();
    std::string content = fs_.ReadAll( src );
    std::vector< std::string > features;
    static const std::string feature = "FEATURE";
    size_t start = content.find( feature );
    while( start != content.npos )
    {
        size_t end = content.find( feature, start + feature.size() );
        features.push_back( content.substr( start, end == content.npos ? content.npos : end - start ) );
        start = end;
    }
    fs_.WriteFile( output, content );
    bool modified = false;
    for( auto it = features.begin(); it != features.end(); ++it )
        for( auto license = ::licenses.begin(); license != ::licenses.end(); ++license )
            if( it->find( " " + *license + " " ) != it->npos )
                try
                {
                    FlexLmLicense flex( *license );
                    fs_.WriteFile( licensesDir_ / ( *license + ".lic" ), *it );
                    modified = true;
                }
                catch( const FlexLmLicense::LicenseError& err )
                {
                    LOG_WARN( log_ ) << "[" << type_ << "] " << err.what();
                }
    fs_.Remove( output );
    if( !modified )
        throw web::HttpException( web::BAD_REQUEST );
    GetAvailableLicences();
    return licenses_;
}

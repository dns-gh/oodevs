// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Node.h"
#include "FileSystem_ABC.h"
#include "PortFactory_ABC.h"
#include "Proxy_ABC.h"
#include "SecurePool.h"
#include "UuidFactory_ABC.h"
#include "cpplog/cpplog.hpp"
#include "runtime/Process_ABC.h"
#include "runtime/Runtime_ABC.h"
#include "runtime/Utf8.h"

#include <xeumeuleu/xml.hpp>

#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4244 )
#endif
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

using namespace host;

namespace
{
    const int MAX_KILL_TIMEOUT_MS = 3*1000;

    // -----------------------------------------------------------------------------
    // Name: Utf8Convert
    // Created: BAX 2012-04-03
    // -----------------------------------------------------------------------------
    std::string Utf8Convert( const boost::filesystem::path& path )
    {
        return runtime::Utf8Convert( path.wstring() );
    }

    template< typename T >
    T ParseItem( xml::xisubstream xis, const std::string& name )
    {
        xis >> xml::start( "root" );
        return xis.attribute< T >( name );
    }

    boost::shared_ptr< runtime::Process_ABC > GetProcess( const runtime::Runtime_ABC& runtime, xml::xisubstream xis )
    {
        boost::shared_ptr< runtime::Process_ABC > nil;
        xis >> xml::start( "root" );
        if( !xis.has_attribute( "process_pid" ) || !xis.has_attribute( "process_name" ) )
            return nil;
        boost::shared_ptr< runtime::Process_ABC > ptr = runtime.GetProcess( xis.attribute< int >( "process_pid" ) );
        if( !ptr || ptr->GetName() != xis.attribute< std::string >( "process_name" ) )
            return nil;
        return ptr;
    }
}

// -----------------------------------------------------------------------------
// Name: Node::Node
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Node::Node( cpplog::BaseLogger& log, Pool_ABC& pool,
            const runtime::Runtime_ABC& runtime, const UuidFactory_ABC& uuids, const FileSystem_ABC& system,
            const Proxy_ABC& proxy, const boost::filesystem::path& java, const boost::filesystem::path& jar,
            const boost::filesystem::path& web, const std::string& type, const std::string& name, PortFactory_ABC& ports )
    : log_    ( log )
    , runtime_( runtime )
    , system_ ( system )
    , proxy_  ( proxy )
    , java_   ( java )
    , jar_    ( jar )
    , web_    ( web )
    , type_   ( type )
    , id_     ( uuids.Create() )
    , name_   ( name )
    , pool_   ( new SecurePool( log, "node", pool ) )
    , access_ ( new boost::shared_mutex() )
    , port_   ( ports.Create() )
{
    CheckPaths();
    LOG_INFO( log_ ) << "[" + type_ + "] + " << id_ << " " << name_;
    proxy.Register( type_ == "cluster" ? type_ : boost::lexical_cast< std::string >( id_ ), "localhost", port_->Get() );
    Save();
}

// -----------------------------------------------------------------------------
// Name: Node::Node
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Node::Node( cpplog::BaseLogger& log, Pool_ABC& pool,
            const runtime::Runtime_ABC& runtime, const FileSystem_ABC& system,
            const Proxy_ABC& proxy, const boost::filesystem::path& java, const boost::filesystem::path& jar,
            const boost::filesystem::path& web, xml::xistream& xis, PortFactory_ABC& ports )
    : log_    ( log )
    , runtime_( runtime )
    , system_ ( system )
    , proxy_  ( proxy )
    , java_   ( java )
    , jar_    ( jar )
    , web_    ( web )
    , type_   ( ParseItem< std::string >( xis, "type" ) )
    , id_     ( boost::uuids::string_generator()( ParseItem< std::string >( xis, "id" ) ) )
    , name_   ( ParseItem< std::string >( xis, "name" ) )
    , pool_   ( new SecurePool( log, "node", pool ) )
    , access_ ( new boost::shared_mutex() )
    , process_( GetProcess( runtime_, xis ) )
    , port_   ( ports.Create( ParseItem< int >( xis, "port" ) ) )
{
    CheckPaths();
    LOG_INFO( log_ ) << "[" + type_ + "] + " << id_ << " " << name_;
    if( !process_ )
        LOG_WARN( log_ ) << "[" + type_ + "] " << name_ << " Unable to reload process";
    proxy.Register( type_ == "cluster" ? type_ : boost::lexical_cast< std::string >( id_ ), "localhost", port_->Get() );
    Save();
}

// -----------------------------------------------------------------------------
// Name: Node::CheckPaths
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
void Node::CheckPaths() const
{
    if( !system_.IsDirectory( web_ ) )
        throw std::runtime_error( Utf8Convert( web_ ) + " is not a directory" );
    if( !system_.Exists( jar_ ) )
        throw std::runtime_error( Utf8Convert( jar_ ) + " is missing" );
    if( !system_.IsFile( jar_ ) )
        throw std::runtime_error( Utf8Convert( jar_ ) + " is not a file" );
}

// -----------------------------------------------------------------------------
// Name: Node::~Node
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Node::~Node()
{
    if( process_ )
        process_->Kill( MAX_KILL_TIMEOUT_MS );
    system_.Remove( GetPath() );
    proxy_.Unregister( boost::lexical_cast< std::string >( id_ ) );
    LOG_INFO( log_ ) << "[" + type_ + "] - " << id_ << " " << name_;
}

// -----------------------------------------------------------------------------
// Name: Node::GetTag
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
boost::uuids::uuid Node::GetTag() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Node::Save
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
void Node::Save() const
{
    const boost::filesystem::path path = GetPath();
    system_.CreateDirectory( path );
    pool_->Post( boost::bind( &FileSystem_ABC::WriteFile, &system_, path / runtime::Utf8Convert( type_ + ".id" ), ToXml() ) );
}

// -----------------------------------------------------------------------------
// Name: Node::ToJson
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
std::string Node::ToJson() const
{
    boost::shared_lock< boost::shared_mutex > lock( *access_ );
    return (boost::format( "{ "
        "\"id\" : \"%1%\", "
        "\"type\" : \"%2%\", "
        "\"name\" : \"%3%\", "
        "\"port\" : %4%, "
        "\"status\" : \"%5%\""
        " }" )
        % id_ % type_ % name_ % port_->Get() % (process_ ? "running" : "stopped")
        ).str();
}

// -----------------------------------------------------------------------------
// Name: Node::ToXml
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
std::string Node::ToXml() const
{
    xml::xostringstream xos;
    xos << xml::start( "root" )
            << xml::attribute( "id", boost::lexical_cast< std::string >( id_ ) )
            << xml::attribute( "type", type_ )
            << xml::attribute( "name", name_ )
            << xml::attribute( "port", port_->Get() );
    if( process_ )
        xos << xml::attribute( "process_pid", process_->GetPid() )
            << xml::attribute( "process_name", process_->GetName() );
    return xos.str();
}

// -----------------------------------------------------------------------------
// Name: Node::GetPath
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
boost::filesystem::path Node::GetPath() const
{
    boost::filesystem::path path = jar_;
    return path.remove_filename() / boost::lexical_cast< std::wstring >( id_ );
}

// -----------------------------------------------------------------------------
// Name: Node::Start
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
void Node::Start()
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    if( process_ ) return;
    const boost::filesystem::path path = GetPath();
    boost::filesystem::path jar_path = jar_;
    process_ = runtime_.Start( Utf8Convert( java_ ), boost::assign::list_of
            ( " " "-jar \""  + Utf8Convert( jar_.filename() ) + "\"" )
            ( "--root \""  + Utf8Convert( web_ ) + "\"" )
            ( "--port \"" + boost::lexical_cast< std::string >( port_->Get() ) + "\"" )
            ( "--proxy \"" + boost::lexical_cast< std::string >( proxy_.GetPort() ) + "\"" )
            ( "--uuid \"" + boost::lexical_cast< std::string >( id_ ) + "\"" )
            ( "--name \"" + name_ + "\"" )
            ( "--type \"" + type_ + "\"" ),
            Utf8Convert( jar_path.remove_filename() ) );
    if( !process_ ) return;

    Save();
}

// -----------------------------------------------------------------------------
// Name: Node::Stop
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
void Node::Stop()
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    if( process_ )
        process_->Kill( MAX_KILL_TIMEOUT_MS );
    process_.reset();
    Save();
}

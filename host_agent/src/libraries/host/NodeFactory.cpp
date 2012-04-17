// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "NodeFactory.h"
#include "PortFactory_ABC.h"
#include "FileSystem_ABC.h"
#include "Node.h"

#include "cpplog/cpplog.hpp"
#include "runtime/Utf8.h"

#include <xeumeuleu/xml.hpp>

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/ref.hpp>

using namespace host;

// -----------------------------------------------------------------------------
// Name: NodeFactory::NodeFactory
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
NodeFactory::NodeFactory( cpplog::BaseLogger& log, const runtime::Runtime_ABC& runtime, const UuidFactory_ABC& uuids,
                          const FileSystem_ABC& system, const Proxy_ABC& proxy, PortFactory_ABC& ports, Pool_ABC& pool,
                          const boost::filesystem::path& java, const boost::filesystem::path& jar, const boost::filesystem::path& web )
    : log_    ( log )
    , runtime_( runtime )
    , uuids_  ( uuids )
    , system_ ( system )
    , proxy_  ( proxy )
    , java_   ( java )
    , jar_    ( jar )
    , web_    ( web )
    , pool_   ( pool )
    , ports_  ( ports )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NodeFactory::~NodeFactory
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
NodeFactory::~NodeFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NodeFactory::Create
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
boost::shared_ptr< Node_ABC > NodeFactory::Create( const std::string& type, const std::string& name ) const
{
    return boost::shared_ptr< Node >( new Node( log_, pool_, runtime_, uuids_, system_, proxy_, java_, jar_, web_, type, name, ports_ ) );
}

// -----------------------------------------------------------------------------
// Name: NodeFactory::Reload
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
NodeFactory_ABC::T_Nodes NodeFactory::Reload( const std::string& type ) const
{
    NodeFactory_ABC::T_Nodes reply;
    boost::filesystem::path dir = jar_;
    const std::wstring wtype = runtime::Utf8Convert( type );
    BOOST_FOREACH( const boost::filesystem::path& path, system_.Glob( dir.remove_filename(), wtype + L".id" ) )
        try
        {
            xml::xistringstream xis( system_.ReadFile( path ) );
            boost::shared_ptr< Node_ABC > ptr( new Node( log_, pool_, runtime_, system_, proxy_, java_, jar_, web_, xis, ports_ ) );
            reply.insert( std::make_pair( ptr->GetTag(), ptr ) );
        }
        catch( const std::exception& err )
        {
            LOG_WARN( log_ ) << "[" + type + "] Unable to reload " << path.string() << " - " << err.what();
            continue; // skip invalid session
        }
    return reply;
}

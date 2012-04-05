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
                          const FileSystem_ABC& system, PortFactory_ABC& ports, const boost::filesystem::wpath& java,
                          const boost::filesystem::wpath& jar, const boost::filesystem::wpath& web, int host )
    : log_    ( log )
    , runtime_( runtime )
    , uuids_  ( uuids )
    , system_ ( system )
    , java_   ( java )
    , jar_    ( jar )
    , web_    ( web )
    , host_   ( host )
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
boost::shared_ptr< Node_ABC > NodeFactory::Create( const std::string& name ) const
{
    return boost::shared_ptr< Node >( new Node( log_, runtime_, uuids_, system_, java_, jar_, web_, host_, name, ports_ ) );
}

// -----------------------------------------------------------------------------
// Name: NodeFactory::Reload
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
NodeFactory_ABC::T_Nodes NodeFactory::Reload() const
{
    NodeFactory_ABC::T_Nodes reply;
    boost::filesystem::wpath dir = jar_;
    BOOST_FOREACH( const boost::filesystem::wpath& path, system_.Glob( dir.remove_filename() , L"node.id" ) )
        try
        {
            xml::xistringstream xis( system_.ReadFile( path ) );
            boost::shared_ptr< Node_ABC > ptr = boost::make_shared< Node >( boost::ref( log_ ), runtime_, system_, java_, jar_, web_, boost::ref( xis ), boost::ref( ports_ ) );
            reply.insert( std::make_pair( ptr->GetTag(), ptr ) );
        }
        catch( const std::exception& err )
        {
            LOG_WARN( log_ ) << "[node] Unable to reload node from " << runtime::Utf8Convert( path.string() ) << " - " << err.what();
            continue; // skip invalid session
        }
    return reply;
}

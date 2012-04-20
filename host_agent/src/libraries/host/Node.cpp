// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Node.h"

#include "PortFactory_ABC.h"
#include "runtime/Process_ABC.h"
#include "runtime/Runtime_ABC.h"

#include <boost/assign/list_of.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/uuid/string_generator.hpp>
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

Node::T_Process GetProcess( const boost::property_tree::ptree& tree, const runtime::Runtime_ABC& runtime )
{
    const boost::optional< int > pid = tree.get_optional< int >( "process.pid" );
    if( pid == boost::none )
        return Node::T_Process();
    return runtime.GetProcess( *pid );
}

Node::T_Process AcquireProcess( const boost::property_tree::ptree& tree, const runtime::Runtime_ABC& runtime, int expected )
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
Node::Node( const boost::uuids::uuid& id, const std::string& name, std::auto_ptr< Port_ABC > port )
    : id_     ( id )
    , name_   ( name )
    , port_   ( port )
    , access_ ( new boost::shared_mutex() )
    , process_()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Node::Node
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
Node::Node( const boost::property_tree::ptree& tree, const runtime::Runtime_ABC& runtime, PortFactory_ABC& ports )
    : id_     ( boost::uuids::string_generator()( tree.get< std::string >( "id" ) ) )
    , name_   ( tree.get< std::string >( "name" ) )
    , port_   ( AcquirePort( tree.get< int >( "port" ), ports ) )
    , access_ ( new boost::shared_mutex() )
    , process_( AcquireProcess( tree, runtime, port_->Get() ) )
{
    // NOTHING
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
boost::uuids::uuid Node::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Node::GetCommonProperties
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
boost::property_tree::ptree Node::GetCommonProperties() const
{
    boost::property_tree::ptree tree;
    tree.put( "id", id_ );
    tree.put( "name", name_ );
    tree.put( "port", port_->Get() );
    return tree;
}

// -----------------------------------------------------------------------------
// Name: Node::GetProperties
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
boost::property_tree::ptree Node::GetProperties() const
{
    boost::property_tree::ptree tree = GetCommonProperties();
    tree.put( "status", process_ ? "running" : "stopped" );
    return tree;
}

// -----------------------------------------------------------------------------
// Name: Node::Save
// Created: BAX 2012-04-17
// -----------------------------------------------------------------------------
boost::property_tree::ptree Node::Save() const
{
    boost::property_tree::ptree tree = GetCommonProperties();
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
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

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Agent.h"

#include "cpplog/cpplog.hpp"
#include "Json.h"
#include "Node_ABC.h"
#include "NodeController_ABC.h"
#include "Session_ABC.h"
#include "SessionController_ABC.h"

#include <boost/algorithm/string/join.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/uuid/uuid_io.hpp>

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4244 )
#endif
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

using namespace host;

namespace
{
#define CALL_MEMBER( obj, ptr ) ( ( obj ).*( ptr ) )

template< typename T >
T Clip( T value, T min, T max )
{
    assert( min <= max );
    return std::min( std::max( value, min ), max );
}

bool HasKnownNode( const Session_ABC& session, const NodeController_ABC& nodes )
{
    return nodes.Has( session.GetNode() );
}

bool IsNode( const Session_ABC& session, const boost::uuids::uuid& id )
{
    return session.GetNode() == id;
}

template< typename T >
Reply List( const T& list )
{
    std::string json;
    BOOST_FOREACH( const T::value_type& value, list )
        json += ToJson( value->GetProperties() ) + ",";
    return Reply( "[" + json.substr( 0, json.size() - 1 ) + "]" );
}

Reply Count( size_t count )
{
    boost::property_tree::ptree tree;
    tree.put( "count", count );
    return Reply( ToJson( tree ) );
}

template< typename T >
Reply Create( T ptr, const std::string& name )
{
    if( !ptr )
        return Reply( "unable to create new " + name, false );
    return Reply( ToJson( ptr->GetProperties() ) );
}

template< typename T, typename U >
Reply Dispatch( T& controller, const U& member, const boost::uuids::uuid& id, const std::string& action )
{
    boost::shared_ptr< typename T::T_Base > ptr = CALL_MEMBER( controller, member )( id );
    if( !ptr )
        return Reply( "unable to " + action + " " + boost::lexical_cast< std::string >( id ), false );
    return Reply( ToJson( ptr->GetProperties() ) );
}
}

// -----------------------------------------------------------------------------
// Name: Agent::Agent
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Agent::Agent( cpplog::BaseLogger& log, NodeController_ABC& nodes, SessionController_ABC& sessions )
    : log_     ( log )
    , nodes_   ( nodes )
    , sessions_( sessions )
    , access_  ( new boost::mutex() )
{
    nodes_.Reload();
    sessions_.Reload( boost::bind( &HasKnownNode, _1, boost::cref( nodes_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::~Agent
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent::ListNodes
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Reply Agent::ListNodes( int offset, int limit ) const
{
    return List( nodes_.List( offset, limit ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::CountNodes
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Reply Agent::CountNodes() const
{
    return Count( nodes_.Count() );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Reply Agent::GetNode( const boost::uuids::uuid& id ) const
{
    return Dispatch( nodes_, &NodeController_ABC::Get, id, "find node" );
}

// -----------------------------------------------------------------------------
// Name: Agent::CreateNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Reply Agent::CreateNode( const std::string& name )
{
    boost::lock_guard< boost::mutex > lock( *access_ );
    return Create( nodes_.Create( name ), "node" );
}

// -----------------------------------------------------------------------------
// Name: Agent::DeleteNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Reply Agent::DeleteNode( const boost::uuids::uuid& id )
{
    NodeController_ABC::T_Node ptr;
    SessionController_ABC::T_Sessions invalid;
    {
        boost::lock_guard< boost::mutex > lock( *access_ );
        ptr = nodes_.Delete( id );
        if( !ptr )
            return Reply( "unable to find node " + boost::lexical_cast< std::string >( id ), false );
        invalid = sessions_.List( boost::bind( &IsNode, _1, id ) );
    }
    // destroy objects outside the lock
    BOOST_FOREACH( SessionController_ABC::T_Session ptr, invalid )
        sessions_.Delete( ptr->GetId() );
    return Reply( ToJson( ptr->GetProperties() ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::StartNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Reply Agent::StartNode( const boost::uuids::uuid& id ) const
{
    return Dispatch( nodes_, &NodeController_ABC::Start, id, "start node" );
}

// -----------------------------------------------------------------------------
// Name: Agent::StopNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Reply Agent::StopNode( const boost::uuids::uuid& id ) const
{
    return Dispatch( nodes_, &NodeController_ABC::Stop, id, "stop node" );
}

// -----------------------------------------------------------------------------
// Name: Agent::ListSessions
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Reply Agent::ListSessions( const boost::uuids::uuid& node, int offset, int limit ) const
{
    SessionController_ABC::T_Predicate predicate;
    if( !node.is_nil() )
        predicate = boost::bind( &IsNode, _1, node );
    return List( sessions_.List( predicate, offset, limit ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::CountSessions
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Reply Agent::CountSessions( const boost::uuids::uuid& node ) const
{
    return Count( node.is_nil() ? sessions_.Count() : sessions_.Count( boost::bind( &IsNode, _1, node ) ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Reply Agent::GetSession( const boost::uuids::uuid& id ) const
{
    return Dispatch( sessions_, &SessionController_ABC::Get, id, "find session" );
}

// -----------------------------------------------------------------------------
// Name: Agent::CreateSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Reply Agent::CreateSession( const boost::uuids::uuid& node, const std::string& name, const std::string& exercise )
{
    boost::lock_guard< boost::mutex > lock( *access_ );
    if( !nodes_.Has( node ) )
        return Reply( "invalid node", false );
    return Create( sessions_.Create( node, name, exercise ), "session" );
}

// -----------------------------------------------------------------------------
// Name: Agent::DeleteSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Reply Agent::DeleteSession( const boost::uuids::uuid& id )
{
    return Dispatch( sessions_, &SessionController_ABC::Delete, id, "delete session" );
}

// -----------------------------------------------------------------------------
// Name: Agent::StartSession
// Created: BAX 2012-03-30
// -----------------------------------------------------------------------------
Reply Agent::StartSession( const boost::uuids::uuid& id ) const
{
    return Dispatch( sessions_, &SessionController_ABC::Start, id, "start session" );
}

// -----------------------------------------------------------------------------
// Name: Agent::StopSession
// Created: BAX 2012-03-30
// -----------------------------------------------------------------------------
Reply Agent::StopSession( const boost::uuids::uuid& id ) const
{
    return Dispatch( sessions_, &SessionController_ABC::Stop, id, "stop session" );
}

// -----------------------------------------------------------------------------
// Name: Agent::ListExercises
// Created: BAX 2012-03-27
// -----------------------------------------------------------------------------
Reply Agent::ListExercises( int offset, int limit ) const
{
    SessionController_ABC::T_Exercises exercises = sessions_.GetExercises();
    if( exercises.empty() )
        return Reply( "[]" );
    offset = Clip< int >( offset, 0, static_cast< int >( exercises.size() ) );
    if( offset > 0 )
        exercises.erase( exercises.begin(), exercises.begin() + offset );
    limit  = Clip< int >( limit,  0, static_cast< int >( exercises.size() ) );
    exercises.resize( limit );
    return Reply( "[\"" + boost::algorithm::join( exercises, "\", \"" ) + "\"]" );
}

// -----------------------------------------------------------------------------
// Name: Agent::CountExercises
// Created: BAX 2012-03-27
// -----------------------------------------------------------------------------
Reply Agent::CountExercises() const
{
    return Count( sessions_.GetExercises().size() );
}

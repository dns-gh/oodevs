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
#include "Node_ABC.h"
#include "NodeController_ABC.h"
#include "PropertyTree.h"
#include "Session_ABC.h"
#include "SessionController_ABC.h"

#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace host;

namespace
{
#define CALL_MEMBER( obj, ptr ) ( ( obj ).*( ptr ) )

Reply MakeReply( const Tree& tree )
{
    return Reply( ToJson( tree ) );
}

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

bool IsNode( const Session_ABC& session, const Uuid& id )
{
    return session.GetNode() == id;
}

template< typename T >
Reply List( const T& list )
{
    std::string json;
    BOOST_FOREACH( const typename T::value_type& value, list )
        json += ToJson( value->GetProperties() ) + ",";
    return Reply( "[" + json.substr( 0, json.size() - 1 ) + "]" );
}

Reply Count( size_t count )
{
    Tree tree;
    tree.put( "count", count );
    return MakeReply( tree );
}

template< typename T >
Reply Create( T ptr, const std::string& name )
{
    if( !ptr )
        return Reply( "unable to create new " + name, false );
    return MakeReply( ptr->GetProperties() );
}

template< typename T, typename U >
Reply Dispatch( T& controller, const U& member, const Uuid& id, const std::string& action )
{
    boost::shared_ptr< typename T::T_Base > ptr = CALL_MEMBER( controller, member )( id );
    if( !ptr )
        return Reply( "unable to " + action + " " + boost::lexical_cast< std::string >( id ), false );
    return MakeReply( ptr->GetProperties() );
}

template< typename T, typename U >
Reply ClusterDispatch( T* controller, const U& member, const Uuid& id, const std::string& action )
{
    if( !controller )
        return Reply( "Cluster not enabled", false );
    return Dispatch( *controller, member, id, action );
}
}

// -----------------------------------------------------------------------------
// Name: Agent::Agent
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Agent::Agent( cpplog::BaseLogger& log, NodeController_ABC* cluster, NodeController_ABC& nodes, SessionController_ABC& sessions )
    : log_     ( log )
    , cluster_ ( cluster )
    , nodes_   ( nodes )
    , sessions_( sessions )
{
    if( cluster_ )
    {
        cluster_->Reload();
        if( cluster_->Count() )
            clusterId_ = cluster_->List( 0, 1 ).front()->GetId();
        else
            clusterId_ = cluster_->Create( "cluster" )->GetId();
        cluster_->Start( clusterId_ );
    }
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
// Name: Agent::GetCluster
// Created: BAX 2012-04-23
// -----------------------------------------------------------------------------
Reply Agent::GetCluster() const
{
    return ClusterDispatch( cluster_, &NodeController_ABC::Get, clusterId_, "find cluster" );
}

// -----------------------------------------------------------------------------
// Name: Agent::StartCluster
// Created: BAX 2012-04-23
// -----------------------------------------------------------------------------
Reply Agent::StartCluster() const
{
    return ClusterDispatch( cluster_, &NodeController_ABC::Start, clusterId_, "start cluster" );
}

// -----------------------------------------------------------------------------
// Name: Agent::StopCluster
// Created: BAX 2012-04-23
// -----------------------------------------------------------------------------
Reply Agent::StopCluster() const
{
    return ClusterDispatch( cluster_, &NodeController_ABC::Stop, clusterId_, "start cluster" );
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
Reply Agent::GetNode( const Uuid& id ) const
{
    return Dispatch( nodes_, &NodeController_ABC::Get, id, "find node" );
}

// -----------------------------------------------------------------------------
// Name: Agent::CreateNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Reply Agent::CreateNode( const std::string& name )
{
    boost::lock_guard< boost::mutex > lock( access_ );
    return Create( nodes_.Create( name ), "node" );
}

// -----------------------------------------------------------------------------
// Name: Agent::DeleteNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Reply Agent::DeleteNode( const Uuid& id )
{
    NodeController_ABC::T_Node ptr;
    SessionController_ABC::T_Sessions invalid;
    {
        boost::lock_guard< boost::mutex > lock( access_ );
        ptr = nodes_.Delete( id );
        if( !ptr )
            return Reply( "unable to find node " + boost::lexical_cast< std::string >( id ), false );
        invalid = sessions_.List( boost::bind( &IsNode, _1, id ), 0, INT_MAX );
    }
    // destroy objects outside the lock
    BOOST_FOREACH( SessionController_ABC::T_Session ptr, invalid )
        sessions_.Delete( ptr->GetId() );
    return MakeReply( ptr->GetProperties() );
}

// -----------------------------------------------------------------------------
// Name: Agent::StartNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Reply Agent::StartNode( const Uuid& id ) const
{
    return Dispatch( nodes_, &NodeController_ABC::Start, id, "start node" );
}

// -----------------------------------------------------------------------------
// Name: Agent::StopNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Reply Agent::StopNode( const Uuid& id ) const
{
    return Dispatch( nodes_, &NodeController_ABC::Stop, id, "stop node" );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetInstall
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
Reply Agent::GetInstall( const Uuid& id ) const
{
    return MakeReply( nodes_.GetInstall( id ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::DeleteInstall
// Created: BAX 2012-05-25
// -----------------------------------------------------------------------------
Reply Agent::DeleteInstall( const Uuid& id, const std::vector< size_t >& list )
{
    return MakeReply( nodes_.DeleteInstall( id, list ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::UploadCache
// Created: BAX 2012-05-11
// -----------------------------------------------------------------------------
Reply Agent::UploadCache( const Uuid& id, std::istream& src )
{
    return MakeReply( nodes_.UploadCache( id, src ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetCache
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
Reply Agent::GetCache( const Uuid& id ) const
{
    return MakeReply( nodes_.GetCache( id ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::DeleteCache
// Created: BAX 2012-05-22
// -----------------------------------------------------------------------------
Reply Agent::DeleteCache( const Uuid& id )
{
    return MakeReply( nodes_.DeleteCache( id ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::InstallFromCache
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
Reply Agent::InstallFromCache( const Uuid& id, const std::vector< size_t >& list )
{
    return MakeReply( nodes_.InstallFromCache( id, list ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::ListSessions
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Reply Agent::ListSessions( const Uuid& node, int offset, int limit ) const
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
Reply Agent::CountSessions( const Uuid& node ) const
{
    if( node.is_nil() )
        return Count( sessions_.Count( SessionController_ABC::T_Predicate() ) );
    else
        return Count( sessions_.Count( boost::bind( &IsNode, _1, node ) ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Reply Agent::GetSession( const Uuid& id ) const
{
    return Dispatch( sessions_, &SessionController_ABC::Get, id, "find session" );
}

// -----------------------------------------------------------------------------
// Name: Agent::CreateSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Reply Agent::CreateSession( const Uuid& node, const std::string& name, const std::string& exercise )
{
    boost::lock_guard< boost::mutex > lock( access_ );
    return Create( sessions_.Create( node, name, exercise ), "session" );
}

// -----------------------------------------------------------------------------
// Name: Agent::DeleteSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Reply Agent::DeleteSession( const Uuid& id )
{
    return Dispatch( sessions_, &SessionController_ABC::Delete, id, "delete session" );
}

// -----------------------------------------------------------------------------
// Name: Agent::StartSession
// Created: BAX 2012-03-30
// -----------------------------------------------------------------------------
Reply Agent::StartSession( const Uuid& id ) const
{
    return Dispatch( sessions_, &SessionController_ABC::Start, id, "start session" );
}

// -----------------------------------------------------------------------------
// Name: Agent::StopSession
// Created: BAX 2012-03-30
// -----------------------------------------------------------------------------
Reply Agent::StopSession( const Uuid& id ) const
{
    return Dispatch( sessions_, &SessionController_ABC::Stop, id, "stop session" );
}

// -----------------------------------------------------------------------------
// Name: Agent::PauseSession
// Created: BAX 2012-06-19
// -----------------------------------------------------------------------------
Reply Agent::PauseSession( const Uuid& id ) const
{
    return Dispatch( sessions_, &SessionController_ABC::Pause, id, "pause session" );
}

// -----------------------------------------------------------------------------
// Name: Agent::ListExercises
// Created: BAX 2012-03-27
// -----------------------------------------------------------------------------
Reply Agent::ListExercises( const Uuid& node, int offset, int limit ) const
{
    std::string json;
    BOOST_FOREACH( NodeController_ABC::T_Exercises::value_type item, nodes_.GetExercises( node, offset, limit ) )
    {
        std::replace( item.begin(), item.end(), '\\', '/' );
        json += "\"" + item + "\",";
    }
    return Reply( "[" + json.substr( 0, json.size()-1 ) + "]" );
}

// -----------------------------------------------------------------------------
// Name: Agent::CountExercises
// Created: BAX 2012-03-27
// -----------------------------------------------------------------------------
Reply Agent::CountExercises( const Uuid& node ) const
{
    return Count( nodes_.CountExercises( node ) );
}

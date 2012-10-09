// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "FormationProxy.h"
#include "LocalAgentResolver_ABC.h"
#include "ChildListener.h"
#include "EventListener_ABC.h"
#include "dispatcher/Formation_ABC.h"
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <numeric>

using namespace plugins::hla;

namespace
{
}

// -----------------------------------------------------------------------------
// Name: FormationProxy constructor
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
FormationProxy::FormationProxy( dispatcher::Formation_ABC& agent, const LocalAgentResolver_ABC& localAgentResolver )
    : agent_( agent )
    , localAgentResolver_( localAgentResolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationProxy destructor
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
FormationProxy::~FormationProxy()
{
    BOOST_FOREACH( const T_Subordinates::value_type& v, subordinates_ )
    {
        unsigned int id = v.first;
        boost::shared_ptr< ChildListener > childListener = childrenListeners_[ id ];
        v.second->Unregister( *childListener );
    }
}

// -----------------------------------------------------------------------------
// Name: FormationProxy::Register
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void FormationProxy::Register( EventListener_ABC& listener )
{
    listeners_.push_back( &listener );
    if( agent_.GetParent() != 0 )
    {
        std::string parentId( localAgentResolver_.Resolve( agent_.GetParent()->GetId() ) );
        if( parentId.size() > 0 )
        listener.ParentChanged( parentId );
    }
}

// -----------------------------------------------------------------------------
// Name: FormationProxy::Unregister
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void FormationProxy::Unregister( EventListener_ABC& listener )
{
    listeners_.erase( std::remove( listeners_.begin(), listeners_.end(), &listener ), listeners_.end() );
}

// -----------------------------------------------------------------------------
// Name: FormationProxy::AddSubordinate
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void FormationProxy::AddSubordinate( unsigned int id, Agent_ABC& agent )
{
    subordinates_.insert( T_Subordinates::value_type( id, &agent ) );
    boost::shared_ptr< ChildListener > childListener( new ChildListener( boost::bind( &FormationProxy::UpdateLocationCallback, this ) ) );
    childrenListeners_.insert( T_ChildrenListeners::value_type( id, childListener ) );
    agent.Register( *childListener );
    NotifyChildren();
}

// -----------------------------------------------------------------------------
// Name: FormationProxy::RemoveSubordinate
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void FormationProxy::RemoveSubordinate( unsigned int id )
{
    T_Subordinates::iterator it( subordinates_.find( id ) );
    if( subordinates_.end() != it )
    {
        boost::shared_ptr< ChildListener > childListener = childrenListeners_[ id ];
        childrenListeners_.erase( id );
        it->second->Unregister( *childListener );
        subordinates_.erase( it );
        NotifyChildren();
    }
}

// -----------------------------------------------------------------------------
// Name: FormationProxy::NotifyChildren
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void FormationProxy::NotifyChildren()
{
    EventListener_ABC::T_ChildrenIds children;
    BOOST_FOREACH( const T_Subordinates::value_type& v, subordinates_ )
    {
        std::string rtiId = localAgentResolver_.Resolve( v.first );
        if( rtiId.size() > 0 )
            children.insert( rtiId );
    }
    BOOST_FOREACH( EventListener_ABC* l, listeners_ )
        l->ChildrenChanged( children );
}

namespace
{
    ChildListener::LocationStruct addLocation( const ChildListener::LocationStruct& lhs, 
        const std::pair< unsigned int, boost::shared_ptr< ChildListener > >& rhs )
    {
        ChildListener::LocationStruct retval = lhs ;
        retval = retval + rhs.second->GetLocation();
        return retval;
    }
}

// -----------------------------------------------------------------------------
// Name: FormationProxy::UpdateLocationCallback
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void FormationProxy::UpdateLocationCallback()
{
    ChildListener::LocationStruct loc = std::accumulate( childrenListeners_.begin(), childrenListeners_.end(), ChildListener::LocationStruct() , &addLocation );
    loc = loc/childrenListeners_.size();
    BOOST_FOREACH( EventListener_ABC* l, listeners_ )
        l->SpatialChanged( loc.latitude, loc.longitude, loc.altitude, loc.speed, loc.direction );
}
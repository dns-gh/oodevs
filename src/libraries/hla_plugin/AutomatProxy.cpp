// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AutomatProxy.h"
#include "EventListener_ABC.h"
#include "LocalAgentResolver_ABC.h"
#include "ChildListener.h"
#include "dispatcher/Automat_ABC.h"
#include "dispatcher/Formation_ABC.h"
#include <boost/bind.hpp>
#include <numeric>
#include <functional>
#include <algorithm>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: AutomatProxy constructor
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
AutomatProxy::AutomatProxy( dispatcher::Automat_ABC& agent, const LocalAgentResolver_ABC& localAgentResolver )
    : agent_( agent )
    , localAgentResolver_( localAgentResolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatProxy destructor
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
AutomatProxy::~AutomatProxy()
{
    std::for_each( subordinates_.begin(), subordinates_.end(), [&](const T_Subordinates::value_type& v)
    {
        unsigned int id = v.first;
        boost::shared_ptr< ChildListener > childListener = childrenListeners_[ id ];
        v.second->Unregister( *childListener );
    });
}

// -----------------------------------------------------------------------------
// Name: AutomatProxy::Register
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void AutomatProxy::Register( EventListener_ABC& listener )
{
    listeners_.Register( listener );
    std::string parentId( localAgentResolver_.Resolve( agent_.GetFormation() != 0 ? agent_.GetFormation()->GetId() : agent_.GetParentAutomat()->GetId() ) );
    if( parentId.size() > 0 )
        listener.ParentChanged( parentId );
}

// -----------------------------------------------------------------------------
// Name: AutomatProxy::Unregister
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void AutomatProxy::Unregister( EventListener_ABC& listener )
{
	listeners_.Unregister( listener );
}

// -----------------------------------------------------------------------------
// Name: AutomatProxy::AddSubordinate
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void AutomatProxy::AddSubordinate( unsigned int id, Agent_ABC& agent )
{
    subordinates_.insert( T_Subordinates::value_type( id, &agent ) );
    boost::shared_ptr< ChildListener > childListener( new ChildListener( boost::bind( &AutomatProxy::UpdateLocationCallback, this, _1 ) ) );
    childrenListeners_.insert( T_ChildrenListeners::value_type( id, childListener ) );
    agent.Register( *childListener );
    NotifyChildren();
}

// -----------------------------------------------------------------------------
// Name: AutomatProxy::RemoveSubordinate
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void AutomatProxy::RemoveSubordinate( unsigned int id )
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
// Name: AutomatProxy::NotifyChildren
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void AutomatProxy::NotifyChildren()
{
    EventListener_ABC::T_ChildrenIds children;
    std::for_each( subordinates_.begin(), subordinates_.end(), [&](const T_Subordinates::value_type& v)
        {
            std::string rtiId = localAgentResolver_.Resolve( v.first );
            if( rtiId.size() > 0 )
                children.insert( rtiId );
        });
    listeners_.ChildrenChanged( children );
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
// Name: AutomatProxy::UpdateLocationCallback
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void AutomatProxy::UpdateLocationCallback( const ChildListener& )
{
    ChildListener::LocationStruct loc = std::accumulate( childrenListeners_.begin(), childrenListeners_.end(), ChildListener::LocationStruct() , &addLocation );
    loc = loc / static_cast< double >( childrenListeners_.size() );
    listeners_.SpatialChanged( loc.latitude, loc.longitude, loc.altitude, loc.speed, loc.direction );
}

// -----------------------------------------------------------------------------
// Name: AutomatProxy::HasSubordinate
// Created: AHC 2013-01-15
// -----------------------------------------------------------------------------
bool AutomatProxy::HasSubordinate( unsigned int id ) const
{
    return subordinates_.end() != subordinates_.find( id );
}

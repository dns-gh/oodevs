// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "Transporters.h"
#include "AgentSubject_ABC.h"
#include "CallsignResolver_ABC.h"
#include "SideChecker_ABC.h"
#include "TransportedUnitsVisitor_ABC.h"
#include "Agent_ABC.h"
#include "AutomatChecker_ABC.h"
#include <limits>
#include <boost/foreach.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: Transporters constructor
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
Transporters::Transporters( AgentSubject_ABC& agentSubject, const CallsignResolver_ABC& callsignResolver,
                            const SideChecker_ABC& sideChecker, const AutomatChecker_ABC& automatChecker )
    : agentSubject_    ( agentSubject )
    , callsignResolver_( callsignResolver )
    , sideChecker_     ( sideChecker )
    , automatChecker_  ( automatChecker )
{
    agentSubject_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Transporters destructor
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
Transporters::~Transporters()
{
    agentSubject_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Transporters::Apply
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
void Transporters::Apply( const std::string& transportedUnitUniqueId, const geometry::Point2d& embarkmentPoint, TransportedUnitsVisitor_ABC& visitor ) const
{
    const unsigned long transportedUnitSimulationIdentifier = callsignResolver_.ResolveSimulationIdentifier( transportedUnitUniqueId );
    unsigned int transporter = 0;
    double minimalDistance = std::numeric_limits< double >::max();
    BOOST_FOREACH( const boost::shared_ptr< T_Vessel > vessel, vessels_ )
        if( sideChecker_.AreSameSide( vessel->identifier_, transportedUnitSimulationIdentifier )
         && automatChecker_.IsAutomatDisengaged( vessel->identifier_ ) )
        {
            const double distance = vessel->position_.Distance( embarkmentPoint );
            if( distance < minimalDistance )
            {
                transporter = vessel->identifier_;
                minimalDistance = distance;
            }
        }
    if( transporter != 0 )
        visitor.Notify( callsignResolver_.ResolveCallsign( transporter ), callsignResolver_.ResolveUniqueId( transporter ) );
}

// -----------------------------------------------------------------------------
// Name: Transporters::AggregateCreated
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
void Transporters::AggregateCreated( Agent_ABC& /*agent*/, unsigned int /*identifier*/, const std::string& /*name*/, rpr::ForceIdentifier /*force*/, const rpr::EntityType& /*type*/, const std::string& /*symbol*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Transporters::SurfaceVesselCreated
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
void Transporters::SurfaceVesselCreated( Agent_ABC& agent, unsigned int identifier, const std::string& /*name*/, rpr::ForceIdentifier /*force*/, const rpr::EntityType& /*type*/ )
{
    boost::shared_ptr< T_Vessel > vessel( new T_Vessel() );
    vessel->identifier_ = identifier;
    agent.Register( *vessel );
    vessels_.push_back( vessel );
}

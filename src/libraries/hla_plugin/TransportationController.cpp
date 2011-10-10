// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "TransportationController.h"
#include "MissionResolver_ABC.h"
#include "TransportationListener_ABC.h"
#include "CallsignResolver_ABC.h"
#include "TransportedUnits_ABC.h"
#include "TransportedUnitsVisitor_ABC.h"
#include "Subordinates_ABC.h"
#include "protocol/Simulation.h"
#include <geometry/Types.h>
#include <xeumeuleu/xml.hpp>
#include <boost/foreach.hpp>
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

using namespace plugins::hla;
namespace bpt = boost::posix_time;

namespace
{
    unsigned int ResolveMission( xml::xisubstream xis, const MissionResolver_ABC& resolver )
    {
        std::string name;
        xis >> xml::start( "missions" )
                >> xml::content( "transport", name );
        return resolver.Resolve( name );
    }
}

// -----------------------------------------------------------------------------
// Name: TransportationController::oller
// Created: SLI 2011-10-06
// -----------------------------------------------------------------------------
TransportationController::TransportationController( xml::xisubstream xis, const MissionResolver_ABC& resolver,
                                                    tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                                    const CallsignResolver_ABC& callsignResolver, const Subordinates_ABC& subordinates )
    : transportIdentifier_( ResolveMission( xis, resolver ) )
    , callsignResolver_   ( callsignResolver )
    , subordinates_       ( subordinates )
{
    CONNECT( controller, *this, automat_order );
}

// -----------------------------------------------------------------------------
// Name: TransportationController destructor
// Created: SLI 2011-10-06
// -----------------------------------------------------------------------------
TransportationController::~TransportationController()
{
    // NOTHING
}

namespace
{
    geometry::Point2d ReadLocation( const sword::MissionParameter& parameter )
    {
        return geometry::Point2d( parameter.value( 0 ).location().coordinates().elem( 0 ).latitude(),
                                  parameter.value( 0 ).location().coordinates().elem( 0 ).longitude() );
    }
    long long ReadTime( const sword::MissionParameter& parameter )
    {
        const std::string data = parameter.value( 0 ).datetime().data();
        bpt::ptime time = bpt::from_iso_string( data );
        return ( time - bpt::from_time_t( 0 ) ).total_seconds();
    }
    unsigned long ReadAgent( const sword::MissionParameter& parameter )
    {
        return parameter.value( 0 ).agent().id();
    }
    class SubordinatesVisitor : public TransportedUnits_ABC
                              , private TransportedUnitsVisitor_ABC
    {
    public:
        SubordinatesVisitor( const Subordinates_ABC& subordinates, unsigned long automatIdentifier )
        {
            subordinates.Apply( automatIdentifier, *this );
        }
        virtual void Apply( TransportedUnitsVisitor_ABC& visitor ) const
        {
            for( std::vector< std::pair< std::string, std::string > >::const_iterator it = units_.begin(); it != units_.end(); ++it )
                visitor.Notify( it->first, it->second );
        }
        virtual void Notify( const std::string& callsign, const std::string& uniqueId )
        {
            units_.push_back( std::make_pair( callsign, uniqueId ) );
        }
    private:
        std::vector< std::pair< std::string, std::string > > units_;
    };
}

// -----------------------------------------------------------------------------
// Name: TransportationController::Notify
// Created: SLI 2011-10-06
// -----------------------------------------------------------------------------
void TransportationController::Notify(  const sword::AutomatOrder& message, int /*context*/ )
{
    if( message.type().id() == transportIdentifier_ && message.parameters().elem_size() == 9 )
    {
        const geometry::Point2d embarkmentPoint = ReadLocation( message.parameters().elem( 4 ) );
        const long long embarkmentTime = ReadTime( message.parameters().elem( 5 ) );
        const geometry::Point2d debarkmentPoint = ReadLocation( message.parameters().elem( 6 ) );
        const long long debarkmentTime = ReadTime( message.parameters().elem( 7 ) );
        const std::string transportingUnitCallsign = callsignResolver_.ResolveCallsign( ReadAgent( message.parameters().elem( 8 ) ) );
        const SubordinatesVisitor visitor( subordinates_, message.tasker().id() );
        BOOST_FOREACH( TransportationListener_ABC* listener, listeners_ )
            listener->ConvoyRequested( transportingUnitCallsign, embarkmentTime, embarkmentPoint, debarkmentTime, debarkmentPoint, visitor );
    }
}

// -----------------------------------------------------------------------------
// Name: TransportationController::Register
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
void TransportationController::Register( TransportationListener_ABC& listener )
{
    listeners_.push_back( &listener );
}

// -----------------------------------------------------------------------------
// Name: TransportationController::Unregister
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
void TransportationController::Unregister( TransportationListener_ABC& listener )
{
    listeners_.erase( std::remove( listeners_.begin(), listeners_.end(), &listener ), listeners_.end() );
}

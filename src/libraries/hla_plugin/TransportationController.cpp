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
#include "ContextFactory_ABC.h"
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
    unsigned int ResolveReportId( xml::xisubstream xis )
    {
        unsigned int reportType = 0;
        xis >> xml::start( "reports" )
                >> xml::content( "mission-complete", reportType );
        return reportType;
    }
}

// -----------------------------------------------------------------------------
// Name: TransportationController::oller
// Created: SLI 2011-10-06
// -----------------------------------------------------------------------------
TransportationController::TransportationController( xml::xisubstream xis, const MissionResolver_ABC& resolver,
                                                    tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                                    const CallsignResolver_ABC& callsignResolver, const Subordinates_ABC& subordinates,
                                                    const ContextFactory_ABC& contextFactory )
    : transportIdentifier_    ( ResolveMission( xis, resolver ) )
    , missionCompleteReportId_( ResolveReportId( xis ) )
    , callsignResolver_       ( callsignResolver )
    , subordinates_           ( subordinates )
    , contextFactory_         ( contextFactory )
{
    CONNECT( controller, *this, automat_order );
    CONNECT( controller, *this, report );
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
        const unsigned int context = contextFactory_.Create();
        pendingRequests_.insert( T_Requests::value_type( context, message.tasker().id() ) );
        BOOST_FOREACH( TransportationListener_ABC* listener, listeners_ )
            listener->ConvoyRequested( transportingUnitCallsign, embarkmentTime, embarkmentPoint, debarkmentTime, debarkmentPoint, visitor, context );
    }
}

// -----------------------------------------------------------------------------
// Name: TransportationController::Notify
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
void TransportationController::Notify( const sword::Report& message, int /*context*/ )
{
    if( !message.source().has_automat() )
        return;
    if( message.type().id() != missionCompleteReportId_ )
        return;
    const unsigned int automatId = message.source().automat().id();
    T_Requests::right_const_iterator request = acceptedRequests_.right.find( automatId );
    if( request == acceptedRequests_.right.end() )
        return;
    const unsigned int context = request->second;
    BOOST_FOREACH( TransportationListener_ABC* listener, listeners_ )
        listener->ReadyToReceiveService( context, contextProviders_[ context ] );
    Transfer( acceptedRequests_, readyToReceiveRequests_, context );
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

// -----------------------------------------------------------------------------
// Name: TransportationController::OfferReceived
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
void TransportationController::OfferReceived( unsigned int context, bool fullOffer, const std::string& provider )
{
    if( pendingRequests_.left.find( context ) == pendingRequests_.left.end() )
    {
        if( acceptedRequests_.left.find( context ) != acceptedRequests_.left.end() )
            BOOST_FOREACH( TransportationListener_ABC* listener, listeners_ )
                listener->OfferRejected( context, provider, "An other offer has already been accepted" );
        return;
    }
    if( fullOffer )
    {
        BOOST_FOREACH( TransportationListener_ABC* listener, listeners_ )
            listener->OfferAccepted( context, provider );
        contextProviders_[ context ] = provider;
        Transfer( pendingRequests_, acceptedRequests_, context );
    }
    else
        BOOST_FOREACH( TransportationListener_ABC* listener, listeners_ )
            listener->OfferRejected( context, provider, "Not offering service or partial offer" );
}

// -----------------------------------------------------------------------------
// Name: TransportationController::Transfer
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
void TransportationController::Transfer( T_Requests& from, T_Requests& to, unsigned int context ) const
{
    const unsigned int automatId = from.left.find( context )->second;
    from.left.erase( context );
    to.insert( T_Requests::value_type( context, automatId ) );
}

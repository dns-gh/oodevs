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
#include "protocol/SimulationSenders.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include <set>
#include <xeumeuleu/xml.hpp>
#include <boost/foreach.hpp>
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

using namespace plugins::hla;
namespace bpt = boost::posix_time;

namespace
{
    unsigned int ResolveMission( xml::xisubstream xis, const MissionResolver_ABC& resolver, const std::string& mission )
    {
        std::string name;
        xis >> xml::start( "missions" )
                >> xml::content( mission, name );
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
                                                    const ContextFactory_ABC& contextFactory, dispatcher::SimulationPublisher_ABC& publisher )
    : transportIdentifier_    ( ResolveMission( xis, resolver, "transport" ) )
    , missionCompleteReportId_( ResolveReportId( xis ) )
    , callsignResolver_       ( callsignResolver )
    , subordinates_           ( subordinates )
    , contextFactory_         ( contextFactory )
    , publisher_              ( publisher )
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
    if( message.type().id() == transportIdentifier_ && message.parameters().elem_size() == 8 )
    {
        const geometry::Point2d embarkmentPoint = ReadLocation( message.parameters().elem( 4 ) );
        const long long embarkmentTime = ReadTime( message.parameters().elem( 5 ) );
        const geometry::Point2d debarkmentPoint = ReadLocation( message.parameters().elem( 6 ) );
        const long long debarkmentTime = ReadTime( message.parameters().elem( 7 ) );
        const SubordinatesVisitor visitor( subordinates_, message.tasker().id() );
        const unsigned int context = contextFactory_.Create();
        pendingRequests_.right.erase( message.tasker().id() );
        pendingRequests_.insert( T_Requests::value_type( context, message.tasker().id() ) );
        contextRequests_[ context ].embarkmentPoint = embarkmentPoint;
        contextRequests_[ context ].debarkmentPoint = debarkmentPoint;
        BOOST_FOREACH( TransportationListener_ABC* listener, listeners_ )
            listener->ConvoyRequested( embarkmentTime, embarkmentPoint, debarkmentTime, debarkmentPoint, visitor, context );
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
        listener->ReadyToReceiveService( context, contextRequests_[ context ].provider );
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
void TransportationController::OfferReceived( unsigned int context, bool fullOffer, const std::string& provider, const interactions::ListOfTransporters& listOfTransporters )
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
        contextRequests_[ context ].provider = provider;
        contextRequests_[ context ].listOfTransporters = listOfTransporters;
        Transfer( pendingRequests_, acceptedRequests_, context );
    }
    else
        BOOST_FOREACH( TransportationListener_ABC* listener, listeners_ )
            listener->OfferRejected( context, provider, "Offering only partial offer" );
}

// -----------------------------------------------------------------------------
// Name: TransportationController::ServiceStarted
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
void TransportationController::ServiceStarted( unsigned int context )
{
    if( readyToReceiveRequests_.left.find( context ) == readyToReceiveRequests_.left.end() )
        return;
    Transfer( readyToReceiveRequests_, serviceStartedRequests_, context );
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

namespace
{
     class TransportedSubordinates : private TransportedUnitsVisitor_ABC
    {
    public:
        TransportedSubordinates( const TransportedUnits_ABC& transportedUnits )
        {
            transportedUnits.Apply( *this );
        }
    private:
        virtual void Notify( const std::string& /*callsign*/, const std::string& uniqueId )
        {
            transportedUnits_.insert( uniqueId );
        }
    public:
        std::set< std::string > transportedUnits_;
    };
     std::string ResolveUniqueIdFromCallsign( const std::string& callsign, const interactions::ListOfTransporters& listOfTransporters )
     {
         BOOST_FOREACH( const NetnObjectDefinitionStruct& unit, listOfTransporters.list )
             if( unit.callsign.str() == callsign )
                 return unit.uniqueId.str();
         return "";
     }
}

// -----------------------------------------------------------------------------
// Name: TransportationController::NotifyEmbarkationStatus
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
void TransportationController::NotifyEmbarkationStatus( unsigned int context, const std::string& transporterCallsign, const TransportedUnits_ABC& transportedUnits )
{
    SendTransportMagicAction( context, transporterCallsign, transportedUnits, sword::UnitMagicAction::load_unit );
}

// -----------------------------------------------------------------------------
// Name: TransportationController::NotifyDisembarkationStatus
// Created: SLI 2011-10-17
// -----------------------------------------------------------------------------
void TransportationController::NotifyDisembarkationStatus( unsigned int context, const std::string& transporterCallsign, const TransportedUnits_ABC& transportedUnits )
{
    SendTransportMagicAction( context, transporterCallsign, transportedUnits, sword::UnitMagicAction::unload_unit );
}

// -----------------------------------------------------------------------------
// Name: TransportationController::SendTransportMagicAction
// Created: SLI 2011-10-17
// -----------------------------------------------------------------------------
void TransportationController::SendTransportMagicAction( unsigned int context, const std::string& transporterCallsign, const TransportedUnits_ABC& transportedUnits, unsigned int actionType )
{
    T_Requests::left_const_iterator request = serviceStartedRequests_.left.find( context );
    if( request == serviceStartedRequests_.left.end() )
        return;
    const TransportedSubordinates subordinates( transportedUnits ) ;
    const T_Request& contextRequest = contextRequests_[ context ];
    const std::string transporterUniqueId = ResolveUniqueIdFromCallsign( transporterCallsign, contextRequest.listOfTransporters );
    const unsigned int transporterId = callsignResolver_.ResolveSimulationIdentifier( transporterUniqueId );
    BOOST_FOREACH( const std::string& uniqueId, subordinates.transportedUnits_ )
    {
        const unsigned int id = callsignResolver_.ResolveSimulationIdentifier( uniqueId );
        simulation::UnitMagicAction message;
        message().mutable_tasker()->mutable_unit()->set_id( transporterId );
        message().set_type( static_cast< sword::UnitMagicAction_Type >( actionType ) );
        message().mutable_parameters()->add_elem()->add_value()->mutable_agent()->set_id( id );
        message.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: TransportationController::ServiceComplete
// Created: SLI 2011-10-17
// -----------------------------------------------------------------------------
void TransportationController::ServiceComplete( unsigned int context, const std::string& provider )
{
    if( serviceStartedRequests_.left.find( context ) == serviceStartedRequests_.left.end() )
        return;
    Transfer( serviceStartedRequests_, completeRequests_, context );
    BOOST_FOREACH( TransportationListener_ABC* listener, listeners_ )
        listener->ServiceReceived( context, provider );
}

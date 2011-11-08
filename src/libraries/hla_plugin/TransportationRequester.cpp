// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "TransportationRequester.h"
#include "MissionResolver_ABC.h"
#include "CallsignResolver_ABC.h"
#include "TransportedUnitsVisitor_ABC.h"
#include "Subordinates_ABC.h"
#include "ContextFactory_ABC.h"
#include "InteractionSender_ABC.h"
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
    unsigned int Resolve( xml::xisubstream xis, const MissionResolver_ABC& resolver, const std::string& category, const std::string& mission )
    {
        std::string name;
        xis >> xml::start( "missions" )
                >> xml::start( category )
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

    template< typename T_Requests >
    void Transfer( T_Requests& from, T_Requests& to, unsigned int context )
    {
        const unsigned int automatId = from.left.find( context )->second;
        from.left.erase( context );
        to.insert( T_Requests::value_type( context, automatId ) );
    }

    std::string ResolveUniqueIdFromCallsign( const std::string& callsign, const interactions::ListOfUnits& listOfTransporters )
    {
        BOOST_FOREACH( const NetnObjectDefinitionStruct& unit, listOfTransporters.list )
            if( unit.callsign.str() == callsign )
                return unit.uniqueId.str();
        return "";
    }

    template< typename From, typename To >
    void CopyService( const From& from, To& to )
    {
       to.serviceId = from.serviceId;
       to.consumer = from.consumer;
       to.provider = from.provider;
       to.serviceType = from.serviceType;
    }

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

    bool CheckService( const interactions::NetnService& service )
    {
        return service.serviceType == 4 && service.serviceId.issuingObjectIdentifier.str() == "SWORD";
    }
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::oller
// Created: SLI 2011-10-06
// -----------------------------------------------------------------------------
TransportationRequester::TransportationRequester( xml::xisubstream xis, const MissionResolver_ABC& resolver,
                                                    tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                                    const CallsignResolver_ABC& callsignResolver, const Subordinates_ABC& subordinates,
                                                    const ContextFactory_ABC& contextFactory, dispatcher::SimulationPublisher_ABC& publisher,
                                                    InteractionSender_ABC< interactions::NetnRequestConvoy >& requestSender,
                                                    InteractionSender_ABC< interactions::NetnAcceptOffer >& acceptSender,
                                                    InteractionSender_ABC< interactions::NetnRejectOfferConvoy >& rejectSender,
                                                    InteractionSender_ABC< interactions::NetnReadyToReceiveService >& readySender,
                                                    InteractionSender_ABC< interactions::NetnServiceReceived >& receivedSender )
    : transportIdentifier_    ( Resolve( xis, resolver, "request", "transport" ) )
    , missionCompleteReportId_( ResolveReportId( xis ) )
    , pauseId_                ( Resolve( xis, resolver, "fragOrders", "pause" ) )
    , resumeId_               ( Resolve( xis, resolver, "fragOrders", "resume" ) )
    , cancelId_               ( Resolve( xis, resolver, "fragOrders", "cancel" ) )
    , callsignResolver_       ( callsignResolver )
    , subordinates_           ( subordinates )
    , contextFactory_         ( contextFactory )
    , publisher_              ( publisher )
    , requestSender_          ( requestSender )
    , acceptSender_           ( acceptSender )
    , rejectSender_           ( rejectSender )
    , readySender_            ( readySender )
    , receivedSender_         ( receivedSender )
{
    CONNECT( controller, *this, automat_order );
    CONNECT( controller, *this, report );
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester destructor
// Created: SLI 2011-10-06
// -----------------------------------------------------------------------------
TransportationRequester::~TransportationRequester()
{
    // NOTHING
}

namespace
{
    class SubordinatesVisitor : public TransportedUnitsVisitor_ABC
    {
    public:
        explicit SubordinatesVisitor( std::vector< NetnObjectDefinitionStruct >& units )
            : units_( units )
        {}
        virtual void Notify( const std::string& callsign, const std::string& uniqueId )
        {
            units_.push_back( NetnObjectDefinitionStruct( callsign, uniqueId, NetnObjectFeatureStruct() ) );
        }
    private:
        std::vector< NetnObjectDefinitionStruct >& units_;
    };
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Notify
// Created: SLI 2011-10-06
// -----------------------------------------------------------------------------
void TransportationRequester::Notify( const sword::AutomatOrder& message, int /*context*/ )
{
    if( message.type().id() == transportIdentifier_ && message.parameters().elem_size() == 8 )
    {
        const geometry::Point2d embarkmentPoint = ReadLocation( message.parameters().elem( 4 ) );
        const long long embarkmentTime = ReadTime( message.parameters().elem( 5 ) );
        const geometry::Point2d debarkmentPoint = ReadLocation( message.parameters().elem( 6 ) );
        const long long debarkmentTime = ReadTime( message.parameters().elem( 7 ) );
        const unsigned int context = contextFactory_.Create();
        pendingRequests_.right.erase( message.tasker().id() );
        pendingRequests_.insert( T_Requests::value_type( context, message.tasker().id() ) );
        interactions::NetnRequestConvoy request;
        request.serviceId = NetnEventIdentifier( context, "SWORD" );
        request.consumer = UnicodeString( "SWORD" );
        request.provider = UnicodeString( "Any carrier" );
        request.serviceType = 4; // convoy
        request.requestTimeOut = 0; // no timeout
        NetnDataTStruct transport;
        transport.appointment = NetnAppointmentStruct( embarkmentTime, rpr::WorldLocation( embarkmentPoint.X(), embarkmentPoint.Y(), 0. ) );
        transport.finalAppointment = NetnAppointmentStruct( debarkmentTime, rpr::WorldLocation( debarkmentPoint.X(), debarkmentPoint.Y(), 0. ) );
        SubordinatesVisitor subordinatesVisitor( transport.objectToManage );
        subordinates_.Apply( message.tasker().id(), subordinatesVisitor );
        request.transportData = NetnTransportStruct( transport );
        CopyService( request, contextRequests_[ context ] );
        contextRequests_[ context ].transportData = request.transportData;
        requestSender_.Send( request );
        Pause( message.tasker().id() );
    }
}

namespace
{
    void Reject( InteractionSender_ABC< interactions::NetnRejectOfferConvoy >& sender, interactions::NetnService& service, const std::string& reason )
    {
        interactions::NetnRejectOfferConvoy reject;
        CopyService( service, reject );
        reject.reason = reason;
        sender.Send( reject );
    }
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Receive
// Created: SLI 2011-10-25
// -----------------------------------------------------------------------------
void TransportationRequester::Receive( interactions::NetnOfferConvoy& interaction )
{
    if( ! CheckService( interaction ) )
        return;
    if( interaction.transportData.convoyType != 0 )
        return;
    if( !interaction.isOffering )
        return;
    const unsigned int context = interaction.serviceId.eventCount;
    if( pendingRequests_.left.find( context ) == pendingRequests_.left.end() )
    {
        if( acceptedRequests_.left.find( context ) != acceptedRequests_.left.end() )
            Reject( rejectSender_, interaction, "An other offer has already been accepted" );
        return;
    }
    if( interaction.offerType != 1 ) // full offer
        return Reject( rejectSender_, interaction, "Offering only partial offer" );
    interactions::NetnAcceptOffer accept;
    CopyService( interaction, accept );
    acceptSender_.Send( accept );
    Resume( pendingRequests_.left.find( context )->second );
    contextRequests_[ context ] = interaction;
    Transfer( pendingRequests_, acceptedRequests_, context );
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Notify
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
void TransportationRequester::Notify( const sword::Report& message, int /*context*/ )
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
    interactions::NetnReadyToReceiveService ready;
    CopyService( contextRequests_[ context ], ready );
    readySender_.Send( ready );
    Transfer( acceptedRequests_, readyToReceiveRequests_, context );
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Receive
// Created: SLI 2011-10-25
// -----------------------------------------------------------------------------
void TransportationRequester::Receive( interactions::NetnServiceStarted& interaction )
{
    if( ! CheckService( interaction ) )
        return;
    const unsigned int context = interaction.serviceId.eventCount;
    if( readyToReceiveRequests_.left.find( context ) == readyToReceiveRequests_.left.end() )
        return;
    Transfer( readyToReceiveRequests_, serviceStartedRequests_, context );
}

namespace
{
    void TeleportToDestination( unsigned int identifier, double latitude, double longitude, dispatcher::SimulationPublisher_ABC& publisher )
    {
        simulation::UnitMagicAction message;
        message().mutable_tasker()->mutable_unit()->set_id( identifier );
        message().set_type( sword::UnitMagicAction::move_to );
        sword::MissionParameter& parameter = *message().mutable_parameters()->add_elem();
        parameter.set_null_value( false );
        sword::Location& location = *parameter.add_value()->mutable_point()->mutable_location();
        location.set_type( sword::Location::point );
        sword::CoordLatLong& coordinates = *location.mutable_coordinates()->add_elem();
        coordinates.set_latitude( latitude );
        coordinates.set_longitude( longitude );
        message.Send( publisher );
    }
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::SendTransportMagicAction
// Created: SLI 2011-10-17
// -----------------------------------------------------------------------------
void TransportationRequester::SendTransportMagicAction( unsigned int context, const std::string& transporterCallsign, const interactions::ListOfUnits& units, unsigned int actionType, bool teleport )
{
    T_Requests::left_const_iterator request = serviceStartedRequests_.left.find( context );
    if( request == serviceStartedRequests_.left.end() )
        return;
    const interactions::NetnOfferConvoy& contextRequest = contextRequests_[ context ];
    const std::string transporterUniqueId = ResolveUniqueIdFromCallsign( transporterCallsign, contextRequest.listOfTransporters );
    const unsigned int transporterId = callsignResolver_.ResolveSimulationIdentifier( transporterUniqueId );
    BOOST_FOREACH( const NetnObjectDefinitionStruct& unit, units.list )
    {
        const unsigned int id = callsignResolver_.ResolveSimulationIdentifier( unit.uniqueId.str() );
        simulation::UnitMagicAction message;
        message().mutable_tasker()->mutable_unit()->set_id( transporterId );
        message().set_type( static_cast< sword::UnitMagicAction_Type >( actionType ) );
        message().mutable_parameters()->add_elem()->add_value()->mutable_agent()->set_id( id );
        message.Send( publisher_ );
        if( teleport )
            TeleportToDestination( id, contextRequest.transportData.dataTransport.finalAppointment.location.Latitude(),
                                       contextRequest.transportData.dataTransport.finalAppointment.location.Longitude(), publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Receive
// Created: SLI 2011-10-25
// -----------------------------------------------------------------------------
void TransportationRequester::Receive( interactions::NetnConvoyEmbarkmentStatus& interaction )
{
    if( ! CheckService( interaction ) )
        return;
    const unsigned int context = interaction.serviceId.eventCount;
    SendTransportMagicAction( context, interaction.transportUnitIdentifier.str(), interaction.listOfObjectEmbarked, sword::UnitMagicAction::load_unit, false );
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Receive
// Created: SLI 2011-10-25
// -----------------------------------------------------------------------------
void TransportationRequester::Receive( interactions::NetnConvoyDisembarkmentStatus& interaction )
{
    if( ! CheckService( interaction ) )
        return;
    const unsigned int context = interaction.serviceId.eventCount;
    SendTransportMagicAction( context, interaction.transportUnitIdentifier.str(), interaction.listOfObjectDisembarked, sword::UnitMagicAction::unload_unit, true );
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Receive
// Created: SLI 2011-10-25
// -----------------------------------------------------------------------------
void TransportationRequester::Receive( interactions::NetnServiceComplete& interaction )
{
    if( ! CheckService( interaction ) )
        return;
    const unsigned int context = interaction.serviceId.eventCount;
    if( serviceStartedRequests_.left.find( context ) == serviceStartedRequests_.left.end() )
        return;
    Transfer( serviceStartedRequests_, completeRequests_, context );
    interactions::NetnServiceReceived received;
    CopyService( interaction, received );
    receivedSender_.Send( received );
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Receive
// Created: SLI 2011-11-07
// -----------------------------------------------------------------------------
void TransportationRequester::Receive( interactions::NetnConvoyDestroyedEntities& interaction )
{
    if( ! CheckService( interaction ) )
        return;
    const unsigned int context = interaction.serviceId.eventCount;
    if( serviceStartedRequests_.left.find( context ) == serviceStartedRequests_.left.end() )
        return;
    BOOST_FOREACH( const NetnObjectDefinitionStruct& entity, interaction.listOfEmbarkedObjectDestroyed.list )
    {
        const unsigned int entityIdentifier = callsignResolver_.ResolveSimulationIdentifier( entity.uniqueId.str() );
        simulation::UnitMagicAction message;
        message().mutable_tasker()->mutable_unit()->set_id( entityIdentifier );
        message().set_type( sword::UnitMagicAction::destroy_all );
        message().mutable_parameters();
        message.Send( publisher_ );
    }
}

namespace
{
    void SendFragOrder( unsigned int automat, unsigned int fragOrderType, dispatcher::SimulationPublisher_ABC& publisher )
    {
        simulation::FragOrder order;
        order().mutable_tasker()->mutable_automat()->set_id( automat );
        order().mutable_type()->set_id( fragOrderType );
        order.Send( publisher );
    }
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Pause
// Created: SLI 2011-11-07
// -----------------------------------------------------------------------------
void TransportationRequester::Pause( unsigned int automat )
{
    SendFragOrder( automat, pauseId_, publisher_ );
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Resume
// Created: SLI 2011-11-07
// -----------------------------------------------------------------------------
void TransportationRequester::Resume( unsigned int automat )
{
    SendFragOrder( automat, resumeId_, publisher_ );
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Cancel
// Created: SLI 2011-11-07
// -----------------------------------------------------------------------------
void TransportationRequester::Cancel( unsigned int automat )
{
    SendFragOrder( automat, cancelId_, publisher_ );
}

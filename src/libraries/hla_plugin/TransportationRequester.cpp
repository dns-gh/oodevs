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
    : transportIdentifier_    ( ResolveMission( xis, resolver, "transport" ) )
    , missionCompleteReportId_( ResolveReportId( xis ) )
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
        contextRequests_[ context ].embarkmentPoint = embarkmentPoint;
        contextRequests_[ context ].debarkmentPoint = debarkmentPoint;
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
        requestSender_.Send( request );
    }
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Receive
// Created: SLI 2011-10-25
// -----------------------------------------------------------------------------
void TransportationRequester::Receive( interactions::NetnOfferConvoy& interaction )
{
    if( interaction.serviceType != 4 )
        return;
    if( interaction.transportData.convoyType != 0 )
        return;
    if( interaction.serviceId.issuingObjectIdentifier.str() != "SWORD" )
        return;
    if( !interaction.isOffering )
        return;
    const unsigned int context = interaction.serviceId.eventCount;
    if( pendingRequests_.left.find( context ) == pendingRequests_.left.end() )
    {
        if( acceptedRequests_.left.find( context ) != acceptedRequests_.left.end() )
        {
            interactions::NetnRejectOfferConvoy reject;
            CopyService( interaction, reject );
            reject.reason = "An other offer has already been accepted";
            rejectSender_.Send( reject );
        }
        return;
    }
    if( interaction.offerType == 1 ) // full offer
    {
        interactions::NetnAcceptOffer accept;
        CopyService( interaction, accept );
        acceptSender_.Send( accept );
        contextRequests_[ context ].provider = interaction.provider.str();
        contextRequests_[ context ].listOfTransporters = interaction.listOfTransporters;
        Transfer( pendingRequests_, acceptedRequests_, context );
    }
    else
    {
        interactions::NetnRejectOfferConvoy reject;
        CopyService( interaction, reject );
        reject.reason = "Offering only partial offer";
        rejectSender_.Send( reject );
    }
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
    // $$$$
    readySender_.Send( ready );
    Transfer( acceptedRequests_, readyToReceiveRequests_, context );
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Receive
// Created: SLI 2011-10-25
// -----------------------------------------------------------------------------
void TransportationRequester::Receive( interactions::NetnServiceStarted& interaction )
{
    if( interaction.serviceType != 4 )
        return;
    if( interaction.serviceId.issuingObjectIdentifier.str() != "SWORD" )
        return;
    const unsigned int context = interaction.serviceId.eventCount;
    if( readyToReceiveRequests_.left.find( context ) == readyToReceiveRequests_.left.end() )
        return;
    Transfer( readyToReceiveRequests_, serviceStartedRequests_, context );
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::SendTransportMagicAction
// Created: SLI 2011-10-17
// -----------------------------------------------------------------------------
void TransportationRequester::SendTransportMagicAction( unsigned int context, const std::string& transporterCallsign, const interactions::ListOfUnits& units, unsigned int actionType )
{
    T_Requests::left_const_iterator request = serviceStartedRequests_.left.find( context );
    if( request == serviceStartedRequests_.left.end() )
        return;
    const T_Request& contextRequest = contextRequests_[ context ];
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
    }
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Receive
// Created: SLI 2011-10-25
// -----------------------------------------------------------------------------
void TransportationRequester::Receive( interactions::NetnConvoyEmbarkmentStatus& interaction )
{
    if( interaction.serviceType != 4 )
        return;
    if( interaction.serviceId.issuingObjectIdentifier.str() != "SWORD" )
        return;
    const unsigned int context = interaction.serviceId.eventCount;
    SendTransportMagicAction( context, interaction.transportUnitIdentifier.str(), interaction.listOfObjectEmbarked, sword::UnitMagicAction::load_unit );
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Receive
// Created: SLI 2011-10-25
// -----------------------------------------------------------------------------
void TransportationRequester::Receive( interactions::NetnConvoyDisembarkmentStatus& interaction )
{
    if( interaction.serviceType != 4 )
        return;
    if( interaction.serviceId.issuingObjectIdentifier.str() != "SWORD" )
        return;
    const unsigned int context = interaction.serviceId.eventCount;
    SendTransportMagicAction( context, interaction.transportUnitIdentifier.str(), interaction.listOfObjectDisembarked, sword::UnitMagicAction::unload_unit );
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Receive
// Created: SLI 2011-10-25
// -----------------------------------------------------------------------------
void TransportationRequester::Receive( interactions::NetnServiceComplete& interaction )
{
    if( interaction.serviceType != 4 )
        return;
    if( interaction.serviceId.issuingObjectIdentifier.str() != "SWORD" )
        return;
    const unsigned int context = interaction.serviceId.eventCount;
    if( serviceStartedRequests_.left.find( context ) == serviceStartedRequests_.left.end() )
        return;
    Transfer( serviceStartedRequests_, completeRequests_, context );
    interactions::NetnServiceReceived received;
    CopyService( interaction, received );
    receivedSender_.Send( received );
}

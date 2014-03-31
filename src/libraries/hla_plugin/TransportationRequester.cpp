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
#include "ProtocolTools.h"
#include "protocol/SimulationSenders.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include <set>
#include <algorithm>
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )


using namespace plugins::hla;
namespace bpt = boost::posix_time;

namespace
{
    std::string GetName( xml::xisubstream xis, const std::string& category, const std::string& mission )
    {
        std::string name;
        xis >> xml::start( "missions" )
                >> xml::start( category )
                    >> xml::content( mission, name );
        return name;
    }

    unsigned int ResolveReportId( xml::xisubstream xis, const std::string& report, bool optional=false )
    {
        unsigned int reportType = 0;
        if(!optional)
        {
            xis >> xml::start( "reports" )
                >> xml::content( report, reportType );
        }
        else
        {
            xis >> xml::start( "reports" )
                >> xml::optional >> xml::content( report, reportType );
        }
        return reportType;
    }

    template< typename T_Requests >
    void Transfer( T_Requests& from, T_Requests& to, unsigned int context )
    {
        const unsigned int automatId = from.left.find( context )->second;
        from.left.erase( context );
        to.insert( T_Requests::value_type( context, automatId ) );
    }

    std::vector< char > ResolveUniqueIdFromCallsign( const std::string& callsign, const interactions::ListOfUnits& listOfTransporters )
    {
        for( auto it = listOfTransporters.list.begin(); listOfTransporters.list.end() != it; ++it )
        {
            const NetnObjectDefinitionStruct& unit( *it );
            if( unit.callsign.str() == callsign )
                return unit.uniqueId;
        }
        return std::vector< char >();
    }

    template< typename From, typename To >
    void CopyService( const From& from, To& to )
    {
       to.serviceId = from.serviceId;
       to.consumer = from.consumer;
       to.provider = from.provider;
       to.serviceType = from.serviceType;
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

    bool CheckService( const interactions::NetnService& service, const std::string& federateName )
    {
        return service.serviceType == 4 && service.serviceId.issuingObjectIdentifier.str() == federateName;
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
                                                  InteractionSender_ABC< interactions::NetnServiceReceived >& receivedSender,
                                                  InteractionSender_ABC< interactions::NetnCancelConvoy >& cancelSender )
    : missionCompleteReportId_( ResolveReportId( xis, "mission-complete" ) )
    , awaitingCarriersReportId_( ResolveReportId( xis, "awaiting-carriers", true ) )
    , pauseId_                ( resolver.ResolveAutomat( GetName( xis, "fragOrders", "pause" ) ) )
    , resumeId_               ( resolver.ResolveAutomat( GetName( xis, "fragOrders", "resume" ) ) )
    , cancelId_               ( resolver.ResolveAutomat( GetName( xis, "fragOrders", "cancel" ) ) )
    , callsignResolver_       ( callsignResolver )
    , subordinates_           ( subordinates )
    , contextFactory_         ( contextFactory )
    , publisher_              ( publisher )
    , requestSender_          ( requestSender )
    , acceptSender_           ( acceptSender )
    , rejectSender_           ( rejectSender )
    , readySender_            ( readySender )
    , receivedSender_         ( receivedSender )
    , cancelSender_           ( cancelSender )
    , federateName_           ( xis.attribute<std::string>( "name", "SWORD" ) )
{
    CONNECT( controller, *this, automat_order );
    CONNECT( controller, *this, unit_order );
    CONNECT( controller, *this, report );
    CONNECT( controller, *this, frag_order );
    xis >> xml::start("missions") >> xml::start("request")
        >> xml::list( "transport", *this, &TransportationRequester::ReadMission, transportIds_, resolver )
        >> xml::list( "embarkment", *this, &TransportationRequester::ReadMission, embarkmentIds_, resolver )
        >> xml::list( "disembarkment", *this, &TransportationRequester::ReadMission, disembarkmentIds_, resolver );
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
        virtual void Notify( const std::string& callsign, const std::vector< char >& uniqueId )
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
    ProcessTransport(message, true);
    ProcessEmbark(message, true, embarkmentIds_, true);
    ProcessEmbark(message, true, disembarkmentIds_, false);
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::ProcessTransport
// Created: AHC 2012-02-09
// -----------------------------------------------------------------------------
template <typename T>
void TransportationRequester::ProcessTransport(const T& message, bool isAutmaton)
{
   if( message.parameters().elem_size() == 8 &&
        std::find( transportIds_.begin(), transportIds_.end(), message.type().id() ) != transportIds_.end() )
    {
        const geometry::Point2d embarkmentPoint = ProtocolTools::ConvertToPoint( message.parameters().elem( 4 ) );
        const long long embarkmentTime = ReadTime( message.parameters().elem( 5 ) );
        const geometry::Point2d debarkmentPoint = ProtocolTools::ConvertToPoint( message.parameters().elem( 6 ) );
        const long long debarkmentTime = ReadTime( message.parameters().elem( 7 ) );
        const unsigned int context = contextFactory_.Create();
        pendingRequests_.right.erase( message.tasker().id() );
        pendingRequests_.insert( T_Requests::value_type( context, message.tasker().id() ) );
        interactions::NetnRequestConvoy request;
        request.serviceId = NetnEventIdentifier( context, federateName_ );
        request.consumer = UnicodeString( federateName_ );
        request.provider = UnicodeString(); // Empty provider
        request.serviceType = 4; // convoy
        request.requestTimeOut = 0; // no timeout
        NetnDataTStruct transport;
        transport.appointment = NetnAppointmentStruct( embarkmentTime, rpr::WorldLocation( embarkmentPoint.X(), embarkmentPoint.Y(), 0. ) );
        transport.finalAppointment = NetnAppointmentStruct( debarkmentTime, rpr::WorldLocation( debarkmentPoint.X(), debarkmentPoint.Y(), 0. ) );
        if( isAutmaton )
        {
            SubordinatesVisitor subordinatesVisitor( transport.objectToManage );
            subordinates_.Apply( message.tasker().id(), subordinatesVisitor );
        }
        else
            transport.objectToManage.push_back( NetnObjectDefinitionStruct( callsignResolver_.ResolveCallsign( message.tasker().id() ), callsignResolver_.ResolveUniqueId( message.tasker().id() ), NetnObjectFeatureStruct() ) );
        request.transportData = NetnTransportStruct( transport );
        CopyService( request, contextRequests_[ context ] );
        contextRequests_[ context ].transportData = request.transportData;
        requestSender_.Send( request );
        Pause( message.tasker().id() );
    }
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::ProcessTransport
// Created: AHC 2012-02-09
// -----------------------------------------------------------------------------
template <typename T>
void TransportationRequester::ProcessEmbark(const T& message, bool isAutomaton, const std::vector< unsigned int >& missions, bool embark)
{
    if( message.parameters().elem_size() == 6 &&
        std::find( missions.begin(), missions.end(), message.type().id() ) != missions.end() )
    {
        const NetnTransportStruct::ConvoyType transportType = embark ? NetnTransportStruct::E_Embarkment : NetnTransportStruct::E_Disembarkment ;
        const geometry::Point2d embarkmentPoint = ProtocolTools::ConvertToPoint( message.parameters().elem( 4 ) );
        const long long embarkmentTime = ReadTime( message.parameters().elem( 5 ) );
        const unsigned int context = contextFactory_.Create();
        pendingRequests_.right.erase( message.tasker().id() );
        pendingRequests_.insert( T_Requests::value_type( context, message.tasker().id() ) );
        interactions::NetnRequestConvoy request;
        request.serviceId = NetnEventIdentifier( context, federateName_ );
        request.consumer = UnicodeString( federateName_ );
        request.provider = UnicodeString(); // Empty provider
        request.serviceType = 4; // convoy
        request.requestTimeOut = 0; // no timeout
        NetnDataEDStruct transport;
        transport.appointment = NetnAppointmentStruct( embarkmentTime, rpr::WorldLocation( embarkmentPoint.X(), embarkmentPoint.Y(), 0. ) );
        if( isAutomaton )
        {
            SubordinatesVisitor subordinatesVisitor( transport.objectToManage );
            subordinates_.Apply( message.tasker().id(), subordinatesVisitor );
        }
        else
            transport.objectToManage.push_back( NetnObjectDefinitionStruct( callsignResolver_.ResolveCallsign( message.tasker().id() ), callsignResolver_.ResolveUniqueId( message.tasker().id() ), NetnObjectFeatureStruct() ) );
        request.transportData = NetnTransportStruct( transport, transportType );
        CopyService( request, contextRequests_[ context ] );
        contextRequests_[ context ].transportData = request.transportData;
        requestSender_.Send( request );
        Pause( message.tasker().id() );
    }
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Notify
// Created: SLI 2011-10-06
// -----------------------------------------------------------------------------
void TransportationRequester::Notify( const sword::UnitOrder& message, int /*context*/ )
{
    ProcessTransport(message, false);
    ProcessEmbark(message, false, embarkmentIds_, true);
    ProcessEmbark(message, false, disembarkmentIds_, false);
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
    if( ! CheckService( interaction, federateName_ ) )
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
    {
        Reject( rejectSender_, interaction, "Offering only partial offer" );
        return;
    }
    interactions::NetnAcceptOffer accept;
    CopyService( interaction, accept );
    acceptSender_.Send( accept );
    Resume( pendingRequests_.left.find( context )->second );
    contextRequests_[ context ] = interaction;
    if( interaction.transportData.convoyType == NetnTransportStruct::E_Disembarkment )
    {
        interactions::NetnReadyToReceiveService ready;
        CopyService( contextRequests_[ context ], ready );
        readySender_.Send( ready );
        Transfer( pendingRequests_, readyToReceiveRequests_, context );
    }
    else
        Transfer( pendingRequests_, acceptedRequests_, context );

}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Notify
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
void TransportationRequester::Notify( const sword::Report& message, int /*context*/ )
{
    if( !( message.source().has_automat() || message.source().has_unit() ) )
        return;
    if( message.type().id() != missionCompleteReportId_ && 
        message.type().id() != awaitingCarriersReportId_ )
        return;
    const unsigned int identifier = message.source().has_automat() ? message.source().automat().id() : message.source().unit().id();
    T_Requests::right_const_iterator request = acceptedRequests_.right.find( identifier );
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
    if( ! CheckService( interaction, federateName_ ) )
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
    const std::vector< char > transporterUniqueId = ResolveUniqueIdFromCallsign( transporterCallsign, contextRequest.listOfTransporters );
    const unsigned int transporterId = callsignResolver_.ResolveSimulationIdentifier( transporterUniqueId );
    std::for_each( units.list.begin(), units.list.end(), [&](const NetnObjectDefinitionStruct& unit)
    {
        const unsigned int id = callsignResolver_.ResolveSimulationIdentifier( unit.uniqueId );

        simulation::UnitMagicAction message;
        message().mutable_tasker()->mutable_unit()->set_id( transporterId );
        message().set_type( static_cast< sword::UnitMagicAction_Type >( actionType ) );
        message().mutable_parameters()->add_elem()->add_value()->mutable_agent()->set_id( id );
        message.Send( publisher_ );
        if( teleport )
            if( contextRequest.transportData.convoyType == NetnTransportStruct::E_Transport )
                TeleportToDestination( id, contextRequest.transportData.dataTransport.finalAppointment.location.Latitude(),
                                       contextRequest.transportData.dataTransport.finalAppointment.location.Longitude(), publisher_ );
            else
                TeleportToDestination( id, contextRequest.transportData.dataDisembarkment.appointment.location.Latitude(),
                                       contextRequest.transportData.dataDisembarkment.appointment.location.Longitude(), publisher_ );
    });
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Receive
// Created: SLI 2011-10-25
// -----------------------------------------------------------------------------
void TransportationRequester::Receive( interactions::NetnConvoyEmbarkmentStatus& interaction )
{
    if( ! CheckService( interaction, federateName_ ) )
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
    if( ! CheckService( interaction, federateName_ ) )
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
    if( ! CheckService( interaction, federateName_ ) )
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
    if( ! CheckService( interaction, federateName_ ) )
        return;
    const unsigned int context = interaction.serviceId.eventCount;
    if( serviceStartedRequests_.left.find( context ) == serviceStartedRequests_.left.end() )
        return;

    std::for_each( interaction.listOfEmbarkedObjectDestroyed.list.begin(), interaction.listOfEmbarkedObjectDestroyed.list.end(), [&](const NetnObjectDefinitionStruct& entity)
    {
        const unsigned int entityIdentifier = callsignResolver_.ResolveSimulationIdentifier( entity.uniqueId );

        simulation::UnitMagicAction message;
        message().mutable_tasker()->mutable_unit()->set_id( entityIdentifier );
        message().set_type( sword::UnitMagicAction::destroy_all );
        message().mutable_parameters();
        message.Send( publisher_ );
    });
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
void TransportationRequester::Pause( unsigned int entity )
{
    SendFragOrder( entity, pauseId_, publisher_ );
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Resume
// Created: SLI 2011-11-07
// -----------------------------------------------------------------------------
void TransportationRequester::Resume( unsigned int entity )
{
    SendFragOrder( entity, resumeId_, publisher_ );
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Cancel
// Created: SLI 2011-11-07
// -----------------------------------------------------------------------------
void TransportationRequester::Cancel( unsigned int entity )
{
    SendFragOrder( entity, cancelId_, publisher_ );
}
// -----------------------------------------------------------------------------
// Name: TransportationRequester::ReadMission
// Created: AHC 2012-02-09
// -----------------------------------------------------------------------------
void TransportationRequester::ReadMission(xml::xistream& xis, std::vector<unsigned int>& v,  const MissionResolver_ABC& resolver) const
{
    std::string name;
    xis >> name;
    try
    {
        v.push_back( resolver.ResolveAutomat( name ) );
    }
    catch( const tools::Exception& )
    {
        // NOTHING
    }
    v.push_back( resolver.ResolveUnit( name ) );
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Receive
// Created: AHC 2012-02-10
// -----------------------------------------------------------------------------
void TransportationRequester::Receive( interactions::NetnCancelConvoy& interaction )
{    
    if( ! CheckService( interaction, federateName_ ) )
        return;

    const unsigned int context = interaction.serviceId.eventCount;
    if( pendingRequests_.left.find( context ) != pendingRequests_.left.end() )
    {
        Cancel( pendingRequests_.left.find( context )->second );
        Transfer( pendingRequests_, completeRequests_, context );
    }
    else if( acceptedRequests_.left.find( context ) != acceptedRequests_.left.end() )
    {
        Cancel( acceptedRequests_.left.find( context )->second );
        Transfer( acceptedRequests_, completeRequests_, context );
    }
    else if( readyToReceiveRequests_.left.find( context ) != readyToReceiveRequests_.left.end() )
    {
        Transfer( readyToReceiveRequests_, completeRequests_, context );
    }
    else if( serviceStartedRequests_.left.find( context ) != serviceStartedRequests_.left.end() )
    {
        Transfer( serviceStartedRequests_, completeRequests_, context );
    }
    
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::Receive
// Created: AHC 2012-02-10
// -----------------------------------------------------------------------------
void TransportationRequester::Notify( const sword::FragOrder& message, int /*context*/ )
{
    if( message.type().id() != cancelId_ ||
        ( !message.tasker().has_automat() && !message.tasker().has_unit() ) )
        return;
    unsigned int unit = message.tasker().has_automat() ?
                        message.tasker().automat().id() :
                        message.tasker().unit().id();
    T_Requests::right_const_iterator it = pendingRequests_.right.find( unit );
    if( it != pendingRequests_.right.end() )
    {
        CancelOffer(pendingRequests_, it->second);
    }
    else if( (it = acceptedRequests_.right.find( unit )) != acceptedRequests_.right.end() )
    {
        CancelOffer(acceptedRequests_, it->second);
    }
    else if( (it = readyToReceiveRequests_.right.find( unit )) != readyToReceiveRequests_.right.end() )
    {
        CancelOffer(readyToReceiveRequests_, it->second);
    }
    else if( (it = serviceStartedRequests_.right.find( unit )) != serviceStartedRequests_.right.end() )
    {
        CancelOffer(serviceStartedRequests_, it->second);
    }
}

// -----------------------------------------------------------------------------
// Name: TransportationRequester::CancelOffer
// Created: AHC 2012-02-16
// -----------------------------------------------------------------------------
void TransportationRequester::CancelOffer(T_Requests& container, unsigned int context)
{
    const interactions::NetnOfferConvoy& offer = contextRequests_[context];
    interactions::NetnCancelConvoy cancel;
    cancel.consumer = offer.consumer;
    cancel.provider = offer.provider;
    cancel.serviceType = offer.serviceType;
    cancel.serviceId = offer.serviceId;
    cancel.reason = "mission cancelled";
    cancelSender_.Send( cancel );
    Transfer(container, completeRequests_, context);
}

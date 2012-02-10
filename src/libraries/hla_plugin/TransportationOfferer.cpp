// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "TransportationOfferer.h"
#include "InteractionSender_ABC.h"
#include "ContextFactory_ABC.h"
#include "Interactions.h"
#include "MissionResolver_ABC.h"
#include "TransportedUnitsVisitor_ABC.h"
#include "CallsignResolver_ABC.h"
#include "protocol/Simulation.h"
#include "protocol/MessengerSenders.h"
#include "protocol/ClientPublisher_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/lexical_cast.hpp>
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )
#include <boost/foreach.hpp>

using namespace plugins::hla;
namespace bpt = boost::posix_time;

namespace
{
    unsigned int ResolveMission( xml::xisubstream xis, const MissionResolver_ABC& resolver, const std::string& mission )
    {
        std::string name;
        xis >> xml::content( mission, name );
        return resolver.ResolveUnit( name );
    }

    unsigned int ResolveReportId( xml::xisubstream xis )
    {
        unsigned int reportType = 0;
        xis >> xml::start( "reports" )
                >> xml::content( "mission-complete", reportType );
        return reportType;
    }

    template< typename T >
    void Transfer( T& from, T& to, const std::string& identifier )
    {
        T::mapped_type value = from[ identifier ];
        from.erase( identifier );
        to[ identifier ] = value;
    }
    std::string ServiceIdentifier( const interactions::NetnService& service )
    {
        return service.serviceId.issuingObjectIdentifier.str() + boost::lexical_cast< std::string >( service.serviceId.eventCount );
    }
}

// -----------------------------------------------------------------------------
// Name: TransportationOfferer constructor
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
TransportationOfferer::TransportationOfferer( xml::xisubstream xis, const MissionResolver_ABC& missionResolver,
                                              InteractionSender_ABC< interactions::NetnOfferConvoy >& offerInteractionSender,
                                              InteractionSender_ABC< interactions::NetnServiceStarted >& serviceStartedInteractionSender,
                                              InteractionSender_ABC< interactions::NetnConvoyEmbarkmentStatus >& convoyEmbarkmentStatusSender,
                                              InteractionSender_ABC< interactions::NetnConvoyDisembarkmentStatus >& convoyDisembarkmentStatusSender,
                                              InteractionSender_ABC< interactions::NetnConvoyDestroyedEntities >& convoyDestroyedEntitiesSender,
                                              InteractionSender_ABC< interactions::NetnServiceComplete >& serviceCompleteSender,
                                              tools::MessageController_ABC< sword::SimToClient_Content >& messageController, const ContextFactory_ABC& factory,
                                              const CallsignResolver_ABC& callsignRevoler, dispatcher::ClientPublisher_ABC& clientsPublisher )
    : offerInteractionSender_         ( offerInteractionSender )
    , serviceStartedInteractionSender_( serviceStartedInteractionSender )
    , convoyEmbarkmentStatusSender_   ( convoyEmbarkmentStatusSender )
    , convoyDisembarkmentStatusSender_( convoyDisembarkmentStatusSender )
    , convoyDestroyedEntitiesSender_  ( convoyDestroyedEntitiesSender )
    , serviceCompleteSender_          ( serviceCompleteSender )
    , messageController_              ( messageController )
    , factory_                        ( factory )
    , callsignResolver_                ( callsignRevoler )
    , clientsPublisher_               ( clientsPublisher )
    , missionCompleteReportId_        ( ResolveReportId( xis ) )
{
    xml::xisubstream sxis(xis);
    xis >> xml::start( "missions" )
        >> xml::start( "offer" );
    transportIdentifier_ = ResolveMission( xis, missionResolver, "transport" );
    embarkIdentifier_ = ResolveMission( xis, missionResolver, "embarkment" );
    disembarkIdentifier_ = ResolveMission( xis, missionResolver, "disembarkment" );

    CONNECT( messageController, *this, unit_order );
    CONNECT( messageController, *this, unit_attributes );
    CONNECT( messageController, *this, control_end_tick );
}

// -----------------------------------------------------------------------------
// Name: TransportationOfferer destructor
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
TransportationOfferer::~TransportationOfferer()
{
    // NOTHING
}

namespace
{
    std::string Appointment( const NetnAppointmentStruct& appointment )
    {
        bpt::ptime time( bpt::from_time_t( appointment.dateTime ) );
        return " at: " + bpt::to_simple_string( time );
    }
    std::string ListUnits( const std::vector< NetnObjectDefinitionStruct >& objectsToManage, const CallsignResolver_ABC& callsignResolver )
    {
        std::string result;
        for( std::vector< NetnObjectDefinitionStruct >::const_iterator it = objectsToManage.begin(); it != objectsToManage.end(); )
        {
            result +=  "<a href='id://agent/" + boost::lexical_cast< std::string >( callsignResolver.ResolveSimulationIdentifier( it->uniqueId.str() ) ) + "#select'>" +  it->callsign.str() + "</a>";
            if( ++it == objectsToManage.end() )
                return result;
            result += ", ";
        }
        return result;
    }
    void SendDrawingPoint( dispatcher::ClientPublisher_ABC& clientsPublisher, const NetnAppointmentStruct& appointment, unsigned int identifier, unsigned int red, unsigned int green, unsigned int blue )
    {
        sword::MessengerToClient point; // $$$$ _RC_ SLI 2011-10-20: should send a ClientToMessenger::ShapeCreationRequest instead
        sword::ShapeCreation* shape = point.mutable_message()->mutable_shape_creation();
        shape->mutable_id()->set_id( identifier );
        shape->mutable_shape()->set_category( "Markers" );
        shape->mutable_shape()->set_pattern( "Point" );
        shape->mutable_shape()->mutable_color()->set_red( red );
        shape->mutable_shape()->mutable_color()->set_green( green );
        shape->mutable_shape()->mutable_color()->set_blue( blue );
        sword::CoordLatLong* coord = shape->mutable_shape()->mutable_points()->add_elem();
        coord->set_latitude( appointment.location.Latitude() );
        coord->set_longitude( appointment.location.Longitude() );
        clientsPublisher.Send( point );
    }
    rpr::WorldLocation ReadLocation( const sword::MissionParameter& parameter )
    {
        return rpr::WorldLocation( parameter.value( 0 ).point().location().coordinates().elem( 0 ).latitude(),
                                   parameter.value( 0 ).point().location().coordinates().elem( 0 ).longitude(),
                                   0. );
    }
}

// -----------------------------------------------------------------------------
// Name: TransportationOfferer::Receive
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
void TransportationOfferer::Receive( interactions::NetnRequestConvoy& request )
{
    if( request.serviceType != 4 ) // Convoy service type
        return;
    if( request.transportData.dataTransport.objectToManage.empty() )
        return;
    sword::MessengerToClient message;
    sword::TextMessage* text = message.mutable_message()->mutable_text_message();
    text->mutable_source()->set_profile( "" );
    text->mutable_target()->set_profile( "" );

    switch( static_cast<NetnTransportStruct::ConvoyType>(request.transportData.convoyType) )
    {
    case NetnTransportStruct::E_Transport:
        text->set_message( "/display \"<b>Transport request received:</b><br>"
                       "<font color='red'>Embarking point</font>" + Appointment( request.transportData.dataTransport.appointment ) + "<br>"
                       "<font color='blue'>Debarking point</font>" + Appointment( request.transportData.dataTransport.finalAppointment ) + "<br>"
                       "Units to embark: " + ListUnits( request.transportData.dataTransport.objectToManage, callsignResolver_ ) + "<br>"
                       "Request identifier: " + ServiceIdentifier( request ) + "<br>\"" );
        clientsPublisher_.Send( message );
        SendDrawingPoint( clientsPublisher_, request.transportData.dataTransport.appointment, factory_.Create(), 255, 0, 0 );
        SendDrawingPoint( clientsPublisher_, request.transportData.dataTransport.finalAppointment, factory_.Create(), 0, 0, 255 );
        break;
    case NetnTransportStruct::E_Embarkment:
        text->set_message( "/display \"<b>Embarkment request received:</b><br>"
                       "<font color='red'>Embarking point</font>" + Appointment( request.transportData.dataEmbarkment.appointment ) + "<br>"
                       "Units to embark: " + ListUnits( request.transportData.dataEmbarkment.objectToManage, callsignResolver_ ) + "<br>"
                       "Request identifier: " + ServiceIdentifier( request ) + "<br>\"" );
        clientsPublisher_.Send( message );
        SendDrawingPoint( clientsPublisher_, request.transportData.dataEmbarkment.appointment, factory_.Create(), 255, 0, 0 );
        break;
    case NetnTransportStruct::E_Disembarkment:
        text->set_message( "/display \"<b>Disembarkment request received:</b><br>"
                       "<font color='blue'>Disembarking point</font>" + Appointment( request.transportData.dataDisembarkment.appointment ) + "<br>"
                       "Units to debark: " + ListUnits( request.transportData.dataDisembarkment.objectToManage, callsignResolver_ ) + "<br>"
                       "Request identifier: " + ServiceIdentifier( request ) + "<br>\"" );
        clientsPublisher_.Send( message );
        SendDrawingPoint( clientsPublisher_, request.transportData.dataDisembarkment.appointment, factory_.Create(), 0, 0, 255 );
        break;
    }
    interactions::NetnOfferConvoy offer;
    offer.isOffering = 1; // True
    offer.requestTimeOut = 0u; // No timeout
    offer.serviceId = request.serviceId;
    offer.consumer = request.consumer;
    offer.serviceType = request.serviceType;
    offer.transportData = request.transportData;
    offer.offerType = 1; // RequestAcknowledgePositive
    pendingOffers_[ ServiceIdentifier( offer ) ] = offer;
}

// -----------------------------------------------------------------------------
// Name: TransportationOfferer::Notify
// Created: SLI 2011-10-27
// -----------------------------------------------------------------------------
void TransportationOfferer::Notify( const sword::UnitOrder& message, int /*context*/ )
{
    if( message.type().id() == transportIdentifier_ && message.parameters().elem_size() >= 7 )
    {
        const std::string& serviceId = message.parameters().elem( 8 ).value( 0 ).acharstr();
        T_Offers::iterator it = pendingOffers_.find( serviceId );
        if( it == pendingOffers_.end() )
            return;
        interactions::NetnOfferConvoy& offer = it->second;
        offer.provider = UnicodeString( callsignResolver_.ResolveCallsign( message.tasker().id() ) );
        offer.transportData.dataTransport.appointment.location = ReadLocation( message.parameters().elem( 6 ) );
        offer.transportData.dataTransport.finalAppointment.location = ReadLocation( message.parameters().elem( 5 ) );
        offer.listOfTransporters.list.push_back( NetnObjectDefinitionStruct( callsignResolver_.ResolveCallsign( message.tasker().id() ),
                                                                             callsignResolver_.ResolveUniqueId( message.tasker().id() ),
                                                                             NetnObjectFeatureStruct() ) );
        transporters_.left.erase( message.tasker().id() );
        transporters_.left.insert( T_Transporters::left_value_type( message.tasker().id(), serviceId ) );
        for( int i = 0; i < message.parameters().elem( 4 ).value_size(); ++i )
        {
            const unsigned int agent = message.parameters().elem( 4 ).value( i ).agent().id();
            offer.transportData.dataTransport.objectToManage.push_back( NetnObjectDefinitionStruct( callsignResolver_.ResolveCallsign( agent ),
                                                                                                    callsignResolver_.ResolveUniqueId( agent ),
                                                                                                    NetnObjectFeatureStruct() ) );
        }
        if( message.parameters().elem( 9 ).value( 0 ).booleanvalue() )
            offer.offerType = 0;
        offerInteractionSender_.Send( offer );
        Transfer( pendingOffers_, offeredOffers_, serviceId );
    }
    else if( message.type().id() == embarkIdentifier_ && message.parameters().elem_size() >= 5 )
    {
        const std::string& serviceId = message.parameters().elem( 6 ).value( 0 ).acharstr();
        T_Offers::iterator it = pendingOffers_.find( serviceId );
        if( it == pendingOffers_.end() )
            return;
        interactions::NetnOfferConvoy& offer = it->second;
        offer.provider = UnicodeString( callsignResolver_.ResolveCallsign( message.tasker().id() ) );
        offer.transportData.dataEmbarkment.appointment.location = ReadLocation( message.parameters().elem( 5 ) );
        offer.listOfTransporters.list.push_back( NetnObjectDefinitionStruct( callsignResolver_.ResolveCallsign( message.tasker().id() ),
                                                                             callsignResolver_.ResolveUniqueId( message.tasker().id() ),
                                                                             NetnObjectFeatureStruct() ) );
        transporters_.left.erase( message.tasker().id() );
        transporters_.left.insert( T_Transporters::left_value_type( message.tasker().id(), serviceId ) );
        for( int i = 0; i < message.parameters().elem( 4 ).value_size(); ++i )
        {
            const unsigned int agent = message.parameters().elem( 4 ).value( i ).agent().id();
            offer.transportData.dataEmbarkment.objectToManage.push_back( NetnObjectDefinitionStruct( callsignResolver_.ResolveCallsign( agent ),
                                                                                                    callsignResolver_.ResolveUniqueId( agent ),
                                                                                                    NetnObjectFeatureStruct() ) );
        }
        if( message.parameters().elem( 7 ).value( 0 ).booleanvalue() )
            offer.offerType = 0;
        offerInteractionSender_.Send( offer );
        Transfer( pendingOffers_, offeredOffers_, serviceId );
    }
    else if( message.type().id() == disembarkIdentifier_ && message.parameters().elem_size() >= 5 )
    {
        const std::string& serviceId = message.parameters().elem( 6 ).value( 0 ).acharstr();
        T_Offers::iterator it = pendingOffers_.find( serviceId );
        if( it == pendingOffers_.end() )
            return;
        interactions::NetnOfferConvoy& offer = it->second;
        offer.provider = UnicodeString( callsignResolver_.ResolveCallsign( message.tasker().id() ) );
        offer.transportData.dataDisembarkment.appointment.location = ReadLocation( message.parameters().elem( 5 ) );
        offer.listOfTransporters.list.push_back( NetnObjectDefinitionStruct( callsignResolver_.ResolveCallsign( message.tasker().id() ),
                                                                             callsignResolver_.ResolveUniqueId( message.tasker().id() ),
                                                                             NetnObjectFeatureStruct() ) );
        transporters_.left.erase( message.tasker().id() );
        transporters_.left.insert( T_Transporters::left_value_type( message.tasker().id(), serviceId ) );
        for( int i = 0; i < message.parameters().elem( 4 ).value_size(); ++i )
        {
            const unsigned int agent = message.parameters().elem( 4 ).value( i ).agent().id();
            offer.transportData.dataDisembarkment.objectToManage.push_back( NetnObjectDefinitionStruct( callsignResolver_.ResolveCallsign( agent ),
                                                                                                    callsignResolver_.ResolveUniqueId( agent ),
                                                                                                    NetnObjectFeatureStruct() ) );
        }
        if( message.parameters().elem( 7 ).value( 0 ).booleanvalue() )
            offer.offerType = 0;
        offerInteractionSender_.Send( offer );
        Transfer( pendingOffers_, offeredOffers_, serviceId );
    }
}

// -----------------------------------------------------------------------------
// Name: TransportationOfferer::Receive
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
void TransportationOfferer::Receive( interactions::NetnAcceptOffer& accept )
{
    const std::string serviceId = ServiceIdentifier( accept );
    if( offeredOffers_.find( serviceId ) == offeredOffers_.end() )
        return;
    const interactions::NetnOfferConvoy& offer = offeredOffers_[ serviceId ];
    switch( static_cast<NetnTransportStruct::ConvoyType>(offer.transportData.convoyType) )
    {
    case NetnTransportStruct::E_Transport:
        BOOST_FOREACH( const NetnObjectDefinitionStruct& unit, offer.transportData.dataTransport.objectToManage )
            remainingTransported_[ serviceId ].insert( callsignResolver_.ResolveSimulationIdentifier( unit.uniqueId.str() ) );
        break;
    case NetnTransportStruct::E_Embarkment:
        BOOST_FOREACH( const NetnObjectDefinitionStruct& unit, offer.transportData.dataEmbarkment.objectToManage )
            remainingTransported_[ serviceId ].insert( callsignResolver_.ResolveSimulationIdentifier( unit.uniqueId.str() ) );
        break;
    case NetnTransportStruct::E_Disembarkment:
        BOOST_FOREACH( const NetnObjectDefinitionStruct& unit, offer.transportData.dataDisembarkment.objectToManage )
            remainingTransported_[ serviceId ].insert( callsignResolver_.ResolveSimulationIdentifier( unit.uniqueId.str() ) );
        break;
    }
    Transfer( offeredOffers_, acceptedOffers_, serviceId );
}

// -----------------------------------------------------------------------------
// Name: TransportationOfferer::Receive
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
void TransportationOfferer::Receive( interactions::NetnReadyToReceiveService& readyToReceive )
{
    const std::string serviceId = ServiceIdentifier( readyToReceive );
    if( acceptedOffers_.find( serviceId ) == acceptedOffers_.end() )
        return;
    Transfer( acceptedOffers_, startedOffers_, serviceId );
    interactions::NetnServiceStarted serviceStarted;
    serviceStarted.serviceId = readyToReceive.serviceId;
    serviceStarted.consumer = readyToReceive.consumer;
    serviceStarted.provider = readyToReceive.provider;
    serviceStarted.serviceType = readyToReceive.serviceType;
    serviceStartedInteractionSender_.Send( serviceStarted );
}

// -----------------------------------------------------------------------------
// Name: TransportationOfferer::Receive
// Created: SLI 2011-11-07
// -----------------------------------------------------------------------------
void TransportationOfferer::Receive( interactions::NetnServiceReceived& serviceReceived )
{
    const std::string serviceId = ServiceIdentifier( serviceReceived );
    if( completetedOffers_.find( serviceId ) == completetedOffers_.end() )
        return;
    Transfer( completetedOffers_, receivedOffers_, serviceId );
    remainingTransported_.erase( serviceId );
    transported_.erase( transporters_.right.find( serviceId )->second );
    transporters_.right.erase( serviceId );
}

// -----------------------------------------------------------------------------
// Name: TransportationOfferer::Notify
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
void TransportationOfferer::Notify( const sword::UnitAttributes& message, int /*context*/ )
{
    if( !( message.has_transporting_unit() || message.has_dead() ) )
        return;
    if( message.has_transporting_unit() )
        HandleConvoyStatus( message );
    if( message.has_dead() )
        HandleTransporterDeath( message );
}

// -----------------------------------------------------------------------------
// Name: TransportationOfferer::Notify
// Created: SLI 2011-11-08
// -----------------------------------------------------------------------------
void TransportationOfferer::Notify( const sword::ControlEndTick& /*message*/, int /*context*/ )
{
    droppedThisTick_.clear();
}

// -----------------------------------------------------------------------------
// Name: TransportationOfferer::HandleTransporterDeath
// Created: SLI 2011-11-08
// -----------------------------------------------------------------------------
void TransportationOfferer::HandleTransporterDeath( const sword::UnitAttributes& message )
{
    T_Transporters::left_const_iterator transporter = transporters_.left.find( message.unit().id() );
    if( transporter == transporters_.left.end() )
        return;
    if( !transported_[ message.unit().id() ].empty() || !droppedThisTick_[ message.unit().id() ].empty() )
    {
        const interactions::NetnOfferConvoy& offer = startedOffers_[ transporter->second ];
        interactions::NetnConvoyDestroyedEntities destruction;
        destruction.serviceId = offer.serviceId;
        destruction.consumer = offer.consumer;
        destruction.provider = offer.provider;
        destruction.serviceType = offer.serviceType;
        BOOST_FOREACH( const unsigned int transported, transported_[ message.unit().id() ] )
            destruction.listOfEmbarkedObjectDestroyed.list.push_back( NetnObjectDefinitionStruct(
                                                                        callsignResolver_.ResolveCallsign( transported ),
                                                                        callsignResolver_.ResolveUniqueId( transported ), NetnObjectFeatureStruct() ) );
        BOOST_FOREACH( const unsigned int transported, droppedThisTick_[ message.unit().id() ] )
            destruction.listOfEmbarkedObjectDestroyed.list.push_back( NetnObjectDefinitionStruct(
                                                                        callsignResolver_.ResolveCallsign( transported ),
                                                                        callsignResolver_.ResolveUniqueId( transported ), NetnObjectFeatureStruct() ) );
        convoyDestroyedEntitiesSender_.Send( destruction );
    }
}

namespace
{
    template< typename T >
    void Send( T& status, interactions::ListOfUnits& listOfUnits, unsigned int transportedId, InteractionSender_ABC< T >& sender, const interactions::NetnService& offer, unsigned int transporterId, const CallsignResolver_ABC& callsignResolver )
    {
        status.consumer = offer.consumer;
        status.provider = offer.provider;
        status.serviceId = offer.serviceId;
        status.serviceType = offer.serviceType;
        status.transportUnitIdentifier = UnicodeString( callsignResolver.ResolveCallsign( transporterId ) );
        listOfUnits.list.push_back( NetnObjectDefinitionStruct(
            callsignResolver.ResolveCallsign( transportedId ),
            callsignResolver.ResolveUniqueId( transportedId ), NetnObjectFeatureStruct() ) );
        sender.Send( status );
    }
    template< typename T >
    bool CheckEndOfTransportation( T& remainingTransported, InteractionSender_ABC< interactions::NetnServiceComplete >& sender, const interactions::NetnOfferConvoy& offer )
    {
        if( remainingTransported.empty() )
        {
            interactions::NetnServiceComplete complete;
            complete.serviceId = offer.serviceId;
            complete.consumer = offer.consumer;
            complete.provider = offer.provider;
            complete.serviceType = offer.serviceType;
            sender.Send( complete );
            return true;
        }
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: TransportationOfferer::HandleConvoyStatus
// Created: SLI 2011-11-08
// -----------------------------------------------------------------------------
void TransportationOfferer::HandleConvoyStatus( const sword::UnitAttributes& message )
{
    const unsigned int transported = message.unit().id();
    unsigned int transporter = message.transporting_unit().id();
    if( transporter != 0 )
    {
        T_Transporters::left_const_iterator it = transporters_.left.find( transporter );
        if( it == transporters_.left.end() )
            return;
        if( remainingTransported_[ it->second ].find( transported ) == remainingTransported_[ it->second ].end() )
            return;
        transported_[ transporter ].insert( transported );
        transportedBy_[ transported ] = transporter;
        SendEmbarkmentStatus( transporter, transported );
        const interactions::NetnOfferConvoy& offer = startedOffers_[ it->second ];
        if( offer.transportData.convoyType == NetnTransportStruct::E_Embarkment )
        { 
            remainingTransported_[ it->second ].erase( transported );
            if( CheckEndOfTransportation( remainingTransported_[ it->second ], serviceCompleteSender_, offer ) )
            {
                remainingTransported_.erase( it->second );
                Transfer( startedOffers_, completetedOffers_, it->second );
            }
        }
    }
    else
    {
        T_TransportedBy::const_iterator transportedBy = transportedBy_.find( transported );
        if( transportedBy == transportedBy_.end() )
            return;
        transporter = transportedBy->second;
        T_Transporters::left_const_iterator it = transporters_.left.find( transporter );
        if( it == transporters_.left.end() )
            return;
        remainingTransported_[ it->second ].erase( transported );
        transportedBy_.erase( transported );
        transported_[ transporter ].erase( transported );
        SendDisembarkmentStatus( transporter, transported );
        droppedThisTick_[ transporter ].insert( transported );
        const interactions::NetnOfferConvoy& offer = startedOffers_[ it->second ];
        if( CheckEndOfTransportation( remainingTransported_[ it->second ], serviceCompleteSender_, offer ) )
        {
            remainingTransported_.erase( it->second );
            Transfer( startedOffers_, completetedOffers_, it->second );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: TransportationOfferer::SendEmbarkmentStatus
// Created: SLI 2011-11-08
// -----------------------------------------------------------------------------
void TransportationOfferer::SendEmbarkmentStatus( unsigned int transporter, unsigned int transported )
{
    T_Transporters::left_const_iterator it = transporters_.left.find( transporter );
    const interactions::NetnOfferConvoy& offer = startedOffers_[ it->second ];
    interactions::NetnConvoyEmbarkmentStatus embarkmentStatus;
    Send( embarkmentStatus, embarkmentStatus.listOfObjectEmbarked, transported, convoyEmbarkmentStatusSender_, offer, transporter, callsignResolver_ );
}

// -----------------------------------------------------------------------------
// Name: TransportationOfferer::SendDisembarkmentStatus
// Created: SLI 2011-11-08
// -----------------------------------------------------------------------------
void TransportationOfferer::SendDisembarkmentStatus( unsigned int transporter, unsigned int untransported )
{
    T_Transporters::left_const_iterator it = transporters_.left.find( transporter );
    const interactions::NetnOfferConvoy& offer = startedOffers_[ it->second ];
    interactions::NetnConvoyDisembarkmentStatus disembarkmentStatus;
    Send( disembarkmentStatus, disembarkmentStatus.listOfObjectDisembarked, untransported, convoyDisembarkmentStatusSender_, offer, transporter, callsignResolver_ );
}

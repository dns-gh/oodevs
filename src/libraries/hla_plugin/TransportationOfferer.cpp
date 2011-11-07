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
        xis >> xml::start( "missions" )
                >> xml::start( "offer" )
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
                                              tools::MessageController_ABC< sword::SimToClient_Content >& messageController, const ContextFactory_ABC& factory,
                                              const CallsignResolver_ABC& callsignRevoler, dispatcher::ClientPublisher_ABC& clientsPublisher )
    : offerInteractionSender_         ( offerInteractionSender )
    , serviceStartedInteractionSender_( serviceStartedInteractionSender )
    , convoyEmbarkmentStatusSender_   ( convoyEmbarkmentStatusSender )
    , convoyDisembarkmentStatusSender_( convoyDisembarkmentStatusSender )
    , messageController_              ( messageController )
    , factory_                        ( factory )
    , callsignRevoler_                ( callsignRevoler )
    , clientsPublisher_               ( clientsPublisher )
    , transportIdentifier_            ( ResolveMission( xis, missionResolver, "transport" ) )
    , missionCompleteReportId_        ( ResolveReportId( xis ) )
{
    CONNECT( messageController, *this, unit_order );
    CONNECT( messageController, *this, unit_attributes );
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
    if( request.transportData.convoyType != 0 ) // Transport convoy type
        return;
    if( request.transportData.dataTransport.objectToManage.empty() )
        return;
    sword::MessengerToClient message;
    sword::TextMessage* text = message.mutable_message()->mutable_text_message();
    text->mutable_source()->set_profile( "" );
    text->mutable_target()->set_profile( "" );
    text->set_message( "/display \"<b>Transport request received:</b><br>"
                       "<font color='red'>Embarking point</font>" + Appointment( request.transportData.dataTransport.appointment ) + "<br>"
                       "<font color='blue'>Debarking point</font>" + Appointment( request.transportData.dataTransport.finalAppointment ) + "<br>"
                       "Units to embark: " + ListUnits( request.transportData.dataTransport.objectToManage, callsignRevoler_ ) + "<br>"
                       "Request identifier: " + ServiceIdentifier( request ) + "<br>\"" );
    clientsPublisher_.Send( message );
    SendDrawingPoint( clientsPublisher_, request.transportData.dataTransport.appointment, factory_.Create(), 255, 0, 0 );
    SendDrawingPoint( clientsPublisher_, request.transportData.dataTransport.finalAppointment, factory_.Create(), 0, 0, 255 );
    interactions::NetnOfferConvoy offer;
    offer.isOffering = 1; // True
    offer.requestTimeOut = 0u; // No timeout
    offer.serviceId = request.serviceId;
    offer.consumer = request.consumer;
    offer.provider = UnicodeString( "SWORD" );
    offer.serviceType = request.serviceType;
    offer.offerType = 1; // RequestAcknowledgePositive
    offer.transportData = request.transportData;
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
        offer.transportData.dataTransport.appointment.location = ReadLocation( message.parameters().elem( 6 ) );
        offer.transportData.dataTransport.finalAppointment.location = ReadLocation( message.parameters().elem( 5 ) );
        offer.listOfTransporters.list.push_back( NetnObjectDefinitionStruct( callsignRevoler_.ResolveCallsign( message.tasker().id() ),
                                                                             callsignRevoler_.ResolveUniqueId( message.tasker().id() ),
                                                                             NetnObjectFeatureStruct() ) );
        transporters_[ message.tasker().id() ] = serviceId;
        for( int i = 0; i < message.parameters().elem( 4 ).value_size(); ++i )
        {
            const unsigned int agent = message.parameters().elem( 4 ).value( i ).agent().id();
            offer.transportData.dataTransport.objectToManage.push_back( NetnObjectDefinitionStruct( callsignRevoler_.ResolveCallsign( agent ),
                                                                                                    callsignRevoler_.ResolveUniqueId( agent ),
                                                                                                    NetnObjectFeatureStruct() ) );
        }
        if( message.parameters().elem( 9 ).value( 0 ).booleanvalue() )
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
    if( accept.provider.str() != "SWORD" )
        return;
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
    if( readyToReceive.provider.str() != "SWORD" )
        return;
    Transfer( acceptedOffers_, startedOffers_, serviceId );
    interactions::NetnServiceStarted serviceStarted;
    serviceStarted.serviceId = readyToReceive.serviceId;
    serviceStarted.consumer = readyToReceive.consumer;
    serviceStarted.provider = readyToReceive.provider;
    serviceStarted.serviceType = readyToReceive.serviceType;
    serviceStartedInteractionSender_.Send( serviceStarted );
}

namespace
{
    template< typename T >
    void UpdateTransported( T& oldEmbarked, T& embarked, T& debarked, const sword::UnitIdList& unitList )
    {
        T currentEmbarked;
        for( int i = 0; i < unitList.elem_size(); ++i )
            currentEmbarked.insert( unitList.elem( i ).id() );
        std::set_difference( oldEmbarked.begin(), oldEmbarked.end(), currentEmbarked.begin(), currentEmbarked.end(), std::insert_iterator< T >( debarked, debarked.begin() ) );
        std::set_difference( currentEmbarked.begin(), currentEmbarked.end(), oldEmbarked.begin(), oldEmbarked.end(), std::insert_iterator< T >( embarked, embarked.begin() ) );
        oldEmbarked.clear();
        oldEmbarked.insert( currentEmbarked.begin(), currentEmbarked.end() );
    }

    template< typename T, typename U >
    void Send( U& status, interactions::ListOfUnits& listOfUnits, const T& units, InteractionSender_ABC< U >& sender, const interactions::NetnService& offer, unsigned int transporterId, const CallsignResolver_ABC& callsignResolver )
    {
        if( units.empty() )
            return;
        status.consumer = offer.consumer;
        status.provider = offer.provider;
        status.serviceId = offer.serviceId;
        status.serviceType = offer.serviceType;
        status.transportUnitIdentifier = UnicodeString( callsignResolver.ResolveCallsign( transporterId ) );
        BOOST_FOREACH( unsigned int identifier, units )
            listOfUnits.list.push_back( NetnObjectDefinitionStruct( 
                callsignResolver.ResolveCallsign( identifier ),
                callsignResolver.ResolveUniqueId( identifier ), NetnObjectFeatureStruct() ) );
        sender.Send( status );
    }
}

// -----------------------------------------------------------------------------
// Name: TransportationOfferer::Notify
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
void TransportationOfferer::Notify( const sword::UnitAttributes& message, int /*context*/ )
{
    if( !message.has_transported_units() )
        return;
    T_Transporters::const_iterator transporter = transporters_.find( message.unit().id() );
    if( transporter == transporters_.end() )
        return;
    T_Transported embarked;
    T_Transported debarked;
    UpdateTransported( transported_[ message.unit().id() ], embarked, debarked, message.transported_units() );
    const interactions::NetnOfferConvoy& offer = startedOffers_[ transporter->second ];
    interactions::NetnConvoyEmbarkmentStatus embarkmentStatus;
    Send( embarkmentStatus, embarkmentStatus.listOfObjectEmbarked, embarked, convoyEmbarkmentStatusSender_, offer, message.unit().id(), callsignRevoler_ );
    interactions::NetnConvoyDisembarkmentStatus disembarkmentStatus;
    Send( disembarkmentStatus, disembarkmentStatus.listOfObjectDisembarked, debarked, convoyDisembarkmentStatusSender_, offer, message.unit().id(), callsignRevoler_ );
}

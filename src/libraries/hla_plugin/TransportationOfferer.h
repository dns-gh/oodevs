// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_TransportationOfferer_h
#define plugins_hla_TransportationOfferer_h

#include "Interactions.h"
#include "tools/MessageObserver.h"
#include <hla/InteractionNotification_ABC.h>
#include <set>
#include <map>
#pragma warning( push, 0 )
#include <boost/bimap.hpp>
#pragma warning( pop )

namespace sword
{
    class SimToClient_Content;
    class UnitAttributes;
    class UnitOrder;
    class ControlEndTick;
}

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace tools
{
    template< typename T > class MessageController_ABC;
}

namespace xml
{
    class xisubstream;
}

namespace plugins
{
namespace hla
{
namespace interactions
{
    struct NetnRequestConvoy;
    struct NetnOfferConvoy;
    struct NetnAcceptOffer;
    struct NetnReadyToReceiveService;
    struct NetnServiceStarted;
    struct NetnConvoyEmbarkmentStatus;
    struct NetnConvoyDisembarkmentStatus;
    struct NetnConvoyDestroyedEntities;
}
    template< typename T > class InteractionSender_ABC;
    class CallsignResolver_ABC;
    class ContextFactory_ABC;
    class MissionResolver_ABC;

// =============================================================================
/** @class  TransportationOfferer
    @brief  Netn offer convoy sender
*/
// Created: SLI 2011-10-11
// =============================================================================
class TransportationOfferer : public ::hla::InteractionNotification_ABC< interactions::NetnRequestConvoy >
                            , public ::hla::InteractionNotification_ABC< interactions::NetnAcceptOffer >
                            , public ::hla::InteractionNotification_ABC< interactions::NetnReadyToReceiveService >
                            , private tools::MessageObserver< sword::UnitAttributes >
                            , private tools::MessageObserver< sword::UnitOrder >
                            , private tools::MessageObserver< sword::ControlEndTick >
{
public:
    //! @name Constructors/Destructor
    //@{
             TransportationOfferer( xml::xisubstream xis, const MissionResolver_ABC& missionResolver,
                                    InteractionSender_ABC< interactions::NetnOfferConvoy >& offerInteractionSender,
                                    InteractionSender_ABC< interactions::NetnServiceStarted >& serviceStartedInteractionSender,
                                    InteractionSender_ABC< interactions::NetnConvoyEmbarkmentStatus >& convoyEmbarkmentStatusSender,
                                    InteractionSender_ABC< interactions::NetnConvoyDisembarkmentStatus >& convoyDisembarkmentStatusSender,
                                    InteractionSender_ABC< interactions::NetnConvoyDestroyedEntities >& convoyDestroyedEntitiesSender,
                                    InteractionSender_ABC< interactions::NetnServiceComplete >& serviceCompleteSender,
                                    tools::MessageController_ABC< sword::SimToClient_Content >& messageController, const ContextFactory_ABC& factory,
                                    const CallsignResolver_ABC& callsignRevoler, dispatcher::ClientPublisher_ABC& clientsPublisher );
    virtual ~TransportationOfferer();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( interactions::NetnRequestConvoy& request );
    virtual void Receive( interactions::NetnAcceptOffer& accept );
    virtual void Receive( interactions::NetnReadyToReceiveService& readyToReceive );
    virtual void Receive( interactions::NetnServiceReceived& serviceReceived );
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const sword::UnitAttributes& message, int context );
    virtual void Notify( const sword::UnitOrder& message, int context );
    virtual void Notify( const sword::ControlEndTick& message, int context );
    //@}

private:
    //! @name Helpers
    //@{
    void HandleConvoyStatus( const sword::UnitAttributes& message );
    void HandleTransporterDeath( const sword::UnitAttributes& message );
    void SendEmbarkmentStatus( unsigned int transporter, unsigned int transported );
    void SendDisembarkmentStatus( unsigned int transporter, unsigned int untransported );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, interactions::NetnOfferConvoy > T_Offers;
    typedef boost::bimap< unsigned int, std::string > T_Transporters;
    typedef std::set< unsigned int > T_Transported;
    typedef std::map< unsigned int, T_Transported > T_TransportedList;
    typedef std::map< std::string, T_Transported > T_RemainingTransportedList;
    typedef std::map< unsigned int, unsigned int > T_TransportedBy;
    //@}

private:
    //! @name Member data
    //@{
    InteractionSender_ABC< interactions::NetnOfferConvoy >& offerInteractionSender_;
    InteractionSender_ABC< interactions::NetnServiceStarted >& serviceStartedInteractionSender_;
    InteractionSender_ABC< interactions::NetnConvoyEmbarkmentStatus >& convoyEmbarkmentStatusSender_;
    InteractionSender_ABC< interactions::NetnConvoyDisembarkmentStatus >& convoyDisembarkmentStatusSender_;
    InteractionSender_ABC< interactions::NetnConvoyDestroyedEntities >& convoyDestroyedEntitiesSender_;
    InteractionSender_ABC< interactions::NetnServiceComplete >& serviceCompleteSender_;
    tools::MessageController_ABC< sword::SimToClient_Content >& messageController_;
    const ContextFactory_ABC& factory_;
    const CallsignResolver_ABC& callsignResolver_;
    dispatcher::ClientPublisher_ABC& clientsPublisher_;
    const unsigned int transportIdentifier_;
    const unsigned int missionCompleteReportId_;
    T_Offers pendingOffers_;
    T_Offers offeredOffers_;
    T_Offers acceptedOffers_;
    T_Offers startedOffers_;
    T_Offers completetedOffers_;
    T_Offers receivedOffers_;
    T_Transporters transporters_;
    T_TransportedList transported_;
    T_RemainingTransportedList remainingTransported_;
    T_TransportedBy transportedBy_;
    T_TransportedList droppedThisTick_;
    //@}
};

}
}

#endif // plugins_hla_TransportationOfferer_h

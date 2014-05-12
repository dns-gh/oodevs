// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_TransportationFacade_h
#define plugins_hla_TransportationFacade_h

#include <hla/InteractionNotification_ABC.h>
#include <memory>

namespace xml
{
    class xisubstream;
}

namespace tools
{
    template< typename Category > class MessageController_ABC;
}

namespace sword
{
    class SimToClient_Content;
}

namespace dispatcher
{
    class SimulationPublisher_ABC;
    class ClientPublisher_ABC;
    class Logger_ABC;
}

namespace hla
{
    template< typename T > class Interaction;
    template< typename T > class InteractionNotification_ABC;
}

namespace plugins
{
namespace hla
{
    class MissionResolver_ABC;
    class CallsignResolver_ABC;
    class Subordinates_ABC;
    class InteractionBuilder;
    class ContextFactory_ABC;
    class TransportationRequester_ABC;
    template< typename T > class InteractionSender_ABC;
    class TransportationOfferer;
    class TransportationRequester;

namespace interactions
{
    struct NetnRequestConvoy;
    struct NetnOfferConvoy;
    struct NetnAcceptOffer;
    struct NetnRejectOfferConvoy;
    struct NetnCancelConvoy;
    struct NetnReadyToReceiveService;
    struct NetnServiceStarted;
    struct NetnConvoyEmbarkmentStatus;
    struct NetnConvoyDisembarkmentStatus;
    struct NetnConvoyDestroyedEntities;
    struct NetnServiceComplete;
    struct NetnServiceReceived;
}

// =============================================================================
/** @class  TransportationFacade
    @brief  Transportation facade
*/
// Created: SLI 2011-10-10
// =============================================================================
class TransportationFacade : private ::hla::InteractionNotification_ABC< interactions::NetnRequestConvoy >
                           , private ::hla::InteractionNotification_ABC< interactions::NetnOfferConvoy >
                           , private ::hla::InteractionNotification_ABC< interactions::NetnAcceptOffer >
                           , private ::hla::InteractionNotification_ABC< interactions::NetnRejectOfferConvoy >
                           , private ::hla::InteractionNotification_ABC< interactions::NetnCancelConvoy >
                           , private ::hla::InteractionNotification_ABC< interactions::NetnReadyToReceiveService >
                           , private ::hla::InteractionNotification_ABC< interactions::NetnServiceStarted >
                           , private ::hla::InteractionNotification_ABC< interactions::NetnConvoyEmbarkmentStatus >
                           , private ::hla::InteractionNotification_ABC< interactions::NetnConvoyDisembarkmentStatus >
                           , private ::hla::InteractionNotification_ABC< interactions::NetnConvoyDestroyedEntities >
                           , private ::hla::InteractionNotification_ABC< interactions::NetnServiceComplete >
                           , private ::hla::InteractionNotification_ABC< interactions::NetnServiceReceived >
{
public:
    //! @name Constructors/Destructor
    //@{
             TransportationFacade( dispatcher::Logger_ABC& logger, xml::xisubstream xis, const MissionResolver_ABC& missionResolver,
                                   tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                   const CallsignResolver_ABC& callsignResolver, const Subordinates_ABC& subordinates,
                                   const InteractionBuilder& builder, const ContextFactory_ABC& contextFactory,
                                   dispatcher::SimulationPublisher_ABC& simulationPublisher, dispatcher::ClientPublisher_ABC& clientsPublisher );
    virtual ~TransportationFacade();
    //@}

private:
    //! @name Receive
    //@{
   virtual void Receive( interactions::NetnRequestConvoy& interaction );
   virtual void Receive( interactions::NetnOfferConvoy& interaction );
   virtual void Receive( interactions::NetnAcceptOffer& interaction );
   virtual void Receive( interactions::NetnRejectOfferConvoy& interaction );
   virtual void Receive( interactions::NetnCancelConvoy& interaction );
   virtual void Receive( interactions::NetnReadyToReceiveService& interaction );
   virtual void Receive( interactions::NetnServiceStarted& interaction );
   virtual void Receive( interactions::NetnConvoyEmbarkmentStatus& interaction );
   virtual void Receive( interactions::NetnConvoyDisembarkmentStatus& interaction );
   virtual void Receive( interactions::NetnConvoyDestroyedEntities& interaction );
   virtual void Receive( interactions::NetnServiceComplete& interaction );
   virtual void Receive( interactions::NetnServiceReceived& interaction );
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< InteractionSender_ABC< interactions::NetnRequestConvoy > > pNetnRequestConvoy_;
    std::unique_ptr< InteractionSender_ABC< interactions::NetnOfferConvoy > > pNetnOfferConvoy_;
    std::unique_ptr< InteractionSender_ABC< interactions::NetnAcceptOffer > > pNetnAcceptOffer_;
    std::unique_ptr< InteractionSender_ABC< interactions::NetnRejectOfferConvoy > > pNetnRejectOfferConvoy_;
    std::unique_ptr< InteractionSender_ABC< interactions::NetnCancelConvoy > > pNetnCancelConvoy_;
    std::unique_ptr< InteractionSender_ABC< interactions::NetnReadyToReceiveService > > pNetnReadyToReceiveService_;
    std::unique_ptr< InteractionSender_ABC< interactions::NetnServiceStarted > > pNetnServiceStarted_;
    std::unique_ptr< InteractionSender_ABC< interactions::NetnConvoyEmbarkmentStatus > > pNetnConvoyEmbarkmentStatus_;
    std::unique_ptr< InteractionSender_ABC< interactions::NetnConvoyDisembarkmentStatus > > pNetnConvoyDisembarkmentStatus_;
    std::unique_ptr< InteractionSender_ABC< interactions::NetnConvoyDestroyedEntities > > pNetnConvoyDestroyedEntities_;
    std::unique_ptr< InteractionSender_ABC< interactions::NetnServiceComplete > > pNetnServiceComplete_;
    std::unique_ptr< InteractionSender_ABC< interactions::NetnServiceReceived > > pNetnServiceReceived_;
    std::unique_ptr< TransportationRequester > pTransportationRequester_;
    std::unique_ptr< TransportationOfferer > pTransportationOfferer_;
    //@}
};

}
}

#endif // plugins_hla_TransportationFacade_h

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

#include "InteractionSender_ABC.h"
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
    class Federate_ABC;
    class ContextFactory_ABC;
    class TransportationController_ABC;
    template< typename T > class InteractionSender_ABC;
    class NetnRequestConvoySender;
    class TransportationOfferer;
    class Transporters_ABC;
    class NetnOfferResponseSender;

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
                           , private InteractionSender_ABC< interactions::NetnRequestConvoy >
                           , private InteractionSender_ABC< interactions::NetnOfferConvoy >
                           , private InteractionSender_ABC< interactions::NetnAcceptOffer >
                           , private InteractionSender_ABC< interactions::NetnRejectOfferConvoy >
                           , private InteractionSender_ABC< interactions::NetnCancelConvoy >
                           , private InteractionSender_ABC< interactions::NetnReadyToReceiveService >
                           , private InteractionSender_ABC< interactions::NetnServiceStarted >
                           , private InteractionSender_ABC< interactions::NetnConvoyEmbarkmentStatus >
                           , private InteractionSender_ABC< interactions::NetnConvoyDisembarkmentStatus >
                           , private InteractionSender_ABC< interactions::NetnConvoyDestroyedEntities >
                           , private InteractionSender_ABC< interactions::NetnServiceComplete >
                           , private InteractionSender_ABC< interactions::NetnServiceReceived >
{
public:
    //! @name Constructors/Destructor
    //@{
             TransportationFacade( xml::xisubstream xis, const MissionResolver_ABC& missionResolver,
                                   tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                   const CallsignResolver_ABC& callsignResolver, const Subordinates_ABC& subordinates,
                                   Federate_ABC& federate, const ContextFactory_ABC& contextFactory, const Transporters_ABC& transporters,
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

    //! @name Send
    //@{
   virtual void Send( const interactions::NetnRequestConvoy& interaction );
   virtual void Send( const interactions::NetnOfferConvoy& interaction );
   virtual void Send( const interactions::NetnAcceptOffer& interaction );
   virtual void Send( const interactions::NetnRejectOfferConvoy& interaction );
   virtual void Send( const interactions::NetnCancelConvoy& interaction );
   virtual void Send( const interactions::NetnReadyToReceiveService& interaction );
   virtual void Send( const interactions::NetnServiceStarted& interaction );
   virtual void Send( const interactions::NetnConvoyEmbarkmentStatus& interaction );
   virtual void Send( const interactions::NetnConvoyDisembarkmentStatus& interaction );
   virtual void Send( const interactions::NetnConvoyDestroyedEntities& interaction );
   virtual void Send( const interactions::NetnServiceComplete& interaction );
   virtual void Send( const interactions::NetnServiceReceived& interaction );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ::hla::Interaction< interactions::NetnRequestConvoy > > pNetnRequestConvoy_;
    std::auto_ptr< ::hla::Interaction< interactions::NetnOfferConvoy > > pNetnOfferConvoy_;
    std::auto_ptr< ::hla::Interaction< interactions::NetnAcceptOffer > > pNetnAcceptOffer_;
    std::auto_ptr< ::hla::Interaction< interactions::NetnRejectOfferConvoy > > pNetnRejectOfferConvoy_;
    std::auto_ptr< ::hla::Interaction< interactions::NetnCancelConvoy > > pNetnCancelConvoy_;
    std::auto_ptr< ::hla::Interaction< interactions::NetnReadyToReceiveService > > pNetnReadyToReceiveService_;
    std::auto_ptr< ::hla::Interaction< interactions::NetnServiceStarted > > pNetnServiceStarted_;
    std::auto_ptr< ::hla::Interaction< interactions::NetnConvoyEmbarkmentStatus > > pNetnConvoyEmbarkmentStatus_;
    std::auto_ptr< ::hla::Interaction< interactions::NetnConvoyDisembarkmentStatus > > pNetnConvoyDisembarkmentStatus_;
    std::auto_ptr< ::hla::Interaction< interactions::NetnConvoyDestroyedEntities > > pNetnConvoyDestroyedEntities_;
    std::auto_ptr< ::hla::Interaction< interactions::NetnServiceComplete > > pNetnServiceComplete_;
    std::auto_ptr< ::hla::Interaction< interactions::NetnServiceReceived > > pNetnServiceReceived_;
    std::auto_ptr< TransportationController_ABC > pTransportationController_;
    std::auto_ptr< NetnRequestConvoySender > pNetnRequestConvoySender_;
    std::auto_ptr< ::hla::InteractionNotification_ABC< interactions::NetnOfferConvoy > > pNetnOfferConvoyReceiver_;
    std::auto_ptr< ::hla::InteractionNotification_ABC< interactions::NetnServiceStarted > > pNetnServiceStartedReceiver_;
    std::auto_ptr< ::hla::InteractionNotification_ABC< interactions::NetnConvoyEmbarkmentStatus > > pNetnConvoyEmbarkmentStatusReceiver_;
    std::auto_ptr< ::hla::InteractionNotification_ABC< interactions::NetnConvoyDisembarkmentStatus > > pNetnConvoyDisembarkmentStatusReceiver_;
    std::auto_ptr< TransportationOfferer > pTransportationOfferer_;
    std::auto_ptr< NetnOfferResponseSender > pNetnOfferResponseSender_;
    std::auto_ptr< ::hla::InteractionNotification_ABC< interactions::NetnServiceComplete > > pNetnServiceCompleteReceiver_;
    //@}
};

}
}

#endif // plugins_hla_TransportationFacade_h

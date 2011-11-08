// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_TransportationRequester_h
#define plugins_hla_TransportationRequester_h

#include "tools/MessageObserver.h"
#include "Interactions.h"
#include <hla/InteractionNotification_ABC.h>
#include <vector>
#include <map>
#include <geometry/Types.h>
#pragma warning( push, 0 )
#include <boost/bimap.hpp>
#pragma warning( pop )

namespace xml
{
    class xisubstream;
}

namespace dispatcher
{
    class SimulationPublisher_ABC;
}

namespace tools
{
    template< typename Category > class MessageController_ABC;
}

namespace sword
{
    class SimToClient_Content;
    class AutomatOrder;
    class Report;
}

namespace plugins
{
namespace hla
{
    class MissionResolver_ABC;
    class CallsignResolver_ABC;
    class Subordinates_ABC;
    class ContextFactory_ABC;
    class TransportedUnits_ABC;
    template< typename T > class InteractionSender_ABC;

// =============================================================================
/** @class  TransportationRequester
    @brief  Transportation requester
*/
// Created: SLI 2011-10-06
// =============================================================================
class TransportationRequester : private tools::MessageObserver< sword::AutomatOrder >
                              , private tools::MessageObserver< sword::Report >
                              , private ::hla::InteractionNotification_ABC< interactions::NetnOfferConvoy >
                              , private ::hla::InteractionNotification_ABC< interactions::NetnServiceStarted >
                              , private ::hla::InteractionNotification_ABC< interactions::NetnConvoyEmbarkmentStatus >
                              , private ::hla::InteractionNotification_ABC< interactions::NetnConvoyDisembarkmentStatus >
                              , private ::hla::InteractionNotification_ABC< interactions::NetnServiceComplete >
{
public:
    //! @name Constructors/Destructor
    //@{
             TransportationRequester( xml::xisubstream xis, const MissionResolver_ABC& missionResolver,
                                      tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                      const CallsignResolver_ABC& callsignResolver, const Subordinates_ABC& subordinates,
                                      const ContextFactory_ABC& contextFactory, dispatcher::SimulationPublisher_ABC& publisher,
                                      InteractionSender_ABC< interactions::NetnRequestConvoy >& requestSender,
                                      InteractionSender_ABC< interactions::NetnAcceptOffer >& acceptSender,
                                      InteractionSender_ABC< interactions::NetnRejectOfferConvoy >& rejectSender,
                                      InteractionSender_ABC< interactions::NetnReadyToReceiveService >& readySender,
                                      InteractionSender_ABC< interactions::NetnServiceReceived >& receivedSender );
    virtual ~TransportationRequester();
    //@}

public:
    //! @name Operations
    //@{
    virtual void Receive( interactions::NetnOfferConvoy& interaction );
    virtual void Receive( interactions::NetnServiceStarted& interaction );
    virtual void Receive( interactions::NetnConvoyEmbarkmentStatus& interaction );
    virtual void Receive( interactions::NetnConvoyDisembarkmentStatus& interaction );
    virtual void Receive( interactions::NetnServiceComplete& interaction );
    virtual void Receive( interactions::NetnConvoyDestroyedEntities& interaction );
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const sword::AutomatOrder& message, int context );
    virtual void Notify( const sword::Report& message, int context );
    //@}

private:
    //! @name Types
    //@{
    typedef boost::bimap< unsigned int, unsigned int > T_Requests;
    typedef std::map< unsigned int, interactions::NetnOfferConvoy > T_ContextRequests;
    //@}

private:
    //! @name Helpers
    //@{
    void SendTransportMagicAction( unsigned int context, const std::string& transporterCallsign, const interactions::ListOfUnits& units, unsigned int actionType, bool teleport );
    void Pause( unsigned int automat );
    void Resume( unsigned int automat );
    void Cancel( unsigned int automat );
    //@}

private:
    //! @name Member data
    //@{
    const unsigned int transportIdentifier_;
    const unsigned int missionCompleteReportId_;
    const unsigned int pauseId_;
    const unsigned int resumeId_;
    const unsigned int cancelId_;
    const CallsignResolver_ABC& callsignResolver_;
    const Subordinates_ABC& subordinates_;
    const ContextFactory_ABC& contextFactory_;
    dispatcher::SimulationPublisher_ABC& publisher_;
    InteractionSender_ABC< interactions::NetnRequestConvoy >& requestSender_;
    InteractionSender_ABC< interactions::NetnAcceptOffer >& acceptSender_;
    InteractionSender_ABC< interactions::NetnRejectOfferConvoy >& rejectSender_;
    InteractionSender_ABC< interactions::NetnReadyToReceiveService >& readySender_;
    InteractionSender_ABC< interactions::NetnServiceReceived >& receivedSender_;
    T_Requests pendingRequests_;
    T_Requests acceptedRequests_;
    T_Requests readyToReceiveRequests_;
    T_Requests serviceStartedRequests_;
    T_Requests completeRequests_;
    T_ContextRequests contextRequests_;
    //@}
};

}
}

#endif // plugins_hla_TransportationRequester_h

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnOfferConvoySender_h
#define plugins_hla_NetnOfferConvoySender_h

#include "Interactions.h"
#include "tools/MessageObserver.h"
#include <hla/InteractionNotification_ABC.h>
#include <set>
#include <map>

namespace sword
{
    class SimToClient_Content;
    class UnitAttributes;
}

namespace tools
{
    template< typename T > class MessageController_ABC;
}

namespace plugins
{
namespace hla
{
    template< typename T > class InteractionSender_ABC;
    class Transporters_ABC;
    class CallsignResolver_ABC;

namespace interactions
{
    struct NetnRequestConvoy;
    struct NetnOfferConvoy;
    struct NetnAcceptOffer;
    struct NetnReadyToReceiveService;
    struct NetnServiceStarted;
    struct NetnConvoyEmbarkmentStatus;
}

// =============================================================================
/** @class  NetnOfferConvoySender
    @brief  Netn offer convoy sender
*/
// Created: SLI 2011-10-11
// =============================================================================
class NetnOfferConvoySender : public ::hla::InteractionNotification_ABC< interactions::NetnRequestConvoy >
                            , public ::hla::InteractionNotification_ABC< interactions::NetnAcceptOffer >
                            , public ::hla::InteractionNotification_ABC< interactions::NetnReadyToReceiveService >
                            , public ::hla::InteractionNotification_ABC< interactions::NetnServiceReceived >
                            , public ::hla::InteractionNotification_ABC< interactions::NetnRejectOfferConvoy >
                            , public ::hla::InteractionNotification_ABC< interactions::NetnCancelConvoy >
                            , public ::hla::InteractionNotification_ABC< interactions::NetnConvoyDestroyedEntities >
                            , private tools::MessageObserver< sword::UnitAttributes >
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnOfferConvoySender( InteractionSender_ABC< interactions::NetnOfferConvoy >& offerInteractionSender,
                                    InteractionSender_ABC< interactions::NetnServiceStarted >& serviceStartedInteractionSender,
                                    InteractionSender_ABC< interactions::NetnConvoyEmbarkmentStatus >& convoyEmbarkmentStatusSender,
                                    const Transporters_ABC& transporters, tools::MessageController_ABC< sword::SimToClient_Content >& messageController,
                                    const CallsignResolver_ABC& callsignRevoler );
    virtual ~NetnOfferConvoySender();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( interactions::NetnRequestConvoy& request );
    virtual void Receive( interactions::NetnAcceptOffer& accept );
    virtual void Receive( interactions::NetnReadyToReceiveService& readyToReceive );
    virtual void Receive( interactions::NetnServiceReceived& received );
    virtual void Receive( interactions::NetnRejectOfferConvoy& rejected );
    virtual void Receive( interactions::NetnCancelConvoy& canceled );
    virtual void Receive( interactions::NetnConvoyDestroyedEntities& destroyed );
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const sword::UnitAttributes& message, int context );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< unsigned int > T_Offers;
    struct T_Transporter
    {
        std::string uniqueId;
        std::string callsign;
        interactions::NetnOfferConvoy offer;
    };
    typedef std::map< unsigned int, T_Transporter > T_Transporters;
    //@}

private:
    //! @name Helpers
    //@{
    void Transfer( T_Offers& from, T_Offers& to, unsigned int eventCount ) const;
    //@}

private:
    //! @name Member data
    //@{
    InteractionSender_ABC< interactions::NetnOfferConvoy >& offerInteractionSender_;
    InteractionSender_ABC< interactions::NetnServiceStarted >& serviceStartedInteractionSender_;
    InteractionSender_ABC< interactions::NetnConvoyEmbarkmentStatus >& convoyEmbarkmentStatusSender_;
    const Transporters_ABC& transporters_;
    tools::MessageController_ABC< sword::SimToClient_Content >& messageController_;
    const CallsignResolver_ABC& callsignRevoler_;
    T_Transporters pendingTransporters_;
    T_Offers pendingOffers_;
    T_Offers acceptedOffers_;
    T_Offers startedOffers_;
    T_Offers embarkedOffers_;
    //@}
};

}
}

#endif // plugins_hla_NetnOfferConvoySender_h

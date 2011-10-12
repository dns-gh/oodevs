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

#include <hla/InteractionNotification_ABC.h>
#include <set>

namespace plugins
{
namespace hla
{
    template< typename T > class InteractionSender_ABC;
    class Transporters_ABC;

namespace interactions
{
    struct NetnRequestConvoy;
    struct NetnOfferConvoy;
    struct NetnAcceptOffer;
    struct NetnReadyToReceiveService;
    struct NetnServiceStarted;
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
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnOfferConvoySender( InteractionSender_ABC< interactions::NetnOfferConvoy >& offerInteractionSender,
                                    InteractionSender_ABC< interactions::NetnServiceStarted >& serviceStartedInteractionSender,
                                    const Transporters_ABC& transporters );
    virtual ~NetnOfferConvoySender();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( interactions::NetnRequestConvoy& request );
    virtual void Receive( interactions::NetnAcceptOffer& accept );
    virtual void Receive( interactions::NetnReadyToReceiveService& readyToReceive );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< unsigned int > T_Offers;
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
    const Transporters_ABC& transporters_;
    T_Offers pendingOffers_;
    T_Offers acceptedOffers_;
    T_Offers startedOffers_;
    //@}
};

}
}

#endif // plugins_hla_NetnOfferConvoySender_h

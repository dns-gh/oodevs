// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnOfferResponseSender_h
#define plugins_hla_NetnOfferResponseSender_h

#include "TransportationListener_ABC.h"

namespace plugins
{
namespace hla
{
namespace interactions
{
    struct NetnAcceptOffer;
    struct NetnRejectOfferConvoy;
    struct NetnReadyToReceiveService;
    struct NetnServiceReceived;
}

    class TransportationController_ABC;
    template< typename T > class InteractionSender_ABC;

// =============================================================================
/** @class  NetnOfferResponseSender
    @brief  NetnOfferResponseSender
*/
// Created: SLI 2011-10-12
// =============================================================================
class NetnOfferResponseSender : private TransportationListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnOfferResponseSender( TransportationController_ABC& controller,
                                      InteractionSender_ABC< interactions::NetnAcceptOffer >& acceptOfferSender,
                                      InteractionSender_ABC< interactions::NetnRejectOfferConvoy >& rejectOfferSender,
                                      InteractionSender_ABC< interactions::NetnReadyToReceiveService >& readyToReceiveServiceSender,
                                      InteractionSender_ABC< interactions::NetnServiceReceived >& serviceReceivedSender );
    virtual ~NetnOfferResponseSender();
    //@}

private:
    //! @name Operations
    //@{
    virtual void ConvoyRequested( long long embarkmentTime, const geometry::Point2d& embarkmentPoint,
                                  long long disembarkmentTime, const geometry::Point2d& disembarkmentPoint,
                                  const TransportedUnits_ABC& transportedUnits, unsigned int context );
    virtual void OfferAccepted( unsigned int context, const std::string& provider );
    virtual void OfferRejected( unsigned int context, const std::string& provider, const std::string& reason );
    virtual void ReadyToReceiveService( unsigned int context, const std::string& provider );
    virtual void ServiceReceived( unsigned int context, const std::string& provider );
    //@}

private:
    //! @name Member data
    //@{
    TransportationController_ABC& controller_;
    InteractionSender_ABC< interactions::NetnAcceptOffer >& acceptOfferSender_;
    InteractionSender_ABC< interactions::NetnRejectOfferConvoy >& rejectOfferSender_;
    InteractionSender_ABC< interactions::NetnReadyToReceiveService >& readyToReceiveServiceSender_;
    InteractionSender_ABC< interactions::NetnServiceReceived >& serviceReceivedSender_;
    //@}
};

}
}

#endif // plugins_hla_NetnOfferResponseSender_h

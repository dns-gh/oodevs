// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NETN_InteractionBuilder_ABC_h
#define plugins_hla_NETN_InteractionBuilder_ABC_h

#include <boost/noncopyable.hpp>

namespace hla
{
    template <typename T> class Interaction;
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
    struct NetnRejectOfferConvoy;
    struct NetnCancelConvoy;
    struct NetnReadyToReceiveService;
    struct NetnServiceStarted;
    struct NetnConvoyEmbarkmentStatus;
    struct NetnConvoyDisembarkmentStatus;
    struct NetnConvoyDestroyedEntities;
    struct NetnServiceComplete;
    struct NetnServiceReceived;
    struct TMR_OfferTransferModellingResponsibility;
    struct TMR_InitiateTransferModellingResponsibility;
    struct TMR_RequestTransferModellingResponsibility;
    struct TMR_CancelRequest;
    struct TMR_TransferResult;
    struct MRM_DisaggregationRequest;
    struct MRM_DisaggregationResponse;
    struct MRM_AggregationRequest;
    struct MRM_AggregationResponse;
    struct MRM_CancelRequest;
    struct MRM_ActionComplete;
    struct MRM_Trigger;
    struct MRM_TriggerResponse;
}

// =============================================================================
/** @class  NETN_InteractionBuilder_ABC
    @brief  Transportation interaction builder
*/
// Created: AHC 2012-08-31
// =============================================================================
class NETN_InteractionBuilder_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    virtual ~NETN_InteractionBuilder_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool Build( ::hla::Interaction< interactions::NetnRequestConvoy >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::NetnOfferConvoy >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::NetnAcceptOffer >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::NetnRejectOfferConvoy >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::NetnCancelConvoy >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::NetnReadyToReceiveService >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::NetnServiceStarted >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::NetnConvoyEmbarkmentStatus >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::NetnConvoyDisembarkmentStatus >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::NetnConvoyDestroyedEntities >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::NetnServiceComplete >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::NetnServiceReceived >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::TMR_OfferTransferModellingResponsibility >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::TMR_InitiateTransferModellingResponsibility >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::TMR_RequestTransferModellingResponsibility >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::TMR_CancelRequest >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::TMR_TransferResult >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::MRM_DisaggregationRequest >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::MRM_DisaggregationResponse >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::MRM_AggregationRequest >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::MRM_AggregationResponse >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::MRM_CancelRequest >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::MRM_ActionComplete >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::MRM_Trigger >& interaction ) const = 0;
    virtual bool Build( ::hla::Interaction< interactions::MRM_TriggerResponse >& interaction ) const = 0;
    //@}
};

}
}
#endif // plugins_hla_NETN_InteractionBuilder_ABC_h

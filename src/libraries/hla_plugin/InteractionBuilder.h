// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_TransportationInteractionBuilder_h
#define plugins_hla_TransportationInteractionBuilder_h

#include <boost/noncopyable.hpp>

namespace hla
{
    template< typename T > class Interaction;
    class Interaction_ABC;
}

namespace dispatcher
{
    class Logger_ABC;
}

namespace plugins
{
namespace hla
{
namespace interactions
{
    struct Acknowledge;
    struct MunitionDetonation;
    struct ServiceRequest;
    struct ResupplyCancel;
    struct ResupplyOffer;
    struct ResupplyReceived;
    struct TransferControl;
    struct Comment;
    struct WeaponFire;
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
    class Federate_ABC;
    class NETN_InteractionBuilder_ABC;

// =============================================================================
/** @class  InteractionBuilder
    @brief  Transportation interaction builder
*/
// Created: SLI 2011-10-24
// =============================================================================
class InteractionBuilder : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             InteractionBuilder( dispatcher::Logger_ABC& logger, Federate_ABC& federate, const NETN_InteractionBuilder_ABC& netnBuilder );
    virtual ~InteractionBuilder();
    //@}

    //! @name Operations
    //@{
    //! @name RPR
    //@{
    bool Build( ::hla::Interaction< interactions::Acknowledge >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::MunitionDetonation >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::ServiceRequest >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::ResupplyCancel >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::ResupplyOffer >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::ResupplyReceived >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::TransferControl >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::Comment >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::WeaponFire >& interaction ) const;

    //@}
    //! @name NETN
    //@{
    bool Build( ::hla::Interaction< interactions::NetnRequestConvoy >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::NetnOfferConvoy >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::NetnAcceptOffer >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::NetnRejectOfferConvoy >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::NetnCancelConvoy >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::NetnReadyToReceiveService >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::NetnServiceStarted >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::NetnConvoyEmbarkmentStatus >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::NetnConvoyDisembarkmentStatus >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::NetnConvoyDestroyedEntities >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::NetnServiceComplete >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::NetnServiceReceived >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::TMR_OfferTransferModellingResponsibility >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::TMR_InitiateTransferModellingResponsibility >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::TMR_RequestTransferModellingResponsibility >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::TMR_CancelRequest >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::TMR_TransferResult >& interaction ) const;

    bool Build( ::hla::Interaction< interactions::MRM_DisaggregationRequest >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::MRM_DisaggregationResponse >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::MRM_AggregationRequest >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::MRM_AggregationResponse >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::MRM_CancelRequest >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::MRM_ActionComplete >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::MRM_Trigger >& interaction ) const;
    bool Build( ::hla::Interaction< interactions::MRM_TriggerResponse >& interaction ) const;
    //@}
    //@}

private:
    //! @name Operations
    //@{
    bool DoRegister(const std::string& name, ::hla::Interaction_ABC& interactionClass, bool publish, bool subscribe ) const;
    //@}
    //! @name Member data
    //@{
    dispatcher::Logger_ABC& logger_;
    Federate_ABC& federate_;
    const NETN_InteractionBuilder_ABC& netnBuilder_;
    //@}
};

}
}

#endif // TransportationInteractionBuilder_h

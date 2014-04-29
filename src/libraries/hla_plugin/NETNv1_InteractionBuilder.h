// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NETNV1_InteractionBuilder_H_
#define plugins_hla_NETNV1_InteractionBuilder_H_

#include "NETN_InteractionBuilder_ABC.h"
#include <string>

namespace hla
{
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
    class Federate_ABC;

/// =============================================================================
/// @class hla::NETNv1_InteractionBuilder
/// @brief hla::NETNv1_InteractionBuilder
/// @thread This type is not thread safe
///
/// Created: Antony 31 août 2012
/// =============================================================================
class NETNv1_InteractionBuilder : public NETN_InteractionBuilder_ABC
{
public:
    NETNv1_InteractionBuilder( dispatcher::Logger_ABC& logger, Federate_ABC& federate );
    virtual ~NETNv1_InteractionBuilder();

    //! @name Operations
    //@{
    virtual bool Build( ::hla::Interaction< interactions::NetnRequestConvoy >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::NetnOfferConvoy >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::NetnAcceptOffer >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::NetnRejectOfferConvoy >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::NetnCancelConvoy >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::NetnReadyToReceiveService >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::NetnServiceStarted >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::NetnConvoyEmbarkmentStatus >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::NetnConvoyDisembarkmentStatus >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::NetnConvoyDestroyedEntities >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::NetnServiceComplete >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::NetnServiceReceived >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::TMR_OfferTransferModellingResponsibility >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::TMR_InitiateTransferModellingResponsibility >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::TMR_RequestTransferModellingResponsibility >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::TMR_CancelRequest >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::TMR_TransferResult >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::MRM_DisaggregationRequest >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::MRM_DisaggregationResponse >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::MRM_AggregationRequest >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::MRM_AggregationResponse >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::MRM_CancelRequest >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::MRM_ActionComplete >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::MRM_Trigger >& interaction ) const;
    virtual bool Build( ::hla::Interaction< interactions::MRM_TriggerResponse >& interaction ) const;
    //@}

private:
    bool DoRegister( const std::string& name, ::hla::Interaction_ABC& interactionClass, bool publish, bool subscribe ) const;

private:
    dispatcher::Logger_ABC& logger_;
    Federate_ABC& federate_;
};

}
}
#endif // plugins_hla_NETNV1_InteractionBuilder_H_

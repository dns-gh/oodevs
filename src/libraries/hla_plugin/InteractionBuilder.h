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

namespace hla
{
    template< typename T > class Interaction;
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
}
    class Federate_ABC;

// =============================================================================
/** @class  InteractionBuilder
    @brief  Transportation interaction builder
*/
// Created: SLI 2011-10-24
// =============================================================================
class InteractionBuilder
{
public:
    //! @name Constructors/Destructor
    //@{
             InteractionBuilder();
    virtual ~InteractionBuilder();
    //@}

    //! @name Operations
    //@{
    void Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnRequestConvoy >& interaction ) const;
    void Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnOfferConvoy >& interaction ) const;
    void Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnAcceptOffer >& interaction ) const;
    void Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnRejectOfferConvoy >& interaction ) const;
    void Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnCancelConvoy >& interaction ) const;
    void Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnReadyToReceiveService >& interaction ) const;
    void Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnServiceStarted >& interaction ) const;
    void Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnConvoyEmbarkmentStatus >& interaction ) const;
    void Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnConvoyDisembarkmentStatus >& interaction ) const;
    void Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnConvoyDestroyedEntities >& interaction ) const;
    void Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnServiceComplete >& interaction ) const;
    void Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnServiceReceived >& interaction ) const;
    //@}
};

}
}

#endif // TransportationInteractionBuilder_h

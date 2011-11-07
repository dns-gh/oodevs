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
    struct MunitionDetonation;
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
class InteractionBuilder : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             InteractionBuilder( dispatcher::Logger_ABC& logger, Federate_ABC& federate );
    virtual ~InteractionBuilder();
    //@}

    //! @name Operations
    //@{
    void Build( ::hla::Interaction< interactions::MunitionDetonation >& interaction ) const;
    void Build( ::hla::Interaction< interactions::NetnRequestConvoy >& interaction ) const;
    void Build( ::hla::Interaction< interactions::NetnOfferConvoy >& interaction ) const;
    void Build( ::hla::Interaction< interactions::NetnAcceptOffer >& interaction ) const;
    void Build( ::hla::Interaction< interactions::NetnRejectOfferConvoy >& interaction ) const;
    void Build( ::hla::Interaction< interactions::NetnCancelConvoy >& interaction ) const;
    void Build( ::hla::Interaction< interactions::NetnReadyToReceiveService >& interaction ) const;
    void Build( ::hla::Interaction< interactions::NetnServiceStarted >& interaction ) const;
    void Build( ::hla::Interaction< interactions::NetnConvoyEmbarkmentStatus >& interaction ) const;
    void Build( ::hla::Interaction< interactions::NetnConvoyDisembarkmentStatus >& interaction ) const;
    void Build( ::hla::Interaction< interactions::NetnConvoyDestroyedEntities >& interaction ) const;
    void Build( ::hla::Interaction< interactions::NetnServiceComplete >& interaction ) const;
    void Build( ::hla::Interaction< interactions::NetnServiceReceived >& interaction ) const;
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Logger_ABC& logger_;
    Federate_ABC& federate_;
    //@}
};

}
}

#endif // TransportationInteractionBuilder_h

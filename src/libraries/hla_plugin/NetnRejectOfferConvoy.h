// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnRejectOfferConvoy_h
#define plugins_hla_NetnRejectOfferConvoy_h

#include "InteractionSender_ABC.h"
#include <hla/InteractionNotification_ABC.h>

namespace hla
{
    template< typename T > class Interaction;
    template< typename T > class InteractionNotification_ABC;
}

namespace plugins
{
namespace hla
{
    class Federate_ABC;

namespace interactions
{
    struct NetnRejectOfferConvoy;
}

// =============================================================================
/** @class  NetnRejectOfferConvoy
    @brief  Netn reject offer convoy
*/
// Created: SLI 2011-10-11
// =============================================================================
class NetnRejectOfferConvoy : public InteractionSender_ABC< interactions::NetnRejectOfferConvoy >
                            , private ::hla::InteractionNotification_ABC< interactions::NetnRejectOfferConvoy >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit NetnRejectOfferConvoy( Federate_ABC& federate );
    virtual ~NetnRejectOfferConvoy();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const interactions::NetnRejectOfferConvoy& interaction );
    //@}

private:
    //! @name Operations
    //@{
    virtual void Receive( interactions::NetnRejectOfferConvoy& interaction );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ::hla::Interaction< interactions::NetnRejectOfferConvoy > > pInteraction_;
    //@}
};

}
}

#endif // plugins_hla_NetnRejectOfferConvoy_h

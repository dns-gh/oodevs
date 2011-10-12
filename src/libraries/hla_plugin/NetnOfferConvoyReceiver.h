// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnOfferConvoyReceiver_h
#define plugins_hla_NetnOfferConvoyReceiver_h

#include <hla/InteractionNotification_ABC.h>

namespace plugins
{
namespace hla
{
namespace interactions
{
    struct NetnOfferConvoy;
}
    class TransportationController_ABC;

// =============================================================================
/** @class  NetnOfferConvoyReceiver
    @brief  Netn offer convoy receiver
*/
// Created: SLI 2011-10-12
// =============================================================================
class NetnOfferConvoyReceiver : public ::hla::InteractionNotification_ABC< interactions::NetnOfferConvoy >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit NetnOfferConvoyReceiver( TransportationController_ABC& controller );
    virtual ~NetnOfferConvoyReceiver();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( interactions::NetnOfferConvoy& interaction );
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    TransportationController_ABC& controller_;
    //@}
};

}
}

#endif // plugins_hla_NetnOfferConvoyReceiver_h
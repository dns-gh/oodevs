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
}

// =============================================================================
/** @class  NetnOfferConvoySender
    @brief  Netn offer convoy sender
*/
// Created: SLI 2011-10-11
// =============================================================================
class NetnOfferConvoySender : public ::hla::InteractionNotification_ABC< interactions::NetnRequestConvoy >
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnOfferConvoySender( InteractionSender_ABC< interactions::NetnOfferConvoy >& interactionSender,
                                    const Transporters_ABC& transporters );
    virtual ~NetnOfferConvoySender();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( interactions::NetnRequestConvoy& request );
    //@}

private:
    //! @name Member data
    //@{
    InteractionSender_ABC< interactions::NetnOfferConvoy >& interactionSender_;
    const Transporters_ABC& transporters_;
    //@}
};

}
}

#endif // plugins_hla_NetnOfferConvoySender_h

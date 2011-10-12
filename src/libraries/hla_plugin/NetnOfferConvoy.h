// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnOfferConvoy_h
#define plugins_hla_NetnOfferConvoy_h

#include "InteractionSender_ABC.h"

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
    struct NetnOfferConvoy;
}

// =============================================================================
/** @class  NetnOfferConvoy
    @brief  Netn offer convoy
*/
// Created: SLI 2011-10-11
// =============================================================================
class NetnOfferConvoy : public InteractionSender_ABC< interactions::NetnOfferConvoy >
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnOfferConvoy( Federate_ABC& federate, ::hla::InteractionNotification_ABC< interactions::NetnOfferConvoy >& notification );
    virtual ~NetnOfferConvoy();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const interactions::NetnOfferConvoy& interaction );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ::hla::Interaction< interactions::NetnOfferConvoy > > pInteraction_;
    //@}
};

}
}

#endif // plugins_hla_NetnOfferConvoy_h

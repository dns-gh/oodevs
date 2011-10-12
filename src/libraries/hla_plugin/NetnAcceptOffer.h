// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnAcceptOffer_h
#define plugins_hla_NetnAcceptOffer_h

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
    struct NetnAcceptOffer;
}
// =============================================================================
/** @class  NetnAcceptOffer
    @brief  NetnAcceptOffer
*/
// Created: VPR 2011-10-11
// =============================================================================
class NetnAcceptOffer : public InteractionSender_ABC< interactions::NetnAcceptOffer >
                      , private ::hla::InteractionNotification_ABC< interactions::NetnAcceptOffer >
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnAcceptOffer( Federate_ABC& federate );
    virtual ~NetnAcceptOffer();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const interactions::NetnAcceptOffer& interaction );
    //@}

private:
    //! @name Operations
    //@{
    virtual void Receive( interactions::NetnAcceptOffer& interaction );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ::hla::Interaction< interactions::NetnAcceptOffer > > pInteraction_;
    //@}
};

}
}

#endif // plugins_hla_NetnAcceptOffer_h

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnServiceComplete_h
#define plugins_hla_NetnServiceComplete_h

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
    struct NetnServiceComplete;
}
// =============================================================================
/** @class  NetnServiceComplete
    @brief  Netn service started
*/
// Created: VPR 2011-10-12
// =============================================================================
class NetnServiceComplete : public InteractionSender_ABC< interactions::NetnServiceComplete >
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnServiceComplete( Federate_ABC& federate, ::hla::InteractionNotification_ABC< interactions::NetnServiceComplete >& notification );
    virtual ~NetnServiceComplete();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const interactions::NetnServiceComplete& interaction );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ::hla::Interaction< interactions::NetnServiceComplete > > pInteraction_;
    //@}
};

}
}

#endif // plugins_hla_NetnServiceComplete_h

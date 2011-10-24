// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnServiceReceived_h
#define plugins_hla_NetnServiceReceived_h

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
    struct NetnServiceReceived;
}
// =============================================================================
/** @class  NetnServiceReceived
    @brief  Netn service started
*/
// Created: VPR 2011-10-12
// =============================================================================
class NetnServiceReceived : public InteractionSender_ABC< interactions::NetnServiceReceived >
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnServiceReceived( Federate_ABC& federate, ::hla::InteractionNotification_ABC< interactions::NetnServiceReceived >& notification );
    virtual ~NetnServiceReceived();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const interactions::NetnServiceReceived& interaction );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ::hla::Interaction< interactions::NetnServiceReceived > > pInteraction_;
    //@}
};

}
}

#endif // plugins_hla_NetnServiceReceived_h

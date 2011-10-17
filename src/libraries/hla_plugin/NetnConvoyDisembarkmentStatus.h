// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnConvoyDisembarkmentStatus_h
#define plugins_hla_NetnConvoyDisembarkmentStatus_h

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
    struct NetnConvoyDisembarkmentStatus;
}
// =============================================================================
/** @class  NetnConvoyDisembarkmentStatus
    @brief  Netn service started
*/
// Created: VPR 2011-10-12
// =============================================================================
class NetnConvoyDisembarkmentStatus : public InteractionSender_ABC< interactions::NetnConvoyDisembarkmentStatus >
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnConvoyDisembarkmentStatus( Federate_ABC& federate, ::hla::InteractionNotification_ABC< interactions::NetnConvoyDisembarkmentStatus >& notification );
    virtual ~NetnConvoyDisembarkmentStatus();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const interactions::NetnConvoyDisembarkmentStatus& interaction );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ::hla::Interaction< interactions::NetnConvoyDisembarkmentStatus > > pInteraction_;
    //@}
};

}
}

#endif // plugins_hla_NetnConvoyDisembarkmentStatus_h

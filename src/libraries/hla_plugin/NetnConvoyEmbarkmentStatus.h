// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnConvoyEmbarkmentStatus_h
#define plugins_hla_NetnConvoyEmbarkmentStatus_h

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
    struct NetnConvoyEmbarkmentStatus;
}

// =============================================================================
/** @class  NetnConvoyEmbarkmentStatus
    @brief  Netn convoy embarkment status
*/
// Created: SLI 2011-10-11
// =============================================================================
class NetnConvoyEmbarkmentStatus : public InteractionSender_ABC< interactions::NetnConvoyEmbarkmentStatus >
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnConvoyEmbarkmentStatus( Federate_ABC& federate, ::hla::InteractionNotification_ABC< interactions::NetnConvoyEmbarkmentStatus >& notification );
    virtual ~NetnConvoyEmbarkmentStatus();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const interactions::NetnConvoyEmbarkmentStatus& interaction );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ::hla::Interaction< interactions::NetnConvoyEmbarkmentStatus > > pInteraction_;
    //@}
};

}
}

#endif // plugins_hla_NetnConvoyEmbarkmentStatus_h

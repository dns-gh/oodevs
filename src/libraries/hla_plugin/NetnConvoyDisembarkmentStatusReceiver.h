// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnConvoyDisembarkmentStatusReceiver_h
#define plugins_hla_NetnConvoyDisembarkmentStatusReceiver_h

#include <hla/InteractionNotification_ABC.h>

namespace plugins
{
namespace hla
{
namespace interactions
{
    struct NetnConvoyDisembarkmentStatus;
}
    class TransportationRequester_ABC;

// =============================================================================
/** @class  NetnConvoyDisembarkmentStatusReceiver
    @brief  Netn convoy disembarkment status receiver
*/
// Created: SLI 2011-10-12
// =============================================================================
class NetnConvoyDisembarkmentStatusReceiver : public ::hla::InteractionNotification_ABC< interactions::NetnConvoyDisembarkmentStatus >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit NetnConvoyDisembarkmentStatusReceiver( TransportationRequester_ABC& controller );
    virtual ~NetnConvoyDisembarkmentStatusReceiver();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( interactions::NetnConvoyDisembarkmentStatus& interaction );
    //@}

private:
    //! @name Member data
    //@{
    TransportationRequester_ABC& controller_;
    //@}
};

}
}

#endif // plugins_hla_NetnConvoyDisembarkmentStatusReceiver_h

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnConvoyEmbarkmentStatusReceiver_h
#define plugins_hla_NetnConvoyEmbarkmentStatusReceiver_h

#include <hla/InteractionNotification_ABC.h>

namespace plugins
{
namespace hla
{
namespace interactions
{
    struct NetnConvoyEmbarkmentStatus;
}
    class TransportationController_ABC;

// =============================================================================
/** @class  NetnConvoyEmbarkmentStatusReceiver
    @brief  Netn offer convoy receiver
*/
// Created: SLI 2011-10-12
// =============================================================================
class NetnConvoyEmbarkmentStatusReceiver : public ::hla::InteractionNotification_ABC< interactions::NetnConvoyEmbarkmentStatus >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit NetnConvoyEmbarkmentStatusReceiver( TransportationController_ABC& controller );
    virtual ~NetnConvoyEmbarkmentStatusReceiver();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( interactions::NetnConvoyEmbarkmentStatus& interaction );
    //@}

private:
    //! @name Member data
    //@{
    TransportationController_ABC& controller_;
    //@}
};

}
}

#endif // plugins_hla_NetnConvoyEmbarkmentStatusReceiver_h
// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnServiceStartedReceiver_h
#define plugins_hla_NetnServiceStartedReceiver_h

#include <hla/InteractionNotification_ABC.h>

namespace plugins
{
namespace hla
{
namespace interactions
{
    struct NetnServiceStarted;
}
    class TransportationRequester_ABC;

// =============================================================================
/** @class  NetnServiceStartedReceiver
    @brief  Netn service started receiver
*/
// Created: SLI 2011-10-12
// =============================================================================
class NetnServiceStartedReceiver : public ::hla::InteractionNotification_ABC< interactions::NetnServiceStarted >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit NetnServiceStartedReceiver( TransportationRequester_ABC& controller );
    virtual ~NetnServiceStartedReceiver();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( interactions::NetnServiceStarted& interaction );
    //@}

private:
    //! @name Member data
    //@{
    TransportationRequester_ABC& controller_;
    //@}
};

}
}

#endif // plugins_hla_NetnServiceStartedReceiver_h
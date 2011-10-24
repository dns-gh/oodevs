// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnServiceCompleteReceiver_h
#define plugins_hla_NetnServiceCompleteReceiver_h

#include <hla/InteractionNotification_ABC.h>

namespace plugins
{
namespace hla
{
namespace interactions
{
    struct NetnServiceComplete;
}
    class TransportationRequester_ABC;

// =============================================================================
/** @class  NetnServiceCompleteReceiver
    @brief  Netn service started receiver
*/
// Created: SLI 2011-10-12
// =============================================================================
class NetnServiceCompleteReceiver : public ::hla::InteractionNotification_ABC< interactions::NetnServiceComplete >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit NetnServiceCompleteReceiver( TransportationRequester_ABC& controller );
    virtual ~NetnServiceCompleteReceiver();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( interactions::NetnServiceComplete& interaction );
    //@}

private:
    //! @name Member data
    //@{
    TransportationRequester_ABC& controller_;
    //@}
};

}
}

#endif // plugins_hla_NetnServiceCompleteReceiver_h

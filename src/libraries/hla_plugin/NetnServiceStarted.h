// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnServiceStarted_h
#define plugins_hla_NetnServiceStarted_h

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
    struct NetnServiceStarted;
}
// =============================================================================
/** @class  NetnServiceStarted
    @brief  NetnServiceStarted
*/
// Created: VPR 2011-10-12
// =============================================================================
class NetnServiceStarted : public InteractionSender_ABC< interactions::NetnServiceStarted >
                         , private ::hla::InteractionNotification_ABC< interactions::NetnServiceStarted >
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnServiceStarted( Federate_ABC& federate );
    virtual ~NetnServiceStarted();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const interactions::NetnServiceStarted& interaction );
    //@}

private:
    //! @name Operations
    //@{
    virtual void Receive( interactions::NetnServiceStarted& interaction );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ::hla::Interaction< interactions::NetnServiceStarted > > pInteraction_;
    //@}
};

}
}

#endif // plugins_hla_NetnServiceStarted_h

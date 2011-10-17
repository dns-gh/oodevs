// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnConvoyDestroyedEntities_h
#define plugins_hla_NetnConvoyDestroyedEntities_h

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
    struct NetnConvoyDestroyedEntities;
}
// =============================================================================
/** @class  NetnConvoyDestroyedEntities
    @brief  Netn service started
*/
// Created: VPR 2011-10-12
// =============================================================================
class NetnConvoyDestroyedEntities : public InteractionSender_ABC< interactions::NetnConvoyDestroyedEntities >
                                  , private ::hla::InteractionNotification_ABC< interactions::NetnConvoyDestroyedEntities >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit NetnConvoyDestroyedEntities( Federate_ABC& federate );
    virtual ~NetnConvoyDestroyedEntities();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const interactions::NetnConvoyDestroyedEntities& interaction );
    //@}

private:
    //! @name Operations
    //@{
    virtual void Receive( interactions::NetnConvoyDestroyedEntities& interaction );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ::hla::Interaction< interactions::NetnConvoyDestroyedEntities > > pInteraction_;
    //@}
};

}
}

#endif // plugins_hla_NetnConvoyDestroyedEntities_h

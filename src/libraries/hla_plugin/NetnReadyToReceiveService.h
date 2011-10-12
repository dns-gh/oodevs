// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnReadyToReceiveService_h
#define plugins_hla_NetnReadyToReceiveService_h

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
    struct NetnReadyToReceiveService;
}
// =============================================================================
/** @class  NetnReadyToReceiveService
    @brief  Netn ready to receive service
*/
// Created: VPR 2011-10-11
// =============================================================================
class NetnReadyToReceiveService : public InteractionSender_ABC< interactions::NetnReadyToReceiveService >
                                , private ::hla::InteractionNotification_ABC< interactions::NetnReadyToReceiveService >
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnReadyToReceiveService( Federate_ABC& federate );
    virtual ~NetnReadyToReceiveService();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const interactions::NetnReadyToReceiveService& interaction );
    //@}

private:
    //! @name Operations
    //@{
    virtual void Receive( interactions::NetnReadyToReceiveService& interaction );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ::hla::Interaction< interactions::NetnReadyToReceiveService > > pInteraction_;
    //@}
};

}
}

#endif // plugins_hla_NetnReadyToReceiveService_h

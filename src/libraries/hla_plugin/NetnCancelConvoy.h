// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnCancelConvoy_h
#define plugins_hla_NetnCancelConvoy_h

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
    struct NetnCancelConvoy;
}
// =============================================================================
/** @class  NetnCancelConvoy
    @brief  Netn service started
*/
// Created: VPR 2011-10-12
// =============================================================================
class NetnCancelConvoy : public InteractionSender_ABC< interactions::NetnCancelConvoy >
                       , private ::hla::InteractionNotification_ABC< interactions::NetnCancelConvoy >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit NetnCancelConvoy( Federate_ABC& federate );
    virtual ~NetnCancelConvoy();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const interactions::NetnCancelConvoy& interaction );
    //@}

private:
    virtual void Receive( interactions::NetnCancelConvoy& interaction );

private:
    //! @name Member data
    //@{
    std::auto_ptr< ::hla::Interaction< interactions::NetnCancelConvoy > > pInteraction_;
    //@}
};

}
}

#endif // plugins_hla_NetnCancelConvoy_h

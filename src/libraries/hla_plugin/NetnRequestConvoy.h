// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnRequestConvoy_h
#define plugins_hla_NetnRequestConvoy_h

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
    struct NetnRequestConvoy;
}

// =============================================================================
/** @class  NetnRequestConvoy
    @brief  Netn request convoy
*/
// Created: SLI 2011-10-06
// =============================================================================
class NetnRequestConvoy : public InteractionSender_ABC< interactions::NetnRequestConvoy >
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnRequestConvoy( Federate_ABC& federate, ::hla::InteractionNotification_ABC< interactions::NetnRequestConvoy >& notification );
    virtual ~NetnRequestConvoy();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const interactions::NetnRequestConvoy& interaction );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ::hla::Interaction< interactions::NetnRequestConvoy > > pInteraction_;
    //@}
};

}
}

#endif // plugins_hla_NetnRequestConvoy_h

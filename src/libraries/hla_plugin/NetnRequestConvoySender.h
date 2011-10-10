// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnRequestConvoySender_h
#define plugins_hla_NetnRequestConvoySender_h

#include "TransportationListener_ABC.h"

namespace plugins
{
namespace hla
{
    class TransportationController_ABC;
    template< typename Interaction > class InteractionSender_ABC;
    class ContextFactory_ABC;

namespace interactions
{
    struct NetnRequestConvoy;
}

// =============================================================================
/** @class  NetnRequestConvoySender
    @brief  Netn request convoy sender
*/
// Created: SLI 2011-10-07
// =============================================================================
class NetnRequestConvoySender : private TransportationListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnRequestConvoySender( TransportationController_ABC& controller,
                                      InteractionSender_ABC< interactions::NetnRequestConvoy >& interactionSender,
                                      const ContextFactory_ABC& contextFactory );
    virtual ~NetnRequestConvoySender();
    //@}

private:
    //! @name Operations
    //@{
    virtual void ConvoyRequested( const std::string& carrier, long long embarkmentTime, const geometry::Point2d& embarkmentPoint,
                                  long long disembarkmentTime, const geometry::Point2d& disembarkmentPoint,
                                  const TransportedUnits_ABC& transportedUnits );
    //@}

private:
    //! @name Member data
    //@{
    TransportationController_ABC& controller_;
    InteractionSender_ABC< interactions::NetnRequestConvoy >& interactionSender_;
    const ContextFactory_ABC& contextFactory_;
    //@}
};

}
}

#endif // plugins_hla_NetnRequestConvoySender_h

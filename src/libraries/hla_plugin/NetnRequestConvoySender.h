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
                                      InteractionSender_ABC< interactions::NetnRequestConvoy >& interactionSender );
    virtual ~NetnRequestConvoySender();
    //@}

private:
    //! @name Operations
    //@{
    virtual void ConvoyRequested( long long embarkmentTime, const geometry::Point2d& embarkmentPoint,
                                  long long disembarkmentTime, const geometry::Point2d& disembarkmentPoint,
                                  const TransportedUnits_ABC& transportedUnits, unsigned int context );
    virtual void OfferAccepted( unsigned int context, const std::string& provider );
    virtual void OfferRejected( unsigned int context, const std::string& provider, const std::string& reason );
    virtual void ReadyToReceiveService( unsigned int context, const std::string& provider );
    virtual void ServiceReceived( unsigned int context, const std::string& provider );
    //@}

private:
    //! @name Member data
    //@{
    TransportationController_ABC& controller_;
    InteractionSender_ABC< interactions::NetnRequestConvoy >& interactionSender_;
    //@}
};

}
}

#endif // plugins_hla_NetnRequestConvoySender_h

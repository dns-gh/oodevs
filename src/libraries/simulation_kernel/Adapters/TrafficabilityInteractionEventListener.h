// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef SWORD_TRAFFICABILITY_INTERACTION_EVENT_LISTENER_H
#define SWORD_TRAFFICABILITY_INTERACTION_EVENT_LISTENER_H

#include "EventListenerBase.h"

namespace core
{
    class Model;
}

namespace sword
{
// =============================================================================
/** @class  TrafficabilityInteractionEventListener
    @brief  Trafficability interaction event listener
*/
// Created: MMC 2013-04-16
// =============================================================================
class TrafficabilityInteractionEventListener : public EventListenerBase
{
public:
    //! @name Constructors/Destructor
    //@{
             TrafficabilityInteractionEventListener( const core::Model& model, core::Facade& facade );
    virtual ~TrafficabilityInteractionEventListener();
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const core::Model& callback );
    //@}

private:
    //! @name Member data
    //@{
    const core::Model& model_;
    //@}
};

}

#endif // SWORD_TRAFFICABILITY_INTERACTION_EVENT_LISTENER_H

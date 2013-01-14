// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_CALLBACK_EVENT_LISTENER_H
#define SWORD_PERCEPTION_CALLBACK_EVENT_LISTENER_H

#include "EventListenerBase.h"

namespace sword
{
// =============================================================================
/** @class  PerceptionCallbackEventListener
    @brief  Perception callback event listener
*/
// Created: SLI 2012-08-22
// =============================================================================
class PerceptionCallbackEventListener : public EventListenerBase
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PerceptionCallbackEventListener( core::Facade& facade );
    virtual ~PerceptionCallbackEventListener();
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const core::Model& callback );
    //@}
};

}

#endif // SWORD_PERCEPTION_CALLBACK_EVENT_LISTENER_H

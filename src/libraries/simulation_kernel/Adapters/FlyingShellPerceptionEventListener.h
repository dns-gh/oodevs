// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_FLYING_SHELL_PERCEPTION_EVENT_LISTENER_H
#define SWORD_FLYING_SHELL_PERCEPTION_EVENT_LISTENER_H

#include "EventListenerBase.h"

namespace sword
{
// =============================================================================
/** @class  FlyingShellPerceptionEventListener
    @brief  Flying shell perception event listener
*/
// Created: MCO 2012-06-15
// =============================================================================
class FlyingShellPerceptionEventListener : public EventListenerBase
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FlyingShellPerceptionEventListener( core::Facade& facade );
    virtual ~FlyingShellPerceptionEventListener();
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const core::Model& perception );
    //@}
};

}

#endif // SWORD_FLYING_SHELL_PERCEPTION_EVENT_LISTENER_H

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_DIRECT_FIRE_PERCEPTION_EVENT_LISTENER_H
#define SWORD_DIRECT_FIRE_PERCEPTION_EVENT_LISTENER_H

#include "EventListenerBase.h"

namespace sword
{
// =============================================================================
/** @class  DirectFirePerceptionEventListener
    @brief  Direct fire perception event listener
*/
// Created: MCO 2012-01-26
// =============================================================================
class DirectFirePerceptionEventListener : public EventListenerBase
{
public:
    //! @name Constructor/Destructor
    //@{
    explicit DirectFirePerceptionEventListener( core::Facade& facade );
    virtual ~DirectFirePerceptionEventListener();
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const core::Model& event );
    //@}
};

}

#endif // SWORD_DIRECT_FIRE_PERCEPTION_EVENT_LISTENER_H

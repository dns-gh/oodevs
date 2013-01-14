// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_EXTERNAL_PERCEPTION_EVENT_LISTENER_H
#define SWORD_EXTERNAL_PERCEPTION_EVENT_LISTENER_H

#include "EventListenerBase.h"

namespace sword
{
// =============================================================================
/** @class  ExternalPerceptionEventListener
    @brief  External perception event listener
*/
// Created: MCO 2012-01-26
// =============================================================================
class ExternalPerceptionEventListener : public EventListenerBase
{
public:
    //! @name Constructor/Destructor
    //@{
    explicit ExternalPerceptionEventListener( core::Facade& facade );
    virtual ~ExternalPerceptionEventListener();
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const core::Model& event );
    //@}
};

}

#endif // SWORD_EXTERNAL_PERCEPTION_EVENT_LISTENER_H

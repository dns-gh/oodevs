// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_DIRECT_FIRE_PION_ATTACK_EVENT_LISTENER_H
#define SWORD_DIRECT_FIRE_PION_ATTACK_EVENT_LISTENER_H

#include "EventListenerBase.h"
#include "tools/Resolver.h"

class MIL_AgentPion;

namespace sword
{
// =============================================================================
/** @class  DirectFirePionAttackEventListener
    @brief  Direct fire pion attack event listener
*/
// Created: MCO 2012-01-26
// =============================================================================
class DirectFirePionAttackEventListener : public EventListenerBase
{
public:
    //! @name Constructor/Destructor
    //@{
    explicit DirectFirePionAttackEventListener( core::Facade& facade );
    virtual ~DirectFirePionAttackEventListener();
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const core::Model& event );
    //@}
};

}

#endif // SWORD_DIRECT_FIRE_PION_ATTACK_EVENT_LISTENER_H

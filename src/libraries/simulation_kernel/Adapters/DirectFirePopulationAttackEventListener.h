// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_DIRECT_FIRE_POPULATION_ATTACK_EVENT_LISTENER_H
#define SWORD_DIRECT_FIRE_POPULATION_ATTACK_EVENT_LISTENER_H

#include "EventListenerBase.h"

namespace sword
{
// =============================================================================
/** @class  DirectFirePopulationAttackEventListener
    @brief  Direct fire population attack event listener
*/
// Created: MCO  2012-09-21
// =============================================================================
class DirectFirePopulationAttackEventListener : public EventListenerBase
{
public:
    //! @name Constructor/Destructor
    //@{
    explicit DirectFirePopulationAttackEventListener( core::Facade& facade );
    virtual ~DirectFirePopulationAttackEventListener();
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const core::Model& event );
    //@}
};

}

#endif // SWORD_DIRECT_FIRE_POPULATION_ATTACK_EVENT_LISTENER_H

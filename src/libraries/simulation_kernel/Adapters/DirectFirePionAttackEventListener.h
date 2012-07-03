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

#include "tools/Resolver.h"
#include <core/EventListener_ABC.h>

class MIL_AgentPion;

namespace core
{
    class Facade;
}

namespace sword
{
// =============================================================================
/** @class  DirectFirePionAttackEventListener
    @brief  Direct fire pion attack event listener
*/
// Created: MCO 2012-01-26
// =============================================================================
class DirectFirePionAttackEventListener : public core::EventListener_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             DirectFirePionAttackEventListener( const core::Model& model, core::Facade& facade, tools::Resolver< MIL_AgentPion >& resolver );
    virtual ~DirectFirePionAttackEventListener();
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const core::Model& event );
    //@}

private:
    //! @name Member data
    //@{
    const core::Model& model_;
    core::Facade& facade_;
    tools::Resolver< MIL_AgentPion >& resolver_;
    //@}
};

}

#endif // SWORD_DIRECT_FIRE_PION_ATTACK_EVENT_LISTENER_H

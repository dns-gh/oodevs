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

#include <core/EventListener_ABC.h>

namespace core
{
    class Facade;
}

namespace sword
{
// =============================================================================
/** @class  DirectFirePopulationAttackEventListener
    @brief  Direct fire population attack event listener
*/
// Created: MCO  2012-09-21
// =============================================================================
class DirectFirePopulationAttackEventListener : public core::EventListener_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             DirectFirePopulationAttackEventListener( const core::Model& model, core::Facade& facade );
    virtual ~DirectFirePopulationAttackEventListener();
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
    //@}
};

}

#endif // SWORD_DIRECT_FIRE_POPULATION_ATTACK_EVENT_LISTENER_H

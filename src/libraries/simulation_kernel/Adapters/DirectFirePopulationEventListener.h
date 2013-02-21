// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_DIRECT_FIRE_POPULATION_EVENT_LISTENER_H
#define SWORD_DIRECT_FIRE_POPULATION_EVENT_LISTENER_H

#include "EventListenerBase.h"
#include <map>

class PHY_FireResults_Pion;

namespace sword
{
// =============================================================================
/** @class  DirectFirePopulationEventListener
    @brief  Direct fire population event listener
*/
// Created: MCO 2012-01-26
// =============================================================================
class DirectFirePopulationEventListener : public EventListenerBase
{
public:
    //! @name Constructor/Destructor
    //@{
    explicit DirectFirePopulationEventListener( core::Facade& facade );
    virtual ~DirectFirePopulationEventListener();
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const core::Model& event );
    //@}

private:
    //! @name Helpers
    //@{
    void Update( const core::Model& event );
    void Remove( const core::Model& event );
    //@}

private:
    //! @name Member data
    //@{
    std::map< unsigned int, PHY_FireResults_Pion* > results_;
    //@}
};

}

#endif // SWORD_DIRECT_FIRE_POPULATION_EVENT_LISTENER_H

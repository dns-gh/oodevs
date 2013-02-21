// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_DIRECT_FIRE_PION_EVENT_LISTENER_H
#define SWORD_DIRECT_FIRE_PION_EVENT_LISTENER_H

#include "EventListenerBase.h"
#include <map>

class PHY_FireResults_Pion;

namespace sword
{
// =============================================================================
/** @class  DirectFirePionEventListener
    @brief  Direct fire pion event listener
*/
// Created: MCO 2012-01-26
// =============================================================================
class DirectFirePionEventListener : public EventListenerBase
{
public:
    //! @name Constructor/Destructor
    //@{
    explicit DirectFirePionEventListener( core::Facade& facade );
    virtual ~DirectFirePionEventListener();
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

#endif // SWORD_DIRECT_FIRE_PION_EVENT_LISTENER_H

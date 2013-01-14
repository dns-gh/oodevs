// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_MOVEMENT_EVENT_LISTENER_H
#define SWORD_MOVEMENT_EVENT_LISTENER_H

#include "EventListenerBase.h"

namespace core
{
    class Model;
}

namespace sword
{
// =============================================================================
/** @class  MovementEventListener
    @brief  Movement event listener
*/
// Created: MCO 2012-06-15
// =============================================================================
class MovementEventListener : public EventListenerBase
{
public:
    //! @name Constructors/Destructor
    //@{
             MovementEventListener( const core::Model& model, core::Facade& facade );
    virtual ~MovementEventListener();
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const core::Model& callback );
    //@}

private:
    //! @name Member data
    //@{
    const core::Model& model_;
    //@}
};

}

#endif // SWORD_MOVEMENT_EVENT_LISTENER_H

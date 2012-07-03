// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_MOVEMENT_CALLBACK_EVENT_LISTENER_H
#define SWORD_MOVEMENT_CALLBACK_EVENT_LISTENER_H

#include <core/EventListener_ABC.h>

namespace core
{
    class Model;
    class Facade;
}

namespace sword
{
// =============================================================================
/** @class  MovementCallbackEventListener
    @brief  Movement callback event listener
*/
// Created: MCO 2012-06-15
// =============================================================================
class MovementCallbackEventListener : public core::EventListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MovementCallbackEventListener( const core::Model& model, core::Facade& facade );
    virtual ~MovementCallbackEventListener();
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const core::Model& callback );
    //@}

private:
    //! @name Member data
    //@{
    const core::Model& model_;
    core::Facade& facade_;
    //@}
};

}

#endif // SWORD_MOVEMENT_CALLBACK_EVENT_LISTENER_H

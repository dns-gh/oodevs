// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_CALLBACK_EVENT_LISTENER_H
#define SWORD_CALLBACK_EVENT_LISTENER_H

#include "EventListenerBase.h"

namespace core
{
    class Model;
    class Facade;
}

namespace sword
{
// =============================================================================
/** @class  CallbackEventListener
    @brief  Callback event listener
*/
// Created: MCO 2012-09-24
// =============================================================================
class CallbackEventListener : public EventListenerBase
{
public:
    //! @name Constructors/Destructor
    //@{
             CallbackEventListener( const core::Model& model, core::Facade& facade, const std::string& event );
    virtual ~CallbackEventListener();
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

#endif // SWORD_CALLBACK_EVENT_LISTENER_H
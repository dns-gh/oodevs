// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef SWORD_EVENT_LISTENER_BASE_H
#define SWORD_EVENT_LISTENER_BASE_H

#include <core/EventListener_ABC.h>
#include <string>

namespace core
{
    class Facade;
}

namespace sword
{
// =============================================================================
/** @class  EventListenerBase
    @brief  EventListenerBase
*/
// Created: MCO 2013-01-14
// =============================================================================
class EventListenerBase : public core::EventListener_ABC
{
protected:
    //! @name Constructors/Destructor
    //@{
             EventListenerBase( core::Facade& facade, const std::string& event );
    virtual ~EventListenerBase();
    //@}

private:
    //! @name Member data
    //@{
    core::Facade& facade_;
    const std::string event_;
    //@}
};

}

#endif // SWORD_EVENT_LISTENER_BASE_H

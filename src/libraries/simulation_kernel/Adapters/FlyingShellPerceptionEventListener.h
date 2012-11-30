// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_FLYING_SHELL_PERCEPTION_EVENT_LISTENER_H
#define SWORD_FLYING_SHELL_PERCEPTION_EVENT_LISTENER_H

#include <core/EventListener_ABC.h>

namespace core
{
    class Facade;
}

namespace sword
{
// =============================================================================
/** @class  FlyingShellPerceptionEventListener
    @brief  Flying shell perception event listener
*/
// Created: MCO 2012-06-15
// =============================================================================
class FlyingShellPerceptionEventListener : public core::EventListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FlyingShellPerceptionEventListener( core::Facade& facade );
    virtual ~FlyingShellPerceptionEventListener();
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const core::Model& perception );
    //@}

private:
    //! @name Member data
    //@{
    core::Facade& facade_;
    //@}
};

}

#endif // SWORD_FLYING_SHELL_PERCEPTION_EVENT_LISTENER_H

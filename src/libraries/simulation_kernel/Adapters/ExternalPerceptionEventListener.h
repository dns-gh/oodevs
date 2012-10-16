// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_EXTERNAL_PERCEPTION_EVENT_LISTENER_H
#define SWORD_EXTERNAL_PERCEPTION_EVENT_LISTENER_H

#include <core/EventListener_ABC.h>

namespace core
{
    class Facade;
}

namespace sword
{
// =============================================================================
/** @class  ExternalPerceptionEventListener
    @brief  Direct fire pion attack event listener
*/
// Created: MCO 2012-01-26
// =============================================================================
class ExternalPerceptionEventListener : public core::EventListener_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
    explicit ExternalPerceptionEventListener( core::Facade& facade );
    virtual ~ExternalPerceptionEventListener();
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const core::Model& event );
    //@}

private:
    //! @name Member data
    //@{
    core::Facade& facade_;
    //@}
};

}

#endif // SWORD_EXTERNAL_PERCEPTION_EVENT_LISTENER_H

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_ALAT_MONITORING_EVENT_LISTENER_H
#define SWORD_ALAT_MONITORING_EVENT_LISTENER_H

#include "EventListenerBase.h"

namespace core
{
    class Facade;
}

namespace sword
{
// =============================================================================
/** @class  AlatMonitoringEventListener
    @brief  ALAT monitoring event listener
*/
// Created: MCO 2012-01-26
// =============================================================================
class AlatMonitoringEventListener : public EventListenerBase
{
public:
    //! @name Constructor/Destructor
    //@{
    explicit AlatMonitoringEventListener( core::Facade& facade );
    virtual ~AlatMonitoringEventListener();
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const core::Model& event );
    //@}
};

}

#endif // SWORD_ALAT_MONITORING_EVENT_LISTENER_H

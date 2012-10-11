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

#include <core/EventListener_ABC.h>

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
class AlatMonitoringEventListener : public core::EventListener_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             AlatMonitoringEventListener( const core::Model& model, core::Facade& facade );
    virtual ~AlatMonitoringEventListener();
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

#endif // SWORD_ALAT_MONITORING_EVENT_LISTENER_H

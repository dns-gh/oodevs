// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_MOVEMENT_REPORT_EVENT_LISTENER_H
#define SWORD_MOVEMENT_REPORT_EVENT_LISTENER_H

#include <core/EventListener_ABC.h>

namespace core
{
    class Facade;
}

namespace sword
{
// =============================================================================
/** @class  MovementReportEventListener
    @brief  Movement report event listener
*/
// Created: MCO 2012-06-15
// =============================================================================
class MovementReportEventListener : public core::EventListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MovementReportEventListener( core::Facade& facade );
    virtual ~MovementReportEventListener();
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const core::Model& report );
    //@}

private:
    //! @name Member data
    //@{
    core::Facade& facade_;
    //@}
};

}

#endif // SWORD_MOVEMENT_REPORT_EVENT_LISTENER_H

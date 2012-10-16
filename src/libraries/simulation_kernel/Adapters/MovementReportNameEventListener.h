// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_REPORT_NAME_EVENT_LISTENER_H
#define SWORD_REPORT_NAME_EVENT_LISTENER_H

#include <core/EventListener_ABC.h>

namespace core
{
    class Facade;
}

namespace sword
{
// =============================================================================
/** @class  MovementReportNameEventListener
    @brief  Movement report name event listener
*/
// Created: MCO 2012-06-15
// =============================================================================
class MovementReportNameEventListener : public core::EventListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MovementReportNameEventListener( core::Facade& facade );
    virtual ~MovementReportNameEventListener();
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

#endif // SWORD_REPORT_NAME_EVENT_LISTENER_H

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_REPORT_EVENT_LISTENER_H
#define SWORD_REPORT_EVENT_LISTENER_H

#include "EventListenerBase.h"

namespace sword
{
// =============================================================================
/** @class  ReportEventListener
    @brief  Report event listener
*/
// Created: MCO 2012-06-15
// =============================================================================
class ReportEventListener : public EventListenerBase
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ReportEventListener( core::Facade& facade );
    virtual ~ReportEventListener();
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const core::Model& report );
    //@}
};

}

#endif // SWORD_REPORT_EVENT_LISTENER_H

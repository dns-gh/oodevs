// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __EventMarker_h_
#define __EventMarker_h_

#include "Event.h"

namespace gui
{

// =============================================================================
/** @class  EventMarker
    @brief  EventMarker
*/
// Created: JSR 2014-10-24
// =============================================================================
class EventMarker : public Event
{

public:
    //! @name Constructors/Destructor
    //@{
             EventMarker( E_EventTypes type, const timeline::Event& event );
    virtual ~EventMarker();
    //@}

    //! @name Operations
    //@{
    virtual Event* Clone() const;
    //@}
};

} //! namespace gui

#endif // __EventMarker_h_

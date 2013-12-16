// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EventTaskView_ABC_h_
#define __EventTaskView_ABC_h_

#include "EventView_ABC.h"

namespace gui
{
    struct EventTaskViewState;

// =============================================================================
/** @class  EventTaskView_ABC
    @brief  Event task view declaration
*/
// Created: ABR 2013-10-03
// =============================================================================
class EventTaskView_ABC : public EventView_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             EventTaskView_ABC() {}
    virtual ~EventTaskView_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Build( const gui::EventTaskViewState& state ) = 0;
    //@}
};

} //! namespace gui

#endif // __EventTaskView_ABC_h_

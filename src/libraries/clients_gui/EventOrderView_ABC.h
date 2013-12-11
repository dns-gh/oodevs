// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EventOrderView_ABC_h_
#define __EventOrderView_ABC_h_

#include "EventView_ABC.h"

namespace gui
{
    struct EventOrderViewState;

// =============================================================================
/** @class  EventOrderView_ABC
    @brief  Event order view declaration
*/
// Created: LGY 2013-10-03
// =============================================================================
class EventOrderView_ABC : public EventView_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             EventOrderView_ABC() {}
    virtual ~EventOrderView_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Build( const gui::EventOrderViewState& state ) = 0;
    //@}
};

} //! namespace gui

#endif // __EventOrderView_ABC_h_

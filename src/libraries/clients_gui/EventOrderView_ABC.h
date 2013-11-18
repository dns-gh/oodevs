// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EventBuilder_ABC_h_
#define __EventBuilder_ABC_h_

namespace gui
{
    struct EventOrderViewState;

// =============================================================================
/** @class  EventOrderView_ABC
    @brief  Event order view declaration
*/
// Created: LGY 2013-10-03
// =============================================================================
class EventOrderView_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             EventOrderView_ABC() {}
    virtual ~EventOrderView_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Build( const EventOrderViewState& state ) = 0;

    virtual void UpdateActions() = 0;
    //@}
};

}

#endif // __EventBuilder_ABC_h_

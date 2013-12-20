// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventMagicViewState_h_
#define __EventMagicViewState_h_

#include <string>

namespace gui
{

// =============================================================================
/** @class  EventMagicViewState
    @brief  EventMagicViewState
*/
// Created: ABR 2013-12-17
// =============================================================================
struct EventMagicViewState
{
    //! @name Constructors/Destructor
    //@{
    EventMagicViewState()
        : target_( 0 )
        , hasTarget_( false )
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    void Purge()
    {
        *this = EventMagicViewState();
    }
    //@}

    //! @name Member data
    //@{
    std::string name_;
    unsigned long target_;
    bool hasTarget_;
    //@}
};

} //! namespace gui

#endif // __EventMagicViewState_h_

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __EventReplayViewState_h_
#define __EventReplayViewState_h_

namespace gui
{
// =============================================================================
/** @class  EventReplayViewState
    @brief  EventReplayViewState
*/
// Created: SLI 2014-11-05
// =============================================================================
struct EventReplayViewState
{
    //! @name Constructors/Destructor
    //@{
    EventReplayViewState()
    {
        // NOTHING
    }

    EventReplayViewState( const QString& label, const QString& description )
        : label_( label )
        , description_( description )
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    void Purge()
    {
        *this = EventReplayViewState();
    }
    //@}

    //! @name Member data
    //@{
    QString label_;
    QString description_;
    //@}
};

} //! namespace gui

#endif // __EventReplayViewState_h_

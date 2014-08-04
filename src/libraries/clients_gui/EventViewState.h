// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventViewState_h_
#define __EventViewState_h_

#include "Event.h"
#include "ENT/ENT_Enums.h"
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>

namespace gui
{

// =============================================================================
/** @class  EventViewState
    @brief  EventViewState
*/
// Created: ABR 2013-11-19
// =============================================================================
struct EventViewState
{
    //! @name Constructors
    //@{
    EventViewState()
        : mode_( eEventDockModes_None )
        , trigger_( false )
        , save_( false )
        , saveAs_( false )
        , clear_( false )
        , raise_( false )
        , detail_( false )
        , bottomToolBar_( false )
        , warningColor_( Qt::transparent )
    {
        // NOTHING
    }
    EventViewState( boost::shared_ptr< gui::Event > event,
                    E_EventDockModes mode = eEventDockModes_None,
                    bool trigger = false,
                    bool save = false,
                    bool saveAs = false,
                    bool clear = false,
                    bool raise = false,
                    bool detail = false,
                    bool bottomToolBar = false,
                    QColor warningColor = Qt::transparent,
                    std::string warning = "" )
        : event_( event )
        , mode_( mode )
        , trigger_( trigger )
        , save_( save )
        , saveAs_( saveAs )
        , clear_( clear )
        , raise_( raise )
        , detail_( detail )
        , bottomToolBar_( bottomToolBar )
        , warningColor_( warningColor )
        , warning_( warning )
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    void Purge()
    {
        *this = EventViewState();
    }
    //@}

    //! @name Member data
    //@{
    boost::shared_ptr< gui::Event > event_;
    E_EventDockModes mode_;
    bool trigger_;
    // Set on engagement errors
    // true: unit order when automat is engaged
    // false: automat order when automat is disengaged
    boost::optional< bool > misengaged_;
    bool save_;
    bool saveAs_;
    bool clear_;
    bool raise_;
    bool detail_;
    bool bottomToolBar_;
    QColor warningColor_;
    std::string warning_;
    //@}
};

} //! namespace gui

#endif // __EventViewState_h_

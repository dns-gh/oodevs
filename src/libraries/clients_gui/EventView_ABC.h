// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventView_ABC_h_
#define __EventView_ABC_h_

#include <boost/noncopyable.hpp>

namespace gui
{
    struct EventViewState;
    class Viewport_ABC;

// =============================================================================
/** @class  EventView_ABC
    @brief  EventView definition
*/
// Created: ABR 2013-11-19
// =============================================================================
class EventView_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             EventView_ABC() {}
    virtual ~EventView_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Purge() = 0;
    virtual void Build( const gui::EventViewState& state ) = 0;
    virtual void Update( const gui::EventViewState& state ) = 0;

    virtual void BlockSignals( bool blocked ) = 0;
    virtual void Draw( gui::Viewport_ABC& viewport ) = 0;
    //@}
};

} //! namespace gui

#endif // __EventView_ABC_h_

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
    struct EventMagicViewState;
    struct EventOrderViewState;
    struct EventTaskViewState;
    struct EventViewState;

    class Viewport_ABC;

class EventBaseView_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             EventBaseView_ABC() {}
    virtual ~EventBaseView_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Purge() = 0;
    virtual void BlockSignals( bool blocked ) = 0;
    virtual void Draw( gui::Viewport_ABC& viewport ) = 0;
    //@}
};

// =============================================================================
/** @class  EventView_ABC
    @brief  EventView definition
*/
// Created: ABR 2013-11-19
// =============================================================================
template< typename ViewState >
class EventView_ABC : public EventBaseView_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             EventView_ABC() {}
    virtual ~EventView_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Build( const ViewState& state ) = 0;
    //@}
};

typedef EventView_ABC< gui::EventViewState >                 EventDefaultView_ABC;
typedef EventView_ABC< gui::EventMagicViewState >            EventMagicView_ABC;
typedef EventView_ABC< gui::EventOrderViewState >            EventOrderView_ABC;
typedef EventView_ABC< gui::EventTaskViewState >             EventTaskView_ABC;

} //! namespace gui

#endif // __EventView_ABC_h_

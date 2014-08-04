// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventSubPresenter_ABC_h_
#define __EventSubPresenter_ABC_h_

#include "ENT/ENT_Enums.h"
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <boost/scoped_ptr.hpp>

namespace timeline
{
    struct Event;
}

namespace gui
{
    template< typename T > class EventView_ABC;
    class Event;
    class Viewport_ABC;

// =============================================================================
/** @class  EventPresenter_ABC
    @brief  EventPresenter_ABC
*/
// Created: ABR 2013-11-20
// =============================================================================
class EventPresenter_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit EventPresenter_ABC( E_EventTypes type )
        : type_( type )
    {
        // NOTHING
    }
    virtual ~EventPresenter_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool ShouldEnableTrigger() const
    {
        return false;
    }
    virtual boost::optional< bool > IsMisengaged() const
    {
        return boost::none;
    }
    virtual bool ShouldEnableClear() const
    {
        return false;
    }
    E_EventTypes GetType() const
    {
        return type_;
    }
    //@}

    //! @name Abstract operations
    //@{
    virtual void Trigger( const gui::Event& event ) = 0;
    virtual void FillFrom( const gui::Event& event ) = 0;
    virtual void CommitTo( timeline::Event& event ) const = 0;
    virtual void BuildView() = 0;
    virtual void Purge() = 0; // reset all
    virtual void Clear() = 0; // reset content
    virtual void Draw( gui::Viewport_ABC& viewPort ) const = 0;
    //@}

    //! @name Types
    //@{
    typedef boost::shared_ptr< EventPresenter_ABC > T_SharedPtr;
    //@}

private:
    //! @name Member data
    //@{
    E_EventTypes type_;
    //@}
};

// =============================================================================
/** @class  EventSubPresenter_ABC
    @brief  EventSubPresenter_ABC
*/
// Created: ABR 2013-11-20
// =============================================================================
template< typename ViewState >
class EventSubPresenter_ABC : public EventPresenter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit EventSubPresenter_ABC( E_EventTypes type, EventView_ABC< ViewState >& view )
        : EventPresenter_ABC( type )
        , view_( view )
        , state_( new ViewState() )

    {
        // NOTHING
    }
    virtual ~EventSubPresenter_ABC() {}
    //@}

protected:
    //! @name Operations
    //@{
    virtual void BuildView()
    {
        view_.BlockSignals( true );
        view_.Build( *state_ );
        view_.BlockSignals( false );
    }
    virtual void Purge()
    {
        state_->Purge();
        view_.BlockSignals( true );
        view_.Purge();
        view_.BlockSignals( false );
    }
    virtual void Draw( gui::Viewport_ABC& viewPort ) const
    {
        view_.Draw( viewPort );
    }
    //@}

protected:
    //! @name Member data
    //@{
    EventView_ABC< ViewState >& view_;
    boost::scoped_ptr< ViewState > state_;
    //@}
};

} //! namespace gui

#endif // __EventSubPresenter_ABC_h_

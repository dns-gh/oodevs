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

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace timeline
{
    struct Event;
}

namespace gui
{

// =============================================================================
/** @class  EventSubPresenter_ABC
    @brief  EventSubPresenter_ABC
*/
// Created: ABR 2013-11-20
// =============================================================================
class EventSubPresenter_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             EventSubPresenter_ABC() {}
    virtual ~EventSubPresenter_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool ShouldEnableTrigger() const
    {
        return false;
    }
    virtual bool ShouldEnableClear() const
    {
        return false;
    }
    //@}

    //! @name Abstract operations
    //@{
    virtual void Trigger() = 0;
    virtual void CommitTo( timeline::Event& event ) const = 0;
    virtual void Purge() = 0; // reset all
    virtual void Clear() = 0; // reset content
    //@}

    //! @name Types
    //@{
    typedef boost::shared_ptr< EventSubPresenter_ABC > T_SharedPtr;
    //@}
};

} //! namespace gui

#endif // __EventSubPresenter_ABC_h_

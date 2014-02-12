// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventMagicPresenter_h_
#define __EventMagicPresenter_h_

#include "EventSubPresenter_ABC.h"
#include "clients_kernel/SafePointer.h"
#include <boost/scoped_ptr.hpp>

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
     class Controllers;
     class Decisions_ABC;
     class Entity_ABC;
}

namespace gui
{
    struct EventMagicViewState;
    template< typename T > class EventView_ABC;

// =============================================================================
/** @class  EventMagicPresenter
    @brief  EventMagicPresenter
*/
// Created: ABR 2013-12-17
// =============================================================================
class EventMagicPresenter :public EventSubPresenter_ABC< EventMagicViewState >
{

public:
    //! @name Constructors/Destructor
    //@{
             EventMagicPresenter( EventView_ABC< EventMagicViewState >& view,
                                  kernel::Controllers& controllers,
                                  actions::ActionsModel& actionsModel );
    virtual ~EventMagicPresenter();
    //@}

    //! @name Operations
    //@{
    void OnTargetChanged( const kernel::Entity_ABC* entity );
    //@}

private:
    //! @name EventSubPresenter_ABC implementation
    //@{
    virtual bool ShouldEnableTrigger() const;

    virtual void Trigger( const gui::Event& event );
    virtual void FillFrom( const gui::Event& event );
    virtual void CommitTo( timeline::Event& event ) const;
    virtual void Purge();
    virtual void Clear();
    //@}

private:
    //! @name Member data
    //@{
    actions::ActionsModel& actionsModel_;
    kernel::SafePointer< kernel::Entity_ABC > entity_;
    const kernel::Decisions_ABC* decisions_;
    //@}
};

} //! namespace gui

#endif // __EventMagicPresenter_h_

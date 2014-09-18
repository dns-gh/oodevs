// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __Event_h_
#define __Event_h_

#include "clients_kernel/GraphicalEntity_ABC.h"
#include "ENT/ENT_Enums.h"
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace actions
{
    class Action_ABC;
};

namespace kernel
{
    class ActionController;
}

namespace timeline
{
    struct Event;
}

namespace gui
{

// =============================================================================
/** @class  Event
    @brief  Event
*/
// Created: ABR 2013-05-28
// =============================================================================
class Event : public kernel::GraphicalEntity_ABC
            , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             Event( E_EventTypes type, const timeline::Event& event );
    virtual ~Event();
    //@}

    //! @name Operations
    //@{
    E_EventTypes GetType() const;
    timeline::Event& GetEvent() const;
    virtual Event* Clone() const;
    void Update();
    virtual const actions::Action_ABC* GetAction() const;
    virtual void Update( const timeline::Event& event );
    //@}

    //! @name GraphicalEntity_ABC implementation
    //@{
    virtual QString GetName() const;
    virtual QString GetTooltip() const;

    virtual void Select( kernel::ActionController& controller ) const;
    virtual void MultipleSelect( kernel::ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const;
    virtual void ContextMenu( kernel::ActionController& controller, const QPoint& where ) const;
    virtual void Activate( kernel::ActionController& controller ) const;
    virtual void OverFly( kernel::ActionController& controller ) const;
    //@}

protected:
    //! @name Member data
    //@{
    E_EventTypes type_;
    boost::scoped_ptr< timeline::Event > event_;
    //@}
};

} //! namespace gui

#endif // __Event_h_

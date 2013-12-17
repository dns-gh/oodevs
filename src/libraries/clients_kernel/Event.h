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

#include "GraphicalEntity_ABC.h"
#include "ENT/ENT_Enums.h"
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#define EVENT_DATE_FORMAT "yyyy-MM-ddTHH:mm:ssZ"

#define EVENT_ORDER_PROTOCOL "sword"
#define EVENT_REPORT_PROTOCOL "report"
#define EVENT_SIMULATION_SERVICE "simulation"
#define EVENT_MULTIMEDIA_PROTOCOL "sword"
#define EVENT_MULTIMEDIA_SERVICE "multimedia"

#define CREATE_EVENT_TARGET( PROTOCOL, SERVICE ) PROTOCOL "://" SERVICE

namespace timeline
{
    struct Event;
}

namespace kernel
{
    class ActionController;

// =============================================================================
/** @class  Event
    @brief  Event
*/
// Created: ABR 2013-05-28
// =============================================================================
class Event : public GraphicalEntity_ABC
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
    virtual void Update( const timeline::Event& event );
    virtual void Select( ActionController& eventController, ActionController& actionController ) const;
    //@}

    //! @name GraphicalEntity_ABC implementation
    //@{
    virtual QString GetName() const;
    virtual QString GetTooltip() const;

    virtual void Select( ActionController& controller ) const;
    virtual void MultipleSelect( ActionController& controller, const std::vector< const GraphicalEntity_ABC* >& elements ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    virtual void Activate( ActionController& controller ) const;
    virtual void OverFly( ActionController& controller ) const;
    //@}

protected:
    //! @name Member data
    //@{
    E_EventTypes type_;
    boost::scoped_ptr< timeline::Event > event_;
    //@}
};

} //! namespace kernel

#endif // __Event_h_

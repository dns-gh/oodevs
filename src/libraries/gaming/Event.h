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

#include <boost/noncopyable.hpp>
#include "ENT/ENT_Enums_Gen.h"

namespace timeline
{
    struct Event;
}

#define EVENT_DATE_FORMAT "yyyy-MM-ddTHH:mm:ssZ"

#define EVENT_ORDER_PROTOCOL "sword"
#define EVENT_REPORT_PROTOCOL "report"
#define EVENT_SIMULATION_SERVICE "simulation"
#define EVENT_MULTIMEDIA_PROTOCOL "sword"
#define EVENT_MULTIMEDIA_SERVICE "multimedia"

#define CREATE_EVENT_TARGET( PROTOCOL, SERVICE ) PROTOCOL "://" SERVICE

// =============================================================================
/** @class  Event
    @brief  Event
*/
// Created: ABR 2013-05-28
// =============================================================================
class Event : private boost::noncopyable
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
    virtual void Update( const timeline::Event& event );
    //@}

protected:
    //! @name Member data
    //@{
    E_EventTypes type_;
    std::auto_ptr< timeline::Event > event_;
    //@}
};

#endif // __Event_h_

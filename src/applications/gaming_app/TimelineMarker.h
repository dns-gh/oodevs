// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TimelineMarker_h_
#define __TimelineMarker_h_

#include "TimelineItem_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

class ActionsScheduler;
class Simulation;
class TimelineRuler;

// =============================================================================
/** @class  TimelineMarker
    @brief  TimelineMarker
*/
// Created: SBO 2007-07-16
// =============================================================================
class TimelineMarker : public TimelineItem_ABC
                     , public kernel::Observer_ABC
                     , public kernel::ElementObserver_ABC< Simulation >
{

public:
    //! @name Constructors/Destructor
    //@{
             TimelineMarker( QCanvas* canvas, ActionsScheduler& scheduler, kernel::Controllers& controllers, const TimelineRuler& ruler );
    virtual ~TimelineMarker();
    //@}

    //! @name Operations
    //@{
    virtual void Shift( long shift );
    virtual void Update();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimelineMarker( const TimelineMarker& );            //!< Copy constructor
    TimelineMarker& operator=( const TimelineMarker& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Simulation& simulation );
    virtual void draw( QPainter& painter );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ActionsScheduler& scheduler_;
    const TimelineRuler& ruler_;
    //@}
};

#endif // __TimelineMarker_h_

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TimelinePanel_h_
#define __TimelinePanel_h_

namespace kernel
{
    class Controllers;
}

class ActionsScheduler;
class Simulation;

// =============================================================================
/** @class  TimelinePanel
    @brief  TimelinePanel
*/
// Created: SBO 2007-07-04
// =============================================================================
class TimelinePanel : public QDockWindow
{

public:
    //! @name Constructors/Destructor
    //@{
             TimelinePanel( QMainWindow* parent, kernel::Controllers& controllers, ActionsScheduler& scheduler, const Simulation& simulation );
    virtual ~TimelinePanel();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimelinePanel( const TimelinePanel& );            //!< Copy constructor
    TimelinePanel& operator=( const TimelinePanel& ); //!< Assignment operator
    //@}
};

#endif // __TimelinePanel_h_

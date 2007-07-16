// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionsScheduler_h_
#define __ActionsScheduler_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include "Simulation.h"

namespace kernel
{
    class Controllers;
}

class ActionsModel;
class Publisher_ABC;
class Simulation;

// =============================================================================
/** @class  ActionsScheduler
    @brief  ActionsScheduler
*/
// Created: SBO 2007-07-13
// =============================================================================
class ActionsScheduler : public QObject
                       , public kernel::Observer_ABC
                       , public kernel::ElementObserver_ABC< Simulation::sStartTick >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionsScheduler( QObject* parent, kernel::Controllers& controllers, const Simulation& simulation, const ActionsModel& actions, Publisher_ABC& publisher );
    virtual ~ActionsScheduler();
    //@}

    //! @name Operations
    //@{
    void Start();
    void Stop();
    bool IsRunning() const;
    unsigned long GetCurrentTime() const;
    void Shift( long shift );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionsScheduler( const ActionsScheduler& );            //!< Copy constructor
    ActionsScheduler& operator=( const ActionsScheduler& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Simulation::sStartTick& tick );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const Simulation& simulation_;
    const ActionsModel& actions_;
    Publisher_ABC& publisher_;
    unsigned long currentTime_;
    unsigned long lastTick_;
    bool running_;
    bool paused_;
    //@}
};

#endif // __ActionsScheduler_h_

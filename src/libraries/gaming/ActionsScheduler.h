// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ActionsScheduler_h_
#define __ActionsScheduler_h_

#include "tools/ElementObserver_ABC.h"
#include "Simulation.h"
#include "protocol/ServerPublisher_ABC.h"

namespace kernel
{
    class Controllers;
}
namespace actions
{
    class ActionsModel;
}
class Publisher_ABC;
class Simulation;

// =============================================================================
/** @class  ActionsScheduler
    @brief  ActionsScheduler
*/
// Created: SBO 2007-07-13
// =============================================================================
class ActionsScheduler : public QObject // $$$$ AGE 2008-05-19: why ?
                       , public tools::Observer_ABC
                       , public tools::ElementObserver_ABC< Simulation::sStartTick >
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionsScheduler( QObject* parent, kernel::Controllers& controllers, const Simulation& simulation, const actions::ActionsModel& actions, Publisher_ABC& publisher );
    virtual ~ActionsScheduler();
    //@}

    //! @name Operations
    //@{
    QDateTime GetDateTime() const;
    void Shift( long secs );
    void SetDate( const QDateTime& dateTime );
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
    const actions::ActionsModel& actions_;
    Publisher_ABC& publisher_;
    QDateTime currentTime_;
    //@}
};

#endif // __ActionsScheduler_h_

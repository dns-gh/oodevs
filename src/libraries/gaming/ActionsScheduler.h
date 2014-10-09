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

#include "Simulation.h"
#include "protocol/ServerPublisher_ABC.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
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
                       , public tools::ElementObserver_ABC< Simulation >
                       , public tools::ElementObserver_ABC< Simulation::sStartTick >
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionsScheduler( QObject* parent, kernel::Controllers& controllers, const Simulation& simulation, Publisher_ABC& publisher );
    virtual ~ActionsScheduler();
    //@}

    //! @name Operations
    //@{
    QDateTime GetDateTime() const;
    void Shift( long secs );
    void SetDate( const QDateTime& dateTime );
    void SkipToDate( const QDateTime& dateTime );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Simulation& simulation );
    virtual void NotifyUpdated( const Simulation::sStartTick& tick );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const Simulation& simulation_;
    Publisher_ABC& publisher_;
    QDateTime currentTime_;
    //@}
};

#endif // __ActionsScheduler_h_

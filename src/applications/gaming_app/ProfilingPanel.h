// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ProfilingPanel_h_
#define __ProfilingPanel_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include "gaming/Simulation.h"

namespace kernel
{
    class Controllers;
}

class Simulation;
class Network;
class StatisticsWidget;

// =============================================================================
/** @class  ProfilingPanel
    @brief  ProfilingPanel
*/
// Created: SBO 2007-01-04
// =============================================================================
class ProfilingPanel : public QVBox
                     , public kernel::Observer_ABC
                     , public kernel::ElementObserver_ABC< Simulation::sEndTick >
{

public:
    //! @name Constructors/Destructor
    //@{
             ProfilingPanel( QWidget* parent, kernel::Controllers& controllers, const Network& network );
    virtual ~ProfilingPanel();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ProfilingPanel( const ProfilingPanel& );            //!< Copy constructor
    ProfilingPanel& operator=( const ProfilingPanel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Simulation::sEndTick& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const Network& network_;
    StatisticsWidget* networkReceived_;
    StatisticsWidget* networkSent_;
    QLabel* networkTotalReceived_;
    QLabel* networkTotalSent_;

    unsigned long previousTotalReceived_;
    unsigned long previousTotalSent_;
    unsigned int ticks_;
    //@}
};

#endif // __ProfilingPanel_h_

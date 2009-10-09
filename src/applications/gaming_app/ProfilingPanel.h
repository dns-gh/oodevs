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

#include "tools/ElementObserver_ABC.h"
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
class ProfilingPanel : public QTabWidget
                     , public tools::Observer_ABC
                     , public tools::ElementObserver_ABC< Simulation::sEndTick >
{

public:
    //! @name Constructors/Destructor
    //@{
             ProfilingPanel( QWidget* parent, kernel::Controllers& controllers, const Network& network, const Simulation& simulation );
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
    const Simulation& simulation_;
    StatisticsWidget* networkReceived_;
    StatisticsWidget* networkSent_;
    StatisticsWidget* memory_;
    StatisticsWidget* virtualMemory_;
    StatisticsWidget* shortPathfinds_;
    StatisticsWidget* longPathfinds_;
    QLabel* networkTotalReceived_;
    QLabel* networkTotalSent_;
    QLabel* memoryUsage_;
    QLabel* virtualMemoryUsage_;
    QLabel* shortPathfindsCount_;
    QLabel* longPathfindsCount_;

    unsigned long previousTotalReceived_;
    unsigned long previousTotalSent_;
    unsigned int ticks_;
    //@}
};

#endif // __ProfilingPanel_h_

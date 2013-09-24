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

#include "clients_gui/RichDockWidget.h"
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
class ProfilingPanel : public gui::RichDockWidget
                     , public tools::ElementObserver_ABC< Simulation::sEndTick >
{
public:
    //! @name Constructors/Destructor
    //@{
             ProfilingPanel( QWidget* parent, kernel::Controllers& controllers, const Network& network, const Simulation& simulation );
    virtual ~ProfilingPanel();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Simulation::sEndTick& );
    //@}

private:
    //! @name Member data
    //@{
    QTabWidget* tabWidget_;
    const Network& network_;
    const Simulation& simulation_;
    StatisticsWidget* networkBytesReceived_;
    StatisticsWidget* networkBytesSent_;
    StatisticsWidget* networkMsgsReceived_;
    StatisticsWidget* networkMsgsSent_;
    StatisticsWidget* memory_;
    StatisticsWidget* virtualMemory_;
    StatisticsWidget* shortPathfinds_;
    StatisticsWidget* longPathfinds_;
    QLabel* networkTotalBytesReceived_;
    QLabel* networkTotalBytesSent_;
    QLabel* networkTotalMsgsReceived_;
    QLabel* networkTotalMsgsSent_;
    QLabel* memoryUsage_;
    QLabel* virtualMemoryUsage_;

    uint64_t previousTotalBytesReceived_;
    uint64_t previousTotalBytesSent_;
    unsigned long previousTotalMsgsReceived_;
    unsigned long previousTotalMsgsSent_;
    unsigned int ticks_;
    //@}
};

#endif // __ProfilingPanel_h_

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __StatusBar_h_
#define __StatusBar_h_

#include "clients_gui/StatusBar.h"
#include "tools/ElementObserver_ABC.h"
#include "gaming/Simulation.h"

namespace kernel
{
    class Controllers;
}

// =============================================================================
/** @class  StatusBar
    @brief  StatusBar
*/
// Created: SBO 2006-08-22
// =============================================================================
class StatusBar : public gui::StatusBar
                , public tools::Observer_ABC
                , public tools::ElementObserver_ABC< Simulation >
                , public tools::ElementObserver_ABC< Simulation::sStartTick >
                , public tools::ElementObserver_ABC< Simulation::sEndTick >
                , public tools::ElementObserver_ABC< Simulation::sCheckPoint >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             StatusBar( QStatusBar* bar, const kernel::DetectionMap& detection, const kernel::CoordinateConverter_ABC& converter, kernel::Controllers& controllers, QDockWindow* profilingDock );
    virtual ~StatusBar();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnLag();
    void OnCheckPoint();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    StatusBar( const StatusBar& );            //!< Copy constructor
    StatusBar& operator=( const StatusBar& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Simulation& simulation );
    virtual void NotifyUpdated( const Simulation::sStartTick& startTick );
    virtual void NotifyUpdated( const Simulation::sEndTick& endTick );
    virtual void NotifyUpdated( const Simulation::sCheckPoint& checkpoint );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    bool lastSimulationStatus_;
    QPushButton* pSpeed_;
    QPushButton* pTime_;
    QPushButton* pTick_;
    QTimer* pLagTimer_;
    QLabel* checkPoint_;
    QTimer* checkPointTimer_;
    QPixmap tickred_;
    QPixmap tickoff_;
    QPixmap tickon_;
    //@}
};

#endif // __StatusBar_h_

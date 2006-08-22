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
#include "clients_kernel/ElementObserver_ABC.h"
#include "gaming/Simulation.h"

// =============================================================================
/** @class  StatusBar
    @brief  StatusBar
*/
// Created: SBO 2006-08-22
// =============================================================================
class StatusBar : public gui::StatusBar
                , public kernel::Observer_ABC
                , public kernel::ElementObserver_ABC< Simulation >
                , public kernel::ElementObserver_ABC< Simulation::sStartTick >
                , public kernel::ElementObserver_ABC< Simulation::sEndTick >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             StatusBar( QStatusBar* bar, const kernel::DetectionMap& detection, const kernel::CoordinateConverter_ABC& converter, kernel::Controllers& controllers );
    virtual ~StatusBar();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnLag();
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
    //@}

private:
    //! @name Member data
    //@{
    bool lastSimulationStatus_;
    QLabel* pTime_;
    QLabel* pTick_;
    QTimer* pLagTimer_;
    //@}
};

#endif // __StatusBar_h_

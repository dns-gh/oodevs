// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ClockEditDialog_h_
#define __ClockEditDialog_h_

#include <tools/Observer_ABC.h>
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
}

class Simulation;
class SimulationController;

// =============================================================================
/** @class  ClockEditDialog
    @brief  ClockEditDialog
*/
// Created: SBO 2010-05-31
// =============================================================================
class ClockEditDialog : public QDialog
                      , public tools::Observer_ABC
                      , public tools::ElementObserver_ABC< Simulation >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ClockEditDialog( QWidget* parent, kernel::Controllers& controllers, const SimulationController& simulationController );
    virtual ~ClockEditDialog();
    //@}

private slots:
    //! @name Operations
    //@{
    void OnCommit();
    void OnCancel();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Simulation& simulation );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const SimulationController& simulationController_;
    QDateTimeEdit* editor_;
    //@}
};

#endif // __ClockEditDialog_h_

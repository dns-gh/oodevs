// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ReplayerToolbar_h_
#define __ReplayerToolbar_h_

#include "clients_gui/RichToolBar.h"
#include "tools/ElementObserver_ABC.h"
#include "gaming/Simulation.h"

namespace kernel
{
    class ContextMenu;
    class Controllers;
}

class Publisher_ABC;
class SimulationController;

// =============================================================================
/** @class  ReplayerToolbar
    @brief  ReplayerToolbar
*/
// Created: AGE 2007-04-11
// =============================================================================
class ReplayerToolbar : public gui::RichToolBar
                      , public tools::ElementObserver_ABC< Simulation >
                      , public tools::ElementObserver_ABC< Simulation::sTimeTable >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ReplayerToolbar( QMainWindow* pParent, kernel::Controllers& controllers, SimulationController& simulationController, Publisher_ABC& network );
    virtual ~ReplayerToolbar();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void NotifyUpdated( const Simulation& simulation );
    virtual void NotifyUpdated( const Simulation::sTimeTable& timeTable );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSliderMoved( int value );
    void OnSpinBoxChanged();
    void OnDateTimeChanged();
    void OnSliderPressed();
    void OnSliderReleased();
    void OnMenuActivated( int index );
    void OnTimeTable();
    void OnRefresh();
    //@}

private:
    //! @name Helpers
    //@{
    void SkipToTick( unsigned int tick );
    QSpinBox* SpinBox();
    QDateTimeEdit* DateTimeEdit();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    SimulationController& simulationController_;
    Publisher_ABC& network_;
    QSlider* slider_;
    QAction* spinBoxAction_;
    QAction* dateTimeAction_;
    kernel::ContextMenu* menu_;
    QPushButton* button_;
    int sliderTick_;
    //@}
};

#endif // __ReplayerToolbar_h_

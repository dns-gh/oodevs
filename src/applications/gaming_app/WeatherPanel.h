    // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __WeatherPanel_h_
#define __WeatherPanel_h_

#include "clients_gui/WeatherPanel.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Location_ABC;
    class ModelLoaded;
    class Time_ABC;
    class Viewport_ABC;
}

namespace gui
{
    class PanelStack_ABC;
    class ShapeHandler_ABC;
}

namespace actions
{
    class ActionsModel;
}

class Simulation;
class StaticModel;
class WeatherWidget;
class MeteoModel;
class WeatherLayer;

// =============================================================================
/** @class  WeatherPanel
    @brief  WeatherPanel
*/
// Created: ABR 2011-06-06
// =============================================================================
class WeatherPanel : public gui::WeatherPanel
                   , public tools::Observer_ABC
                   , public tools::ElementObserver_ABC< MeteoModel >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             WeatherPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, actions::ActionsModel& actionsModel,
                           const StaticModel& model, const kernel::Time_ABC& simulation, WeatherLayer& layer );
    virtual ~WeatherPanel();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyUpdated( const MeteoModel& model );
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void Commit();
    virtual void Reset();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    MeteoModel* currentModel_;
    //@}
};

#endif // __WeatherPanel_h_

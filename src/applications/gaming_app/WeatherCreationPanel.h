// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __WeatherCreationPanel_h_
#define __WeatherCreationPanel_h_

#include "clients_gui/InfoPanel_ABC.h"
#include "clients_gui/ShapeHandler_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "LocationSerializer.h"


namespace kernel
{
    class Controllers;
    class GlTools_ABC;
    class ModelLoaded;
    class Location_ABC;
    class Viewport_ABC;
}

namespace gui
{
    class ParametersLayer;
    class PanelStack_ABC;
    class ShapeHandler_ABC;
}

namespace actions
{
    class ActionsModel;
}

class StaticModel;
class WeatherWidget;
class ActionPublisher;
class Simulation;

// =============================================================================
/** @class  WeatherCreationPanel
    @brief  WeatherCreationPanel
*/
// Created: SLG 2010-03-24
// =============================================================================
class WeatherCreationPanel : public gui::InfoPanel_ABC
                           , public tools::Observer_ABC
                           , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                           , public gui::ShapeHandler_ABC
{
        Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
    WeatherCreationPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, ActionPublisher& actionPublisher, actions::ActionsModel& actionsModel, const StaticModel& model, const Simulation& simulation, gui::ParametersLayer& layer, const kernel::GlTools_ABC& tools );
    virtual ~WeatherCreationPanel();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    void Draw( const kernel::Viewport_ABC& viewport );
    virtual void Handle ( kernel::Location_ABC& location );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    WeatherCreationPanel( const WeatherCreationPanel& );            //!< Copy constructor
    WeatherCreationPanel& operator=( const WeatherCreationPanel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void showEvent( QShowEvent* );
    virtual void hideEvent( QHideEvent* );
    void ResetLocation();
    bool CheckValidity();
    //@}

private slots:
    //! @name Slots
    //@{
    void Commit();
    void Reset();
    void StartEditingLocation();
    void OnToogleWeatherType( bool );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::ParametersLayer& layer_;
    ActionPublisher& actionPublisher_;
    actions::ActionsModel& actionsModel_;
    const StaticModel& model_;
    const Simulation& simulation_;
    const kernel::GlTools_ABC& tools_;
    LocationSerializer serializer_;
    WeatherWidget* weather_;
    QDateTimeEdit* startTime_;
    QDateTimeEdit* endTime_;
    QButton* positionBtn_;
    kernel::Location_ABC* location_;
    bool bLocationDisplay_;
    bool isGlobal_;
    //@}
};

#endif // __WeatherCreationPanel_h_

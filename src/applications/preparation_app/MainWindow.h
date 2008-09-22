// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MainWindow_h_
#define __MainWindow_h_

#include <qmainwindow.h>
#include "clients_kernel/ControllerObserver_ABC.h"

namespace kernel
{
    class Options;
    class Controllers;
    class Profile_ABC;
}

namespace tools
{
    class ExerciseConfig;
}

namespace gui
{
    class OptionsPanel;
    class GlProxy;
    class GlWidget;
    class ParametersLayer;
    class GlPlaceHolder;
    class IconLayout;
    class ColorStrategy;
    class CircularEventStrategy;
    class ExclusiveEventStrategy;
    class GraphicPreferences;
    class LightingProxy;
    class GlSelector;
    class PreferencesDialog;
    class LocationsLayer;
}

class AgentsLayer;
class Model;
class ModelBuilder;
class StaticModel;
class ObjectCreationPanel;
class WeatherLayer;
class MapLayer_ABC;

// =============================================================================
/** @class  MainWindow
    @brief  Main window of the application.

    The main window serves as a transit point for nearly all GUI QT signals, whether
    they are inter-GUI (ie. one GUI element signaling the others) or GUI-data.
    This allows GUI objects to be independant: The only other element they know of
    is the main window, and thus they can be moved around in the class hierachy
    without having to redo the connexions every time.
*/
// Created: APE 2004-03-01
// =============================================================================
class MainWindow : public QMainWindow
                 , public kernel::Observer_ABC
                 , public kernel::ControllerObserver_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor/Accessor
    //@{
             MainWindow( kernel::Controllers& controllers, StaticModel& staticModel, Model& model, tools::ExerciseConfig& config, const QString& expiration );
    virtual ~MainWindow();
    //@}

    //! @name Operations
    //@{
    bool Load();
    kernel::Options& GetOptions() const;
    //@}

public slots:
    //! @name Slots
    //@{
    bool New();
    void Open();
    void Close();
    bool Save();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MainWindow( const MainWindow& );
    MainWindow& operator=( const MainWindow& );
    //@}

    //! @name Helpers
    //@{
    void LoadExercise();
    void CreateLayers( ObjectCreationPanel& objects, gui::ParametersLayer& parameters, gui::LocationsLayer& locations, WeatherLayer& weather, AgentsLayer& agents, gui::GraphicPreferences& setup, gui::PreferencesDialog& preferences, const kernel::Profile_ABC& profile );
    void closeEvent( QCloseEvent* pEvent );
    void WriteSettings();
    void ReadSettings();
    void WriteOptions();
    void ReadOptions();

    virtual void NotifyCreated();
    virtual void NotifyUpdated();
    virtual void NotifyDeleted();
    void SetWindowTitle( bool needsSaving );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    StaticModel& staticModel_;
    Model& model_;
    ModelBuilder* modelBuilder_;
    tools::ExerciseConfig& config_;

    std::auto_ptr< gui::CircularEventStrategy >  forward_;
    std::auto_ptr< gui::ExclusiveEventStrategy > eventStrategy_;
    std::auto_ptr< MapLayer_ABC >                moveLayer_;

    gui::GlProxy*       glProxy_;
    gui::ColorStrategy* strategy_;
    gui::LightingProxy* lighting_;

    gui::GlSelector* selector_;

    gui::OptionsPanel*    pOptionsPanel_;
    gui::ParametersLayer* parameters_;
    bool                  loading_;
    bool                  needsSaving_;
    //@}
};

#endif // __MainWindow_h_

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

#include "tools/ControllerObserver_ABC.h"

namespace kernel
{
    class Options;
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class GlProxy;
    class ParametersLayer;
    class ColorStrategy;
    class CircularEventStrategy;
    class ExclusiveEventStrategy;
    class LightingProxy;
    class GlSelector;
    class PreferencesDialog;
    class TerrainLayer;
    class Layer_ABC;
    class Painter_ABC;
    class TerrainPicker;
    class LayerFilter_ABC;
    class AutomatsLayer;
    class FormationLayer;
}

class Config;
class FileToolbar;
class FilterDialogs;
class Menu;
class Model;
class ModelBuilder;
class ModelConsistencyDialog;
class ScoreDialog;
class StaticModel;
class ColorController;
class DockManager;

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
                 , public tools::Observer_ABC
                 , public tools::ControllerObserver_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor/Accessor
    //@{
             MainWindow( kernel::Controllers& controllers, StaticModel& staticModel, Model& model, Config& config, const QString& expiration );
    virtual ~MainWindow();
    //@}

    //! @name Operations
    //@{
    bool Load();
    kernel::Options& GetOptions() const;
    bool CheckSaving();
    //@}

public slots:
    //! @name Slots
    //@{
    void New();
    void Open();
    bool Close();
    bool Save();
    void SaveAs();
    void ToggleFullScreen();
    void ToggleDocks();
    void ReloadExercise();
    void CheckConsistency();
    void ClearLoadingErrors();
    void OnForceSaveAndAddActionPlanning( const std::string& filename );
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
    void CreateLayers( gui::ParametersLayer& parameters, gui::Layer_ABC& locations,
                       gui::Layer_ABC& weather, gui::TerrainLayer& terrain, gui::Layer_ABC& profilerLayer,
                       gui::PreferencesDialog& preferences, const kernel::Profile_ABC& profile, gui::TerrainPicker& picker,
                       gui::AutomatsLayer& automats, gui::FormationLayer& formation );
    void closeEvent( QCloseEvent* pEvent );
    void WriteOptions();
    void ReadOptions();
    void DoLoad( QString filename );
    void MigrateExercises();

    virtual void NotifyCreated();
    virtual void NotifyUpdated();
    virtual void NotifyDeleted();
    void SetWindowTitle( bool needsSaving );
    void SetNeedsSaving( bool status );
    void EnableWorkspace( bool enabled );
    void SetProgression( int value, const QString& text );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    StaticModel& staticModel_;
    Model& model_;
    Config& config_;
    std::auto_ptr< ModelBuilder > modelBuilder_;
    std::auto_ptr< gui::CircularEventStrategy > forward_;
    std::auto_ptr< gui::ExclusiveEventStrategy > eventStrategy_;
    std::auto_ptr< gui::Painter_ABC > pPainter_;
    std::auto_ptr< gui::LayerFilter_ABC > simpleFilter_;
    std::auto_ptr< gui::LayerFilter_ABC > urbanFilter_;
    std::auto_ptr< ColorController > colorController_;
    std::auto_ptr< DockManager > pDockManager_;
    std::auto_ptr< gui::GlProxy > glProxy_;
    std::auto_ptr< gui::LightingProxy > lighting_;
    std::auto_ptr< gui::ColorStrategy > strategy_;
    gui::GlSelector* selector_;
    Menu* menu_;
    FileToolbar* fileToolBar_;
    bool loading_;
    bool needsSaving_;
    QString savedState_;
    QByteArray docks_;
    QByteArray toolbars_;
    ScoreDialog* pScoreDialog_;
    FilterDialogs* filterDialogs_;
    QProgressDialog* progressDialog_;
    ModelConsistencyDialog* consistencyDialog_;
    //@}
};

#endif // __MainWindow_h_

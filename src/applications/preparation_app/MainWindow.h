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
#include "clients_kernel/ModesObserver_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
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
    class Layer;
    class Painter_ABC;
    class TerrainPicker;
    class AutomatsLayer;
    class FormationLayer;
    class HelpSystem;
    class EntitySymbols;
}

class ColorController;
class Config;
class DialogContainer;
class DockContainer;
class Menu;
class Model;
class ModelBuilder;
class StaticModel;
class ToolbarContainer;

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
                 , public kernel::ModesObserver_ABC
                 , private boost::noncopyable
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             MainWindow( kernel::Controllers& controllers, StaticModel& staticModel, Model& model, Config& config, const QString& expiration );
    virtual ~MainWindow();
    //@}

    //! @name Operations
    //@{
    QMessageBox::StandardButton CheckSaving( bool checkConsistency = false );
    void ExternalReload();
    //@}

public slots:
    //! @name Slots
    //@{
    void ShowHelp();
    void New();
    void Open();
    bool Close();
    void Save( bool checkConsistency = true );
    void SaveAs();
    void ToggleFullScreen();
    void ToggleDocks();
    void ReloadExercise();
    void ClearLoadingErrors();
    void OnForceSaveAndAddActionPlanning( const std::string& filename );
    void OnAddRaster();
    void OnRasterProcessExited( int exitCode, QProcess::ExitStatus exitStatus );
    //@}

signals:
    //! @name Signals
    //@{
    void CheckConsistency();
    //@}

private:
    //! @name Helpers
    //@{
    bool Load();
    void LoadExercise( bool checkConsistency = true );
    void CreateLayers( gui::ParametersLayer& parameters, gui::Layer& locations, gui::Layer& weather, gui::Layer& profilerLayer,
                       const kernel::Profile_ABC& profile, gui::TerrainPicker& picker, gui::AutomatsLayer& automats, gui::FormationLayer& formation );
    void closeEvent( QCloseEvent* pEvent );
    void WriteOptions();
    void ReadOptions();
    void DoClose();
    void DoLoad( QString filename, bool checkConsistency = true );
    void MigrateExercises();

    virtual void NotifyCreated();
    virtual void NotifyUpdated();
    virtual void NotifyDeleted();
    virtual void NotifyModeChanged( int newMode );
    void SetWindowTitle( bool needsSaving );
    void SetNeedsSaving( bool status );
    void SetProgression( int value, const QString& text );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    StaticModel&         staticModel_;
    Model&               model_;
    Config&              config_;
    bool                 loading_;
    bool                 needsSaving_;
    bool                 terrainHasChanged_;

    std::auto_ptr< ModelBuilder >                modelBuilder_;
    std::auto_ptr< gui::CircularEventStrategy >  forward_;
    std::auto_ptr< gui::ExclusiveEventStrategy > eventStrategy_;
    std::auto_ptr< gui::Painter_ABC >            pPainter_;
    std::auto_ptr< ColorController >             colorController_;
    std::auto_ptr< DockContainer >               dockContainer_;
    std::auto_ptr< DialogContainer >             dialogContainer_;
    std::auto_ptr< ToolbarContainer >            toolbarContainer_;
    std::auto_ptr< gui::GlProxy >                glProxy_;
    std::auto_ptr< gui::LightingProxy >          lighting_;
    std::auto_ptr< gui::ColorStrategy >          strategy_;
    std::auto_ptr< Menu >                        menu_;
    std::auto_ptr< gui::GlSelector >             selector_;
    std::auto_ptr< QProgressDialog >             progressDialog_;
    std::auto_ptr< gui::HelpSystem >             help_;
    std::auto_ptr< QProcess >                    process_;
    std::auto_ptr< gui::EntitySymbols >          icons_;
    //@}
};

#endif // __MainWindow_h_

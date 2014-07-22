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

#include "clients_kernel/ModesObserver_ABC.h"
#include <tools/ControllerObserver_ABC.h>
#include <boost/shared_ptr.hpp>

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class AutomatsLayer;
    class CircularEventStrategy;
    class ColorStrategy;
    class Elevation2dLayer;
    class EntitySymbols;
    class ExclusiveEventStrategy;
    class FormationLayer;
    class GlProxy;
    class GlSelector;
    class GraphicPreferences;
    class HelpSystem;
    class Layer;
    class LightingProxy;
    class Painter_ABC;
    class ParametersLayer;
    class TerrainPicker;
    class TextEditor;
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
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             MainWindow( kernel::Controllers& controllers, StaticModel& staticModel, Model& model, Config& config, const QString& expiration );
    virtual ~MainWindow();
    //@}

    //! @name Operations
    //@{
    QMessageBox::StandardButton CheckSaving( bool checkConsistency = false );

    void Load();
    //@}

public slots:
    //! @name Slots
    //@{
    void New();
    void Open();
    bool Close();
    void Save( bool checkConsistency = true );
    void SaveAs();
    void ToggleFullScreen();
    void ToggleDocks();
    void ReloadExercise();
    void ClearLoadingErrors();
    void OnForceSaveAndAddActionPlanning( const tools::Path& filename );
    void OnAddRaster();
    //@}

signals:
    //! @name Signals
    //@{
    void CheckConsistency();
    void ShowHelp();
    void CloseDialogs();
    //@}

private:
    //! @name Helpers
    //@{
    bool DoLoad();
    void LoadExercise();
    void CreateLayers( gui::ParametersLayer& parameters, gui::Layer& locations, gui::Layer& weather, gui::Layer& profilerLayer,
                       const kernel::Profile_ABC& profile, gui::TerrainPicker& picker, gui::AutomatsLayer& automats, gui::FormationLayer& formation,
                       gui::Elevation2dLayer& elevation2d );
    void closeEvent( QCloseEvent* pEvent );
    void DoClose();
    void DoLoad( const tools::Path& filename );
    void MigrateExercises();
    bool MigrateExercise( const tools::Path& path );

    void NotifyModeChanged( E_Modes newMode );
    virtual void NotifyCreated();
    virtual void NotifyUpdated();
    virtual void NotifyDeleted();
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

    std::unique_ptr< gui::GraphicPreferences >     graphicPreferences_;
    std::unique_ptr< ModelBuilder >                modelBuilder_;
    std::unique_ptr< gui::CircularEventStrategy >  forward_;
    std::unique_ptr< gui::ExclusiveEventStrategy > eventStrategy_;
    std::unique_ptr< gui::Painter_ABC >            pPainter_;
    std::unique_ptr< ColorController >             colorController_;
    std::unique_ptr< DockContainer >               dockContainer_;
    std::unique_ptr< DialogContainer >             dialogContainer_;
    std::unique_ptr< ToolbarContainer >            toolbarContainer_;
    std::unique_ptr< gui::GlProxy >                glProxy_;
    std::unique_ptr< gui::LightingProxy >          lighting_;
    std::unique_ptr< gui::ColorStrategy >          strategy_;
    std::unique_ptr< Menu >                        menu_;
    std::unique_ptr< gui::GlSelector >             selector_;
    std::unique_ptr< QProgressDialog >             progressDialog_;
    boost::shared_ptr< QProcess >                  process_;
    std::unique_ptr< gui::EntitySymbols >          icons_;
    std::unique_ptr< gui::TextEditor >             textEditor_;
    //@}
};

#endif // __MainWindow_h_

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
}

namespace gui
{
    class CircularEventStrategy;
    class ColorStrategy;
    class EntitySymbols;
    class ExclusiveEventStrategy;
    class GLMainProxy;
    class GLWidgetManager;
    class HelpSystem;
    class Layer_ABC;
    class Painter_ABC;
    class ParametersLayer;
    class SymbolIcons;
    class TerrainPicker;
    class TextEditor;
}

class ColorController;
class DialogContainer;
class DockContainer;
class Menu;
class Model;
class ModelBuilder;
class PrepaConfig;
class StaticModel;
class ToolbarContainer;

// =============================================================================
/** @class  MainWindow
    @brief  Main window of the application.
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
             MainWindow( kernel::Controllers& controllers,
                         StaticModel& staticModel,
                         Model& model,
                         PrepaConfig& config,
                         const QString& expiration );
    virtual ~MainWindow();
    //@}

    //! @name Operations
    //@{
    QMessageBox::StandardButton CheckSaving( bool checkConsistency = false );
    void Initialize();
    //@}

public slots:
    //! @name Slots
    //@{
    void New();
    void Open();
    bool Close( bool askForSaving = true );
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
    void CreateLayers( const std::shared_ptr< gui::ParametersLayer >& parameters,
                       const std::shared_ptr< gui::Layer_ABC >& locations,
                       const std::shared_ptr< gui::Layer_ABC >& weather,
                       const std::shared_ptr< gui::Layer_ABC >& profilerLayer,
                       gui::TerrainPicker& picker );
    // Load exercise and generate score if needed
    void LoadExercise();
    // Initialize config with filename if needed, Load static model, LoadExercise, then load gui
    void FullLoad( const tools::Path& filename = tools::Path() );
    // Close if needed, then FullLoad
    void Reload( const tools::Path& filename );

    void GenerateScores();
    void MigrateExercises();
    bool MigrateExercise( const tools::Path& path );
    void SetWindowTitle( bool needsSaving );
    void SetNeedsSaving( bool status );
    void SetProgression( int value, const QString& text );
    void CatchException( const QString& msg, const std::exception& e );

    virtual void closeEvent( QCloseEvent* event );

    virtual void NotifyModeChanged( E_Modes newMode );
    virtual void NotifyCreated();
    virtual void NotifyUpdated();
    virtual void NotifyDeleted();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    StaticModel&         staticModel_;
    Model&               model_;
    PrepaConfig&         config_;
    bool                 loadingExercise_;
    bool                 needsSaving_;

    std::unique_ptr< gui::TextEditor >             textEditor_; // should move in parameter layers
    boost::shared_ptr< QProcess >                  process_; // should move in layers panel

    std::shared_ptr< gui::GLMainProxy >            glProxy_;
    std::unique_ptr< gui::GLWidgetManager >        glWidgetManager_;
    std::unique_ptr< ModelBuilder >                modelBuilder_;
    std::unique_ptr< gui::CircularEventStrategy >  forward_;
    std::unique_ptr< gui::ExclusiveEventStrategy > eventStrategy_;
    std::unique_ptr< ColorController >             colorController_;
    std::unique_ptr< DockContainer >               dockContainer_;
    std::unique_ptr< DialogContainer >             dialogContainer_;
    std::unique_ptr< ToolbarContainer >            toolbarContainer_;
    std::unique_ptr< gui::ColorStrategy >          strategy_;
    std::unique_ptr< Menu >                        menu_;
    std::unique_ptr< QProgressDialog >             progressDialog_;
    std::unique_ptr< gui::EntitySymbols >          icons_;
    std::unique_ptr< gui::SymbolIcons >            symbolIcons_;
    //@}
};

#endif // __MainWindow_h_

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
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
    class GlProxy;
    class GLStackedWidget;
    class HelpSystem;
    class Layer_ABC;
    class Painter_ABC;
    class ParametersLayer;
    class SelectionMenu;
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
    void CreateLayers( const std::shared_ptr< gui::ParametersLayer >& parameters,
                       const std::shared_ptr< gui::Layer_ABC >& locations,
                       const std::shared_ptr< gui::Layer_ABC >& weather,
                       const std::shared_ptr< gui::Layer_ABC >& profilerLayer,
                       gui::TerrainPicker& picker );
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
    PrepaConfig&         config_;
    bool                 loading_;
    bool                 needsSaving_;

    std::unique_ptr< gui::TextEditor >             textEditor_; // should move in parameter layers
    boost::shared_ptr< QProcess >                  process_; // should move in layers panel

    std::shared_ptr< gui::GlProxy >                glProxy_;
    std::shared_ptr< gui::SelectionMenu >          selectionMenu_;
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

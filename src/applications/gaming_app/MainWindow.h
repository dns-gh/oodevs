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

#include "clients_kernel/OptionsObserver_ABC.h"
#include "gaming/Simulation.h"
#include "ENT/ENT_Enums.h"
#include <tools/ElementObserver_ABC.h>
#include <boost/shared_ptr.hpp>

namespace kernel
{
    class Controllers;
    class KnowledgeConverter_ABC;
    class Profile_ABC;
    class Workers;
}

namespace gui
{
    class AddRasterDialog;
    class AgentsLayer;
    class AutomatsLayer;
    class CircularEventStrategy;
    class ColorStrategy;
    class DisplayExtractor;
    class EntitySymbols;
    class ExclusiveEventStrategy;
    class FormationLayer;
    class GLMainProxy;
    class GLWidgetManager;
    class HelpSystem;
    class Layer_ABC;
    class LightingProxy;
    class Painter_ABC;
    class ParametersLayer;
    class PreferencesDialog;
    class RichToolBar;
    class SelectionMenu;
    class SymbolIcons;
    class TerrainPicker;
    class TextEditor;
}

namespace tools
{
    class Path;
}

class GamingConfig;
class Services;
class StatusBar;
class Model;
class StaticModel;
class Profile;
class Network;
class LoggerProxy;
class Simulation;
class SimulationController;
class ColorController;
class ProfileFilter;
class DockContainer;
class IndicatorPlotFactory;
class FirePlayer;
class LockMapViewController;
class DrawingsBuilder;
class UnitStateDialog;

// =============================================================================
/** @class  MainWindow
    @brief  Main window of the application.
*/
// Created: APE 2004-03-01
// =============================================================================
class MainWindow : public QMainWindow
                 , public tools::Observer_ABC
                 , public tools::ElementObserver_ABC< Simulation >
                 , public tools::ElementObserver_ABC< Simulation::Reconnection >
                 , public tools::ElementObserver_ABC< Services >
                 , public tools::ElementObserver_ABC< Profile >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor/Accessor
    //@{
             MainWindow( kernel::Controllers& controllers,
                         StaticModel& staticModel,
                         Model& model,
                         const Simulation& simulation,
                         SimulationController& simulationController,
                         Network& network,
                         ProfileFilter& profile,
                         GamingConfig& config,
                         LoggerProxy& logger,
                         const kernel::KnowledgeConverter_ABC& converter,
                         kernel::Workers& workers );
    virtual ~MainWindow();
    //@}

    //! @name Operations
    //@{
    //@}

public slots:
    //! @name Slots
    //@{
    void ToggleFullScreen();
    void ToggleDocks();
    void OnAddRaster();
    void PlayPauseSoundControl( bool play );
    //@}

signals:
    //! @name Signals
    //@{
    void ShowHelp();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void closeEvent( QCloseEvent* pEvent );

    void Close();
    void LoadPhysical();
    void LoadGUI();
    void SetProgression( int value, const QString& text );
    void HandleError( const QString& msg, const std::exception& e );

    virtual void NotifyUpdated( const Simulation& simulation );
    virtual void NotifyUpdated( const Simulation::Reconnection& reconnection );
    virtual void NotifyUpdated( const Services& connection );
    virtual void NotifyUpdated( const Profile& profile );

    void CreateLayers( const std::shared_ptr< gui::ParametersLayer >& parameters,
                       const std::shared_ptr< gui::Layer_ABC >& locations,
                       const std::shared_ptr< gui::Layer_ABC >& weather,
                       const std::shared_ptr< gui::Layer_ABC >& profiler,
                       const Simulation& simulation,
                       gui::TerrainPicker& picker );

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    StaticModel& staticModel_;
    Model& model_;
    Network& network_;
    QString login_;
    GamingConfig& config_;
    const kernel::Profile_ABC& profile_;
    kernel::Workers& workers_;

    QByteArray states_;
    bool connected_;
    bool staticModelLoaded_;
    bool onPlanif_;
    QString planifName_;
    QString savedState_;
    E_Modes currentMode_;

    std::unique_ptr< gui::TextEditor > textEditor_; // should move in parameter layer
    boost::shared_ptr< QProcess > process_; // should move in layers panel

    std::unique_ptr< QProgressDialog > progressDialog_;
    std::shared_ptr< gui::GLMainProxy > glProxy_;
    std::unique_ptr< gui::GLWidgetManager > glWidgetManager_;
    std::shared_ptr< gui::SelectionMenu > selectionMenu_;
    std::unique_ptr< gui::CircularEventStrategy > forward_;
    std::unique_ptr< gui::ExclusiveEventStrategy > eventStrategy_;
    std::unique_ptr< ColorController > pColorController_;
    std::unique_ptr< DockContainer > dockContainer_;
    std::unique_ptr< gui::PreferencesDialog > preferenceDialog_;
    std::unique_ptr< gui::EntitySymbols > icons_;
    std::unique_ptr< gui::ColorStrategy > strategy_;
    std::unique_ptr< LockMapViewController > lockMapViewController_;
    std::unique_ptr< StatusBar > pStatus_;
    std::unique_ptr< FirePlayer > firePlayer_;
    std::unique_ptr< IndicatorPlotFactory > plotFactory_;
    std::unique_ptr< DrawingsBuilder > drawingsBuilder_;
    std::unique_ptr< UnitStateDialog > unitStateDialog_;
    std::unique_ptr< gui::DisplayExtractor > displayExtractor_;
    std::unique_ptr< gui::SymbolIcons > symbolIcons_;
    //@}
};

#endif // __MainWindow_h_

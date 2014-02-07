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
#include "clients_kernel/ModesObserver_ABC.h"
#include "gaming/Simulation.h"
#include <tools/ElementObserver_ABC.h>
#include <boost/shared_ptr.hpp>

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class AddRasterDialog;
    class AgentsLayer;
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
    class PreferencesDialog;
    class RichToolBar;
    class TerrainLayer;
    class TerrainPicker;
    class TooltipsLayer_ABC;
}

namespace tools
{
    class Path;
}

class Config;
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
class OrbatDockWidget;
class ProfileFilter;
class DockContainer;
class IndicatorPlotFactory;
class FirePlayer;
class LockMapViewController;

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
                 , public kernel::ModesObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor/Accessor
    //@{
             MainWindow( kernel::Controllers& controllers, StaticModel& staticModel, Model& model, const Simulation& simulation,
                         SimulationController& simulationController, Network& network, ProfileFilter& profile,
                         Config& config, LoggerProxy& logger, const QString& license );
    virtual ~MainWindow();
    //@}

    //! @name Operations
    //@{
    void Load();
    //@}

public slots:
    //! @name Slots
    //@{
    void Close();
    void ToggleFullScreen();
    void ToggleDocks();
    void OnAddRaster();
    void OnRasterProcessExited( int exitCode, const tools::Path& output );
    void OnGenerateSymbols();
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
    void closeEvent( QCloseEvent* pEvent );

    virtual void NotifyModeChanged( E_Modes newMode );
    virtual void NotifyUpdated( const Simulation& simulation );
    virtual void NotifyUpdated( const Simulation::Reconnection& reconnection );
    virtual void NotifyUpdated( const Services& connection );
    virtual void NotifyUpdated( const Profile& profile );

    static std::string BuildRemotePath( std::string server, std::string path );

    void CreateLayers( gui::Layer& locationsLayer, gui::Layer& weather, gui::Layer& profilerLayer,
                       gui::Layer& automats, gui::Layer& formationLayer, const Simulation& simulation, gui::TerrainPicker& picker,
                       gui::Elevation2dLayer& elevation2dLayer );
    void AddLayer( gui::Layer& layer, const std::string& passes = "", const QString& text = "" );
    void AddTooltipLayer( gui::TooltipsLayer_ABC& layer );

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    StaticModel& staticModel_;
    Model& model_;
    Network& network_;
    QString login_;
    Config& config_;
    const kernel::Profile_ABC& profile_;
    std::auto_ptr< gui::GraphicPreferences > graphicPreferences_;
    std::auto_ptr< gui::CircularEventStrategy > forward_;
    std::auto_ptr< gui::ExclusiveEventStrategy > eventStrategy_;
    std::auto_ptr< gui::Painter_ABC > pPainter_;
    std::auto_ptr< ColorController > pColorController_;
    std::auto_ptr< DockContainer > dockContainer_;
    boost::shared_ptr< QProcess > process_;
    std::auto_ptr< gui::AddRasterDialog > addRasterDialog_;
    std::auto_ptr< gui::PreferencesDialog > preferenceDialog_;
    std::auto_ptr< gui::EntitySymbols > icons_;
    std::auto_ptr< gui::GlProxy > glProxy_;
    std::auto_ptr< gui::ColorStrategy > strategy_;
    std::auto_ptr< gui::LightingProxy > lighting_;
    std::auto_ptr< gui::GlSelector > selector_;
    std::auto_ptr< LockMapViewController > lockMapViewController_;
    std::auto_ptr< StatusBar > pStatus_;
    std::auto_ptr< FirePlayer > firePlayer_;
    std::auto_ptr< IndicatorPlotFactory > plotFactory_;
    gui::ParametersLayer* parameters_; // $$$$ ABR 2013-02-14: Can't make an auto_ptr of this one, because every layers are destroyed into GlProxy destructor.
    QByteArray states_;
    bool connected_;
    bool onPlanif_;

    QString planifName_;
    QString savedState_;
    //@}
};

#endif // __MainWindow_h_

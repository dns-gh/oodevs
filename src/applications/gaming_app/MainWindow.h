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

#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/ModesObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "gaming/Simulation.h"

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
    class EntitySymbols;
    class ExclusiveEventStrategy;
    class FormationLayer;
    class GlProxy;
    class GlSelector;
    class HelpSystem;
    class Layer;
    class LightingProxy;
    class Painter_ABC;
    class ParametersLayer;
    class PreferencesDialog;
    class RichToolBar;
    class TerrainLayer;
    class TerrainPicker;
}

class Config;
class Services;
class StatusBar;
class Model;
class StaticModel;
class Profile;
class Network;
class MissionPanel;
class LoggerProxy;
class Simulation;
class ColorController;
class OrbatDockWidget;
class ProfileFilter;
class DockContainer;
class IndicatorPlotFactory;

// =============================================================================
/** @class  MainWindow
    @brief  Main window of the application.
*/
// Created: APE 2004-03-01
// =============================================================================
class MainWindow : public QMainWindow
                 , public tools::Observer_ABC
                 , public tools::ElementObserver_ABC< Simulation >
                 , public tools::ElementObserver_ABC< Services >
                 , public tools::ElementObserver_ABC< Profile >
                 , public kernel::ModesObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor/Accessor
    //@{
             MainWindow( kernel::Controllers& controllers, StaticModel& staticModel, Model& model, const Simulation& simulation,
                         Network& network, const kernel::Profile_ABC& profile, Config& config, LoggerProxy& logger,
                         const QString& license );
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
    void OnRasterProcessExited( int exitCode, QProcess::ExitStatus exitStatus );
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
    virtual void NotifyUpdated( const Services& connection );
    virtual void NotifyUpdated( const Profile& profile );

    void GeneratePixmapSymbols();
    static std::string BuildRemotePath( std::string server, std::string path );

    void CreateLayers( gui::Layer& locationsLayer, gui::Layer& weather, gui::Layer& profilerLayer,
                       gui::Layer& automats, gui::Layer& formationLayer, const Simulation& simulation, gui::TerrainPicker& picker );

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    StaticModel& staticModel_;
    Model& model_;
    Network& network_;
    QString profile_;
    Config& config_;
    std::auto_ptr< ProfileFilter > pProfile_;
    std::auto_ptr< gui::CircularEventStrategy > forward_;
    std::auto_ptr< gui::ExclusiveEventStrategy > eventStrategy_;
    std::auto_ptr< gui::Painter_ABC > pPainter_;
    std::auto_ptr< ColorController > pColorController_;
    std::auto_ptr< DockContainer > dockContainer_;
    std::auto_ptr< QProcess > process_;
    std::auto_ptr< gui::AddRasterDialog > addRasterDialog_;
    std::auto_ptr< gui::PreferencesDialog > preferenceDialog_;
    std::auto_ptr< gui::EntitySymbols > icons_;
    std::auto_ptr< gui::GlProxy > glProxy_;
    std::auto_ptr< gui::ColorStrategy > strategy_;
    std::auto_ptr< gui::LightingProxy > lighting_;
    std::auto_ptr< gui::GlSelector > selector_;
    std::auto_ptr< StatusBar > pStatus_;
    std::auto_ptr< IndicatorPlotFactory > plotFactory_;
    gui::ParametersLayer* parameters_; // $$$$ ABR 2013-02-14: Can't make an auto_ptr of this one, because every layers are destroyed into GlProxy destructor.
    QByteArray docks_;
    QByteArray toolbars_;
    bool connected_;
    bool onPlanif_;

    QString planifName_;
    QString savedState_;
    //@}
};

#endif // __MainWindow_h_

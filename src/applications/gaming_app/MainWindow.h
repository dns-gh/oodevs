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
#include "tools/ElementObserver_ABC.h"
#include "gaming/Simulation.h"

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;
    }
}

namespace kernel
{
    class Options;
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class AddRasterDialog;
    class OptionsPanel;
    class ParametersLayer;
    class AgentsLayer;
    class AutomatsLayer;
    class FormationLayer;
    class ColorStrategy;
    class ExclusiveEventStrategy;
    class EntitySymbols;
    class FormationLayer;
    class CircularEventStrategy;
    class PreferencesDialog;
    class LightingProxy;
    class GlProxy;
    class GlSelector;
    template< typename T >
    class LogisticList;
    class TerrainLayer;
    class Layer;
    class Picker;
    class TerrainPicker;
    class Painter_ABC;
    class HelpSystem;
}

class AfterAction;
class Config;
class Services;
class StatusBar;
class Model;
class StaticModel;
class Profile;
class Network;
class MissionPanel;
class CreationPanels;
class LogisticListView;
class LoginDialog;
class Publisher_ABC;
class LoggerProxy;
class Simulation;
class RcEntityResolver_ABC;
class ColorController;
class PlanificationModePanel;
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
                 , public tools::ElementObserver_ABC< Services >
                 , public tools::ElementObserver_ABC< Profile >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor/Accessor
    //@{
             MainWindow( kernel::Controllers& controllers, StaticModel& staticModel, Model& model, const Simulation& simulation,
                         Network& network, const kernel::Profile_ABC& profile, Config& config, LoggerProxy& logger,
                         const RcEntityResolver_ABC& rcResolver, const QString& license );
    virtual ~MainWindow();
    //@}

    //! @name Operations
    //@{
    void Load();
    virtual QMenu* createPopupMenu();
    //@}

public slots:
    //! @name Slots
    //@{
    void ShowHelp();
    void Close();
    void OnPlanifStateChange();
    void ToggleFullScreen();
    void ToggleDocks();
    void OnAddRaster();
    void OnRasterProcessExited( int exitCode, QProcess::ExitStatus exitStatus );
    //@}

private:
    //! @name Helpers
    //@{
    void closeEvent( QCloseEvent* pEvent );

    void WriteOptions();
    void ReadOptions();

    virtual void NotifyUpdated( const Simulation& simulation );
    virtual void NotifyUpdated( const Services& connection );
    virtual void NotifyUpdated( const Profile& profile );

    static std::string BuildRemotePath( std::string server, std::string path );

    void CreateLayers( MissionPanel& missions, CreationPanels& creationPanels, gui::ParametersLayer& parameters, gui::Layer& locationsLayer,
                       gui::AgentsLayer& agents, gui::AutomatsLayer& automats, gui::FormationLayer& formationLayer, gui::TerrainLayer& terrain, gui::Layer& weather, gui::Layer& profilerLayer,
                       gui::PreferencesDialog& preferences, const kernel::Profile_ABC& profile, const Simulation& simulation, gui::TerrainPicker& picker );

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    StaticModel& staticModel_;
    Model& model_;
    Network& network_;
    QString profile_;
    Config& config_;
    std::auto_ptr< gui::CircularEventStrategy > forward_;
    std::auto_ptr< gui::ExclusiveEventStrategy > eventStrategy_;
    std::auto_ptr< gui::Painter_ABC > pPainter_;
    std::auto_ptr< ColorController > pColorController_;
    std::auto_ptr< actions::gui::InterfaceBuilder_ABC > interfaceBuilder_;
    std::auto_ptr< QProcess > process_;
    std::auto_ptr< gui::AddRasterDialog > addRasterDialog_;
    std::auto_ptr< gui::PreferencesDialog > preferenceDialog_;
    std::auto_ptr< gui::EntitySymbols > icons_;
    std::auto_ptr< gui::GlProxy > glProxy_;
    std::auto_ptr< gui::ColorStrategy > strategy_;
    std::auto_ptr< gui::LightingProxy > lighting_;
    std::auto_ptr< gui::GlSelector > selector_;
    std::auto_ptr< LockMapViewController > lockMapViewController_;
    gui::OptionsPanel* pOptionsPanel_;
    gui::ParametersLayer* parameters_;
    gui::AgentsLayer* agents_;
    ::LogisticListView* logisticListView_;
    StatusBar* pStatus_;
    QByteArray docks_;
    QByteArray toolbars_;
    bool connected_;
    bool onPlanif_;
    MissionPanel* pMissionPanel_;
    QDockWidget* pExtensionsPanel_;
    PlanificationModePanel* pPlanificationModePanel_;
    QString planifName_;
    QString savedState_;
    gui::HelpSystem* help_;
    AfterAction* aar_;
    //@}
};

#endif // __MainWindow_h_

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
#include "tools/ElementObserver_ABC.h"
#include "gaming/Simulation.h"
#include <qmainwindow.h>

namespace kernel
{
    class Options;
    class Controllers;
    class Profile_ABC;
}

namespace tools
{
    class SessionConfig;
}

namespace gui
{
    class OptionsPanel;
    class ParametersLayer;
    class AgentsLayer;
    class AutomatsLayer;
    class ColorStrategy;
    class GlProxy;
    class ExclusiveEventStrategy;
    class CircularEventStrategy;
    class PreferencesDialog;
    class LightingProxy;
    class GlSelector;
    class TerrainLayer;
    class Layer_ABC;
    class Picker;
    class ElevationResolver_ABC;
}

class Services;
class StatusBar;
class Model;
class StaticModel;
class Profile;
class Network;
class MissionPanel;
class CreationPanels;
class LoginDialog;
class Publisher_ABC;
class LoggerProxy;
class Simulation;

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
                         Network& network, const kernel::Profile_ABC& profile, tools::SessionConfig& config, LoggerProxy& logger,
                         const QString& license );
    virtual ~MainWindow();
    //@}

    //! @name Operations
    //@{
    void Load();
    kernel::Options& GetOptions() const;
    //@}

public slots:
    //! @name Slots
    //@{
    void Open();
    void Close();
    void OnPlanifStateChange();
    void OnNameChanged();
    //@}

private:
    //! @name Helpers
    //@{
    void closeEvent( QCloseEvent* pEvent );
    void WriteSettings();
    void ReadSettings();

    void WriteOptions();
    void ReadOptions();

    virtual void NotifyUpdated( const Simulation& simulation );
    virtual void NotifyUpdated( const Services& connection );
    virtual void NotifyUpdated( const Profile& profile );

    static std::string BuildRemotePath( std::string server, std::string path );

   void CreateLayers( MissionPanel& missions, CreationPanels& creationPanels, gui::ParametersLayer& parameters, gui::Layer_ABC& locationsLayer,
       gui::AgentsLayer& agents, gui::AutomatsLayer& automats, gui::TerrainLayer& terrain, gui::Layer_ABC& weather, gui::Layer_ABC& profilerLayer,
        gui::PreferencesDialog& preferences, const kernel::Profile_ABC& profile, const Simulation& simulation );

    //! @name Copy/Assignment
    //@{
    MainWindow( const MainWindow& );
    MainWindow& operator=( const MainWindow& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    StaticModel& staticModel_;
    Model& model_;
    Network& network_;
    QString profile_;
    tools::SessionConfig& config_;
    std::auto_ptr< gui::CircularEventStrategy >  forward_;
    std::auto_ptr< gui::ExclusiveEventStrategy > eventStrategy_;
    std::auto_ptr< gui::ElevationResolver_ABC > pResolver_;
    gui::GlProxy* glProxy_;
    gui::ColorStrategy* strategy_;
    gui::LightingProxy* lighting_;
    gui::GlSelector* selector_;
    gui::OptionsPanel* pOptionsPanel_;
    gui::ParametersLayer* parameters_;
    gui::AgentsLayer* agents_;
    StatusBar* pStatus_;
    bool connected_;
    MissionPanel* pMissionPanel_;
    QString               planifName_;
    bool                  onPlanif_;
    //@}
};

#endif // __MainWindow_h_

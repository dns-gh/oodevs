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
#include "clients_kernel/ElementObserver_ABC.h"
#include "gaming/Simulation.h"
#include <qmainwindow.h>

namespace kernel
{
    class Options;
    class Controllers;
    class Profile_ABC;
    class ExerciseConfig;
}

namespace gui
{
    class OptionsPanel;
    class ParametersLayer;
    class AgentsLayer;
    class AutomatsLayer;
    class ColorStrategy;
    class GlProxy;
    class DrawerLayer;
    class ExclusiveEventStrategy;
    class CircularEventStrategy;
    class PreferencesDialog;
    class LightingProxy;
    class GlSelector;
    class LocationsLayer;
}

class Spy;
class Model;
class StaticModel;
class Profile;
class Network;
class MissionPanel;
class CreationPanels;
class LoginDialog;
class Publisher_ABC;
class FlexLmLicense;

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
                 , public kernel::Observer_ABC
                 , public kernel::ElementObserver_ABC< Simulation >
                 , public kernel::ElementObserver_ABC< Simulation::sConnection >
                 , public kernel::ElementObserver_ABC< Profile >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor/Accessor
    //@{
             MainWindow( kernel::Controllers& controllers, StaticModel& staticModel, Model& model, const Simulation& simulation, Network& network, const kernel::Profile_ABC& profile, kernel::ExerciseConfig& config, const FlexLmLicense* license );
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
    virtual void NotifyUpdated( const Simulation::sConnection& connection );
    virtual void NotifyUpdated( const Profile& profile );

    static std::string BuildRemotePath( std::string server, std::string path );

    void CreateLayers( MissionPanel& missions, CreationPanels& creationPanels, gui::ParametersLayer& parameters, gui::LocationsLayer& locationsLayer,
                       gui::AgentsLayer& agents, gui::AutomatsLayer& automats, gui::DrawerLayer& drawer, gui::PreferencesDialog& preferences, const kernel::Profile_ABC& profile, Publisher_ABC& publisher );
    //@}

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
    kernel::ExerciseConfig& config_;

    std::auto_ptr< gui::CircularEventStrategy >  forward_;
    std::auto_ptr< gui::ExclusiveEventStrategy > eventStrategy_;
    gui::GlProxy*                glProxy_;
    gui::ColorStrategy*          strategy_;
    gui::LightingProxy*          lighting_;

    gui::GlSelector* selector_;

    gui::OptionsPanel*    pOptionsPanel_;
    gui::ParametersLayer* parameters_;
    gui::AgentsLayer*     agents_;
    Spy*      spy_;
    //@}
};

#endif // __MainWindow_h_

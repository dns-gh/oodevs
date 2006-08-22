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
#include <qmainwindow.h>

namespace kernel
{
    class Options;
    class Controllers;
}

namespace gui
{
    class OptionsPanel;
    class GlWidget;
    class Gl3dWidget;
    class StatusBar;
    class ParametersLayer;
    class AgentsLayer;
    class GlPlaceHolder;
    class IconLayout;
    class ColorStrategy;
    class GlProxy;
}

class Spy; // $$$$ AGE 2006-08-02: Duh !
class Model;
class StaticModel;
class Simulation;
class Network;
class MissionPanel;
class ObjectCreationPanel;
class GraphicSetup_ABC;
class EventStrategy_ABC;

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
                 , public kernel::OptionsObserver_ABC
                 , public kernel::ElementObserver_ABC< Simulation >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor/Accessor
    //@{
             MainWindow( kernel::Controllers& controllers, StaticModel& staticModel, Model& model, Network& network );
    virtual ~MainWindow();
    //@}

    //! @name Operations
    //@{
    void Load( const std::string& scipioXml );
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

    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    virtual void NotifyUpdated( const Simulation& simulation );

    void CompareConfigPath( const std::string& server, const std::string& serverPath );
    static std::string BuildRemotePath( std::string server, std::string path );

    void BuildIconLayout();

    void CreateLayers( MissionPanel& missions, ObjectCreationPanel& objects, gui::ParametersLayer& parameters, gui::AgentsLayer& agents, GraphicSetup_ABC& setup );
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
    std::string scipioXml_;

    gui::GlProxy*       glProxy_;
    gui::ColorStrategy* strategy_;
    EventStrategy_ABC* eventStrategy_;
    gui::GlWidget*      widget2d_;
    gui::Gl3dWidget*    widget3d_;
    gui::GlPlaceHolder* glPlaceHolder_;
    gui::IconLayout* iconLayout_;

    bool b3d_;

    QTimer* displayTimer_;

    gui::OptionsPanel*    pOptionsPanel_;
    gui::StatusBar*       pStatus_;
    gui::ParametersLayer* parameters_;
    gui::AgentsLayer*     agents_;
    Spy*      spy_;
    //@}
};

#endif // __MainWindow_h_

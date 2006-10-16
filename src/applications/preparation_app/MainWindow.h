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

#include <qmainwindow.h>

namespace kernel
{
    class Options;
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class OptionsPanel;
    class GlProxy;
    class GlWidget;
    class StatusBar;
    class ParametersLayer;
    class GlPlaceHolder;
    class IconLayout;
    class ColorStrategy;
}

class AgentsLayer;
class Model;
class ModelBuilder;
class StaticModel;
class GraphicSetup_ABC;
class EventStrategy_ABC;
class ObjectCreationPanel;

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
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor/Accessor
    //@{
             MainWindow( kernel::Controllers& controllers, StaticModel& staticModel, Model& model );
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
    void New();
    void Open();
    void Close();
    void Save();
    //@}

private:
    //! @name Helpers
    //@{
    void BuildIconLayout();
    void CreateLayers( ObjectCreationPanel& objects, gui::ParametersLayer& parameters, AgentsLayer& agents, GraphicSetup_ABC& setup, const kernel::Profile_ABC& profile );
    void closeEvent( QCloseEvent* pEvent );
    void WriteSettings();
    void ReadSettings();

    void WriteOptions();
    void ReadOptions();
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
    ModelBuilder* modelBuilder_;
    std::string scipioXml_;

    gui::GlProxy*       glProxy_;
    gui::ColorStrategy* strategy_;
    EventStrategy_ABC* eventStrategy_;
    gui::GlWidget*   widget2d_;
    gui::GlPlaceHolder* glPlaceHolder_;
    gui::IconLayout* iconLayout_;

    QTimer* displayTimer_;

    gui::OptionsPanel*    pOptionsPanel_;
    gui::StatusBar*       pStatus_;
    gui::ParametersLayer* parameters_;
    AgentsLayer*     agents_;
    //@}
};

#endif // __MainWindow_h_

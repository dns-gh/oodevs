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

#include "OptionsObserver_ABC.h"
#include "ElementObserver_ABC.h"

class MapEventHandler_ABC;
class Options;
class MapEventFilter_ABC;
class OptionsPanel;

class QProgressBar;
class Controllers;
class Model;
class GlLayers;
class GlWidget;
class Gl3dWidget;
class StatusBar;
class Simulation;
class ParametersLayer;
class AgentsLayer;
class MsgRecorder;
class GlPlaceHolder;

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
                 , public Observer_ABC
                 , public OptionsObserver_ABC
                 , public ElementObserver_ABC< Simulation >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor/Accessor
    //@{
             MainWindow( Controllers& controllers, Model& model, MsgRecorder& recorder );
    virtual ~MainWindow();
    //@}

    //! @name Operations
    //@{
    void Load( const std::string& scipioXml );
    Options& GetOptions() const;
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

    virtual void OptionChanged( const std::string& name, const OptionVariant& value );
    virtual void NotifyUpdated( const Simulation& simulation );

    void InitializeHumanFactors( const std::string& scipioXml );
    //@}

    //! @name Copy/Assignment
    //@{
    MainWindow( const MainWindow& );
    MainWindow& operator=( const MainWindow& );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    Model& model_;
    std::string scipioXml_;

    GlLayers* layers_;
    GlWidget*   widget2d_;
    Gl3dWidget* widget3d_;
    GlPlaceHolder* glPlaceHolder_;

    bool b3d_;

    QTimer* displayTimer_;

    OptionsPanel*  pOptionsPanel_;
    StatusBar*     pStatus_;
    ParametersLayer* parameters_;
    AgentsLayer*     agents_;

    QLabel* portrait_;
    //@}
};

#endif // __MainWindow_h_

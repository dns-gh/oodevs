// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-01 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/MainWindow.h $
// $Author: Age $
// $Modtime: 17/05/05 14:05 $
// $Revision: 14 $
// $Workfile: MainWindow.h $
//
// *****************************************************************************

#ifndef __MainWindow_h_
#define __MainWindow_h_

#include "OptionsObserver_ABC.h"

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
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor/Accessor
    //@{
             MainWindow( Controllers& controllers, Model& model, const std::string& scipioXml );
    virtual ~MainWindow();
    //@}

    //! @name Operations
    //@{
    Options& GetOptions() const;
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
    bool b3d_;

    QTimer* displayTimer_;

    OptionsPanel*  pOptionsPanel_;
    StatusBar*     pStatus_;
    //@}
};

#endif // __MainWindow_h_

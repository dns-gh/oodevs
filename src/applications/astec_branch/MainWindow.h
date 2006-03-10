// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
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

class GL3DWidget;
class GLWidget;
class MapEventHandler_ABC;
class Options;
class MapEventFilter_ABC;
class OptionsPanel;

class QProgressBar;
class Controller;
class ActionController;

#include "MapEventFilter_ABC.h"

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
    MT_COPYNOTALLOWED( MainWindow )
    friend class GLTool;

public:
    //! @name Constructors/Destructor/Accessor
    //@{
    explicit MainWindow( Controller& controller );
    virtual ~MainWindow();

    static MainWindow& GetMainWindow();
    //@}

    //! @name Operations
    //@{
    void Update();

    void PushMapEventFilter( MapEventFilter_ABC& filter );
    void PopMapEventFilter( MapEventFilter_ABC& filter );
    const T_MapEventFilterVector& GetMapEvents() const;

    Options& GetOptions() const;

    QGLWidget* GetQGLWidget( bool b3Dmode ) const;
    //@}


signals:
    void CenterOnPoint( const MT_Vector2D& vPoint );
    void TeamChanged();

private:
    void closeEvent( QCloseEvent* pEvent );
    void WriteSettings();
    void ReadSettings();

    void WriteOptions();
    void ReadOptions();

private:
    //! @name Member data
    //@{
    ActionController& actionController_;
    Options* pOptions_;
    OptionsPanel* pOptionsPanel_;

    QDockWindow* pListDockWnd_;
    QDockWindow* pInfoDockWnd_;
    QDockWindow* pLogDockWnd_;
    QDockWindow* pWeatherDockWnd_;
    QDockWindow* pCreationDockWnd_;
    QDockWindow* pOptionsDockWnd_;

    QLabel* pTimeLabel_;
    QLabel* pLocationLabel_;
    QLabel* pLocationLabel2_;
	QLabel* pLocationLabel3_;
    QLabel* pTickLabel_;

    QTimer* pLagTimer_;
    //@}

private:
    static MainWindow* pInstance_;
};

#   include "MainWindow.inl"

#endif // __MainWindow_h_

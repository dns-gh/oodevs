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

#ifdef __GNUG__
#   pragma interface
#endif

class GL3DWidget;
class GLWidget;
class InfoPanel;
class AgentListView;
class ObjectListView;
class PopulationListView;
class MissionPanel;
class MapEventHandler_ABC;
class SelectedElement;
class ActionContext;
class WeatherPanel;
class Logger;
class Options;
class ObjectCreationPanel;
class AgentCreationPanel;
class PopulationCreationPanel;
class Agent_ABC;
class Agent;
class KnowledgeGroup;
class MapEventFilter_ABC;

class QProgressBar;
class Controller;

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
    void ElementSelected( SelectedElement& selectedElement );
    void ElementHovered( SelectedElement& hoveredElement );
    void NewPopupMenu( QPopupMenu& popupMenu, const ActionContext& context );
    void CenterOnPoint( const MT_Vector2D& vPoint );
    void TeamChanged();
    void ReadingReports( Agent_ABC& agent );
    void ODBNewAgent( const std::string& strCategory, KnowledgeGroup& gtia, Agent* pParent );

private:
    void WriteSettings();
    void ReadSettings();

    void WriteOptions();
    void ReadOptions();

private:
    //! @name Member data
    //@{
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

    Options* pOptions_;
    //@}

private:
    static MainWindow* pInstance_;
};

#   include "MainWindow.inl"

#endif // __MainWindow_h_

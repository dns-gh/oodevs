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
class ObjectCreationPanel;
class AgentCreationPanel;
class PopulationCreationPanel;
class Agent_ABC;
class Agent;
class Gtia;
class MapEventFilter_ABC;

class QProgressBar;

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
     MainWindow();
    ~MainWindow();

    static MainWindow& GetMainWindow();
    //@}

    //! @name Operations
    //@{
    void Update();

    void PushMapEventFilter( MapEventFilter_ABC& filter );
    void PopMapEventFilter( MapEventFilter_ABC& filter );
    const T_MapEventFilterVector& GetMapEvents() const;

    QGLWidget* GetQGLWidget( bool b3Dmode ) const;
    //@}

public slots:
    void DisplayTime( uint nTimeInSeconds );
    void DisplayMouseLocation( QMouseEvent* pEvent, const MT_Vector2D& vGLPos );
    void OnTickStartEnd( bool bTickStart, uint tick );
    void OnLag();
    void OnConnexionStatusChanged( bool b );
    void DimensionsChanged();

signals:
    void ElementSelected( SelectedElement& selectedElement );
    void ElementHovered( SelectedElement& hoveredElement );
    void NewPopupMenu( QPopupMenu& popupMenu, const ActionContext& context );
    void CenterOnPoint( const MT_Vector2D& vPoint );
    void TeamChanged();
    void ReadingReports( Agent_ABC& agent );
    void ODBNewAgent( const std::string& strCategory, Gtia& gtia, Agent* pParent );

private:
    void closeEvent( QCloseEvent* pEvent );

    void WriteSettings();
    void ReadSettings();

private:
    //! @name Member data
    //@{
    GL3DWidget*	pGL3DWidget_;
    GLWidget*	pGLWidget_;

    InfoPanel*				 pInfoPanel_;
    AgentListView*			 pAgentList_;
    ObjectListView*			 pObjectList_;
	PopulationListView*		 pPopulationList_;
    MissionPanel*			 pMissionPanel_;
    Logger*					 pLogPanel_;
    WeatherPanel*			 pWeatherPanel_;
    ObjectCreationPanel*	 pObjCreationPanel_;
    AgentCreationPanel*		 pAgentCreationPanel_;
    PopulationCreationPanel* pPopulationCreationPanel_;

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
    QLabel* pNetworkStats_;
    uint    lastTransfered_;

    QTimer* pLagTimer_;
    //@}

private:
    static MainWindow* pInstance_;
};

#   include "MainWindow.inl"

#endif // __MainWindow_h_

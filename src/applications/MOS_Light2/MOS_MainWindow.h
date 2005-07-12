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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_MainWindow.h $
// $Author: Age $
// $Modtime: 17/05/05 14:05 $
// $Revision: 14 $
// $Workfile: MOS_MainWindow.h $
//
// *****************************************************************************

#ifndef __MOS_MainWindow_h_
#define __MOS_MainWindow_h_

#ifdef __GNUG__
#   pragma interface
#endif

class MOS_GL3DWidget;
class MOS_GLWidget;
class MOS_InfoPanel;
class MOS_AgentListView;
class MOS_DynaObjectListView;
class MOS_MissionPanel;
class MOS_MapEventHandler_ABC;
class MOS_SelectedElement;
class MOS_ActionContext;
class MOS_WeatherPanel;
class MOS_Logger;
class MOS_Options;
class MOS_ObjectCreationPanel;
class MOS_AgentCreationPanel;
class MOS_Agent;
class MOS_Gtia;
class MOS_MapEventFilter_ABC;

class QProgressBar;

#include "MOS_MapEventFilter_ABC.h"

// =============================================================================
/** @class  MOS_MainWindow
    @brief  Main window of the application.

    The main window serves as a transit point for nearly all GUI QT signals, whether
    they are inter-GUI (ie. one GUI element signaling the others) or GUI-data.
    This allows GUI objects to be independant: The only other element they know of
    is the main window, and thus they can be moved around in the class hierachy
    without having to redo the connexions every time.
*/
// Created: APE 2004-03-01
// =============================================================================
class MOS_MainWindow : public QMainWindow
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_MainWindow )
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor/Accessor
    //@{
     MOS_MainWindow();
    ~MOS_MainWindow();

    static MOS_MainWindow& GetMainWindow();
    //@}

    //! @name Operations
    //@{
    void Update();

    void PushMapEventFilter( MOS_MapEventFilter_ABC& filter );
    void PopMapEventFilter( MOS_MapEventFilter_ABC& filter );
    const T_MapEventFilterVector& GetMapEvents() const;

    MOS_Options& GetOptions() const;
    //@}

public slots:
    void DisplayTime( uint nTimeInSeconds );
    void DisplayMouseLocation( QMouseEvent* pEvent, const MT_Vector2D& vGLPos );
    void OnTickStartEnd( bool bTickStart );
    void OnLag();
    void OnConnexionStatusChanged( bool b );
    void DimensionsChanged();

signals:
    void ElementSelected( MOS_SelectedElement& selectedElement );
    void NewPopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );
    void CenterOnPoint( const MT_Vector2D& vPoint );
    void TeamChanged();
    void ReadingReports( MOS_Agent& agent );
    void ODBNewAgent( const std::string& strCategory, MOS_Gtia& gtia, MOS_Agent* pParent );

private:
    void closeEvent( QCloseEvent* pEvent );

    void WriteSettings();
    void ReadSettings();

    void WriteOptions();
    void ReadOptions();

private:
    //! @name Member data
    //@{
    MOS_GL3DWidget*     pGL3DWidget_;
    MOS_GLWidget*       pGLWidget_;

    MOS_InfoPanel*      pInfoPanel_;
    MOS_AgentListView*  pAgentList_;
    MOS_DynaObjectListView* pDynaObjectList_;
    MOS_MissionPanel*   pMissionPanel_;
    MOS_Logger*         pLogPanel_;
    MOS_WeatherPanel*   pWeatherPanel_;
    MOS_ObjectCreationPanel* pObjCreationPanel_;
    MOS_AgentCreationPanel* pAgentCreationPanel_;

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

    MOS_Options* pOptions_;
    //@}

private:
    static MOS_MainWindow* pInstance_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_MainWindow.inl"
#endif

#endif // __MOS_MainWindow_h_

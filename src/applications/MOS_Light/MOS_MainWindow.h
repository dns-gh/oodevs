//*****************************************************************************
//
// This file is part of MASA DIN library.
//
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MainWindow.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:54 $
// $Revision: 30 $
// $Workfile: MOS_MainWindow.h $
//
//*****************************************************************************

#ifndef __MOS_MainWindow_h_
#define __MOS_MainWindow_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_LimaEditor.h"
#include "MOS_LimaSelector.h"
#include "MOS_LimitEditor.h"
#include "MOS_LimitSelector.h"
#include "MOS_AgentSelector.h"
#include <qmainwindow.h>

class QStatusBar;
class QLabel;
class QWidgetStack;
class MOS_History;

class MOS_Agent;
class MOS_WorldWidget;


class MOS_AgentEditor;
class MOS_AgentListView;
class MOS_LogWindow;
//class MOS_GraphWindow;
class MOS_LimaListView;
class MOS_LimitListView;
class MOS_AttrEditor;
class MOS_SIMControl;
// $$$ old a vier
//class MOS_SetTimeFactorDialog;
//class MOS_ConnectDialog;


//=============================================================================
// Created: NLD 2002-07-15
//=============================================================================
class MOS_MainWindow : public QMainWindow
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_MainWindow );


    typedef std::stack< QWidget* >          T_WidgetStack;
    typedef const T_WidgetStack             CT_WidgetStack;
public:
    MOS_MainWindow();
    virtual ~MOS_MainWindow();

    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    void Terminate ();
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
//    MOS_AgentsListView& GetAgentsListView () const;
//    QToolBar&           GetToolBar        () const;
    QPopupMenu&         GetMainMenu       () const;
    QMenuBar&           GetMenuBar        () const;
    QStatusBar&         GetStatusBar      () const;
//    MOS_GraphWindow&    GetGraphWindow    () const;
    MOS_WorldWidget&    GetWorldWidget    () const;
    MOS_LimaListView&   GetLimaSelector   () const;
    MOS_LimaListView&   GetLimaEditor     () const;
    MOS_LimitListView&  GetLimitSelector  () const;
    MOS_LimitListView&  GetLimitEditor    () const;
    MOS_AgentEditor&    GetAgentEditor    () const;
    MOS_AttrEditor&     GetAttributeEditor() const;
    MOS_AgentListView&  GetAgentSelector  () const;
    MOS_SIMControl&     GetSimControl     () const;
    MOS_History&        GetHistory        () const;
    //@}


    
    //-------------------------------------------------------------------------
    /** @name WidgetStack*/
    //-------------------------------------------------------------------------
    //@{
    void                UnregisterLeftWidget( QWidget* pWidget );
    void                PushWidgetStackLeft ( QWidget* pWidget );
    void                PopWidgetStackLeft  ();
    QWidgetStack*       GetWidgetStackLeft  () const;

    void                UnregisterRightWidget( QWidget* pWidget );
    void                SetWidgetStackRight( QWidget* pWidget );
    void                PushWidgetStackRight( QWidget* pWidget );
    void                PopWidgetStackRight ();
    QWidgetStack*       GetWidgetStackRight () const;
    //@}
    
    //-------------------------------------------------------------------------
    /** @name Actions */
    //-------------------------------------------------------------------------
    //@{
    void NotifyNetStatistics          ( uint nNbrBytesIn, uint nNbrBytesOut );
    void NotifyDeleteAgent            ( MOS_Agent& agent );
    void NotifyTimeHasChanged         ( uint nTimeInSeconds );
    void NotifyMousePositionHasChanged( const std::string& strMGRSMousePosition, const MT_Vector2D& vPixelMousePos );
    //@}
  

protected slots:
    //-------------------------------------------------------------------------
    /** @name Slots */
    //-------------------------------------------------------------------------
    //@{
/*
    void SlotConnectAction   ();
    void SlotDisconnectAction();
    void SlotStartSIM        ();
    void SlotStopSIM         ();
    void SlotResetSIM        ();
    void SlotPauseSIM        ();
    void SlotResumeSIM       ();
    void SlotSetTimeFactor   ();
    void SlotShowGraphs      ();
*/      //@}

private:
    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    void InitMenus();
    //@}

    //-------------------------------------------------------------------------
    /** @name Events */
    //-------------------------------------------------------------------------
    //@{
    void closeEvent( QCloseEvent* pEvent );
    //@}
    

private:
//    MOS_GraphWindow*         pGraphWindow_;

    // Connection
    MOS_SIMControl*          pSimControl_;


    // Menus en haut de l'écran
//    QToolBar*                pToolBar_;
    QPopupMenu*              pMainMenu_;

    // Info en bas droite de l'écran
    QLabel*                  pSIMTimeLabel_;
    QLabel*                  pMGRSMousePosLabel_;
    QLabel*                  pPixelMousePosLabel_;
    QLabel*                  pNetBytesInLabel_;
    QLabel*                  pNetBytesOutLabel_;

    // OpenGl visu centre de l'écran
    MOS_WorldWidget*         pWorldWidget_;

    // Stack gauche de l'écran
    QWidgetStack*            pToolsWidgetStackLeft_;
    T_WidgetStack            widgetStackLeft_;

    // Stack droite de l'écran
    QWidgetStack*            pToolsWidgetStackRight_;
    T_WidgetStack            widgetStackRight_;

    // Agent
    MOS_AgentEditor*         pAgentEditor_;
    MOS_AgentSelector*       pAgentSelector_;
    MOS_AttrEditor*          pAttributeEditor_;

    // Lima
    MOS_LimaEditor*          pLimaEditor_;
    MOS_LimaSelector*        pLimaSelector_;

    // Limit
    MOS_LimitEditor*         pLimitEditor_;
    MOS_LimitSelector*       pLimitSelector_;

    // Log bas de l'écran
    MOS_LogWindow*           pLogWindow_;

    // Historique des actions des agents
    MOS_History* pHistory_;


};

#include "MOS_MainWindow.inl"


#endif // __MOS_MainWindow_h_

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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MainWindow.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 18 $
// $Workfile: MOS_MainWindow.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::GetAttributeEditor
// Created: AGN 03-06-03
//-----------------------------------------------------------------------------
inline
MOS_AttrEditor& MOS_MainWindow::GetAttributeEditor() const
{
    assert( pAttributeEditor_ != 0 );
    return *pAttributeEditor_;
}


//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::GetMainMenu
// Created: NLD 2002-07-18
//-----------------------------------------------------------------------------
inline
QPopupMenu& MOS_MainWindow::GetMainMenu() const
{
    assert( pMainMenu_ );
    return *pMainMenu_;
}


//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::GetMenuBar
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
inline
QMenuBar& MOS_MainWindow::GetMenuBar() const
{
    return *menuBar();
}


//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::GetStatusBar
// Created: NLD 2002-09-13
//-----------------------------------------------------------------------------
inline
QStatusBar& MOS_MainWindow::GetStatusBar() const
{
    return *statusBar();
}


//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::GetGraphWindow
// Created: NLD 2002-10-14
//-----------------------------------------------------------------------------
//inline
//MOS_GraphWindow& MOS_MainWindow::GetGraphWindow() const
//{
//    assert( pGraphWindow_ );
//    return *pGraphWindow_;
//}


//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::GetWorldWidget
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
inline
MOS_WorldWidget& MOS_MainWindow::GetWorldWidget() const
{
    assert( pWorldWidget_ );
    return *pWorldWidget_;
}

//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::GetLimaEditor
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
inline
MOS_LimaListView& MOS_MainWindow::GetLimaEditor() const
{
    assert( pLimaEditor_ );
    return pLimaEditor_->GetLimaListView();
}

//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::GetLimaSelector
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
inline
MOS_LimaListView& MOS_MainWindow::GetLimaSelector() const
{
    assert( pLimaSelector_ );
    return pLimaSelector_->GetLimaListView();
}

//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::GetWidgetStack
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
inline
QWidgetStack* MOS_MainWindow::GetWidgetStackLeft() const
{
    return pToolsWidgetStackLeft_;
}

//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::GetLimitEditor
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
inline
MOS_LimitListView& MOS_MainWindow::GetLimitEditor() const
{
    assert( pLimitEditor_ );
    return pLimitEditor_->GetLimitListView();
}

//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::GetLimitSelector
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
inline
MOS_LimitListView& MOS_MainWindow::GetLimitSelector() const
{
    assert( pLimitSelector_ );
    return pLimitSelector_->GetLimitListView();
}


//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::GetWidgetStackRight
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
inline
QWidgetStack* MOS_MainWindow::GetWidgetStackRight() const
{
    return pToolsWidgetStackRight_;
}

//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::GetAgentMission
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
inline
MOS_AgentEditor& MOS_MainWindow::GetAgentEditor() const
{
    return *pAgentEditor_;
}


//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::GetAgentSelector
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
inline
MOS_AgentListView& MOS_MainWindow::GetAgentSelector() const
{
    assert( pAgentSelector_ );
    return pAgentSelector_->GetAgentListView();
}


//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::GetSimControl
// Created: FBD 03-01-22
//-----------------------------------------------------------------------------
inline
MOS_SIMControl& MOS_MainWindow::GetSimControl() const
{
    return *pSimControl_;
}


//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::GetHistory
// Created: FBD 03-08-25
//-----------------------------------------------------------------------------
inline
MOS_History& MOS_MainWindow::GetHistory() const
{
    return *pHistory_;
}



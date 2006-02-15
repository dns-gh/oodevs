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
// $Archive: /MVW_v10/Build/SDK/Light2/src/MainWindow.cpp $
// $Author: Age $
// $Modtime: 17/05/05 14:05 $
// $Revision: 20 $
// $Workfile: MainWindow.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "MainWindow.h"
#include "moc_MainWindow.cpp"
#include "Logger.h"
#include "Options.h"
#include "AgentListView.h"
#include "InfoPanel.h"

//#include "ObjectListView.h"
//#include "PopulationListView.h"

MainWindow* MainWindow::pInstance_ = 0;

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::MainWindow( Controller& controller )
    : QMainWindow  ( 0, 0, Qt::WDestructiveClose )
{
    assert( pInstance_ == 0 );
    pInstance_ = this;

    this->setIcon( MAKE_PIXMAP( mosicon ) );
    this->setCaption( APP_NAME );
    
    // Agent list panel
    QDockWindow* pListDockWnd_ = new QDockWindow( this );
    this->moveDockWindow( pListDockWnd_, Qt::DockLeft );
    QTabWidget* pListsTabWidget = new QTabWidget( pListDockWnd_ );
    AgentListView* pAgentList_ = new AgentListView( pListsTabWidget, controller );
    pListsTabWidget->addTab( pAgentList_, tr( "Agents" ) );
    pAgentList_->header()->hide();
//    ObjectListView* pObjectList_ = new ObjectListView( pListsTabWidget );
//    pListsTabWidget->addTab( pObjectList_, tr( "Objets" ) );
//    pObjectList_->header()->hide();
//    PopulationListView* pPopulationList_ = new PopulationListView( pListsTabWidget );
//	pListsTabWidget->addTab( pPopulationList_, tr( "Populations" ) );
//	pPopulationList_->header()->hide();
	pListDockWnd_->setWidget( pListsTabWidget );
    pListDockWnd_->setResizeEnabled( true );
    pListDockWnd_->setCloseMode( QDockWindow::Always );
    pListDockWnd_->setCaption( tr( "Unités" ) );
    this->setDockEnabled( pListDockWnd_, Qt::DockTop, false );

    // Info panel
//    QDockWindow* pInfoDockWnd_ = new QDockWindow( this );
//    this->moveDockWindow( pInfoDockWnd_, Qt::DockRight );
//    InfoPanel* pInfoPanel_ = new InfoPanel( pInfoDockWnd_ );
//    pInfoDockWnd_->setWidget( pInfoPanel_ );
//    pInfoDockWnd_->setResizeEnabled( true );
//    pInfoDockWnd_->setCloseMode( QDockWindow::Always );
//    pInfoDockWnd_->setCaption( tr( "Informations" ) );
//    this->setDockEnabled( pInfoDockWnd_, Qt::DockTop, false );

    // Logger
    QDockWindow*pLogDockWnd_ = new QDockWindow( this );
    this->moveDockWindow( pLogDockWnd_, Qt::DockBottom );
    Logger* pLogPanel_ = new Logger( pLogDockWnd_ );
    pLogDockWnd_->setWidget( pLogPanel_ );
    pLogDockWnd_->setResizeEnabled( true );
    pLogDockWnd_->setCloseMode( QDockWindow::Always );
    pLogDockWnd_->setCaption( tr( "Log" ) );
    this->setDockEnabled( pLogDockWnd_, Qt::DockTop, false );
}


// -----------------------------------------------------------------------------
// Name: MainWindow destructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    assert( pInstance_ != 0 );

    delete pOptions_;

    pInstance_ = 0;
}


// -----------------------------------------------------------------------------
// Name: MainWindow::Update
// Created: APE 2004-03-23
// -----------------------------------------------------------------------------
void MainWindow::Update()
{
//    pGLWidget_->update();
}



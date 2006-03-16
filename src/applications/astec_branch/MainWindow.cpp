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

#include "astec_pch.h"
#include "MainWindow.h"
#include "moc_MainWindow.cpp"
#include "Logger.h"
#include "Options.h"
#include "AgentListView.h"
#include "InfoPanels.h"
#include "ObjectListView.h"
#include "PopulationListView.h"
#include "ActionController.h"
#include "Settings.h"
#include "OptionsPanel.h"
#include "MissionPanel.h"
#include "GlWidget.h"

MainWindow* MainWindow::pInstance_ = 0;

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::MainWindow( Controller& controller, const CoordinateConverter& converter, const std::string& scipioXml )
    : QMainWindow      ( 0, 0, Qt::WDestructiveClose )
    , actionController_( * new ActionController() )
{
    assert( pInstance_ == 0 );
    pInstance_ = this;

    setIcon( MAKE_PIXMAP( astec ) );
    setCaption( APP_NAME );

    pOptions_ = new Options();

    GlWidget* pGlWiget = new GlWidget( this, scipioXml, controller, actionController_, converter );
    setCentralWidget( pGlWiget );
    pGlWiget->show();
    
    // Agent list panel
    QDockWindow* pListDockWnd_ = new QDockWindow( this );
    this->moveDockWindow( pListDockWnd_, Qt::DockLeft );
    QTabWidget* pListsTabWidget = new QTabWidget( pListDockWnd_ );
    AgentListView* pAgentList_ = new AgentListView( pListsTabWidget, controller, actionController_ );
    pListsTabWidget->addTab( pAgentList_, tr( "Agents" ) );
    pAgentList_->header()->hide();
    ObjectListView* pObjectList_ = new ObjectListView( pListsTabWidget, controller, actionController_ );
    pListsTabWidget->addTab( pObjectList_, tr( "Objets" ) );
    pObjectList_->header()->hide();
    PopulationListView* pPopulationList_ = new PopulationListView( pListsTabWidget, controller, actionController_ );
	pListsTabWidget->addTab( pPopulationList_, tr( "Populations" ) );
	pPopulationList_->header()->hide();
	pListDockWnd_->setWidget( pListsTabWidget );
    pListDockWnd_->setResizeEnabled( true );
    pListDockWnd_->setCloseMode( QDockWindow::Always );
    pListDockWnd_->setCaption( tr( "Unités" ) );
    this->setDockEnabled( pListDockWnd_, Qt::DockTop, false );

    // Info panel
    QDockWindow* pInfoDockWnd_ = new QDockWindow( this );
    this->moveDockWindow( pInfoDockWnd_, Qt::DockRight );
    InfoPanels* pInfoPanel_ = new InfoPanels( pInfoDockWnd_, controller, actionController_ );
    pInfoDockWnd_->setWidget( pInfoPanel_ );
    pInfoDockWnd_->setResizeEnabled( true );
    pInfoDockWnd_->setCloseMode( QDockWindow::Always );
    pInfoDockWnd_->setCaption( tr( "Informations" ) );
    this->setDockEnabled( pInfoDockWnd_, Qt::DockTop, false );

     // Mission panel
    MissionPanel* pMissionPanel_ = new MissionPanel( this, actionController_ );
    this->moveDockWindow( pMissionPanel_, Qt::DockLeft );
    pMissionPanel_->hide();
    this->setDockEnabled( pMissionPanel_, Qt::DockTop, false );
    this->setAppropriate( pMissionPanel_, false );

    // Logger
    QDockWindow*pLogDockWnd_ = new QDockWindow( this );
    this->moveDockWindow( pLogDockWnd_, Qt::DockBottom );
    Logger* pLogPanel_ = new Logger( pLogDockWnd_ );
    pLogDockWnd_->setWidget( pLogPanel_ );
    pLogDockWnd_->setResizeEnabled( true );
    pLogDockWnd_->setCloseMode( QDockWindow::Always );
    pLogDockWnd_->setCaption( tr( "Log" ) );
    this->setDockEnabled( pLogDockWnd_, Qt::DockTop, false );

    // Options window
    QDockWindow* pOptionsDockWnd_ = new QDockWindow( this );
    this->moveDockWindow( pOptionsDockWnd_, Qt::DockRight );
    pOptionsDockWnd_->hide();
    pOptionsPanel_ = new OptionsPanel( pOptionsDockWnd_, *pOptions_ );
    pOptionsDockWnd_->setWidget( pOptionsPanel_ );
    pOptionsDockWnd_->setResizeEnabled( true );
    pOptionsDockWnd_->setCloseMode( QDockWindow::Always );
    pOptionsDockWnd_->setCaption( tr( "Options" ) );
    this->setDockEnabled( pOptionsDockWnd_, Qt::DockTop, false );

    ReadSettings();
    ReadOptions();
}


// -----------------------------------------------------------------------------
// Name: MainWindow destructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    assert( pInstance_ != 0 );

//    delete pOptions_;

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

// -----------------------------------------------------------------------------
// Name: MainWindow::closeEvent
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::closeEvent( QCloseEvent* pEvent )
{
    WriteSettings();
    WriteOptions();
    QMainWindow::closeEvent( pEvent );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::WriteSettings
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::WriteSettings()
{
    Settings settings;
    settings.setPath( "MASA", "Astec" );

    // Pannel configuration
    QString strDockConfig;
    QTextStream strDockConfigStream( &strDockConfig, IO_WriteOnly );
    strDockConfigStream << *this;
    settings.writeEntry( "/Panels", strDockConfig );
    settings.WriteEntry( "/MainWindow", *this );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::ReadSettings
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::ReadSettings()
{
    Settings settings;
    settings.setPath( "MASA", "Astec" );

    // Pannel configuration
    QString strDockConfig;
    strDockConfig = settings.readEntry( "/Panels" );
    QTextStream strDockConfigStream( &strDockConfig, IO_ReadOnly );
    strDockConfigStream >> *this;

    // Main window configuration
    settings.ReadEntry( "/MainWindow", *this, 800, 600, 100, 100, false );

    // Always keep the mission panel hidden hidden at launch.
//    pMissionPanel_->hide();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::WriteOptions
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::WriteOptions()
{
    Settings settings;
    settings.setPath( "MASA", "Astec" );
    pOptionsPanel_->WriteOptions( settings );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::ReadOptions
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::ReadOptions()
{
    Settings settings;
    settings.setPath( "MASA", "Astec" );
    pOptionsPanel_->ReadOptions( settings );
}


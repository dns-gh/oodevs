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
#include "Gl3dWidget.h"
#include "Controllers.h"
#include "SIMControlToolbar.h"
#include "MapToolbar.h"
#include "ControllerToolbar.h"
#include "OptionVariant.h"
#include "GlLayers.h"
#include "UnitToolbar.h"
#include "LogisticToolbar.h"
#include "RecorderToolbar.h"
#include "MissionLayer.h"

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::MainWindow( Controllers& controllers, Model& model, const std::string& scipioXml )
    : QMainWindow( 0, 0, Qt::WDestructiveClose )
    , controllers_( controllers )
    , scipioXml_  ( scipioXml )
    , layers_     ( 0 )
    , widget2d_   ( 0 )
    , widget3d_   ( 0 )
    , b3d_        ( false )
{
    setIcon( MAKE_PIXMAP( astec ) );
    setCaption( APP_NAME );

    widget2d_ = new GlWidget( this, controllers, scipioXml );
    setCentralWidget( widget2d_ );
    layers_ = new GlLayers( *widget2d_, scipioXml, controllers, model );

    // Agent list panel
    QDockWindow* pListDockWnd_ = new QDockWindow( this );
    this->moveDockWindow( pListDockWnd_, Qt::DockLeft );
    QTabWidget* pListsTabWidget = new QTabWidget( pListDockWnd_ );
    AgentListView* pAgentList_ = new AgentListView( pListsTabWidget, controllers );
    pListsTabWidget->addTab( pAgentList_, tr( "Agents" ) );
    pAgentList_->header()->hide();
    ObjectListView* pObjectList_ = new ObjectListView( pListsTabWidget, controllers );
    pListsTabWidget->addTab( pObjectList_, tr( "Objets" ) );
    pObjectList_->header()->hide();
    PopulationListView* pPopulationList_ = new PopulationListView( pListsTabWidget, controllers );
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
    InfoPanels* pInfoPanel_ = new InfoPanels( pInfoDockWnd_, controllers );
    pInfoDockWnd_->setWidget( pInfoPanel_ );
    pInfoDockWnd_->setResizeEnabled( true );
    pInfoDockWnd_->setCloseMode( QDockWindow::Always );
    pInfoDockWnd_->setCaption( tr( "Informations" ) );
    this->setDockEnabled( pInfoDockWnd_, Qt::DockTop, false );

     // Mission panel
    MissionPanel* pMissionPanel_ = new MissionPanel( this, controllers, model, layers_->GetParametersLayer(), *layers_ );
    this->moveDockWindow( pMissionPanel_, Qt::DockLeft );
    pMissionPanel_->hide();
    this->setDockEnabled( pMissionPanel_, Qt::DockTop, false );
    this->setAppropriate( pMissionPanel_, false );
    layers_->Register( *new MissionLayer( *pMissionPanel_ ) ); // $$$$ AGE 2006-03-31: 

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
    pOptionsPanel_ = new OptionsPanel( pOptionsDockWnd_, controllers );
    pOptionsDockWnd_->setWidget( pOptionsPanel_ );
    pOptionsDockWnd_->setResizeEnabled( true );
    pOptionsDockWnd_->setCloseMode( QDockWindow::Always );
    pOptionsDockWnd_->setCaption( tr( "Options" ) );
    this->setDockEnabled( pOptionsDockWnd_, Qt::DockTop, false );

    new SIMControlToolbar( this, controllers );
    new MapToolbar( this, controllers );
    new ControllerToolbar( this, controllers );
    new UnitToolbar( this, controllers );
    new LogisticToolbar( this, controllers );
    new RecorderToolbar( this );

    controllers_.Register( *this );

    layers_->ChangeTo( widget2d_ );
    layers_->RegisterTo( widget2d_ );

    // This one refreshes the map display, and is called only a few time per second.
    displayTimer_ = new QTimer( this );
    connect( displayTimer_, SIGNAL( timeout()), centralWidget(), SLOT( updateGL() ) );
    displayTimer_->start( 50 );

    ReadSettings();
    ReadOptions();
}


// -----------------------------------------------------------------------------
// Name: MainWindow destructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    controllers_.Remove( *this );
//    delete pOptions_;
    delete layers_;
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

// -----------------------------------------------------------------------------
// Name: MainWindow::OptionChanged
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void MainWindow::OptionChanged( const std::string& name, const OptionVariant& value )
{
    if( name == "3D" )
    {
        bool new3d = value.To< bool >();
        if( new3d != b3d_ )
        {
            centralWidget()->hide();
            disconnect( displayTimer_, SIGNAL( timeout()), centralWidget(), SLOT( updateGL() ) );
            if( new3d )
            {
                if( ! widget3d_ )
                {
                    widget3d_ = new Gl3dWidget( this, controllers_, scipioXml_, layers_->GetElevationMap() );
                    layers_->RegisterTo( widget3d_ );
                }
                layers_->ChangeTo( widget3d_ );
                setCentralWidget( widget3d_ );
            }
            else
            {
                layers_->ChangeTo( widget2d_ );
                setCentralWidget( widget2d_ );
            }
            centralWidget()->show();
            connect( displayTimer_, SIGNAL( timeout()), centralWidget(), SLOT( updateGL() ) );
            b3d_ = new3d;
        }
    }
}


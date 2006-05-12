// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
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
#include "GraphicsPanel.h"
#include "ObjectCreationPanel.h"
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
#include "DetectionMap.h"
#include "Model.h"
#include "ObjectTypes.h"
#include "App.h"
#include "Network.h"
#include "AgentServerMsgMgr.h"
#include "GraphicPreferences.h"
#include "StatusBar.h"
#include "Dialogs.h"
#include "Simulation.h"
#include "MagicOrdersInterface.h"
#include "PreferencesDialog.h"

#include "Agent.h"
#include "Object.h"
#include "Population.h"
#include "EntitySearchBox.h"
#include "Menu.h"
#include "ParametersLayer.h"
#include "GlPlaceHolder.h"
#include "RichItemFactory.h"
#include "LinkInterpreter.h"
#include "Tiredness.h"
#include "Experience.h"

#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )
namespace bfs = boost::filesystem;

#include "xeumeuleu/xml.h"
using namespace xml;

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::MainWindow( Controllers& controllers, Model& model, MsgRecorder& recorder )
    : QMainWindow( 0, 0, Qt::WDestructiveClose )
    , controllers_( controllers )
    , model_      ( model )
    , layers_     ( 0 )
    , widget2d_   ( 0 )
    , widget3d_   ( 0 )
    , b3d_        ( false )
{
    setIcon( MAKE_PIXMAP( astec ) );
    setCaption( APP_NAME );

    PreferencesDialog* prefDialog = new PreferencesDialog( this, controllers );

    layers_ = new GlLayers( controllers, model, prefDialog->GetPreferences() );

    RichItemFactory* factory = new RichItemFactory( this ); // $$$$ AGE 2006-05-11: aggregate somewhere
    LinkInterpreter* interpreter = new LinkInterpreter( this, controllers, model );
    connect( factory, SIGNAL( LinkClicked( const QString& ) ), interpreter, SLOT( Interprete( const QString& ) ) );

    // Agent list panel
    QDockWindow* pListDockWnd_ = new QDockWindow( this );
    moveDockWindow( pListDockWnd_, Qt::DockLeft );
    QTabWidget* pListsTabWidget = new QTabWidget( pListDockWnd_ );

    QVBox* agentPanel = new QVBox();
    new EntitySearchBox< Agent >( agentPanel, controllers );
    AgentListView* pAgentList_ = new AgentListView( agentPanel, controllers, *factory );
    pListsTabWidget->addTab( agentPanel, tr( "Agents" ) );
    pAgentList_->header()->hide();

    QVBox* objectPanel = new QVBox();
    new EntitySearchBox< Object >( objectPanel, controllers );
    ObjectListView* pObjectList_ = new ObjectListView( objectPanel, controllers, *factory );
    pListsTabWidget->addTab( objectPanel, tr( "Objets" ) );
    pObjectList_->header()->hide();

    QVBox* populationPanel = new QVBox();
    new EntitySearchBox< Population >( populationPanel, controllers );
    PopulationListView* pPopulationList_ = new PopulationListView( populationPanel, controllers, *factory );
	pListsTabWidget->addTab( populationPanel, tr( "Populations" ) );
	pPopulationList_->header()->hide();

	pListDockWnd_->setWidget( pListsTabWidget );
    pListDockWnd_->setResizeEnabled( true );
    pListDockWnd_->setCloseMode( QDockWindow::Always );
    pListDockWnd_->setCaption( tr( "Unités" ) );
    this->setDockEnabled( pListDockWnd_, Qt::DockTop, false );

    // Info panel
    QDockWindow* pInfoDockWnd_ = new QDockWindow( this );
    this->moveDockWindow( pInfoDockWnd_, Qt::DockRight );
    InfoPanels* pInfoPanel_ = new InfoPanels( pInfoDockWnd_, controllers, *factory );
    pInfoDockWnd_->setWidget( pInfoPanel_ );
    pInfoDockWnd_->setResizeEnabled( true );
    pInfoDockWnd_->setCloseMode( QDockWindow::Always );
    pInfoDockWnd_->setCaption( tr( "Informations" ) );
    this->setDockEnabled( pInfoDockWnd_, Qt::DockTop, false );

     // Mission panel
    MissionPanel* pMissionPanel_ = new MissionPanel( this, controllers, model, layers_->GetParametersLayer(), *layers_ );
    moveDockWindow( pMissionPanel_, Qt::DockLeft );
    pMissionPanel_->hide();
    setDockEnabled( pMissionPanel_, Qt::DockTop, false );
    setAppropriate( pMissionPanel_, false );
    layers_->Register( *new MiscLayer< MissionPanel >( *pMissionPanel_ ) ); // $$$$ AGE 2006-03-31: 

    // Logger
    QDockWindow* pLogDockWnd_ = new QDockWindow( this );
    moveDockWindow( pLogDockWnd_, Qt::DockBottom );
    Logger* pLogPanel_ = new Logger( pLogDockWnd_, *factory );
    pLogDockWnd_->setWidget( pLogPanel_ );
    pLogDockWnd_->setResizeEnabled( true );
    pLogDockWnd_->setCloseMode( QDockWindow::Always );
    pLogDockWnd_->setCaption( tr( "Log" ) );
    setDockEnabled( pLogDockWnd_, Qt::DockTop, false );

    // object creation window
    QDockWindow* pObjectCreationWnd = new QDockWindow( this );
    moveDockWindow( pObjectCreationWnd, Qt::DockRight );
    pObjectCreationWnd->hide();
    ObjectCreationPanel* objectCreationPanel = new ObjectCreationPanel( pObjectCreationWnd, controllers, model_, layers_->GetParametersLayer(), *layers_ );
    pObjectCreationWnd->setWidget( objectCreationPanel );
    pObjectCreationWnd->setResizeEnabled( true );
    pObjectCreationWnd->setCloseMode( QDockWindow::Always );
    pObjectCreationWnd->setCaption( tr( "Création d'objet" ) );
    setDockEnabled( pObjectCreationWnd, Qt::DockTop, false );
    layers_->Register( *new MiscLayer< ObjectCreationPanel >( *objectCreationPanel ) );

    new MagicOrdersInterface( this, controllers_ );

    new SIMControlToolbar( this, controllers );
    new MapToolbar( this, controllers );
    new ControllerToolbar( this, controllers );
    new UnitToolbar( this, controllers );
    new LogisticToolbar( this, controllers, layers_->GetAgentLayer() ); // $$$$ AGE 2006-05-02: 
    RecorderToolbar* recorderToolbar = new RecorderToolbar( this, recorder );

    new Menu( this, controllers, *prefDialog, *recorderToolbar, *factory );

    glPlaceHolder_ = new GlPlaceHolder( this );
    setCentralWidget( glPlaceHolder_ );
    
    pStatus_ = new StatusBar( statusBar(), model_.detection_, model_.coordinateConverter_, controllers_ );
    controllers_.Register( *this );

    displayTimer_ = new QTimer( this );

    ReadSettings();
    ReadOptions();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Open
// Created: AGE 2006-05-03
// -----------------------------------------------------------------------------
void MainWindow::Open()
{
    std::string current;
    while( ! bfs::exists( bfs::path( current, bfs::native ) ) )
    {
        const QString filename = QFileDialog::getOpenFileName( "../data/", "Scipio (*.xml)", 0, 0, "Open scipio.xml" );
        if( filename.isEmpty() )
            return;
        current = filename;
        if( current.substr( 0, 2 ) == "//" )
            std::replace( current.begin(), current.end(), '/', '\\' );
    }
    Load( current );
}


// -----------------------------------------------------------------------------
// Name: MainWindow::Load
// Created: AGE 2006-05-03
// -----------------------------------------------------------------------------
void MainWindow::Load( const std::string& scipioXml )
{
    InitializeHumanFactors( scipioXml );
    model_.Load( scipioXml );
    scipioXml_ = scipioXml;
    delete widget2d_; widget2d_ = 0;
    delete widget3d_; widget3d_ = 0;
    widget2d_ = new GlWidget( this, controllers_, scipioXml );
    delete glPlaceHolder_; glPlaceHolder_ = 0;
    setCentralWidget( widget2d_ );
    layers_->Load( scipioXml_ );
    layers_->ChangeTo( widget2d_ );
    layers_->RegisterTo( widget2d_ );
    
    connect( widget2d_, SIGNAL( MouseMove( const geometry::Point2f& ) ), pStatus_, SLOT( OnMouseMove( const geometry::Point2f& ) ) );
    connect( displayTimer_, SIGNAL( timeout()), centralWidget(), SLOT( updateGL() ) );
    displayTimer_->start( 50 );
    widget2d_->show();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::InitializeHumanFactors
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
void MainWindow::InitializeHumanFactors( const std::string& conffile )
{
    xifstream xis( conffile );
    xis >> start( "Scipio" )
            >> start( "Donnees" );
    std::string strHumanFactorsFile;
    xis >> content( "FacteursHumains", strHumanFactorsFile );
    const std::string factorsName = App::BuildChildPath( conffile, strHumanFactorsFile );
    xifstream factors( factorsName );

    Tiredness ::Initialize( factors );
    Experience::Initialize( factors );
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
//    pMissionPanel_->hide(); // $$$$ AGE 2006-04-19: 
}

// -----------------------------------------------------------------------------
// Name: MainWindow::WriteOptions
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::WriteOptions()
{
    Settings settings;
    settings.setPath( "MASA", "Astec" );
    settings.beginGroup( "/Options" );
    controllers_.options_.Save( settings );
    settings.endGroup();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::ReadOptions
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::ReadOptions()
{
    Settings settings;
    settings.setPath( "MASA", "Astec" );
    settings.beginGroup( "/Options" );
    controllers_.options_.Load( settings );
    settings.endGroup();
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
                    widget3d_ = new Gl3dWidget( this, controllers_, scipioXml_, model_.detection_ );
                    connect( widget3d_, SIGNAL( MouseMove( const geometry::Point3f& ) ), pStatus_, SLOT( OnMouseMove( const geometry::Point3f& ) ) );
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

namespace
{
    struct Nothing {};
}

// -----------------------------------------------------------------------------
// Name: MainWindow::NotifyUpdated
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void MainWindow::NotifyUpdated( const Simulation& simulation )
{
    if( ! simulation.IsConnected() )
    {
        controllers_.actions_.Select( Nothing() );
        model_.Purge();
    }
}


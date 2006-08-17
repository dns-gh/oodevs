// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_prepa_app_pch.h"
#include "MainWindow.h"
#include "moc_MainWindow.cpp"

//#include "ControllerToolbar.h"
//#include "EventToolbar.h"
#include "GlLayers.h"
//#include "InfoPanels.h"
//#include "LogisticToolbar.h"
//#include "MapToolbar.h"
//#include "Menu.h"
//#include "ObjectCreationPanel.h"
//#include "UnitToolbar.h"
//#include "LinkInterpreter.h"

#include "astec_kernel/ActionController.h"
#include "astec_kernel/Controllers.h"
#include "astec_kernel/DataDictionary.h"
#include "astec_kernel/DetectionMap.h"
#include "astec_kernel/ObjectTypes.h"
#include "astec_kernel/Options.h"
#include "astec_kernel/OptionVariant.h"
#include "astec_kernel/PathTools.h"

//#include "astec_gaming/CoordinateConverter.h"
//#include "astec_gaming/Experience.h"
#include "astec_preparation/Model.h"
//#include "astec_gaming/Population.h"
#include "astec_preparation/StaticModel.h"
//#include "astec_gaming/Tiredness.h"

#include "astec_gui/AgentList.h"
//#include "astec_gui/GlWidget.h"
#include "astec_gui/GraphicPreferences.h"
#include "astec_gui/Logger.h"
#include "astec_gui/ObjectList.h"
#include "astec_gui/OptionsPanel.h"
#include "astec_gui/ParametersLayer.h"
#include "astec_gui/Settings.h"
#include "astec_gui/StatusBar.h"
#include "astec_gui/PopulationList.h"
#include "astec_gui/PreferencesDialog.h"
#include "astec_gui/GlPlaceHolder.h"
#include "astec_gui/RichItemFactory.h"
#include "astec_gui/resources.h"

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
MainWindow::MainWindow( Controllers& controllers, StaticModel& staticModel, Model& model )
    : QMainWindow( 0, 0, Qt::WDestructiveClose )
    , controllers_( controllers )
    , staticModel_( staticModel )
    , model_      ( model )
    , layers_     ( 0 )
//    , widget2d_   ( 0 )
{
//    setIcon( MAKE_PIXMAP( astec ) );
    setCaption( APP_NAME );

//    PreferencesDialog* prefDialog = new PreferencesDialog( this, controllers );
//
//    layers_ = new GlLayers( controllers, staticModel_, model_, prefDialog->GetPreferences() );
//
//    RichItemFactory* factory = new RichItemFactory( this ); // $$$$ AGE 2006-05-11: aggregate somewhere
//    LinkInterpreter* interpreter = new LinkInterpreter( this, controllers );
//    connect( factory, SIGNAL( LinkClicked( const QString& ) ), interpreter, SLOT( Interprete( const QString& ) ) );
//
//    // Agent list panel
//    QDockWindow* pListDockWnd_ = new QDockWindow( this );
//    moveDockWindow( pListDockWnd_, Qt::DockLeft );
//    QTabWidget* pListsTabWidget = new QTabWidget( pListDockWnd_ );
//
//    pListsTabWidget->addTab( new AgentList     ( controllers, publisher, *factory ), tr( "Agents" ) );
//    pListsTabWidget->addTab( new ObjectList    ( controllers, *factory ),            tr( "Objets" ) );
//    pListsTabWidget->addTab( new PopulationList( controllers, *factory ),            tr( "Populations" ) );
//	pListDockWnd_->setWidget( pListsTabWidget );
//    pListDockWnd_->setResizeEnabled( true );
//    pListDockWnd_->setCloseMode( QDockWindow::Always );
//    pListDockWnd_->setCaption( tr( "Unités" ) );
//    setDockEnabled( pListDockWnd_, Qt::DockTop, false );

//    // Info panel
//    QDockWindow* pInfoDockWnd_ = new QDockWindow( this );
//    moveDockWindow( pInfoDockWnd_, Qt::DockRight );
//    InfoPanels* pInfoPanel_ = new InfoPanels( pInfoDockWnd_, controllers, *factory );
//    pInfoDockWnd_->setWidget( pInfoPanel_ );
//    pInfoDockWnd_->setResizeEnabled( true );
//    pInfoDockWnd_->setCloseMode( QDockWindow::Always );
//    pInfoDockWnd_->setCaption( tr( "Informations" ) );
//    setDockEnabled( pInfoDockWnd_, Qt::DockTop, false );
//
//    // Logger
//    QDockWindow* pLogDockWnd_ = new QDockWindow( this );
//    moveDockWindow( pLogDockWnd_, Qt::DockBottom );
//    Logger* pLogPanel_ = new Logger( pLogDockWnd_, *factory );
//    pLogDockWnd_->setWidget( pLogPanel_ );
//    pLogDockWnd_->setResizeEnabled( true );
//    pLogDockWnd_->setCloseMode( QDockWindow::Always );
//    pLogDockWnd_->setCaption( tr( "Log" ) );
//    setDockEnabled( pLogDockWnd_, Qt::DockTop, false );
//    connect( pLogPanel_, SIGNAL( Error() ), pLogDockWnd_, SLOT( show() ) );

//    // object creation window
//    QDockWindow* pObjectCreationWnd = new QDockWindow( this );
//    moveDockWindow( pObjectCreationWnd, Qt::DockRight );
//    pObjectCreationWnd->hide();
//    ObjectCreationPanel* objectCreationPanel = new ObjectCreationPanel( pObjectCreationWnd, controllers, publisher, staticModel_, layers_->GetParametersLayer(), *layers_ );
//    pObjectCreationWnd->setWidget( objectCreationPanel );
//    pObjectCreationWnd->setResizeEnabled( true );
//    pObjectCreationWnd->setCloseMode( QDockWindow::Always );
//    pObjectCreationWnd->setCaption( tr( "Création d'objet" ) );
//    setDockEnabled( pObjectCreationWnd, Qt::DockTop, false );
//    layers_->Register( *new MiscLayer< ObjectCreationPanel >( *objectCreationPanel ) ); // $$$$ AGE 2006-06-30: 

//    new MapToolbar( this, controllers );
//    new UnitToolbar( this, controllers );
//    new LogisticToolbar( this, controllers, layers_->GetAgentLayer() ); // $$$$ AGE 2006-05-02: 

//    new Menu( this, controllers, *prefDialog, *recorderToolbar, *factory );

    glPlaceHolder_ = new GlPlaceHolder( this );
    setCentralWidget( glPlaceHolder_ );

//    layers_->RegisterBaseLayers();

//    pStatus_ = new StatusBar( statusBar(), staticModel_.detection_, staticModel_.coordinateConverter_, controllers_ );
//    controllers_.Register( *this );

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
    scipioXml_ = scipioXml;
//    delete widget2d_; widget2d_ = 0;
//    widget2d_ = new GlWidget( this, controllers_, scipioXml );
//    delete glPlaceHolder_; glPlaceHolder_ = 0;
//    setCentralWidget( widget2d_ );
    model_.Purge();
    staticModel_.Load( scipioXml );

//    layers_->ChangeTo( widget2d_ );
//    layers_->RegisterTo( widget2d_ );
//    
//    connect( widget2d_, SIGNAL( MouseMove( const geometry::Point2f& ) ), pStatus_, SLOT( OnMouseMove( const geometry::Point2f& ) ) );
    connect( displayTimer_, SIGNAL( timeout()), centralWidget(), SLOT( updateGL() ) );
    displayTimer_->start( 50 );
//    widget2d_->show();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::InitializeHumanFactors
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
void MainWindow::InitializeHumanFactors( const std::string& conffile )
{
//    xifstream xis( conffile );
//    xis >> start( "Scipio" )
//            >> start( "Donnees" );
//    std::string strHumanFactorsFile;
//    xis >> content( "FacteursHumains", strHumanFactorsFile );
//    const std::string factorsName = path_tools::BuildChildPath( conffile, strHumanFactorsFile );
//    xifstream factors( factorsName );
//
//    Tiredness ::Initialize( factors );
//    Experience::Initialize( factors );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Close
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void MainWindow::Close()
{
    model_.Purge();
    glPlaceHolder_ = new GlPlaceHolder( this );
    setCentralWidget( glPlaceHolder_ );
    glPlaceHolder_->show();

//    delete widget2d_; widget2d_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MainWindow destructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
//    controllers_.Remove( *this );
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

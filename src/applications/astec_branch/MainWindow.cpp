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

#include "ControllerToolbar.h"
#include "EventToolbar.h"
#include "GlLayers.h"
#include "InfoPanels.h"
#include "LogisticToolbar.h"
#include "MapToolbar.h"
#include "Menu.h"
#include "RecorderToolbar.h"
#include "SIMControlToolbar.h"
#include "UnitToolbar.h"

#include "astec_kernel/ActionController.h"
#include "astec_kernel/Controllers.h"
#include "astec_kernel/DataDictionary.h"
#include "astec_kernel/DetectionMap.h"
#include "astec_kernel/ObjectTypes.h"
#include "astec_kernel/Options.h"
#include "astec_kernel/OptionVariant.h"
#include "astec_kernel/PathTools.h"

#include "astec_gaming/AgentServerMsgMgr.h"
#include "astec_gaming/CoordinateConverter.h"
#include "astec_gaming/Experience.h"
#include "astec_gaming/Model.h"
#include "astec_gaming/Network.h"
#include "astec_gaming/Population.h"
#include "astec_gaming/Simulation.h"
#include "astec_gaming/StaticModel.h"
#include "astec_gaming/Tiredness.h"

#include "astec_gui/Logger.h"
#include "astec_gui/AgentList.h"
#include "astec_gui/ObjectList.h"
#include "astec_gui/PopulationList.h"
#include "astec_gui/Settings.h"
#include "astec_gui/OptionsPanel.h"
#include "astec_gui/MissionPanel.h"
#include "astec_gui/ObjectCreationPanel.h"
#include "astec_gui/GlWidget.h"
#include "astec_gui/Gl3dWidget.h"
#include "astec_gui/MissionLayer.h"
#include "astec_gui/GraphicPreferences.h"
#include "astec_gui/StatusBar.h"
#include "astec_gui/Dialogs.h"
#include "astec_gui/MagicOrdersInterface.h"
#include "astec_gui/PreferencesDialog.h"
#include "astec_gui/ParametersLayer.h"
#include "astec_gui/GlPlaceHolder.h"
#include "astec_gui/RichItemFactory.h"
#include "astec_gui/LinkInterpreter.h"
#include "astec_gui/BigBrother.h"
#include "astec_gui/MiniViews.h"
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
MainWindow::MainWindow( Controllers& controllers, StaticModel& staticModel, Model& model, Network& network )
    : QMainWindow( 0, 0, Qt::WDestructiveClose )
    , controllers_( controllers )
    , staticModel_( staticModel )
    , model_      ( model )
    , network_    ( network )
    , layers_     ( 0 )
    , widget2d_   ( 0 )
    , widget3d_   ( 0 )
    , b3d_        ( false )
{
    setIcon( MAKE_PIXMAP( astec ) );
    setCaption( APP_NAME );

    Publisher_ABC& publisher = network_.GetMessageMgr();

    PreferencesDialog* prefDialog = new PreferencesDialog( this, controllers );
    new Dialogs( this, controllers, model_, publisher ); // $$$$ SBO 2006-06-30: leak

    layers_ = new GlLayers( controllers, staticModel_, model_, prefDialog->GetPreferences() );

    RichItemFactory* factory = new RichItemFactory( this ); // $$$$ AGE 2006-05-11: aggregate somewhere
    LinkInterpreter* interpreter = new LinkInterpreter( this, controllers, model );
    connect( factory, SIGNAL( LinkClicked( const QString& ) ), interpreter, SLOT( Interprete( const QString& ) ) );

    // Agent list panel
    QDockWindow* pListDockWnd_ = new QDockWindow( this );
    moveDockWindow( pListDockWnd_, Qt::DockLeft );
    QTabWidget* pListsTabWidget = new QTabWidget( pListDockWnd_ );

    pListsTabWidget->addTab( new AgentList     ( controllers, publisher, *factory ), tr( "Agents" ) );
    pListsTabWidget->addTab( new ObjectList    ( controllers, *factory ),            tr( "Objets" ) );
    pListsTabWidget->addTab( new PopulationList( controllers, *factory ),            tr( "Populations" ) );
	pListDockWnd_->setWidget( pListsTabWidget );
    pListDockWnd_->setResizeEnabled( true );
    pListDockWnd_->setCloseMode( QDockWindow::Always );
    pListDockWnd_->setCaption( tr( "Unités" ) );
    setDockEnabled( pListDockWnd_, Qt::DockTop, false );

    // BigBrother
    QDockWindow* pBigBrotherWnd = new QDockWindow( this );
    moveDockWindow( pBigBrotherWnd, Qt::DockLeft );
    spy_ = new BigBrother( pBigBrotherWnd, controllers );
    pBigBrotherWnd->setWidget( spy_ );
    pBigBrotherWnd->setResizeEnabled( true );
    pBigBrotherWnd->setCloseMode( QDockWindow::Always );
    pBigBrotherWnd->setCaption( tr( "Espion" ) );
    setDockEnabled( pBigBrotherWnd, Qt::DockTop, false );

    MiniViews* miniviews = new MiniViews( this, controllers_, widget2d_ );
    miniviews->hide();

    // Info panel
    QDockWindow* pInfoDockWnd_ = new QDockWindow( this );
    moveDockWindow( pInfoDockWnd_, Qt::DockRight );
    InfoPanels* pInfoPanel_ = new InfoPanels( pInfoDockWnd_, controllers, *factory );
    pInfoDockWnd_->setWidget( pInfoPanel_ );
    pInfoDockWnd_->setResizeEnabled( true );
    pInfoDockWnd_->setCloseMode( QDockWindow::Always );
    pInfoDockWnd_->setCaption( tr( "Informations" ) );
    setDockEnabled( pInfoDockWnd_, Qt::DockTop, false );

     // Mission panel
    MissionPanel* pMissionPanel_ = new MissionPanel( this, controllers_, staticModel_, publisher, layers_->GetParametersLayer(), *layers_ );
    moveDockWindow( pMissionPanel_, Qt::DockLeft );
    setDockEnabled( pMissionPanel_, Qt::DockTop, false );
    setAppropriate( pMissionPanel_, false );
    layers_->Register( *new MiscLayer< MissionPanel >( *pMissionPanel_ ) ); // $$$$ AGE 2006-03-31:  // $$$$ AGE 2006-06-30: 
    pMissionPanel_->hide();

    // Logger
    QDockWindow* pLogDockWnd_ = new QDockWindow( this );
    moveDockWindow( pLogDockWnd_, Qt::DockBottom );
    Logger* pLogPanel_ = new Logger( pLogDockWnd_, *factory );
    pLogDockWnd_->setWidget( pLogPanel_ );
    pLogDockWnd_->setResizeEnabled( true );
    pLogDockWnd_->setCloseMode( QDockWindow::Always );
    pLogDockWnd_->setCaption( tr( "Log" ) );
    setDockEnabled( pLogDockWnd_, Qt::DockTop, false );
    connect( pLogPanel_, SIGNAL( Error() ), pLogDockWnd_, SLOT( show() ) );

    // object creation window
    QDockWindow* pObjectCreationWnd = new QDockWindow( this );
    moveDockWindow( pObjectCreationWnd, Qt::DockRight );
    pObjectCreationWnd->hide();
    ObjectCreationPanel* objectCreationPanel = new ObjectCreationPanel( pObjectCreationWnd, controllers, publisher, staticModel_, layers_->GetParametersLayer(), *layers_ );
    pObjectCreationWnd->setWidget( objectCreationPanel );
    pObjectCreationWnd->setResizeEnabled( true );
    pObjectCreationWnd->setCloseMode( QDockWindow::Always );
    pObjectCreationWnd->setCaption( tr( "Création d'objet" ) );
    setDockEnabled( pObjectCreationWnd, Qt::DockTop, false );
    layers_->Register( *new MiscLayer< ObjectCreationPanel >( *objectCreationPanel ) ); // $$$$ AGE 2006-06-30: 

    new MagicOrdersInterface( this, controllers_, publisher, staticModel_, layers_->GetParametersLayer() );

    new SIMControlToolbar( this, controllers, network, publisher );
    new MapToolbar( this, controllers );
    new ControllerToolbar( this, controllers );
    new UnitToolbar( this, controllers );
    new LogisticToolbar( this, controllers, layers_->GetAgentLayer() ); // $$$$ AGE 2006-05-02: 
    new EventToolbar( this, controllers );
    RecorderToolbar* recorderToolbar = new RecorderToolbar( this, network );

    new Menu( this, controllers, *prefDialog, *recorderToolbar, *factory );

    glPlaceHolder_ = new GlPlaceHolder( this );
    setCentralWidget( glPlaceHolder_ );

    layers_->RegisterBaseLayers();

    pStatus_ = new StatusBar( statusBar(), staticModel_.detection_, staticModel_.coordinateConverter_, controllers_ );
    controllers_.Register( *this );

    displayTimer_ = new QTimer( this );

    ReadSettings();
    ReadOptions();
    pMissionPanel_->hide();
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
    delete widget2d_; widget2d_ = 0;
    delete widget3d_; widget3d_ = 0;
    widget2d_ = new GlWidget( this, controllers_, scipioXml );
    delete glPlaceHolder_; glPlaceHolder_ = 0;
    setCentralWidget( widget2d_ );
    model_.Purge();
    staticModel_.Load( scipioXml );

    layers_->ChangeTo( widget2d_ );
    layers_->RegisterTo( widget2d_ );
    b3d_ = false;
    controllers_.options_.Change( "3D", b3d_ );
    
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
    const std::string factorsName = path_tools::BuildChildPath( conffile, strHumanFactorsFile );
    xifstream factors( factorsName );

    Tiredness ::Initialize( factors );
    Experience::Initialize( factors );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Close
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void MainWindow::Close()
{
    network_.Disconnect();
    model_.Purge();
    glPlaceHolder_ = new GlPlaceHolder( this );
    setCentralWidget( glPlaceHolder_ );
    glPlaceHolder_->show();

    delete widget2d_; widget2d_ = 0;
    delete widget3d_; widget3d_ = 0;
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
        if( new3d != b3d_ && ( widget3d_ || ! scipioXml_.empty() ) )
        {
            centralWidget()->hide();
            disconnect( displayTimer_, SIGNAL( timeout()), centralWidget(), SLOT( updateGL() ) );
            if( new3d )
            {
                if( ! widget3d_ )
                {
                    widget3d_ = new Gl3dWidget( this, controllers_, scipioXml_, staticModel_.detection_ );
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
    if( simulation.IsConnected() )
        CompareConfigPath( simulation.GetSimulationHost(), simulation.GetConfigPath() );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::CompareConfigPath
// Created: AGE 2006-07-03
// -----------------------------------------------------------------------------
void MainWindow::CompareConfigPath( const std::string& server, const std::string& serverPath )
{
    if( serverPath.empty() || ! scipioXml_.empty() )
        return;
    
    if( server.find( "127.0.0.1" ) != std::string::npos )
        Load( serverPath );
    else
        Load( BuildRemotePath( server, serverPath ) );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::BuildRemotePath
// Created: AGE 2006-07-03
// -----------------------------------------------------------------------------
std::string MainWindow::BuildRemotePath( std::string server, std::string path )
{
    server = server.substr( 0, server.find( ':' ) );
    const char drive = path.at( 0 );
    path = path.substr( 2 );
    return "\\\\" + server + "\\" + drive + '$' + path;
}

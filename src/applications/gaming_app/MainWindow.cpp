// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MainWindow.h"
#include "moc_MainWindow.cpp"

#include "ControllerToolbar.h"
#include "Dialogs.h"
#include "EventToolbar.h"
#include "InfoPanels.h"
#include "LogisticToolbar.h"
#include "MagicOrdersInterface.h"
#include "MapToolbar.h"
#include "Menu.h"
#include "MissionPanel.h"
#include "ObjectCreationPanel.h"
#include "RecorderToolbar.h"
#include "SIMControlToolbar.h"
#include "UnitToolbar.h"
#include "LinkInterpreter.h"
#include "AgentListViewImp.h"
#include "AgentsLayerImp.h"
#include "PopulationsLayerImp.h"
#include "AgentKnowledgesLayer.h"
#include "PopulationKnowledgesLayer.h"
#include "MeteoLayer.h"
#include "ObjectKnowledgesLayer.h"
#include "StatusBar.h"
#include "Spy.h"

#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DataDictionary.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/PathTools.h"

#include "gaming/AgentServerMsgMgr.h"
#include "gaming/CoordinateConverter.h"
#include "gaming/Model.h"
#include "gaming/Network.h"
#include "gaming/Population.h"
#include "gaming/Simulation.h"
#include "gaming/StaticModel.h"

#include "clients_gui/AgentList.h"
#include "clients_gui/Gl3dWidget.h"
#include "clients_gui/GlWidget.h"
#include "clients_gui/GraphicPreferences.h"
#include "clients_gui/Logger.h"
#include "clients_gui/MissionLayer.h"
#include "clients_gui/ObjectList.h"
#include "clients_gui/OptionsPanel.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/Settings.h"
#include "clients_gui/PopulationList.h"
#include "clients_gui/PreferencesDialog.h"
#include "clients_gui/GlPlaceHolder.h"
#include "clients_gui/RichItemFactory.h"
#include "clients_gui/MiniViews.h"
#include "clients_gui/resources.h"
#include "clients_gui/IconLayout.h"
#include "clients_gui/GlProxy.h"
#include "clients_gui/ColorStrategy.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/Elevation2dLayer.h"
#include "clients_gui/Elevation3dLayer.h"
#include "clients_gui/TerrainLayer.h"
#include "clients_gui/MetricsLayer.h"
#include "clients_gui/GridLayer.h"
#include "clients_gui/LimitsLayer.h"
#include "clients_gui/ObjectsLayer.h"
#include "clients_gui/CircularEventStrategy.h"
#include "clients_gui/DefaultLayer.h"

#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )
namespace bfs = boost::filesystem;

#include "xeumeuleu/xml.h"
using namespace xml;

using namespace kernel;
using namespace gui;

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
    , glProxy_    ( 0 )
    , widget2d_   ( 0 )
    , widget3d_   ( 0 )
    , iconLayout_ ( 0 )
    , b3d_        ( false )
{
    setIcon( MAKE_PIXMAP( astec ) );
    setCaption( APP_NAME );

    Publisher_ABC& publisher = network_.GetMessageMgr();

    PreferencesDialog* prefDialog = new PreferencesDialog( this, controllers );
    new Dialogs( this, controllers, model_, staticModel, publisher ); // $$$$ SBO 2006-06-30: leak

    glProxy_ = new GlProxy();
    strategy_ = new ColorStrategy( controllers, *glProxy_ );

    RichItemFactory* factory = new RichItemFactory( this ); // $$$$ AGE 2006-05-11: aggregate somewhere
    LinkInterpreter* interpreter = new LinkInterpreter( this, controllers );
    connect( factory, SIGNAL( LinkClicked( const QString& ) ), interpreter, SLOT( Interprete( const QString& ) ) );

    // Agent list panel
    QDockWindow* pListDockWnd_ = new QDockWindow( this );
    moveDockWindow( pListDockWnd_, Qt::DockLeft );
    QTabWidget* pListsTabWidget = new QTabWidget( pListDockWnd_ );

    pListsTabWidget->addTab( new AgentList< AgentListViewImp >( controllers, publisher, *factory ), tr( "Agents" ) );
    pListsTabWidget->addTab( new ObjectList    ( controllers, *factory ),            tr( "Objets" ) );
    pListsTabWidget->addTab( new PopulationList( controllers, *factory ),            tr( "Populations" ) );
	pListDockWnd_->setWidget( pListsTabWidget );
    pListDockWnd_->setResizeEnabled( true );
    pListDockWnd_->setCloseMode( QDockWindow::Always );
    pListDockWnd_->setCaption( tr( "Unit�s" ) );
    setDockEnabled( pListDockWnd_, Qt::DockTop, false );

    // Spy
    QDockWindow* pSpyWnd = new QDockWindow( this );
    moveDockWindow( pSpyWnd, Qt::DockLeft );
    spy_ = new Spy( pSpyWnd, controllers );
    pSpyWnd->setWidget( spy_ );
    pSpyWnd->setResizeEnabled( true );
    pSpyWnd->setCloseMode( QDockWindow::Always );
    pSpyWnd->setCaption( tr( "Espion" ) );
    setDockEnabled( pSpyWnd, Qt::DockTop, false );

    MiniViews* miniviews = new MiniViews( this, controllers_, widget2d_ ); // $$$$ AGE 2006-08-21: widget2d en *& d�gueu. Instancier l'un quand l'autre
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

    // A few layers
    ParametersLayer* paramLayer = new ParametersLayer( *glProxy_ );
    AgentsLayer*     agentsLayer = new AgentsLayerImp( controllers, *glProxy_, *strategy_, *glProxy_ );

     // Mission panel
    MissionPanel* pMissionPanel_ = new MissionPanel( this, controllers_, staticModel_, publisher, *paramLayer, *glProxy_ );
    moveDockWindow( pMissionPanel_, Qt::DockLeft );
    setDockEnabled( pMissionPanel_, Qt::DockTop, false );
    setAppropriate( pMissionPanel_, false );
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
    ObjectCreationPanel* objectCreationPanel = new ObjectCreationPanel( pObjectCreationWnd, controllers, publisher, staticModel_, *paramLayer, *glProxy_ );
    pObjectCreationWnd->setWidget( objectCreationPanel );
    pObjectCreationWnd->setResizeEnabled( true );
    pObjectCreationWnd->setCloseMode( QDockWindow::Always );
    pObjectCreationWnd->setCaption( tr( "Cr�ation d'objet" ) );
    setDockEnabled( pObjectCreationWnd, Qt::DockTop, false );
    
    new MagicOrdersInterface( this, controllers_, publisher, staticModel_, *paramLayer );
    new SIMControlToolbar( this, controllers, network, publisher );
    new MapToolbar( this, controllers );
    new ControllerToolbar( this, controllers );
    new UnitToolbar( this, controllers );
    new LogisticToolbar( this, controllers, *agentsLayer );
    new EventToolbar( this, controllers );
    RecorderToolbar* recorderToolbar = new RecorderToolbar( this, network );

    new Menu( this, controllers, *prefDialog, *recorderToolbar, *factory );

    glPlaceHolder_ = new GlPlaceHolder( this );
    setCentralWidget( glPlaceHolder_ );

    // $$$$ AGE 2006-08-22: prefDialog->GetPreferences()
    CreateLayers( *pMissionPanel_, *objectCreationPanel, *paramLayer, *agentsLayer, prefDialog->GetPreferences() );

    pStatus_ = new ::StatusBar( statusBar(), staticModel_.detection_, staticModel_.coordinateConverter_, controllers_ );
    controllers_.Register( *this );

    displayTimer_ = new QTimer( this );

    ReadSettings();
    ReadOptions();
    pMissionPanel_->hide();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::CreateLayers
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void MainWindow::CreateLayers( MissionPanel& missions, ObjectCreationPanel& objects, ParametersLayer& parameters, AgentsLayer& agents, GraphicSetup_ABC& setup )
{
    CircularEventStrategy* eventStrategy = new CircularEventStrategy();
    eventStrategy_ = eventStrategy;
    Layer_ABC& missionsLayer        = *new MiscLayer< MissionPanel >( missions );
    Layer_ABC& objectCreationLayer  = *new MiscLayer< ObjectCreationPanel >( objects );
    Layer_ABC& elevation2d          = *new Elevation2dLayer( controllers_.controller_, staticModel_.detection_ );
    Layer_ABC& elevation3d          = *new Elevation3dLayer( controllers_.controller_, staticModel_.detection_ );
    Layer_ABC& terrain              = *new TerrainLayer( controllers_, *glProxy_, setup );
    Layer_ABC& grid                 = *new GridLayer( controllers_, *glProxy_ );
    Layer_ABC& metrics              = *new MetricsLayer( *glProxy_ );
    Layer_ABC& limits               = *new LimitsLayer( controllers_, *glProxy_, *strategy_, parameters, model_.limits_ );
    Layer_ABC& objectsLayer         = *new ObjectsLayer( controllers_, *glProxy_, *strategy_, *glProxy_ );
    Layer_ABC& populations          = *new PopulationsLayerImp( controllers_, *glProxy_, *strategy_, *glProxy_ );
    Layer_ABC& agentKnowledges      = *new AgentKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_ );
    Layer_ABC& populationKnowledges = *new PopulationKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_ );
    Layer_ABC& objectKnowledges     = *new ObjectKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_ );
    Layer_ABC& meteo                = *new MeteoLayer( controllers_, *glProxy_ );
    Layer_ABC& defaultLayer         = *new DefaultLayer( controllers_ );
    
    // ordre de dessin
    glProxy_->Register( defaultLayer );
    glProxy_->Register( elevation2d );
    glProxy_->Register( elevation3d );
    glProxy_->Register( terrain );
    glProxy_->Register( grid );
    glProxy_->Register( meteo );
    glProxy_->Register( limits );
    glProxy_->Register( objectsLayer );
    glProxy_->Register( populations );
    glProxy_->Register( agents );
    glProxy_->Register( objectKnowledges );
    glProxy_->Register( populationKnowledges );
    glProxy_->Register( agentKnowledges );
    glProxy_->Register( missionsLayer );
    glProxy_->Register( objectCreationLayer );
    glProxy_->Register( parameters );
    glProxy_->Register( metrics );

    // ordre des evenements
    eventStrategy->Register( parameters );
    eventStrategy->Register( agents );
    eventStrategy->Register( populations );
    eventStrategy->Register( objectsLayer );
    eventStrategy->Register( agentKnowledges );
    eventStrategy->Register( populationKnowledges );
    eventStrategy->Register( objectKnowledges );
    eventStrategy->Register( limits );
    eventStrategy->Register( metrics );
    eventStrategy->SetDefault( defaultLayer );
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
    BuildIconLayout();
    scipioXml_ = scipioXml;
    delete widget2d_; widget2d_ = 0;
    delete widget3d_; widget3d_ = 0;
    widget2d_ = new GlWidget( this, controllers_, scipioXml, *iconLayout_, *eventStrategy_ );
    glProxy_->ChangeTo( widget2d_ );
    glProxy_->RegisterTo( widget2d_ );
    delete glPlaceHolder_; glPlaceHolder_ = 0;
    setCentralWidget( widget2d_ );
    model_.Purge();
    staticModel_.Load( scipioXml );

    b3d_ = false;
    controllers_.options_.Change( "3D", b3d_ );
    
    connect( widget2d_, SIGNAL( MouseMove( const geometry::Point2f& ) ), pStatus_, SLOT( OnMouseMove( const geometry::Point2f& ) ) );
    connect( displayTimer_, SIGNAL( timeout()), centralWidget(), SLOT( updateGL() ) );
    displayTimer_->start( 50 );
    widget2d_->show();
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
    delete glProxy_;
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
                    widget3d_ = new Gl3dWidget( this, controllers_, scipioXml_, staticModel_.detection_, *eventStrategy_ );
                    connect( widget3d_, SIGNAL( MouseMove( const geometry::Point3f& ) ), pStatus_, SLOT( OnMouseMove( const geometry::Point3f& ) ) );
                    glProxy_->RegisterTo( widget3d_ );
                }
                glProxy_->ChangeTo( widget3d_ );
                setCentralWidget( widget3d_ );
            }
            else
            {
                glProxy_->ChangeTo( widget2d_ );
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

// -----------------------------------------------------------------------------
// Name: MainWindow::BuildIconLayout
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
void MainWindow::BuildIconLayout()
{
    if( iconLayout_ )
        return;
    iconLayout_ = new IconLayout();
    iconLayout_->AddIcon( xpm_cadenas        , -200, 270 );
    iconLayout_->AddIcon( xpm_radars_on      ,  200, 270 );
    iconLayout_->AddIcon( xpm_brouillage     ,  200, 50 );
    iconLayout_->AddIcon( xpm_talkie_interdit,  100, 50 );
    iconLayout_->AddIcon( xpm_gas            , -200, 170 );
    iconLayout_->AddIcon( xpm_ammo           , -200, 100 );
    iconLayout_->AddIcon( xpm_nbc            , -200, 25 );
    iconLayout_->AddIcon( xpm_construction   ,  200, 150 );
    iconLayout_->AddIcon( xpm_observe        ,  200, 150 );
}

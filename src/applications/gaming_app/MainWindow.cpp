// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MainWindow.h"
#include "moc_MainWindow.cpp"

#include "Dialogs.h"
#include "EventToolbar.h"
#include "InfoPanels.h"
#include "OrbatToolbar.h"
#include "MagicOrdersInterface.h"
#include "Menu.h"
#include "MissionPanel.h"
#include "ObjectCreationPanel.h"
#include "RecorderToolbar.h"
#include "SIMControlToolbar.h"
#include "LinkInterpreter.h"
#include "AgentsLayer.h"
#include "PopulationsLayer.h"
#include "AgentKnowledgesLayer.h"
#include "PopulationKnowledgesLayer.h"
#include "MeteoLayer.h"
#include "ObjectKnowledgesLayer.h"
#include "LimitsLayer.h"
#include "StatusBar.h"
#include "Spy.h"
#include "AgentList.h"
#include "LoginDialog.h"
#include "Properties.h"
#include "TacticalList.h"
#include "XPSPlayer.h"
#include "ProfilingPanel.h"
#include "UserProfileDialog.h"
#include "InfoDock.h"
#include "ActionsPanel.h"
#include "icons.h"

#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/ExerciseConfig.h"

#include "gaming/AgentServerMsgMgr.h"
#include "gaming/Model.h"
#include "gaming/Network.h"
#include "gaming/Population.h"
#include "gaming/Simulation.h"
#include "gaming/StaticModel.h"
#include "gaming/Profile.h"
#include "gaming/ProfileFilter.h"

#include "clients_gui/DisplayToolbar.h"
#include "clients_gui/GlSelector.h"
#include "clients_gui/GraphicPreferences.h"
#include "clients_gui/Logger.h"
#include "clients_gui/MissionLayer.h"
#include "clients_gui/ObjectList.h"
#include "clients_gui/OptionsPanel.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/Settings.h"
#include "clients_gui/PopulationList.h"
#include "clients_gui/PreferencesDialog.h"
#include "clients_gui/RichItemFactory.h"
#include "clients_gui/MiniViews.h"
#include "clients_gui/resources.h"
#include "clients_gui/GlProxy.h"
#include "clients_gui/ColorStrategy.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/Elevation2dLayer.h"
#include "clients_gui/Elevation3dLayer.h"
#include "clients_gui/TerrainLayer.h"
#include "clients_gui/RasterLayer.h"
#include "clients_gui/MetricsLayer.h"
#include "clients_gui/GridLayer.h"
#include "clients_gui/ObjectsLayer.h"
#include "clients_gui/CircularEventStrategy.h"
#include "clients_gui/ExclusiveEventStrategy.h"
#include "clients_gui/DefaultLayer.h"
#include "clients_gui/DrawerLayer.h"
#include "clients_gui/LogoLayer.h"
#include "clients_gui/DrawerToolbar.h"
#include "clients_gui/SymbolIcons.h"
#include "clients_gui/EntitySymbols.h"
#include "clients_gui/LightingProxy.h"
#include "clients_gui/LocationEditorToolbar.h"

#include "xeumeuleu/xml.h"

#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )

namespace bfs = boost::filesystem;
using namespace xml;
using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::MainWindow( Controllers& controllers, StaticModel& staticModel, Model& model, Network& network, const kernel::Profile_ABC& p, kernel::ExerciseConfig& config )
    : QMainWindow( 0, 0, Qt::WDestructiveClose )
    , controllers_  ( controllers )
    , staticModel_  ( staticModel )
    , model_        ( model )
    , network_      ( network )
    , config_       ( config )
    , forward_      ( new gui::CircularEventStrategy() )
    , eventStrategy_( new gui::ExclusiveEventStrategy( *forward_ ) )
    , glProxy_      ( 0 )
{
    setIcon( MAKE_PIXMAP( csword ) );
    setCaption( APP_NAME + tr( " - Not connected" ) );

    ProfileFilter& profile = *new ProfileFilter( controllers, p ); // $$$$ AGE 2006-12-13: mem. // $$$$ _RC_ MCO 2007-01-12: auto_ptr

    Publisher_ABC& publisher = network_.GetMessageMgr();

    lighting_ = new LightingProxy( this );
    PreferencesDialog* prefDialog = new PreferencesDialog( this, controllers, *lighting_ );
    new Dialogs( this, controllers, model_, staticModel, publisher, profile );

    glProxy_ = new GlProxy();
    strategy_ = new ColorStrategy( controllers, *glProxy_ );

    selector_ = new GlSelector( this, *glProxy_, controllers, config, staticModel.detection_, *eventStrategy_ );
    selector_->AddIcon( xpm_cadenas        , -200, 270 );
    selector_->AddIcon( xpm_radars_on      ,  200, 270 );
    selector_->AddIcon( xpm_brouillage     ,  200, 50 );
    selector_->AddIcon( xpm_talkie_interdit,  100, 50 );
    selector_->AddIcon( xpm_gas            , -200, 170 );
    selector_->AddIcon( xpm_ammo           , -200, 100 );
    selector_->AddIcon( xpm_nbc            , -200, 25 );
    selector_->AddIcon( xpm_construction   ,  200, 150 );
    selector_->AddIcon( xpm_observe        ,  200, 150 );

    RichItemFactory* factory = new RichItemFactory( this ); // $$$$ AGE 2006-05-11: aggregate somewhere
    LinkInterpreter* interpreter = new LinkInterpreter( this, controllers );
    connect( factory, SIGNAL( LinkClicked( const QString& ) ), interpreter, SLOT( Interprete( const QString& ) ) );

    // A few layers
    ParametersLayer* paramLayer = new ParametersLayer( *glProxy_, *new gui::LocationEditorToolbar( this, controllers_, staticModel_.coordinateConverter_, *glProxy_ ) );
    ::AgentsLayer* agentsLayer = new ::AgentsLayer( controllers, *glProxy_, *strategy_, *glProxy_, profile );

    // Agent list panel
    QDockWindow* pListDockWnd_ = new QDockWindow( this );
    moveDockWindow( pListDockWnd_, Qt::DockLeft );
    QVBox* box = new QVBox( pListDockWnd_ );
    new OrbatToolbar( box, controllers, profile, *agentsLayer );
    QTabWidget* pListsTabWidget = new QTabWidget( box );

    SymbolIcons* symbols = new SymbolIcons( this );
    connect( selector_, SIGNAL( Widget2dChanged( gui::GlWidget* ) ), symbols, SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );
    EntitySymbols* icons = new EntitySymbols( *symbols, *strategy_ );
    UserProfileDialog* profileDialog = new UserProfileDialog( this, controllers, *factory, profile, *icons, model_.userProfileFactory_ );

    pListsTabWidget->addTab( new TacticalList  ( controllers, *factory, profile, *icons ),            tr( "Tactical" ) );
    pListsTabWidget->addTab( new AgentList     ( controllers, publisher, *factory, profile, *icons ), tr( "Communication" ) );
    pListsTabWidget->addTab( new ObjectList    ( controllers, *factory, profile ),                    tr( "Objects" ) );
    pListsTabWidget->addTab( new PopulationList( controllers, *factory, profile ),                    tr( "Populations" ) );
    pListDockWnd_->setWidget( box );
    pListDockWnd_->setResizeEnabled( true );
    pListDockWnd_->setCloseMode( QDockWindow::Always );
    pListDockWnd_->setCaption( tr( "Orbat" ) );
    setDockEnabled( pListDockWnd_, Qt::DockTop, false );

    // Spy
    QDockWindow* pSpyWnd = new QDockWindow( this );
    moveDockWindow( pSpyWnd, Qt::DockLeft );
    spy_ = new Spy( pSpyWnd, controllers );
    pSpyWnd->setWidget( spy_ );
    pSpyWnd->setResizeEnabled( true );
    pSpyWnd->setCloseMode( QDockWindow::Always );
    pSpyWnd->setCaption( tr( "Spy" ) );
    setDockEnabled( pSpyWnd, Qt::DockTop, false );

    // Mini views
    MiniViews* miniviews = new MiniViews( this, controllers_ );
    connect( selector_, SIGNAL( Widget2dChanged( gui::GlWidget* ) ), miniviews, SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );
    miniviews->hide();

    // Properties
    QDockWindow* pPropertiesWnd = new QDockWindow( this );
    moveDockWindow( pPropertiesWnd, Qt::DockLeft );
    Properties* properties = new Properties( pPropertiesWnd, controllers );
    pPropertiesWnd->setWidget( properties );
    pPropertiesWnd->setResizeEnabled( true );
    pPropertiesWnd->setCloseMode( QDockWindow::Always );
    pPropertiesWnd->setCaption( tr( "Properties" ) );
    setDockEnabled( pPropertiesWnd, Qt::DockTop, false );

    // Info panel
    QDockWindow* pInfoDockWnd_ = new QDockWindow( this );
    moveDockWindow( pInfoDockWnd_, Qt::DockRight );
    InfoPanels* pInfoPanel_ = new InfoPanels( pInfoDockWnd_, controllers, *factory, publisher );
    pInfoDockWnd_->setWidget( pInfoPanel_ );
    pInfoDockWnd_->setResizeEnabled( true );
    pInfoDockWnd_->setCloseMode( QDockWindow::Always );
    pInfoDockWnd_->setCaption( tr( "Information" ) );
    setDockEnabled( pInfoDockWnd_, Qt::DockTop, false );

     // Mission panel
    MissionPanel* pMissionPanel_ = new MissionPanel( this, controllers_, staticModel_, publisher, *paramLayer, *glProxy_, profile, model_.actions_ );
    moveDockWindow( pMissionPanel_, Qt::DockLeft );
    setDockEnabled( pMissionPanel_, Qt::DockTop, false );
    setAppropriate( pMissionPanel_, false );
    pMissionPanel_->hide();

    // Actions panel
    ActionsPanel* actionsPanel = new ActionsPanel( this, controllers_, *factory );
    moveDockWindow( actionsPanel, Qt::DockRight );
    setDockEnabled( actionsPanel, Qt::DockTop, false );
    actionsPanel->hide();


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

    // Info
    QDockWindow* infoWnd = new InfoDock( this, controllers_, p, *icons, *factory );
    moveDockWindow( infoWnd, Qt::DockBottom );
    setDockEnabled( infoWnd, Qt::DockTop, false );

    // Profiler
    QDockWindow* pProfilerDockWnd_ = new QDockWindow( this );
    moveDockWindow( pProfilerDockWnd_, Qt::DockRight );
    ProfilingPanel* profilingPanel_ = new ProfilingPanel( pProfilerDockWnd_, controllers_, network_ );
    pProfilerDockWnd_->setWidget( profilingPanel_ );
    pProfilerDockWnd_->setResizeEnabled( true );
    pProfilerDockWnd_->setCloseMode( QDockWindow::Always );
    pProfilerDockWnd_->setCaption( tr( "Profiling" ) );
    setDockEnabled( pProfilerDockWnd_, Qt::DockTop, false );

    // object creation window
    QDockWindow* pObjectCreationWnd = new QDockWindow( this );
    moveDockWindow( pObjectCreationWnd, Qt::DockRight );
    pObjectCreationWnd->hide();
    ObjectCreationPanel* objectCreationPanel = new ObjectCreationPanel( pObjectCreationWnd, controllers, publisher, staticModel_, *paramLayer, *glProxy_ );
    pObjectCreationWnd->setWidget( objectCreationPanel );
    pObjectCreationWnd->setResizeEnabled( true );
    pObjectCreationWnd->setCloseMode( QDockWindow::Always );
    pObjectCreationWnd->setCaption( tr( "Object creation" ) );
    setDockEnabled( pObjectCreationWnd, Qt::DockTop, false );

    new MagicOrdersInterface( this, controllers_, publisher, staticModel_, *paramLayer, profile );
    new SIMControlToolbar( this, controllers, network, publisher );
    new DisplayToolbar( this, controllers );
    new EventToolbar( this, controllers, profile );
    RecorderToolbar* recorderToolbar = new RecorderToolbar( this, network );

    // Drawer
    DrawerLayer* drawer = new DrawerLayer( *glProxy_ );
    new DrawerToolbar( this, *eventStrategy_, *drawer, *glProxy_ );

    new Menu( this, controllers, *prefDialog, *profileDialog, *recorderToolbar, *factory );

    // $$$$ AGE 2006-08-22: prefDialog->GetPreferences()
    CreateLayers( *pMissionPanel_, *objectCreationPanel, *paramLayer, *agentsLayer, *drawer, *prefDialog, profile );

    ::StatusBar* pStatus = new ::StatusBar( statusBar(), staticModel_.detection_, staticModel_.coordinateConverter_, controllers_ );
    connect( selector_, SIGNAL( MouseMove( const geometry::Point2f& ) ), pStatus, SLOT( OnMouseMove( const geometry::Point2f& ) ) );
    connect( selector_, SIGNAL( MouseMove( const geometry::Point3f& ) ), pStatus, SLOT( OnMouseMove( const geometry::Point3f& ) ) );
    controllers_.Register( *this );

    ReadSettings();
    ReadOptions();
    pMissionPanel_->hide();

    new XPSPlayer( this, controllers_ );

    if( bfs::exists( bfs::path( config_.GetExerciseFile(), bfs::native ) ) )
        Load();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::CreateLayers
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void MainWindow::CreateLayers( MissionPanel& missions, ObjectCreationPanel& objects, ParametersLayer& parameters, gui::AgentsLayer& agents, DrawerLayer& drawer, PreferencesDialog& preferences, const Profile_ABC& profile )
{
    Layer_ABC& missionsLayer        = *new MiscLayer< MissionPanel >( missions );
    Layer_ABC& objectCreationLayer  = *new MiscLayer< ObjectCreationPanel >( objects );
    Elevation2dLayer& elevation2d   = *new Elevation2dLayer( controllers_.controller_, staticModel_.detection_ );
    Layer2d_ABC& raster             = *new RasterLayer( controllers_.controller_ );
    Layer2d_ABC& terrain            = *new TerrainLayer( controllers_, *glProxy_, preferences.GetPreferences() );
    Layer_ABC& elevation3d          = *new Elevation3dLayer( controllers_.controller_, staticModel_.detection_, *lighting_ );
    Layer_ABC& grid                 = *new GridLayer( controllers_, *glProxy_ );
    Layer_ABC& metrics              = *new MetricsLayer( *glProxy_ );
    Layer_ABC& limits               = *new LimitsLayer( controllers_, *glProxy_, *strategy_, parameters, model_.tacticalLineFactory_, *glProxy_, profile );
    Layer_ABC& objectsLayer         = *new ObjectsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    Layer_ABC& populations          = *new ::PopulationsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    Layer_ABC& agentKnowledges      = *new AgentKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    Layer_ABC& populationKnowledges = *new PopulationKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    Layer_ABC& objectKnowledges     = *new ObjectKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    Layer_ABC& meteo                = *new MeteoLayer( controllers_, *glProxy_ );
    Layer_ABC& defaultLayer         = *new DefaultLayer( controllers_ );
    Layer2d_ABC& logoLayer          = *new LogoLayer( *glProxy_, QImage( "logo.png" ), 0.7f );

    preferences.AddLayer( tr( "Terrain" ), terrain );
    preferences.AddLayer( tr( "Raster" ), raster );
    preferences.AddLayer( tr( "Elevation" ), elevation2d );
    preferences.AddLayer( tr( "Logo" ), logoLayer );

    // ordre de dessin
    glProxy_->Register( defaultLayer );
    glProxy_->Register( elevation2d );
    glProxy_->Register( raster );
    glProxy_->Register( terrain );
    glProxy_->Register( elevation3d );
    glProxy_->Register( grid );
    glProxy_->Register( meteo );
    glProxy_->Register( limits );
    glProxy_->Register( objectKnowledges );
    glProxy_->Register( populationKnowledges );
    glProxy_->Register( agentKnowledges );
    glProxy_->Register( objectsLayer );
    glProxy_->Register( populations );
    glProxy_->Register( agents );
    glProxy_->Register( missionsLayer );
    glProxy_->Register( objectCreationLayer );
    glProxy_->Register( parameters );
    glProxy_->Register( metrics );
    glProxy_->Register( drawer );
    glProxy_->Register( logoLayer );
    
    // ordre des evenements
    forward_->Register( parameters );
    forward_->Register( agents );
    forward_->Register( populations );
    forward_->Register( objectsLayer );
    forward_->Register( agentKnowledges );
    forward_->Register( populationKnowledges );
    forward_->Register( objectKnowledges );
    forward_->Register( limits );
    forward_->Register( metrics );
    forward_->Register( elevation3d );
    forward_->SetDefault( defaultLayer );
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
        const QString filename = QFileDialog::getOpenFileName( config_.GetExerciseFile().c_str(), "Exercise (*.xml)", this, 0, tr( "Load exercise definition file (exercise.xml)" ) );
        if( filename.isEmpty() )
            return;
        current = filename;
        if( current.substr( 0, 2 ) == "//" )
            std::replace( current.begin(), current.end(), '/', '\\' );
    }
    config_.LoadExercise( current );
    Load();
    // $$$$ NLD 2007-01-12: change window title ?
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Load
// Created: AGE 2006-05-03
// -----------------------------------------------------------------------------
void MainWindow::Load()
{
    model_.Purge();
    selector_->Load();
    staticModel_.Load( config_ );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Close
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void MainWindow::Close()
{
    network_.Disconnect();
    model_.Purge();
    selector_->Close();
}

// -----------------------------------------------------------------------------
// Name: MainWindow destructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    controllers_.Remove( *this );
    delete selector_;
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
    settings.setPath( "MASA", "CSword" );
    settings.beginGroup( "/Gaming" );

    // Pannel configuration
    QString strDockConfig;
    QTextStream strDockConfigStream( &strDockConfig, IO_WriteOnly );
    strDockConfigStream << *this;
    settings.writeEntry( "/Panels", strDockConfig );
    settings.WriteEntry( "/MainWindow", *this );

    settings.endGroup(); // gaming
}

// -----------------------------------------------------------------------------
// Name: MainWindow::ReadSettings
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::ReadSettings()
{
    Settings settings;
    settings.setPath( "MASA", "CSword" );
    settings.beginGroup( "/Gaming" );

    // Pannel configuration
    QString strDockConfig;
    strDockConfig = settings.readEntry( "/Panels" );
    QTextStream strDockConfigStream( &strDockConfig, IO_ReadOnly );
    strDockConfigStream >> *this;

    // Main window configuration
    settings.ReadEntry( "/MainWindow", *this, 800, 600, 100, 100, false );

    settings.endGroup(); // gaming
}

// -----------------------------------------------------------------------------
// Name: MainWindow::WriteOptions
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::WriteOptions()
{
    Settings settings;
    settings.setPath( "MASA", "CSword" );
    settings.beginGroup( "/Gaming" );
    settings.beginGroup( "/Options" );
    controllers_.options_.Save( settings );
    settings.endGroup();
    settings.endGroup();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::ReadOptions
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::ReadOptions()
{
    Settings settings;
    settings.setPath( "MASA", "CSword" );
    settings.beginGroup( "/Gaming" );
    settings.beginGroup( "/Options" );
    controllers_.options_.Load( settings );
    settings.endGroup();
    settings.endGroup();
}

namespace
{
    struct SelectionStub{};

    QString ExtractExerciceName( const std::string& filename )
    {
        if( filename.empty() )
            return "";
        else
        {
            std::string file = bfs::path( filename, bfs::native ).leaf();
            file = file.substr( 0, file.find_last_of( '.' ) );
            return file.c_str();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::NotifyUpdated
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void MainWindow::NotifyUpdated( const Simulation& simulation )
{
    if( ! simulation.IsConnected() )
    {
        setCaption( APP_NAME + tr( " - Not connected" ) );
        controllers_.actions_.Select( SelectionStub() );
        model_.Purge();
    }
    if( simulation.IsConnected() )
    {
//        CompareConfigPath( simulation.GetSimulationHost(), simulation.GetConfigPath() );
        setCaption( APP_NAME + QString( " - [%1@%2][%3]" ).arg( profile_ ).arg( simulation.GetSimulationHost().c_str() ).arg( ExtractExerciceName( "" ) ) ); //$$$$$ POURRI
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::NotifyUpdated
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void MainWindow::NotifyUpdated( const Profile& profile )
{
    if( ! profile.IsLoggedIn() )
    {
        profile_ = profile.GetLogin();
        static LoginDialog* dialog = new LoginDialog( this, profile, network_.GetMessageMgr() );
        // $$$$ AGE 2006-10-11: exec would create a reentrance...
        QTimer::singleShot( 0, dialog, SLOT(exec()) );
    }
    else
        profile_ = profile.GetLogin();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::CompareConfigPath
// Created: AGE 2006-07-03
// -----------------------------------------------------------------------------
//void MainWindow::CompareConfigPath( const std::string& server, const std::string& serverPath )
//{
    // $$$$ NLD 2007-01-12: TODO: change ASN message...
//    if( serverPath.empty() || ! scipioXml_.empty() )
//        return;
//    
//    if( server.find( "127.0.0.1" ) != std::string::npos )
//        Load( serverPath );
//    else
//        Load( BuildRemotePath( server, serverPath ) );
//}

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


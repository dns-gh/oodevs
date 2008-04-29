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
#include "CreationPanels.h"
#include "OrderBrowser.h"
#include "SIMControlToolbar.h"
#include "LinkInterpreter.h"
#include "AgentsLayer.h"
#include "PopulationsLayer.h"
#include "AgentKnowledgesLayer.h"
#include "PopulationKnowledgesLayer.h"
#include "MeteoLayer.h"
#include "ObjectsLayer.h"
#include "ObjectKnowledgesLayer.h"
#include "LimitsLayer.h"
#include "FogLayer.h"
#include "StatusBar.h"
#include "AgentList.h"
#include "LoginDialog.h"
#include "TacticalList.h"
#include "XPSPlayer.h"
#include "ProfilingPanel.h"
#include "UserProfileDialog.h"
#include "InfoDock.h"
#include "ActionsPanel.h"
#include "TimelinePanel.h"
#include "ReplayerToolbar.h"
#include "icons.h"
#include "ClockDock.h"
#include "AutomatsLayer.h"
#include "FolkLayer.h"
#include "FolkToolbar.h"
#include "AfterAction.h"
#include "SimulationLighting.h"
#include "IntelligencesLayer.h"
#include "Properties.h"

#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"

#include "gaming/AgentServerMsgMgr.h"
#include "gaming/Model.h"
#include "gaming/Network.h"
#include "gaming/Population.h"
#include "gaming/StaticModel.h"
#include "gaming/Profile.h"
#include "gaming/ProfileFilter.h"
#include "gaming/VisionConesToggler.h"
#include "gaming/ActionsScheduler.h"
#include "gaming/IntelligencesModel.h"

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
#include "clients_gui/CircularEventStrategy.h"
#include "clients_gui/ExclusiveEventStrategy.h"
#include "clients_gui/DefaultLayer.h"
#include "clients_gui/DrawerLayer.h"
#include "clients_gui/LogoLayer.h"
#include "clients_gui/DrawerToolbar.h"
#include "clients_gui/SymbolIcons.h"
#include "clients_gui/EntitySymbols.h"
#include "clients_gui/LocationEditorToolbar.h"
#include "clients_gui/LocationsLayer.h"
#include "clients_gui/FormationLayer.h"
#include "clients_gui/IntelligenceList.h"
#include "clients_gui/TooltipsLayer.h"

#include "tools/ExerciseConfig.h"

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
MainWindow::MainWindow( Controllers& controllers, StaticModel& staticModel, Model& model, const Simulation& simulation, Network& network, const kernel::Profile_ABC& p, tools::ExerciseConfig& config, const QString& license )
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

    ProfileFilter& profile = *new ProfileFilter( controllers, p ); // $$$$ AGE 2006-12-13: mem. // $$$$ _RC_ MCO 2007-01-12: auto_ptr // $$$$ AGE 2007-06-19: tégé !

    Publisher_ABC& publisher = network_.GetMessageMgr();

    lighting_ = new SimulationLighting( controllers, this );
    PreferencesDialog* prefDialog = new PreferencesDialog( this, controllers, *lighting_ );
    new Dialogs( this, controllers, model_, staticModel, publisher, profile );
    new VisionConesToggler( controllers, publisher, this );

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
    LocationsLayer* locationsLayer = new LocationsLayer( *glProxy_ );
    ParametersLayer* paramLayer = new ParametersLayer( *glProxy_, *new gui::LocationEditorToolbar( this, controllers_, staticModel_.coordinateConverter_, *glProxy_, *locationsLayer ) );
    ::AgentsLayer* agentsLayer = new ::AgentsLayer( controllers, *glProxy_, *strategy_, *glProxy_, profile );
    ::AutomatsLayer* automatsLayer = new ::AutomatsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, *agentsLayer, publisher, staticModel_.coordinateConverter_ );

    // Agent list panel
    QDockWindow* pListDockWnd_ = new QDockWindow( this );
    moveDockWindow( pListDockWnd_, Qt::DockLeft );
    QVBox* box = new QVBox( pListDockWnd_ );
    new OrbatToolbar( box, controllers, profile, *automatsLayer );
    QTabWidget* pListsTabWidget = new QTabWidget( box );

    SymbolIcons* symbols = new SymbolIcons( this );
    connect( selector_, SIGNAL( Widget2dChanged( gui::GlWidget* ) ), symbols, SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );
    EntitySymbols* icons = new EntitySymbols( *symbols, *strategy_ );
    UserProfileDialog* profileDialog = new UserProfileDialog( this, controllers, *factory, profile, *icons, model_.userProfileFactory_ );

    pListsTabWidget->addTab( new TacticalList    ( controllers, publisher, *factory, profile, *icons ), tr( "Tactical" ) );
    pListsTabWidget->addTab( new AgentList       ( controllers, publisher, *factory, profile, *icons ), tr( "Communication" ) );
    pListsTabWidget->addTab( new ObjectList      ( controllers, *factory, profile ),                    tr( "Objects" ) );
    pListsTabWidget->addTab( new PopulationList  ( controllers, *factory, profile ),                    tr( "Populations" ) );
    pListsTabWidget->addTab( new IntelligenceList( controllers, *factory, *icons, profile ),            tr( "Intelligences" ) );
    pListDockWnd_->setWidget( box );
    pListDockWnd_->setResizeEnabled( true );
    pListDockWnd_->setCloseMode( QDockWindow::Always );
    pListDockWnd_->setCaption( tr( "Orbat" ) );
    setDockEnabled( pListDockWnd_, Qt::DockTop, false );

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
    pPropertiesWnd->hide();

    // Info panel
    QDockWindow* pInfoDockWnd_ = new QDockWindow( this );
    moveDockWindow( pInfoDockWnd_, Qt::DockRight );
    InfoPanels* pInfoPanel_ = new InfoPanels( pInfoDockWnd_, controllers, *factory, publisher );
    pInfoDockWnd_->setWidget( pInfoPanel_ );
    pInfoDockWnd_->setResizeEnabled( true );
    pInfoDockWnd_->setCloseMode( QDockWindow::Always );
    pInfoDockWnd_->setCaption( tr( "Information" ) );
    setDockEnabled( pInfoDockWnd_, Qt::DockTop, false );
    pInfoDockWnd_->hide();

     // Mission panel
    MissionPanel* pMissionPanel_ = new MissionPanel( this, controllers_, staticModel_, publisher, *paramLayer, *glProxy_, profile, model_.actions_, model_.agentKnowledgeConverter_, model_.objectKnowledgeConverter_, simulation );
    moveDockWindow( pMissionPanel_, Qt::DockLeft );
    setDockEnabled( pMissionPanel_, Qt::DockTop, false );
    setAppropriate( pMissionPanel_, false );
    pMissionPanel_->hide();

    // Actions panel
    ActionsScheduler* scheduler = new ActionsScheduler( this, controllers_, simulation, model_.actions_, publisher );
    ActionsPanel* actionsPanel = new ActionsPanel( this, controllers_, *factory, model_.actions_, *scheduler, config_ );
    moveDockWindow( actionsPanel, Qt::DockRight );
    setDockEnabled( actionsPanel, Qt::DockTop, false );
    actionsPanel->hide();

    TimelinePanel* timelinePanel = new TimelinePanel( this, controllers_, model_.actions_, *scheduler );
    moveDockWindow( timelinePanel, Qt::DockTop );
    timelinePanel->hide();

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
    pLogDockWnd_->hide();

    // Info
    QDockWindow* infoWnd = new InfoDock( this, controllers_, p, *icons, *factory );
    moveDockWindow( infoWnd, Qt::DockBottom );
    setDockEnabled( infoWnd, Qt::DockTop, false );

    // Clock
    QDockWindow* clockWnd = new ClockDock( this, controllers_, simulation );
    moveDockWindow( clockWnd, Qt::DockRight );
    setDockEnabled( clockWnd, Qt::DockTop, false );

    // Profiler
    QDockWindow* pProfilerDockWnd_ = new QDockWindow( this );
    moveDockWindow( pProfilerDockWnd_, Qt::DockRight );
    ProfilingPanel* profilingPanel_ = new ProfilingPanel( pProfilerDockWnd_, controllers_, network_, simulation );
    pProfilerDockWnd_->setWidget( profilingPanel_ );
    pProfilerDockWnd_->setResizeEnabled( true );
    pProfilerDockWnd_->setCloseMode( QDockWindow::Always );
    pProfilerDockWnd_->setCaption( tr( "Profiling" ) );
    setDockEnabled( pProfilerDockWnd_, Qt::DockTop, false );
    pProfilerDockWnd_->hide();

    // object/unit creation window
    QDockWindow* pCreationWnd = new QDockWindow( this );
    moveDockWindow( pCreationWnd, Qt::DockRight );
    pCreationWnd->hide();
    CreationPanels* creationPanels = new CreationPanels( pCreationWnd, controllers, staticModel_, *factory, publisher, *paramLayer, *glProxy_, *symbols, *strategy_ );
    pCreationWnd->setWidget( creationPanels );
    pCreationWnd->setResizeEnabled( true );
    pCreationWnd->setCloseMode( QDockWindow::Always );
    pCreationWnd->setCaption( tr( "Creation" ) );
    setDockEnabled( pCreationWnd, Qt::DockTop, false );

    new MagicOrdersInterface( this, controllers_, publisher, staticModel_, *paramLayer, profile );
    new SIMControlToolbar( this, controllers, network, publisher );
    ReplayerToolbar* replayerToolbar = new ReplayerToolbar( this, controllers, publisher );
    new DisplayToolbar( this, controllers );
    new EventToolbar( this, controllers, profile );
    FolkToolbar* folkToolbar = new FolkToolbar( this, controllers, model.folk_ );

    // $$$$ AGE 2007-05-14: mettre un GetOrdersDirectory
    OrderBrowser* browser = new OrderBrowser( this, config.BuildExerciseChildFile( "orders" ).c_str() );
    
    // Drawer
    DrawerLayer* drawer = new DrawerLayer( *glProxy_ );
    new DrawerToolbar( this, *eventStrategy_, *drawer, *glProxy_, controllers_ );

    AfterAction* aar = new AfterAction( this, controllers_, *factory, model.aar_, publisher, *paramLayer, staticModel_ );

    new Menu( this, controllers, *prefDialog, *profileDialog, *browser, *factory, license );

    // $$$$ AGE 2006-08-22: prefDialog->GetPreferences()
    CreateLayers( *pMissionPanel_, *creationPanels, *paramLayer, *locationsLayer, *agentsLayer, *automatsLayer, *drawer, *prefDialog, profile, publisher );

    ::StatusBar* pStatus = new ::StatusBar( statusBar(), staticModel_.detection_, staticModel_.coordinateConverter_, controllers_ );
    connect( selector_, SIGNAL( MouseMove( const geometry::Point2f& ) ), pStatus, SLOT( OnMouseMove( const geometry::Point2f& ) ) );
    connect( selector_, SIGNAL( MouseMove( const geometry::Point3f& ) ), pStatus, SLOT( OnMouseMove( const geometry::Point3f& ) ) );
    controllers_.Register( *this );

    ReadSettings();
    ReadOptions();

    pMissionPanel_->hide();
    replayerToolbar->hide();
    folkToolbar->hide();
    aar->hide();

    new XPSPlayer( this, controllers_ );

    if( bfs::exists( bfs::path( config_.GetExerciseFile(), bfs::native ) ) )
        Load();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::CreateLayers
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void MainWindow::CreateLayers( MissionPanel& missions, CreationPanels& creationPanels, ParametersLayer& parameters, LocationsLayer& locationsLayer, gui::AgentsLayer& agents, gui::AutomatsLayer& automats, DrawerLayer& drawer, PreferencesDialog& preferences, const Profile_ABC& profile, Publisher_ABC& publisher )
{
    TooltipsLayer_ABC& tooltipLayer = *new TooltipsLayer( *glProxy_ );
    Layer_ABC& missionsLayer        = *new MiscLayer< MissionPanel >( missions );
    Layer_ABC& creationsLayer       = *new MiscLayer< CreationPanels >( creationPanels );
    Elevation2dLayer& elevation2d   = *new Elevation2dLayer( controllers_.controller_, staticModel_.detection_ );
    Layer_ABC& raster               = *new RasterLayer( controllers_.controller_ );
    Layer_ABC& terrain              = *new TerrainLayer( controllers_, *glProxy_, preferences.GetPreferences() );
    Layer_ABC& elevation3d          = *new Elevation3dLayer( controllers_.controller_, staticModel_.detection_, *lighting_ );
    Layer_ABC& grid                 = *new GridLayer( controllers_, *glProxy_ );
    Layer_ABC& metrics              = *new MetricsLayer( *glProxy_ );
    Layer_ABC& intelligences        = *new ::IntelligencesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, model_.intelligenceFactory_ );
    Layer_ABC& limits               = *new LimitsLayer( controllers_, *glProxy_, *strategy_, parameters, model_.tacticalLineFactory_, *glProxy_, profile );
    Layer_ABC& objectsLayer         = *new ::ObjectsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, publisher );
    Layer_ABC& populations          = *new ::PopulationsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    Layer_ABC& agentKnowledges      = *new AgentKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    Layer_ABC& populationKnowledges = *new PopulationKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    Layer_ABC& objectKnowledges     = *new ObjectKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    Layer_ABC& meteo                = *new MeteoLayer( controllers_, *glProxy_ );
    Layer_ABC& defaultLayer         = *new DefaultLayer( controllers_ );
    Layer_ABC& logoLayer            = *new LogoLayer( *glProxy_, QImage( "logo.png" ), 0.7f );
    Layer_ABC& formationLayer       = *new FormationLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    Layer_ABC& folkLayer            = *new ::FolkLayer( controllers_.controller_, staticModel_.coordinateConverter_, model_.folk_ );
    Layer_ABC& fogLayer             = *new FogLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );

    // ordre de dessin
    glProxy_->Register( defaultLayer );
    glProxy_->Register( elevation2d );              preferences.AddLayer( tr( "Elevation" ), elevation2d );         elevation2d         .SetPasses( "main,composition,miniviews" );
    glProxy_->Register( raster );                   preferences.AddLayer( tr( "Raster" ), raster );                 raster              .SetPasses( "main,composition,miniviews" );
    glProxy_->Register( terrain );                  preferences.AddLayer( tr( "Terrain" ), terrain );               terrain             .SetPasses( "main,composition,miniviews" );
    glProxy_->Register( elevation3d );
    glProxy_->Register( grid );                                                                                     grid                .SetPasses( "main,miniviews" );
    glProxy_->Register( folkLayer );                preferences.AddLayer( tr( "Folk" ), folkLayer );                folkLayer           .SetPasses( "main,miniviews" );
    glProxy_->Register( meteo );                                                                                    meteo               .SetPasses( "main,miniviews" );
    glProxy_->Register( limits );                                                                                   limits              .SetPasses( "main,miniviews" );
    glProxy_->Register( intelligences );            preferences.AddLayer( tr( "Intelligence" ), intelligences );    intelligences       .SetPasses( "main,miniviews" );
    glProxy_->Register( objectKnowledges );                                                                         objectKnowledges    .SetPasses( "main,miniviews" );
    glProxy_->Register( populationKnowledges );                                                                     populationKnowledges.SetPasses( "main,miniviews" );
    glProxy_->Register( agentKnowledges );                                                                          agentKnowledges     .SetPasses( "main,miniviews" );
    glProxy_->Register( formationLayer );                                                                           formationLayer      .SetPasses( "main,miniviews" );
    glProxy_->Register( objectsLayer );             preferences.AddLayer( tr( "Objects" ), objectsLayer );          objectsLayer        .SetPasses( "main,miniviews" );
    glProxy_->Register( populations );              preferences.AddLayer( tr( "Populations" ), populations );       populations         .SetPasses( "main,miniviews" );
    glProxy_->Register( agents );                   preferences.AddLayer( tr( "Units" ), agents );                  agents              .SetPasses( "main,miniviews" );
    glProxy_->Register( automats );                 preferences.AddLayer( tr( "Automats" ), automats );             automats            .SetPasses( "main,miniviews" );
    glProxy_->Register( missionsLayer );                                                                            missionsLayer       .SetPasses( "main,miniviews" );
    glProxy_->Register( creationsLayer );                                                                           creationsLayer      .SetPasses( "main" );
    glProxy_->Register( parameters );                                                                               parameters          .SetPasses( "main" );
    glProxy_->Register( metrics );                                                                                  metrics             .SetPasses( "main" );
    glProxy_->Register( locationsLayer );                                                                           locationsLayer      .SetPasses( "main" );
    glProxy_->Register( drawer );                                                                                   drawer              .SetPasses( "main,miniviews" );
    glProxy_->Register( fogLayer );                                                                                 fogLayer            .SetPasses( "fog" );
    glProxy_->Register( tooltipLayer );                                                                             tooltipLayer        .SetPasses( "tooltip" );
    glProxy_->Register( logoLayer );                preferences.AddLayer( tr( "Logo" ), logoLayer );                logoLayer           .SetPasses( "main" );
    
    // ordre des evenements
    forward_->Register( parameters );
    forward_->Register( agents );
    forward_->Register( automats );
    forward_->Register( populations );
    forward_->Register( objectsLayer );
    forward_->Register( intelligences );
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
    QString filename = QFileDialog::getOpenFileName( config_.GetExerciseFile().c_str(), "Exercise (*.xml)", this, 0, tr( "Load exercise definition file (exercise.xml)" ) );
    if( filename.isEmpty() )
        return;
    if( filename.startsWith( "//" ) )
        filename.replace( "/", "\\" );
    config_.LoadExercise( filename.ascii() );
    Load();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Load
// Created: AGE 2006-05-03
// -----------------------------------------------------------------------------
void MainWindow::Load()
{
    WriteOptions();
    model_.Purge();
    selector_->Load();
    staticModel_.Load( config_ );
    ReadOptions();
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
    staticModel_.Purge();
}

// -----------------------------------------------------------------------------
// Name: MainWindow destructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    controllers_.Unregister( *this );
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
// Created: SBO 2007-03-20
// -----------------------------------------------------------------------------
void MainWindow::NotifyUpdated( const Simulation& simulation )
{
    if( simulation.IsConnected() )
        setCaption( APP_NAME + QString( " - [%1@%2][%3]" )
                                      .arg( profile_ )
                                      .arg( simulation.GetSimulationHost().c_str() )
                                      .arg( ExtractExerciceName( "" ) ) ); //$$$$$ POURRI
    else
        setCaption( APP_NAME + tr( " - Not connected" ) );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::NotifyUpdated
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void MainWindow::NotifyUpdated( const Simulation::sConnection& connection )
{
    if( connection.connected_ )
        Load();
    else
    {
        controllers_.actions_.Select( SelectionStub() );
        Close();
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
        static LoginDialog* dialog = new LoginDialog( this, profile, network_.GetMessageMgr(), network_ );
        // $$$$ AGE 2006-10-11: exec would create a reentrance...
        QTimer::singleShot( 0, dialog, SLOT(exec()) );
    }
    else
        profile_ = profile.GetLogin();
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


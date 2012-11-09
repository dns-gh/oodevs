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
#include "ActionsLayer.h"
#include "AfterAction.h"
#include "AgentsLayer.h"
#include "AgentKnowledgesLayer.h"
#include "AutomatsLayer.h"
#include "ChatDock.h"
#include "ClientCommandFacade.h"
#include "ClockDock.h"
#include "CommandFacade.h"
#include "Config.h"
#include "CreationPanels.h"
#include "Dialogs.h"
#include "EventToolbar.h"
#include "ExtensionsPanel.h"
#include "FogLayer.h"
#include "FormationLayer.h"
#include "icons.h"
#include "IndicatorExportDialog.h"
#include "IndicatorPlotFactory.h"
#include "InfoDock.h"
#include "InfoPanels.h"
#include "LimitsLayer.h"
#include "LinkInterpreter.h"
#include "LoggerProxy.h"
#include "ConnectLoginDialog.h"
#include "MagicOrdersInterface.h"
#include "Menu.h"
#include "MessagePanel.h"
#include "MissionPanel.h"
#include "NotesPanel.h"
#include "ObjectKnowledgesLayer.h"
#include "OrbatDockWidget.h"
#include "ObjectsLayer.h"
#include "PopulationKnowledgesLayer.h"
#include "PopulationsLayer.h"
#include "ProfilingPanel.h"
#include "ProfilesPanel.h"
#include "InhabitantPanel.h"
#include "Properties.h"
#include "ReplayerToolbar.h"
#include "ResourceLinksDialog.h"
#include "ScorePanel.h"
#include "SIMControlToolbar.h"
#include "SimulationLighting.h"
#include "StatusBar.h"
#include "TeamLayer.h"
#include "TimelinePanel.h"
#include "UserProfileDialog.h"
#include "WeatherLayer.h"
#include "OrbatPanel.h"
#include "PlanificationModePanel.h"
#include "clients_kernel/ActionController.h"
#include "actions_gui/InterfaceBuilder.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/Tools.h"
#include "gaming/AgentServerMsgMgr.h"
#include "gaming/AgentsModel.h"
#include "gaming/ModeController.h"
#include "gaming/Model.h"
#include "gaming/Network.h"
#include "gaming/StaticModel.h"
#include "gaming/Profile.h"
#include "gaming/ProfileFilter.h"
#include "gaming/VisionConesToggler.h"
#include "gaming/ActionsScheduler.h"
#include "gaming/ColorController.h"
#include "clients_gui/AddRasterDialog.h"
#include "clients_gui/DisplayToolbar.h"
#include "clients_gui/DisplayExtractor.h"
#include "clients_gui/GlSelector.h"
#include "clients_gui/Logger.h"
#include "clients_gui/MiscLayer.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/Settings.h"
#include "clients_gui/InhabitantLayer.h"
#include "clients_gui/PreferencesDialog.h"
#include "clients_gui/RichItemFactory.h"
#include "clients_gui/MiniViews.h"
#include "clients_gui/GlProxy.h"
#include "clients_gui/ColorStrategy.h"
#include "clients_gui/SelectionColorModifier.h"
#include "clients_gui/HighlightColorModifier.h"
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
#include "clients_gui/UrbanLayer.h"
#include "clients_gui/LogoLayer.h"
#include "clients_gui/SymbolIcons.h"
#include "clients_gui/EntitySymbols.h"
#include "clients_gui/LocationEditorToolbar.h"
#include "clients_gui/LocationsLayer.h"
#include "clients_gui/TooltipsLayer.h"
#include "clients_gui/HelpSystem.h"
#include "clients_gui/GisToolbar.h"
#include "clients_gui/WatershedLayer.h"
#include "clients_gui/ContourLinesLayer.h"
#include "clients_gui/TerrainPicker.h"
#include "clients_gui/TerrainProfilerLayer.h"
#include "clients_gui/ElevationPainter.h"
#include "clients_gui/TerrainProfiler.h"
#include "geodata/ProjectionException.h"
#include "geodata/ExtentException.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/lexical_cast.hpp>
#pragma warning( pop )

namespace bfs = boost::filesystem;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::MainWindow( Controllers& controllers, ::StaticModel& staticModel, Model& model, const Simulation& simulation,
                        Network& network, const Profile_ABC& p, Config& config, LoggerProxy& logger, const QString& license )
    : QMainWindow()
    , controllers_     ( controllers )
    , staticModel_     ( staticModel )
    , model_           ( model )
    , network_         ( network )
    , config_          ( config )
    , forward_         ( new gui::CircularEventStrategy() )
    , eventStrategy_   ( new gui::ExclusiveEventStrategy( *forward_ ) )
    , pPainter_        ( new gui::ElevationPainter( staticModel_.detection_ ) )
    , pColorController_( new ColorController( controllers_ ) )
    , glProxy_         ( 0 )
    , connected_       ( false )
    , onPlanif_        ( false )
    , pProfile_        ( new ProfileFilter( controllers, p ) )
{
    controllers_.modes_->SetMainWindow( this );
    QSettings settings( "MASA Group", "SWORD" );
    settings.beginGroup( "/Gaming" );
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());

    setAttribute( Qt::WA_DeleteOnClose, true );
    setIcon( QPixmap( tools::GeneralConfig::BuildResourceChildFile( "images/gui/logo32x32.png" ).c_str() ) );
    planifName_ = tools::translate( "Application", "SWORD" ) + tr( " - Not connected" );
    setCaption( planifName_ );

    Publisher_ABC& publisher = network_.GetMessageMgr();

    glProxy_ = new gui::GlProxy();
    strategy_ = new gui::ColorStrategy( controllers, *glProxy_, *pColorController_ );
    strategy_->Add( std::auto_ptr< gui::ColorModifier_ABC >( new gui::SelectionColorModifier( controllers, *glProxy_ ) ) );
    strategy_->Add( std::auto_ptr< gui::ColorModifier_ABC >( new gui::HighlightColorModifier( controllers ) ) );

    QStackedWidget* centralWidget = new QStackedWidget();
    setCentralWidget( centralWidget );
    selector_ = new gui::GlSelector( centralWidget, *glProxy_, controllers, config, staticModel.detection_, *eventStrategy_ );
    selector_->AddIcon( xpm_cadenas        , -260, 360 );
    selector_->AddIcon( xpm_radars_on      ,  200, 270 );
    selector_->AddIcon( xpm_brouillage     ,  200, 50 );
    selector_->AddIcon( xpm_silence_radio_incoming,  100, 50 );
    selector_->AddIcon( xpm_silence_radio_outgoing,  100, 50 );
    selector_->AddIcon( xpm_gas            , -280, 160 );
    selector_->AddIcon( xpm_ammo           , -200, 100 );
    selector_->AddIcon( xpm_nbc            , -200, 25 );
    selector_->AddIcon( xpm_underground    , -200, 50 );
    selector_->AddIcon( xpm_construction   ,  200, 150 );
    selector_->AddIcon( xpm_observe        ,  200, 150 );

    lighting_ = new SimulationLighting( controllers, this );
    gui::RichItemFactory* factory = new  gui::RichItemFactory( this ); // $$$$ AGE 2006-05-11: aggregate somewhere
    preferenceDialog_.reset( new gui::PreferencesDialog( this, controllers, *lighting_, staticModel.coordinateSystems_, *pPainter_, *selector_ ) );
    preferenceDialog_->AddPage( tr( "Orbat" ), *new OrbatPanel( preferenceDialog_.get(), controllers ) );
    new VisionConesToggler( controllers, publisher, this );

    gui::DisplayExtractor* displayExtractor = new gui::DisplayExtractor( this );

    LinkInterpreter* interpreter = new LinkInterpreter( this, controllers, *pProfile_ );
    connect( factory, SIGNAL( LinkClicked( const QString& ) ), interpreter, SLOT( Interprete( const QString& ) ) );
    connect( displayExtractor, SIGNAL( LinkClicked( const QString& ) ), interpreter, SLOT( Interprete( const QString& ) ) );

    // Logger
    QDockWidget* pLogDockWnd_ = new QDockWidget( "log", this );
    pLogDockWnd_->setObjectName( "log" );
    addDockWidget( Qt::BottomDockWidgetArea, pLogDockWnd_ );
    gui::Logger* pLogPanel = new gui::Logger( pLogDockWnd_, simulation, "./Debug/Gaming.log" );
    pLogDockWnd_->setWidget( pLogPanel );
    pLogDockWnd_->setWindowTitle( tr( "Log" ) );
    pLogDockWnd_->hide();
    logger.SetLogger( *pLogPanel );

    // Standard toolbars
    gui::TerrainProfilerLayer* profilerLayer = new gui::TerrainProfilerLayer( *glProxy_ );
    gui::GisToolbar* gToolBar = new gui::GisToolbar( this, controllers, staticModel_.detection_ );
    EventToolbar* eToolBar = new EventToolbar( this, controllers, *pProfile_ );
    gui::DisplayToolbar* dToolBar = new gui::DisplayToolbar( this, controllers );
    SIMControlToolbar* sToolBar = new SIMControlToolbar( this, controllers, network, publisher, *pLogPanel );

    addToolBar( sToolBar );
    addToolBar( dToolBar );
    addToolBar( eToolBar );
    addToolBar( gToolBar );

    // Terrain profile
    gui::TerrainProfiler* terrainProfiler = new gui::TerrainProfiler( this, controllers, staticModel.detection_, *profilerLayer );
    addDockWidget( Qt::RightDockWidgetArea, terrainProfiler );
    terrainProfiler->hide();
    connect( gToolBar->GetTerrainProfilerButton(), SIGNAL( toggled( bool ) ), terrainProfiler, SLOT( setVisible( bool ) ) );
    connect( terrainProfiler, SIGNAL( visibilityChanged( bool ) ), gToolBar->GetTerrainProfilerButton(), SLOT( setOn( bool ) ) );

    // A few layers
    gui::LocationsLayer* locationsLayer = new gui::LocationsLayer( *glProxy_ );
    gui::LocationEditorToolbar* LocEditToolBar = new gui::LocationEditorToolbar( this, controllers_, staticModel.coordinateConverter_, *glProxy_, *locationsLayer );
    addToolBar( LocEditToolBar );
    parameters_ = new gui::ParametersLayer( *glProxy_ );
    ::AgentsLayer* agentsLayer = new ::AgentsLayer( controllers, *glProxy_, *strategy_, *glProxy_, *pProfile_ );
    ::AutomatsLayer* automatsLayer = new ::AutomatsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, *pProfile_, model_.actions_, simulation, network_.GetMessageMgr(), model.agents_ );
    ::FormationLayer* formationLayer = new ::FormationLayer( controllers_, *glProxy_, *strategy_, *glProxy_, *pProfile_, model_.actions_, staticModel_, simulation, network_.GetMessageMgr(), model_.agents_ );

    addToolBarBreak();

    //Dialogs
    new Dialogs( this, controllers, model_, staticModel, publisher, model_.actions_, simulation, *pProfile_, network.GetCommands(), config, *parameters_ );
    addRasterDialog_.reset( new gui::AddRasterDialog( this ) );

    // Profile
    gui::SymbolIcons* symbols = new gui::SymbolIcons( this, *glProxy_ );
    connect( selector_, SIGNAL( Widget2dChanged( gui::GlWidget* ) ), symbols, SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );
    gui::EntitySymbols* icons = new gui::EntitySymbols( *symbols, *strategy_ );
    UserProfileDialog* profileDialog = new UserProfileDialog( this, controllers, *pProfile_, *icons, model_.userProfileFactory_ );

    // Agent list panel
    orbatDockWidget_ = new OrbatDockWidget( controllers_, this, "agentList", tr( "Orbat" ), *pProfile_, *automatsLayer, *formationLayer, model_.actions_, staticModel, simulation, *icons );
    addDockWidget( Qt::LeftDockWidgetArea, orbatDockWidget_ );

    // Mini views
    gui::MiniViews* miniviews = new gui::MiniViews( this, controllers_ );
    addDockWidget( Qt::RightDockWidgetArea, miniviews );
    connect( selector_, SIGNAL( Widget2dChanged( gui::GlWidget* ) ), miniviews, SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );
    miniviews->hide();

    // Properties
    QDockWidget* pPropertiesWnd = new QDockWidget( "properties", this );
    pPropertiesWnd->setObjectName( "properties" );
    Properties* properties = new Properties( pPropertiesWnd, controllers, *glProxy_ );
    pPropertiesWnd->setWidget( properties );
    pPropertiesWnd->setWindowTitle( tr( "Properties" ) );
    addDockWidget( Qt::LeftDockWidgetArea, pPropertiesWnd );
    pPropertiesWnd->hide();

    // Info panel
    QDockWidget* pInfoDockWnd_ = new QDockWidget( "oldinfo", this );
    pInfoDockWnd_->setObjectName( "oldInfo" );
    addDockWidget( Qt::RightDockWidgetArea, pInfoDockWnd_ );
    InfoPanels* pInfoPanel_ = new InfoPanels( pInfoDockWnd_, controllers, *factory, *displayExtractor );
    pInfoDockWnd_->setWidget( pInfoPanel_ );
    pInfoDockWnd_->setWindowTitle( tr( "Knowledge" ) );
    pInfoDockWnd_->hide();

    // Interface Builder
    interfaceBuilder_.reset( new actions::gui::InterfaceBuilder( controllers_, *parameters_, staticModel_, &model_.agentKnowledgeConverter_, &model_.objectKnowledgeConverter_, &simulation ) );
    // Mission panel
    pMissionPanel_ = new MissionPanel( this, controllers_, staticModel_, publisher, *glProxy_, *pProfile_, model_.actions_, simulation, *interfaceBuilder_, config_ );
    addDockWidget( Qt::LeftDockWidgetArea, pMissionPanel_ );
    pMissionPanel_->setProperty( "notAppropriate", QVariant( true ) );
    pMissionPanel_->hide();

    // Chat
    QDockWidget* chatDock = new ChatDock( this, controllers_, publisher, network.GetCommands() );
    addDockWidget( Qt::BottomDockWidgetArea, chatDock );
    chatDock->hide();

    new CommandFacade( this, controllers_, config, network.GetCommands(), *interpreter, *glProxy_, *pProfile_ );
    new ClientCommandFacade( this, controllers_, publisher );

    // Info
    QDockWidget* infoWnd = new InfoDock( this, controllers_, p, *icons, *factory, *displayExtractor, staticModel_, model_.actions_, simulation ); // $$$$ ABR 2011-08-09: p or profile ???
    addDockWidget( Qt::BottomDockWidgetArea, infoWnd );

    // Clock
    ActionsScheduler* scheduler = new ActionsScheduler( this, controllers_, simulation, model_.actions_, publisher );
    QDockWidget* clockWnd = new ClockDock( this, controllers_, simulation, *scheduler );
    addDockWidget( Qt::RightDockWidgetArea, clockWnd );

    // Profiler
    QDockWidget* pProfilerDockWnd_ = new QDockWidget( "profiler", this );
    pProfilerDockWnd_->setObjectName( "profiler" );
    addDockWidget( Qt::RightDockWidgetArea, pProfilerDockWnd_ );
    ProfilingPanel* profilingPanel_ = new ProfilingPanel( pProfilerDockWnd_, controllers_, network_, simulation );
    pProfilerDockWnd_->setWidget( profilingPanel_ );
    pProfilerDockWnd_->setWindowTitle( tr( "Profiling" ) );
    pProfilerDockWnd_->setProperty( "notAppropriate", QVariant( true ) );
    pProfilerDockWnd_->hide();

    // Layers
    gui::TerrainPicker* picker = new gui::TerrainPicker( this );
    gui::TerrainLayer* terrainLayer = new gui::TerrainLayer( controllers_, *glProxy_, preferenceDialog_->GetPreferences(), *picker );
    WeatherLayer* meteoLayer = new WeatherLayer( *glProxy_, *eventStrategy_, controllers_, model_.meteo_, *picker );

    // object/unit creation window
    QDockWidget* pCreationWnd = new QDockWidget( "creation", this );
    pCreationWnd->setObjectName( "creation" );
    addDockWidget( Qt::RightDockWidgetArea, pCreationWnd );
    pCreationWnd->hide();
    CreationPanels* creationPanels = new CreationPanels( pCreationWnd, controllers, staticModel_, model_, simulation, *parameters_, *meteoLayer, *glProxy_, *symbols, *strategy_, config_ );
    pCreationWnd->setWidget( creationPanels );
    pCreationWnd->setWindowTitle( tr( "Creation" ) );

    new MagicOrdersInterface( this, controllers_, model_.actions_, staticModel_, simulation, *parameters_, *pProfile_, *selector_ );
    ReplayerToolbar* replayerToolbar = new ReplayerToolbar( this, controllers, publisher );
    addToolBar( replayerToolbar );
    IndicatorExportDialog* indicatorExportDialog = new IndicatorExportDialog( this );
    IndicatorPlotFactory* plotFactory = new IndicatorPlotFactory( this, controllers_, publisher, *indicatorExportDialog, simulation );
    AfterAction* aar = new AfterAction( this, controllers_, model.aar_, *plotFactory, *interfaceBuilder_ );

    // Actions panel
    {
        TimelinePanel* timelinePanel = new TimelinePanel( this, controllers_, model_.actions_, *scheduler, config_, *pProfile_, *displayExtractor );
        addDockWidget( Qt::TopDockWidgetArea, timelinePanel );
        timelinePanel->hide();
    }
    // Profiles panel
    {
        ProfilesPanel* profilesPanel = new ProfilesPanel( this, controllers_, network_, *pProfile_, model_.teams_ );
        addDockWidget( Qt::RightDockWidgetArea, profilesPanel );
        profilesPanel->hide();
    }
    // Inhabitant panel
    {
        InhabitantPanel* inhabitantPanel = new InhabitantPanel( this, controllers_, model_ );
        addDockWidget( Qt::RightDockWidgetArea, inhabitantPanel );
        inhabitantPanel->hide();
    }
    // Score panel
    {
        ScorePanel* scorePanel = new ScorePanel( this, controllers_, *displayExtractor, *interpreter, *plotFactory, *indicatorExportDialog, model_.scores_, config );
        addDockWidget( Qt::RightDockWidgetArea, scorePanel );
        scorePanel->hide();
    }
    // Notes panel
    {
        NotesPanel* notePanel = new NotesPanel( this, controllers_.controller_, model_.notes_, publisher );
        addDockWidget( Qt::RightDockWidgetArea, notePanel );
    }
    // Message panel
    {
        QToolBar* messagePanel = new MessagePanel( this, controllers_, publisher, network.GetCommands(), *factory );
        addToolBar( messagePanel );
    }
    // ResourceNetwork panel
    {
        QDockWidget* pResourceWnd = new ResourceLinksDialog( this, controllers, model_.actions_, staticModel, simulation );
        addDockWidget( Qt::LeftDockWidgetArea, pResourceWnd );
        pResourceWnd->hide();
    }
    // Extensions panel
    {
        pExtensionsPanel_ = new ExtensionsPanel( this, controllers, model, staticModel_, simulation );
        addDockWidget( Qt::LeftDockWidgetArea, pExtensionsPanel_ );
        pExtensionsPanel_->hide();
    }
    // Planification mode panel
    {
        pPlanificationModePanel_ = new PlanificationModePanel( this, controllers );
        pPlanificationModePanel_->SetModes( eGamingMode_Exercise | eGamingMode_Default, eGamingMode_Planification );
        addDockWidget( Qt::TopDockWidgetArea, pPlanificationModePanel_ );
        pPlanificationModePanel_->hide();
    }

    help_ = new gui::HelpSystem( this, config_.BuildResourceChildFile( "help/gaming.xml" ) );
    setMenuBar( new Menu( this, controllers, staticModel_, *preferenceDialog_, *profileDialog, *factory, license, *help_, *interpreter, network_, logger ) );

    CreateLayers( *pMissionPanel_, *creationPanels, *parameters_, *locationsLayer, *agentsLayer, *automatsLayer, *formationLayer, *terrainLayer, *meteoLayer, *profilerLayer, *preferenceDialog_, *pProfile_, simulation, *picker );
    ::StatusBar* pStatus_ = new ::StatusBar( statusBar(), *picker, staticModel_.detection_, staticModel_.coordinateConverter_, controllers_, pProfilerDockWnd_ );
    connect( selector_, SIGNAL( MouseMove( const geometry::Point2f& ) ), pStatus_, SLOT( OnMouseMove( const geometry::Point2f& ) ) );
    connect( selector_, SIGNAL( MouseMove( const geometry::Point3f& ) ), pStatus_, SLOT( OnMouseMove( const geometry::Point3f& ) ) );
    controllers_.Register( *this );

//    ReadSettings();
    ReadOptions();

    pMissionPanel_->hide();
    replayerToolbar->hide();
    aar->SetStartup();

    restoreState( settings.value( "mainWindowState" ).toByteArray() );

    controllers_.ChangeMode( eGamingMode_Default );

    // Raster_app process
    process_.reset( new QProcess( this ) );
    connect( process_.get(), SIGNAL( finished( int, QProcess::ExitStatus ) ), this, SLOT( OnRasterProcessExited( int, QProcess::ExitStatus ) ) );
}

// -----------------------------------------------------------------------------
// Name: MainWindow destructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    process_->kill();
    controllers_.Unregister( *this );
    delete pMissionPanel_;
    delete parameters_;
    delete selector_;
}

namespace
{
    template< typename Layer >
    void AddLayer( gui::GlProxy& glProxy, gui::PreferencesDialog& preference, Layer& layer, const std::string& passes = "", const QString& text = "" )
    {
        glProxy.Register( layer );
        if( !text.isEmpty() )
            preference.AddLayer( text, layer );
        if( !passes.empty() )
            layer.SetPasses( passes );
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::CreateLayers
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void MainWindow::CreateLayers( MissionPanel& missions, CreationPanels& creationPanels, gui::ParametersLayer& parameters, gui::Layer_ABC& locationsLayer,
                               gui::AgentsLayer& agents, gui::AutomatsLayer& automats, gui::FormationLayer& formationLayer, gui::TerrainLayer& terrain, gui::Layer_ABC& weather, gui::Layer_ABC& profilerLayer,
                               gui::PreferencesDialog& preferences, const Profile_ABC& profile, const Simulation& simulation, gui::TerrainPicker& picker )
{
    gui::TooltipsLayer_ABC& tooltipLayer = *new gui::TooltipsLayer( *glProxy_ );
    gui::Layer_ABC& missionsLayer        = *new gui::MiscLayer< MissionPanel >( missions );
    gui::Layer_ABC& creationsLayer       = *new gui::MiscLayer< CreationPanels >( creationPanels );
    gui::Elevation2dLayer& elevation2d   = *new gui::Elevation2dLayer( controllers_.controller_, staticModel_.detection_ );
    gui::Layer_ABC& raster               = *new gui::RasterLayer( controllers_.controller_ );
    gui::Layer_ABC& watershed            = *new gui::WatershedLayer( controllers_, staticModel_.detection_ );
    gui::Layer_ABC& elevation3d          = *new gui::Elevation3dLayer( controllers_.controller_, staticModel_.detection_, *lighting_ );
    gui::Layer_ABC& urbanLayer           = *new gui::UrbanLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    gui::Layer_ABC& grid                 = *new gui::GridLayer( controllers_, *glProxy_ );
    gui::Layer_ABC& metrics              = *new gui::MetricsLayer( staticModel_.detection_, *glProxy_ );
    gui::Layer_ABC& limits               = *new LimitsLayer( controllers_, *glProxy_, *strategy_, parameters, model_.tacticalLineFactory_, *glProxy_, profile );
    gui::Layer_ABC& objectsLayer         = *new ::ObjectsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, model_.actions_, staticModel_, simulation, picker );
    gui::Layer_ABC& populations          = *new ::PopulationsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    gui::Layer_ABC& inhabitants          = *new gui::InhabitantLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    gui::Layer_ABC& agentKnowledges      = *new AgentKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    gui::Layer_ABC& populationKnowledges = *new PopulationKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    gui::Layer_ABC& objectKnowledges     = *new ObjectKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    gui::Layer_ABC& defaultLayer         = *new gui::DefaultLayer( controllers_ );
    gui::Layer_ABC& logoLayer            = *new gui::LogoLayer( *glProxy_, QImage( config_.BuildResourceChildFile( "logo.png" ).c_str() ), 0.7f );
    gui::Layer_ABC& teamLayer            = *new ::TeamLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, model_.actions_, staticModel_, simulation, network_.GetMessageMgr() );
    gui::Layer_ABC& fogLayer             = *new FogLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    gui::Layer_ABC& drawerLayer          = *new gui::DrawerLayer( controllers_, *glProxy_, *strategy_, parameters, *glProxy_, profile );
    gui::Layer_ABC& actionsLayer         = *new ActionsLayer( controllers_, *glProxy_ );
    gui::Layer_ABC& contour              = *new gui::ContourLinesLayer( controllers_, staticModel_.detection_ );

    // ordre de dessin
    AddLayer( *glProxy_, preferences, defaultLayer );
    AddLayer( *glProxy_, preferences, elevation2d, "main,composition,miniviews", tr( "Elevation" ) );
    AddLayer( *glProxy_, preferences, raster, "main,composition,miniviews", tr( "Raster" ) );
    AddLayer( *glProxy_, preferences, terrain, "main,composition,miniviews", tr( "Terrain" ) );
    AddLayer( *glProxy_, preferences, contour, "main,composition,miniviews", tr( "Contour Lines" ) );
    AddLayer( *glProxy_, preferences, urbanLayer, "main,miniviews" );
    AddLayer( *glProxy_, preferences, watershed, "main,composition,miniviews", tr( "Watershed" ) );
    glProxy_->Register( elevation3d );
    AddLayer( *glProxy_, preferences, grid, "main,miniviews" );
    AddLayer( *glProxy_, preferences, weather, "main,miniviews" );
    AddLayer( *glProxy_, preferences, limits, "main,miniviews" );
    AddLayer( *glProxy_, preferences, objectKnowledges, "main,miniviews" );
    AddLayer( *glProxy_, preferences, populationKnowledges, "main,miniviews" );
    AddLayer( *glProxy_, preferences, agentKnowledges, "main,miniviews" );
    AddLayer( *glProxy_, preferences, formationLayer, "main,miniviews", tr( "Formations" ) );
    AddLayer( *glProxy_, preferences, teamLayer, "main,miniviews" );
    AddLayer( *glProxy_, preferences, objectsLayer, "main,miniviews", tr( "Objects" ) );
    AddLayer( *glProxy_, preferences, populations, "main,miniviews", tr( "Crowds" ) );
    AddLayer( *glProxy_, preferences, inhabitants, "main,miniviews", tr( "Populations" ) );
    AddLayer( *glProxy_, preferences, agents, "main,miniviews", tr( "Units" ) );
    AddLayer( *glProxy_, preferences, automats, "main,miniviews", tr( "Automats" ) );
    AddLayer( *glProxy_, preferences, missionsLayer, "main,miniviews" );
    AddLayer( *glProxy_, preferences, actionsLayer, "main" );
    AddLayer( *glProxy_, preferences, creationsLayer, "main" );
    AddLayer( *glProxy_, preferences, parameters, "main" );
    AddLayer( *glProxy_, preferences, metrics, "main" );
    AddLayer( *glProxy_, preferences, locationsLayer, "main" );
    AddLayer( *glProxy_, preferences, profilerLayer, "main" );
    AddLayer( *glProxy_, preferences, drawerLayer, "main,miniviews" );
    AddLayer( *glProxy_, preferences, fogLayer, "fog" );
    AddLayer( *glProxy_, preferences, tooltipLayer, "tooltip" );
    AddLayer( *glProxy_, preferences, logoLayer, "main", tr( "Logo" ) );

    // ordre des evenements
    forward_->Register( terrain );
    forward_->Register( parameters );
    forward_->Register( agents );
    forward_->Register( automats );
    forward_->Register( formationLayer );
    forward_->Register( teamLayer );
    forward_->Register( populations );
    forward_->Register( inhabitants );
    forward_->Register( objectsLayer );
    forward_->Register( agentKnowledges );
    forward_->Register( populationKnowledges );
    forward_->Register( objectKnowledges );
    forward_->Register( limits );
    forward_->Register( urbanLayer );
    forward_->Register( drawerLayer );
    forward_->Register( metrics );
    forward_->Register( elevation3d );
    forward_->Register( weather );
    forward_->SetDefault( defaultLayer );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::OnPlanifStateChange
// Created: HBD 2010-09-06
// -----------------------------------------------------------------------------
void MainWindow::OnPlanifStateChange()
{
    pMissionPanel_->ActivatePlanification();
    onPlanif_ = !onPlanif_;
    if( onPlanif_ )
        setCaption( planifName_ + tools::translate( "Application", " - Planning mode on" ) );
    else
        setCaption( planifName_ );
}

// -----------------------------------------------------------------------------

// Name: MainWindow::Load
// Created: AGE 2006-05-03
// -----------------------------------------------------------------------------
void MainWindow::Load()
{
    try
    {
        WriteOptions();
        orbatDockWidget_->Purge();
        model_.Purge();
        selector_->Close();
        selector_->Load();
        staticModel_.Load( config_ );
        pExtensionsPanel_->hide();
        ReadOptions();
        controllers_.ChangeMode( eGamingMode_Exercise );
    }
    catch( xml::exception& e )
    {
        Close();
        QMessageBox::critical( this, tools::translate( "Application", "SWORD" )
                                   , ( tools::translate( "MainWindow", "Error loading exercise: " ) + e.what() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::ShowHelp
// Created: JSR 2012-03-27
// -----------------------------------------------------------------------------
void MainWindow::ShowHelp()
{
    help_->ShowHelp();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Close
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void MainWindow::Close()
{
    controllers_.ChangeMode( eGamingMode_Default );
    network_.Disconnect();
    parameters_->Reset();
    selector_->Close();
    orbatDockWidget_->Purge();
    model_.Purge();
    staticModel_.Purge();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::closeEvent
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::closeEvent( QCloseEvent* pEvent )
{
    QSettings settings( "MASA Group", "SWORD" );
    settings.beginGroup( "/Gaming" );
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());

    WriteOptions();

    Close();
    QMainWindow::closeEvent( pEvent );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::WriteOptions
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::WriteOptions()
{
    gui::Settings settings( "MASA Group", "SWORD" );
    settings.beginGroup( "/Gaming/Options" );
    controllers_.options_.Save( settings );
    settings.endGroup();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::ReadOptions
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::ReadOptions()
{
    gui::Settings settings( "MASA Group", "SWORD" );
    settings.beginGroup( "/Gaming/Options" );
    controllers_.options_.Load( settings );
    settings.endGroup();
}

namespace
{
    struct SelectionStub{};

    QString ExtractExerciceName( const std::string& filename )
    {
        if( !bfs::exists( filename ) )
            return "";

        std::string name;
        xml::xifstream xis( filename );
        xis >> xml::start( "exercise" )
                >> xml::optional >> xml::start( "meta" );
        if( xis.has_child( "name" ) )
            xis >> xml::content( "name", name );
        return ( !name.empty() ) ? name.c_str() : bfs::path( filename ).parent_path().filename().string().c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::NotifyUpdated
// Created: SBO 2007-03-20
// -----------------------------------------------------------------------------
void MainWindow::NotifyUpdated( const Simulation& simulation )
{
    const QString appName = tools::translate( "Application", "SWORD" );
    const QString modePlanif =  tools::translate( "Application", " - Planning mode on" );
    if( simulation.IsConnected() )
    {
        if( !connected_ && simulation.IsInitialized() )
        {
            connected_ = true; // we update the caption until Model is totally loaded
            if( profile_.isEmpty() )
                profile_ = tools::translate( "LoginDialog", "Anonymous" );
        }
        QString planifName = appName + QString( " - [%1@%2][%3]" )
                            .arg( profile_ )
                            .arg( simulation.GetSimulationHost().c_str() )
                            .arg( ExtractExerciceName( config_.GetExerciseFile() ));
        if( planifName_ != planifName )
        {
            setCaption( onPlanif_ ? planifName + modePlanif : planifName );
            planifName_ = planifName;
        }
    }
    else
    {
        planifName_ = appName + tr( " - Not connected" ) ;
        if( onPlanif_ )
            setCaption( planifName_ + modePlanif );
        else
            setCaption( planifName_ );
        controllers_.actions_.Select( SelectionStub() );
        connected_ = false;
        profile_ = "";
        Close();
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::NotifyUpdated
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void MainWindow::NotifyUpdated( const Services& /*services*/ )
{
    Load();
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
        static ConnectLoginDialog* dialog = new ConnectLoginDialog( this, profile, network_, controllers_ );
        // $$$$ AGE 2006-10-11: exec would create a reentrance...
        QTimer::singleShot( 0, dialog, SLOT( show() ) );
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

// -----------------------------------------------------------------------------
// Name: MainWindow::ToggleFullScreen
// Created: SBO 2011-04-05
// -----------------------------------------------------------------------------
void MainWindow::ToggleFullScreen()
{
    if( isFullScreen() )
        showNormal();
    else
        showFullScreen();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::ToggleDocks
// Created: SBO 2011-04-08
// -----------------------------------------------------------------------------
void MainWindow::ToggleDocks()
{
    if( docks_.isNull() || docks_.isEmpty() && toolbars_.isNull() || toolbars_.isEmpty())
    {
        docks_ = saveState();
        toolbars_ = saveState();
        QList< QToolBar* > toolbars = qFindChildren< QToolBar* >( this , QString() );
        for( QList< QToolBar* >::iterator it = toolbars.begin(); it != toolbars.end(); ++it )
            (*it)->hide();
        QList< QDockWidget* > docks = qFindChildren< QDockWidget* >( this , QString() );
        for( QList< QDockWidget* >::iterator it = docks.begin(); it != docks.end(); ++it )
            (*it)->hide();
    }
    else
    {
        if( restoreState( docks_ ) )
            docks_ = 0;
        if( restoreState( toolbars_ ) )
            toolbars_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::createPopupMenu
// Created: LDC 2011-08-26
// -----------------------------------------------------------------------------
QMenu* MainWindow::createPopupMenu()
{
    QMenu* menu = new QMenu( this );
    QList<QDockWidget *> dockwidgets = qFindChildren<QDockWidget *>( this );
    for( QList<QDockWidget *>::iterator it = dockwidgets.begin(); it != dockwidgets.end(); ++it )
    {
        QDockWidget* dockWidget = *it;
        if( !dockWidget->property( "notAppropriate" ).isValid() )
            menu->addAction( dockWidget->toggleViewAction() );
    }
    menu->addSeparator();
    QList<QToolBar *> toolbars = qFindChildren<QToolBar *>(this);
    for( QList<QToolBar *>::iterator it = toolbars.begin(); it != toolbars.end(); ++it )
    {
        QToolBar* toolbar = *it;
        menu->addAction( toolbar->toggleViewAction() );
    }
    return menu;
}

// -----------------------------------------------------------------------------
// Name: MainWindow::OnAddRaster
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
void MainWindow::OnAddRaster()
{
    try
    {
        if( !bfs::exists( config_.BuildTerrainChildFile( "config.xml" ) ) )
        {
            QMessageBox::warning( 0, tr( "Warning" ), tr( "This functionality is not available with old terrain format." ) );
            return;
        }

        QDialog::DialogCode result = static_cast< QDialog::DialogCode >( addRasterDialog_->exec() );
        if( result == QDialog::Accepted )
        {
            QStringList parameters;
            parameters << ( std::string( "--config=" ) + bfs::system_complete( config_.BuildTerrainChildFile( "config.xml" ) ).string() ).c_str();
            parameters << ( std::string( "--raster=" ) + addRasterDialog_->GetFiles().toStdString() ).c_str();
            parameters << ( std::string( "--pixelsize=" ) + boost::lexical_cast< std::string >( addRasterDialog_->GetPixelSize() ) ).c_str();
            bfs::path filename = bfs::system_complete( bfs::path( config_.GetGraphicsDirectory() ) / "~~tmp.texture.bin" );
            parameters << ( std::string( "--file=" ) + filename.string() ).c_str();
            bfs::path workingDirectory = bfs::system_complete( "../Terrain/applications/" );
            process_->setWorkingDirectory( workingDirectory.string().c_str() );
            process_->start( bfs::path( workingDirectory / "raster_app.exe" ).string().c_str(), parameters );
        }
    }
    catch( geodata::ProjectionException& )
    {
        QMessageBox::information( this, tr( "Error loading image file" ), tr( "The following raster you add is missing spatial reference information.\nThis data can't be projected.") ) ;
        // Created: AME 2010-09-16 : TODO->allow user to set the projection in UI
    }
    catch( geodata::ExtentException& )
    {
        QMessageBox::information( this, tr( "Error loading image file" ), tr( "The following raster you add is missing extent information.") ) ;
    }
    catch( ... )
    {
        QMessageBox::critical( this, tr( "Error loading image file" ), tr( "Fatal error adding Raster source." ), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton );
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::OnRasterProcessExited
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
void MainWindow::OnRasterProcessExited( int exitCode, QProcess::ExitStatus exitStatus )
{
    if( exitStatus == QProcess::NormalExit && exitCode == EXIT_SUCCESS )
    {
        gui::RasterLayer& raster = *new gui::RasterLayer( controllers_.controller_, "~~tmp.texture.bin" );
        raster.SetPasses( "main" );
        selector_->AddLayer( raster );
        preferenceDialog_->AddLayer( tr( "User layer [%1]" ).arg( addRasterDialog_->GetName() ), raster, true );
        raster.NotifyUpdated( kernel::ModelLoaded( config_ ) );
        raster.GenerateTexture();
        try
        {
            const bfs::path aggregated = bfs::path( config_.GetGraphicsDirectory() ) / "~~tmp.texture.bin";
            if( bfs::exists( aggregated ) )
                bfs::remove( aggregated );
        }
        catch( ... )
        {
            // NOTHING
        }
    }
    else
        QMessageBox::warning( this, tr( "Error loading image file" ), tr( "Error while loading Raster source." ) );
}

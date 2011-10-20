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
#include "AgentList.h"
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
#include "LoginDialog.h"
#include "LogisticListView.h"
#include "MagicOrdersInterface.h"
#include "Menu.h"
#include "MessagePanel.h"
#include "MissionPanel.h"
#include "NotesPanel.h"
#include "ObjectKnowledgesLayer.h"
#include "OrbatToolbar.h"
#include "ObjectsLayer.h"
#include "PopulationKnowledgesLayer.h"
#include "PopulationsLayer.h"
#include "ProfilingPanel.h"
#include "Properties.h"
#include "ReplayerToolbar.h"
#include "ResourceLinksDialog.h"
#include "ScorePanel.h"
#include "SIMControlToolbar.h"
#include "SimulationLighting.h"
#include "StatusBar.h"
#include "TacticalList.h"
#include "TeamLayer.h"
#include "TimelinePanel.h"
#include "UserProfileDialog.h"
#include "WeatherLayer.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Team_ABC.h"
#include "gaming/AgentServerMsgMgr.h"
#include "gaming/AgentsModel.h"
#include "gaming/Model.h"
#include "gaming/Network.h"
#include "gaming/Population.h"
#include "gaming/StaticModel.h"
#include "gaming/Profile.h"
#include "gaming/ProfileFilter.h"
#include "gaming/VisionConesToggler.h"
#include "gaming/ActionsScheduler.h"
#include "gaming/Tools.h"
#include "gaming/ColorController.h"
#include "clients_gui/DisplayToolbar.h"
#include "clients_gui/GlSelector.h"
#include "clients_gui/GraphicPreferences.h"
#include "clients_gui/Logger.h"
#include "clients_gui/MiscLayer.h"
#include "clients_gui/ObjectList.h"
#include "clients_gui/OptionsPanel.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/Settings.h"
#include "clients_gui/PopulationList.h"
#include "clients_gui/InhabitantList.h"
#include "clients_gui/InhabitantLayer.h"
#include "clients_gui/PreferencesDialog.h"
#include "clients_gui/RichItemFactory.h"
#include "clients_gui/MiniViews.h"
#include "clients_gui/resources.h"
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
#include "clients_gui/SimpleFilter.h"
#include "clients_gui/UrbanFilter.h"
#include "clients_gui/LogisticList.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )

namespace bfs = boost::filesystem;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::MainWindow( Controllers& controllers, ::StaticModel& staticModel, Model& model, const Simulation& simulation,
                        Network& network, const Profile_ABC& p, Config& config, LoggerProxy& logger,
                        const RcEntityResolver_ABC& rcResolver, const QString& license )
                        : QMainWindow()
    , controllers_     ( controllers )
    , staticModel_     ( staticModel )
    , model_           ( model )
    , network_         ( network )
    , config_          ( config )
    , forward_         ( new gui::CircularEventStrategy() )
    , eventStrategy_   ( new gui::ExclusiveEventStrategy( *forward_ ) )
    , pPainter_        ( new gui::ElevationPainter( staticModel_.detection_ ) )
    , simpleFilter_    ( new gui::SimpleFilter() )
    , urbanFilter_     ( new gui::UrbanFilter() )
    , pColorController_( new ColorController( controllers_ ) )
    , glProxy_         ( 0 )
    , connected_       ( false )
    , onPlanif_        ( false )

{
    QSettings settings;
    settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
    settings.beginGroup( "/Gaming" );
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());

    setAttribute( Qt::WA_DeleteOnClose, true );
    setIcon( QPixmap( tools::GeneralConfig::BuildResourceChildFile( "images/gui/logo32x32.png" ).c_str() ) );
    planifName_ = tools::translate( "Application", "SWORD" ) + tr( " - Not connected" );
    setCaption( planifName_ );

    ProfileFilter& profile = *new ProfileFilter( controllers, p ); // $$$$ AGE 2006-12-13: mem. // $$$$ _RC_ MCO 2007-01-12: auto_ptr // $$$$ AGE 2007-06-19: tégé !

    Publisher_ABC& publisher = network_.GetMessageMgr();

    lighting_ = new SimulationLighting( controllers, this );
    gui::RichItemFactory* factory = new  gui::RichItemFactory( this ); // $$$$ AGE 2006-05-11: aggregate somewhere
    gui::PreferencesDialog* prefDialog = new gui::PreferencesDialog( this, controllers, *lighting_, staticModel.coordinateSystems_, *pPainter_ );
    new VisionConesToggler( controllers, publisher, this );

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

    LinkInterpreter* interpreter = new LinkInterpreter( this, controllers, profile );
    connect( factory, SIGNAL( LinkClicked( const QString& ) ), interpreter, SLOT( Interprete( const QString& ) ) );

    // Logger
    QDockWidget* pLogDockWnd_ = new QDockWidget( "log", this );
    pLogDockWnd_->setObjectName( "log" );
    addDockWidget( Qt::BottomDockWidgetArea, pLogDockWnd_ );
    gui::Logger* pLogPanel_ = new gui::Logger( pLogDockWnd_, *factory, simulation, "./Debug/Gaming.log" );
    pLogDockWnd_->setWidget( pLogPanel_ );
    pLogDockWnd_->setWindowTitle( tr( "Log" ) );
    pLogDockWnd_->hide();
    logger.SetLogger( *pLogPanel_ );

    // Standard toolbars

    gui::TerrainProfilerLayer* profilerLayer = new gui::TerrainProfilerLayer( *glProxy_ );
    gui::GisToolbar* gToolBar = new gui::GisToolbar( this, controllers, staticModel_.detection_, *profilerLayer );
    EventToolbar* eToolBar = new EventToolbar( this, controllers, profile );
    gui::DisplayToolbar* dToolBar = new gui::DisplayToolbar( this, controllers );
    SIMControlToolbar* sToolBar = new SIMControlToolbar( this, controllers, network, publisher, *pLogPanel_ );

    addToolBar( sToolBar );
    addToolBar( dToolBar );
    addToolBar( eToolBar );
    addToolBar( gToolBar );

    // A few layers
    gui::LocationsLayer* locationsLayer = new gui::LocationsLayer( *glProxy_ );
    gui::LocationEditorToolbar* LocEditToolBar = new gui::LocationEditorToolbar( this, controllers_, staticModel.coordinateConverter_, *glProxy_, *locationsLayer );
    addToolBar( LocEditToolBar );
    gui::ParametersLayer* paramLayer = new gui::ParametersLayer( *glProxy_, *LocEditToolBar );
    ::AgentsLayer* agentsLayer = new ::AgentsLayer( controllers, *glProxy_, *strategy_, *glProxy_, profile, *simpleFilter_ );
    ::AutomatsLayer* automatsLayer = new ::AutomatsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, model_.actions_, simulation, network_.GetMessageMgr(), model.agents_, *simpleFilter_ );
    ::FormationLayer* formationLayer = new ::FormationLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, model_.actions_, staticModel_, simulation, network_.GetMessageMgr(), model_.agents_, *simpleFilter_ );

    addToolBarBreak();

    //Dialogs
    new Dialogs( this, controllers, model_, staticModel, publisher, model_.actions_, simulation, profile, network.GetCommands(), config, rcResolver, *factory, *paramLayer );

    // Profile
    gui::SymbolIcons* symbols = new gui::SymbolIcons( this, *glProxy_ );
    connect( selector_, SIGNAL( Widget2dChanged( gui::GlWidget* ) ), symbols, SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );
    gui::EntitySymbols* icons = new gui::EntitySymbols( *symbols, *strategy_ );
    UserProfileDialog* profileDialog = new UserProfileDialog( this, controllers, *factory, profile, *icons, model_.userProfileFactory_ );

    // Agent list panel
    QDockWidget* pListDockWnd_ = new QDockWidget( this );
    pListDockWnd_->setObjectName( "agentList" );
    addDockWidget( Qt::LeftDockWidgetArea, pListDockWnd_ );
    Q3VBox* box = new Q3VBox( pListDockWnd_ );
    new OrbatToolbar( box, controllers, profile, *automatsLayer, *formationLayer );
    QTabWidget* pListsTabWidget = new QTabWidget( box );

    pListsTabWidget->addTab( new TacticalList( controllers, model_.actions_, staticModel, simulation, *factory, profile, *icons ), tr( "Tactical" ) );
    pListsTabWidget->addTab( new AgentList( controllers, model_.actions_, staticModel, simulation, *factory, profile, *icons ), tr( "Communication" ) );
    logisticListView_ = new gui::LogisticList< ::LogisticListView >( controllers, *factory, profile, *icons, model_.actions_, staticModel, simulation );
    pListsTabWidget->addTab( logisticListView_, tr( "Logistic" ) );
    pListsTabWidget->addTab( new gui::ObjectList( controllers, *factory, profile ), tr( "Objects" ) );
    pListsTabWidget->addTab( new gui::PopulationList( controllers, *factory, profile ), tr( "Crowds" ) );
    pListsTabWidget->addTab( new gui::InhabitantList( controllers, *factory, profile ), tr( "Populations" ) );
    pListDockWnd_->setWidget( box );
    pListDockWnd_->setWindowTitle( tr( "Orbat" ) );

    // Mini views
    gui::MiniViews* miniviews = new gui::MiniViews( this, controllers_ );
    addDockWidget( Qt::RightDockWidgetArea, miniviews );
    connect( selector_, SIGNAL( Widget2dChanged( gui::GlWidget* ) ), miniviews, SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );
    miniviews->hide();

    // Properties
    QDockWidget* pPropertiesWnd = new QDockWidget( "properties", this );
    pPropertiesWnd->setObjectName( "properties" );
    Properties* properties = new Properties( pPropertiesWnd, controllers );
    pPropertiesWnd->setWidget( properties );
    pPropertiesWnd->setWindowTitle( tr( "Properties" ) );
    addDockWidget( Qt::LeftDockWidgetArea, pPropertiesWnd );
    pPropertiesWnd->hide();

    // Info panel
    QDockWidget* pInfoDockWnd_ = new QDockWidget( "oldinfo", this );
    pInfoDockWnd_->setObjectName( "oldInfo" );
    addDockWidget( Qt::RightDockWidgetArea, pInfoDockWnd_ );
    InfoPanels* pInfoPanel_ = new InfoPanels( pInfoDockWnd_, controllers, *factory, model_.actions_, staticModel_, simulation );
    pInfoDockWnd_->setWidget( pInfoPanel_ );
    pInfoDockWnd_->setWindowTitle( tr( "Knowledge" ) );
    pInfoDockWnd_->hide();

     // Mission panel
    pMissionPanel_ = new MissionPanel( this, controllers_, staticModel_, publisher, *paramLayer, *glProxy_, profile, model_.actions_, model_.agentKnowledgeConverter_, model_.objectKnowledgeConverter_, simulation );
    addDockWidget( Qt::LeftDockWidgetArea, pMissionPanel_ );
    pMissionPanel_->setProperty( "notAppropriate", QVariant( true ) );
    pMissionPanel_->hide();

    // Chat
    QDockWidget* chatDock = new ChatDock( this, controllers_, publisher, network.GetCommands() );
    addDockWidget( Qt::BottomDockWidgetArea, chatDock );
    chatDock->hide();

    new CommandFacade( this, controllers_, config, network.GetCommands(), *interpreter, *glProxy_, profile );
    new ClientCommandFacade( this, controllers_, publisher );

    // Info
    QDockWidget* infoWnd = new InfoDock( this, controllers_, p, *icons, *factory, staticModel_, model_.actions_, simulation ); // $$$$ ABR 2011-08-09: p or profile ???
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
    gui::TerrainLayer* terrainLayer = new gui::TerrainLayer( controllers_, *glProxy_, prefDialog->GetPreferences(), *picker );
    WeatherLayer* meteoLayer = new WeatherLayer( *glProxy_, *eventStrategy_, controllers_, model_.meteo_, *picker );

    // object/unit creation window
    QDockWidget* pCreationWnd = new QDockWidget( "creation", this );
    pCreationWnd->setObjectName( "creation" );
    addDockWidget( Qt::RightDockWidgetArea, pCreationWnd );
    pCreationWnd->hide();
    CreationPanels* creationPanels = new CreationPanels( pCreationWnd, controllers, staticModel_, *factory, model_.actions_, simulation, *paramLayer, *meteoLayer, *glProxy_, *symbols, *strategy_, model_.drawings_, config_ );
    pCreationWnd->setWidget( creationPanels );
    pCreationWnd->setWindowTitle( tr( "Creation" ) );

    new MagicOrdersInterface( this, controllers_, model_.actions_, staticModel_, simulation, *paramLayer, profile );
    ReplayerToolbar* replayerToolbar = new ReplayerToolbar( this, controllers, publisher );
    addToolBar( replayerToolbar );
    IndicatorExportDialog* indicatorExportDialog = new IndicatorExportDialog( this );
    IndicatorPlotFactory* plotFactory = new IndicatorPlotFactory( this, controllers_, publisher, *indicatorExportDialog, simulation );
    AfterAction* aar = new AfterAction( this, controllers_, *factory, model.aar_, *paramLayer, staticModel_, *plotFactory );

    // Actions panel
    {
        TimelinePanel* timelinePanel = new TimelinePanel( this, controllers_, model_.actions_, *scheduler, config_, *factory, profile );
        addDockWidget( Qt::TopDockWidgetArea, timelinePanel );
        connect( timelinePanel, SIGNAL( PlanificationModeChange() ), this, SLOT( OnPlanifStateChange() ) );
        connect( timelinePanel, SIGNAL( PlanificationModeChange() ), this, SLOT( OnNameChanged() ) );
        timelinePanel->hide();
    }
    // Score panel
    {
        ScorePanel* scorePanel = new ScorePanel( this, controllers_, *factory, *interpreter, *plotFactory, *indicatorExportDialog, model_.scores_, config );
        addDockWidget( Qt::RightDockWidgetArea, scorePanel );
        scorePanel->hide();
    }
    // Notes panel
    {
        NotesPanel* notePanel = new NotesPanel( this, controllers_.controller_, *factory, model_.notes_, publisher );
        addDockWidget( Qt::RightDockWidgetArea, notePanel );
    }
    // Message panel
    {
        QDockWidget* messageDock = new MessagePanel( this, controllers_, publisher, network.GetCommands() );
        messageDock->setProperty( "notAppropriate", QVariant( true ) );
        addDockWidget( Qt::TopDockWidgetArea, messageDock );
    }
    // ResourceNetwork panel
    {
        QDockWidget* pResourceWnd = new ResourceLinksDialog( this, controllers, model_.actions_, staticModel, simulation );
        addDockWidget( Qt::LeftDockWidgetArea, pResourceWnd );
        pResourceWnd->hide();
    }
    // Extensions panel
    {
        pExtensionsPanel_ = new ExtensionsPanel( this, controllers, model, staticModel_, simulation, *factory, *icons, profile, "ExtensionsPanel" );
        addDockWidget( Qt::LeftDockWidgetArea, pExtensionsPanel_ );
        pExtensionsPanel_->hide();
    }

    gui::HelpSystem* help = new gui::HelpSystem( this, config_.BuildResourceChildFile( "help/gaming.xml" ) );
    setMenuBar( new Menu( this, controllers, staticModel_, *prefDialog, *profileDialog, *factory, license, *help, *interpreter, network_, logger ) );

    CreateLayers( *pMissionPanel_, *creationPanels, *paramLayer, *locationsLayer, *agentsLayer, *automatsLayer, *formationLayer, *terrainLayer, *meteoLayer, *profilerLayer, *prefDialog, profile, simulation, *picker );
    ::StatusBar* pStatus_ = new ::StatusBar( statusBar(), *picker, staticModel_.detection_, staticModel_.coordinateConverter_, controllers_, pProfilerDockWnd_ );
    connect( selector_, SIGNAL( MouseMove( const geometry::Point2f& ) ), pStatus_, SLOT( OnMouseMove( const geometry::Point2f& ) ) );
    connect( selector_, SIGNAL( MouseMove( const geometry::Point3f& ) ), pStatus_, SLOT( OnMouseMove( const geometry::Point3f& ) ) );
    controllers_.Register( *this );

//    ReadSettings();
    ReadOptions();

    pMissionPanel_->hide();
    replayerToolbar->hide();
    aar->SetStartup();

    restoreState(settings.value("mainWindowState").toByteArray());
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
    gui::Layer_ABC& urbanLayer           = *new gui::UrbanLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, *simpleFilter_ );
    gui::Layer_ABC& grid                 = *new gui::GridLayer( controllers_, *glProxy_ );
    gui::Layer_ABC& metrics              = *new gui::MetricsLayer( staticModel_.detection_, *glProxy_ );
    gui::Layer_ABC& limits               = *new LimitsLayer( controllers_, *glProxy_, *strategy_, parameters, model_.tacticalLineFactory_, *glProxy_, profile, *simpleFilter_ );
    gui::Layer_ABC& objectsLayer         = *new ::ObjectsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, model_.actions_, staticModel_, simulation, picker, *urbanFilter_ );
    gui::Layer_ABC& populations          = *new ::PopulationsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, *simpleFilter_ );
    gui::Layer_ABC& inhabitants          = *new gui::InhabitantLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, *simpleFilter_ );
    gui::Layer_ABC& agentKnowledges      = *new AgentKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, *simpleFilter_ );
    gui::Layer_ABC& populationKnowledges = *new PopulationKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, *simpleFilter_ );
    gui::Layer_ABC& objectKnowledges     = *new ObjectKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, *simpleFilter_ );
    gui::Layer_ABC& defaultLayer         = *new gui::DefaultLayer( controllers_ );
    gui::Layer_ABC& logoLayer            = *new gui::LogoLayer( *glProxy_, QImage( config_.BuildResourceChildFile( "logo.png" ).c_str() ), 0.7f );
    gui::Layer_ABC& teamLayer            = *new ::TeamLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, model_.actions_, staticModel_, simulation, network_.GetMessageMgr(), *simpleFilter_ );
    gui::Layer_ABC& fogLayer             = *new FogLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, *simpleFilter_ );
    gui::Layer_ABC& drawerLayer          = *new gui::DrawerLayer( controllers_, *glProxy_, *strategy_, parameters, *glProxy_, profile, *simpleFilter_ );
    gui::Layer_ABC& actionsLayer         = *new ActionsLayer( controllers_, *glProxy_ );
    gui::Layer_ABC& contour              = *new gui::ContourLinesLayer( controllers_ );

    // ordre de dessin
    glProxy_->Register( defaultLayer );
    glProxy_->Register( elevation2d );              preferences.AddLayer( tr( "Elevation" ), elevation2d );         elevation2d         .SetPasses( "main,composition,miniviews" );
    glProxy_->Register( raster );                   preferences.AddLayer( tr( "Raster" ), raster );                 raster              .SetPasses( "main,composition,miniviews" );
    glProxy_->Register( terrain );                  preferences.AddLayer( tr( "Terrain" ), terrain );               terrain             .SetPasses( "main,composition,miniviews" );
    glProxy_->Register( contour );                  preferences.AddLayer( tr( "Contour Lines" ), contour );         contour             .SetPasses( "main,composition,miniviews" );
    glProxy_->Register( urbanLayer );               /*preferences.AddLayer( tr( "Urban" ), urbanLayer );*/          urbanLayer          .SetPasses( "main,miniviews" );
    glProxy_->Register( watershed );                preferences.AddLayer( tr( "Watershed" ), watershed );           watershed           .SetPasses( "main,composition,miniviews" );
    glProxy_->Register( elevation3d );
    glProxy_->Register( grid );                                                                                     grid                .SetPasses( "main,miniviews" );
    glProxy_->Register( weather );                                                                                  weather             .SetPasses( "main,miniviews" );
    glProxy_->Register( limits );                                                                                   limits              .SetPasses( "main,miniviews" );
    glProxy_->Register( objectKnowledges );                                                                         objectKnowledges    .SetPasses( "main,miniviews" );
    glProxy_->Register( populationKnowledges );                                                                     populationKnowledges.SetPasses( "main,miniviews" );
    glProxy_->Register( agentKnowledges );                                                                          agentKnowledges     .SetPasses( "main,miniviews" );
    glProxy_->Register( formationLayer );           preferences.AddLayer( tr( "Formations" ), formationLayer );     formationLayer      .SetPasses( "main,miniviews" );
    glProxy_->Register( teamLayer );                                                                                teamLayer           .SetPasses( "main,miniviews" );
    glProxy_->Register( objectsLayer );             preferences.AddLayer( tr( "Objects" ), objectsLayer );          objectsLayer        .SetPasses( "main,miniviews" );
    glProxy_->Register( populations );              preferences.AddLayer( tr( "Crowds" ),  populations );           populations         .SetPasses( "main,miniviews" );
    glProxy_->Register( inhabitants );              preferences.AddLayer( tr( "Populations" ), inhabitants );       inhabitants         .SetPasses( "main,miniviews" );
    glProxy_->Register( agents );                   preferences.AddLayer( tr( "Units" ), agents );                  agents              .SetPasses( "main,miniviews" );
    glProxy_->Register( automats );                 preferences.AddLayer( tr( "Automats" ), automats );             automats            .SetPasses( "main,miniviews" );
    glProxy_->Register( missionsLayer );                                                                            missionsLayer       .SetPasses( "main,miniviews" );
    glProxy_->Register( actionsLayer );                                                                             actionsLayer        .SetPasses( "main" );
    glProxy_->Register( creationsLayer );                                                                           creationsLayer      .SetPasses( "main" );
    glProxy_->Register( parameters );                                                                               parameters          .SetPasses( "main" );
    glProxy_->Register( metrics );                                                                                  metrics             .SetPasses( "main" );
    glProxy_->Register( locationsLayer );                                                                           locationsLayer      .SetPasses( "main" );
    glProxy_->Register( profilerLayer );                                                                            profilerLayer       .SetPasses( "main" );
    glProxy_->Register( drawerLayer );                                                                              drawerLayer         .SetPasses( "main,miniviews" );
    glProxy_->Register( fogLayer );                                                                                 fogLayer            .SetPasses( "fog" );
    glProxy_->Register( tooltipLayer );                                                                             tooltipLayer        .SetPasses( "tooltip" );
    glProxy_->Register( logoLayer );                preferences.AddLayer( tr( "Logo" ), logoLayer );                logoLayer           .SetPasses( "main" );

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
        if( logisticListView_ )
            logisticListView_->Purge();
        model_.Purge();
        selector_->Close();
        selector_->Load();
        staticModel_.Load( config_ );
        pExtensionsPanel_->hide();
        ReadOptions();
    }
    catch( xml::exception& e )
    {
        Close();
        QMessageBox::critical( this, tools::translate( "Application", "SWORD" )
                                   , ( tools::translate( "MainWindow", "Error loading exercise: " ) + e.what() ).ascii() );
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Close
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void MainWindow::Close()
{
    network_.Disconnect();
    selector_->Close();
    if( logisticListView_ )
        logisticListView_->Purge();
    model_.Purge();
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
    QSettings settings;
    settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
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
    gui::Settings settings;
    settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
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
    gui::Settings settings;
    settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
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
        return ( !name.empty() ) ? name.c_str() : bfs::path( filename, bfs::native ).parent_path().leaf().c_str();
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
        if( !connected_ )
        {
            if( simulation.IsInitialized() )
            {
                connected_ = true; // we update the caption until Model is totally loaded
                if( profile_.isEmpty() )
                    profile_ = tools::translate( "LoginDialog", "Anonymous" );
            }
            planifName_ = appName + QString( " - [%1@%2][%3]" )
                .arg( profile_ )
                .arg( simulation.GetSimulationHost().c_str() )
                .arg( ExtractExerciceName( config_.GetExerciseFile() ));
            if( onPlanif_ )
              setCaption( planifName_ + modePlanif );
            else
             setCaption( planifName_ );
            // $$$$ SBO 2009-12-18: Use exercise META data
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
        static LoginDialog* dialog = new LoginDialog( this, profile, network_, controllers_ );
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

// -----------------------------------------------------------------------------
// Name: MainWindow::OnNameChanged
// Created: HBD 2010-09-23
// -----------------------------------------------------------------------------
void MainWindow::OnNameChanged()
{
    onPlanif_ = !onPlanif_;
    if( onPlanif_ )
        setCaption( planifName_ + tools::translate( "Application", " - Planning mode on" ) );
    else
        setCaption( planifName_ );

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

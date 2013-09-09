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
#include "AgentListView.h"
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
#include "LockMapViewController.h"
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
#include "TacticalListView.h"
#include "TeamLayer.h"
#include "TimelinePanel.h"
#include "UserProfileDialog.h"
#include "WeatherLayer.h"
#include "OrbatPanel.h"
#include "PlanificationModePanel.h"
#include "clients_kernel/ActionController.h"
#include "actions_gui/InterfaceBuilder.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Tools.h"
#include "gaming/AgentServerMsgMgr.h"
#include "gaming/AgentsModel.h"
#include "gaming/ModeController.h"
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
#include "gaming/TeamsModel.h"
#include "clients_gui/AddRasterDialog.h"
#include "clients_gui/AggregateToolbar.h"
#include "clients_gui/DisplayToolbar.h"
#include "clients_gui/GlSelector.h"
#include "clients_gui/GraphicPreferences.h"
#include "clients_gui/Logger.h"
#include "clients_gui/MiscLayer.h"
#include "clients_gui/OptionsPanel.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/Settings.h"
#include "clients_gui/ObjectListView.h"
#include "clients_gui/SearchListView.h"
#include "clients_gui/PopulationListView.h"
#include "clients_gui/InhabitantListView.h"
#include "clients_gui/InhabitantLayer.h"
#include "clients_gui/PreferencesDialog.h"
#include "clients_gui/RichItemFactory.h"
#include "clients_gui/MiniViews.h"
#include "clients_gui/resources.h"
#include "clients_gui/GlProxy.h"
#include "clients_gui/ColorStrategy.h"
#include "clients_gui/SelectionMenu.h"
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
                        Network& network, const Profile_ABC& p, Config& config, LoggerProxy& logger,
                        const RcEntityResolver_ABC& rcResolver, const QString& license )
                        : QMainWindow()
    , controllers_     ( controllers )
    , staticModel_     ( staticModel )
    , model_           ( model )
    , network_         ( network )
    , config_          ( config )
    , pPainter_        ( new gui::ElevationPainter( staticModel_.detection_ ) )
    , pColorController_( new ColorController( controllers_ ) )
    , glProxy_         ( 0 )
    , connected_       ( false )
    , onPlanif_        ( false )
    , icons_           ( 0 )
{
    controllers_.modes_->SetMainWindow( this );
    QSettings settings( "MASA Group", tools::translate( "Application", "SWORD" ) );
    settings.beginGroup( "/Gaming" );
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());

    setAttribute( Qt::WA_DeleteOnClose, true ); 
    setIcon( QPixmap( tools::GeneralConfig::BuildResourceChildFile( "images/gui/logo32x32.png" ).c_str() ) );
    planifName_ = tools::translate( "Application", "SWORD" ) + tr( " - Not connected" );
    setCaption( planifName_ );

    ProfileFilter& profile = *new ProfileFilter( controllers, p ); // $$$$ AGE 2006-12-13: mem. // $$$$ _RC_ MCO 2007-01-12: auto_ptr // $$$$ AGE 2007-06-19: tégé !

    Publisher_ABC& publisher = network_.GetMessageMgr();

    glProxy_.reset( new gui::GlProxy() );
    strategy_.reset( new gui::ColorStrategy( controllers, *glProxy_, *pColorController_ ) );
    strategy_->Add( std::auto_ptr< gui::ColorModifier_ABC >( new gui::SelectionColorModifier( controllers, *glProxy_ ) ) );
    strategy_->Add( std::auto_ptr< gui::ColorModifier_ABC >( new gui::HighlightColorModifier( controllers ) ) );

    // Symbols
    gui::SymbolIcons* symbols = new gui::SymbolIcons( this, *glProxy_ );
    icons_.reset( new gui::EntitySymbols( *symbols, *strategy_ ) );

    forward_.reset( new gui::CircularEventStrategy( *icons_, *strategy_, staticModel_.drawings_, *glProxy_ ) );
    eventStrategy_.reset( new gui::ExclusiveEventStrategy( *forward_ ) );

    QStackedWidget* centralWidget = new QStackedWidget();
    setCentralWidget( centralWidget );
    selector_.reset( new gui::GlSelector( centralWidget, *glProxy_, controllers, config, staticModel.detection_, *eventStrategy_ ) );
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
    connect( selector_.get(), SIGNAL( Widget2dChanged( gui::GlWidget* ) ), forward_->GetSelectionMenu(), SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );
    connect( selector_.get(), SIGNAL( Widget2dChanged( gui::GlWidget* ) ), symbols, SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );

    lighting_.reset( new SimulationLighting( controllers, this ) );
    gui::RichItemFactory* factory = new  gui::RichItemFactory( this ); // $$$$ AGE 2006-05-11: aggregate somewhere
    preferenceDialog_.reset( new gui::PreferencesDialog( this, controllers, *lighting_, staticModel.coordinateSystems_, *pPainter_, *selector_ ) );
    preferenceDialog_->AddPage( tr( "Orbat" ), *new OrbatPanel( preferenceDialog_.get(), controllers ) );
    new VisionConesToggler( controllers, publisher, this );

    LinkInterpreter* interpreter = new LinkInterpreter( this, controllers, profile );
    connect( factory, SIGNAL( LinkClicked( const QString& ) ), interpreter, SLOT( Interprete( const QString& ) ) );

    lockMapViewController_.reset( new LockMapViewController( controllers, *glProxy_ ) );

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
    gui::GisToolbar* gToolBar = new gui::GisToolbar( this, controllers, staticModel_.detection_ );
    EventToolbar* eToolBar = new EventToolbar( this, controllers, profile );
    gui::DisplayToolbar* dToolBar = new gui::DisplayToolbar( this, controllers );
    SIMControlToolbar* sToolBar = new SIMControlToolbar( this, controllers, network, publisher, *pLogPanel_ );

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
    ::AgentsLayer* agentsLayer = new ::AgentsLayer( controllers, *glProxy_, *strategy_, *glProxy_, profile );
    ::AutomatsLayer* automatsLayer = new ::AutomatsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, model_.actions_, simulation, network_.GetMessageMgr(), model.agents_ );
    ::FormationLayer* formationLayer = new ::FormationLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, model_.actions_, staticModel_, simulation, network_.GetMessageMgr(), model_.agents_ );

    addToolBarBreak();

    //Dialogs
    new Dialogs( this, controllers, model_, staticModel, publisher, model_.actions_, simulation, profile, network.GetCommands(), config, rcResolver, *factory, *parameters_ );
    addRasterDialog_.reset( new gui::AddRasterDialog( this ) );

    // Profile

    UserProfileDialog* profileDialog = new UserProfileDialog( this, controllers, *factory, profile, *icons_, model_.userProfileFactory_ );

    // Agent list panel
    QDockWidget* pListDockWnd_ = new QDockWidget( this );
    pListDockWnd_->setObjectName( "agentList" );
    addDockWidget( Qt::LeftDockWidgetArea, pListDockWnd_ );
    Q3VBox* box = new Q3VBox( pListDockWnd_ );
    OrbatToolbar* orbatToolbar = new OrbatToolbar( box, controllers, profile, *automatsLayer, *formationLayer );
    const gui::AggregateToolbar* aggregateToolbar = orbatToolbar->GetToolbar();
    QTabWidget* pListsTabWidget = new QTabWidget( box );

    gui::SearchListView_ABC* searchListView = 0;
    {
        searchListView = new gui::SearchListView< TacticalListView >( pListsTabWidget, controllers, model_.actions_, staticModel, simulation, *factory, profile, *icons_ );
        searchListView->connect( aggregateToolbar, SIGNAL( LockDragAndDrop( bool ) ), searchListView->GetRichListView(), SLOT( LockDragAndDrop( bool ) ) );
        pListsTabWidget->addTab( searchListView, tr( "Tactical" ) );
    }
    {
        searchListView = new gui::SearchListView< AgentListView >( pListsTabWidget, controllers, model_.actions_, staticModel, simulation, *factory, profile, *icons_ );
        searchListView->connect( aggregateToolbar, SIGNAL( LockDragAndDrop( bool ) ), searchListView->GetRichListView(), SLOT( LockDragAndDrop( bool ) ) );
        pListsTabWidget->addTab( searchListView, tr( "Communication" ) );
    }
    {
        gui::SearchListView< ::LogisticListView >* logisticSearchListView = new gui::SearchListView< ::LogisticListView >( pListsTabWidget, controllers, *factory, profile, *icons_, model_.actions_, staticModel, simulation );
        logisticSearchListView->connect( aggregateToolbar, SIGNAL( LockDragAndDrop( bool ) ), logisticSearchListView->GetRichListView(), SLOT( LockDragAndDrop( bool ) ) );
        logisticListView_ = logisticSearchListView->GetListView();
        pListsTabWidget->addTab( logisticSearchListView, tr( "Logistic" ) );
    }
    {
        searchListView = new gui::SearchListView< gui::ObjectListView >( pListsTabWidget, controllers, *factory, profile );
        searchListView->connect( aggregateToolbar, SIGNAL( LockDragAndDrop( bool ) ), searchListView->GetRichListView(), SLOT( LockDragAndDrop( bool ) ) );
        pListsTabWidget->addTab( searchListView, tr( "Objects" ) );

    }
    {
        searchListView = new gui::SearchListView< gui::PopulationListView >( pListsTabWidget, controllers, *factory, profile );
        searchListView->connect( aggregateToolbar, SIGNAL( LockDragAndDrop( bool ) ), searchListView->GetRichListView(), SLOT( LockDragAndDrop( bool ) ) );
        pListsTabWidget->addTab( searchListView, tr( "Crowds" ) );
    }
    {
        searchListView = new gui::SearchListView< gui::InhabitantListView >( pListsTabWidget, controllers, *factory, profile );
        searchListView->connect( aggregateToolbar, SIGNAL( LockDragAndDrop( bool ) ), searchListView->GetRichListView(), SLOT( LockDragAndDrop( bool ) ) );
        pListsTabWidget->addTab( searchListView, tr( "Populations" ) );
    }

    pListDockWnd_->setWidget( box );
    pListDockWnd_->setWindowTitle( tr( "Orbat" ) );

    // Mini views
    gui::MiniViews* miniviews = new gui::MiniViews( this, controllers_ );
    addDockWidget( Qt::RightDockWidgetArea, miniviews );
    connect( selector_.get(), SIGNAL( Widget2dChanged( gui::GlWidget* ) ), miniviews, SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );
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
    InfoPanels* pInfoPanel_ = new InfoPanels( pInfoDockWnd_, controllers, *factory, model_.actions_, staticModel_, simulation );
    pInfoDockWnd_->setWidget( pInfoPanel_ );
    pInfoDockWnd_->setWindowTitle( tr( "Knowledge" ) );
    pInfoDockWnd_->hide();

    // Interface Builder
    interfaceBuilder_.reset( new actions::gui::InterfaceBuilder( controllers_, *parameters_, staticModel_, &model_.agentKnowledgeConverter_, &model_.objectKnowledgeConverter_, &simulation ) );
    // Mission panel
    pMissionPanel_ = new MissionPanel( this, controllers_, staticModel_, publisher, *glProxy_, profile, model_.actions_, simulation, *interfaceBuilder_, config );
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
    QDockWidget* infoWnd = new InfoDock( this, controllers_, p, *icons_, *factory, staticModel_, model_.actions_, simulation ); // $$$$ ABR 2011-08-09: p or profile ???
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
    WeatherLayer* meteoLayer = new WeatherLayer( *glProxy_, *eventStrategy_, controllers_, model_.meteo_, *picker, profile );

    // object/unit creation window
    QDockWidget* pCreationWnd = new QDockWidget( "creation", this );
    pCreationWnd->setObjectName( "creation" );
    addDockWidget( Qt::RightDockWidgetArea, pCreationWnd );
    pCreationWnd->hide();
    CreationPanels* creationPanels = new CreationPanels( pCreationWnd, controllers, staticModel_, *factory, model_.actions_, simulation, *parameters_, *meteoLayer, *glProxy_, *symbols, *strategy_, model_.drawings_, config_ );
    pCreationWnd->setWidget( creationPanels );
    pCreationWnd->setWindowTitle( tr( "Creation" ) );

    new MagicOrdersInterface( this, controllers_, model_.actions_, staticModel_, simulation, *parameters_, profile, *selector_ );
    ReplayerToolbar* replayerToolbar = new ReplayerToolbar( this, controllers, publisher );
    addToolBar( replayerToolbar );
    IndicatorExportDialog* indicatorExportDialog = new IndicatorExportDialog( this );
    IndicatorPlotFactory* plotFactory = new IndicatorPlotFactory( this, controllers_, publisher, *indicatorExportDialog, simulation );
    aar_ = new AfterAction( this, controllers_, *factory, model.aar_, *plotFactory, *interfaceBuilder_ );

    // Actions panel
    {
        TimelinePanel* timelinePanel = new TimelinePanel( this, controllers_, model_.actions_, *scheduler, config_, *factory, profile );
        addDockWidget( Qt::TopDockWidgetArea, timelinePanel );
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

    CreateLayers( *pMissionPanel_, *creationPanels, *parameters_, *locationsLayer, *agentsLayer, *automatsLayer, *formationLayer, *terrainLayer, *meteoLayer, *profilerLayer, *preferenceDialog_, profile, simulation, *picker );
    ::StatusBar* pStatus_ = new ::StatusBar( statusBar(), *picker, staticModel_.detection_, staticModel_.coordinateConverter_, controllers_, pProfilerDockWnd_ );
    connect( selector_.get(), SIGNAL( MouseMove( const geometry::Point2f& ) ), pStatus_, SLOT( OnMouseMove( const geometry::Point2f& ) ) );
    connect( selector_.get(), SIGNAL( MouseMove( const geometry::Point3f& ) ), pStatus_, SLOT( OnMouseMove( const geometry::Point3f& ) ) );
    controllers_.Register( *this );

//    ReadSettings();
    ReadOptions();

    pMissionPanel_->hide();
    replayerToolbar->hide();
    aar_->SetStartup();

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
    delete aar_;
    delete pMissionPanel_;
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
void MainWindow::CreateLayers( MissionPanel& missions, CreationPanels& creationPanels, gui::ParametersLayer& parameters, gui::Layer& locationsLayer,
                               gui::AgentsLayer& agents, gui::AutomatsLayer& automats, gui::FormationLayer& formationLayer, gui::TerrainLayer& terrain, gui::Layer& weather, gui::Layer& profilerLayer,
                               gui::PreferencesDialog& preferences, const Profile_ABC& profile, const Simulation& simulation, gui::TerrainPicker& picker )
{
    gui::TooltipsLayer_ABC& tooltipLayer = *new gui::TooltipsLayer( *glProxy_ );
    gui::Layer& missionsLayer        = *new gui::MiscLayer< MissionPanel >( missions );
    gui::Layer& creationsLayer       = *new gui::MiscLayer< CreationPanels >( creationPanels );
    gui::Elevation2dLayer& elevation2d   = *new gui::Elevation2dLayer( controllers_.controller_, staticModel_.detection_ );
    gui::Layer& raster               = *new gui::RasterLayer( controllers_.controller_ );
    gui::Layer& watershed            = *new gui::WatershedLayer( controllers_, staticModel_.detection_ );
    gui::Layer& elevation3d          = *new gui::Elevation3dLayer( controllers_.controller_, staticModel_.detection_, *lighting_ );
    gui::Layer& urbanLayer           = *new gui::UrbanLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    gui::Layer& grid                 = *new gui::GridLayer( controllers_, *glProxy_ );
    gui::Layer& metrics              = *new gui::MetricsLayer( staticModel_.detection_, *glProxy_ );
    gui::Layer& limits               = *new LimitsLayer( controllers_, *glProxy_, *strategy_, parameters, model_.tacticalLineFactory_, *glProxy_, profile );
    gui::Layer& objectsLayer         = *new ::ObjectsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, model_.actions_, staticModel_, simulation, picker );
    gui::Layer& populations          = *new ::PopulationsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    gui::Layer& inhabitants          = *new gui::InhabitantLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    gui::Layer& agentKnowledges      = *new AgentKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    gui::Layer& populationKnowledges = *new PopulationKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    gui::Layer& objectKnowledges     = *new ObjectKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    gui::Layer& defaultLayer         = *new gui::DefaultLayer( controllers_ );
    gui::Layer& logoLayer            = *new gui::LogoLayer( *glProxy_, QImage( config_.BuildResourceChildFile( "logo.png" ).c_str() ), 0.7f );
    gui::Layer& teamLayer            = *new ::TeamLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, model_.actions_, staticModel_, simulation, network_.GetMessageMgr() );
    gui::Layer& fogLayer             = *new FogLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    gui::Layer& drawerLayer          = *new gui::DrawerLayer( controllers_, *glProxy_, *strategy_, parameters, *glProxy_, profile );
    gui::Layer& actionsLayer         = *new ActionsLayer( controllers_, *glProxy_ );
    gui::Layer& contour              = *new gui::ContourLinesLayer( controllers_, staticModel_.detection_ );

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
        if( logisticListView_ )
            logisticListView_->Purge();
        if( lockMapViewController_.get() )
            lockMapViewController_->Clear();
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
    if( logisticListView_ )
        logisticListView_->Purge();
    if( lockMapViewController_.get() )
        lockMapViewController_->Clear();
    model_.Purge();
    staticModel_.Purge();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::closeEvent
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::closeEvent( QCloseEvent* pEvent )
{
    QSettings settings( "MASA Group", tools::translate( "Application", "SWORD" ) );
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
    gui::Settings settings( "MASA Group", tools::translate( "Application", "SWORD" ) );
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
    gui::Settings settings( "MASA Group", tools::translate( "Application", "SWORD" ) );
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
        xml::xifstream xis( filename );
        xis >> xml::start( "exercise" )
                >> xml::optional >> xml::start( "meta" );

        return bfs::path( filename, bfs::native ).parent_path().leaf().c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::NotifyUpdated
// Created: SBO 2007-03-20
// -----------------------------------------------------------------------------
void MainWindow::NotifyUpdated( const Simulation& simulation )
{
    static bool firstPass = true;
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
    if( simulation.IsInitialized() && firstPass )
    {
        icons_->GenerateSymbols( model_.teams_ );
        firstPass = false;
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
        if ( restoreState( docks_ ) )
            docks_ = 0;
        if ( restoreState( toolbars_ ) )
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
            parameters << ( std::string( "--raster=" ) + addRasterDialog_->GetFiles().toAscii().constData() ).c_str();
            parameters << ( std::string( "--pixelsize=" ) + boost::lexical_cast< std::string >( addRasterDialog_->GetPixelSize() ) ).c_str();
            bfs::path filename = bfs::system_complete( bfs::path( config_.GetGraphicsDirectory(), bfs::native ) / "~~tmp.texture.bin" );
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
            const bfs::path aggregated = bfs::path( config_.GetGraphicsDirectory(), bfs::native ) / "~~tmp.texture.bin";
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

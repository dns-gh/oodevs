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
#include "ClientCommandFacade.h"
#include "CommandFacade.h"
#include "CreationPanels.h"
#include "Dialogs.h"
#include "DockContainer.h"
#include "EventDockWidget.h"
#include "EventToolbar.h"
#include "FirePlayer.h"
#include "FogLayer.h"
#include "FormationLayer.h"
#include "GamingConfig.h"
#include "icons.h"
#include "IndicatorExportDialog.h"
#include "IndicatorPlotFactory.h"
#include "ItineraryEditionDockWidget.h"
#include "LimitsLayer.h"
#include "LinkInterpreter.h"
#include "LockMapViewController.h"
#include "LoggerProxy.h"
#include "LogisticMagicInterface.h"
#include "ConnectLoginDialog.h"
#include "MagicOrdersInterface.h"
#include "Menu.h"
#include "MessagePanel.h"
#include "ObjectKnowledgesLayer.h"
#include "ObjectsLayer.h"
#include "PopulationKnowledgesLayer.h"
#include "PopulationsLayer.h"
#include "ProfilingPanel.h"
#include "ReplayerToolbar.h"
#include "SIMControlToolbar.h"
#include "SimulationLighting.h"
#include "StatusBar.h"
#include "TeamLayer.h"
#include "DrawingsBuilder.h"
#include "WeatherLayer.h"
#include "PathfindLayer.h"
#include "UnitStateDialog.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ModeController.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"
#include "gaming/AgentServerMsgMgr.h"
#include "gaming/AgentsModel.h"
#include "gaming/Model.h"
#include "gaming/Network.h"
#include "gaming/StaticModel.h"
#include "gaming/Profile.h"
#include "gaming/ProfileFilter.h"
#include "gaming/Services.h"
#include "gaming/UserProfilesModel.h"
#include "gaming/VisionConesToggler.h"
#include "gaming/ColorController.h"
#include "gaming/TeamsModel.h"
#include "clients_gui/AddRasterDialog.h"
#include "clients_gui/CircularEventStrategy.h"
#include "clients_gui/ContourLinesLayer.h"
#include "clients_gui/ColorStrategy.h"
#include "clients_gui/DefaultLayer.h"
#include "clients_gui/DisplayExtractor.h"
#include "clients_gui/DisplayToolbar.h"
#include "clients_gui/DrawerLayer.h"
#include "clients_gui/Elevation2dLayer.h"
#include "clients_gui/Elevation3dLayer.h"
#include "clients_gui/ElevationPainter.h"
#include "clients_gui/EntitySymbols.h"
#include "clients_gui/ExclusiveEventStrategy.h"
#include "clients_gui/GisToolbar.h"
#include "clients_gui/GlProxy.h"
#include "clients_gui/GlSelector.h"
#include "clients_gui/GraphicPreferences.h"
#include "clients_gui/GridLayer.h"
#include "clients_gui/HelpSystem.h"
#include "clients_gui/HighlightColorModifier.h"
#include "clients_gui/ImageWrapper.h"
#include "clients_gui/InhabitantLayer.h"
#include "clients_gui/Layer.h"
#include "clients_gui/LocationEditorToolbar.h"
#include "clients_gui/LocationsLayer.h"
#include "clients_gui/Logger.h"
#include "clients_gui/MetricsLayer.h"
#include "clients_gui/MiniViews.h"
#include "clients_gui/MiscLayer.h"
#include "clients_gui/OrbatPanel.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/PreferencesDialog.h"
#include "clients_gui/ProfileDialog.h"
#include "clients_gui/RasterLayer.h"
#include "clients_gui/MapnikLayer.h"
#include "clients_gui/RasterProcess.h"
#include "clients_gui/ResourceNetworksLayer.h"
#include "clients_gui/RichItemFactory.h"
#include "clients_gui/SelectionColorModifier.h"
#include "clients_gui/SelectionMenu.h"
#include "clients_gui/SoundPanel.h"
#include "clients_gui/WeaponRangesPanel.h"
#include "clients_gui/SymbolIcons.h"
#include "clients_gui/TerrainLayer.h"
#include "clients_gui/TerrainPicker.h"
#include "clients_gui/TerrainProfiler.h"
#include "clients_gui/TerrainProfilerLayer.h"
#include "clients_gui/TextEditor.h"
#include "clients_gui/TooltipsLayer.h"
#include "clients_gui/UrbanLayer.h"
#include "clients_gui/WatershedLayer.h"
#include "clients_kernel/Workers.h"
#include "geodata/ProjectionException.h"
#include "protocol/Dispatcher.h"
#include "protocol/ReplaySenders.h"
#include "protocol/SimulationSenders.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

namespace
{
    void AddToolBar( QMainWindow& mainWindow, gui::RichToolBar* toolBar, int hiddenModes = eModes_None, int visibleModes = eModes_None, bool visibleByDefault = true )
    {
        mainWindow.addToolBar( toolBar );
        toolBar->SetModes( hiddenModes, visibleModes, visibleByDefault );
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::MainWindow( Controllers& controllers, ::StaticModel& staticModel, Model& model,
    const Simulation& simulation, SimulationController& simulationController,
    Network& network, ProfileFilter& filter, GamingConfig& config, LoggerProxy& logger,
    const kernel::KnowledgeConverter_ABC& converter, kernel::Workers& workers, const QString& license )
    : QMainWindow()
    , controllers_       ( controllers )
    , staticModel_       ( staticModel )
    , model_             ( model )
    , network_           ( network )
    , config_            ( config )
    , profile_           ( filter )
    , workers_           ( workers )
    , graphicPreferences_( new gui::GraphicPreferences( controllers ) )
    , pPainter_          ( new gui::ElevationPainter( staticModel_.detection_ ) )
    , pColorController_  ( new ColorController( controllers_ ) )
    , glProxy_           ( new gui::GlProxy( logger ) )
    , connected_         ( false )
    , onPlanif_          ( false )
    , drawingsBuilder_   ( new DrawingsBuilder( controllers_, profile_ ) )
{
    controllers_.modes_.SetMainWindow( this );
    controllers_.modes_.AddRegistryEntry( eModes_Gaming, "Gaming" );
    controllers_.modes_.AddRegistryEntry( eModes_Replay, "Replayer" );

    // Text editor
    textEditor_.reset( new gui::TextEditor( this ) );

    // Strategy
    strategy_.reset( new gui::ColorStrategy( controllers, *glProxy_, *pColorController_ ) );
    strategy_->Add( std::unique_ptr< gui::ColorModifier_ABC >( new gui::SelectionColorModifier( controllers, *glProxy_, profile_ ) ) );
    strategy_->Add( std::unique_ptr< gui::ColorModifier_ABC >( new gui::HighlightColorModifier( controllers, profile_ ) ) );

    // Symbols
    gui::SymbolIcons* symbols = new gui::SymbolIcons( this, *glProxy_ );
    icons_.reset( new gui::EntitySymbols( *symbols, *strategy_ ) );

    // Event strategy
    forward_.reset( new gui::CircularEventStrategy( controllers_, *icons_, *strategy_, staticModel_.drawings_, *glProxy_ ) );
    eventStrategy_.reset( new gui::ExclusiveEventStrategy( *forward_ ) );

    // Main widget
    selector_.reset( new gui::GlSelector( this, *glProxy_, controllers, config, staticModel.detection_, *eventStrategy_, logger ) );
    connect( selector_.get(), SIGNAL( Widget2dChanged( gui::GlWidget* ) ), symbols, SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );
    connect( selector_.get(), SIGNAL( Widget2dChanged( gui::GlWidget* ) ), forward_->GetSelectionMenu(), SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );
    connect( selector_.get(), SIGNAL( Widget3dChanged( gui::Gl3dWidget* ) ), forward_->GetSelectionMenu(), SLOT( OnWidget3dChanged( gui::Gl3dWidget* ) ) );
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

    //sound player
    firePlayer_.reset( new FirePlayer( controllers, profile_, simulation ) );

    // Misc
    lighting_.reset( new SimulationLighting( controllers, this ) );
    LinkInterpreter* interpreter = new LinkInterpreter( this, controllers, filter );
    gui::RichItemFactory* factory = new  gui::RichItemFactory( this ); // $$$$ AGE 2006-05-11: aggregate somewhere
    connect( factory, SIGNAL( LinkClicked( const QString& ) ), interpreter, SLOT( Interprete( const QString& ) ) );

    lockMapViewController_.reset( new LockMapViewController( controllers, *glProxy_ ) );
    gui::Elevation2dLayer& elevation2d = *new gui::Elevation2dLayer( controllers_.controller_, staticModel_.detection_ );
    preferenceDialog_.reset( new gui::PreferencesDialog( this, controllers, *lighting_, staticModel.coordinateConverter_, *pPainter_, *selector_, elevation2d, *graphicPreferences_ ) );
    preferenceDialog_->AddPage( tr( "Orbat" ), *new gui::OrbatPanel( preferenceDialog_.get(), controllers ) );
    preferenceDialog_->AddPage( tr( "Sound" ), *new gui::SoundPanel( preferenceDialog_.get(), controllers, *firePlayer_ ) );
    preferenceDialog_->AddPage( tr( "Weapon Ranges" ), *new gui::WeaponRangesPanel( preferenceDialog_.get(), controllers_, staticModel_ ) );
    new VisionConesToggler( controllers, simulationController, this );
    new CommandFacade( this, controllers_, config, network.GetCommands(), *interpreter, *glProxy_, filter, staticModel.coordinateConverter_ );
    new ClientCommandFacade( this, controllers_, network_.GetMessageMgr() );

    // First layers
    parameters_ = new gui::ParametersLayer( *glProxy_, *textEditor_ );
    gui::LocationsLayer* locationsLayer      = new gui::LocationsLayer( *glProxy_ );
    gui::TerrainPicker* picker               = new gui::TerrainPicker( this );
    WeatherLayer* meteoLayer                 = new WeatherLayer( *glProxy_, *eventStrategy_, controllers_, model_.meteo_, *picker, profile_ );
    AutomatsLayer* automatsLayer             = new AutomatsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile_, model_.actions_ );
    FormationLayer* formationLayer           = new FormationLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile_, model_.actions_, staticModel_ );
    gui::TerrainProfilerLayer* profilerLayer = new gui::TerrainProfilerLayer( *glProxy_ );

    // Misc
    new MagicOrdersInterface( this, controllers_, model_.actions_, staticModel_, simulation, *parameters_, profile_, *selector_ );
    new LogisticMagicInterface( this, controllers_, model_, staticModel_, simulation, profile_, *parameters_ );
    displayExtractor_.reset( new gui::DisplayExtractor( this ) );
    connect( displayExtractor_.get(), SIGNAL( LinkClicked( const QString& ) ), interpreter, SLOT( Interprete( const QString& ) ) );

    //Dialogs
    new Dialogs( this, controllers, staticModel, network_.GetMessageMgr(), model_.actions_, simulation, profile_, network.GetCommands(), config );
    addRasterDialog_.reset( new gui::AddRasterDialog( this ) );
    gui::ProfileDialog* profileDialog = new gui::ProfileDialog( this, controllers, profile_, *icons_, model_, model.profiles_ );
    IndicatorExportDialog* indicatorExportDialog = new IndicatorExportDialog( this );
    unitStateDialog_.reset( new UnitStateDialog( this, controllers, config, model.static_, model.actions_, simulation, filter, *displayExtractor_ ) );

    // Dock widgets
    dockContainer_.reset( new DockContainer( this, controllers_, staticModel, model, network_, simulation, config, filter,
                                             *parameters_, *profilerLayer, *automatsLayer, *formationLayer, *meteoLayer,
                                             *glProxy_, *factory, *strategy_, *symbols, *icons_, *indicatorExportDialog,
                                             simulationController, *drawingsBuilder_, *displayExtractor_, converter, *unitStateDialog_ ) );
    logger.SetLogger( dockContainer_->GetLoggerPanel() );
    connect( selector_.get(), SIGNAL( Widget2dChanged( gui::GlWidget* ) ), &dockContainer_->GetMiniView(), SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );

    // Tool bars
    AddToolBar( *this, new SIMControlToolbar( this, controllers, simulationController, network, dockContainer_->GetLoggerPanel() ), eModes_None, eModes_Default );
    AddToolBar( *this, new gui::DisplayToolbar( this, controllers ), eModes_Default );
    AddToolBar( *this, new EventToolbar( this, controllers, profile_ ), eModes_Default );
    AddToolBar( *this, new gui::GisToolbar( this, controllers, staticModel_.detection_, dockContainer_->GetTerrainProfiler() ), eModes_Default );
    AddToolBar( *this, new gui::LocationEditorToolbar( this, controllers_, staticModel.coordinateConverter_, *glProxy_, *locationsLayer ), eModes_Default );
    addToolBarBreak();
    AddToolBar( *this, new ReplayerToolbar( this, controllers, simulationController, network_.GetMessageMgr() ), eModes_Default | eModes_Gaming, eModes_Replay );
    AddToolBar( *this, new MessagePanel( this, controllers_, network_.GetMessageMgr(), network.GetCommands(), *factory ), eModes_Default );

    // Help
    gui::HelpSystem* help = new gui::HelpSystem( this, config_.BuildResourceChildFile( "help/gaming.xml" ), &controllers_.modes_, &profile_ );
    connect( this, SIGNAL( ShowHelp() ), help, SLOT( ShowHelp() ) );

    // Last layers
    CreateLayers( *locationsLayer, *meteoLayer, *profilerLayer, *automatsLayer, *formationLayer, simulation, *picker, elevation2d );

    // Menu bar & status bar
    setMenuBar( new Menu( this, controllers, staticModel_, *preferenceDialog_, *profileDialog, license, network_, logger ) );
    StatusBar* pStatus = new StatusBar( statusBar(), *picker, staticModel_.detection_, staticModel_.coordinateConverter_, controllers_, *selector_, &dockContainer_->GetProfilingPanel() );
    pStatus->SetModes( eModes_Default, eModes_None, true );

    // Initialize
    setCentralWidget( selector_.get() );
    setIcon( gui::Pixmap( tools::GeneralConfig::BuildResourceChildFile( "images/gui/logo32x32.png" ) ) );
    planifName_ = tr( "SWORD" ) + tr( " - Not connected" );
    setCaption( planifName_ );
    resize( 800, 600 );
    // Read settings
    controllers_.LoadOptions( eModes_Gaming );
    controllers_.modes_.LoadGeometry( eModes_Gaming );
    controllers_.ChangeMode( eModes_Default );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: MainWindow destructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    if( process_ )
        process_->kill();
    controllers_.Unregister( *this );
    dockContainer_.reset();
    glProxy_.reset();
    selector_.reset();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::CreateLayers
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void MainWindow::CreateLayers( gui::Layer& locationsLayer, gui::Layer& weather, gui::Layer& profilerLayer,
                               gui::Layer& automats, gui::Layer& formationLayer, const Simulation& simulation, gui::TerrainPicker& picker,
                               gui::Elevation2dLayer& elevation2dLayer )
{
    gui::TooltipsLayer& tooltipLayer = *new gui::TooltipsLayer( *glProxy_ );
    gui::Layer& terrainLayer         = *new gui::TerrainLayer( controllers_, *glProxy_, *graphicPreferences_, picker );
    gui::Layer& agents               = *new AgentsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile_, model_.actions_, simulation );
    gui::Layer& creationsLayer       = *new gui::MiscLayer< CreationPanels >( dockContainer_->GetCreationPanel() );
    gui::Layer& eventLayer           = *new gui::MiscLayer< EventDockWidget >( dockContainer_->GetEventDockWidget() );
    gui::Layer& raster               = *new gui::RasterLayer( controllers_.controller_ );
    gui::Layer* mapnik               = config_.HasMapnik() ? new gui::MapnikLayer( controllers_.controller_, config_.GetMapnikThreads() ) : 0;
    gui::Layer& watershed            = *new gui::WatershedLayer( controllers_, staticModel_.detection_ );
    gui::Layer& elevation3d          = *new gui::Elevation3dLayer( controllers_.controller_, staticModel_.detection_, *lighting_ );
    gui::Layer& resourceNetworksLayer = *new gui::ResourceNetworksLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile_ );
    gui::Layer& urbanLayer           = *new gui::UrbanLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile_ );
    gui::Layer& grid                 = *new gui::GridLayer( controllers_, *glProxy_, staticModel_.coordinateConverter_ );
    gui::Layer& metrics              = *new gui::MetricsLayer( staticModel_.detection_, *glProxy_ );
    gui::Layer& limits               = *new LimitsLayer( controllers_, *glProxy_, *strategy_, *parameters_, model_.tacticalLineFactory_, *glProxy_, profile_, *drawingsBuilder_ );
    gui::Layer& objectsLayer         = *new ObjectsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile_, model_.actions_, staticModel_, simulation, picker );
    gui::Layer& populations          = *new PopulationsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile_ );
    gui::Layer& inhabitants          = *new gui::InhabitantLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile_ );
    gui::Layer& agentKnowledges      = *new AgentKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile_ );
    gui::Layer& populationKnowledges = *new PopulationKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile_ );
    gui::Layer& objectKnowledges     = *new ObjectKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile_ );
    gui::Layer& defaultLayer         = *new gui::DefaultLayer( controllers_ );
    gui::Layer& teamLayer            = *new TeamLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile_, model_.actions_, staticModel_, simulation, network_.GetMessageMgr() );
    gui::Layer& fogLayer             = *new FogLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile_ );
    gui::Layer& drawerLayer          = *new gui::DrawerLayer( controllers_, *glProxy_, *strategy_, *parameters_, *glProxy_, profile_, *drawingsBuilder_ );
    gui::Layer& actionsLayer         = *new ActionsLayer( controllers_, *glProxy_ );
    gui::Layer& contour              = *new gui::ContourLinesLayer( controllers_, staticModel_.detection_ );

    // ordre de dessin
    AddLayer( defaultLayer );
    AddLayer( elevation2dLayer, "main,composition,miniviews", tr( "Elevation" ) );
    AddLayer( raster, "main,composition,miniviews", tr( "Raster" ) );
    if( mapnik )
        AddLayer( *mapnik, "main,composition,miniviews", tr( "Terrain (new rendering)" ) );
    AddLayer( terrainLayer, "main,composition,miniviews", tr( "Terrain" ) );
    AddLayer( contour, "main,composition,miniviews", tr( "Contour Lines" ) );
    AddLayer( urbanLayer, "main,miniviews", tr( "Urban blocks" ) );
    AddLayer( resourceNetworksLayer,"main", tr( "Resource networks" ) );
    AddLayer( watershed, "main,composition,miniviews", tr( "Watershed" ) );
    glProxy_->Register( elevation3d );
    AddLayer( grid, "main,miniviews" );
    AddLayer( weather, "main,miniviews", tr( "Weather" ) );
    AddLayer( limits, "main,miniviews" );
    AddLayer( objectKnowledges, "main,miniviews" );
    AddLayer( populationKnowledges, "main,miniviews" );
    AddLayer( agentKnowledges, "main,miniviews" );
    AddLayer( formationLayer, "main,miniviews", tr( "Formations" ) );
    AddLayer( teamLayer, "main,miniviews" );
    AddLayer( objectsLayer, "main,miniviews", tr( "Objects" ) );
    AddLayer( populations, "main,miniviews", tr( "Crowds" ) );
    AddLayer( inhabitants, "main,miniviews", tr( "Populations" ) );
    AddLayer( agents, "main,miniviews", tr( "Units" ) );
    AddLayer( automats, "main,miniviews", tr( "Automats" ) );
    AddLayer( actionsLayer, "main" );
    AddLayer( creationsLayer, "main" );
    AddLayer( eventLayer, "main,miniviews" );
    AddLayer( *parameters_, "main" );
    AddLayer( metrics, "main" );
    AddLayer( locationsLayer, "main" );
    AddLayer( profilerLayer, "main" );
    AddLayer( drawerLayer, "main,miniviews" );
    AddLayer( fogLayer, "fog" );
    AddTooltipLayer( tooltipLayer );

    // ordre des evenements
    forward_->Register( terrainLayer );
    forward_->Register( *parameters_ );
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
    forward_->Register( resourceNetworksLayer );
    forward_->Register( urbanLayer );
    forward_->Register( drawerLayer );
    forward_->Register( metrics );
    forward_->Register( elevation3d );
    forward_->Register( weather );

    if( tools::HasFeature( "pathfind" ) )
    {
        gui::Layer& pathfindLayer = *new PathfindLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile_, model_.publisher_, staticModel_.coordinateConverter_,
                                                        model_.agents_, model_.agents_, *drawingsBuilder_, model_.actions_ );
        connect( &dockContainer_->GetItineraryDockWidget(), SIGNAL( ItineraryAccepted() ), &pathfindLayer, SLOT( OnAcceptEdit() ) );
        connect( &dockContainer_->GetItineraryDockWidget(), SIGNAL( ItineraryRejected() ), &pathfindLayer, SLOT( OnRejectEdit() ) );
        AddLayer( pathfindLayer, "main" );
        forward_->Register( pathfindLayer );
        pathfindLayer.SetReadOnlyModes( 0 );
    }

    forward_->SetDefault( defaultLayer );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::AddLayer
// Created: LDC 2013-06-21
// -----------------------------------------------------------------------------
void MainWindow::AddLayer( gui::Layer& layer, const std::string& passes /*= ""*/, const QString& text /*= ""*/ )
{
    glProxy_->Register( layer );
    if( !text.isEmpty() )
        preferenceDialog_->AddLayer( text, layer );
    if( !passes.empty() )
        layer.SetPasses( passes );
    layer.SetReadOnlyModes( eModes_Itinerary );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::AddTooltipLayer
// Created: LDC 2013-06-21
// -----------------------------------------------------------------------------
void MainWindow::AddTooltipLayer( gui::TooltipsLayer_ABC& layer )
{
    glProxy_->Register( layer );
    layer.SetPasses( "tooltip" );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Load
// Created: AGE 2006-05-03
// -----------------------------------------------------------------------------
void MainWindow::Load()
{
    try
    {
        controllers_.SaveOptions( eModes_Gaming );
        unitStateDialog_->Purge();
        dockContainer_->Purge();
        if( lockMapViewController_.get() )
            lockMapViewController_->Clear();
        workers_.Terminate();
        model_.Purge();
        selector_->Close();
        selector_->Load();
        workers_.Initialize();
        staticModel_.Load( config_ );
        controllers_.LoadOptions( eModes_Gaming );
        unitStateDialog_->Load();
    }
    catch( const xml::exception& e )
    {
        Close();
        QMessageBox::critical( this, tr( "SWORD" ), tr( "Error loading exercise: " ) + tools::GetExceptionMsg( e ).c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Close
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void MainWindow::Close()
{
    controllers_.ChangeMode( eModes_Default );
    network_.Disconnect();
    parameters_->Reset();
    selector_->Close();
    unitStateDialog_->Purge();
    dockContainer_->Purge();
    if( lockMapViewController_.get() )
        lockMapViewController_->Clear();
    workers_.Terminate();
    model_.Purge();
    staticModel_.Purge();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::closeEvent
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::closeEvent( QCloseEvent* pEvent )
{
    Close();
    controllers_.modes_.SaveGeometry( eModes_Gaming );
    controllers_.SaveOptions( eModes_Gaming );
    QMainWindow::closeEvent( pEvent );
}

namespace
{
    struct SelectionStub{};

    QString ExtractExerciceName( const tools::Path& filename )
    {
        return QString::fromStdString( filename.Parent().FileName().ToUTF8() );
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::NotifyModeChanged
// Created: ABR 2013-02-15
// -----------------------------------------------------------------------------
void MainWindow::NotifyModeChanged( E_Modes newMode )
{
    ModesObserver_ABC::NotifyModeChanged( newMode );
    setCaption( ( newMode == eModes_Planning ) ? planifName_ + tr( " - Planning mode on" ) : planifName_ );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::NotifyUpdated
// Created: SBO 2007-03-20
// -----------------------------------------------------------------------------
void MainWindow::NotifyUpdated( const Simulation& simulation )
{
    static bool firstPass = true;
    const QString appName = tr( "SWORD" );
    const QString modePlanif =  tr( " - Planning mode on" );
    if( simulation.IsConnected() )
    {
        if( !connected_ && simulation.IsInitialized() )
        {
            // Legacy timeline need both the profile and the model to be initialized before we
            // can load the new timeline. When legacy timeline will be removed, this line could be
            // moved into Load()
            dockContainer_->Load();
            connected_ = true; // we update the caption until Model is totally loaded
            if( login_.isEmpty() )
                login_ = tools::translate( "LoginDialog", "Anonymous" );
        }
        QString planifName = appName + QString( " - [%1@%2][%3]" )
                            .arg( login_ )
                            .arg( simulation.GetSimulationHost().c_str() )
                            .arg( ExtractExerciceName( config_.GetExerciseFile() ));
        if( planifName_ != planifName )
            planifName_ = planifName;
    }
    else
    {
        planifName_ = appName + tr( " - Not connected" ) ;
        controllers_.actions_.Select( SelectionStub() );
        connected_ = false;
        login_ = "";
        Close();
    }
    setCaption( GetCurrentMode() == eModes_Planning ? planifName_ + modePlanif : planifName_ );

    if( simulation.IsInitialized() && firstPass )
    {
        QTimer::singleShot( 500, this, SLOT( OnGenerateSymbols() ) );
        firstPass = false;
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::NotifyUpdated
// Created: LGY 2014-02-07
// -----------------------------------------------------------------------------
void MainWindow::NotifyUpdated( const Simulation::Reconnection& reconnection )
{
    // we need to reset connected_ here to ensure dockContainer_->Load is called when reconnecting
    connected_ = false;
    network_.Reconnect();
    Load();
    network_.GetMessageMgr().Reconnect( reconnection.login_, reconnection.password_ );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::NotifyUpdated
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void MainWindow::NotifyUpdated( const ::Services& services )
{
    Load();
    if( services.HasService( sword::service_simulation ) )
        controllers_.ChangeMode( eModes_Gaming );
    else if( services.HasService( sword::service_replay ) )
        controllers_.ChangeMode( eModes_Replay );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::NotifyUpdated
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void MainWindow::NotifyUpdated( const Profile& profile )
{
    if( ! profile.IsLoggedIn() )
    {
        login_ = profile.GetLogin();
        static ConnectLoginDialog* dialog = new ConnectLoginDialog( this, profile, network_, controllers_ );
        // $$$$ AGE 2006-10-11: exec would create a reentrance...
        QTimer::singleShot( 0, dialog, SLOT( show() ) );
    }
    else
        login_ = profile.GetLogin();
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
    static QByteArray states_;

    if( states_.isNull() || states_.isEmpty() )
    {
        states_ = saveState();
        QList< QToolBar* > toolbars = qFindChildren< QToolBar* >( this , QString() );
        for( QList< QToolBar* >::iterator it = toolbars.begin(); it != toolbars.end(); ++it )
        {
            if( kernel::DisplayableModesObserver_ABC* observer = dynamic_cast< kernel::DisplayableModesObserver_ABC* >( *it ) )
                if( GetCurrentMode() & observer->GetVisibleModes() )
                    continue;
            if( ( *it )->parent() == this )
                ( *it )->hide();
        }
        QList< QDockWidget* > docks = qFindChildren< QDockWidget* >( this , QString() );
        for( QList< QDockWidget* >::iterator it = docks.begin(); it != docks.end(); ++it )
        {
            if( kernel::DisplayableModesObserver_ABC* observer = dynamic_cast< kernel::DisplayableModesObserver_ABC* >( *it ) )
                if( GetCurrentMode() & observer->GetVisibleModes() )
                    continue;
            ( *it )->hide();
        }
    }
    else if( restoreState( states_ ) )
        states_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MainWindow::OnAddRaster
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
void MainWindow::OnAddRaster()
{
    if( !config_.BuildTerrainChildFile( "config.xml" ).Exists() )
    {
        QMessageBox::warning( 0, tr( "Warning" ), tr( "This functionality is not available with old terrain format." ) );
        return;
    }
    try
    {
        QDialog::DialogCode result = static_cast< QDialog::DialogCode >( addRasterDialog_->exec() );
        if( result == QDialog::Accepted )
        {
            const auto input = tools::Path::FromUnicode( addRasterDialog_->GetFiles().toStdWString() );
            process_ = RunRasterApp( input, addRasterDialog_->GetPixelSize(), config_,
                [&]( int exitCode, const tools::Path& output, const std::string& error )
                {
                    if( !exitCode )
                    {
                        gui::RasterLayer& raster = *new gui::RasterLayer( controllers_.controller_, output );
                        raster.SetPasses( "main" );
                        selector_->AddLayer( raster );
                        preferenceDialog_->AddLayer( tr( "User layer [%1]" ).arg( addRasterDialog_->GetName() ), raster, true );
                    }
                    else
                        QMessageBox::warning( this, tr( "Error loading image file" ),
                            error.empty() ? tr( "Error while loading Raster source." ) : error.c_str() );
                } );
        }
    }
    catch( const geodata::ProjectionException& )
    {
        QMessageBox::information( this, tr( "Error loading image file" ), tr( "The following raster you add is missing spatial reference information.\nThis data can't be projected.") ) ;
        // Created: AME 2010-09-16 : TODO->allow user to set the projection in UI
    }
    catch( ... )
    {
        QMessageBox::critical( this, tr( "Error loading image file" ), tr( "Fatal error adding Raster source." ), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton );
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::OnGenerateSymbols
// Created: NPT 2013-05-15
// -----------------------------------------------------------------------------
void MainWindow::OnGenerateSymbols()
{
    icons_->GenerateSymbols( model_.teams_ );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::PlayPauseSoundControl
// Created: NPT 2013-07-22
// -----------------------------------------------------------------------------
void MainWindow::PlayPauseSoundControl( bool play )
{
    firePlayer_->PlayPauseSoundControl( play );
}


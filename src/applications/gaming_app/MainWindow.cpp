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
#include "VisionConesLayer.h"
#include "WeaponRangeLayer.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ModeController.h"
#include "clients_kernel/OptionsController.h"
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
#include "clients_gui/EntitySymbols.h"
#include "clients_gui/ExclusiveEventStrategy.h"
#include "clients_gui/GisToolbar.h"
#include "clients_gui/GlProxy.h"
#include "clients_gui/GlOptions.h"
#include "clients_gui/GlSelector.h"
#include "clients_gui/GradientPreferences.h"
#include "clients_gui/GridLayer.h"
#include "clients_gui/HelpSystem.h"
#include "clients_gui/HighlightColorModifier.h"
#include "clients_gui/ImageWrapper.h"
#include "clients_gui/InhabitantLayer.h"
#include "clients_gui/Layer.h"
#include "clients_gui/LayerComposite.h"
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

#include <boost/optional.hpp>
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
MainWindow::MainWindow( Controllers& controllers,
                        ::StaticModel& staticModel,
                        Model& model,
                        const Simulation& simulation,
                        SimulationController& simulationController,
                        Network& network,
                        ProfileFilter& filter,
                        GamingConfig& config,
                        LoggerProxy& logger,
                        const kernel::KnowledgeConverter_ABC& converter,
                        kernel::Workers& workers )
    : QMainWindow()
    , controllers_       ( controllers )
    , staticModel_       ( staticModel )
    , model_             ( model )
    , network_           ( network )
    , config_            ( config )
    , profile_           ( filter )
    , workers_           ( workers )
    , pColorController_  ( new ColorController( controllers_ ) )
    , connected_         ( false )
    , onPlanif_          ( false )
    , drawingsBuilder_   ( new DrawingsBuilder( controllers_, profile_ ) )
{
    controllers_.modes_.SetMainWindow( this );
    controllers_.modes_.AddRegistryEntry( eModes_Gaming, "Gaming" );
    controllers_.modes_.AddRegistryEntry( eModes_Replay, "Replayer" );
    controllers_.actions_.AddSelectionner( new Selectionner< actions::Action_ABC >() );
    controllers_.eventActions_.AddSelectionner( new Selectionner< gui::Event >() );
    gui::layers::CheckConsistency();

    glProxy_.reset( new gui::GlProxy( controllers, profile_, staticModel, model, std::make_shared< SimulationLighting >( controllers ) ) );

    // Text editor
    textEditor_.reset( new gui::TextEditor( this ) );

    // Strategy
    strategy_.reset( new gui::ColorStrategy( controllers, *glProxy_, *pColorController_ ) );
    strategy_->Add( std::unique_ptr< gui::ColorModifier_ABC >( new gui::SelectionColorModifier( controllers, *glProxy_, profile_ ) ) );
    strategy_->Add( std::unique_ptr< gui::ColorModifier_ABC >( new gui::HighlightColorModifier( controllers, profile_ ) ) );
    glProxy_->GetOptions().SetColorStrategy( *strategy_ ); // $$$$ MCO 2014-10-27: not that great...

    // Symbols
    gui::SymbolIcons* symbols = new gui::SymbolIcons( this, *glProxy_ );
    icons_.reset( new gui::EntitySymbols( *symbols, *strategy_ ) );

    // Event strategy
    forward_.reset( new gui::CircularEventStrategy( controllers_, *icons_, *strategy_, staticModel_.drawings_, *glProxy_ ) );
    eventStrategy_.reset( new gui::ExclusiveEventStrategy( *forward_ ) );

    // Main widget
    selector_.reset( new gui::GlSelector( this, *glProxy_, controllers, config, staticModel.detection_, *eventStrategy_, staticModel_.drawings_ ) );
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
    connect( selector_.get(), SIGNAL( UpdateGL() ), this, SLOT( OnUpdateGL() ) );

    //sound player
    firePlayer_.reset( new FirePlayer( controllers, profile_, simulation ) );

    // Misc
    LinkInterpreter* interpreter = new LinkInterpreter( this, controllers, filter );
    gui::RichItemFactory* factory = new  gui::RichItemFactory( this ); // $$$$ AGE 2006-05-11: aggregate somewhere
    connect( factory, SIGNAL( LinkClicked( const QString& ) ), interpreter, SLOT( Interprete( const QString& ) ) );

    lockMapViewController_.reset( new LockMapViewController( controllers, *glProxy_ ) );
    preferenceDialog_.reset( new gui::PreferencesDialog( this, controllers, staticModel, *glProxy_ ) );
    preferenceDialog_->AddPage( tr( "Orbat" ), *new gui::OrbatPanel( preferenceDialog_.get(), controllers.options_ ) );
    preferenceDialog_->AddPage( tr( "Sound" ), *new gui::SoundPanel( preferenceDialog_.get(), controllers.options_, *firePlayer_ ) );
    preferenceDialog_->AddPage( tr( "Weapon Ranges" ), *new gui::WeaponRangesPanel( preferenceDialog_.get(), controllers.options_, staticModel_ ) );
    new VisionConesToggler( controllers, simulationController, this );
    new CommandFacade( this, controllers_, config, network.GetCommands(), *interpreter, *glProxy_, filter, staticModel.coordinateConverter_ );
    new ClientCommandFacade( this, controllers_, network_.GetMessageMgr() );

    // First layers
    auto picker = new gui::TerrainPicker( this );
    auto parameters = std::make_shared< gui::ParametersLayer >( controllers_, *glProxy_, *textEditor_ );
    auto locationsLayer = std::make_shared< gui::LocationsLayer >( controllers_, *glProxy_ );
    auto meteoLayer = std::make_shared< WeatherLayer >( *glProxy_, *eventStrategy_, controllers_, model_.meteo_, *picker, profile_ );
    auto profilerLayer = std::make_shared< gui::TerrainProfilerLayer >( controllers_, *glProxy_ );

    // Misc
    new MagicOrdersInterface( this, controllers_, model_.actions_, staticModel_, simulation, parameters, profile_, *selector_ );
    new LogisticMagicInterface( this, controllers_, model_, staticModel_, simulation, profile_, *icons_ );
    displayExtractor_.reset( new gui::DisplayExtractor( this ) );
    connect( displayExtractor_.get(), SIGNAL( LinkClicked( const QString& ) ), interpreter, SLOT( Interprete( const QString& ) ) );

    //Dialogs
    new Dialogs( this, controllers, staticModel, network_.GetMessageMgr(), model_.actions_, simulation, profile_, network.GetCommands(), config, *strategy_, *pColorController_ );
    addRasterDialog_.reset( new gui::AddRasterDialog( this ) );
    gui::ProfileDialog* profileDialog = new gui::ProfileDialog( this, controllers, profile_, *icons_, model_, model.profiles_ );
    IndicatorExportDialog* indicatorExportDialog = new IndicatorExportDialog( this );
    unitStateDialog_.reset( new UnitStateDialog( this, controllers, config, model.static_, model.actions_, simulation, filter, *displayExtractor_ ) );

    // Dock widgets
    dockContainer_.reset( new DockContainer( this, controllers_, staticModel, model, network_, simulation, config, filter,
                                             parameters, profilerLayer, meteoLayer,
                                             *glProxy_, *factory, *strategy_, *symbols, *icons_, *indicatorExportDialog,
                                             simulationController, *drawingsBuilder_, *displayExtractor_, converter, *unitStateDialog_ ) );
    logger.SetLogger( dockContainer_->GetLoggerPanel() );
    connect( selector_.get(), SIGNAL( Widget2dChanged( gui::GlWidget* ) ), &dockContainer_->GetMiniView(), SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );

    // Tool bars
    AddToolBar( *this, new SIMControlToolbar( this, controllers, simulationController, network, dockContainer_->GetLoggerPanel() ), eModes_None, eModes_Default );
    AddToolBar( *this, new gui::DisplayToolbar( this, controllers ), eModes_Default );
    AddToolBar( *this, new EventToolbar( this, controllers, profile_ ), eModes_Default );
    AddToolBar( *this, new gui::GisToolbar( this, controllers, staticModel_.detection_, dockContainer_->GetTerrainProfiler() ), eModes_Default );
    AddToolBar( *this, new gui::LocationEditorToolbar( this, controllers_, staticModel.coordinateConverter_, *glProxy_, locationsLayer ), eModes_Default );
    addToolBarBreak();
    AddToolBar( *this, new ReplayerToolbar( this, controllers, simulationController, network_.GetMessageMgr() ), eModes_Default | eModes_Gaming, eModes_Replay );
    AddToolBar( *this, new MessagePanel( this, controllers_, network_.GetMessageMgr(), network.GetCommands(), *factory ), eModes_Default );

    // Help
    gui::HelpSystem* help = new gui::HelpSystem( this, config_.BuildResourceChildFile( "help/gaming.xml" ), &controllers_.modes_, &profile_ );
    connect( this, SIGNAL( ShowHelp() ), help, SLOT( ShowHelp() ) );

    // Last layers
    CreateLayers( parameters, locationsLayer, meteoLayer, profilerLayer, simulation, *picker );

    // Menu bar & status bar
    setMenuBar( new Menu( this, controllers, staticModel_, *preferenceDialog_, *profileDialog, network_, logger ) );
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
void MainWindow::CreateLayers( const std::shared_ptr< gui::ParametersLayer >& parameters,
                               const std::shared_ptr< gui::Layer_ABC >& locations,
                               const std::shared_ptr< gui::Layer_ABC >& weather,
                               const std::shared_ptr< gui::Layer_ABC >& profiler,
                               const Simulation& simulation,
                               gui::TerrainPicker& picker )
{
    gui::T_LayersMap layers;
    auto tooltips = std::make_shared< gui::TooltipsLayer >( controllers_, *glProxy_ );
    layers[ eLayerTypes_Actions ]                = std::make_shared< ActionsLayer >( controllers_, *glProxy_ );
    layers[ eLayerTypes_AgentKnowledges ]        = std::make_shared< AgentKnowledgesLayer >( controllers_, *glProxy_, *strategy_, profile_ );
    layers[ eLayerTypes_Agents ]                 = std::make_shared< AgentsLayer >( controllers_, *glProxy_, *strategy_, profile_, model_.actions_, simulation );
    layers[ eLayerTypes_Automats ]               = std::make_shared< AutomatsLayer >( controllers_, *glProxy_, *strategy_, profile_, model_.actions_ );
    layers[ eLayerTypes_ContourLines ]           = std::make_shared< gui::ContourLinesLayer >( controllers_, *glProxy_ );
    layers[ eLayerTypes_CrowdKnowledges ]        = std::make_shared< PopulationKnowledgesLayer >( controllers_, *glProxy_, *strategy_, profile_ );
    layers[ eLayerTypes_Creations ]              = std::make_shared< gui::MiscLayer< CreationPanels > >( controllers_, *glProxy_, eLayerTypes_Creations, dockContainer_->GetCreationPanel() );
    layers[ eLayerTypes_Crowds ]                 = std::make_shared< PopulationsLayer >( controllers_, *glProxy_, *strategy_, profile_ );
    layers[ eLayerTypes_Default ]                = std::make_shared< gui::DefaultLayer >( controllers_, *glProxy_ );
    layers[ eLayerTypes_Drawings ]               = std::make_shared< gui::DrawerLayer >( controllers_, *glProxy_, *strategy_, parameters, profile_, *drawingsBuilder_ );
    layers[ eLayerTypes_EventCreation ]          = std::make_shared< gui::MiscLayer< EventDockWidget > >( controllers_, *glProxy_, eLayerTypes_EventCreation, dockContainer_->GetEventDockWidget() );
    layers[ eLayerTypes_Fog ]                    = std::make_shared< FogLayer >( controllers_, *glProxy_, *strategy_, profile_ );
    layers[ eLayerTypes_Formations ]             = std::make_shared< FormationLayer >( controllers_, *glProxy_, *strategy_, profile_, model_.actions_, staticModel_ );
    layers[ eLayerTypes_Inhabitants ]            = std::make_shared< gui::InhabitantLayer >( controllers_, *glProxy_, *strategy_, profile_ );
    layers[ eLayerTypes_Elevation2d ]            = std::make_shared< gui::Elevation2dLayer >( controllers_, *glProxy_, staticModel_.detection_ );
    layers[ eLayerTypes_Elevation3d ]            = std::make_shared< gui::Elevation3dLayer >( controllers_, *glProxy_, staticModel_.detection_ );
    layers[ eLayerTypes_Grid ]                   = std::make_shared< gui::GridLayer >( controllers_, *glProxy_, staticModel_.coordinateConverter_ );
    layers[ eLayerTypes_Locations ]              = locations;
    if( config_.HasMapnik() )
        layers[ eLayerTypes_Mapnik ]             = std::make_shared< gui::MapnikLayer >( controllers_, *glProxy_, config_.GetMapnikThreads() );
    layers[ eLayerTypes_Metric ]                 = std::make_shared< gui::MetricsLayer >( controllers_, staticModel_.detection_, *glProxy_ );
    layers[ eLayerTypes_ObjectKnowledges ]       = std::make_shared< ObjectKnowledgesLayer >( controllers_, *glProxy_, *strategy_, profile_ );
    layers[ eLayerTypes_Objects ]                = std::make_shared< ObjectsLayer >( controllers_, *glProxy_, *strategy_, profile_, model_.actions_, staticModel_, simulation, picker );
    layers[ eLayerTypes_Parameters ]             = parameters;
    layers[ eLayerTypes_Parties ]                = std::make_shared< TeamLayer >( controllers_, *glProxy_, *strategy_, profile_, model_.actions_, staticModel_, simulation, network_.GetMessageMgr() );
    layers[ eLayerTypes_Pathfinds ]              = std::make_shared< PathfindLayer >( controllers_, *glProxy_, *strategy_, profile_, model_, *drawingsBuilder_ );
    layers[ eLayerTypes_Raster ]                 = std::make_shared< gui::RasterLayer >( controllers_, *glProxy_ );
    layers[ eLayerTypes_ResourceNetworks ]       = std::make_shared< gui::ResourceNetworksLayer >( controllers_, *glProxy_, *strategy_, profile_ );
    layers[ eLayerTypes_TacticalLines ]          = std::make_shared< LimitsLayer >( controllers_, *glProxy_, *strategy_, parameters, model_.tacticalLineFactory_, profile_, *drawingsBuilder_ );
    layers[ eLayerTypes_Terrain ]                = std::make_shared< gui::TerrainLayer >( controllers_, *glProxy_, picker );
    layers[ eLayerTypes_TerrainProfiler ]        = profiler;
    layers[ eLayerTypes_Tooltips ]               = tooltips;
    layers[ eLayerTypes_Urban ]                  = std::make_shared< gui::UrbanLayer >( controllers_, *glProxy_, *strategy_, profile_ );
    layers[ eLayerTypes_VisionCones ]            = std::make_shared< VisionConesLayer >( controllers_, *glProxy_, *strategy_, profile_ );
    layers[ eLayerTypes_Watershed ]              = std::make_shared< gui::WatershedLayer >( controllers_, *glProxy_, staticModel_.detection_ );
    layers[ eLayerTypes_WeaponRanges ]           = std::make_shared< WeaponRangeLayer >( controllers_, *glProxy_, *strategy_, profile_ );
    layers[ eLayerTypes_Weather ]                = weather;

    // composites, need others to be created
    layers[ eLayerTypes_CrowdsComposite ]        = std::make_shared< gui::LayerComposite >( controllers_, *glProxy_, layers, eLayerTypes_CrowdsComposite );
    layers[ eLayerTypes_ObjectsComposite ]       = std::make_shared< gui::LayerComposite >( controllers_, *glProxy_, layers, eLayerTypes_ObjectsComposite );
    layers[ eLayerTypes_TacticalLinesComposite ] = std::make_shared< gui::LayerComposite >( controllers_, *glProxy_, layers, eLayerTypes_TacticalLinesComposite );
    layers[ eLayerTypes_UnitsComposite ]         = std::make_shared< gui::LayerComposite >( controllers_, *glProxy_, layers, eLayerTypes_UnitsComposite );
    layers[ eLayerTypes_WeatherComposite ]       = std::make_shared< gui::LayerComposite >( controllers_, *glProxy_, layers, eLayerTypes_WeatherComposite );
    // init orders
    glProxy_->AddLayers( gui::layers::GetDefaultDrawingOrder( layers, eModes_AllGaming ) );
    forward_->AddLayers( gui::layers::GetEventOrder( layers, eModes_AllGaming ) );
    forward_->SetDefault( layers.at( eLayerTypes_Default ) );
    // init options
    controllers_.options_.InitializeLayers( gui::layers::GetDefaultConfigurableOrder( layers, eModes_AllGaming ) );
    // other init
    glProxy_->SetTooltipsLayer( tooltips );
    connect( &dockContainer_->GetItineraryDockWidget(), SIGNAL( ItineraryAccepted() ), layers.at( eLayerTypes_Pathfinds ).get(), SLOT( OnAcceptEdit() ) );
    connect( &dockContainer_->GetItineraryDockWidget(), SIGNAL( ItineraryRejected() ), layers.at( eLayerTypes_Pathfinds ).get(), SLOT( OnRejectEdit() ) );
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
        workers_.Terminate();
        model_.Purge();
        selector_->Close();
        workers_.Initialize();
        staticModel_.Load( config_ );
        dockContainer_->Load();
        controllers_.LoadOptions( eModes_Gaming );
        unitStateDialog_->Load();
        // will move to GLMainProxy
        auto& options = *controllers_.options_.GetViewOptions();
        glProxy_->UpdateLayerOrder( options );
        selector_->Load();
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
    selector_->Close();
    glProxy_->Purge();
    unitStateDialog_->Purge();
    dockContainer_->Purge();
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
    boost::optional< E_Modes > mode;
    if( services.HasService( sword::service_simulation ) )
        mode = eModes_Gaming;
    else if( services.HasService( sword::service_replay ) )
        mode = eModes_Replay;
    if( !mode )
        return;
    Load();
    controllers_.ChangeMode( *mode );
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
                        glProxy_->AddLayers( gui::T_LayersVector( 1, std::make_shared< gui::RasterLayer >( controllers_, *glProxy_, output, addRasterDialog_->GetName() ) ) );
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


// -----------------------------------------------------------------------------
// Name: MainWindow::OnUpdateGL
// Created: SLI 2014-06-16
// -----------------------------------------------------------------------------
void MainWindow::OnUpdateGL()
{
    model_.agents_.Resolver< Agent_ABC >::Apply( []( Agent_ABC& agent ){ agent.Get< kernel::Positions >().Compute(); } );
}

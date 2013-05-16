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
#include "ActionsLayer.h"
#include "AfterAction.h"
#include "AgentsLayer.h"
#include "AgentKnowledgesLayer.h"
#include "AutomatsLayer.h"
#include "ClientCommandFacade.h"
#include "CommandFacade.h"
#include "Config.h"
#include "CreationPanels.h"
#include "Dialogs.h"
#include "DockContainer.h"
#include "EventToolbar.h"
#include "FogLayer.h"
#include "FormationLayer.h"
#include "icons.h"
#include "IndicatorExportDialog.h"
#include "IndicatorPlotFactory.h"
#include "LimitsLayer.h"
#include "LinkInterpreter.h"
#include "LoggerProxy.h"
#include "ConnectLoginDialog.h"
#include "MagicOrdersInterface.h"
#include "Menu.h"
#include "MessagePanel.h"
#include "MissionPanel.h"
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
#include "UserProfileDialog.h"
#include "WeatherLayer.h"
#include "OrbatPanel.h"
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
#include "gaming/VisionConesToggler.h"
#include "gaming/ColorController.h"
#include "gaming/TeamsModel.h"
#include "clients_gui/AddRasterDialog.h"
#include "clients_gui/DisplayToolbar.h"
#include "clients_gui/GlSelector.h"
#include "clients_gui/Logger.h"
#include "clients_gui/MiscLayer.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/ImageWrapper.h"
#include "clients_gui/InhabitantLayer.h"
#include "clients_gui/ResourceNetworksLayer.h"
#include "clients_gui/PreferencesDialog.h"
#include "clients_gui/RichItemFactory.h"
#include "clients_gui/GlProxy.h"
#include "clients_gui/ColorStrategy.h"
#include "clients_gui/SelectionMenu.h"
#include "clients_gui/SelectionColorModifier.h"
#include "clients_gui/HighlightColorModifier.h"
#include "clients_gui/Elevation2dLayer.h"
#include "clients_gui/Elevation3dLayer.h"
#include "clients_gui/TerrainLayer.h"
#include "clients_gui/RasterLayer.h"
#include "clients_gui/RasterProcess.h"
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
#include "clients_gui/MiniViews.h"
#include "clients_gui/TerrainProfiler.h"
#include "geodata/ProjectionException.h"
#include "protocol/ReplaySenders.h"
#include "protocol/SimulationSenders.h"
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>

using namespace kernel;

namespace
{
    void AddToolBar( QMainWindow& mainWindow, gui::RichToolBar* toolBar, int hiddenModes = eModes_None, int visibleModes = eModes_None, bool visibleByDefault = true )
    {
        mainWindow.addToolBar( toolBar );
        toolBar->SetModes( hiddenModes, visibleModes, visibleByDefault );
    }

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
// Name: MainWindow constructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::MainWindow( Controllers& controllers, ::StaticModel& staticModel, Model& model, const Simulation& simulation, SimulationController& simulationController,
                        Network& network, const Profile_ABC& p, Config& config, LoggerProxy& logger, const QString& license )
    : QMainWindow()
    , controllers_     ( controllers )
    , staticModel_     ( staticModel )
    , model_           ( model )
    , network_         ( network )
    , config_          ( config )
    , pPainter_        ( new gui::ElevationPainter( staticModel_.detection_ ) )
    , pColorController_( new ColorController( controllers_ ) )
    , glProxy_         ( new gui::GlProxy() )
    , connected_       ( false )
    , onPlanif_        ( false )
    , pProfile_        ( new ProfileFilter( controllers, p ) )
    , icons_           ( 0 )
    , dockContainer_   ( 0 )
{
    controllers_.modes_.SetMainWindow( this );
    controllers_.modes_.AddRegistryEntry( eModes_Gaming, "Gaming" );
    controllers_.modes_.AddRegistryEntry( eModes_Replay, "Replayer" );

    // Strategy
    strategy_.reset( new gui::ColorStrategy( controllers, *glProxy_, *pColorController_ ) );
    strategy_->Add( std::auto_ptr< gui::ColorModifier_ABC >( new gui::SelectionColorModifier( controllers, *glProxy_, *pProfile_ ) ) );
    strategy_->Add( std::auto_ptr< gui::ColorModifier_ABC >( new gui::HighlightColorModifier( controllers, *pProfile_ ) ) );

    // Symbols
    gui::SymbolIcons* symbols = new gui::SymbolIcons( this, *glProxy_ );
    icons_.reset( new gui::EntitySymbols( *symbols, *strategy_ ) );

    // Event strategy
    forward_.reset( new gui::CircularEventStrategy( controllers_, *icons_, *strategy_, staticModel_.drawings_, *glProxy_ ) );
    eventStrategy_.reset( new gui::ExclusiveEventStrategy( *forward_ ) );

    // Main widget
    selector_.reset( new gui::GlSelector( this, *glProxy_, controllers, config, staticModel.detection_, *eventStrategy_ ) );
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

    // Misc
    lighting_.reset( new SimulationLighting( controllers, this ) );
    LinkInterpreter* interpreter = new LinkInterpreter( this, controllers, *pProfile_ );
    gui::RichItemFactory* factory = new  gui::RichItemFactory( this ); // $$$$ AGE 2006-05-11: aggregate somewhere
    connect( factory, SIGNAL( LinkClicked( const QString& ) ), interpreter, SLOT( Interprete( const QString& ) ) );

    gui::Elevation2dLayer& elevation2d = *new gui::Elevation2dLayer( controllers_.controller_, staticModel_.detection_ );
    preferenceDialog_.reset( new gui::PreferencesDialog( this, controllers, *lighting_, staticModel.coordinateSystems_, *pPainter_, *selector_, elevation2d ) );

    preferenceDialog_->AddPage( tr( "Orbat" ), *new OrbatPanel( preferenceDialog_.get(), controllers ) );
    new VisionConesToggler( controllers, network_.GetMessageMgr(), this );
    new CommandFacade( this, controllers_, config, network.GetCommands(), *interpreter, *glProxy_, *pProfile_ );
    new ClientCommandFacade( this, controllers_, network_.GetMessageMgr() );

    // First layers
    parameters_ = new gui::ParametersLayer( *glProxy_ );
    gui::LocationsLayer* locationsLayer      = new gui::LocationsLayer( *glProxy_ );
    gui::TerrainPicker* picker               = new gui::TerrainPicker( this );
    WeatherLayer* meteoLayer                 = new WeatherLayer( *glProxy_, *eventStrategy_, controllers_, model_.meteo_, *picker, *pProfile_ );
    AutomatsLayer* automatsLayer             = new AutomatsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, *pProfile_, model_.actions_, simulation, network_.GetMessageMgr(), model_.agents_ );
    FormationLayer* formationLayer           = new FormationLayer( controllers_, *glProxy_, *strategy_, *glProxy_, *pProfile_, model_.actions_, staticModel_, simulation, network_.GetMessageMgr(), model_.agents_ );
    gui::TerrainProfilerLayer* profilerLayer = new gui::TerrainProfilerLayer( *glProxy_ );

    // Misc
    new MagicOrdersInterface( this, controllers_, model_.actions_, staticModel_, simulation, *parameters_, *pProfile_, *selector_ );

    //Dialogs
    new Dialogs( this, controllers, model_, staticModel, network_.GetMessageMgr(), model_.actions_, simulation, *pProfile_, network.GetCommands(), config, *parameters_ );
    addRasterDialog_.reset( new gui::AddRasterDialog( this ) );
    UserProfileDialog* profileDialog = new UserProfileDialog( this, controllers, *pProfile_, *icons_, model_.userProfileFactory_ );
    IndicatorExportDialog* indicatorExportDialog = new IndicatorExportDialog( this );

    // Dock widgets
    dockContainer_.reset( new DockContainer( this, controllers_, staticModel, model, network_, simulation, config, *pProfile_,
                                             *parameters_, *profilerLayer, *automatsLayer, *formationLayer, *meteoLayer,
                                             *glProxy_, *factory, *interpreter, *strategy_, *symbols, *icons_, *indicatorExportDialog ) );
    logger.SetLogger( dockContainer_->GetLoggerPanel() );
    connect( selector_.get(), SIGNAL( Widget2dChanged( gui::GlWidget* ) ), &dockContainer_->GetMiniView(), SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );

    // Tool bars
    AddToolBar( *this, new SIMControlToolbar( this, controllers, simulationController, network, dockContainer_->GetLoggerPanel() ), eModes_None, eModes_Default );
    AddToolBar( *this, new gui::DisplayToolbar( this, controllers ), eModes_Default );
    AddToolBar( *this, new EventToolbar( this, controllers, *pProfile_ ), eModes_Default );
    AddToolBar( *this, new gui::GisToolbar( this, controllers, staticModel_.detection_, dockContainer_->GetTerrainProfiler() ), eModes_Default );
    AddToolBar( *this, new gui::LocationEditorToolbar( this, controllers_, staticModel.coordinateConverter_, *glProxy_, *locationsLayer ), eModes_Default );
    addToolBarBreak();
    AddToolBar( *this, new ReplayerToolbar( this, controllers, simulationController, network_.GetMessageMgr() ), eModes_Default | eModes_Gaming, eModes_Replay );
    AddToolBar( *this, new MessagePanel( this, controllers_, network_.GetMessageMgr(), network.GetCommands(), *factory ), eModes_Default );

    // Help
    gui::HelpSystem* help = new gui::HelpSystem( this, config_.BuildResourceChildFile( "help/gaming.xml" ) );
    connect( this, SIGNAL( ShowHelp() ), help, SLOT( ShowHelp() ) );

    // Last layers
    CreateLayers( *locationsLayer, *meteoLayer, *profilerLayer, *automatsLayer, *formationLayer, simulation, *picker, elevation2d );

    // Menu bar & status bar
    setMenuBar( new Menu( this, controllers, staticModel_, *preferenceDialog_, *profileDialog, *factory, license, network_, logger ) );
    StatusBar* pStatus = new StatusBar( statusBar(), *picker, staticModel_.detection_, staticModel_.coordinateConverter_, controllers_, *selector_, &dockContainer_->GetProfilingPanel() );
    pStatus->SetModes( eModes_Default, eModes_None, true );

    // Initialize
    setCentralWidget( selector_.get() );
    setAttribute( Qt::WA_DeleteOnClose, true );
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
    gui::Layer& terrainLayer         = *new gui::TerrainLayer( controllers_, *glProxy_, preferenceDialog_->GetPreferences(), picker );
    gui::Layer& agents               = *new AgentsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, *pProfile_, model_.actions_, simulation );
    gui::Layer& missionsLayer        = *new gui::MiscLayer< MissionPanel >( dockContainer_->GetMissionPanel() );
    gui::Layer& creationsLayer       = *new gui::MiscLayer< CreationPanels >( dockContainer_->GetCreationPanel() );
    gui::Layer& raster               = *new gui::RasterLayer( controllers_.controller_ );
    gui::Layer& watershed            = *new gui::WatershedLayer( controllers_, staticModel_.detection_ );
    gui::Layer& elevation3d          = *new gui::Elevation3dLayer( controllers_.controller_, staticModel_.detection_, *lighting_ );
    gui::Layer& resourceNetworksLayer = *new gui::ResourceNetworksLayer( controllers_, *glProxy_, *strategy_, *glProxy_, *pProfile_ );
    gui::Layer& urbanLayer           = *new gui::UrbanLayer( controllers_, *glProxy_, *strategy_, *glProxy_, *pProfile_ );
    gui::Layer& grid                 = *new gui::GridLayer( controllers_, *glProxy_ );
    gui::Layer& metrics              = *new gui::MetricsLayer( staticModel_.detection_, *glProxy_ );
    gui::Layer& limits               = *new LimitsLayer( controllers_, *glProxy_, *strategy_, *parameters_, model_.tacticalLineFactory_, *glProxy_, *pProfile_ );
    gui::Layer& objectsLayer         = *new ObjectsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, *pProfile_, model_.actions_, staticModel_, simulation, picker );
    gui::Layer& populations          = *new PopulationsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, *pProfile_ );
    gui::Layer& inhabitants          = *new gui::InhabitantLayer( controllers_, *glProxy_, *strategy_, *glProxy_, *pProfile_ );
    gui::Layer& agentKnowledges      = *new AgentKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, *pProfile_ );
    gui::Layer& populationKnowledges = *new PopulationKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, *pProfile_ );
    gui::Layer& objectKnowledges     = *new ObjectKnowledgesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, *pProfile_ );
    gui::Layer& defaultLayer         = *new gui::DefaultLayer( controllers_ );
    gui::Layer& logoLayer            = *new gui::LogoLayer( *glProxy_, gui::Image( config_.BuildResourceChildFile( "logo.png" ) ), 0.7f );
    gui::Layer& teamLayer            = *new TeamLayer( controllers_, *glProxy_, *strategy_, *glProxy_, *pProfile_, model_.actions_, staticModel_, simulation, network_.GetMessageMgr() );
    gui::Layer& fogLayer             = *new FogLayer( controllers_, *glProxy_, *strategy_, *glProxy_, *pProfile_ );
    gui::Layer& drawerLayer          = *new gui::DrawerLayer( controllers_, *glProxy_, *strategy_, *parameters_, *glProxy_, *pProfile_ );
    gui::Layer& actionsLayer         = *new ActionsLayer( controllers_, *glProxy_ );
    gui::Layer& contour              = *new gui::ContourLinesLayer( controllers_, staticModel_.detection_ );

    // ordre de dessin
    AddLayer( *glProxy_, *preferenceDialog_, defaultLayer );
    AddLayer( *glProxy_, *preferenceDialog_, elevation2dLayer, "main,composition,miniviews", tr( "Elevation" ) );
    AddLayer( *glProxy_, *preferenceDialog_, raster, "main,composition,miniviews", tr( "Raster" ) );
    AddLayer( *glProxy_, *preferenceDialog_, terrainLayer, "main,composition,miniviews", tr( "Terrain" ) );
    AddLayer( *glProxy_, *preferenceDialog_, contour, "main,composition,miniviews", tr( "Contour Lines" ) );
    AddLayer( *glProxy_, *preferenceDialog_, urbanLayer, "main,miniviews", tr( "Urban blocks" ) );
    AddLayer( *glProxy_, *preferenceDialog_, resourceNetworksLayer,"main", tr( "Resource networks" ) );
    AddLayer( *glProxy_, *preferenceDialog_, watershed, "main,composition,miniviews", tr( "Watershed" ) );
    glProxy_->Register( elevation3d );
    AddLayer( *glProxy_, *preferenceDialog_, grid, "main,miniviews" );
    AddLayer( *glProxy_, *preferenceDialog_, weather, "main,miniviews" );
    AddLayer( *glProxy_, *preferenceDialog_, limits, "main,miniviews" );
    AddLayer( *glProxy_, *preferenceDialog_, objectKnowledges, "main,miniviews" );
    AddLayer( *glProxy_, *preferenceDialog_, populationKnowledges, "main,miniviews" );
    AddLayer( *glProxy_, *preferenceDialog_, agentKnowledges, "main,miniviews" );
    AddLayer( *glProxy_, *preferenceDialog_, formationLayer, "main,miniviews", tr( "Formations" ) );
    AddLayer( *glProxy_, *preferenceDialog_, teamLayer, "main,miniviews" );
    AddLayer( *glProxy_, *preferenceDialog_, objectsLayer, "main,miniviews", tr( "Objects" ) );
    AddLayer( *glProxy_, *preferenceDialog_, populations, "main,miniviews", tr( "Crowds" ) );
    AddLayer( *glProxy_, *preferenceDialog_, inhabitants, "main,miniviews", tr( "Populations" ) );
    AddLayer( *glProxy_, *preferenceDialog_, agents, "main,miniviews", tr( "Units" ) );
    AddLayer( *glProxy_, *preferenceDialog_, automats, "main,miniviews", tr( "Automats" ) );
    AddLayer( *glProxy_, *preferenceDialog_, missionsLayer, "main,miniviews" );
    AddLayer( *glProxy_, *preferenceDialog_, actionsLayer, "main" );
    AddLayer( *glProxy_, *preferenceDialog_, creationsLayer, "main" );
    AddLayer( *glProxy_, *preferenceDialog_, *parameters_, "main" );
    AddLayer( *glProxy_, *preferenceDialog_, metrics, "main" );
    AddLayer( *glProxy_, *preferenceDialog_, locationsLayer, "main" );
    AddLayer( *glProxy_, *preferenceDialog_, profilerLayer, "main" );
    AddLayer( *glProxy_, *preferenceDialog_, drawerLayer, "main,miniviews" );
    AddLayer( *glProxy_, *preferenceDialog_, fogLayer, "fog" );
    AddLayer( *glProxy_, *preferenceDialog_, tooltipLayer, "tooltip" );
    AddLayer( *glProxy_, *preferenceDialog_, logoLayer, "main", tr( "Logo" ) );

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
    forward_->Register( urbanLayer );
    forward_->Register( drawerLayer );
    forward_->Register( metrics );
    forward_->Register( elevation3d );
    forward_->Register( weather );
    forward_->SetDefault( defaultLayer );
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
        dockContainer_->Purge();
        dockContainer_->Load();
        model_.Purge();
        selector_->Close();
        selector_->Load();
        staticModel_.Load( config_ );
        controllers_.LoadOptions( eModes_Gaming );
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
    dockContainer_->Purge();
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
        if( !filename.Exists() )
            return "";
        tools::Xifstream xis( filename );
        xis >> xml::start( "exercise" )
                >> xml::optional >> xml::start( "meta" );
        return filename.Parent().FileName().ToUTF8().c_str();
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
            if( profile_.isEmpty() )
                profile_ = tools::translate( "LoginDialog", "Anonymous" );
        }
        QString planifName = appName + QString( " - [%1@%2][%3]" )
                            .arg( profile_ )
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
        profile_ = "";
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
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void MainWindow::NotifyUpdated( const Services& services )
{
    Load();
    if( services.HasService< simulation::Service >() )
        controllers_.ChangeMode( eModes_Gaming );
    else if( services.HasService< replay::Service >() )
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
            (*it)->hide();
        }
        QList< QDockWidget* > docks = qFindChildren< QDockWidget* >( this , QString() );
        for( QList< QDockWidget* >::iterator it = docks.begin(); it != docks.end(); ++it )
        {
            if( kernel::DisplayableModesObserver_ABC* observer = dynamic_cast< kernel::DisplayableModesObserver_ABC* >( *it ) )
                if( GetCurrentMode() & observer->GetVisibleModes() )
                    continue;
            (*it)->hide();
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
    try
    {
        if( !config_.BuildTerrainChildFile( "config.xml" ).Exists() )
        {
            QMessageBox::warning( 0, tr( "Warning" ), tr( "This functionality is not available with old terrain format." ) );
            return;
        }

        QDialog::DialogCode result = static_cast< QDialog::DialogCode >( addRasterDialog_->exec() );
        if( result == QDialog::Accepted )
        {
            auto input = tools::Path::FromUnicode( addRasterDialog_->GetFiles().toStdWString() );
            auto callback = boost::bind( &MainWindow::OnRasterProcessExited,
                    this, _1, _2 );
            process_ = RunRasterApp( input, addRasterDialog_->GetPixelSize(),
                    config_, callback );
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
// Name: MainWindow::OnRasterProcessExited
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
void MainWindow::OnRasterProcessExited( int exitCode, const tools::Path& output )
{
    if( !exitCode )
    {
        gui::RasterLayer& raster = *new gui::RasterLayer( controllers_.controller_,
                output.FileName().ToUTF8() );
        raster.SetPasses( "main" );
        selector_->AddLayer( raster );
        preferenceDialog_->AddLayer( tr( "User layer [%1]" ).arg( addRasterDialog_->GetName() ), raster, true );
        raster.NotifyUpdated( kernel::ModelLoaded( config_ ) );
        raster.GenerateTexture();
    }
    else
        QMessageBox::warning( this, tr( "Error loading image file" ), tr( "Error while loading Raster source." ) );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::OnGenerateSymbols
// Created: NPT 2013-05-15
// -----------------------------------------------------------------------------
void MainWindow::OnGenerateSymbols()
{
    icons_->GenerateSymbols( model_.teams_ );
}

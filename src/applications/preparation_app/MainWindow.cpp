// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "MainWindow.h"
#include "moc_MainWindow.cpp"
#include "AgentsLayer.h"
#include "Config.h"
#include "CommunicationListView.h"
#include "ExerciseCreationDialog.h"
#include "CreationPanels.h"
#include "Dialogs.h"
#include "ExerciseDialog.h"
#include "FileToolbar.h"
#include "FilterDialog.h"
#include "FilterDialogs.h"
#include "FilterOrbatReIndexer.h"
#include "IntelligencesLayer.h"
#include "LimitsLayer.h"
#include "LogisticListView.h"
#include "Menu.h"
#include "ModelBuilder.h"
#include "ModelConsistencyDialog.h"
#include "ObjectCreationPanel.h"
#include "InhabitantCreationPanel.h"
#include "ObjectListView.h"
#include "ObjectsLayer.h"
#include "PopulationListView.h"
#include "InhabitantListView.h"
#include "PopulationsLayer.h"
#include "PreparationProfile.h"
#include "ProfileDialog.h"
#include "ProfileWizardDialog.h"
#include "PropertiesPanel.h"
#include "ResourceNetworkDialog.h"
#include "SaveModelChecker.h"
#include "ScoreDialog.h"
#include "SuccessFactorDialog.h"
#include "TacticalListView.h"
#include "LivingAreaEditor.h"
#include "clients_gui/AutomatsLayer.h"
#include "clients_gui/CircularEventStrategy.h"
#include "clients_gui/ColorStrategy.h"
#include "clients_gui/DefaultLayer.h"
#include "clients_gui/DisplayToolbar.h"
#include "clients_gui/DrawerFactory.h"
#include "clients_gui/DrawerLayer.h"
#include "clients_gui/Elevation2dLayer.h"
#include "clients_gui/Elevation3dLayer.h"
#include "clients_gui/EntitySearchBox.h"
#include "clients_gui/EntitySymbols.h"
#include "clients_gui/ExclusiveEventStrategy.h"
#include "clients_gui/ExtensionsPanel.h"
#include "clients_gui/GlProxy.h"
#include "clients_gui/GlSelector.h"
#include "clients_gui/GisToolbar.h"
#include "clients_gui/GraphicPreferences.h"
#include "clients_gui/GridLayer.h"
#include "clients_gui/HelpSystem.h"
#include "clients_gui/HighlightColorModifier.h"
#include "clients_gui/IntelligenceList.h"
#include "clients_gui/LightingProxy.h"
#include "clients_gui/LocationEditorToolbar.h"
#include "clients_gui/LogisticList.h"
#include "clients_gui/InhabitantLayer.h"
#include "clients_gui/LocationsLayer.h"
#include "clients_gui/MetricsLayer.h"
#include "clients_gui/MiscLayer.h"
#include "clients_gui/OptionsPanel.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/PreferencesDialog.h"
#include "clients_gui/RasterLayer.h"
#include "clients_gui/RichItemFactory.h"
#include "clients_gui/SelectionColorModifier.h"
#include "clients_gui/Settings.h"
#include "clients_gui/SimpleFilter.h"
#include "clients_gui/StatusBar.h"
#include "clients_gui/SymbolIcons.h"
#include "clients_gui/TerrainLayer.h"
#include "clients_gui/TerrainPicker.h"
#include "clients_gui/TerrainProfilerLayer.h"
#include "clients_gui/TooltipsLayer.h"
#include "clients_gui/UrbanFilter.h"
#include "clients_gui/UrbanLayer.h"
#include "clients_gui/FormationLayer.h"
#include "clients_gui/resources.h"
#include "clients_gui/WatershedLayer.h"
#include "clients_gui/WeatherLayer.h"
#include "clients_gui/ContourLinesLayer.h"
#include "clients_gui/resources.h"
#include "clients_gui/ElevationPainter.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Options.h"
#include "frontend/commands.h"
#include "preparation/AgentsModel.h"
#include "preparation/FormationModel.h"
#include "preparation/IntelligencesModel.h"
#include "preparation/Model.h"
#include "preparation/ScoresModel.h"
#include "preparation/StaticModel.h"
#include "preparation/TeamsModel.h"
#include "preparation/Tools.h"
#include "preparation/ColorController.h"
#include "tools/ExerciseConfig.h"
#include "tools/SchemaWriter.h"
#include <iostream>
#include <tools/XmlCrc32Signature.h>
#include <graphics/DragMovementLayer.h>
#include <xeumeuleu/xml.hpp>
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::MainWindow( Controllers& controllers, StaticModel& staticModel, Model& model, Config& config, const QString& expiration )
    : QMainWindow( 0, 0, Qt::WDestructiveClose )
    , controllers_    ( controllers )
    , staticModel_    ( staticModel )
    , model_          ( model )
    , modelBuilder_   ( new ModelBuilder( controllers, model ) )
    , config_         ( config )
    , forward_        ( new CircularEventStrategy() )
    , eventStrategy_  ( new ExclusiveEventStrategy( *forward_ ) )
    , pPainter_       ( new ElevationPainter( staticModel_.detection_ ) )
    , simpleFilter_   ( new gui::SimpleFilter() )
    , urbanFilter_    ( new gui::UrbanFilter() )
    , colorController_( new ColorController( controllers_ ) )
    , glProxy_        ( 0 )
    , menu_           ( 0 )
    , fileToolBar_    ( 0 )
    , needsSaving_    ( false )
    , loading_        ( false )
    , progressDialog_ ( 0 )
{
    QSettings settings;
    settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
    settings.beginGroup( "/Gaming" );
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());

    if( config_.HasGenerateScores() || !config_.GetFolderToMigrate().empty() )
    {
        if( config_.HasGenerateScores() )
        {
            staticModel_.Load( config_ );
            LoadExercise();
        }
        if( !config_.GetFolderToMigrate().empty() )
            MigrateExercises();
        return;
    }

    // Progress dialog
    progressDialog_ = new QProgressDialog( this, Qt::SplashScreen );
    progressDialog_->setAutoClose( true );
    progressDialog_->setMinimum( 0 );
    progressDialog_->setMaximum( 100 );
    progressDialog_->setCancelButton( 0 );
    QLabel* label = new QLabel( "", this );
    label->setMargin( 5 );
    progressDialog_->setLabel( label );
    SetProgression( 0, tr( "Initialize data ..." ) );

    setIcon( QPixmap( tools::GeneralConfig::BuildResourceChildFile( "images/gui/logo32x32.png" ).c_str() ) );

    lighting_ = new LightingProxy( this );
    PreferencesDialog* prefDialog = new PreferencesDialog( this, controllers, *lighting_, staticModel_.coordinateSystems_, *pPainter_ );
    glProxy_ = new GlProxy();

    strategy_ = new ColorStrategy( controllers, *glProxy_, *colorController_ );
    strategy_->Add( std::auto_ptr< ColorModifier_ABC >( new SelectionColorModifier( controllers, *glProxy_ ) ) );
    strategy_->Add( std::auto_ptr< ColorModifier_ABC >( new HighlightColorModifier( controllers ) ) );

    QStackedWidget* centralWidget = new QStackedWidget();
    setCentralWidget( centralWidget );
    selector_ = new GlSelector( centralWidget, *glProxy_, controllers, config, staticModel.detection_, *eventStrategy_ );

    gui::SymbolIcons* symbols = new gui::SymbolIcons( this, *glProxy_ );
    connect( selector_, SIGNAL( Widget2dChanged( gui::GlWidget* ) ), symbols, SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );
    gui::EntitySymbols* icons = new gui::EntitySymbols( *symbols, *strategy_ );

    new Dialogs( this, controllers, staticModel, PreparationProfile::GetProfile(), *strategy_, *colorController_, *icons, config );

    RichItemFactory* factory = new RichItemFactory( this );
    ProfileDialog* profileDialog = new ProfileDialog( this, controllers, *factory, *icons, model_, staticModel_.extensions_ );
    ProfileWizardDialog* profileWizardDialog = new ProfileWizardDialog( this, model_, model_.profiles_ );

    // Agent list panel
    QDockWidget* pListDockWnd_ = new QDockWidget( "orbat", this );
    pListDockWnd_->setObjectName( "Orbat" );
    addDockWidget( Qt::LeftDockWidgetArea, pListDockWnd_ );
    QTabWidget* pListsTabWidget = new QTabWidget( this );
    {
        QTabWidget* pAgentsTabWidget = new QTabWidget( pListsTabWidget );
        Q3VBox* listsTabBox = new Q3VBox( pListsTabWidget );
        new EntitySearchBox< Agent_ABC >( listsTabBox, controllers );
        new ::TacticalListView( listsTabBox, controllers, *factory, *icons, *modelBuilder_, model_.formations_.levels_ );
        pAgentsTabWidget->addTab( listsTabBox, tr( "Tactical" ) );

        listsTabBox = new Q3VBox( pListsTabWidget );
        new EntitySearchBox< Agent_ABC >( listsTabBox, controllers );
        new ::CommunicationListView( listsTabBox, controllers, *factory, *icons, *modelBuilder_ );
        pAgentsTabWidget->addTab( listsTabBox, tr( "Communication" ) );

        pAgentsTabWidget->addTab( new gui::LogisticList< ::LogisticListView >( controllers, *factory, PreparationProfile::GetProfile(), *icons, *modelBuilder_ ), tr( "Logistic" ) );
        pListsTabWidget->addTab( pAgentsTabWidget, tr( "Units" ) );
    }
    {
        Q3VBox* listsTabBox = new Q3VBox( pListsTabWidget );
        new EntitySearchBox< Object_ABC >( listsTabBox, controllers );
        new ::ObjectListView( listsTabBox, controllers, *factory, *modelBuilder_ );
        pListsTabWidget->addTab( listsTabBox, tr( "Objects" ) );
    }
    {
        Q3VBox* listsTabBox = new Q3VBox( pListsTabWidget );
        new EntitySearchBox< Population_ABC >( listsTabBox, controllers );
        new ::PopulationListView( listsTabBox, controllers, *factory, *modelBuilder_ );
        pListsTabWidget->addTab( listsTabBox, tr( "Crowds" ) );
    }
    {
        Q3VBox* listsTabBox = new Q3VBox( pListsTabWidget );
        new EntitySearchBox< Inhabitant_ABC >( listsTabBox, controllers );
        new ::InhabitantListView( listsTabBox, controllers, *factory, *modelBuilder_ );
        pListsTabWidget->addTab( listsTabBox, tr( "Populations" ) );
        pListsTabWidget->addTab( new IntelligenceList( controllers, *factory, *icons, PreparationProfile::GetProfile() ), tr( "Intelligences" ) );
    }
    pListDockWnd_->setWindowTitle( tr( "ORBAT" ) );
    pListDockWnd_->setWidget( pListsTabWidget );

    // Properties panel
    {
        QDockWidget* pPropertiesDockWnd = new QDockWidget( "properties", this );
        pPropertiesDockWnd->setObjectName( "properties" );
        addDockWidget( Qt::RightDockWidgetArea, pPropertiesDockWnd );
        ::PropertiesPanel* propertiesPanel = new ::PropertiesPanel( pPropertiesDockWnd, controllers, model_, staticModel_ );
        pPropertiesDockWnd->setWidget( propertiesPanel );
        pPropertiesDockWnd->setWindowTitle( tr( "Properties" ) );
    }
    // ResourceNetwork panel
    {
        QDockWidget* pResourceWnd = new ResourceNetworkDialog( this, controllers, staticModel_, model_ );
        addDockWidget( Qt::LeftDockWidgetArea, pResourceWnd );
        pResourceWnd->hide();
    }
    // Extensions panel
    {
        pExtensionsPanel_ = new gui::ExtensionsPanel( this, controllers, staticModel_.extensions_, model.agents_, *factory, *icons, PreparationProfile::GetProfile(), "ExtensionsPanel" );
        addDockWidget( Qt::LeftDockWidgetArea, pExtensionsPanel_ );
        pExtensionsPanel_->hide();
    }

    // A few layers
    LocationsLayer* locationsLayer = new LocationsLayer( *glProxy_ );
    gui::LocationEditorToolbar* LocEditToolBar = new gui::LocationEditorToolbar( this, controllers_, staticModel_.coordinateConverter_, *glProxy_, *locationsLayer );
    ParametersLayer* paramLayer = new ParametersLayer( *glProxy_, *LocEditToolBar );
    ::AgentsLayer* agentsLayer = new ::AgentsLayer( controllers, *glProxy_, *strategy_, *glProxy_, model_, *modelBuilder_, PreparationProfile::GetProfile(), *simpleFilter_, this );
    gui::WeatherLayer* weatherLayer = new gui::WeatherLayer( *glProxy_, *eventStrategy_ );
    gui::TerrainProfilerLayer* profilerLayer = new gui::TerrainProfilerLayer( *glProxy_ );
    gui::TerrainPicker* picker = new gui::TerrainPicker( this );
    gui::TerrainLayer* terrainLayer = new gui::TerrainLayer( controllers_, *glProxy_, prefDialog->GetPreferences(), *picker );

    // Creation panel
    QDockWidget* pCreationDockWnd = new QDockWidget( "creation", this );
    pCreationDockWnd->setObjectName( "creation" );
    addDockWidget( Qt::RightDockWidgetArea, pCreationDockWnd );
    pCreationDockWnd->hide();
    pCreationPanel_ = new CreationPanels( pCreationDockWnd, controllers, staticModel_, model_, config_, *factory, *symbols, *strategy_, *paramLayer, *weatherLayer, *glProxy_ );
    pCreationDockWnd->setWidget( pCreationPanel_ );
    pCreationDockWnd->setWindowTitle( tr( "Creation" ) );

    // Dialogs
    QDialog* exerciseDialog = new ExerciseDialog( this, controllers, model.exercise_, config_, model.teams_.InfiniteDotations() );
    pScoreDialog_ = new ScoreDialog( this, controllers, *factory, model_.scores_, *paramLayer, staticModel_, config_, *glProxy_ );
    SuccessFactorDialog* successFactorDialog = new SuccessFactorDialog( this, controllers, model_.successFactors_, *factory, staticModel_.successFactorActionTypes_, model_.scores_ );
    fileToolBar_ = new FileToolbar( this, controllers );
    consistencyDialog_ = new ModelConsistencyDialog( this, model, staticModel_, controllers_.actions_ );
    LivingAreaEditor* pLivingAreaEditor = new LivingAreaEditor( this, controllers, *paramLayer, *glProxy_ );

    addToolBar( fileToolBar_ );
    addToolBar( new DisplayToolbar( this, controllers ) );
    addToolBar( new gui::GisToolbar( this, controllers, staticModel_.detection_, *profilerLayer ) );
    addToolBar( LocEditToolBar );

    // Menu
    gui::HelpSystem* help = new gui::HelpSystem( this, config_.BuildResourceChildFile( "help/preparation.xml" ) );
    menu_ = new Menu( this, controllers, *prefDialog, *profileDialog, *profileWizardDialog, *pScoreDialog_, *successFactorDialog, *exerciseDialog, *consistencyDialog_, *factory, expiration, *help );
    setMenuBar( menu_ );
    filterDialogs_ = new FilterDialogs( this, config_, model, *menu_ );

    // Layers
    CreateLayers( *pCreationPanel_, *paramLayer, *locationsLayer, *weatherLayer, *agentsLayer, *terrainLayer,
                  *profilerLayer, *prefDialog, PreparationProfile::GetProfile(), *picker, *pLivingAreaEditor );

    // Status bar
    StatusBar* pStatus = new StatusBar( statusBar(), *picker, staticModel_.detection_, staticModel_.coordinateConverter_ );
    connect( selector_, SIGNAL( MouseMove( const geometry::Point2f& ) ), pStatus, SLOT( OnMouseMove( const geometry::Point2f& ) ) );
    connect( selector_, SIGNAL( MouseMove( const geometry::Point3f& ) ), pStatus, SLOT( OnMouseMove( const geometry::Point3f& ) ) );
    controllers_.Register( *this );

    SetWindowTitle( false );
    EnableWorkspace( false );
    ReadOptions();

    if( bfs::exists( bfs::path( config_.GetExerciseFile(), bfs::native ) ) && Load() )
        LoadExercise();

    restoreState(settings.value("mainWindowState").toByteArray());
    SetProgression( 100, tr( "Loading complete" ) );
}

// -----------------------------------------------------------------------------
// Name: MainWindow destructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    controllers_.Unregister( *this );
    delete glProxy_;
    delete modelBuilder_;
}

// -----------------------------------------------------------------------------
// Name: MainWindow::CreateLayers
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void MainWindow::CreateLayers( const CreationPanels& creationPanels, ParametersLayer& parameters, gui::Layer_ABC& locations, 
                               gui::Layer_ABC& weather, ::AgentsLayer& agents, gui::TerrainLayer& terrain, gui::Layer_ABC& profilerLayer,
                               PreferencesDialog& preferences, const Profile_ABC& profile, gui::TerrainPicker& picker,
                               LivingAreaEditor& livingAreaEditor )
{
    TooltipsLayer_ABC& tooltipLayer     = *new TooltipsLayer( *glProxy_ );
    AutomatsLayer& automats             = *new AutomatsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, *simpleFilter_ );
    Layer_ABC& formation                = *new FormationLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, *simpleFilter_ );
    Layer_ABC& objectCreationLayer      = *new MiscLayer< ObjectCreationPanel >( creationPanels.GetObjectCreationPanel() );
    Layer_ABC& inhabitantCreationLayer  = *new MiscLayer< InhabitantCreationPanel >( creationPanels.GetInhabitantCreationPanel() );
    Layer_ABC& indicatorCreationLayer   = *new MiscLayer< ScoreDialog >( *pScoreDialog_ );
    Layer_ABC& livingAreaEditorLayer    = *new MiscLayer< LivingAreaEditor >( livingAreaEditor );
    Elevation2dLayer& elevation2d       = *new Elevation2dLayer( controllers_.controller_, staticModel_.detection_ );
    Layer_ABC& raster                   = *new RasterLayer( controllers_.controller_ );
    Layer_ABC& watershed                = *new WatershedLayer( controllers_, staticModel_.detection_ );
    Layer_ABC& elevation3d              = *new Elevation3dLayer( controllers_.controller_, staticModel_.detection_, *lighting_ );
    Layer_ABC& urbanLayer               = *new UrbanLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, *simpleFilter_ );
    Layer_ABC& grid                     = *new GridLayer( controllers_, *glProxy_ );
    Layer_ABC& metrics                  = *new MetricsLayer( staticModel_.detection_, *glProxy_ );
    Layer_ABC& intelligences            = *new ::IntelligencesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, model_.intelligences_, *simpleFilter_ );
    Layer_ABC& limits                   = *new LimitsLayer( controllers_, *glProxy_, *strategy_, parameters, *modelBuilder_, *glProxy_, *eventStrategy_, profile, *simpleFilter_ );
    Layer_ABC& objectsLayer             = *new ::ObjectsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, picker, *urbanFilter_ );
    Layer_ABC& populations              = *new ::PopulationsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, model_, profile, *simpleFilter_ );
    Layer_ABC& defaultLayer             = *new DefaultLayer( controllers_ );
    Layer_ABC& drawerLayer              = *new DrawerLayer( controllers_, *glProxy_, *strategy_, parameters, *glProxy_, profile, *simpleFilter_ );
    Layer_ABC& inhabitantLayer          = *new InhabitantLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, *simpleFilter_ );
    Layer_ABC& contour                  = *new ::ContourLinesLayer( controllers_ );

    // ordre de dessin
    glProxy_->Register( defaultLayer );
    glProxy_->Register( elevation2d );             preferences.AddLayer( tr( "Elevation" ), elevation2d );          elevation2d         .SetPasses( "main" );
    glProxy_->Register( raster );                  preferences.AddLayer( tr( "Raster" ), raster );                  raster              .SetPasses( "main" );
    glProxy_->Register( terrain );                 preferences.AddLayer( tr( "Terrain" ), terrain );                terrain             .SetPasses( "main" );
    glProxy_->Register( contour );                  preferences.AddLayer( tr( "Contour Lines" ), contour );         contour             .SetPasses( "main,composition,miniviews" );
    glProxy_->Register( urbanLayer );               /*preferences.AddLayer( tr( "Urban" ), urbanLayer );*/          urbanLayer          .SetPasses( "main" );
    glProxy_->Register( watershed );               preferences.AddLayer( tr( "Watershed" ), watershed );            watershed           .SetPasses( "main" );
    glProxy_->Register( elevation3d );
    glProxy_->Register( grid );                                                                                     grid                .SetPasses( "main" );
    glProxy_->Register( weather );                                                                                  weather             .SetPasses( "main" );
    glProxy_->Register( limits );                                                                                   limits              .SetPasses( "main" );
    glProxy_->Register( indicatorCreationLayer );
    glProxy_->Register( livingAreaEditorLayer );                                                                    livingAreaEditorLayer.SetPasses( "main" );
    glProxy_->Register( intelligences );            preferences.AddLayer( tr( "Intelligence" ), intelligences );    intelligences       .SetPasses( "main" );
    glProxy_->Register( inhabitantLayer );          preferences.AddLayer( tr( "Populations" ), inhabitantLayer );   inhabitantLayer     .SetPasses( "main" );
    glProxy_->Register( objectsLayer );             preferences.AddLayer( tr( "Objects" ), objectsLayer );          objectsLayer        .SetPasses( "main" );
    glProxy_->Register( populations );              preferences.AddLayer( tr( "Crowd" ), populations );             populations         .SetPasses( "main" );
    glProxy_->Register( agents );                   preferences.AddLayer( tr( "Units" ), agents );                  agents              .SetPasses( "main" );
    glProxy_->Register( automats );                 preferences.AddLayer( tr( "Automats" ), automats );             automats            .SetPasses( "main" );
    glProxy_->Register( formation );                preferences.AddLayer( tr( "Formations" ), formation );         formation           .SetPasses( "main" );
    glProxy_->Register( objectCreationLayer );                                                                      objectCreationLayer .SetPasses( "main" );
    glProxy_->Register( inhabitantCreationLayer );                                                                  inhabitantCreationLayer .SetPasses( "main" );
    glProxy_->Register( parameters );                                                                               parameters          .SetPasses( "main" );
    glProxy_->Register( metrics );                                                                                  metrics             .SetPasses( "main" );
    glProxy_->Register( locations );                                                                                locations           .SetPasses( "main" );
    glProxy_->Register( profilerLayer );                                                                            profilerLayer       .SetPasses( "main" );
    glProxy_->Register( drawerLayer );                                                                              drawerLayer         .SetPasses( "main" );
    glProxy_->Register( tooltipLayer );                                                                             tooltipLayer        .SetPasses( "tooltip" );

    // ordre des evenements
    forward_->Register( terrain );
    forward_->Register( parameters );
    forward_->Register( agents );
    forward_->Register( automats );
    forward_->Register( formation );
    forward_->Register( populations );
    forward_->Register( objectsLayer );
    forward_->Register( intelligences );
    forward_->Register( weather );
    forward_->Register( inhabitantLayer );
    forward_->Register( limits );
    forward_->Register( urbanLayer );
    forward_->Register( drawerLayer );
    forward_->Register( metrics );
    forward_->Register( elevation3d );
    forward_->SetDefault( defaultLayer );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::New
// Created: FDS 2010-11-03
// -----------------------------------------------------------------------------
void MainWindow::New()
{
    static ExerciseCreationDialog* exerciseCreationDialog_ = new ExerciseCreationDialog( this, config_ );
    if( exerciseCreationDialog_->exec() == QDialog::Accepted )
    {
        QString filename = exerciseCreationDialog_->GetFileName();
        DoLoad( filename );
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::DoLoad
// Created: LDC 2010-12-01
// -----------------------------------------------------------------------------
void MainWindow::DoLoad( QString filename )
{
    if( filename.isEmpty() )
        return;
    SetProgression( 0, tr( "Initialize data ..." ) );
    if( filename.startsWith( "//" ) )
        filename.replace( "/", "\\" );
    config_.LoadExercise( filename.ascii() );
    pCreationPanel_->Load( config_ );
    pScoreDialog_->Load();
    if( Load() )
    {
        SetWindowTitle( true );
        LoadExercise();
    }
    SetProgression( 100, tr( "Loading complete" ) );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::MigrateExercises
// Created: JSR 2011-09-07
// -----------------------------------------------------------------------------
void MainWindow::MigrateExercises()
{
    const bfs::path root = bfs::path( config_.GetFolderToMigrate(), bfs::native );
    if( ! bfs::exists( root ) )
        throw std::exception( ( "The folder " + config_.GetFolderToMigrate() + " does not exist" ).c_str() );

    bfs::recursive_directory_iterator end;
    for( bfs::recursive_directory_iterator it( root ); it != end; ++it )
    {
        const bfs::path dirPath = ( *it );
        const bfs::path child = dirPath / "exercise.xml";
        if( bfs::exists( child ) )
        {
            try
            {
                std::cout << "Loading exercise " << dirPath.leaf() << "...\n";
                config_.LoadExercise( child.file_string() );
                staticModel_.Load( config_ );
                LoadExercise();
                std::cout << "Saving exercise " << dirPath.leaf() << "...\n";
                needsSaving_ = true;
                Save();
            }
            catch( ... )
            {
                // NOTHING
            }
            model_.Purge();
            staticModel_.Purge();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Open
// Created: FDS 2010-11-03
// -----------------------------------------------------------------------------
void MainWindow::Open()
{
    if( model_.IsLoaded() && !CheckSaving())
        return;
    // Open exercise file dialog
    QString filename = Q3FileDialog::getOpenFileName( config_.GetExerciseFile().c_str(), "Exercise (exercise.xml)", this, 0, tr( "Load exercise definition file (exercise.xml)" ) );
    // Load exercise
    DoLoad( filename );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Load
// Created: AGE 2006-05-03
// -----------------------------------------------------------------------------
bool MainWindow::Load()
{
    try
    {
        SetProgression( 10, tr( "Loading configuration ..." ) );
        WriteOptions();
        model_.Purge();
        selector_->Close();
        selector_->Load();
        SetProgression( 20, tr( "Loading physical model ..." ) );
        staticModel_.Load( config_ );
        SetProgression( 50, tr( "Loading filters ..." ) );
        filterDialogs_->Purge();
        filterDialogs_->Load();
        SetProgression( 60, tr( "Loading options ..." ) );
        pExtensionsPanel_->hide();
        SetWindowTitle( false );
    }
    catch( xml::exception& e )
    {
        SetNeedsSaving( false );
        Close();
        QMessageBox::critical( this, tools::translate( "Application", "SWORD" )
                                   , ( tr( "Error reading xml file: " ) + e.what() ).ascii() );
        return false;
    }
    ReadOptions();
    return true;
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Close
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void MainWindow::Close()
{
    if( model_.IsLoaded() && !CheckSaving() )
        return;
    model_.Purge();
    staticModel_.Purge();
    selector_->Close();
    filterDialogs_->Purge();
    SetWindowTitle( false );
    EnableWorkspace( false );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::EnableWorkspace
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
void MainWindow::EnableWorkspace( bool enabled )
{
    QList< QDockWidget* > docks = qFindChildren< QDockWidget* >( this , QString() );
    for( QList< QDockWidget* >::iterator it = docks.begin(); it != docks.end(); ++it )
        if( *it != dynamic_cast< QWidget* >( fileToolBar_ ) )
            (*it)->setEnabled( enabled );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::LoadExercise
// Created: SBO 2008-08-21
// -----------------------------------------------------------------------------
void MainWindow::LoadExercise()
{
    try
    {
        loading_ = true;
        std::string loadingErrors;
        SetProgression( 70, tr( "Loading exercise ..." ) );
        model_.Load( config_, loadingErrors );
        if( config_.HasGenerateScores() )
        {
            model_.scores_.GenerateScoresFromTemplate( config_.GetLoader() );
            const tools::SchemaWriter schemaWriter;
            SaveModelChecker checker( this );
            if( model_.scores_.CheckValidity( checker, schemaWriter ) )
            {
                model_.scores_.Serialize( config_.GetScoresFile(), schemaWriter );
                tools::WriteXmlCrc32Signature( config_.GetScoresFile() );
            }
            return;
        }
        loading_ = false;
        bool errors = !loadingErrors.empty();
        SetWindowTitle( errors );
        if( errors )
            QMessageBox::critical( this, tools::translate( "Application", "SWORD" )
                , tr( "The following entities cannot be loaded: " ) + "\n" + loadingErrors.c_str() );
        EnableWorkspace( true );
        CheckConsistency();
    }
    catch( std::exception& e )
    {
        SetNeedsSaving( false );
        Close();
        QMessageBox::critical( this, tools::translate( "Application", "SWORD" )
                                   , ( tr( "Error loading exercise: " ) + e.what() ).ascii() );
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::ReloadExercise
// Created: ABR 2011-06-24
// -----------------------------------------------------------------------------
void MainWindow::ReloadExercise()
{
    Close();
    DoLoad( config_.GetExerciseFile().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Save
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
bool MainWindow::Save()
{
    bool result = false;
    if( needsSaving_ )
    {
        SaveModelChecker checker( this );
        result = model_.Save( config_, checker );
        if( result )
        {
            SetWindowTitle( false );
            CheckConsistency();
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: MainWindow::SaveAs
// Created: ABR 2011-04-13
// -----------------------------------------------------------------------------
void MainWindow::SaveAs()
{
    bool exist = false;
    QString name;
    bfs::path exerciseDirectory;
    do
    {
        bool ok = false;
        name = QInputDialog::getText( tr( "Save exercise as ..." ),
                                      ( exist ) ? tr( "The exercise '%1' already exist. Please, enter a new exercise name:" ).arg( name ) : tr( "Enter an exercise name:" ),
                                      QLineEdit::Normal, tr( "Type exercise name here" ), &ok, this );
        if( ok && !name.isEmpty() )
        {
            exerciseDirectory = bfs::path( config_.GetExercisesDir(), bfs::native ) / name.ascii();
            exist = frontend::commands::ExerciseExists( config_, name.ascii() ) || bfs::exists( exerciseDirectory );
        }
        else
            return;
    } while( exist );
    bfs::create_directories( exerciseDirectory );
    bfs::path exerciseFile( config_.tools::ExerciseConfig::GeneralConfig::GetExerciseFile( name.ascii() ) );
    bfs::copy_file( config_.GetExerciseFile(), exerciseFile );
    config_.LoadExercise( exerciseFile.string() );
    needsSaving_ = true;
    Save();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::closeEvent
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::closeEvent( QCloseEvent* pEvent )
{
    if( !CheckSaving() )
    {
        pEvent->ignore();
        return;
    }
    QSettings settings;
    settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
    settings.beginGroup( "/Gaming" );
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());

    WriteOptions();

    QMainWindow::closeEvent( pEvent );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::CheckConsistency
// Created: ABR 2011-09-23
// -----------------------------------------------------------------------------
void MainWindow::CheckConsistency( unsigned int filters /*= 0*/ )
{
    if( !consistencyDialog_ )
        return;
    if( filters )
        consistencyDialog_->CheckConsistency( filters );
    else
        consistencyDialog_->CheckConsistency();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::WriteOptions
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::WriteOptions()
{
    Settings settings;
    settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
    settings.beginGroup( "/Preparation/Options" );
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
    settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
    settings.beginGroup( "/Preparation/Options" );
    controllers_.options_.Load( settings );
    settings.endGroup();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::NotifyCreated
// Created: SBO 2006-11-21
// -----------------------------------------------------------------------------
void MainWindow::NotifyCreated()
{
    if( !loading_ )
        SetWindowTitle( true );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::NotifyUpdated
// Created: SBO 2006-11-21
// -----------------------------------------------------------------------------
void MainWindow::NotifyUpdated()
{
    if( !loading_ )
        SetWindowTitle( true );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::NotifyDeleted
// Created: SBO 2006-11-21
// -----------------------------------------------------------------------------
void MainWindow::NotifyDeleted()
{
    if( !loading_ )
        SetWindowTitle( true );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::SetWindowTitle
// Created: SBO 2006-11-21
// -----------------------------------------------------------------------------
void MainWindow::SetWindowTitle( bool needsSaving )
{
    SetNeedsSaving( needsSaving );
    QString filename = tr( "No file loaded" );
    if( model_.IsLoaded() )
    {
        filename = model_.GetName().isEmpty() ? tr( "New ORBAT" ) : model_.GetName();
        if( needsSaving )
            filename += "*";
    }
    setCaption( tr( "Preparation - [%1]" ).arg( filename ) );
    if( menu_ && fileToolBar_ )
    {
        menu_->EnableSaveItem( needsSaving );
        fileToolBar_->EnableSaveItem( needsSaving );
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::SetNeedsSaving
// Created: RPD 2010-01-29
// -----------------------------------------------------------------------------
void MainWindow::SetNeedsSaving( bool status )
{
    needsSaving_ = status;
}

// -----------------------------------------------------------------------------
// Name: MainWindow::CheckSaving
// Created: RPD 2010-02-01
// -----------------------------------------------------------------------------
bool MainWindow::CheckSaving()
{
    if( needsSaving_ )
    {
        int result = QMessageBox::question( this, tools::translate( "Application", "SWORD" )
                                                , tr( "Save modifications?" )
                                                , QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel );
        if( ( result == QMessageBox::Yes && !Save() ) || result == QMessageBox::Cancel )
            return false;
    }
    return true;
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
// Name: MainWindow::SetProgression
// Created: ABR 2011-09-20
// -----------------------------------------------------------------------------
void MainWindow::SetProgression( int value, const QString& text )
{
    if( !progressDialog_ )
        return;
    if( !value )
        progressDialog_->show();
    progressDialog_->setLabelText( text );
    progressDialog_->setValue( value );
    qApp->processEvents();
}

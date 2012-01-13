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
#include "UrbanLayer.h"
#include "Config.h"
#include "InhabitantLayer.h"
#include "ExerciseCreationDialog.h"
#include "Dialogs.h"
#include "ExerciseDialog.h"
#include "ExtensionsPanel.h"
#include "FileToolbar.h"
#include "FilterDialog.h"
#include "FilterDialogs.h"
#include "FilterOrbatReIndexer.h"
#include "GhostsLayer.h"
#include "LimitsLayer.h"

#include "Menu.h"
#include "ModelBuilder.h"
#include "ModelConsistencyDialog.h"
#include "ObjectCreationPanel.h"
#include "InhabitantCreationPanel.h"
#include "ObjectsLayer.h"
#include "PopulationsLayer.h"
#include "PreparationProfile.h"
#include "ProfileDialog.h"
#include "ProfileWizardDialog.h"
#include "PropertiesPanel.h"
#include "ResourceNetworkDialog.h"
#include "ScoreDialog.h"
#include "SuccessFactorDialog.h"
#include "DockManager.h"
#include "LocationEditorToolbar.h"
#include "LivingAreaPanel.h"
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
#include "clients_gui/GlProxy.h"
#include "clients_gui/GlSelector.h"
#include "clients_gui/GisToolbar.h"
#include "clients_gui/GraphicPreferences.h"
#include "clients_gui/GridLayer.h"
#include "clients_gui/HelpSystem.h"
#include "clients_gui/HighlightColorModifier.h"
#include "clients_gui/LightingProxy.h"
#include "clients_gui/LogisticList.h"
#include "clients_gui/InhabitantLayer.h"
#include "clients_gui/LocationsLayer.h"
#include "clients_gui/MetricsLayer.h"
#include "clients_gui/MiscLayer.h"
#include "clients_gui/OverFlyingColorModifier.h"
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
#include "preparation/GhostModel.h"
#include "preparation/Model.h"
#include "preparation/ScoresModel.h"
#include "preparation/StaticModel.h"
#include "preparation/TeamsModel.h"
#include "clients_kernel/Tools.h"
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
MainWindow::MainWindow( Controllers& controllers, ::StaticModel& staticModel, Model& model, Config& config, const QString& expiration )
    : QMainWindow( 0, 0, Qt::WDestructiveClose )
    , controllers_      ( controllers )
    , staticModel_      ( staticModel )
    , model_            ( model )
    , modelBuilder_     ( new ModelBuilder( controllers, model ) )
    , config_           ( config )
    , forward_          ( new CircularEventStrategy() )
    , eventStrategy_    ( new ExclusiveEventStrategy( *forward_ ) )
    , pPainter_         ( new ElevationPainter( staticModel_.detection_ ) )
    , simpleFilter_     ( new gui::SimpleFilter() )
    , urbanFilter_      ( new gui::UrbanFilter() )
    , colorController_  ( new ColorController( controllers_ ) )
    , glProxy_          ( new GlProxy() )
    , lighting_         ( new LightingProxy( this ) )
    , strategy_         ( new ColorStrategy( controllers, *glProxy_, *colorController_ ) )
    , pDockManager_     ( 0 )
    , menu_             ( 0 )
    , fileToolBar_      ( 0 )
    , needsSaving_      ( false )
    , loading_          ( false )
    , progressDialog_   ( 0 )
    , consistencyDialog_( 0 )
{
    QSettings settings;
    settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
    settings.beginGroup( "/Gaming" );
    restoreGeometry( settings.value("mainWindowGeometry").toByteArray() );

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
    setIcon( QPixmap( tools::GeneralConfig::BuildResourceChildFile( "images/gui/logo32x32.png" ).c_str() ) );

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

    strategy_->Add( std::auto_ptr< ColorModifier_ABC >( new SelectionColorModifier( controllers, *glProxy_ ) ) );
    strategy_->Add( std::auto_ptr< ColorModifier_ABC >( new HighlightColorModifier( controllers ) ) );
    strategy_->Add( std::auto_ptr< ColorModifier_ABC >( new OverFlyingColorModifier( controllers ) ) );

    QStackedWidget* centralWidget = new QStackedWidget();
    setCentralWidget( centralWidget );
    selector_ = new GlSelector( centralWidget, *glProxy_, controllers, config, staticModel.detection_, *eventStrategy_ );

    gui::SymbolIcons* symbols = new gui::SymbolIcons( this, *glProxy_ );
    connect( selector_, SIGNAL( Widget2dChanged( gui::GlWidget* ) ), symbols, SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );
    gui::EntitySymbols* icons = new gui::EntitySymbols( *symbols, *strategy_ );

    // Dialogs
    RichItemFactory* factory = new RichItemFactory( this );
    ProfileDialog* profileDialog = new ProfileDialog( this, controllers, *factory, *icons, model_, staticModel_.extensions_ );
    ProfileWizardDialog* profileWizardDialog = new ProfileWizardDialog( this, model_, model_.profiles_ );
    PreferencesDialog* prefDialog = new PreferencesDialog( this, controllers, *lighting_, staticModel_.coordinateSystems_, *pPainter_ );
    new Dialogs( this, controllers, staticModel, PreparationProfile::GetProfile(), *strategy_, *colorController_, *icons, config, model_.GetSymbolsFactory(), *symbols );

    // A few layers
    AutomatsLayer& automats = *new AutomatsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, PreparationProfile::GetProfile(), *simpleFilter_ );
    FormationLayer& formation = *new FormationLayer( controllers_, *glProxy_, *strategy_, *glProxy_, PreparationProfile::GetProfile(), *simpleFilter_ );

    LocationsLayer* locationsLayer = new LocationsLayer( *glProxy_ );
    ::LocationEditorToolbar* locEditToolBar = new ::LocationEditorToolbar( this, controllers_, staticModel_.coordinateConverter_, *glProxy_, *locationsLayer );
    ParametersLayer* paramLayer = new ParametersLayer( *glProxy_, *locEditToolBar );
    gui::WeatherLayer* weatherLayer = new gui::WeatherLayer( *glProxy_, *eventStrategy_ );
    gui::TerrainProfilerLayer* profilerLayer = new gui::TerrainProfilerLayer( *glProxy_ );
    gui::TerrainPicker* picker = new gui::TerrainPicker( this );
    gui::TerrainLayer* terrainLayer = new gui::TerrainLayer( controllers_, *glProxy_, prefDialog->GetPreferences(), *picker );

    // Dialogs
    QDialog* exerciseDialog = new ExerciseDialog( this, controllers, model.exercise_, config_ );
    pScoreDialog_ = new ScoreDialog( this, controllers, *factory, model_.scores_, *paramLayer, staticModel_, config_, *glProxy_ );
    SuccessFactorDialog* successFactorDialog = new SuccessFactorDialog( this, controllers, model_.successFactors_, *factory, staticModel_.successFactorActionTypes_, model_.scores_ );
    fileToolBar_ = new FileToolbar( this, controllers );
    consistencyDialog_ = new ModelConsistencyDialog( this, model, staticModel_, controllers_ );

    addToolBar( fileToolBar_ );
    addToolBar( new DisplayToolbar( this, controllers ) );
    addToolBar( new gui::GisToolbar( this, controllers, staticModel_.detection_, *profilerLayer ) );
    addToolBar( locEditToolBar );

    // Dock widgets
    pDockManager_.reset( new DockManager( this, controllers_, automats, formation, *icons, *modelBuilder_, *factory, model_, staticModel_, config_, *symbols, *strategy_, *paramLayer, *weatherLayer, *glProxy_ ) );

    // Menu
    gui::HelpSystem* help = new gui::HelpSystem( this, config_.BuildResourceChildFile( "help/preparation.xml" ) );
    menu_ = new Menu( this, controllers, *prefDialog, *profileDialog, *profileWizardDialog, *pScoreDialog_, *successFactorDialog, *exerciseDialog, *consistencyDialog_, *factory, expiration, *help );
    setMenuBar( menu_ );
    filterDialogs_ = new FilterDialogs( this, config_, model, *menu_, staticModel_.coordinateConverter_ );

    // Layers
    CreateLayers( *paramLayer, *locationsLayer, *weatherLayer, *terrainLayer,
                *profilerLayer, *prefDialog, PreparationProfile::GetProfile(), *picker, automats, formation );


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
}

// -----------------------------------------------------------------------------
// Name: MainWindow::CreateLayers
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void MainWindow::CreateLayers( gui::ParametersLayer& parameters, gui::Layer_ABC& locations,
                               gui::Layer_ABC& weather, gui::TerrainLayer& terrain, gui::Layer_ABC& profilerLayer,
                               gui::PreferencesDialog& preferences, const kernel::Profile_ABC& profile, gui::TerrainPicker& picker,
                               gui::AutomatsLayer& automats, gui::FormationLayer& formation )
{
    ::AgentsLayer& agents               = *new ::AgentsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, model_, PreparationProfile::GetProfile(), *simpleFilter_, this );
    TooltipsLayer_ABC& tooltipLayer     = *new TooltipsLayer( *glProxy_ );
    Layer_ABC& objectCreationLayer      = *new MiscLayer< ObjectCreationPanel >( pDockManager_->GetObjectCreationPanel() );
    Layer_ABC& inhabitantCreationLayer  = *new MiscLayer< InhabitantCreationPanel >( pDockManager_->GetInhabitantCreationPanel() );
    Layer_ABC& indicatorCreationLayer   = *new MiscLayer< ScoreDialog >( *pScoreDialog_ );
    Elevation2dLayer& elevation2d       = *new Elevation2dLayer( controllers_.controller_, staticModel_.detection_ );
    Layer_ABC& raster                   = *new RasterLayer( controllers_.controller_ );
    Layer_ABC& watershed                = *new WatershedLayer( controllers_, staticModel_.detection_ );
    Layer_ABC& elevation3d              = *new Elevation3dLayer( controllers_.controller_, staticModel_.detection_, *lighting_ );
    Layer_ABC& urbanLayer               = *new ::UrbanLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, *simpleFilter_ );
    Layer_ABC& grid                     = *new GridLayer( controllers_, *glProxy_ );
    Layer_ABC& metrics                  = *new MetricsLayer( staticModel_.detection_, *glProxy_ );
    Layer_ABC& limits                   = *new LimitsLayer( controllers_, *glProxy_, *strategy_, parameters, *modelBuilder_, *glProxy_, *eventStrategy_, profile, *simpleFilter_ );
    Layer_ABC& objectsLayer             = *new ::ObjectsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, picker, *urbanFilter_ );
    Layer_ABC& populations              = *new ::PopulationsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, model_, profile, *simpleFilter_ );
    Layer_ABC& ghosts                   = *new GhostsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, model_, profile, *simpleFilter_ );
    Layer_ABC& defaultLayer             = *new DefaultLayer( controllers_ );
    Layer_ABC& drawerLayer              = *new DrawerLayer( controllers_, *glProxy_, *strategy_, parameters, *glProxy_, profile, *simpleFilter_ );
    Layer_ABC& inhabitantLayer          = *new ::InhabitantLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, *simpleFilter_, pDockManager_->GetLivingAreaPanel() );
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
    glProxy_->Register( inhabitantLayer );          preferences.AddLayer( tr( "Populations" ), inhabitantLayer );   inhabitantLayer     .SetPasses( "main" );
    glProxy_->Register( objectsLayer );             preferences.AddLayer( tr( "Objects" ), objectsLayer );          objectsLayer        .SetPasses( "main" );
    glProxy_->Register( populations );              preferences.AddLayer( tr( "Crowd" ), populations );             populations         .SetPasses( "main" );
    glProxy_->Register( ghosts );                   preferences.AddLayer( tr( "Phantom" ), ghosts );                ghosts              .SetPasses( "main" );
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

    // events order
    forward_->Register( terrain );
    forward_->Register( parameters );
    forward_->Register( agents );
    forward_->Register( automats );
    forward_->Register( formation );
    forward_->Register( populations );
    forward_->Register( objectsLayer );
    forward_->Register( weather );
    forward_->Register( inhabitantLayer );
    forward_->Register( ghosts );
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
    // Open exercise file dialog
    QString filename = Q3FileDialog::getOpenFileName( config_.GetExerciseFile().c_str(), "Exercise (exercise.xml)", this, 0, tr( "Load exercise definition file (exercise.xml)" ) );
    if( filename.isEmpty() || !Close() )
        return;
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
        pDockManager_->Purge();
        model_.Purge();
        pScoreDialog_->Load();
        selector_->Close();
        selector_->Load();
        SetProgression( 20, tr( "Loading physical model ..." ) );
        staticModel_.Load( config_ );
        SetProgression( 50, tr( "Loading filters ..." ) );
        filterDialogs_->Purge();
        filterDialogs_->Load();
        SetProgression( 60, tr( "Loading options ..." ) );
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
bool MainWindow::Close()
{
    if( model_.IsLoaded() && !CheckSaving() )
        return false;
    pDockManager_->Purge();
    model_.Purge();
    staticModel_.Purge();
    selector_->Close();
    filterDialogs_->Purge();
    SetWindowTitle( false );
    EnableWorkspace( false );
    return true;
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
        SetProgression( 70, tr( "Loading exercise ..." ) );
        model_.Load( config_ );
        pDockManager_->Load();
        if( config_.HasGenerateScores() )
        {
            model_.scores_.GenerateScoresFromTemplate( config_.GetLoader() );
            const tools::SchemaWriter schemaWriter;
            if( model_.scores_.CheckValidity( schemaWriter ) )
            {
                model_.scores_.Serialize( config_.GetScoresFile(), schemaWriter );
                tools::WriteXmlCrc32Signature( config_.GetScoresFile() );
            }
            return;
        }
        loading_ = false;
        SetWindowTitle( !model_.GetLoadingErrors().empty() || model_.ghosts_.NeedSaving() );
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
    if( Close() )
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
        CheckConsistency();
        model_.Save( config_ );
        SetWindowTitle( false );
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
    model_.exercise_.SetName( name );
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
    settings.setValue( "mainWindowGeometry", pDockManager_->SaveGeometry() );
    settings.setValue( "mainWindowState", pDockManager_->SaveState() );

    WriteOptions();

    QMainWindow::closeEvent( pEvent );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::OnForceSaveAndAddActionPlanning
// Created: ABR 2011-10-18
// -----------------------------------------------------------------------------
void MainWindow::OnForceSaveAndAddActionPlanning( const std::string& filename )
{
    model_.exercise_.SetActionPlanning( filename );
    needsSaving_ = true;
    Save();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::CheckConsistency
// Created: ABR 2011-09-23
// -----------------------------------------------------------------------------
void MainWindow::CheckConsistency()
{
    if( !consistencyDialog_ )
        return;
    consistencyDialog_->CheckConsistency();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::ClearLoadingErrors
// Created: JSR 2012-01-05
// -----------------------------------------------------------------------------
void MainWindow::ClearLoadingErrors()
{
    model_.ClearLoadingErrors();
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
        filename = model_.exercise_.GetName().isEmpty() ? tr( "New ORBAT" ) : model_.exercise_.GetName();
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

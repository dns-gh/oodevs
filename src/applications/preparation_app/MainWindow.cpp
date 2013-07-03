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
#include "DialogContainer.h"
#include "DockContainer.h"
#include "ExtensionsPanel.h"
#include "FileToolbar.h"
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
#include "ScoreDialog.h"
#include "ToolbarContainer.h"
#include "LocationEditorToolbar.h"
#include "LivingAreaPanel.h"
#include "OrbatPanel.h"
#include "clients_gui/AddRasterDialog.h"
#include "clients_gui/AutomatsLayer.h"
#include "clients_gui/CircularEventStrategy.h"
#include "clients_gui/ColorStrategy.h"
#include "clients_gui/DefaultLayer.h"
#include "clients_gui/DrawerFactory.h"
#include "clients_gui/DrawerLayer.h"
#include "clients_gui/Elevation2dLayer.h"
#include "clients_gui/Elevation3dLayer.h"
#include "clients_gui/EntitySymbols.h"
#include "clients_gui/ExclusiveEventStrategy.h"
#include "clients_gui/GlProxy.h"
#include "clients_gui/GlSelector.h"
#include "clients_gui/GisToolbar.h"
#include "clients_gui/GridLayer.h"
#include "clients_gui/HelpSystem.h"
#include "clients_gui/HighlightColorModifier.h"
#include "clients_gui/LightingProxy.h"
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
#include "clients_gui/SelectionLayer.h"
#include "clients_gui/SelectionMenu.h"
#include "clients_gui/Settings.h"
#include "clients_gui/StatusBar.h"
#include "clients_gui/SymbolIcons.h"
#include "clients_gui/TerrainLayer.h"
#include "clients_gui/TerrainPicker.h"
#include "clients_gui/TerrainProfiler.h"
#include "clients_gui/TerrainProfilerLayer.h"
#include "clients_gui/TooltipsLayer.h"
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
#include "clients_kernel/ModeController_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"
#include "frontend/commands.h"
#include "frontend/CreateExercise.h"
#include "geodata/ProjectionException.h"
#include "geodata/ExtentException.h"
#include "preparation/AgentsModel.h"
#include "preparation/FormationModel.h"
#include "preparation/GhostModel.h"
#include "preparation/Exercise.h"
#include "preparation/Model.h"
#include "preparation/ScoresModel.h"
#include "preparation/StaticModel.h"
#include "preparation/TeamsModel.h"
#include "preparation/ColorController.h"
#include "tools/ExerciseConfig.h"
#include "tools/SchemaWriter.h"
#include "ENT/ENT_Tr_Gen.h"
#include <iostream>
#include <tools/XmlCrc32Signature.h>
#include <graphics/DragMovementLayer.h>
#include <xeumeuleu/xml.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::MainWindow( kernel::Controllers& controllers, StaticModel& staticModel, Model& model, Config& config, const QString& expiration )
    : QMainWindow( 0, 0, Qt::WDestructiveClose )
    , controllers_      ( controllers )
    , staticModel_      ( staticModel )
    , model_            ( model )
    , config_           ( config )
    , loading_          ( false )
    , needsSaving_      ( false )
    , terrainHasChanged_( false )
    , modelBuilder_     ( new ModelBuilder( controllers, model ) )
    , pPainter_         ( new gui::ElevationPainter( staticModel_.detection_ ) )
    , colorController_  ( new ColorController( controllers_ ) )
    , glProxy_          ( new gui::GlProxy() )
    , lighting_         ( new gui::LightingProxy( this ) )
    , strategy_         ( new gui::ColorStrategy( controllers, *glProxy_, *colorController_ ) )
    , dockContainer_    ( 0 )
    , dialogContainer_  ( 0 )
    , toolbarContainer_ ( 0 )
    , progressDialog_   ( 0 )
    , menu_             ( 0 )
    , icons_            ( 0 )
{
    controllers_.modes_->SetMainWindow( this );

    // Migration
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

    // Symbols
    gui::SymbolIcons* symbols = new gui::SymbolIcons( this, *glProxy_ );
    icons_.reset( new gui::EntitySymbols( *symbols, *strategy_ ) );
    gui::RichItemFactory* factory = new gui::RichItemFactory( this );

    // Event strategy
    forward_.reset( new gui::CircularEventStrategy( *icons_, *strategy_, staticModel_.drawings_, *glProxy_ ) );
    eventStrategy_.reset( new gui::ExclusiveEventStrategy( *forward_ ) );

    // Central Widget
    QStackedWidget* centralWidget = new QStackedWidget();
    setCentralWidget( centralWidget );
    selector_.reset( new gui::GlSelector( centralWidget, *glProxy_, controllers, config, staticModel.detection_, *eventStrategy_ ) );
    connect( selector_.get(), SIGNAL( Widget2dChanged( gui::GlWidget* ) ), symbols, SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );
    connect( selector_.get(), SIGNAL( Widget2dChanged( gui::GlWidget* ) ), forward_->GetSelectionMenu(), SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );

    // Strategy
    strategy_->Add( std::auto_ptr< gui::ColorModifier_ABC >( new gui::SelectionColorModifier( controllers, *glProxy_ ) ) );
    strategy_->Add( std::auto_ptr< gui::ColorModifier_ABC >( new gui::HighlightColorModifier( controllers ) ) );
    strategy_->Add( std::auto_ptr< gui::ColorModifier_ABC >( new gui::OverFlyingColorModifier( controllers ) ) );

    // Layer 1
    gui::LocationsLayer* locationsLayer = new gui::LocationsLayer( *glProxy_ );
    gui::ParametersLayer* paramLayer = new gui::ParametersLayer( *glProxy_ );
    gui::AutomatsLayer& automats = *new gui::AutomatsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, PreparationProfile::GetProfile() );
    gui::FormationLayer& formation = *new gui::FormationLayer( controllers_, *glProxy_, *strategy_, *glProxy_, PreparationProfile::GetProfile() );
    gui::WeatherLayer* weatherLayer = new gui::WeatherLayer( *glProxy_, *eventStrategy_ );
    gui::TerrainPicker* picker = new gui::TerrainPicker( this );
    gui::TerrainProfilerLayer* profilerLayer = new gui::TerrainProfilerLayer( *glProxy_ );

    // Dialogs
    dialogContainer_.reset( new DialogContainer( this, controllers, model_, staticModel, PreparationProfile::GetProfile(), *strategy_, *colorController_, *icons_, config, *symbols, *lighting_, *pPainter_, *factory, *paramLayer, *glProxy_, *selector_ ) );

    // ToolBars
    toolbarContainer_.reset( new ToolbarContainer( this, controllers, staticModel, *glProxy_, *locationsLayer, *eventStrategy_, *paramLayer, model_.urban_, dialogContainer_->GetRemoveBlocksDialog() ) );

    // Dock widgets
    dockContainer_.reset( new DockContainer( this, controllers_, automats, formation, *icons_, *modelBuilder_, *factory, model_, staticModel_, config_, *symbols, *strategy_, *paramLayer, *weatherLayer, *glProxy_, *colorController_, *profilerLayer ) );
    connect( toolbarContainer_->GetGisToolbar().GetTerrainProfilerButton(), SIGNAL( toggled( bool ) ), &dockContainer_->GetTerrainProfiler(), SLOT( setVisible( bool ) ) );
    connect( &dockContainer_->GetTerrainProfiler(), SIGNAL( visibilityChanged( bool ) ), toolbarContainer_->GetGisToolbar().GetTerrainProfilerButton(), SLOT( setOn( bool ) ) );

    // Layers 2
    CreateLayers( *paramLayer, *locationsLayer, *weatherLayer, *profilerLayer, PreparationProfile::GetProfile(), *picker, automats, formation );

    // Menu (must be created after DockWidgets and ToolBars for 'Windows' menu)
    help_.reset( new gui::HelpSystem( this, config_.BuildResourceChildFile( "help/preparation.xml" ) ) );
    menu_.reset( new Menu( this, controllers, *dialogContainer_, *factory, expiration, *help_ ) );
    toolbarContainer_->GetFileToolbar().Fill( *menu_ );
    setMenuBar( menu_.get() );

    // Status bar
    gui::StatusBar* pStatus = new gui::StatusBar( controllers_, statusBar(), *picker, staticModel_.detection_, staticModel_.coordinateConverter_ );
    pStatus->SetModes( ePreparationMode_Default );
    connect( selector_.get(), SIGNAL( MouseMove( const geometry::Point2f& ) ), pStatus, SLOT( OnMouseMove( const geometry::Point2f& ) ) );
    connect( selector_.get(), SIGNAL( MouseMove( const geometry::Point3f& ) ), pStatus, SLOT( OnMouseMove( const geometry::Point3f& ) ) );

    // Progress dialog
    progressDialog_.reset( new QProgressDialog( "", "", 0, 100, this, Qt::SplashScreen ) );
    progressDialog_->setAutoClose( true );
    progressDialog_->setModal( true );
    progressDialog_->setContentsMargins( 5, 5, 5, 5 );
    progressDialog_->setCancelButton( 0 );

    // Read settings
    ReadOptions();
    QSettings settings( "MASA Group", tools::translate( "Application", "SWORD" ) );
    settings.beginGroup( "/" + controllers_.modes_->GetRegisteryEntry() );
    restoreGeometry( settings.value( "mainWindowGeometry").toByteArray() );

    // Raster_app process
    process_.reset( new QProcess( this ) );
    connect( process_.get(), SIGNAL( finished( int, QProcess::ExitStatus ) ), this, SLOT( OnRasterProcessExited( int, QProcess::ExitStatus ) ) );

    // Initialize
    SetWindowTitle( false );
    controllers_.Register( *this );
    controllers_.ChangeMode( ePreparationMode_Default );
    setLocale( QLocale() );
    setMinimumSize( 800, 600 );
    setIcon( QPixmap( tools::GeneralConfig::BuildResourceChildFile( "images/gui/logo32x32.png" ).c_str() ) );

    // Load exercise IFN
    if( bfs::exists( bfs::path( config_.GetExerciseFile(), bfs::native ) ) )
    {
        SetProgression( 0, tr( "Initialize data ..." ) );
        if( Load() )
            LoadExercise();
        SetProgression( 100, tr( "Loading complete" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow destructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    controllers_.Unregister( *this );
    process_->kill();
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
        layer.SetReadOnlyModes( ePreparationMode_Terrain );
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::CreateLayers
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void MainWindow::CreateLayers( gui::ParametersLayer& parameters, gui::Layer& locations, gui::Layer& weather, gui::Layer& profilerLayer,
                               const kernel::Profile_ABC& profile, gui::TerrainPicker& picker, gui::AutomatsLayer& automats, gui::FormationLayer& formation )
{
    assert( dialogContainer_.get() && dockContainer_.get() );
    gui::PreferencesDialog& preferences     = dialogContainer_->GetPrefDialog();
    gui::Layer& terrain                 = *new gui::TerrainLayer( controllers_, *glProxy_, preferences.GetPreferences(), picker );
    ::AgentsLayer& agents                   = *new AgentsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, model_, *modelBuilder_, PreparationProfile::GetProfile(), this );
    gui::TooltipsLayer_ABC& tooltipLayer    = *new gui::TooltipsLayer( *glProxy_ );
    gui::Layer& objectCreationLayer     = *new gui::MiscLayer< ObjectCreationPanel >( dockContainer_->GetObjectCreationPanel() );
    gui::Layer& inhabitantCreationLayer = *new gui::MiscLayer< InhabitantCreationPanel >( dockContainer_->GetInhabitantCreationPanel() );
    gui::Layer& indicatorCreationLayer  = *new gui::MiscLayer< ScoreDialog >( dialogContainer_->GetScoreDialog() );
    gui::Elevation2dLayer& elevation2d      = *new gui::Elevation2dLayer( controllers_.controller_, staticModel_.detection_ );
    gui::Layer& raster                  = *new gui::RasterLayer( controllers_.controller_ );
    gui::Layer& watershed               = *new gui::WatershedLayer( controllers_, staticModel_.detection_ );
    gui::Layer& elevation3d             = *new gui::Elevation3dLayer( controllers_.controller_, staticModel_.detection_, *lighting_ );
    gui::Layer& urbanLayer              = *new UrbanLayer( controllers_, *glProxy_, *strategy_, *glProxy_, model_.urban_, profile );
    gui::Layer& grid                    = *new gui::GridLayer( controllers_, *glProxy_ );
    gui::Layer& metrics                 = *new gui::MetricsLayer( staticModel_.detection_, *glProxy_ );
    gui::Layer& limits                  = *new LimitsLayer( controllers_, *glProxy_, *strategy_, parameters, *modelBuilder_, *glProxy_, profile );
    gui::Layer& objectsLayer            = *new ObjectsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, picker );
    gui::Layer& populations             = *new PopulationsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, model_, profile );
    gui::Layer& ghosts                  = *new GhostsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, model_, profile );
    gui::Layer& defaultLayer            = *new gui::DefaultLayer( controllers_ );
    gui::Layer& drawerLayer             = *new gui::DrawerLayer( controllers_, *glProxy_, *strategy_, parameters, *glProxy_, profile );
    gui::Layer& inhabitantLayer         = *new InhabitantLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, dockContainer_->GetLivingAreaPanel() );
    gui::Layer& contour                 = *new gui::ContourLinesLayer( controllers_, staticModel_.detection_ );
    gui::Layer& selection               = *new gui::SelectionLayer( controllers_, *glProxy_ );


    // Drawing order
    AddLayer( *glProxy_, preferences, defaultLayer );
    AddLayer( *glProxy_, preferences, elevation2d,              "main",                         tr( "Elevation" ) );
    AddLayer( *glProxy_, preferences, raster,                   "main",                         tr( "Raster" ) );
    AddLayer( *glProxy_, preferences, terrain,                  "main",                         tr( "Terrain" ) );
    AddLayer( *glProxy_, preferences, contour,                  "main,composition,miniviews",   tr( "Contour Lines" ) );
    AddLayer( *glProxy_, preferences, urbanLayer,               "main" );
    AddLayer( *glProxy_, preferences, watershed,                "main",                         tr( "Watershed" ) );
    AddLayer( *glProxy_, preferences, elevation3d );
    AddLayer( *glProxy_, preferences, grid,                     "main" );
    AddLayer( *glProxy_, preferences, metrics,                  "main" );
    AddLayer( *glProxy_, preferences, weather,                  "main" );
    AddLayer( *glProxy_, preferences, limits,                   "main" );
    AddLayer( *glProxy_, preferences, indicatorCreationLayer );
    AddLayer( *glProxy_, preferences, inhabitantLayer,          "main",                         tr( "Populations" ) );
    AddLayer( *glProxy_, preferences, objectsLayer,             "main",                         tr( "Objects" ) );
    AddLayer( *glProxy_, preferences, populations,              "main",                         tr( "Crowd" ) );
    AddLayer( *glProxy_, preferences, ghosts,                   "main",                         tr( "Ghost" ) );
    AddLayer( *glProxy_, preferences, agents,                   "main",                         tr( "Units" ) );
    AddLayer( *glProxy_, preferences, automats,                 "main",                         tr( "Automats" ) );
    AddLayer( *glProxy_, preferences, formation,                "main",                         tr( "Formations" ) );
    AddLayer( *glProxy_, preferences, objectCreationLayer,      "main" );
    AddLayer( *glProxy_, preferences, inhabitantCreationLayer,  "main" );
    AddLayer( *glProxy_, preferences, parameters,               "main" );
    AddLayer( *glProxy_, preferences, locations,                "main" );
    AddLayer( *glProxy_, preferences, profilerLayer,            "main" );
    AddLayer( *glProxy_, preferences, drawerLayer,              "main" );
    AddLayer( *glProxy_, preferences, selection,                "main" );
    AddLayer( *glProxy_, preferences, tooltipLayer,             "tooltip" );

    // Display modes
    // $$$$ ABR 2012-05-14: Modes only work on EntityLayer for now. Layer or Layer_ABC should implement a function 'ShouldDisplay', which call IsEnabled, and use that ShouldDisplay in all classes that inherit from Layer.
    agents.SetModes( ePreparationMode_LivingArea, ePreparationMode_None, true );
    limits.SetModes( ePreparationMode_LivingArea, ePreparationMode_None, true );
    objectsLayer.SetModes( ePreparationMode_LivingArea, ePreparationMode_None, true );
    populations.SetModes( ePreparationMode_LivingArea, ePreparationMode_None, true );
    ghosts.SetModes( ePreparationMode_LivingArea, ePreparationMode_None, true );
    // Readonly modes
    urbanLayer.SetReadOnlyModes( ePreparationMode_None );
    // Multiple Selection
    controllers_.actions_.AllowMultipleSelection< kernel::UrbanObject_ABC >( ePreparationMode_Terrain );

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
    forward_->Register( selection );
    forward_->SetDefault( defaultLayer );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::ShowHelp
// Created: JSR 2012-03-20
// -----------------------------------------------------------------------------
void MainWindow::ShowHelp()
{
    help_->ShowHelp();
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
void MainWindow::DoLoad( QString filename, bool checkConsistency /*= true*/ )
{
    if( filename.isEmpty() )
        return;
    SetProgression( 0, tr( "Initialize data ..." ) );
    if( filename.startsWith( "//" ) )
        filename.replace( "/", "\\" );
    config_.LoadExercise( filename.toAscii().constData() );
    if( Load() )
    {
        SetWindowTitle( true );
        LoadExercise( checkConsistency );
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
        dockContainer_->Purge();
        model_.Purge();
        selector_->Close();
        selector_->Load();
        SetProgression( 20, tr( "Loading physical model ..." ) );
        staticModel_.Load( config_ );
        SetProgression( 50, tr( "Loading graphical interface ..." ) );
        dialogContainer_->Purge();
        dialogContainer_->Load();
        SetProgression( 60, tr( "Loading options ..." ) );
        SetWindowTitle( false );
    }
    catch( xml::exception& e )
    {
        SetNeedsSaving( false );
        Close();
        QMessageBox::critical( this, tools::translate( "Application", "SWORD" )
                                   , ( tr( "Error reading xml file: " ) + e.what() ) );
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
    if( model_.IsLoaded() && CheckSaving() == QMessageBox::Cancel )
        return false;
    DoClose();
    controllers_.ChangeMode( ePreparationMode_Default );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MainWindow::PrepareClosing
// Created: JSR 2012-06-21
// -----------------------------------------------------------------------------
void MainWindow::DoClose()
{
    dockContainer_->Purge();
    dockContainer_->BlockCreationOnListViews( true );
    model_.Purge();
    dockContainer_->BlockCreationOnListViews( false );
    staticModel_.Purge();
    selector_->Close();
    dialogContainer_->Purge();
    SetWindowTitle( false );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::LoadExercise
// Created: SBO 2008-08-21
// -----------------------------------------------------------------------------
void MainWindow::LoadExercise( bool checkConsistency /*= true*/ )
{
    try
    {
        loading_ = true;
        SetProgression( 70, tr( "Loading exercise ..." ) );
        model_.Load( config_ );
        dockContainer_->Load();
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
        SetProgression( 90, tr( "Generate symbols" ) );
        icons_->GenerateSymbols( model_.teams_ );
        loading_ = false;
        controllers_.ChangeMode( ePreparationMode_Exercise );
        if( checkConsistency )
            emit CheckConsistency();
        SetWindowTitle( model_.NeedsSaving() );
    }
    catch( std::exception& e )
    {
        SetNeedsSaving( false );
        Close();
        QMessageBox::critical( this, tools::translate( "Application", "SWORD" )
                                   , ( tr( "Error loading exercise: " ) + e.what() ) );
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
// Name: MainWindow::ExternalReload
// Created: JSR 2012-06-21
// -----------------------------------------------------------------------------
void MainWindow::ExternalReload()
{
    DoClose();
    DoLoad( config_.GetExerciseFile().c_str(), false );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Save
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void MainWindow::Save( bool checkConsistency /* = true */ )
{
    if( needsSaving_ )
    {
        assert( controllers_.modes_ );
        if( controllers_.modes_->GetCurrentMode() == ePreparationMode_Exercise )
        {
            if( checkConsistency )
            {
                ClearLoadingErrors();
                emit CheckConsistency();
            }
            model_.SaveExercise( config_ );
            if( !config_.IsTerrainSamePhysicalRef() )
            {
                model_.SaveTerrain( config_, false );
                config_.SetTerrainSamePhysicalRef( true );
                if( model_.IsLoaded() )
                    QMessageBox::warning( 0, tools::translate( "Application", "SWORD" ),
                                             tools::translate( "MainWindow", "Terrain file modified in order to match the exercise physical base."
                                                                             "\nPossible impacts on the exercises using this terrain!" ) );
            }
        }
        else
        {
            assert( controllers_.modes_->GetCurrentMode() == ePreparationMode_Terrain );
            model_.SaveTerrain( config_ );
        }
        SetWindowTitle( false );
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::SaveAs
// Created: ABR 2011-04-13
// -----------------------------------------------------------------------------
void MainWindow::SaveAs()
{
    bool exist = false;
    QString name;
    if( controllers_.modes_->GetCurrentMode() == ePreparationMode_Exercise )
    {
        bfs::path exerciseDirectory;
        do
        {
            bool ok = false;
            name = QInputDialog::getText( tr( "Save exercise as ..." ),
                ( exist ) ? tr( "The exercise '%1' already exist. Please, enter a new exercise name:" ).arg( name ) : tr( "Enter an exercise name:" ),
                QLineEdit::Normal, tr( "Type exercise name here" ), &ok, this );
            if( ok && !name.isEmpty() )
            {
                exerciseDirectory = bfs::path( config_.GetExercisesDir(), bfs::native ) / name.toAscii().constData();
                exist = frontend::commands::ExerciseExists( config_, name.toAscii().constData() ) || bfs::exists( exerciseDirectory );
            }
            else
                return;
        } while( exist );
        bfs::create_directories( exerciseDirectory );
        bfs::path exerciseFile( config_.tools::ExerciseConfig::GeneralConfig::GetExerciseFile( name.toAscii().constData() ) );
        bfs::copy_file( config_.GetExerciseFile(), exerciseFile );
        config_.LoadExercise( exerciseFile.native_file_string() );
        model_.exercise_.SetName( name );
    }
    else
    {
        assert( controllers_.modes_->GetCurrentMode() == ePreparationMode_Terrain );
        bfs::path terrainDirectory;
        do
        {
            bool ok = false;
            name = QInputDialog::getText( tr( "Save terrain as ..." ),
                ( exist ) ? tr( "The terrain '%1' already exist. Please, enter a new terrain name:" ).arg( name ) : tr( "Enter a terrain name:" ),
                QLineEdit::Normal, tr( "Type terrain name here" ), &ok, this );
            if( ok && !name.isEmpty() )
            {
                terrainDirectory = bfs::path( config_.GetTerrainsDir(), bfs::native ) / name.toAscii().constData();
                exist = frontend::commands::TerrainExists( config_, name.toAscii().constData() ) || bfs::exists( terrainDirectory );
            }
            else
                return;
        } while( exist );
        bfs::create_directories( terrainDirectory );
        frontend::Copy( config_.GetTerrainDir( config_.GetTerrainName() ), terrainDirectory.string() );
        config_.LoadTerrain( name.toAscii().constData() );
        terrainHasChanged_ = true;
    }
    dialogContainer_->Purge();
    needsSaving_ = true;
    Save();
    dialogContainer_->Load();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::closeEvent
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::closeEvent( QCloseEvent* pEvent )
{
    QMessageBox::StandardButton result = CheckSaving();
    if( result == QMessageBox::Cancel || result == QMessageBox::Yes )
    {
        pEvent->ignore();
        return;
    }
    assert( controllers_.modes_ );
    controllers_.ChangeMode( ePreparationMode_Default );
    QSettings settings( "MASA Group", tools::translate( "Application", "SWORD" ) );
    settings.beginGroup( "/" + controllers_.modes_->GetRegisteryEntry() );
    settings.setValue( "mainWindowGeometry", saveGeometry() );

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
    gui::Settings settings( "MASA Group", tools::translate( "Application", "SWORD" ) );
    settings.beginGroup( "/" + controllers_.modes_->GetRegisteryEntry() + "/Options" );
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
    settings.beginGroup( "/" + controllers_.modes_->GetRegisteryEntry() + "/Options" );
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
    if( QThread::currentThread() != thread() ) // si setCaption est appelé par un autre thread -> assert QT.
        return;
    SetNeedsSaving( needsSaving );
    QString filename = "";
    QString mode = ENT_Tr::ConvertFromPreparationMode( static_cast< E_PreparationMode >( controllers_.modes_->GetCurrentMode() ), ENT_Tr_ABC::eToTr ).c_str();
    if( model_.IsLoaded() )
    {
        if( controllers_.modes_->GetCurrentMode() == ePreparationMode_Terrain )
        {
            filename = config_.GetTerrainName().c_str();
            filename += ( needsSaving ) ? "*" : "";
        }
        else
        {
            filename = model_.exercise_.GetName().isEmpty()
                ? ( config_.GetExerciseName().empty() ? tr( "Untitled" ) : config_.GetExerciseName().c_str() )
                : model_.exercise_.GetName();
            filename += ( needsSaving ) ? "*" : "";
        }
    }
    if( filename.isEmpty() )
        setCaption( tr( "Preparation - [%1]" ).arg( tr( "No file loaded" ) ) );
    else
        setCaption( tr( "Preparation - %1 - [%2]" ).arg( mode ).arg( filename ) );
    if( menu_.get() )
        menu_->GetSaveAction()->setEnabled( needsSaving );
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
QMessageBox::StandardButton MainWindow::CheckSaving( bool checkConsistency /* = false */ )
{
    QMessageBox::StandardButton result = QMessageBox::NoButton;
    if( needsSaving_ )
    {
        assert( controllers_.modes_ != 0 );
        result = QMessageBox::question( this, tools::translate( "Application", "SWORD" )
                                                , controllers_.modes_->GetCurrentMode() == ePreparationMode_Exercise
                                                ? tr( "Unsaved modification detected.\nDo you want to save the exercise \'%1\'?" ).arg( config_.GetExerciseName().c_str() )
                                                : tr( "Unsaved modification detected.\nDo you want to save the terrain \'%1\'?" ).arg( config_.GetTerrainName().c_str() )
                                                , QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes );
        if( result == QMessageBox::Yes )
            Save( !checkConsistency );
    }
    return result;
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
                if( controllers_.modes_ && controllers_.modes_->GetCurrentMode() & observer->GetVisibleModes() )
                    continue;
            (*it)->hide();
        }
        QList< QDockWidget* > docks = qFindChildren< QDockWidget* >( this , QString() );
        for( QList< QDockWidget* >::iterator it = docks.begin(); it != docks.end(); ++it )
        {
            if( kernel::DisplayableModesObserver_ABC* observer = dynamic_cast< kernel::DisplayableModesObserver_ABC* >( *it ) )
                if( controllers_.modes_ && controllers_.modes_->GetCurrentMode() & observer->GetVisibleModes() )
                    continue;
            (*it)->hide();
        }
    }
    else if( restoreState( states_ ) )
        states_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MainWindow::SetProgression
// Created: ABR 2011-09-20
// -----------------------------------------------------------------------------
void MainWindow::SetProgression( int value, const QString& text )
{
    if( !progressDialog_.get() )
        return;
    if( !value )
        progressDialog_->show();
    progressDialog_->setLabelText( text );
    progressDialog_->setValue( value );
    qApp->processEvents();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::NotifyModeChanged
// Created: ABR 2012-06-11
// -----------------------------------------------------------------------------
void MainWindow::NotifyModeChanged( int newMode )
{
    kernel::ModesObserver_ABC::NotifyModeChanged( newMode );
    SetWindowTitle( terrainHasChanged_ && ( newMode & ePreparationMode_Exercise ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::OnAddRaster
// Created: ABR 2012-05-15
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

        gui::AddRasterDialog& dialog = dialogContainer_->GetAddRasterDialog();
        QDialog::DialogCode result = static_cast< QDialog::DialogCode >( dialog.exec() );
        if( result == QDialog::Accepted )
        {
            QStringList parameters;
            parameters << ( std::string( "--config=" ) + bfs::system_complete( config_.BuildTerrainChildFile( "config.xml" ) ).string() ).c_str();
            parameters << ( std::string( "--raster=" ) + dialog.GetFiles().toAscii().constData() ).c_str();
            parameters << ( std::string( "--pixelsize=" ) + boost::lexical_cast< std::string >( dialog.GetPixelSize() ) ).c_str();
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
// Created: JSR 2012-02-10
// -----------------------------------------------------------------------------
void MainWindow::OnRasterProcessExited( int exitCode, QProcess::ExitStatus exitStatus )
{
    if( exitStatus == QProcess::NormalExit && exitCode == EXIT_SUCCESS )
    {
        gui::RasterLayer& raster = *new gui::RasterLayer( controllers_.controller_, "~~tmp.texture.bin" );
        raster.SetPasses( "main" );
        selector_->AddLayer( raster );
        dialogContainer_->GetPrefDialog().AddLayer( tr( "User layer [%1]" ).arg( dialogContainer_->GetAddRasterDialog().GetName() ), raster, true );
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

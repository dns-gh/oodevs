// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
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
#include "RasterProcess.h"
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
#include "clients_gui/FileDialog.h"
#include "clients_gui/HelpSystem.h"
#include "clients_gui/HighlightColorModifier.h"
#include "clients_gui/LightingProxy.h"
#include "clients_gui/InhabitantLayer.h"
#include "clients_gui/ImageWrapper.h"
#include "clients_gui/LocationsLayer.h"
#include "clients_gui/MetricsLayer.h"
#include "clients_gui/MiscLayer.h"
#include "clients_gui/OverFlyingColorModifier.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/PreferencesDialog.h"
#include "clients_gui/RasterLayer.h"
#include "clients_gui/RichItemFactory.h"
#include "clients_gui/RichLineEdit.h"
#include "clients_gui/SelectionColorModifier.h"
#include "clients_gui/SelectionLayer.h"
#include "clients_gui/SelectionMenu.h"
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
#include "clients_kernel/ModeController.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"
#include "frontend/commands.h"
#include "frontend/CreateExercise.h"
#include "geodata/ProjectionException.h"
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
#include <graphics/DragMovementLayer.h>
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>

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
    gui::SubObjectName subObject( "MainWindow" );
    controllers_.modes_.SetMainWindow( this );
    controllers_.modes_.AddRegistryEntry( eModes_Prepare, "Preparation" );

    // Migration
    if( config_.HasGenerateScores() || !config_.GetFolderToMigrate().IsEmpty() )
    {
        if( config_.HasGenerateScores() )
        {
            staticModel_.Load( config_ );
            LoadExercise();
        }
        if( !config_.GetFolderToMigrate().IsEmpty() )
            MigrateExercises();
        return;
    }

    // Symbols
    gui::SymbolIcons* symbols = new gui::SymbolIcons( this, *glProxy_ );
    icons_.reset( new gui::EntitySymbols( *symbols, *strategy_ ) );
    gui::RichItemFactory* factory = new gui::RichItemFactory( this );

    // Event strategy
    forward_.reset( new gui::CircularEventStrategy( controllers, *icons_, *strategy_, staticModel_.drawings_, *glProxy_ ) );
    eventStrategy_.reset( new gui::ExclusiveEventStrategy( *forward_ ) );

    // Main widget
    selector_.reset( new gui::GlSelector( this, *glProxy_, controllers, config, staticModel.detection_, *eventStrategy_ ) );
    connect( selector_.get(), SIGNAL( Widget2dChanged( gui::GlWidget* ) ), symbols, SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );
    connect( selector_.get(), SIGNAL( Widget2dChanged( gui::GlWidget* ) ), forward_->GetSelectionMenu(), SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );
    connect( selector_.get(), SIGNAL( Widget3dChanged( gui::Gl3dWidget* ) ), forward_->GetSelectionMenu(), SLOT( OnWidget3dChanged( gui::Gl3dWidget* ) ) );

    // Strategy
    strategy_->Add( std::auto_ptr< gui::ColorModifier_ABC >( new gui::SelectionColorModifier( controllers, *glProxy_, PreparationProfile::GetProfile() ) ) );
    strategy_->Add( std::auto_ptr< gui::ColorModifier_ABC >( new gui::HighlightColorModifier( controllers, PreparationProfile::GetProfile() ) ) );
    strategy_->Add( std::auto_ptr< gui::ColorModifier_ABC >( new gui::OverFlyingColorModifier( controllers ) ) );

    // Layer 1
    gui::LocationsLayer* locationsLayer = new gui::LocationsLayer( *glProxy_ );
    gui::ParametersLayer* paramLayer = new gui::ParametersLayer( *glProxy_ );
    gui::AutomatsLayer& automats = *new gui::AutomatsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, PreparationProfile::GetProfile() );
    gui::FormationLayer& formation = *new gui::FormationLayer( controllers_, *glProxy_, *strategy_, *glProxy_, PreparationProfile::GetProfile() );
    gui::WeatherLayer* weatherLayer = new gui::WeatherLayer( *glProxy_, *eventStrategy_ );
    gui::TerrainPicker* picker = new gui::TerrainPicker( this );
    gui::TerrainProfilerLayer* profilerLayer = new gui::TerrainProfilerLayer( *glProxy_ );
    gui::Elevation2dLayer& elevation2d = *new gui::Elevation2dLayer( controllers_.controller_, staticModel_.detection_ );

    // Dialogs
    dialogContainer_.reset( new DialogContainer( this, controllers, model_, staticModel, PreparationProfile::GetProfile(), *strategy_, *colorController_,
                                                 *icons_, config, *symbols, *lighting_, *pPainter_, *paramLayer, *glProxy_, *selector_, elevation2d ) );

    // Dock widgets
    dockContainer_.reset( new DockContainer( this, controllers_, automats, formation, *icons_, *modelBuilder_, model_, staticModel_, config_, *symbols,
                                             *strategy_, *paramLayer, *weatherLayer, *glProxy_, *colorController_, *profilerLayer ) );

    // ToolBars
    toolbarContainer_.reset( new ToolbarContainer( this, controllers, staticModel, *glProxy_, *locationsLayer, *eventStrategy_, *paramLayer, model_.urban_, dialogContainer_->GetRemoveBlocksDialog(), dockContainer_->GetTerrainProfiler() ) );

    // Layers 2
    CreateLayers( *paramLayer, *locationsLayer, *weatherLayer, *profilerLayer, PreparationProfile::GetProfile(), *picker, automats, formation, elevation2d );

    // Help
    gui::HelpSystem* help = new gui::HelpSystem( this, config_.BuildResourceChildFile( "help/preparation.xml" ) );
    connect( this, SIGNAL( ShowHelp() ), help, SLOT( ShowHelp() ) );

    // Menu (must be created after DockWidgets and ToolBars for 'Windows' menu)
    menu_.reset( new Menu( "mainMenu", this, controllers, *dialogContainer_, *factory, expiration ) );
    toolbarContainer_->GetFileToolbar().Fill( *menu_ );
    setMenuBar( menu_.get() );

    // Status bar
    gui::StatusBar* pStatus = new gui::StatusBar( controllers_, statusBar(), *picker, staticModel_.detection_, staticModel_.coordinateConverter_, *selector_ );
    pStatus->SetModes( eModes_Default, 0, true );

    // Progress dialog
    progressDialog_.reset( new QProgressDialog( "", "", 0, 100, this, Qt::SplashScreen ) );
    progressDialog_->setAutoClose( true );
    progressDialog_->setModal( true );
    progressDialog_->setContentsMargins( 5, 5, 5, 5 );
    progressDialog_->setCancelButton( 0 );

    // Read settings
    controllers_.LoadOptions( eModes_Prepare );
    controllers_.modes_.LoadGeometry( eModes_Prepare );

    // Initialize
    setCentralWidget( selector_.get() );
    SetWindowTitle( false );
    setLocale( QLocale() );
    setMinimumSize( 800, 600 );
    setIcon( gui::Pixmap( tools::GeneralConfig::BuildResourceChildFile( "images/gui/logo32x32.png" ) ) );

    controllers_.ChangeMode( eModes_Default );
    controllers_.Register( *this );
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
// Name: MainWindow::Load
// Created: MCO 2013-04-10
// -----------------------------------------------------------------------------
void MainWindow::Load()
{
    if( config_.GetExerciseFile().Exists() )
    {
        SetProgression( 0, tr( "Initialize data ..." ) );
        if( DoLoad() )
            LoadExercise();
        SetProgression( 100, tr( "Loading complete" ) );
    }
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
        layer.SetReadOnlyModes( eModes_Terrain );
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::CreateLayers
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void MainWindow::CreateLayers( gui::ParametersLayer& parameters, gui::Layer& locations, gui::Layer& weather, gui::Layer& profilerLayer,
                               const kernel::Profile_ABC& profile, gui::TerrainPicker& picker, gui::AutomatsLayer& automats, gui::FormationLayer& formation,
                               gui::Elevation2dLayer& elevation2d )
{
    assert( dialogContainer_.get() && dockContainer_.get() );
    gui::PreferencesDialog& preferences     = dialogContainer_->GetPrefDialog();
    gui::Layer& terrain                 = *new gui::TerrainLayer( controllers_, *glProxy_, preferences.GetPreferences(), picker );
    ::AgentsLayer& agents                   = *new AgentsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, model_, *modelBuilder_, PreparationProfile::GetProfile() );
    gui::TooltipsLayer_ABC& tooltipLayer    = *new gui::TooltipsLayer( *glProxy_ );
    gui::Layer& objectCreationLayer     = *new gui::MiscLayer< ObjectCreationPanel >( dockContainer_->GetObjectCreationPanel() );
    gui::Layer& inhabitantCreationLayer = *new gui::MiscLayer< InhabitantCreationPanel >( dockContainer_->GetInhabitantCreationPanel() );
    gui::Layer& indicatorCreationLayer  = *new gui::MiscLayer< ScoreDialog >( dialogContainer_->GetScoreDialog() );
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
    agents.SetModes( eModes_LivingArea, eModes_None, true );
    limits.SetModes( eModes_LivingArea, eModes_None, true );
    objectsLayer.SetModes( eModes_LivingArea, eModes_None, true );
    populations.SetModes( eModes_LivingArea, eModes_None, true );
    ghosts.SetModes( eModes_LivingArea, eModes_None, true );
    // Readonly modes
    urbanLayer.SetReadOnlyModes( eModes_None );
    // Multiple Selection
    controllers_.actions_.AllowMultipleSelection< kernel::UrbanObject_ABC >( eModes_Terrain );

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
// Name: MainWindow::New
// Created: FDS 2010-11-03
// -----------------------------------------------------------------------------
void MainWindow::New()
{
    static ExerciseCreationDialog* exerciseCreationDialog_ = new ExerciseCreationDialog( this, config_ );
    if( exerciseCreationDialog_->exec() == QDialog::Accepted )
        DoLoad( exerciseCreationDialog_->GetFileName() );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::DoLoad
// Created: LDC 2010-12-01
// -----------------------------------------------------------------------------
void MainWindow::DoLoad( const tools::Path& filename )
{
    if( filename.IsEmpty() )
        return;
    SetProgression( 0, tr( "Initialize data ..." ) );
    try
    {
        config_.LoadExercise( filename );
    }
    catch( ... )
    {
        SetProgression( 100, "" );
        throw;
    }
    if( DoLoad() )
    {
        SetWindowTitle( true );
        LoadExercise();
    }
    SetProgression( 100, tr( "Loading complete" ) );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::MigrateExercise
// Created: ABR 2013-03-07
// -----------------------------------------------------------------------------
bool MainWindow::MigrateExercise( const tools::Path& path )
{
    tools::Path child = path / "exercise.xml";
    if( child.Exists() )
    {
        try
        {
            std::cout << "Loading exercise " << path.FileName() << "...\n";
            config_.LoadExercise( child );
            staticModel_.Load( config_ );
            LoadExercise();
            std::cout << "Saving exercise " << path.FileName() << "...\n";
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
    return false;
}

// -----------------------------------------------------------------------------
// Name: MainWindow::MigrateExercises
// Created: JSR 2011-09-07
// -----------------------------------------------------------------------------
void MainWindow::MigrateExercises()
{
    const tools::Path root = config_.GetFolderToMigrate();
    if( !root.Exists() )
        throw MASA_EXCEPTION( ( "The folder " + config_.GetFolderToMigrate().ToUTF8() + " does not exist" ).c_str() );
    root.Apply( boost::bind( &MainWindow::MigrateExercise, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Open
// Created: FDS 2010-11-03
// -----------------------------------------------------------------------------
void MainWindow::Open()
{
    // Open exercise file dialog
    tools::Path filename = gui::FileDialog::getOpenFileName( this, tr( "Load exercise definition file (exercise.xml)" ), config_.GetExerciseFile(), "Exercise (exercise.xml)" );
    if( filename.IsEmpty() || !Close() )
        return;
    // Load exercise
    DoLoad( filename );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::DoLoad
// Created: AGE 2006-05-03
// -----------------------------------------------------------------------------
bool MainWindow::DoLoad()
{
    try
    {
        SetProgression( 10, tr( "Loading configuration ..." ) );
        controllers_.SaveOptions( eModes_Prepare );
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
    catch( const xml::exception& e )
    {
        SetNeedsSaving( false );
        Close();
        QMessageBox::critical( this, tools::translate( "Application", "SWORD" )
                                   , tr( "Error reading xml file: " ) + tools::GetExceptionMsg( e ).c_str() );
        return false;
    }
    controllers_.LoadOptions( eModes_Prepare );
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
    controllers_.ChangeMode( eModes_Default );
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
void MainWindow::LoadExercise()
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
                model_.scores_.Serialize( config_.GetScoresFile(), schemaWriter );
            return;
        }
        SetProgression( 90, tr( "Generate symbols" ) );
        icons_->GenerateSymbols( model_.teams_ );
        loading_ = false;
        controllers_.ChangeMode( eModes_Prepare );
        emit CheckConsistency();
        SetWindowTitle( !model_.GetLoadingErrors().empty() || model_.ghosts_.NeedSaving() || model_.HasConsistencyErrorsOnLoad() ||  model_.OldUrbanMode() );
    }
    catch( const std::exception& e )
    {
        SetNeedsSaving( false );
        Close();
        QMessageBox::critical( this, tools::translate( "Application", "SWORD" )
                                   , tr( "Error loading exercise: " ) + tools::GetExceptionMsg( e ).c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::ReloadExercise
// Created: ABR 2011-06-24
// -----------------------------------------------------------------------------
void MainWindow::ReloadExercise()
{
    if( Close() )
        DoLoad( config_.GetExerciseFile() );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Save
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void MainWindow::Save( bool checkConsistency /* = true */ )
{
    if( needsSaving_ )
    {
        if( checkConsistency )
        {
            ClearLoadingErrors();
            emit CheckConsistency();
        }
        model_.SaveExercise( config_ );
        SetWindowTitle( false );
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::SaveAs
// Created: ABR 2011-04-13
// -----------------------------------------------------------------------------
void MainWindow::SaveAs()
{
    static const QString title = tr( "Save exercise as ..." );
    static const QString enterNewName = tr( "Enter an exercise name:" );
    static const QString nameAlreadyExist = tr( "The exercise '%1' already exists. Please, enter a new exercise name:" );
    static const QString content = tr( "Type exercise name here" );
    bool exist = false;
    tools::Path exerciseName;
    tools::Path exerciseDirectory;
    QString name;
    do
    {
        bool ok = false;
        name = QInputDialog::getText( title, ( exist ) ? nameAlreadyExist.arg( name ) : enterNewName, gui::RichLineEdit::Normal, content, &ok, this );
        if( ok && !name.isEmpty() )
        {
            exerciseName = tools::Path::FromUnicode( name.toStdWString() );
            exerciseName = exerciseName.Relative();
            exerciseDirectory = config_.GetExercisesDir() / exerciseName;
            exist = frontend::commands::ExerciseExists( config_, exerciseName ) || exerciseDirectory.Exists();
        }
        else
            return;
    } while( exist );

    exerciseDirectory.CreateDirectories();
    tools::Path newExerciseFile = config_.tools::ExerciseConfig::GeneralConfig::GetExerciseFile( exerciseName );
    config_.GetExerciseFile().Copy( newExerciseFile );
    config_.LoadExercise( newExerciseFile );
    model_.exercise_.SetName( exerciseName.FileName().ToUTF8().c_str() );
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
    controllers_.ChangeMode( eModes_Default );
    controllers_.modes_.SaveGeometry( eModes_Prepare );
    controllers_.SaveOptions( eModes_Prepare );
    QMainWindow::closeEvent( pEvent );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::OnForceSaveAndAddActionPlanning
// Created: ABR 2011-10-18
// -----------------------------------------------------------------------------
void MainWindow::OnForceSaveAndAddActionPlanning( const tools::Path& filename )
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
// Name: MainWindow::NotifyModeChanged
// Created: ABR 2013-02-15
// -----------------------------------------------------------------------------
void MainWindow::NotifyModeChanged( E_Modes newMode )
{
    ModesObserver_ABC::NotifyModeChanged( newMode );
    SetWindowTitle( needsSaving_ );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::SetWindowTitle
// Created: SBO 2006-11-21
// -----------------------------------------------------------------------------
void MainWindow::SetWindowTitle( bool needsSaving )
{
    if( QThread::currentThread() != thread() ) // si setCaption est appel� par un autre thread -> assert QT.
        return;
    SetNeedsSaving( needsSaving );
    QString filename;
    QString mode = ENT_Tr::ConvertFromModes( GetCurrentMode(), ENT_Tr_ABC::eToTr ).c_str();
    if( model_.IsLoaded() && isVisible() )
    {
        filename = model_.exercise_.GetName().isEmpty()
            ? ( config_.GetExerciseName().IsEmpty() ? tr( "Untitled" ) : config_.GetExerciseName().ToUTF8().c_str() )
            : model_.exercise_.GetName();
        filename += ( needsSaving ) ? "*" : "";
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
        result = QMessageBox::question( this, tools::translate( "Application", "SWORD" ),
                                        tr( "Unsaved modification detected.\nDo you want to save the exercise \'%1\'?" ).arg( config_.GetExerciseName().ToUTF8().c_str() ),
                                        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes );
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
// Name: MainWindow::SetProgression
// Created: ABR 2011-09-20
// -----------------------------------------------------------------------------
void MainWindow::SetProgression( int value, const QString& text )
{
    if( !progressDialog_.get() )
        return;
    if( !value )
    {
        const QRect rc = QApplication::desktop()->availableGeometry( QApplication::desktop()->screenNumber( this ) );
        const QSize size = progressDialog_->size();
        progressDialog_->move( ( rc.topLeft() + rc.bottomRight() - QPoint( size.width(), size.height() ) ) / 2 );
        progressDialog_->show();
    }
    progressDialog_->setLabelText( text );
    progressDialog_->setValue( value );
    qApp->processEvents();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::OnAddRaster
// Created: ABR 2012-05-15
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

        gui::AddRasterDialog& dialog = dialogContainer_->GetAddRasterDialog();
        QDialog::DialogCode result = static_cast< QDialog::DialogCode >( dialog.exec() );
        if( result == QDialog::Accepted )
        {
            auto input = tools::Path::FromUnicode( dialog.GetFiles().toStdWString() );
            auto callback = boost::bind( &MainWindow::OnRasterProcessExited,
                    this, _1, _2 );
            process_ = RunRasterApp( input, dialog.GetPixelSize(), config_, callback );
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
// Created: JSR 2012-02-10
// -----------------------------------------------------------------------------
void MainWindow::OnRasterProcessExited( int exitCode, const tools::Path& output )
{
    if( !exitCode )
    {
        gui::RasterLayer& raster = *new gui::RasterLayer( controllers_.controller_,
                output.FileName().ToUTF8() );
        raster.SetPasses( "main" );
        selector_->AddLayer( raster );
        dialogContainer_->GetPrefDialog().AddLayer( tr( "User layer [%1]" ).arg( dialogContainer_->GetAddRasterDialog().GetName() ), raster, true );
        raster.NotifyUpdated( kernel::ModelLoaded( config_ ) );
        raster.GenerateTexture();
    }
    else
        QMessageBox::warning( this, tr( "Error loading image file" ), tr( "Error while loading Raster source." ) );
    try
    {
        output.Remove();
    }
    catch( const std::exception& )
    {
        // NOTHING
    }
}

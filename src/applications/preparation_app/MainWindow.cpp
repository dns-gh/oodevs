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
#include "AutomatsLayer.h"
#include "UrbanLayer.h"
#include "PrepaConfig.h"
#include "InhabitantLayer.h"
#include "ExerciseCreationDialog.h"
#include "DialogContainer.h"
#include "DockContainer.h"
#include "FileToolbar.h"
#include "FilterOrbatReIndexer.h"
#include "FormationLayer.h"
#include "GhostsLayer.h"
#include "LimitsLayer.h"
#include "Menu.h"
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
#include "clients_gui/AddRasterDialog.h"
#include "clients_gui/CircularEventStrategy.h"
#include "clients_gui/ColorStrategy.h"
#include "clients_gui/ContourLinesLayer.h"
#include "clients_gui/DefaultLayer.h"
#include "clients_gui/DrawerFactory.h"
#include "clients_gui/DrawerLayer.h"
#include "clients_gui/Elevation2dLayer.h"
#include "clients_gui/Elevation3dLayer.h"
#include "clients_gui/EntitySymbols.h"
#include "clients_gui/ExclusiveEventStrategy.h"
#include "clients_gui/FileDialog.h"
#include "clients_gui/GisToolbar.h"
#include "clients_gui/GLMainProxy.h"
#include "clients_gui/GLStackedWidget.h"
#include "clients_gui/GLWidgetManager.h"
#include "clients_gui/GridLayer.h"
#include "clients_gui/HelpSystem.h"
#include "clients_gui/HighlightColorModifier.h"
#include "clients_gui/IconLayout.h"
#include "clients_gui/ImageWrapper.h"
#include "clients_gui/InhabitantLayer.h"
#include "clients_gui/LayerComposite.h"
#include "clients_gui/LightingProxy.h"
#include "clients_gui/LocationsLayer.h"
#include "clients_gui/MapnikLayer.h"
#include "clients_gui/MetricsLayer.h"
#include "clients_gui/MiscLayer.h"
#include "clients_gui/OverFlyingColorModifier.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/PreferencesDialog.h"
#include "clients_gui/RasterLayer.h"
#include "clients_gui/RasterProcess.h"
#include "clients_gui/ResourceNetworksLayer.h"
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
#include "clients_gui/TextEditor.h"
#include "clients_gui/Tools.h"
#include "clients_gui/TooltipsLayer.h"
#include "clients_gui/UrbanLayer.h"
#include "clients_gui/WatershedLayer.h"
#include "clients_gui/WeatherLayer.h"
#include "clients_gui/resources.h"
#include "clients_gui/resources.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/Logger_ABC.h"
#include "clients_kernel/ModeController.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"
#include "frontend/commands.h"
#include "frontend/CreateExercise.h"
#include "geodata/ProjectionException.h"
#include "preparation/AgentsModel.h"
#include "preparation/ColorController.h"
#include "preparation/Exercise.h"
#include "preparation/FormationModel.h"
#include "preparation/GhostModel.h"
#include "preparation/Model.h"
#include "preparation/ProfilesModel.h"
#include "preparation/ScoresModel.h"
#include "preparation/StaticModel.h"
#include "preparation/TeamsModel.h"
#include "tools/ExerciseConfig.h"
#include "tools/SchemaWriter.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>
#include <boost/assign.hpp>
#include <boost/bind.hpp>

namespace ba = boost::assign;

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::MainWindow( kernel::Controllers& controllers,
                        StaticModel& staticModel,
                        Model& model,
                        PrepaConfig& config,
                        const QString& expiration )
    : QMainWindow( 0, 0, Qt::WDestructiveClose )
    , controllers_( controllers )
    , staticModel_( staticModel )
    , model_( model )
    , config_( config )
    , loadingExercise_( false )
    , needsSaving_( false )
    , modelBuilder_( new ModelBuilder( controllers, model ) )
    , colorController_( new ColorController( controllers_ ) )
    , glProxy_( new gui::GLMainProxy( controllers_, PreparationProfile::GetProfile(), *model.formations_ ) )
    , symbolIcons_( new gui::SymbolIcons() )
    , forward_( new gui::CircularEventStrategy( controllers ) )
{
    const auto& profile = PreparationProfile::GetProfile();
    gui::SubObjectName subObject( "MainWindow" );
    controllers_.modes_.SetMainWindow( this );
    controllers_.actions_.AllowMultipleSelection< kernel::UrbanObject_ABC >( eModes_Terrain );
    gui::layers::CheckConsistency();

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

    // Event strategy
    eventStrategy_.reset( new gui::ExclusiveEventStrategy( *forward_ ) );

    // Color strategy
    strategy_.reset( new gui::ColorStrategy( controllers, *glProxy_, *colorController_ ) );
    strategy_->Add( std::unique_ptr< gui::ColorModifier_ABC >( new gui::SelectionColorModifier( controllers, *glProxy_, profile ) ) );
    strategy_->Add( std::unique_ptr< gui::ColorModifier_ABC >( new gui::HighlightColorModifier( controllers, profile ) ) );
    strategy_->Add( std::unique_ptr< gui::ColorModifier_ABC >( new gui::OverFlyingColorModifier( controllers ) ) );
    glProxy_->SetColorStrategy( *strategy_ ); // $$$$ MCO 2014-10-27: not that great...

    // Central Widget
    glWidgetManager_.reset( new gui::GLWidgetManager( *this,
                                                      controllers,
                                                      profile,
                                                      config,
                                                      staticModel,
                                                      model_,
                                                      std::make_shared< gui::IconLayout >(),
                                                      *eventStrategy_,
                                                      *glProxy_,
                                                      std::make_shared< gui::LightingProxy >(),
                                                      config.GetMapnikThreads(),
                                                      *model_.profiles_ ) );

    // Text editor
    textEditor_.reset( new gui::TextEditor( this ) );

    // Symbols
    icons_.reset( new gui::EntitySymbols( *symbolIcons_, *strategy_ ) );

    // Layer 1
    auto locationsLayer = std::make_shared< gui::LocationsLayer >( controllers_, *glProxy_ );
    auto paramLayer = std::make_shared< gui::ParametersLayer >( controllers_, *glProxy_, *textEditor_ );
    auto weatherLayer = std::make_shared< gui::WeatherLayer >( controllers_, *glProxy_, *eventStrategy_ );
    auto profilerLayer = std::make_shared< gui::TerrainProfilerLayer >( controllers_, *glProxy_ );
    gui::TerrainPicker* picker = new gui::TerrainPicker( this );

    // Dialogs
    dialogContainer_.reset( new DialogContainer( this, controllers, model_, staticModel, profile, *strategy_, *colorController_,
                                                 *icons_, config, *symbolIcons_, paramLayer, *glProxy_ ) );

    // Dock widgets
    dockContainer_.reset( new DockContainer( this, controllers_, paramLayer, weatherLayer, profilerLayer, *icons_, *modelBuilder_, model_,
                                             staticModel_, config_, *symbolIcons_, *strategy_, *glProxy_, *colorController_, profile ) );

    // ToolBars
    toolbarContainer_.reset( new ToolbarContainer( this, controllers, staticModel, *glProxy_, locationsLayer, paramLayer, *eventStrategy_, *model_.urban_, dialogContainer_->GetRemoveBlocksDialog(), dockContainer_->GetTerrainProfiler() ) );

    // Layers 2
    CreateLayers( paramLayer, locationsLayer, weatherLayer, profilerLayer, *picker );

    // Help
    gui::HelpSystem* help = new gui::HelpSystem( this, config_.BuildResourceChildFile( "help/preparation.xml" ), &controllers_.modes_ );
    connect( this, SIGNAL( ShowHelp() ), help, SLOT( ShowHelp() ) );

    // Menu (must be created after DockWidgets and ToolBars for 'Windows' menu)
    menu_.reset( new Menu( "mainMenu", *this, controllers, *glWidgetManager_, *dialogContainer_, expiration ) );
    toolbarContainer_->GetFileToolbar().Fill( *menu_ );
    setMenuBar( menu_.get() );

    // Status bar
    gui::StatusBar* pStatus = new gui::StatusBar( controllers_, statusBar(), *picker, staticModel_.detection_, staticModel_.coordinateConverter_, *glProxy_ );
    pStatus->SetModes( eModes_Default, 0, true );
    connect( glWidgetManager_.get(), SIGNAL( MouseMove( const geometry::Point2f& ) ),
             pStatus, SLOT( OnMouseMove( const geometry::Point2f& ) ) );
    connect( glWidgetManager_.get(), SIGNAL( MouseMove( const geometry::Point3f& ) ),
             pStatus, SLOT( OnMouseMove( const geometry::Point3f& ) ) );

    // Progress dialog
    progressDialog_.reset( new QProgressDialog( "", "", 0, 100, this, Qt::SplashScreen ) );
    progressDialog_->setAutoClose( true );
    progressDialog_->setModal( true );
    progressDialog_->setContentsMargins( 5, 5, 5, 5 );
    progressDialog_->setCancelButton( 0 );

    setLocale( QLocale() );
    setMinimumSize( 800, 600 );
    setIcon( gui::Pixmap( tools::GeneralConfig::BuildResourceChildFile( "images/gui/logo32x32.png" ) ) );

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
}

// -----------------------------------------------------------------------------
// Name: MainWindow::CreateLayers
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void MainWindow::CreateLayers( const std::shared_ptr< gui::ParametersLayer >& parameters,
                               const std::shared_ptr< gui::Layer_ABC >& locations,
                               const std::shared_ptr< gui::Layer_ABC >& weather,
                               const std::shared_ptr< gui::Layer_ABC >& profiler,
                               gui::TerrainPicker& picker )
{
    assert( dialogContainer_.get() && dockContainer_.get() );
    const auto& profile = PreparationProfile::GetProfile();
    auto tooltips = std::make_shared< gui::TooltipsLayer >( controllers_, *glProxy_ );
    gui::T_LayersMap layers;
    layers[ eLayerTypes_Agents ]             = std::make_shared< AgentsLayer >( controllers_, *glProxy_, *strategy_, model_, *modelBuilder_, PreparationProfile::GetProfile() );
    layers[ eLayerTypes_Automats ]           = std::make_shared< AutomatsLayer >( controllers_, *glProxy_, *strategy_, profile );
    layers[ eLayerTypes_ContourLines ]       = std::make_shared< gui::ContourLinesLayer >( controllers_, *glProxy_ );
    layers[ eLayerTypes_Crowds ]             = std::make_shared< PopulationsLayer >( controllers_, *glProxy_, *strategy_, model_, profile );
    layers[ eLayerTypes_Default ]            = std::make_shared< gui::DefaultLayer >( controllers_, *glProxy_ );
    layers[ eLayerTypes_Drawings ]           = std::make_shared< gui::DrawerLayer >( controllers_, *glProxy_, *strategy_, parameters, profile, *modelBuilder_ );
    layers[ eLayerTypes_Elevation2d ]        = std::make_shared< gui::Elevation2dLayer >( controllers_, *glProxy_, staticModel_.detection_ );
    layers[ eLayerTypes_Elevation3d ]        = std::make_shared< gui::Elevation3dLayer >( controllers_, *glProxy_, staticModel_.detection_ );
    layers[ eLayerTypes_Formations ]         = std::make_shared< FormationLayer >( controllers_, *glProxy_, *strategy_, profile );
    layers[ eLayerTypes_Ghosts ]             = std::make_shared< GhostsLayer >( controllers_, *glProxy_, *strategy_, model_, profile );
    layers[ eLayerTypes_Grid ]               = std::make_shared< gui::GridLayer >( controllers_, *glProxy_, staticModel_.coordinateConverter_ );
    layers[ eLayerTypes_IndicatorCreation ]  = std::make_shared< gui::MiscLayer< ScoreDialog > >( controllers_, *glProxy_, eLayerTypes_IndicatorCreation, dialogContainer_->GetScoreDialog() );
    layers[ eLayerTypes_InhabitantCreation ] = std::make_shared< gui::MiscLayer< InhabitantCreationPanel > >( controllers_, *glProxy_, eLayerTypes_InhabitantCreation, dockContainer_->GetInhabitantCreationPanel() );
    layers[ eLayerTypes_Inhabitants ]        = std::make_shared< InhabitantLayer >( controllers_, *glProxy_, *strategy_, profile, dockContainer_->GetLivingAreaPanel() );
    layers[ eLayerTypes_Locations ]          = locations;
    if( config_.HasMapnik() )
        layers[ eLayerTypes_Mapnik ]         = std::make_shared< gui::MapnikLayer >( controllers_, *glProxy_ );
    layers[ eLayerTypes_Metric ]             = std::make_shared< gui::MetricsLayer >( controllers_, staticModel_.detection_, *glProxy_ );
    layers[ eLayerTypes_ObjectCreation ]     = std::make_shared< gui::MiscLayer< ObjectCreationPanel > >( controllers_, *glProxy_, eLayerTypes_ObjectCreation, dockContainer_->GetObjectCreationPanel() );
    layers[ eLayerTypes_Objects ]            = std::make_shared< ObjectsLayer >( controllers_, *glProxy_, *strategy_, profile, picker );
    layers[ eLayerTypes_Parameters ]         = parameters;
    layers[ eLayerTypes_Raster ]             = std::make_shared< gui::RasterLayer >( controllers_, *glProxy_ );
    layers[ eLayerTypes_ResourceNetworks ]   = std::make_shared< gui::ResourceNetworksLayer >( controllers_, *glProxy_, *strategy_, profile );
    layers[ eLayerTypes_Selection ]          = std::make_shared< gui::SelectionLayer >( controllers_, *glProxy_ );
    layers[ eLayerTypes_TacticalLines ]      = std::make_shared< LimitsLayer >( controllers_, *glProxy_, *strategy_, parameters, *modelBuilder_, profile );
    layers[ eLayerTypes_Terrain ]            = std::make_shared< gui::TerrainLayer >( controllers_, *glWidgetManager_, *glProxy_, picker );
    layers[ eLayerTypes_TerrainProfiler ]    = profiler;
    layers[ eLayerTypes_Tooltips ]           = tooltips;
    layers[ eLayerTypes_Urban ]              = std::make_shared< UrbanLayer >( controllers_, *glProxy_, *strategy_, *model_.urban_, profile );
    layers[ eLayerTypes_Watershed ]          = std::make_shared< gui::WatershedLayer >( controllers_, *glProxy_, staticModel_.detection_ );
    layers[ eLayerTypes_Weather ]            = weather;
    // composites, need others to be created
    layers[ eLayerTypes_InhabitantsComposite ]   = std::make_shared< gui::LayerComposite >( controllers_, *glProxy_, layers, eLayerTypes_InhabitantsComposite );
    layers[ eLayerTypes_ObjectsComposite ]       = std::make_shared< gui::LayerComposite >( controllers_, *glProxy_, layers, eLayerTypes_ObjectsComposite );
    layers[ eLayerTypes_TacticalLinesComposite ] = std::make_shared< gui::LayerComposite >( controllers_, *glProxy_, layers, eLayerTypes_TacticalLinesComposite );
    layers[ eLayerTypes_UnitsComposite ]         = std::make_shared< gui::LayerComposite >( controllers_, *glProxy_, layers, eLayerTypes_UnitsComposite );
    layers[ eLayerTypes_WeatherComposite ]       = std::make_shared< gui::LayerComposite >( controllers_, *glProxy_, layers, eLayerTypes_WeatherComposite );
    // init
    glProxy_->AddLayers( gui::layers::GetDefaultDrawingOrder( layers, eModes_AllPrepare ) );
    glProxy_->SetTooltipsLayer( tooltips );
    forward_->Initialize( controllers_,
                          *icons_,
                          *strategy_,
                          staticModel_.drawings_,
                          glProxy_,
                          layers.at( eLayerTypes_Default ),
                          gui::layers::GetEventOrder( layers, eModes_AllPrepare ) );
    controllers_.options_.InitializeLayers( gui::layers::GetDefaultConfigurableOrder( layers, eModes_AllPrepare ) );
}

// -----------------------------------------------------------------------------
// First load, last close
// -----------------------------------------------------------------------------
void MainWindow::Initialize()
{
    controllers_.modes_.LoadOptions( eModes_Prepare );
    controllers_.modes_.LoadGeometry( eModes_Prepare );
    controllers_.ChangeMode( eModes_Default );
    if( config_.GetExerciseFile().Exists() )
        FullLoad(); // config already initialized in ExerciseConfig::Parse
}

void MainWindow::closeEvent( QCloseEvent* event )
{
    if( !Close() )
    {
        event->ignore();
        return;
    }
    QMainWindow::closeEvent( event );
}

// -----------------------------------------------------------------------------
// Close / Load
// -----------------------------------------------------------------------------
bool MainWindow::Close( bool askForSaving /* = true */ )
{
    if( !askForSaving )
        needsSaving_ = false;
    try
    {
        if( model_.IsLoaded() && CheckSaving() == QMessageBox::Cancel )
            return false;
        // save options
        controllers_.modes_.SaveGeometry( eModes_Prepare );
        controllers_.modes_.SaveOptions( eModes_Prepare );
        // close gui
        tools::RejectVisibleDialogs( *this );
        // score dialogs open others score dialogs when they close, so they need
        // a specific close signal
        emit CloseDialogs();
        dialogContainer_->Purge();
        dockContainer_->Purge();
        glWidgetManager_->Purge();
        // restore default gui
        SetWindowTitle( false );
        controllers_.ChangeMode( eModes_Default );
        // purge data
        model_.Purge();
        staticModel_.Purge();
    }
    catch( const std::exception& e )
    {
        CatchException( tr( "Error closing exercise: " ), e );
    }
    return true;
}

void MainWindow::FullLoad( const tools::Path& filename /* = tools::Path() */ )
{
    try
    {
        SetProgression( 0, tr( "Initialize data ..." ) );
        if( !filename.IsEmpty() )
        {
            SetProgression( 10, tr( "Loading configuration ..." ) );
            config_.LoadExercise( filename );
        }
        SetProgression( 20, tr( "Loading physical model ..." ) );
        staticModel_.Load( config_ );
        SetProgression( 50, tr( "Loading exercise ..." ) );
        LoadExercise();
        SetProgression( 80, tr( "Loading graphical interface ..." ) );
        dialogContainer_->Load();
        dockContainer_->Load();
        glWidgetManager_->Load( staticModel_.drawings_,
                                config_.GetExerciseDir( config_.GetExerciseName() ) );
        SetProgression( 85, tr( "Generate symbols ..." ) );
        symbolIcons_->Initialize( glWidgetManager_->GetMainWidget()->GetWidget2d().get() );
        icons_->GenerateSymbols( *model_.teams_ );
        SetProgression( 90, tr( "Loading options ..." ) );
        controllers_.modes_.LoadOptions( eModes_Prepare );
        controllers_.ChangeMode( eModes_Prepare );
        SetProgression( 95, tr( "Checking consistency ..." ) );
        emit CheckConsistency();
        SetWindowTitle( !model_.GetLoadingErrors().empty() ||
                        model_.ghosts_->NeedSaving() ||
                        model_.HasConsistencyErrorsOnLoad() ||
                        model_.OldUrbanMode() );
        SetProgression( 100, tr( "Loading complete" ) );
    }
    catch( const std::exception& e )
    {
        SetProgression( 100, tr( "Loading failed" ) );
        CatchException( tr( "Error loading exercise: " ), e );
    }
}

void MainWindow::LoadExercise()
{
    try
    {
        loadingExercise_ = true;
        model_.Load( config_ );
        if( config_.HasGenerateScores() )
            GenerateScores();
        loadingExercise_ = false;
    }
    catch( const std::exception& e )
    {
        loadingExercise_ = false;
        CatchException( tr( "Error loading exercise: " ), e );
        throw;
    }
}

void MainWindow::Reload( const tools::Path& filename )
{
    if( filename.IsEmpty() || !Close() )
        return;
    FullLoad( filename );
}

void MainWindow::CatchException( const QString& msg, const std::exception& e )
{
    Close( false );
    QMessageBox::critical( this,
                           tools::translate( "Application", "SWORD" ),
                           msg + QString::fromStdString( tools::GetExceptionMsg( e ) ) );
}

// -----------------------------------------------------------------------------
// New, Open, Reload
// -----------------------------------------------------------------------------
void MainWindow::New()
{
    ExerciseCreationDialog exerciseCreationDialog( this, config_ );
    if( exerciseCreationDialog.exec() == QDialog::Accepted )
        Reload( exerciseCreationDialog.GetFileName() );
}

void MainWindow::Open()
{
    Reload( gui::FileDialog::getOpenFileName( this, tr( "Load exercise definition file (exercise.xml)" ),
                                                      config_.GetExerciseFile(), "Exercise (exercise.xml)" ) );
}

void MainWindow::ReloadExercise()
{
    Reload( config_.GetExerciseFile() );
}

// -----------------------------------------------------------------------------
// Save, Save As
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

void MainWindow::SaveAs()
{
    static const QString title = tools::translate( "MainWindow", "Save exercise as ..." );
    static const QString enterNewName = tools::translate( "MainWindow", "Enter an exercise name:" );
    static const QString nameAlreadyExist = tools::translate( "MainWindow", "The exercise '%1' already exists. Please, enter a new exercise name:" );
    static const QString content = tools::translate( "MainWindow", "Type exercise name here" );
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
    }
    while( exist );

    exerciseDirectory.CreateDirectories();
    tools::Path newExerciseFile = config_.tools::ExerciseConfig::GeneralConfig::GetExerciseFile( exerciseName );
    config_.GetExerciseFile().Copy( newExerciseFile );
    config_.LoadExercise( newExerciseFile );
    model_.exercise_->SetName( exerciseName.FileName().ToUTF8().c_str() );
    dialogContainer_->Purge();
    needsSaving_ = true;
    Save();
    dialogContainer_->Load();
}

// -----------------------------------------------------------------------------
// Scores generation
// -----------------------------------------------------------------------------
void MainWindow::GenerateScores()
{
    model_.scores_->GenerateScoresFromTemplate( config_.GetPhyLoader() );
    const tools::SchemaWriter schemaWriter;
    if( model_.scores_->CheckValidity( schemaWriter ) )
        model_.scores_->Serialize( config_.GetScoresFile(), schemaWriter );
}

// -----------------------------------------------------------------------------
// Migrations
// -----------------------------------------------------------------------------
bool MainWindow::MigrateExercise( const tools::Path& path )
{
    tools::Path child = path / "exercise.xml";
    if( child.Exists() )
    {
        try
        {
            MT_LOG_ERROR_MSG( "Loading exercise " << path.FileName() );
            config_.LoadExercise( child );
            staticModel_.Load( config_ );
            LoadExercise();
            MT_LOG_ERROR_MSG( "Saving exercise " << path.FileName() );
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

void MainWindow::MigrateExercises()
{
    const tools::Path root = config_.GetFolderToMigrate();
    if( !root.Exists() )
        throw MASA_EXCEPTION( ( "The folder " + config_.GetFolderToMigrate().ToUTF8() + " does not exist" ).c_str() );
    root.Apply( boost::bind( &MainWindow::MigrateExercise, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::OnForceSaveAndAddActionPlanning
// Created: ABR 2011-10-18
// -----------------------------------------------------------------------------
void MainWindow::OnForceSaveAndAddActionPlanning( const tools::Path& filename )
{
    model_.exercise_->SetActionPlanning( filename );
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
    if( !loadingExercise_ )
        SetWindowTitle( true );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::NotifyUpdated
// Created: SBO 2006-11-21
// -----------------------------------------------------------------------------
void MainWindow::NotifyUpdated()
{
    if( !loadingExercise_ )
        SetWindowTitle( true );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::NotifyDeleted
// Created: SBO 2006-11-21
// -----------------------------------------------------------------------------
void MainWindow::NotifyDeleted()
{
    if( !loadingExercise_ )
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
    if( QThread::currentThread() != thread() ) // si setCaption est appelé par un autre thread -> assert QT.
        return;
    needsSaving_ = needsSaving;
    QString filename;
    QString mode = ENT_Tr::ConvertFromModes( GetCurrentMode(), ENT_Tr::eToTr ).c_str();
    if( model_.IsLoaded() && isVisible() )
    {
        filename = model_.exercise_->GetName().isEmpty()
            ? ( config_.GetExerciseName().IsEmpty() ? tools::translate( "MainWindow", "Untitled" ) : config_.GetExerciseName().ToUTF8().c_str() )
            : model_.exercise_->GetName();
        filename += ( needsSaving ) ? "*" : "";
    }
    if( filename.isEmpty() )
        setCaption( tools::translate( "MainWindow", "Preparation - [%1]" ).arg( tools::translate( "MainWindow", "No file loaded" ) ) );
    else
        setCaption( tools::translate( "MainWindow", "Preparation - %1 - [%2]" ).arg( mode ).arg( filename ) );
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
        const QString unsavedText = tr( "Unsaved modification detected.\nDo you want to save the exercise \'%1\'?" ).arg( config_.GetExerciseName().ToUTF8().c_str() );
        result = QMessageBox::question( this, tools::translate( "Application", "SWORD" ), unsavedText,
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
    if( !config_.BuildTerrainChildFile( "config.xml" ).Exists() )
    {
            QMessageBox::warning( 0, tools::translate( "MainWindow", "Warning" ),
                tools::translate( "MainWindow", "This functionality is not available with old terrain format." ) );
        return;
    }
    try
    {
        auto dialog = new gui::AddRasterDialog( this );
        auto result = static_cast< QDialog::DialogCode >( dialog->exec() );
        if( result == QDialog::Accepted )
        {
            const auto input = tools::Path::FromUnicode( dialog->GetFiles().toStdWString() );
            process_ = RunRasterApp( input, dialog->GetPixelSize(), config_,
                [&]( int exitCode, const tools::Path& output, const std::string& error )
                {
                    if( !exitCode )
                        glProxy_->AddLayers( gui::T_LayersVector( 1, std::make_shared< gui::RasterLayer >( controllers_, *glProxy_, output, dialog->GetName() ) ) );
                    else
                        QMessageBox::warning( this, tools::translate( "MainWindow", "Error loading image file" ),
                            error.empty() ? tools::translate( "MainWindow", "Error while loading Raster source." ) : error.c_str() );
                } );
        }
    }
    catch( const geodata::ProjectionException& )
    {
        QMessageBox::information( this, tools::translate( "MainWindow", "Error loading image file" ), tools::translate( "MainWindow", "The following raster you add is missing spatial reference information.\nThis data can't be projected.") ) ;
        // Created: AME 2010-09-16 : TODO->allow user to set the projection in UI
    }
    catch( ... )
    {
        QMessageBox::critical( this, tools::translate( "MainWindow", "Error loading image file" ), tools::translate( "MainWindow", "Fatal error adding Raster source." ), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton );
    }
}

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
#include "CommunicationListView.h"
#include "ExerciseCreationDialog.h"
#include "CreationPanels.h"
#include "Dialogs.h"
#include "ExerciseDialog.h"
#include "ExportDialog.h"
#include "FileToolbar.h"
#include "ImportOrbatDialog.h"
#include "IntelligencesLayer.h"
#include "LimitsLayer.h"
#include "Menu.h"
#include "ModelBuilder.h"
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
#include "ScoreDialog.h"
#include "SuccessFactorDialog.h"
#include "TacticalListView.h"
#include "TemplatesPanel.h"
#include "WeatherLayer.h"
#include "WeatherPanel.h"
#include "clients_gui/AutomatsLayer.h"
#include "clients_gui/CircularEventStrategy.h"
#include "clients_gui/ColorStrategy.h"
#include "clients_gui/DefaultLayer.h"
#include "clients_gui/DisplayToolbar.h"
#include "clients_gui/DrawerFactory.h"
#include "clients_gui/DrawerLayer.h"
#include "clients_gui/DrawerPanel.h"
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
#include "clients_gui/InhabitantLayer.h"
#include "clients_gui/LocationsLayer.h"
#include "clients_gui/MetricsLayer.h"
#include "clients_gui/MissionLayer.h"
#include "clients_gui/OptionsPanel.h"
#include "clients_gui/ParametersLayer.h"
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
#include "preparation/ModelChecker_ABC.h"
#include "preparation/StaticModel.h"
#include "preparation/TeamsModel.h"
#include "preparation/Tools.h"
#include "tools/ExerciseConfig.h"
#include <graphics/DragMovementLayer.h>
#include <xeumeuleu/xml.hpp>
#include <boost/filesystem.hpp>
#include <qinputdialog.h>

namespace bfs = boost::filesystem;

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::MainWindow( Controllers& controllers, StaticModel& staticModel, Model& model, tools::ExerciseConfig& config, const QString& expiration )
    : QMainWindow   ( 0, 0, Qt::WDestructiveClose )
    , controllers_  ( controllers )
    , staticModel_  ( staticModel )
    , model_        ( model )
    , modelBuilder_ ( new ModelBuilder( controllers, model ) )
    , config_       ( config )
    , forward_      ( new CircularEventStrategy() )
    , eventStrategy_( new ExclusiveEventStrategy( *forward_ ) )
    , pPainter_     ( new ElevationPainter( staticModel_.detection_ ) )
    , simpleFilter_ ( new gui::SimpleFilter() )
    , urbanFilter_  ( new gui::UrbanFilter() )
    , glProxy_      ( 0 )
    , menu_         ( 0 )
    , fileToolBar_  ( 0 )
    , needsSaving_  ( false )
    , loading_      ( false )
{
    setIcon( QPixmap( tools::GeneralConfig::BuildResourceChildFile( "images/gui/logo32x32.png" ).c_str() ) );

    lighting_ = new LightingProxy( this );
    PreferencesDialog* prefDialog = new PreferencesDialog( this, controllers, *lighting_, staticModel_.coordinateSystems_, *pPainter_ );
    new Dialogs( this, controllers, staticModel, PreparationProfile::GetProfile() );

    glProxy_ = new GlProxy();
    strategy_ = new ColorStrategy( controllers, *glProxy_ );
    strategy_->Add( std::auto_ptr< ColorModifier_ABC >( new SelectionColorModifier( controllers, *glProxy_ ) ) );
    strategy_->Add( std::auto_ptr< ColorModifier_ABC >( new HighlightColorModifier( controllers ) ) );

    selector_ = new GlSelector( this, *glProxy_, controllers, config, staticModel.detection_, *eventStrategy_ );

    RichItemFactory* factory = new RichItemFactory( this );

    // Agent list panel
    QDockWindow* pListDockWnd_ = new QDockWindow( this, "orbat" );
    moveDockWindow( pListDockWnd_, Qt::DockLeft );
    QTabWidget* pListsTabWidget = new QTabWidget( pListDockWnd_ );

    gui::SymbolIcons* symbols = new gui::SymbolIcons( this, *glProxy_ );
    connect( selector_, SIGNAL( Widget2dChanged( gui::GlWidget* ) ), symbols, SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );
    gui::EntitySymbols* icons = new gui::EntitySymbols( *symbols, *strategy_ );
    ProfileDialog* profileDialog = new ProfileDialog( this, controllers, *factory, *icons, model_.profiles_, staticModel_.extensions_ );
    ProfileWizardDialog* profileWizardDialog = new ProfileWizardDialog( this, model_, model_.profiles_ );

    QTabWidget* pAgentsTabWidget = new QTabWidget( pListsTabWidget );
    QVBox* listsTabBox = new QVBox( pListsTabWidget );
    new EntitySearchBox< Agent_ABC >( listsTabBox, controllers );
    new ::TacticalListView( listsTabBox, controllers, *factory, *icons, *modelBuilder_, model_.formations_.levels_ );
    pAgentsTabWidget->addTab( listsTabBox, tr( "Tactical" ) );

    listsTabBox = new QVBox( pListsTabWidget );
    new EntitySearchBox< Agent_ABC >( listsTabBox, controllers );
    new ::CommunicationListView( listsTabBox, controllers, *factory, *icons, *modelBuilder_ );
    pAgentsTabWidget->addTab( listsTabBox, tr( "Communication" ) );

    pListsTabWidget->addTab( pAgentsTabWidget, tr( "Units" ) );
    listsTabBox = new QVBox( pListsTabWidget );
    new EntitySearchBox< Object_ABC >( listsTabBox, controllers );
    new ::ObjectListView( listsTabBox, controllers, *factory, *modelBuilder_ );
    pListsTabWidget->addTab( listsTabBox, tr( "Objects" ) );
    listsTabBox = new QVBox( pListsTabWidget );
    new EntitySearchBox< Population_ABC >( listsTabBox, controllers );
    new ::PopulationListView( listsTabBox, controllers, *factory, *modelBuilder_ );
    pListsTabWidget->addTab( listsTabBox, tr( "Crowds" ) );

    listsTabBox = new QVBox( pListsTabWidget );
    new EntitySearchBox< Inhabitant_ABC >( listsTabBox, controllers );
    new ::InhabitantListView( listsTabBox, controllers, *factory, *modelBuilder_ );
    pListsTabWidget->addTab( listsTabBox, tr( "Populations" ) );

    pListsTabWidget->addTab( new IntelligenceList( controllers, *factory, *icons, PreparationProfile::GetProfile() ), tr( "Intelligences" ) );
    pListDockWnd_->setWidget( pListsTabWidget );
    pListDockWnd_->setResizeEnabled( true );
    pListDockWnd_->setCloseMode( QDockWindow::Always );
    pListDockWnd_->setCaption( tr( "Units" ) );
    setDockEnabled( pListDockWnd_, Qt::DockTop, false );

    // Creation panel
    QDockWindow* pCreationDockWnd = new QDockWindow( this, "creation" );
    moveDockWindow( pCreationDockWnd, Qt::DockRight );
    CreationPanels* pCreationPanel = new CreationPanels( pCreationDockWnd, controllers, staticModel_, *factory, *symbols, *strategy_ );
    pCreationDockWnd->setWidget( pCreationPanel );
    pCreationDockWnd->setResizeEnabled( true );
    pCreationDockWnd->setCloseMode( QDockWindow::Always );
    pCreationDockWnd->setCaption( tr( "Creation" ) );
    setDockEnabled( pCreationDockWnd, Qt::DockTop, false );

    // Properties panel
    {
        QDockWindow* pPropertiesDockWnd = new QDockWindow( this, "properties" );
        moveDockWindow( pPropertiesDockWnd, Qt::DockRight );
        ::PropertiesPanel* propertiesPanel = new ::PropertiesPanel( pPropertiesDockWnd, controllers, model_, staticModel_ );
        pPropertiesDockWnd->setWidget( propertiesPanel );
        pPropertiesDockWnd->setResizeEnabled( true );
        pPropertiesDockWnd->setCloseMode( QDockWindow::Always );
        pPropertiesDockWnd->setCaption( tr( "Properties" ) );
        setDockEnabled( pPropertiesDockWnd, Qt::DockTop, false );
    }
    // ResourceNetwork panel
    {
        QDockWindow* pResourceWnd = new ResourceNetworkDialog( this, controllers, staticModel_.objectTypes_ );
        moveDockWindow( pResourceWnd, Qt::DockLeft );
        setDockEnabled( pResourceWnd, Qt::DockTop, false );
        pResourceWnd->hide();
    }
    // Extensions panel
    {
        pExtensionsPanel_ = new gui::ExtensionsPanel( this, controllers, staticModel_.extensions_, model.agents_, *factory, *icons, PreparationProfile::GetProfile(), "ExtensionsPanel" );
        moveDockWindow( pExtensionsPanel_, Qt::DockLeft );
        setDockEnabled( pExtensionsPanel_, Qt::DockTop, false );
        setAppropriate( pExtensionsPanel_, false );
        pExtensionsPanel_->hide();
    }

    // A few layers
    LocationsLayer* locationsLayer = new LocationsLayer( *glProxy_ );
    ParametersLayer* paramLayer = new ParametersLayer( *glProxy_, *new gui::LocationEditorToolbar( this, controllers_, staticModel_.coordinateConverter_, *glProxy_, *locationsLayer ) );
    ::AgentsLayer* agentsLayer = new ::AgentsLayer( controllers, *glProxy_, *strategy_, *glProxy_, model_, *modelBuilder_, PreparationProfile::GetProfile(), *simpleFilter_ );

    // object creation window
    ObjectCreationPanel* objectCreationPanel = new ObjectCreationPanel( pCreationDockWnd, *pCreationPanel, controllers, staticModel_, model.objects_, model.urban_, *paramLayer, *glProxy_, config_ );
    pCreationPanel->AddPanel( objectCreationPanel );

    InhabitantCreationPanel* inhabitantCreationPanel = new InhabitantCreationPanel( pCreationDockWnd, *pCreationPanel, controllers, /*( tools::Resolver< InhabitantType >&)(*/ staticModel.types_ /*)*/, model.agents_, *paramLayer, *glProxy_ );
    pCreationPanel->AddPanel( inhabitantCreationPanel );

    ::WeatherLayer* weatherLayer = new ::WeatherLayer( *glProxy_, *eventStrategy_ );
    WeatherPanel* weatherPanel = new WeatherPanel( pCreationDockWnd, *pCreationPanel, controllers, staticModel_.coordinateConverter_, *weatherLayer );
    pCreationPanel->AddPanel( weatherPanel );

    TemplatesPanel* templates = new TemplatesPanel( pCreationDockWnd, *pCreationPanel, controllers, model.agents_, model.formations_, staticModel.types_ );
    pCreationPanel->AddPanel( templates );

    gui::DrawerPanel* drawerPanel = new DrawerPanel( pCreationDockWnd, *pCreationPanel, *paramLayer, controllers, model.drawings_ );
    pCreationPanel->AddPanel( drawerPanel );

    QDialog* exerciseDialog = new ExerciseDialog( this, controllers, model.exercise_, config_, model.teams_.InfiniteDotations() );
    QDialog* importDialog = new ImportOrbatDialog( this, config_, model );
    QDialog* exportDialog = new ExportDialog( this, config_ );
    ScoreDialog* scoreDialog = new ScoreDialog( this, controllers, *factory, model_.scores_, *paramLayer, staticModel_, config_ );
    SuccessFactorDialog* successFactorDialog = new SuccessFactorDialog( this, controllers, model_.successFactors_, *factory, staticModel_.successFactorActionTypes_, model_.scores_ );
    fileToolBar_ = new FileToolbar( this, controllers );
    new DisplayToolbar( this, controllers );
    gui::TerrainProfilerLayer* profilerLayer = new gui::TerrainProfilerLayer( *glProxy_ );
    new gui::GisToolbar( this, controllers, staticModel_.detection_, *profilerLayer );

    gui::HelpSystem* help = new gui::HelpSystem( this, config_.BuildResourceChildFile( "help/preparation.xml" ) );
    menu_ = new Menu( this, controllers, *prefDialog, *profileDialog, *profileWizardDialog, *importDialog, *exportDialog, *scoreDialog, *successFactorDialog, *exerciseDialog, *factory, expiration, *help );

    // $$$$ AGE 2006-08-22: prefDialog->GetPreferences()
    gui::TerrainPicker* picker = new gui::TerrainPicker( this );
    gui::TerrainLayer* terrainLayer = new gui::TerrainLayer( controllers_, *glProxy_, prefDialog->GetPreferences(), *picker );

    CreateLayers( *objectCreationPanel, *inhabitantCreationPanel, *paramLayer, *locationsLayer, *weatherLayer, *agentsLayer, *terrainLayer, *profilerLayer, *prefDialog, PreparationProfile::GetProfile(), *picker );

    StatusBar* pStatus = new StatusBar( statusBar(), *picker, staticModel_.detection_, staticModel_.coordinateConverter_ );
    connect( selector_, SIGNAL( MouseMove( const geometry::Point2f& ) ), pStatus, SLOT( OnMouseMove( const geometry::Point2f& ) ) );
    connect( selector_, SIGNAL( MouseMove( const geometry::Point3f& ) ), pStatus, SLOT( OnMouseMove( const geometry::Point3f& ) ) );
    controllers_.Register( *this );

    SetWindowTitle( false );
    EnableWorkspace( false );
    ReadSettings();
    ReadOptions();

    if( bfs::exists( bfs::path( config_.GetExerciseFile(), bfs::native ) ) && Load() )
        LoadExercise();
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
void MainWindow::CreateLayers( ObjectCreationPanel& objects, InhabitantCreationPanel& inhabitants,  ParametersLayer& parameters, gui::Layer_ABC& locations, gui::Layer_ABC& weather, ::AgentsLayer& agents, gui::TerrainLayer& terrain, gui::Layer_ABC& profilerLayer, PreferencesDialog& preferences, const Profile_ABC& profile, gui::TerrainPicker& picker )
{
    TooltipsLayer_ABC& tooltipLayer     = *new TooltipsLayer( *glProxy_ );
    AutomatsLayer& automats             = *new AutomatsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, *simpleFilter_ );
    Layer_ABC& formation                = *new FormationLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, *simpleFilter_ );
    Layer_ABC& objectCreationLayer      = *new MiscLayer< ObjectCreationPanel >( objects );
    Layer_ABC& inhabitantCreationLayer  = *new MiscLayer< InhabitantCreationPanel >( inhabitants );
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

    // ordre de dessin
    glProxy_->Register( defaultLayer );
    glProxy_->Register( elevation2d );             preferences.AddLayer( tr( "Elevation" ), elevation2d );          elevation2d         .SetPasses( "main" );
    glProxy_->Register( raster );                  preferences.AddLayer( tr( "Raster" ), raster );                  raster              .SetPasses( "main" );
    glProxy_->Register( terrain );                 preferences.AddLayer( tr( "Terrain" ), terrain );                terrain             .SetPasses( "main" );
    glProxy_->Register( urbanLayer );               /*preferences.AddLayer( tr( "Urban" ), urbanLayer );*/          urbanLayer          .SetPasses( "main" );
    glProxy_->Register( watershed );               preferences.AddLayer( tr( "Watershed" ), watershed );            watershed           .SetPasses( "main" );
    glProxy_->Register( elevation3d );
    glProxy_->Register( grid );                                                                                     grid                .SetPasses( "main" );
    glProxy_->Register( weather );                                                                                  weather             .SetPasses( "main" );
    glProxy_->Register( limits );                                                                                   limits              .SetPasses( "main" );
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
    if( filename.startsWith( "//" ) )
        filename.replace( "/", "\\" );

    config_.LoadExercise( filename.ascii() );
    if( Load() )
    {
        SetWindowTitle( true );
        LoadExercise();
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
    QString filename = QFileDialog::getOpenFileName( config_.GetExerciseFile().c_str(), "Exercise (exercise.xml)", this, 0, tr( "Load exercise definition file (exercise.xml)" ) );
    // Load exercise
    DoLoad( filename );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Load
// Created: AGE 2006-05-03
// -----------------------------------------------------------------------------
bool MainWindow::Load()
{
    WriteOptions();
    try
    {
        model_.Purge();
        selector_->Close();
        selector_->Load();
        staticModel_.Load( config_ );
        if( staticModel_.extensions_.tools::StringResolver< ExtensionType >::Find( "orbat-attributes" ) )
            setAppropriate( pExtensionsPanel_, true );
        else
        {
            setAppropriate( pExtensionsPanel_, false );
            pExtensionsPanel_->hide();
        }
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
    SetWindowTitle( false );
    EnableWorkspace( false );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::EnableWorkspace
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
void MainWindow::EnableWorkspace( bool enabled )
{
    QPtrList< QDockWindow > docks = dockWindows();
    for( QPtrList< QDockWindow >::iterator it = docks.begin(); it != docks.end(); ++it )
        if( *it != fileToolBar_ )
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
        model_.Load( config_, loadingErrors );
        loading_ = false;
        bool errors = !loadingErrors.empty();
        SetWindowTitle( errors );
        if( errors )
            QMessageBox::critical( this, tools::translate( "Application", "SWORD" )
                , tr( "The following entities cannot be loaded: " ) + "\n" + loadingErrors.c_str() );
        EnableWorkspace( true );
    }
    catch( std::exception& e )
    {
        SetNeedsSaving( false );
        Close();
        QMessageBox::critical( this, tools::translate( "Application", "SWORD" )
                                   , ( tr( "Error loading exercise: " ) + e.what() ).ascii() );
    }
}

namespace
{
    struct SaveModelChecker : public ModelChecker_ABC
    {
        explicit SaveModelChecker( QMainWindow* window ) : window_( window ) {}
        virtual bool Validate()
        {
            return true;
        }

        virtual bool Reject( const QString& reason )
        {
            QMessageBox::critical( window_, tools::translate( "Application", "SWORD" ), reason );
            return false;
        }

        virtual bool Prompt( const QString& question )
        {
            if( QMessageBox::question( window_, tools::translate( "Application", "SWORD" )
                                     , question + tools::translate( "MainWindow", "\nDo you want to save anyway?" ), QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
                return true;
            return false;
        }

        QMainWindow* window_;
    };
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
            exerciseDirectory = bfs::path( tools::GeneralConfig::BuildChildPath( config_.GetExercisesDir(), name.ascii() ), bfs::native );
            exist = frontend::commands::ExerciseExists( config_, name.ascii() ) || bfs::exists( exerciseDirectory );
        }
        else
            return;
    } while( exist );

    bfs::create_directories( exerciseDirectory );
    bfs::path exerciseFile = bfs::path( config_.tools::GeneralConfig::GetExerciseFile( name.ascii() ) );
    bfs::copy_file( bfs::path( config_.GetExerciseFile() ), exerciseFile );
    config_.LoadExercise( exerciseFile.string() );
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
    WriteSettings();
    WriteOptions();
    QMainWindow::closeEvent( pEvent );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::WriteSettings
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::WriteSettings()
{
    Settings settings;
    settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
    settings.beginGroup( "/Preparation" );
    if( savedState_.isNull() || savedState_.isEmpty() )
    {
        QTextStream stream( &savedState_, IO_WriteOnly );
        stream << *this;
    }
    settings.writeEntry( "/Panels", savedState_ );
    settings.WriteEntry( "/MainWindow", *this );
    settings.endGroup();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::ReadSettings
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::ReadSettings()
{
    Settings settings;
    settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
    settings.beginGroup( "/Preparation" );

    // Pannel configuration
    QString strDockConfig;
    strDockConfig = settings.readEntry( "/Panels" );
    QTextStream strDockConfigStream( &strDockConfig, IO_ReadOnly );
    strDockConfigStream >> *this;

    // Main window configuration
    settings.ReadEntry( "/MainWindow", *this, 800, 600, 100, 100, false );
    settings.endGroup();
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
    if( savedState_.isNull() || savedState_.isEmpty() )
    {
        QTextStream stream( &savedState_, IO_WriteOnly );
        stream << *this;
        QPtrList< QDockWindow > docks = dockWindows();
        for( QPtrList< QDockWindow >::iterator it = docks.begin(); it != docks.end(); ++it )
            (*it)->hide();
    }
    else
    {
        QTextStream stream( &savedState_, IO_ReadOnly );
        stream >> *this;
        savedState_ = QString::null;
    }
}

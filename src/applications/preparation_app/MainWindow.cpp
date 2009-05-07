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

#include "PropertiesPanel.h"
#include "CreationPanels.h"
#include "Menu.h"
#include "ObjectCreationPanel.h"
#include "ObjectsLayer.h"
#include "TacticalListView.h"
#include "CommunicationListView.h"
#include "ObjectListView.h"
#include "PopulationListView.h"
#include "AgentsLayer.h"
#include "ModelBuilder.h"
#include "Dialogs.h"
#include "LimitsLayer.h"
#include "PopulationsLayer.h"
#include "WeatherLayer.h"
#include "PreparationProfile.h"
#include "WeatherPanel.h"
#include "FileToolbar.h"
#include "ProfileDialog.h"
#include "ProfileWizardDialog.h"
#include "TemplatesPanel.h"
#include "IntelligencesLayer.h"
#include "ImportOrbatDialog.h"
#include "ScoreDialog.h"

#include "preparation/Model.h"
#include "preparation/StaticModel.h"
#include "preparation/FormationModel.h"
#include "preparation/IntelligencesModel.h"
#include "preparation/ModelChecker_ABC.h"
#include "preparation/Tools.h"

#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/FormationLevels.h"

#include "clients_gui/GlProxy.h"
#include "clients_gui/GraphicPreferences.h"
#include "clients_gui/OptionsPanel.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/Settings.h"
#include "clients_gui/StatusBar.h"
#include "clients_gui/PreferencesDialog.h"
#include "clients_gui/RichItemFactory.h"
#include "clients_gui/resources.h"
#include "clients_gui/ColorStrategy.h"
#include "clients_gui/SelectionColorModifier.h"
#include "clients_gui/HighlightColorModifier.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/Elevation2dLayer.h"
#include "clients_gui/TerrainLayer.h"
#include "clients_gui/MetricsLayer.h"
#include "clients_gui/GridLayer.h"
#include "clients_gui/MissionLayer.h"
#include "clients_gui/CircularEventStrategy.h"
#include "clients_gui/ExclusiveEventStrategy.h"
#include "clients_gui/DefaultLayer.h"
#include "clients_gui/EntitySearchBox.h"
#include "clients_gui/SymbolIcons.h"
#include "clients_gui/EntitySymbols.h"
#include "clients_gui/LightingProxy.h"
#include "clients_gui/LocationEditorToolbar.h"
#include "clients_gui/DrawerLayer.h"
#include "clients_gui/DrawerFactory.h"
#include "clients_gui/DrawerPanel.h"
#include "clients_gui/GlSelector.h"
#include "clients_gui/DisplayToolbar.h"
#include "clients_gui/RasterLayer.h"
#include "clients_gui/Elevation3dLayer.h"
#include "clients_gui/LocationsLayer.h"
#include "clients_gui/AutomatsLayer.h"
#include "clients_gui/IntelligenceList.h"
#include "clients_gui/TooltipsLayer.h"
#include "clients_gui/HelpSystem.h"

#include "graphics/DragMovementLayer.h"

#include "tools/ExerciseConfig.h"

#include <xeumeuleu/xml.h>

#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )
namespace bfs = boost::filesystem;

using namespace xml;
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
    , glProxy_      ( 0 )
    , needsSaving_  ( false )
{
    setIcon( MAKE_PIXMAP( csword ) );
    SetWindowTitle( false );

    lighting_ = new LightingProxy( this );
    PreferencesDialog* prefDialog = new PreferencesDialog( this, controllers, *lighting_ );
    new Dialogs( this, controllers, PreparationProfile::GetProfile() );
    
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
    ProfileDialog* profileDialog = new ProfileDialog( this, controllers, *factory, *icons, model_.profiles_ );
    ProfileWizardDialog* profileWizardDialog = new ProfileWizardDialog( this, model_, model_.profiles_ );
    ScoreDialog* scoreDialog = new ScoreDialog( this, controllers, *factory, model_.scores_, staticModel_.indicators_, staticModel_.gaugeTypes_ );

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
    new ::ObjectListView( listsTabBox, controllers, *factory );
    pListsTabWidget->addTab( listsTabBox, tr( "Objects" ) );
    listsTabBox = new QVBox( pListsTabWidget );
    new EntitySearchBox< Population_ABC >( listsTabBox, controllers );
    new ::PopulationListView( listsTabBox, controllers, *factory, *modelBuilder_ );
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

    // A few layers
    LocationsLayer* locationsLayer = new LocationsLayer( *glProxy_ );
    ParametersLayer* paramLayer = new ParametersLayer( *glProxy_, *new gui::LocationEditorToolbar( this, controllers_, staticModel_.coordinateConverter_, *glProxy_, *locationsLayer ) );
    ::AgentsLayer* agentsLayer = new ::AgentsLayer( controllers, *glProxy_, *strategy_, *glProxy_, model_, *modelBuilder_, PreparationProfile::GetProfile() );

    // object creation window
    ObjectCreationPanel* objectCreationPanel = new ObjectCreationPanel( pCreationDockWnd, *pCreationPanel, controllers, staticModel_, model.teams_, *paramLayer, *symbols, *glProxy_ );
    pCreationPanel->AddPanel( objectCreationPanel );

    WeatherLayer* weatherLayer = new WeatherLayer( *glProxy_, *eventStrategy_ );
    WeatherPanel* weatherPanel = new WeatherPanel( pCreationDockWnd, *pCreationPanel, controllers, staticModel_.coordinateConverter_, *weatherLayer );
    pCreationPanel->AddPanel( weatherPanel );

    TemplatesPanel* templates = new TemplatesPanel( pCreationDockWnd, *pCreationPanel, controllers, model.agents_, model.formations_, staticModel.types_ );
    pCreationPanel->AddPanel( templates );

    gui::DrawerPanel* drawerPanel = new DrawerPanel( pCreationDockWnd, *pCreationPanel, *paramLayer, controllers, model.drawings_ );
    pCreationPanel->AddPanel( drawerPanel );

    QDialog* importDialog = new ImportOrbatDialog( this, config_, model );
    new FileToolbar( this );
    new DisplayToolbar( this, controllers );

    gui::HelpSystem* help = new gui::HelpSystem( this, config_.BuildResourceChildFile( "help/preparation.xml" ) );
    new Menu( this, controllers, *prefDialog, *profileDialog, *profileWizardDialog, *importDialog, *scoreDialog, *factory, expiration, *help );

    // $$$$ AGE 2006-08-22: prefDialog->GetPreferences()
    CreateLayers( *objectCreationPanel, *paramLayer, *locationsLayer, *weatherLayer, *agentsLayer, prefDialog->GetPreferences(), *prefDialog, PreparationProfile::GetProfile() );

    StatusBar* pStatus = new StatusBar( statusBar(), staticModel_.detection_, staticModel_.coordinateConverter_ );
    connect( selector_, SIGNAL( MouseMove( const geometry::Point2f& ) ), pStatus, SLOT( OnMouseMove( const geometry::Point2f& ) ) );
    connect( selector_, SIGNAL( MouseMove( const geometry::Point3f& ) ), pStatus, SLOT( OnMouseMove( const geometry::Point3f& ) ) );
    controllers_.Register( *this );

    ReadSettings();
    ReadOptions();

    if( bfs::exists( bfs::path( config_.GetExerciseFile(), bfs::native ) ) && Load() )
        LoadExercise();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::CreateLayers
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void MainWindow::CreateLayers( ObjectCreationPanel& objects, ParametersLayer& parameters, LocationsLayer& locations, WeatherLayer& weather, ::AgentsLayer& agents, GraphicPreferences& setup, PreferencesDialog& preferences, const Profile_ABC& profile )
{
    TooltipsLayer_ABC& tooltipLayer = *new TooltipsLayer( *glProxy_ );
    Layer_ABC& automats             = *new AutomatsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, agents );
    Layer_ABC& objectCreationLayer  = *new MiscLayer< ObjectCreationPanel >( objects );
    Elevation2dLayer& elevation2d   = *new Elevation2dLayer( controllers_.controller_, staticModel_.detection_ );
    Layer_ABC& raster               = *new RasterLayer( controllers_.controller_ );
    Layer_ABC& terrain              = *new TerrainLayer( controllers_, *glProxy_, setup );
    Layer_ABC& elevation3d          = *new Elevation3dLayer( controllers_.controller_, staticModel_.detection_, *lighting_ );
    Layer_ABC& grid                 = *new GridLayer( controllers_, *glProxy_ );
    Layer_ABC& metrics              = *new MetricsLayer( *glProxy_ );
    Layer_ABC& intelligences        = *new ::IntelligencesLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile, model_.intelligences_ );
    Layer_ABC& limits               = *new LimitsLayer( controllers_, *glProxy_, *strategy_, parameters, *modelBuilder_, *glProxy_, *eventStrategy_, profile );
    Layer_ABC& objectsLayer         = *new ::ObjectsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    Layer_ABC& populations          = *new ::PopulationsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, model_, profile );
    Layer_ABC& defaultLayer         = *new DefaultLayer( controllers_ );
    Layer_ABC& drawerLayer          = *new DrawerLayer( controllers_, *glProxy_, *strategy_, parameters, *glProxy_, profile );

    // ordre de dessin
    glProxy_->Register( defaultLayer );
    glProxy_->Register( elevation2d );             preferences.AddLayer( tr( "Elevation" ), elevation2d );          elevation2d         .SetPasses( "main" );
    glProxy_->Register( raster );                  preferences.AddLayer( tr( "Raster" ), raster );                  raster              .SetPasses( "main" );
    glProxy_->Register( terrain );                 preferences.AddLayer( tr( "Terrain" ), terrain );                terrain             .SetPasses( "main" );
    glProxy_->Register( elevation3d );
    glProxy_->Register( grid );                                                                                     grid                .SetPasses( "main" );
    glProxy_->Register( weather );                                                                                  weather             .SetPasses( "main" );
    glProxy_->Register( limits );                                                                                   limits              .SetPasses( "main" );
    glProxy_->Register( intelligences );            preferences.AddLayer( tr( "Intelligence" ), intelligences );    intelligences       .SetPasses( "main" );
    glProxy_->Register( objectsLayer );             preferences.AddLayer( tr( "Objects" ), objectsLayer );          objectsLayer        .SetPasses( "main" );
    glProxy_->Register( populations );              preferences.AddLayer( tr( "Populations" ), populations );       populations         .SetPasses( "main" );
    glProxy_->Register( agents );                   preferences.AddLayer( tr( "Units" ), agents );                  agents              .SetPasses( "main" );
    glProxy_->Register( automats );                 preferences.AddLayer( tr( "Automats" ), automats );             automats            .SetPasses( "main" );
    glProxy_->Register( objectCreationLayer );                                                                      objectCreationLayer .SetPasses( "main" );
    glProxy_->Register( parameters );                                                                               parameters          .SetPasses( "main" );
    glProxy_->Register( metrics );                                                                                  metrics             .SetPasses( "main" );
    glProxy_->Register( locations );                                                                                locations           .SetPasses( "main" );
    glProxy_->Register( drawerLayer );                                                                              drawerLayer         .SetPasses( "main" );
    glProxy_->Register( tooltipLayer );                                                                             tooltipLayer        .SetPasses( "tooltip" );

    // ordre des evenements
    forward_->Register( parameters );
    forward_->Register( agents );
    forward_->Register( automats );
    forward_->Register( populations );
    forward_->Register( objectsLayer );
    forward_->Register( intelligences );
    forward_->Register( weather );
    forward_->Register( limits );
    forward_->Register( drawerLayer );
    forward_->Register( metrics );
    forward_->Register( elevation3d );
    forward_->SetDefault( defaultLayer );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::New
// Created: AGE 2006-05-03
// -----------------------------------------------------------------------------
bool MainWindow::New()
{
    QString filename = QFileDialog::getOpenFileName( config_.GetExerciseFile().c_str(), "Exercise (*.xml)", this, 0, tr( "Load exercise definition file (exercise.xml)" ) );
    if( filename.isEmpty() )
        return false;
    if( filename.startsWith( "//" ) )
        filename.replace( "/", "\\" );
    config_.LoadExercise( filename.ascii() );
    if( Load() )
    {
        SetWindowTitle( true );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Open
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
void MainWindow::Open()
{
    if( New() )
        LoadExercise();
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
        SetWindowTitle( false );
    }
    catch( xml::exception& e )
    {
        Close();
        QMessageBox::critical( this, tools::translate( "Application", "SWORD Officer Training" )
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
    model_.Purge();
    SetWindowTitle( false );
    selector_->Close();
    staticModel_.Purge();
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
        model_.Load( config_ );
        loading_ = false;
        SetWindowTitle( false );
    }
    catch( std::exception& e )
    {
        QMessageBox::critical( this, tools::translate( "Application", "SWORD Officer Training" )
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
            QMessageBox::critical( window_, tools::translate( "Application", "SWORD Officer Training" ), reason );
            return false;
        }

        virtual bool Prompt( const QString& question )
        {
            if( QMessageBox::question( window_, tools::translate( "Application", "SWORD Officer Training" )
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
    SaveModelChecker checker( this );
    const bool result = model_.Save( config_, checker );
    if( result )
        SetWindowTitle( false );
    return result;
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
// Name: MainWindow::closeEvent
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::closeEvent( QCloseEvent* pEvent )
{
    if( needsSaving_ )
    {
        int result = QMessageBox::question( this, tools::translate( "Application", "SWORD Officer Training" )
                                                , tr( "Save modifications?" )
                                                , QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel );
        if( ( result == QMessageBox::Yes && !Save() ) || result == QMessageBox::Cancel )
        {
            pEvent->ignore();
            return;
        }
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
    settings.setPath( "MASA Group", tools::translate( "Application", "SWORD Officer Training" ) );
    settings.beginGroup( "/Preparation" );

    // Pannel configuration
    QString strDockConfig;
    QTextStream strDockConfigStream( &strDockConfig, IO_WriteOnly );
    strDockConfigStream << *this;
    settings.writeEntry( "/Panels", strDockConfig );
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
    settings.setPath( "MASA Group", tools::translate( "Application", "SWORD Officer Training" ) );
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
    settings.setPath( "MASA Group", tools::translate( "Application", "SWORD Officer Training" ) );
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
    settings.setPath( "MASA Group", tools::translate( "Application", "SWORD Officer Training" ) );
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
    needsSaving_ = needsSaving;
    QString filename = model_.GetName().isEmpty() ? tr( "New ORBAT" ) : model_.GetName();
    if( needsSaving )
        filename += "*";
    setCaption( tr( "Preparation - [%1]" ).arg( filename ) );
}

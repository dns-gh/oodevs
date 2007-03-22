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

#include "InfoPanels.h"
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

#include "preparation/Exceptions.h"
#include "preparation/Model.h"
#include "preparation/StaticModel.h"
#include "preparation/FormationModel.h"

#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/ExerciseConfig.h"

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
#include "clients_gui/DrawerToolbar.h"
#include "clients_gui/GlSelector.h"
#include "clients_gui/DisplayToolbar.h"
#include "clients_gui/RasterLayer.h"
#include "clients_gui/Elevation3dLayer.h"
#include "graphics/DragMovementLayer.h"

#include "xeumeuleu/xml.h"

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
MainWindow::MainWindow( Controllers& controllers, StaticModel& staticModel, Model& model, ExerciseConfig& config )
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
    new Dialogs( this, controllers );
    
    glProxy_ = new GlProxy();
    strategy_ = new ColorStrategy( controllers, *glProxy_ );

    selector_ = new GlSelector( this, *glProxy_, controllers, config, staticModel.detection_, *eventStrategy_ );

    RichItemFactory* factory = new RichItemFactory( this ); // $$$$ AGE 2006-05-11: aggregate somewhere

    // Agent list panel
    QDockWindow* pListDockWnd_ = new QDockWindow( this );
    moveDockWindow( pListDockWnd_, Qt::DockLeft );
    QTabWidget* pListsTabWidget = new QTabWidget( pListDockWnd_ );

    gui::SymbolIcons* symbols = new gui::SymbolIcons( this );
    connect( selector_, SIGNAL( Widget2dChanged( gui::GlWidget* ) ), symbols, SLOT( OnWidget2dChanged( gui::GlWidget* ) ) );
    gui::EntitySymbols* icons = new gui::EntitySymbols( *symbols, *strategy_ );
    ProfileDialog* profileDialog = new ProfileDialog( this, controllers, *factory, *icons, model_.profiles_ );

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
    new ::PopulationListView( listsTabBox, controllers, *factory );
    pListsTabWidget->addTab( listsTabBox, tr( "Populations" ) );
    pListDockWnd_->setWidget( pListsTabWidget );
    pListDockWnd_->setResizeEnabled( true );
    pListDockWnd_->setCloseMode( QDockWindow::Always );
    pListDockWnd_->setCaption( tr( "Units" ) );
    setDockEnabled( pListDockWnd_, Qt::DockTop, false );

    // Creation panel
    QDockWindow* pCreationDockWnd = new QDockWindow( this );
    moveDockWindow( pCreationDockWnd, Qt::DockRight );
    CreationPanels* pCreationPanel = new CreationPanels( pCreationDockWnd, controllers, staticModel_, *factory );
    pCreationDockWnd->setWidget( pCreationPanel );
    pCreationDockWnd->setResizeEnabled( true );
    pCreationDockWnd->setCloseMode( QDockWindow::Always );
    pCreationDockWnd->setCaption( tr( "Creation" ) );
    setDockEnabled( pCreationDockWnd, Qt::DockTop, false );

    // Info panel
    QDockWindow* pInfoDockWnd_ = new QDockWindow( this );
    moveDockWindow( pInfoDockWnd_, Qt::DockRight );
    InfoPanels* pInfoPanel_ = new InfoPanels( pInfoDockWnd_, controllers, model_, staticModel_, *factory );
    pInfoDockWnd_->setWidget( pInfoPanel_ );
    pInfoDockWnd_->setResizeEnabled( true );
    pInfoDockWnd_->setCloseMode( QDockWindow::Always );
    pInfoDockWnd_->setCaption( tr( "Information" ) );
    setDockEnabled( pInfoDockWnd_, Qt::DockTop, false );

    // A few layers
    ParametersLayer* paramLayer = new ParametersLayer( *glProxy_, *new gui::LocationEditorToolbar( this, controllers_, staticModel_.coordinateConverter_, *glProxy_ ) );
    ::AgentsLayer* agentsLayer = new ::AgentsLayer( controllers, *glProxy_, *strategy_, *glProxy_, model_, *modelBuilder_, PreparationProfile::GetProfile() );

    // object creation window
    ObjectCreationPanel* objectCreationPanel = new ObjectCreationPanel( pCreationDockWnd, *pCreationPanel, controllers, staticModel_, model.teams_, *paramLayer, *glProxy_ );
    pCreationPanel->AddPanel( objectCreationPanel );

    WeatherLayer* weatherLayer = new WeatherLayer( *glProxy_, *eventStrategy_ );
    WeatherPanel* weatherPanel = new WeatherPanel( pCreationDockWnd, *pCreationPanel, controllers, staticModel_.coordinateConverter_, *weatherLayer );
    pCreationPanel->AddPanel( weatherPanel );

    new FileToolbar( this );
    new DisplayToolbar( this, controllers );

    // Drawer
    DrawerLayer* drawer = new DrawerLayer( *glProxy_ );
    new DrawerToolbar( this, *eventStrategy_, *drawer, *glProxy_, controllers_ );

    new Menu( this, controllers, *prefDialog, *profileDialog );

    // $$$$ AGE 2006-08-22: prefDialog->GetPreferences()
    CreateLayers( *objectCreationPanel, *paramLayer, *weatherLayer, *agentsLayer, *drawer, prefDialog->GetPreferences(), PreparationProfile::GetProfile() );

    StatusBar* pStatus = new StatusBar( statusBar(), staticModel_.detection_, staticModel_.coordinateConverter_ );
    connect( selector_, SIGNAL( MouseMove( const geometry::Point2f& ) ), pStatus, SLOT( OnMouseMove( const geometry::Point2f& ) ) );
    connect( selector_, SIGNAL( MouseMove( const geometry::Point3f& ) ), pStatus, SLOT( OnMouseMove( const geometry::Point3f& ) ) );
    controllers_.Register( *this );

    ReadSettings();
    ReadOptions();

    if( bfs::exists( bfs::path( config_.GetExerciseFile(), bfs::native ) ) && Load() )
        if( bfs::exists( bfs::path( config_.GetOrbatFile(), bfs::native ) ) )
        {
            loading_ = true;
            model_.Load( config_ );
            loading_ = false;
            SetWindowTitle( false );
        }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::CreateLayers
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void MainWindow::CreateLayers( ObjectCreationPanel& objects, ParametersLayer& parameters, WeatherLayer& weather, ::AgentsLayer& agents, DrawerLayer& drawer, GraphicPreferences& setup, const Profile_ABC& profile )
{
    // $$$$ AGE 2007-03-09: preferences !
    Layer_ABC& objectCreationLayer = *new MiscLayer< ObjectCreationPanel >( objects );
    Elevation2dLayer& elevation2d  = *new Elevation2dLayer( controllers_.controller_, staticModel_.detection_ );
    Layer2d_ABC& raster            = *new RasterLayer( controllers_.controller_ );
    Layer_ABC& terrain             = *new TerrainLayer( controllers_, *glProxy_, setup );
    Layer_ABC& elevation3d         = *new Elevation3dLayer( controllers_.controller_, staticModel_.detection_, *lighting_ );
    Layer_ABC& grid                = *new GridLayer( controllers_, *glProxy_ );
    Layer_ABC& metrics             = *new MetricsLayer( *glProxy_ );
    Layer_ABC& limits              = *new LimitsLayer( controllers_, *glProxy_, *strategy_, parameters, *modelBuilder_, *glProxy_, *eventStrategy_, profile );
    Layer_ABC& objectsLayer        = *new ::ObjectsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    Layer_ABC& populations         = *new ::PopulationsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, model_, profile );
    Layer_ABC& defaultLayer        = *new DefaultLayer( controllers_ );

    // ordre de dessin
    glProxy_->Register( defaultLayer );
    glProxy_->Register( elevation2d );
    glProxy_->Register( raster );
    glProxy_->Register( terrain );
    glProxy_->Register( elevation3d );
    glProxy_->Register( grid );
    glProxy_->Register( weather );
    glProxy_->Register( limits );
    glProxy_->Register( objectsLayer );
    glProxy_->Register( populations );
    glProxy_->Register( agents );
    glProxy_->Register( objectCreationLayer );
    glProxy_->Register( parameters );
    glProxy_->Register( metrics );
    glProxy_->Register( drawer );

    // ordre des evenements
    forward_->Register( parameters );
    forward_->Register( agents );
    forward_->Register( populations );
    forward_->Register( objectsLayer );
    forward_->Register( weather );
    forward_->Register( limits );
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
    try
    {
        if( New() && bfs::exists( bfs::path( config_.GetOrbatFile(), bfs::native ) ) )
        {
            loading_ = true;
            model_.Load( config_ );
            loading_ = false;
            SetWindowTitle( false );
        }
    }
    catch( InvalidModelException& e )
    {
        Close();
        QMessageBox::critical( this, APP_NAME, e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Load
// Created: AGE 2006-05-03
// -----------------------------------------------------------------------------
bool MainWindow::Load()
{
    try
    {
        model_.Purge();
        selector_->Load();
        staticModel_.Load( config_ );
        SetWindowTitle( false );
    }
    catch( xml::exception& e )
    {
        Close();
        QMessageBox::critical( this, APP_NAME, ( tr( "Error reading xml file: " ) + e.what() ).ascii() );
        return false;
    }
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
// Name: MainWindow::Save
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
bool MainWindow::Save()
{
    if( model_.GetName().isEmpty() )
        return SaveAs();
    else
    {
        try
        {
            model_.Save( config_ );
            SetWindowTitle( false );
        }
        catch( InvalidModelException& e )
        {
            QMessageBox::critical( this, APP_NAME, e.what() );
            return false;
        }
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MainWindow::SaveAs
// Created: SBO 2006-11-21
// -----------------------------------------------------------------------------
bool MainWindow::SaveAs()
{
    // $$$$ NLD 2007-01-12: user probably shouldn't be able to save orbat to a different file
    // $$$$ NLD 2007-01-12: at least until exercise.xml can be actually modified
//    std::string current;
//    const QString filename = QFileDialog::getSaveFileName( "../data/", "Ordre de bataille (*.xml)", 0, 0, "Sauvegarde ODB" );
//    if( filename.isEmpty() )
//        return false;
//    current = filename;
//    if( current.substr( 0, 2 ) == "//" )
//        std::replace( current.begin(), current.end(), '/', '\\' );
//
//    model_.Save( current.c_str() );
//    SetWindowTitle( false );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MainWindow destructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    controllers_.Remove( *this );
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
        int result = QMessageBox::question( this, APP_NAME, tr( "Save modifications?" ), QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel );
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
    settings.setPath( "MASA", "CSword" );
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
    settings.setPath( "MASA", "CSword" );
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
    settings.setPath( "MASA", "CSword" );
    settings.beginGroup( "/Preparation" );
    settings.beginGroup( "/Options" );
    controllers_.options_.Save( settings );
    settings.endGroup();
    settings.endGroup();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::ReadOptions
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::ReadOptions()
{
    Settings settings;
    settings.setPath( "MASA", "CSword" );
    settings.beginGroup( "/Preparation" );
    settings.beginGroup( "/Options" );
    controllers_.options_.Load( settings );
    settings.endGroup();
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
    setCaption( QString( APP_NAME " - [%1]" ).arg( filename ) );
}

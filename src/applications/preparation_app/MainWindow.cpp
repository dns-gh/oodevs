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

//#include "ControllerToolbar.h"
//#include "EventToolbar.h"
#include "InfoPanels.h"
#include "CreationPanels.h"
//#include "LogisticToolbar.h"
//#include "MapToolbar.h"
#include "Menu.h"
#include "ObjectCreationPanel.h"
#include "ObjectsLayer.h"
//#include "UnitToolbar.h"
//#include "LinkInterpreter.h"
#include "TacticalListView.h"
#include "CommunicationListView.h"
#include "ObjectListView.h"
#include "AgentsLayer.h"
#include "ModelBuilder.h"
#include "Dialogs.h"
#include "PreparationProfile.h"
#include "preparation/Exceptions.h"

#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/PathTools.h"
#include "clients_kernel/FormationLevels.h"

#include "preparation/Model.h"
//#include "gaming/Population.h"
#include "preparation/StaticModel.h"
#include "preparation/FormationModel.h"

#include "clients_gui/GlWidget.h"
#include "clients_gui/GlProxy.h"
#include "clients_gui/GraphicPreferences.h"
//#include "clients_gui/Logger.h"
#include "clients_gui/OptionsPanel.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/Settings.h"
#include "clients_gui/StatusBar.h"
#include "clients_gui/PopulationList.h"
#include "clients_gui/PreferencesDialog.h"
#include "clients_gui/GlPlaceHolder.h"
#include "clients_gui/RichItemFactory.h"
#include "clients_gui/resources.h"
#include "clients_gui/ColorStrategy.h"
#include "clients_gui/PopulationsLayer.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/Elevation2dLayer.h"
#include "clients_gui/TerrainLayer.h"
#include "clients_gui/MetricsLayer.h"
#include "clients_gui/GridLayer.h"
#include "clients_gui/MissionLayer.h"
//#include "clients_gui/LimitsLayer.h"
#include "clients_gui/CircularEventStrategy.h"
#include "clients_gui/DefaultLayer.h"
#include "clients_gui/IconLayout.h"
#include "clients_gui/EntitySearchBox.h"

//#include "clients_gui/NatureEditionWidget.h"

#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )
namespace bfs = boost::filesystem;

#include "xeumeuleu/xml.h"
using namespace xml;

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::MainWindow( Controllers& controllers, StaticModel& staticModel, Model& model )
    : QMainWindow  ( 0, 0, Qt::WDestructiveClose )
    , controllers_ ( controllers )
    , staticModel_ ( staticModel )
    , model_       ( model )
    , modelBuilder_( new ModelBuilder( controllers, model ) )
    , glProxy_     ( 0 )
    , widget2d_    ( 0 )
    , iconLayout_  ( 0 )
{
    setIcon( MAKE_PIXMAP( astec ) );
    setCaption( APP_NAME );

    PreferencesDialog* prefDialog = new PreferencesDialog( this, controllers );
    new Dialogs( this, controllers, model_, staticModel );

    glProxy_ = new GlProxy();
    strategy_ = new ColorStrategy( controllers, *glProxy_ );

    RichItemFactory* factory = new RichItemFactory( this ); // $$$$ AGE 2006-05-11: aggregate somewhere
//    LinkInterpreter* interpreter = new LinkInterpreter( this, controllers );
//    connect( factory, SIGNAL( LinkClicked( const QString& ) ), interpreter, SLOT( Interprete( const QString& ) ) );
//
    // Agent list panel
    QDockWindow* pListDockWnd_ = new QDockWindow( this );
    moveDockWindow( pListDockWnd_, Qt::DockLeft );
    QTabWidget* pListsTabWidget = new QTabWidget( pListDockWnd_ );
    
    QTabWidget* pAgentsTabWidget = new QTabWidget( pListsTabWidget );
    QVBox* listsTabBox = new QVBox( pListsTabWidget );
    new EntitySearchBox< Agent_ABC >( listsTabBox, controllers );
    new TacticalListView( listsTabBox, controllers, *factory, *modelBuilder_, model_.formations_.levels_ );
    pAgentsTabWidget->addTab( listsTabBox, tr( "Tactique" ) );
    
    listsTabBox = new QVBox( pListsTabWidget );
    new EntitySearchBox< Agent_ABC >( listsTabBox, controllers );
    new ::CommunicationListView( listsTabBox, controllers, *factory, *modelBuilder_ );
    pAgentsTabWidget->addTab( listsTabBox, tr( "Communication" ) );

    pListsTabWidget->addTab( pAgentsTabWidget, tr( "Agents" ) );

//    NatureEditionWidget* nature = new NatureEditionWidget( pListsTabWidget, "symbols.xml" );
//    nature->setText( "combat service support/transportation/spod spoe" );
//    pListsTabWidget->addTab( nature, tr( "Test" ) );

    listsTabBox = new QVBox( pListsTabWidget );
    new EntitySearchBox< Object_ABC >( listsTabBox, controllers );
    new ::ObjectListView( listsTabBox, controllers, *factory );
    pListsTabWidget->addTab( listsTabBox, tr( "Objets" ) );
    pListsTabWidget->addTab( new PopulationList( controllers, *factory ), tr( "Populations" ) );
	pListDockWnd_->setWidget( pListsTabWidget );
    pListDockWnd_->setResizeEnabled( true );
    pListDockWnd_->setCloseMode( QDockWindow::Always );
    pListDockWnd_->setCaption( tr( "Unités" ) );
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
    InfoPanels* pInfoPanel_ = new InfoPanels( pInfoDockWnd_, controllers, staticModel_, *factory );
    pInfoDockWnd_->setWidget( pInfoPanel_ );
    pInfoDockWnd_->setResizeEnabled( true );
    pInfoDockWnd_->setCloseMode( QDockWindow::Always );
    pInfoDockWnd_->setCaption( tr( "Informations" ) );
    setDockEnabled( pInfoDockWnd_, Qt::DockTop, false );

    Profile_ABC& profile = *new PreparationProfile( this );

    // A few layers
    ParametersLayer* paramLayer = new ParametersLayer( *glProxy_ );
    ::AgentsLayer* agentsLayer = new ::AgentsLayer( controllers, *glProxy_, *strategy_, *glProxy_, model_, *modelBuilder_, profile );

    // object creation window
    ObjectCreationPanel* objectCreationPanel = new ObjectCreationPanel( pCreationDockWnd, *pCreationPanel, controllers, staticModel_, model.teams_, *paramLayer, *glProxy_ );
    pCreationPanel->AddPanel( objectCreationPanel );

//    new MapToolbar( this, controllers );
//    new UnitToolbar( this, controllers );
//    new LogisticToolbar( this, controllers, layers_->GetAgentLayer() ); // $$$$ AGE 2006-05-02: 

    new Menu( this, controllers, *prefDialog );

    glPlaceHolder_ = new GlPlaceHolder( this );
    setCentralWidget( glPlaceHolder_ );

    // $$$$ AGE 2006-08-22: prefDialog->GetPreferences()
    CreateLayers( *objectCreationPanel, *paramLayer, *agentsLayer, prefDialog->GetPreferences(), profile );

    pStatus_ = new StatusBar( statusBar(), staticModel_.detection_, staticModel_.coordinateConverter_ );
//    controllers_.Register( *this );

    displayTimer_ = new QTimer( this );

    ReadSettings();
    ReadOptions();
}


// -----------------------------------------------------------------------------
// Name: MainWindow::CreateLayers
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void MainWindow::CreateLayers( ObjectCreationPanel& objects, ParametersLayer& parameters, ::AgentsLayer& agents, GraphicSetup_ABC& setup, const Profile_ABC& profile )
{
    CircularEventStrategy* eventStrategy = new CircularEventStrategy();
    eventStrategy_ = eventStrategy;
    Layer_ABC& objectCreationLayer  = *new MiscLayer< ObjectCreationPanel >( objects );
    Layer_ABC& elevation2d          = *new Elevation2dLayer( controllers_.controller_, staticModel_.detection_ );
    Layer_ABC& terrain              = *new TerrainLayer( controllers_, *glProxy_, setup );
    Layer_ABC& grid                 = *new GridLayer( controllers_, *glProxy_ );
    Layer_ABC& metrics              = *new MetricsLayer( *glProxy_ );
//    Layer_ABC& limits               = *new LimitsLayer( controllers_, *glProxy_, *strategy_, parameters, model_.limits_ );
    Layer_ABC& objectsLayer         = *new ::ObjectsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
    Layer_ABC& populations          = *new PopulationsLayer( controllers_, *glProxy_, *strategy_, *glProxy_, profile );
//    Layer_ABC& meteo                = *new MeteoLayer( controllers_, *glProxy_ );
    Layer_ABC& defaultLayer         = *new DefaultLayer( controllers_ );
    
    // ordre de dessin
    glProxy_->Register( defaultLayer );
    glProxy_->Register( elevation2d );
    glProxy_->Register( terrain );
    glProxy_->Register( grid );
//    glProxy_->Register( meteo );
//    glProxy_->Register( limits );
    glProxy_->Register( objectsLayer );
    glProxy_->Register( populations );
    glProxy_->Register( agents );
    glProxy_->Register( objectCreationLayer );
    glProxy_->Register( parameters );
    glProxy_->Register( metrics );

    // ordre des evenements
    eventStrategy->Register( parameters );
    eventStrategy->Register( agents );
    eventStrategy->Register( populations );
    eventStrategy->Register( objectsLayer );
//    eventStrategy->Register( limits );
    eventStrategy->Register( metrics );
    eventStrategy->SetDefault( defaultLayer );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::New
// Created: AGE 2006-05-03
// -----------------------------------------------------------------------------
void MainWindow::New()
{
    std::string current;
    while( ! bfs::exists( bfs::path( current, bfs::native ) ) )
    {
        const QString filename = QFileDialog::getOpenFileName( "../data/", "Scipio (*.xml)", 0, 0, "Choose scipio.xml" );
        if( filename.isEmpty() )
            return;
        current = filename;
        if( current.substr( 0, 2 ) == "//" )
            std::replace( current.begin(), current.end(), '/', '\\' );
    }
    Load( current );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Open
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
void MainWindow::Open()
{
    New();
    try
    {
        if( !scipioXml_.empty() )
            model_.Load( scipioXml_ ); // $$$$ SBO 2006-10-05: should be a different file
    }
    catch( InvalidModelException& e )
    {
        Close();
        QMessageBox::critical( 0, APP_NAME, e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Load
// Created: AGE 2006-05-03
// -----------------------------------------------------------------------------
void MainWindow::Load( const std::string& scipioXml )
{
    BuildIconLayout();
    scipioXml_ = scipioXml;
    delete widget2d_; widget2d_ = 0;
    widget2d_ = new GlWidget( this, controllers_, scipioXml, *iconLayout_, *eventStrategy_ );
    glProxy_->ChangeTo( widget2d_ );
    glProxy_->RegisterTo( widget2d_ );
    delete glPlaceHolder_; glPlaceHolder_ = 0;
    setCentralWidget( widget2d_ );
    model_.Purge();
    staticModel_.Load( scipioXml );

    connect( widget2d_, SIGNAL( MouseMove( const geometry::Point2f& ) ), pStatus_, SLOT( OnMouseMove( const geometry::Point2f& ) ) );
    connect( displayTimer_, SIGNAL( timeout()), centralWidget(), SLOT( updateGL() ) );
    displayTimer_->start( 50 );
    widget2d_->show();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Close
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void MainWindow::Close()
{
    model_.Purge();
    staticModel_.Purge();
    glPlaceHolder_ = new GlPlaceHolder( this );
    setCentralWidget( glPlaceHolder_ );
    glPlaceHolder_->show();

    delete widget2d_; widget2d_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MainWindow::Save
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void MainWindow::Save()
{
    std::string current;
    const QString filename = QFileDialog::getSaveFileName( "../data/", "Ordre de bataille (*.xml)", 0, 0, "Sauvegarde ODB" );
    if( filename.isEmpty() )
        return;
    current = filename;
    if( current.substr( 0, 2 ) == "//" )
        std::replace( current.begin(), current.end(), '/', '\\' );
    xml::xofstream xos( current, xml::encoding( "ISO-8859-1" ) );
    model_.Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: MainWindow destructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
//    controllers_.Remove( *this );
//    delete pOptions_;
    delete glProxy_;
    delete modelBuilder_;
}

// -----------------------------------------------------------------------------
// Name: MainWindow::closeEvent
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void MainWindow::closeEvent( QCloseEvent* pEvent )
{
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
    settings.setPath( "MASA", "Astec" );
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
    settings.setPath( "MASA", "Astec" );
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
    settings.setPath( "MASA", "Astec" );
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
    settings.setPath( "MASA", "Astec" );
    settings.beginGroup( "/Preparation" );
    settings.beginGroup( "/Options" );
    controllers_.options_.Load( settings );
    settings.endGroup();
    settings.endGroup();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::BuildIconLayout
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
void MainWindow::BuildIconLayout()
{
    if( iconLayout_ )
        return;
    iconLayout_ = new IconLayout();
//    iconLayout_->AddIcon( xpm_cadenas        , -200, 270 );
//    iconLayout_->AddIcon( xpm_radars_on      ,  200, 270 );
//    iconLayout_->AddIcon( xpm_brouillage     ,  200, 50 );
//    iconLayout_->AddIcon( xpm_talkie_interdit,  100, 50 );
//    iconLayout_->AddIcon( xpm_gas            , -200, 170 );
//    iconLayout_->AddIcon( xpm_ammo           , -200, 100 );
//    iconLayout_->AddIcon( xpm_nbc            , -200, 25 );
//    iconLayout_->AddIcon( xpm_construction   ,  200, 150 );
//    iconLayout_->AddIcon( xpm_observe        ,  200, 150 );
}
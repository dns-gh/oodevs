// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-01 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/MainWindow.cpp $
// $Author: Age $
// $Modtime: 17/05/05 14:05 $
// $Revision: 20 $
// $Workfile: MainWindow.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "MainWindow.h"
#include "moc_MainWindow.cpp"

#include "App.h"
#include "World.h"
#include "GL3DWidget.h"
#include "GLWidget.h"
#include "SIMControlToolbar.h"
#include "AgentListView.h"
#include "ObjectListView.h"
#include "PopulationListView.h"
#include "InfoPanel.h"
#include "AgentKnowledgePanel.h"
#include "ObjectKnowledgePanel.h"
#include "MapToolbar.h"
#include "UnitToolbar.h"
#include "EventToolbar.h"
#include "ControllerToolbar.h"
#include "RecorderToolbar.h"
#include "ODBToolbar.h"
#include "LogisticToolbar.h"
#include "MissionPanel.h"
#include "WeatherPanel.h"
#include "MT_Settings.h"
#include "Logger.h"
#include "ObjectCreationPanel.h"
#include "AgentCreationPanel.h"
#include "PopulationCreationPanel.h"
#include "OptionsPanel.h"
#include "DefaultMapEventHandler.h"
#include "Network.h"

MainWindow* MainWindow::pInstance_ = 0;

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::MainWindow()
    : QMainWindow  ( 0, 0, Qt::WDestructiveClose )
    , pGL3DWidget_ ( 0 )
    , pGLWidget_   ( 0 )
{
    assert( pInstance_ == 0 );
    pInstance_ = this;

    this->setIcon( MAKE_PIXMAP( astec ) );
    this->setCaption( APP_NAME );

    if( App::GetApp().Is3DEnabled() )
        pGL3DWidget_ = new GL3DWidget( this );
    pGLWidget_   = new GLWidget( this, pGL3DWidget_ );
    DimensionsChanged();

    // Map event handling
    DefaultMapEventHandler* pHandler = new DefaultMapEventHandler( this );
    pHandler->Push();

    // Agent list panel
    pListDockWnd_ = new QDockWindow( this );
    this->moveDockWindow( pListDockWnd_, Qt::DockLeft );
    pListDockWnd_->hide();
    QTabWidget* pListsTabWidget = new QTabWidget( pListDockWnd_ );
    pAgentList_ = new AgentListView( pListsTabWidget );
    pListsTabWidget->addTab( pAgentList_, tr( "Agents" ) );
    pAgentList_->header()->hide();
    pObjectList_ = new ObjectListView( pListsTabWidget );
    pListsTabWidget->addTab( pObjectList_, tr( "Objets" ) );
    pObjectList_->header()->hide();
    pPopulationList_ = new PopulationListView( pListsTabWidget );
	pListsTabWidget->addTab( pPopulationList_, tr( "Populations" ) );
	pPopulationList_->header()->hide();
	pListDockWnd_->setWidget( pListsTabWidget );
    pListDockWnd_->setResizeEnabled( true );
    pListDockWnd_->setCloseMode( QDockWindow::Always );
    pListDockWnd_->setCaption( tr( "Unités" ) );
    this->setDockEnabled( pListDockWnd_, Qt::DockTop, false );

    // Object creation panel
    pCreationDockWnd_ = new QDockWindow( this );
    this->moveDockWindow( pCreationDockWnd_, Qt::DockRight );
    pCreationDockWnd_->hide();
    QTabWidget* pTabWidget = new QTabWidget( pCreationDockWnd_ );
    pObjCreationPanel_ = new ObjectCreationPanel( pTabWidget );
    pTabWidget->addTab( pObjCreationPanel_, tr( "Création objet" ) );
    if( App::GetApp().IsODBEdition() )
    {
        pAgentCreationPanel_ = new AgentCreationPanel( pTabWidget );
        pTabWidget->addTab( pAgentCreationPanel_, tr("Création agent" ) );
        pPopulationCreationPanel_ = new PopulationCreationPanel( pTabWidget );
        pTabWidget->addTab( pPopulationCreationPanel_, tr("Création Population") );
    }
    pCreationDockWnd_->setWidget( pTabWidget );
    pCreationDockWnd_->setResizeEnabled( true );
    pCreationDockWnd_->setCloseMode( QDockWindow::Always );
    pCreationDockWnd_->setCaption( tr( "Création" ) );
    this->setDockEnabled( pCreationDockWnd_, Qt::DockTop, false );

    if( App::GetApp().IsODBEdition() )
    {
        this->ReadSettings();
        pListDockWnd_->show();
        pCreationDockWnd_->show();

        new ODBToolbar( this );
        new MapToolbar( this );
        new LogisticToolbar( this );
        return;
    }

    // Info panel
    pInfoDockWnd_ = new QDockWindow( this );
    this->moveDockWindow( pInfoDockWnd_, Qt::DockRight );
    pInfoDockWnd_->hide();
    pInfoPanel_ = new InfoPanel( pInfoDockWnd_ );
    pInfoDockWnd_->setWidget( pInfoPanel_ );
    pInfoDockWnd_->setResizeEnabled( true );
    pInfoDockWnd_->setCloseMode( QDockWindow::Always );
    pInfoDockWnd_->setCaption( tr( "Informations" ) );
    this->setDockEnabled( pInfoDockWnd_, Qt::DockTop, false );

    // Mission panel
    pMissionPanel_ = new MissionPanel( this );
    this->moveDockWindow( pMissionPanel_, Qt::DockLeft );
    pMissionPanel_->hide();
    this->setDockEnabled( pMissionPanel_, Qt::DockTop, false );
    this->setAppropriate( pMissionPanel_, false );

    // Weather panel
    pWeatherDockWnd_ = new QDockWindow( this );
    this->moveDockWindow( pWeatherDockWnd_, Qt::DockRight );
    pWeatherDockWnd_->hide();
    pWeatherPanel_ = new WeatherPanel( pWeatherDockWnd_ );
    pWeatherDockWnd_->setWidget( pWeatherPanel_ );
    pWeatherDockWnd_->setResizeEnabled( true );
    pWeatherDockWnd_->setCloseMode( QDockWindow::Always );
    pWeatherDockWnd_->setCaption( tr( "Météo" ) );
    this->setDockEnabled( pWeatherDockWnd_, Qt::DockTop, false );

    // Logger
    pLogDockWnd_ = new QDockWindow( this );
    this->moveDockWindow( pLogDockWnd_, Qt::DockBottom );
    pLogDockWnd_->hide();
    pLogPanel_ = new Logger( pLogDockWnd_ );
    pLogDockWnd_->setWidget( pLogPanel_ );
    pLogDockWnd_->setResizeEnabled( true );
    pLogDockWnd_->setCloseMode( QDockWindow::Always );
    pLogDockWnd_->setCaption( tr( "Log" ) );
    this->setDockEnabled( pLogDockWnd_, Qt::DockTop, false );

    // Options window
    pOptionsDockWnd_ = new QDockWindow( this );
    this->moveDockWindow( pOptionsDockWnd_, Qt::DockRight );
    pOptionsDockWnd_->hide();
    OptionsPanel* pOptionsPanel = new OptionsPanel( pOptionsDockWnd_ );
    pOptionsDockWnd_->setWidget( pOptionsPanel );
    pOptionsDockWnd_->setResizeEnabled( true );
    pOptionsDockWnd_->setCloseMode( QDockWindow::Always );
    pOptionsDockWnd_->setCaption( tr( "Options" ) );
    this->setDockEnabled( pOptionsDockWnd_, Qt::DockTop, false );

    // Toolbars
    new SIMControlToolbar( this );
    new MapToolbar( this );
    new UnitToolbar( this );
    new EventToolbar( this );
    new ControllerToolbar( this );
    new LogisticToolbar( this );
    new RecorderToolbar( this );
    
    // Status bar & indicators
    pLocationLabel_  = new QLabel( "---", this );
	pLocationLabel2_ = new QLabel( "---", this );
	pLocationLabel3_ = new QLabel( "---", this );
    pTimeLabel_      = new QLabel( "---", this );
	pTickLabel_      = new QLabel( this );
    pTickLabel_->setAlignment( Qt::AlignCenter );
    pTickLabel_->setPixmap( MAKE_PIXMAP( tickoff ) );
    pTimeLabel_->setMinimumWidth( 50 );
    pNetworkStats_   = new QLabel( "per tick/total", this );
    pLocationLabel_->setMinimumWidth( 195 );
	pLocationLabel2_->setMinimumWidth( 105 );
	pLocationLabel3_->setMinimumWidth( 125 );
    pLocationLabel_->setAlignment( Qt::AlignCenter );
    pLocationLabel2_->setAlignment( Qt::AlignCenter );
    pLocationLabel3_->setAlignment( Qt::AlignCenter );
    pTimeLabel_->setAlignment( Qt::AlignCenter );

    statusBar()->addWidget( pLocationLabel_,  0, true );
	statusBar()->addWidget( pLocationLabel2_, 0, true );
	statusBar()->addWidget( pLocationLabel3_, 0, true );
    statusBar()->addWidget( pTimeLabel_,      0, true );
	statusBar()->addWidget( pTickLabel_,      0, true );
    statusBar()->addWidget( pNetworkStats_,   0, true );

    pLagTimer_ = new QTimer( this );

    // Read the settings to place the windows in their correct locations.
    this->ReadSettings();

    // Connexions
    connect( pGLWidget_  , SIGNAL( MouseMove( QMouseEvent*, const MT_Vector2D& ) ), this, SLOT( DisplayMouseLocation( QMouseEvent*, const MT_Vector2D& ) ) );
    if( App::GetApp().Is3DEnabled() )
        connect( pGL3DWidget_, SIGNAL( MouseMove( QMouseEvent*, const MT_Vector2D& ) ), this, SLOT( DisplayMouseLocation( QMouseEvent*, const MT_Vector2D& ) ) );
    connect( &App::GetApp(), SIGNAL( ConnexionStatusChanged( bool ) ),  this, SLOT( OnConnexionStatusChanged( bool ) ) );
    connect( &App::GetApp(), SIGNAL( TimeChanged( uint ) ),             this, SLOT( DisplayTime( uint ) ) );
    connect( &App::GetApp(), SIGNAL( TickStartEnd( bool ) ),            this, SLOT( OnTickStartEnd( bool ) ) );
}


// -----------------------------------------------------------------------------
// Name: MainWindow destructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    assert( pInstance_ != 0 );
    pInstance_ = 0;
}


// -----------------------------------------------------------------------------
// Name: MainWindow::Update
// Created: APE 2004-03-23
// -----------------------------------------------------------------------------
void MainWindow::Update()
{
    if( App::GetApp().Is3D() )
        pGL3DWidget_->update();
    else
        pGLWidget_->update();
}


// -----------------------------------------------------------------------------
// Name: MainWindow::DisplayTime
/** @param  strMsg 
*/
// Created: APE 2004-04-27
// -----------------------------------------------------------------------------
void MainWindow::DisplayTime( uint nTimeInSeconds )
{
    QString strTime( MT_FormatString( "%02d:%02d:%02d", ( nTimeInSeconds / 3600 ) % 24, ( nTimeInSeconds / 60 ) % 60 , nTimeInSeconds % 60  ).c_str() );
    pTimeLabel_->setText( strTime );
}


// -----------------------------------------------------------------------------
// Name: MainWindow::DisplayMouseLocation
/** @param  strMsg 
*/
// Created: APE 2004-04-27
// -----------------------------------------------------------------------------
void MainWindow::DisplayMouseLocation( QMouseEvent* pEvent, const MT_Vector2D& vGLPos )
{
    const MT_Rect& rectWorld = App::GetApp().GetWorld().GetRect();
    if( pEvent == 0 || ! rectWorld.IsInside( vGLPos ) )
	{
        pLocationLabel_->setText( "---" );
		pLocationLabel2_->setText( "---" );
        pLocationLabel3_->setText( "---" );
	}
	else
    {
		// Display x,y,z position
		RawVisionData& visionData = App::GetApp().GetRawVisionData();
		pLocationLabel_->setText( MT_FormatString( "x:%.2f y:%.2f h:%.2f ( o:%.2f )", vGLPos.rX_, vGLPos.rY_, visionData.GetHeight( vGLPos ), visionData.GetVisionObjectHeight( vGLPos ) ).c_str() );

		// Display Mgrs coord
        std::string strLoc;
        App::GetApp().GetWorld().SimToMosMgrsCoord( vGLPos, strLoc );
        pLocationLabel2_->setText( strLoc.c_str() );

		// Display Lat/Long
        MT_Vector2D vPosDMS;
        App::GetApp().GetWorld().MgrsToGeoCoord( strLoc, vPosDMS );        
		pLocationLabel3_->setText( MT_FormatString( "Lat:%.3f Lon:%.3f", vPosDMS.rY_, vPosDMS.rX_ ).c_str() );
    }
}


namespace
{
    std::string ToUSI( uint bytes )
    {
        static const char* units[] = { "B", "KB", "MB", "GB" };
        unsigned int i = 0;
        while( bytes >> 10 > 0 && i < 4 )
        {
            bytes /= 1024;
            ++i;
        }
        std::stringstream ss;
        ss << bytes << units[i];
        return ss.str();
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::OnTickStartEnd
// Created: APE 2004-06-17
// -----------------------------------------------------------------------------
void MainWindow::OnTickStartEnd( bool bTickStart )
{
    if( bTickStart )
    {
        pLagTimer_->stop();
        pTickLabel_->setPixmap( MAKE_PIXMAP( tickon ) );
    }
    else
    {
        pTickLabel_->setPixmap( MAKE_PIXMAP( tickoff ) );
        pLagTimer_->start( 10000, true );
        std::stringstream ss;
        ss << ToUSI( App::GetApp().GetNetwork().ComputeTransferedAmount() / App::GetApp().GetTime() ) << "/s - " << ToUSI( App::GetApp().GetNetwork().ComputeTransferedAmount() );
        pNetworkStats_->setText( ss.str().c_str() );
    }
}


// -----------------------------------------------------------------------------
// Name: MainWindow::OnLag
// Created: APE 2004-07-30
// -----------------------------------------------------------------------------
void MainWindow::OnLag()
{
    pTickLabel_->setPixmap( MAKE_PIXMAP( tickred ) );
}


// -----------------------------------------------------------------------------
// Name: MainWindow::OnConnexionStatusChanged
/** @param  b 
*/
// Created: APE 2004-07-30
// -----------------------------------------------------------------------------
void MainWindow::OnConnexionStatusChanged( bool b )
{
    if( !b )
    {
        pLagTimer_->stop();
        pTickLabel_->setPixmap( MAKE_PIXMAP( tickoff ) );
        pTimeLabel_->setText( "---" );

//        pOptions_
    }
    else
    {
        pLagTimer_->start( 10000, true );
    }
}


// -----------------------------------------------------------------------------
// Name: MainWindow::closeEvent
/** @param  pEvent 
*/
// Created: APE 2004-06-01
// -----------------------------------------------------------------------------
void MainWindow::closeEvent( QCloseEvent* pEvent )
{
    // Not done in the destructor, as the windows would already be hidden and we wouldn't
    // be able to correctly record their hidden/visible state.
    WriteSettings();
    App::GetApp().GetOptions().Write();

    QMainWindow::closeEvent( pEvent );
}


// -----------------------------------------------------------------------------
// Name: MainWindow::WriteSettings
// Created: APE 2004-06-01
// -----------------------------------------------------------------------------
void MainWindow::WriteSettings()
{
    MT_Settings settings;
    settings.setPath( "MASA", "Astec" );

    // Pannel configuration
    QString strDockConfig;
    QTextStream strDockConfigStream( &strDockConfig, IO_WriteOnly );
    strDockConfigStream << *this;

    if( App::GetApp().IsODBEdition() )
        settings.writeEntry( "/ODBEditorPanels", strDockConfig );
    else
        settings.writeEntry( "/Panels", strDockConfig );

    // Main window configuration
    settings.WriteEntry( "/MainWindow", *this );
}


// -----------------------------------------------------------------------------
// Name: MainWindow::ReadSettings
// Created: APE 2004-06-01
// -----------------------------------------------------------------------------
void MainWindow::ReadSettings()
{
    MT_Settings settings;
    settings.setPath( "MASA", "Astec" );

    // Pannel configuration
    QString strDockConfig;
    if( App::GetApp().IsODBEdition() )
        strDockConfig = settings.readEntry( "/ODBEditorPanels" );
    else
        strDockConfig = settings.readEntry( "/Panels" );

    QTextStream strDockConfigStream( &strDockConfig, IO_ReadOnly );
    strDockConfigStream >> *this;

    // Main window configuration
    settings.ReadEntry( "/MainWindow", *this, 800, 600, 100, 100, false );

    if( App::GetApp().IsODBEdition() )
        return;

    // Always keep the mission panel hidden hidden at launch.
    pMissionPanel_->hide();

    if( ! App::GetApp().GetOptions().bControllerMode_ )
    {
        setAppropriate( pCreationDockWnd_, false );
        setAppropriate( pWeatherDockWnd_, false );
        pCreationDockWnd_->hide();
        pWeatherDockWnd_->hide();
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::DimensionsChanged
// Created: AGE 2005-05-13
// -----------------------------------------------------------------------------
void MainWindow::DimensionsChanged()
{
    if( App::GetApp().Is3D() )
    {
        pGLWidget_->hide();
        setCentralWidget( pGL3DWidget_ );
        pGL3DWidget_->show();
    }
    else
    {
        if( App::GetApp().Is3DEnabled() )
            pGL3DWidget_->hide();
        setCentralWidget( pGLWidget_ );
        pGLWidget_->show();
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow::PushMapEventFilter
// Created: AGE 2005-05-17
// -----------------------------------------------------------------------------
void MainWindow::PushMapEventFilter( MapEventFilter_ABC& filter )
{
    if( App::GetApp().Is3DEnabled() )
        pGL3DWidget_->PushMapEventFilter( filter );
    pGLWidget_->PushMapEventFilter( filter );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::PopMapEventFilter
// Created: AGE 2005-05-17
// -----------------------------------------------------------------------------
void MainWindow::PopMapEventFilter( MapEventFilter_ABC& filter )
{
    if( App::GetApp().Is3DEnabled() )
        pGL3DWidget_->PopMapEventFilter( filter );
    pGLWidget_->PopMapEventFilter( filter );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::GetMapEvents
// Created: AGE 2005-05-17
// -----------------------------------------------------------------------------
const T_MapEventFilterVector& MainWindow::GetMapEvents() const
{
    if( App::GetApp().Is3D() )
        return pGL3DWidget_->GetMapEvents();
    return pGLWidget_->GetMapEvents();
}

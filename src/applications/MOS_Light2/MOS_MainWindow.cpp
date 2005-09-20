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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_MainWindow.cpp $
// $Author: Age $
// $Modtime: 17/05/05 14:05 $
// $Revision: 20 $
// $Workfile: MOS_MainWindow.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_MainWindow.h"
#include "moc_MOS_MainWindow.cpp"

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_GL3DWidget.h"
#include "MOS_GLWidget.h"
#include "MOS_SIMControlToolbar.h"
#include "MOS_AgentListView.h"
#include "MOS_ObjectListView.h"
#include "MOS_InfoPanel.h"
#include "MOS_AgentKnowledgePanel.h"
#include "MOS_ObjectKnowledgePanel.h"
#include "MOS_MapToolbar.h"
#include "MOS_UnitToolbar.h"
#include "MOS_EventToolbar.h"
#include "MOS_ControllerToolbar.h"
#include "MOS_RecorderToolbar.h"
#include "MOS_ODBToolbar.h"
#include "MOS_MissionPanel.h"
#include "MOS_WeatherPanel.h"
#include "MT_Settings.h"
#include "MOS_Logger.h"
#include "MOS_ObjectCreationPanel.h"
#include "MOS_AgentCreationPanel.h"
#include "MOS_OptionsPanel.h"
#include "MOS_DefaultMapEventHandler.h"

MOS_MainWindow* MOS_MainWindow::pInstance_ = 0;

// -----------------------------------------------------------------------------
// Name: MOS_MainWindow constructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MOS_MainWindow::MOS_MainWindow()
    : QMainWindow( 0, 0, Qt::WDestructiveClose )
{
    assert( pInstance_ == 0 );
    pInstance_ = this;

    this->setIcon( MAKE_PIXMAP( mosicon ) );
    this->setCaption( "MOSLight 2" );

    pOptions_ = new MOS_Options();
    this->ReadOptions();

    pGL3DWidget_ = new MOS_GL3DWidget( this );
    pGLWidget_   = new MOS_GLWidget( this, pGL3DWidget_ );
    DimensionsChanged();

    // Map event handling
    MOS_DefaultMapEventHandler* pHandler = new MOS_DefaultMapEventHandler( this );
    pHandler->Push();

    // Agent list panel
    pListDockWnd_ = new QDockWindow( this );
    this->moveDockWindow( pListDockWnd_, Qt::DockLeft );
    pListDockWnd_->hide();
    QTabWidget* pListsTabWidget = new QTabWidget( pListDockWnd_ );
    pAgentList_ = new MOS_AgentListView( pListsTabWidget );
    pListsTabWidget->addTab( pAgentList_, tr( "Agents" ) );
    pAgentList_->header()->hide();
    pObjectList_ = new MOS_ObjectListView( pListsTabWidget );
    pListsTabWidget->addTab( pObjectList_, tr( "Objets" ) );
    pObjectList_->header()->hide();
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
    pObjCreationPanel_ = new MOS_ObjectCreationPanel( pTabWidget );
    pTabWidget->addTab( pObjCreationPanel_, tr( "Création objet" ) );
    if( MOS_App::GetApp().IsODBEdition() )
    {
        pAgentCreationPanel_ = new MOS_AgentCreationPanel( pTabWidget );
        pTabWidget->addTab( pAgentCreationPanel_, tr("Création agent" ) );
    }
    pCreationDockWnd_->setWidget( pTabWidget );
    pCreationDockWnd_->setResizeEnabled( true );
    pCreationDockWnd_->setCloseMode( QDockWindow::Always );
    pCreationDockWnd_->setCaption( tr( "Création" ) );
    this->setDockEnabled( pCreationDockWnd_, Qt::DockTop, false );

    if( MOS_App::GetApp().IsODBEdition() )
    {
        this->ReadSettings();
        pListDockWnd_->show();
        pCreationDockWnd_->show();

        new MOS_ODBToolbar( this );
        new MOS_MapToolbar( this );
        return;
    }

    // Info panel
    pInfoDockWnd_ = new QDockWindow( this );
    this->moveDockWindow( pInfoDockWnd_, Qt::DockRight );
    pInfoDockWnd_->hide();
    pInfoPanel_ = new MOS_InfoPanel( pInfoDockWnd_ );
    pInfoDockWnd_->setWidget( pInfoPanel_ );
    pInfoDockWnd_->setResizeEnabled( true );
    pInfoDockWnd_->setCloseMode( QDockWindow::Always );
    pInfoDockWnd_->setCaption( tr( "Informations" ) );
    this->setDockEnabled( pInfoDockWnd_, Qt::DockTop, false );

    // Mission panel
    pMissionPanel_ = new MOS_MissionPanel( this );
    this->moveDockWindow( pMissionPanel_, Qt::DockLeft );
    pMissionPanel_->hide();
    this->setDockEnabled( pMissionPanel_, Qt::DockTop, false );
    this->setAppropriate( pMissionPanel_, false );

    // Weather panel
    pWeatherDockWnd_ = new QDockWindow( this );
    this->moveDockWindow( pWeatherDockWnd_, Qt::DockRight );
    pWeatherDockWnd_->hide();
    pWeatherPanel_ = new MOS_WeatherPanel( pWeatherDockWnd_ );
    pWeatherDockWnd_->setWidget( pWeatherPanel_ );
    pWeatherDockWnd_->setResizeEnabled( true );
    pWeatherDockWnd_->setCloseMode( QDockWindow::Always );
    pWeatherDockWnd_->setCaption( tr( "Météo" ) );
    this->setDockEnabled( pWeatherDockWnd_, Qt::DockTop, false );

    // Logger
    pLogDockWnd_ = new QDockWindow( this );
    this->moveDockWindow( pLogDockWnd_, Qt::DockBottom );
    pLogDockWnd_->hide();
    pLogPanel_ = new MOS_Logger( pLogDockWnd_ );
    pLogDockWnd_->setWidget( pLogPanel_ );
    pLogDockWnd_->setResizeEnabled( true );
    pLogDockWnd_->setCloseMode( QDockWindow::Always );
    pLogDockWnd_->setCaption( tr( "Log" ) );
    this->setDockEnabled( pLogDockWnd_, Qt::DockTop, false );

    // Options window
    pOptionsDockWnd_ = new QDockWindow( this );
    this->moveDockWindow( pOptionsDockWnd_, Qt::DockRight );
    pOptionsDockWnd_->hide();
    MOS_OptionsPanel* pOptionsPanel = new MOS_OptionsPanel( pOptionsDockWnd_ );
    pOptionsDockWnd_->setWidget( pOptionsPanel );
    pOptionsDockWnd_->setResizeEnabled( true );
    pOptionsDockWnd_->setCloseMode( QDockWindow::Always );
    pOptionsDockWnd_->setCaption( tr( "Options" ) );
    this->setDockEnabled( pOptionsDockWnd_, Qt::DockTop, false );

    // Toolbars
    new MOS_SIMControlToolbar( this );
    new MOS_MapToolbar( this );
    new MOS_UnitToolbar( this );
    new MOS_EventToolbar( this );
    new MOS_ControllerToolbar( this );
    new MOS_RecorderToolbar( this );

    // Status bar & indicators
    pLocationLabel_  = new QLabel( "---", this );
	pLocationLabel2_ = new QLabel( "---", this );
	pLocationLabel3_ = new QLabel( "---", this );
    pTimeLabel_      = new QLabel( "---", this );
	pTickLabel_      = new QLabel( this );
    pTickLabel_->setAlignment( Qt::AlignCenter );
    pTickLabel_->setPixmap( MAKE_PIXMAP( tickoff ) );
    pTimeLabel_->setMinimumWidth( 50 );
    pLocationLabel_->setMinimumWidth( 195 );
	pLocationLabel2_->setMinimumWidth( 105 );
	pLocationLabel3_->setMinimumWidth( 125 );
    pLocationLabel_->setAlignment( Qt::AlignCenter );
    pLocationLabel2_->setAlignment( Qt::AlignCenter );
    pLocationLabel3_->setAlignment( Qt::AlignCenter );
    pTimeLabel_->setAlignment( Qt::AlignCenter );

    this->statusBar()->addWidget( pLocationLabel_,  0, true );
	this->statusBar()->addWidget( pLocationLabel2_, 0, true );
	this->statusBar()->addWidget( pLocationLabel3_, 0, true );
    this->statusBar()->addWidget( pTimeLabel_,      0, true );
	this->statusBar()->addWidget( pTickLabel_,      0, true );

    pLagTimer_ = new QTimer( this );

    // Read the settings to place the windows in their correct locations.
    this->ReadSettings();

    // Connexions
    connect( pGLWidget_  , SIGNAL( MouseMove( QMouseEvent*, const MT_Vector2D& ) ), this, SLOT( DisplayMouseLocation( QMouseEvent*, const MT_Vector2D& ) ) );
    connect( pGL3DWidget_, SIGNAL( MouseMove( QMouseEvent*, const MT_Vector2D& ) ), this, SLOT( DisplayMouseLocation( QMouseEvent*, const MT_Vector2D& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( ConnexionStatusChanged( bool ) ),  this, SLOT( OnConnexionStatusChanged( bool ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( TimeChanged( uint ) ),             this, SLOT( DisplayTime( uint ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( TickStartEnd( bool ) ),            this, SLOT( OnTickStartEnd( bool ) ) );
    connect( pLagTimer_,         SIGNAL( timeout() ),                       this, SLOT( OnLag() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MainWindow destructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MOS_MainWindow::~MOS_MainWindow()
{
    assert( pInstance_ != 0 );

    delete pOptions_;

    pInstance_ = 0;
}


// -----------------------------------------------------------------------------
// Name: MOS_MainWindow::Update
// Created: APE 2004-03-23
// -----------------------------------------------------------------------------
void MOS_MainWindow::Update()
{
    if( MOS_App::GetApp().Is3D() )
        pGL3DWidget_->update();
    else
        pGLWidget_->update();
}


// -----------------------------------------------------------------------------
// Name: MOS_MainWindow::DisplayTime
/** @param  strMsg 
*/
// Created: APE 2004-04-27
// -----------------------------------------------------------------------------
void MOS_MainWindow::DisplayTime( uint nTimeInSeconds )
{
    QString strTime( MT_FormatString( "%02d:%02d:%02d", ( nTimeInSeconds / 3600 ) % 24, ( nTimeInSeconds / 60 ) % 60 , nTimeInSeconds % 60  ).c_str() );
    pTimeLabel_->setText( strTime );
}


// -----------------------------------------------------------------------------
// Name: MOS_MainWindow::DisplayMouseLocation
/** @param  strMsg 
*/
// Created: APE 2004-04-27
// -----------------------------------------------------------------------------
void MOS_MainWindow::DisplayMouseLocation( QMouseEvent* pEvent, const MT_Vector2D& vGLPos )
{
    const MT_Rect& rectWorld = MOS_App::GetApp().GetWorld().GetRect();
    if( pEvent == 0 || ! rectWorld.IsInside( vGLPos ) )
	{
        pLocationLabel_->setText( "---" );
		pLocationLabel2_->setText( "---" );
        pLocationLabel3_->setText( "---" );
	}
	else
    {
		// Display x,y,z position
		MOS_RawVisionData& visionData = MOS_App::GetApp().GetRawVisionData();
		pLocationLabel_->setText( MT_FormatString( "x:%.2f y:%.2f h:%.2f ( o:%.2f )", vGLPos.rX_, vGLPos.rY_, visionData.GetHeight( vGLPos ), visionData.GetVisionObjectHeight( vGLPos ) ).c_str() );

		// Display Mgrs coord
        std::string strLoc;
        MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( vGLPos, strLoc );
        pLocationLabel2_->setText( strLoc.c_str() );

		// Display Lat/Long
        MT_Vector2D vPosDMS;
        MOS_App::GetApp().GetWorld().MgrsToGeoCoord( strLoc, vPosDMS );        
		pLocationLabel3_->setText( MT_FormatString( "Lat:%.3f Lon:%.3f", vPosDMS.rY_, vPosDMS.rX_ ).c_str() );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_MainWindow::OnTickStartEnd
/** @param  bTickStart 
*/
// Created: APE 2004-06-17
// -----------------------------------------------------------------------------
void MOS_MainWindow::OnTickStartEnd( bool bTickStart )
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
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_MainWindow::OnLag
// Created: APE 2004-07-30
// -----------------------------------------------------------------------------
void MOS_MainWindow::OnLag()
{
    pTickLabel_->setPixmap( MAKE_PIXMAP( tickred ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MainWindow::OnConnexionStatusChanged
/** @param  b 
*/
// Created: APE 2004-07-30
// -----------------------------------------------------------------------------
void MOS_MainWindow::OnConnexionStatusChanged( bool b )
{
    if( !b )
    {
        pLagTimer_->stop();
        pTickLabel_->setPixmap( MAKE_PIXMAP( tickoff ) );
        pTimeLabel_->setText( "---" );
    }
    else
    {
        pLagTimer_->start( 10000, true );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_MainWindow::closeEvent
/** @param  pEvent 
*/
// Created: APE 2004-06-01
// -----------------------------------------------------------------------------
void MOS_MainWindow::closeEvent( QCloseEvent* pEvent )
{
    // Not done in the destructor, as the windows would already be hidden and we wouldn't
    // be able to correctly record their hidden/visible state.
    this->WriteSettings();
    this->WriteOptions();

    QMainWindow::closeEvent( pEvent );
}


// -----------------------------------------------------------------------------
// Name: MOS_MainWindow::WriteSettings
// Created: APE 2004-06-01
// -----------------------------------------------------------------------------
void MOS_MainWindow::WriteSettings()
{
    MT_Settings settings;
    settings.setPath( "MASA", "MOS_Light2" );

    // Pannel configuration
    QString strDockConfig;
    QTextStream strDockConfigStream( &strDockConfig, IO_WriteOnly );
    strDockConfigStream << *this;

    if( MOS_App::GetApp().IsODBEdition() )
        settings.writeEntry( "/ODBEditorPanels", strDockConfig );
    else
        settings.writeEntry( "/Panels", strDockConfig );

    // Main window configuration
    settings.WriteEntry( "/MainWindow", *this );
}


// -----------------------------------------------------------------------------
// Name: MOS_MainWindow::ReadSettings
// Created: APE 2004-06-01
// -----------------------------------------------------------------------------
void MOS_MainWindow::ReadSettings()
{
    MT_Settings settings;
    settings.setPath( "MASA", "MOS_Light2" );

    // Pannel configuration
    QString strDockConfig;
    if( MOS_App::GetApp().IsODBEdition() )
        strDockConfig = settings.readEntry( "/ODBEditorPanels" );
    else
        strDockConfig = settings.readEntry( "/Panels" );

    QTextStream strDockConfigStream( &strDockConfig, IO_ReadOnly );
    strDockConfigStream >> *this;

    // Main window configuration
    settings.ReadEntry( "/MainWindow", *this, 800, 600, 100, 100, false );

    if( MOS_App::GetApp().IsODBEdition() )
        return;

    // Always keep the mission panel hidden hidden at launch.
    pMissionPanel_->hide();

    if( ! pOptions_->bControllerMode_ )
    {
        this->setAppropriate( pCreationDockWnd_, false );
        this->setAppropriate( pWeatherDockWnd_, false );
        pCreationDockWnd_->hide();
        pWeatherDockWnd_->hide();
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_MainWindow::WriteOptions
// Created: APE 2004-07-12
// -----------------------------------------------------------------------------
void MOS_MainWindow::WriteOptions()
{
    MT_Settings settings;
    settings.setPath( "MASA", "MOS_Light2" );
    pOptions_->WriteSettings( settings );
}
    

// -----------------------------------------------------------------------------
// Name: MOS_MainWindow::ReadOptions
// Created: APE 2004-07-12
// -----------------------------------------------------------------------------
void MOS_MainWindow::ReadOptions()
{
    MT_Settings settings;
    settings.setPath( "MASA", "MOS_Light2" );
    pOptions_->ReadSettings( settings );
}

// -----------------------------------------------------------------------------
// Name: MOS_MainWindow::DimensionsChanged
// Created: AGE 2005-05-13
// -----------------------------------------------------------------------------
void MOS_MainWindow::DimensionsChanged()
{
    if( MOS_App::GetApp().Is3D() )
    {
        pGLWidget_->hide();
        setCentralWidget( pGL3DWidget_ );
        pGL3DWidget_->show();
    }
    else
    {
        pGL3DWidget_->hide();
        setCentralWidget( pGLWidget_ );
        pGLWidget_->show();
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_MainWindow::PushMapEventFilter
// Created: AGE 2005-05-17
// -----------------------------------------------------------------------------
void MOS_MainWindow::PushMapEventFilter( MOS_MapEventFilter_ABC& filter )
{
    pGL3DWidget_->PushMapEventFilter( filter );
    pGLWidget_->PushMapEventFilter( filter );
}

// -----------------------------------------------------------------------------
// Name: MOS_MainWindow::PopMapEventFilter
// Created: AGE 2005-05-17
// -----------------------------------------------------------------------------
void MOS_MainWindow::PopMapEventFilter( MOS_MapEventFilter_ABC& filter )
{
    pGL3DWidget_->PopMapEventFilter( filter );
    pGLWidget_->PopMapEventFilter( filter );
}

// -----------------------------------------------------------------------------
// Name: MOS_MainWindow::GetMapEvents
// Created: AGE 2005-05-17
// -----------------------------------------------------------------------------
const T_MapEventFilterVector& MOS_MainWindow::GetMapEvents() const
{
    if( MOS_App::GetApp().Is3D() )
        return pGL3DWidget_->GetMapEvents();
    return pGLWidget_->GetMapEvents();
}

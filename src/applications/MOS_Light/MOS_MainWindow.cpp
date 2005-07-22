//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MainWindow.cpp $
// $Author: Age $
// $Modtime: 7/02/05 18:25 $
// $Revision: 49 $
// $Workfile: MOS_MainWindow.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_MainWindow.h"
#include "moc_MOS_MainWindow.cpp"

#include "MOS_LogWindow.h"
#include "MOS_MOSServer.h"
#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_AgentServerController.h"
#include "MOS_AgentServerMsgMgr.h"
#include "MOS_WorldWidget.h"

#include "MOS_MagicPos.h"
#include "MOS_AgentEditor.h"
#include "MOS_LimaEditor.h"
#include "MOS_LimaSelector.h"

#include "MOS_LimitEditor.h"
#include "MOS_LimitSelector.h"
#include "MOS_AttrEditor.h"

#include "MOS_SIMControl.h"
#include "MOS_RCEditor.h"
#include "MOS_TraceEditor.h"
#include "MOS_History.h"

#include "MT_Tools/MT_Version.h"

#include <qmenubar.h>
#include <qaction.h>
#include <qlayout.h>
#include <qstatusbar.h>
#include <qsplitter.h>
#include <qlabel.h>
#include <qwidgetstack.h>

#include <sstream>

//-----------------------------------------------------------------------------
// Name: MOS_MainWindow constructor
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
MOS_MainWindow::MOS_MainWindow()
    : QMainWindow ( 0, "MOS light"/*, Qt::WDestructiveClose*/ )
    , pSimControl_( 0 )
    , pAttributeEditor_( 0 )
{
//    InitMenus();
    setCaption( "MOS_Light - " VERSION " - " MT_COMPILE_TYPE " - " __TIMESTAMP__ );

    // Status bar
    new QStatusBar( this );

    QFrame* pFrame = new QFrame ( this );
    setCentralWidget( pFrame ); 

    // Main layout
    QVBoxLayout* pMainLayout = new QVBoxLayout( pFrame );
    
    QSplitter* pVertSplitter = new QSplitter( QSplitter::Vertical, pFrame );
    pMainLayout->addWidget( pVertSplitter );


    QSplitter* pHorizSplitterLeft = new QSplitter( QSplitter::Horizontal, pVertSplitter );
       
    pToolsWidgetStackLeft_  =    new QWidgetStack     ( pHorizSplitterLeft );
    pToolsWidgetStackLeft_->setMinimumHeight( 660 );
    pAgentEditor_           =    new MOS_AgentEditor  ( pToolsWidgetStackLeft_ );
    pAgentSelector_         =    new MOS_AgentSelector( pToolsWidgetStackLeft_ );
    pLimaEditor_            =    new MOS_LimaEditor   ( pToolsWidgetStackLeft_ );
    pLimaSelector_          =    new MOS_LimaSelector ( pToolsWidgetStackLeft_ );
    pLimitEditor_           =    new MOS_LimitEditor  ( pToolsWidgetStackLeft_ );
    pLimitSelector_         =    new MOS_LimitSelector( pToolsWidgetStackLeft_ );
 
    pToolsWidgetStackLeft_->raiseWidget( pAgentEditor_ );
    

    pWorldWidget_      =        new MOS_WorldWidget( pHorizSplitterLeft );
    
    pToolsWidgetStackRight_     =   new QWidgetStack   ( pHorizSplitterLeft );
    pAttributeEditor_  =   new MOS_AttrEditor ( pToolsWidgetStackRight_ );
    pAttributeEditor_->SetWidgetStack( pToolsWidgetStackRight_ );
    pToolsWidgetStackRight_->raiseWidget( pAttributeEditor_ );

    QSplitter* pHoriTopSplitter =   new QSplitter( QSplitter::Horizontal, pVertSplitter );
    pSimControl_                =   new MOS_SIMControl( pHoriTopSplitter );


    
    pLogWindow_                 =   new MOS_LogWindow( this );  // pHoriTopSplitter
    pHistory_                   =   new MOS_History( this );    // pHoriTopSplitter

    QTabWidget* pTabWidget_ = new QTabWidget( pHoriTopSplitter );
    pTabWidget_->addTab( pLogWindow_, "Log" );
    pTabWidget_->addTab( pHistory_, "History" );


    // Status bar
    pSIMTimeLabel_       = new QLabel( "No time"    , statusBar() );
    pMGRSMousePosLabel_  = new QLabel( "No pos"     , statusBar() );
    pPixelMousePosLabel_ = new QLabel( "No pos"     , statusBar() );
    pNetBytesInLabel_    = new QLabel( "0 bytes in" , statusBar() );
    pNetBytesOutLabel_   = new QLabel( "0 bytes out", statusBar() );


//    pSIMTimeLabel_->setAlignment( AlignCenter );
    statusBar()->addWidget( pMGRSMousePosLabel_ , 0, TRUE );   
    statusBar()->addWidget( pPixelMousePosLabel_, 0, TRUE );   
    statusBar()->addWidget( pNetBytesInLabel_   , 0, TRUE );   
    statusBar()->addWidget( pNetBytesOutLabel_  , 0, TRUE );   
    statusBar()->addWidget( pSIMTimeLabel_      , 0, TRUE );   
    
//    pGraphWindow_ = new MOS_GraphWindow( *this );
    
    setMinimumWidth( 950 );
    setMinimumHeight( 500 );
}


//-----------------------------------------------------------------------------
// Name: MOS_MainWindow destructor
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
MOS_MainWindow::~MOS_MainWindow()
{
}

//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::Initialize
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
void MOS_MainWindow::Initialize()
{
//    pGraphWindow_->hide();

    pAgentEditor_->Initialize();
    pAgentSelector_->Initialize();

    pLimaEditor_->Initialize();
    pLimaSelector_->Initialize();
    pLimitEditor_->Initialize();
    pLimitSelector_->Initialize();
    pSimControl_->Initialize();
    pAttributeEditor_->Initialize();


    if ( pWorldWidget_ )
        pWorldWidget_->Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::Terminate
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
void MOS_MainWindow::Terminate()
{
    if ( pWorldWidget_ )
        pWorldWidget_->Terminate();

    pLimaEditor_->Terminate();
    pAgentSelector_->Terminate();
    pLimaSelector_->Terminate();
    pLimitEditor_->Terminate();
    pLimitSelector_->Terminate();
    pAgentEditor_->Terminate();
    pSimControl_->Terminate();
    pAttributeEditor_->Terminate();

}

//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::InitMenus
// Created: NLD 2002-07-18
//-----------------------------------------------------------------------------
void MOS_MainWindow::InitMenus()
{

}


//=============================================================================
// ACTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::NotifyTimeHasChanged
// Created: NLD 2002-09-16
//-----------------------------------------------------------------------------
void MOS_MainWindow::NotifyTimeHasChanged( uint nTimeInSeconds )
{
    QString sTime( MT_FormatString( "%02d:%02d:%02d", ( nTimeInSeconds / 3600 ) % 24, ( nTimeInSeconds / 60 ) % 60 , nTimeInSeconds % 60  ).c_str() );
    pSIMTimeLabel_->setText( sTime );
}

// -----------------------------------------------------------------------------
// Name: MOS_MainWindow::NotifyNetStatistics
// Created: NLD 2003-10-24
// -----------------------------------------------------------------------------
void MOS_MainWindow::NotifyNetStatistics( uint nNbrBytesIn, uint nNbrBytesOut )
{
    if( nNbrBytesIn < 1024 )
    {
        QString strTmp( MT_FormatString( "%d Bytes in", nNbrBytesIn ).c_str() );
        pNetBytesInLabel_->setText( strTmp );
    }
    else if( nNbrBytesIn < 1048576 )
    {
        QString strTmp( MT_FormatString( "%.3f KBytes in", nNbrBytesIn  / 1024. ).c_str() );
        pNetBytesInLabel_->setText( strTmp );
    }
    else
    {
        QString strTmp( MT_FormatString( "%.3f MBytes in", nNbrBytesIn / 1048576. ).c_str() );
        pNetBytesInLabel_->setText( strTmp );
    }


    if( nNbrBytesOut < 1024 )
    {
        QString strTmp( MT_FormatString( "%d Bytes Out", nNbrBytesOut ).c_str() );
        pNetBytesOutLabel_->setText( strTmp );
    }
    else if( nNbrBytesOut < 1048576 )
    {
        QString strTmp( MT_FormatString( "%.3f KBytes Out", nNbrBytesOut  / 1024. ).c_str() );
        pNetBytesOutLabel_->setText( strTmp );
    }
    else
    {
        QString strTmp( MT_FormatString( "%.3f MBytes Out", nNbrBytesOut / 1048576. ).c_str() );
        pNetBytesOutLabel_->setText( strTmp );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::NotifyMousePositionHasChanged
// Created: NLD 2002-11-29
// Last modified: JVT 04-05-27
//-----------------------------------------------------------------------------
void MOS_MainWindow::NotifyMousePositionHasChanged( const std::string& strMGRSMousePosition, const MT_Vector2D& vPixelMousePos )
{
    if ( MOS_App::GetApp().GetWorld().GetRect().IsInside( vPixelMousePos ) )
    {
        MT_Vector2D vPosDMS;
        MOS_App::GetApp().GetWorld().MgrsToGeoCoord( strMGRSMousePosition, vPosDMS );
        std::stringstream str;
        pPixelMousePosLabel_->setText( 
            MT_FormatString( "%.2f - %.2f - %.2f ( O: %.2f ) Meters",
                vPixelMousePos.rX_,
                vPixelMousePos.rY_,
                MOS_App::GetApp().GetRawVisionData().GetHeight( vPixelMousePos ),
                MOS_App::GetApp().GetRawVisionData().GetVisionObjectHeight( vPixelMousePos ) ).c_str() );
    }
    else
        pPixelMousePosLabel_ ->setText( strMGRSMousePosition.c_str() );

    pMGRSMousePosLabel_ ->setText( strMGRSMousePosition.c_str() );
}

//=============================================================================
// QT SIGNALS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::closeEvent
// Created: NLD 2002-10-14
//-----------------------------------------------------------------------------
void MOS_MainWindow::closeEvent( QCloseEvent* pEvent )
{
//    pGraphWindow_->close();
    pEvent->accept();
}


//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::PushWidgetStackLeft
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
void MOS_MainWindow::PushWidgetStackLeft( QWidget* pWidget )
{
    QWidget* pCurWidget = pToolsWidgetStackLeft_->visibleWidget();
    if ( pCurWidget )
        widgetStackLeft_.push( pCurWidget );
    pToolsWidgetStackLeft_->raiseWidget( pWidget );
}


//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::PopWidgetStackLeft
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
void MOS_MainWindow::PopWidgetStackLeft()
{
    if ( widgetStackLeft_.size() )
    {
        QWidget* pVisWidget = pToolsWidgetStackLeft_->visibleWidget();
        QWidget* pWidget = widgetStackLeft_.top();
        widgetStackLeft_.pop();
        pToolsWidgetStackLeft_->raiseWidget( pWidget );
    }
    else
        pToolsWidgetStackLeft_->raiseWidget( pAgentEditor_ );
}
//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::PushWidgetStackRight
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
void MOS_MainWindow::PushWidgetStackRight( QWidget* pWidget )
{
    QWidget* pCurWidget = pToolsWidgetStackRight_->visibleWidget();
    if ( pCurWidget )
        widgetStackRight_.push( pCurWidget );
    pToolsWidgetStackRight_->raiseWidget( pWidget );
}


//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::PopWidgetStackRight
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
void MOS_MainWindow::PopWidgetStackRight()
{
    if ( widgetStackRight_.size() )
    {
        QWidget* pWidget = widgetStackRight_.top();
        widgetStackRight_.pop();
        pToolsWidgetStackRight_->raiseWidget( pWidget );
    }
    else
        pToolsWidgetStackRight_->raiseWidget( pAgentEditor_ );
}


//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::SetWidgetStackRight
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
void MOS_MainWindow::SetWidgetStackRight( QWidget* pWidget )
{
    while( widgetStackRight_.size() )
        PopWidgetStackRight();
    PushWidgetStackRight( pWidget );
}

//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::UnregisterRightWidget
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
void MOS_MainWindow::UnregisterRightWidget( QWidget* pWidget )
{
    pToolsWidgetStackRight_->removeWidget( pWidget );
    T_WidgetStack tmpStack;
    while( ! widgetStackRight_.empty() )
    {
        QWidget* pCurrent = widgetStackRight_.top();
        widgetStackRight_.pop();
        if( pCurrent == pWidget )
            break;
        tmpStack.push( pCurrent );
    }

    while( ! tmpStack.empty() )
    {
        widgetStackRight_.push( tmpStack.top() );
        tmpStack.pop();
    }

    if( widgetStackRight_.empty() )
        pToolsWidgetStackRight_->raiseWidget( pAgentEditor_ );
    else
    {
        pToolsWidgetStackRight_->raiseWidget( widgetStackRight_.top() );
        widgetStackRight_.pop();
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::UnregisterLeftWidget
// Created: AGN 03-09-04
//-----------------------------------------------------------------------------
void MOS_MainWindow::UnregisterLeftWidget( QWidget* pWidget )
{
    pToolsWidgetStackLeft_->removeWidget( pWidget );
    T_WidgetStack tmpStack;
    while( ! widgetStackLeft_.empty() )
    {
        QWidget* pCurrent = widgetStackLeft_.top();
        widgetStackLeft_.pop();
        if( pCurrent == pWidget )
            break;
        tmpStack.push( pCurrent );
    }

    while( ! tmpStack.empty() )
    {
        widgetStackLeft_.push( tmpStack.top() );
        tmpStack.pop();
    }

    if( widgetStackLeft_.empty() )
        pToolsWidgetStackLeft_->raiseWidget( pAgentEditor_ );
    else
    {
        pToolsWidgetStackLeft_->raiseWidget( widgetStackLeft_.top() );
        widgetStackLeft_.pop();
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MainWindow::NotifyDeleteAgent
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_MainWindow::NotifyDeleteAgent( MOS_Agent& agent )
{
    if ( pLimaEditor_->IsActive( false ) )
        pLimaEditor_->Cancel();

    if ( pLimitEditor_->IsActive( false ) )
        pLimitEditor_->Cancel();

    if ( pLimaSelector_->IsActive( false ) )
        pLimaSelector_->Cancel();
    
    if ( pLimitSelector_->IsActive( false ) )
        pLimitSelector_->Cancel();
    
    if ( pAgentSelector_->IsActive( false ) )
        pAgentSelector_->Cancel();

    if( pAttributeEditor_->IsActive( false ) )
    {
        pAttributeEditor_->SetAgent( 0 );
        pAttributeEditor_->Cancel();
    }
    
    pAgentEditor_->NotifyDeleteAgent( agent );
}

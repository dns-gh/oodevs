// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-10 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ReportListView.cpp $
// $Author: Age $
// $Modtime: 5/04/05 16:41 $
// $Revision: 5 $
// $Workfile: ReportListView.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ReportListView.h"
#include "moc_ReportListView.cpp"

#include "App.h"
#include "AgentManager.h"
#include "ObjectManager.h"
#include "MainWindow.h"
#include "Agent_ABC.h"
#include "Agent.h"
//#include "Object.h"
#include "ObjectKnowledge.h"
#include "AgentKnowledge.h"
#include "Report_ABC.h"
#include "RC.h"
#include "Trace.h"
#include "ActionContext.h"
#include "Gtia.h"
#include "Team.h"
#include "MT_RichListViewItem.h"
#include "ReportFilterOptions.h"
#include "Model.h"

// -----------------------------------------------------------------------------
// Name: ReportListView constructor
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
ReportListView::ReportListView( QWidget* pParent, const ReportFilterOptions& filter )
    : QListView  ( pParent )
    , filter_    ( filter )
    , pAgent_    ( 0 )
    , pPopupMenu_( new QPopupMenu( this ) )
{
    addColumn( tr( "Reçu" ) );
    addColumn( tr( "Compte-rendu" ) );

    // Set a descending sorting order, then disable user sorting.
    setSorting( 0, false );
    setSorting( -1, false );

    setResizeMode( QListView::LastColumn );
    setAllColumnsShowFocus ( true );

    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint&, int ) ) );
    connect( this, SIGNAL( clicked( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnClick( QListViewItem*, const QPoint&, int ) ) );
    connect( this, SIGNAL( doubleClicked( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( spacePressed( QListViewItem* ) ),  this, SLOT( OnRequestCenter() ) );

    connect( this, SIGNAL( CenterOnPoint( const MT_Vector2D& ) )                  , &MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ) );
    connect( this, SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ), &MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ) );
    connect( this, SIGNAL( ReadingReports( Agent_ABC& ) )                     , &MainWindow::GetMainWindow(), SIGNAL( ReadingReports( Agent_ABC& ) ) );

    connect( &App::GetApp(), SIGNAL( ReportCreated( Agent_ABC&, Report_ABC& ) ), this, SLOT( OnReportCreated( Agent_ABC&, Report_ABC& ) ) );
}


// -----------------------------------------------------------------------------
// Name: ReportListView destructor
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
ReportListView::~ReportListView()
{
}


// -----------------------------------------------------------------------------
// Name: ReportListView::SetAgent
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
void ReportListView::SetAgent( Agent_ABC* pAgent )
{
    if( pAgent_ == pAgent )
        return;

    // Before we change the displayed reports, mark the old ones as read.
    
    QListViewItem* pItem = firstChild();
    while( pItem != 0 )
    {
        Report_ABC& report = GetItemValue( *pItem );
        report.SetNew( false );
        pItem = pItem->nextSibling();
    }
    

    clear();
    pPopupMenu_->hide();

    pAgent_ = pAgent;
    if( pAgent_ == 0 )
        return;

    emit ReadingReports( *pAgent_ );

    Agent_ABC::T_ReportVector& reports = pAgent_->GetReports();
    for( Agent_ABC::IT_ReportVector it = reports.begin(); it != reports.end(); ++it )
        OnReportCreated( *pAgent_, **it );
}


// -----------------------------------------------------------------------------
// Name: ReportListView::OnOptionsChanged
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void ReportListView::OnOptionsChanged()
{
    if( pAgent_ )
    {
        clear();
        Agent_ABC::T_ReportVector& reports = pAgent_->GetReports();
        for( Agent_ABC::IT_ReportVector it = reports.begin(); it != reports.end(); ++it )
            OnReportCreated( *pAgent_, **it );
    }
}


// -----------------------------------------------------------------------------
// Name: ReportListView::OnReportCreated
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
void ReportListView::OnReportCreated( Agent_ABC& agent, Report_ABC& report )
{
    if( pAgent_ != &agent )
        return;
    if( ! filter_.ShouldDisplay( report ) )
        return;

    std::string strTime = MT_FormatString( "%02d:%02d:%02d", ( report.GetTime() / 3600 ) % 24, ( report.GetTime() / 60 ) % 60 , report.GetTime() % 60  ).c_str();

    if( report.IsRCType() )
    {
        T_RichReportItem* pItem = new T_RichReportItem( &report, this, strTime.c_str(), report.GetTitle().c_str() );
        pItem->setRenameEnabled( 0, false );
        pItem->SetBold( report.IsNew() );

        RC& rc = (RC&)report;
        if( ! rc.GetFragOrders().empty() )
            pItem->SetFontColor( QColor( 200, 0, 0 ) );
        else if( rc.GetType() == Report_ABC::eMessage ) // blue
            pItem->SetFontColor( QColor( 0, 0, 200 ) );
        else if( rc.GetType() == Report_ABC::eWarning ) // orange
            pItem->SetFontColor( QColor( 255, 128, 64 ) );
        else if( rc.GetType() == Report_ABC::eEvent ) // green
            pItem->SetFontColor( QColor( 32, 200, 64 ) );

    }
    else
    {
        T_ReportItem* pItem = new T_ReportItem( &report, this, strTime.c_str(), report.GetTitle().c_str() );
        pItem->setRenameEnabled( 0, false );
        QFont font = this->font();
        font.setBold( report.IsNew() );
        pItem->SetFont( font );
        pItem->SetFontColor( QColor( 150, 150, 150 ) );
    }
    
    // Trick for making sure the EventToolButton gets the ReadingReports event
    // after receiving the ReportCreated event.
    QTimer::singleShot( 0, this, SLOT( NotifyReadingReports() ) );
}



// -----------------------------------------------------------------------------
// Name: ReportListView::NotifyReadingReports
// Created: APE 2004-09-30
// -----------------------------------------------------------------------------
void ReportListView::NotifyReadingReports()
{
    if( isVisible() && pAgent_ != 0 )
        emit ReadingReports( *pAgent_ );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::OnClick
// Created: APE 2004-09-07
// -----------------------------------------------------------------------------
void ReportListView::OnClick( QListViewItem* pItem, const QPoint& pos, int nCol )
{
    if( pItem == 0 || pItem->rtti() != eRichItem )
        return;

    MT_RichListViewItem* pRItem = (MT_RichListViewItem*)pItem;
    QString str = pRItem->GetAnchorAt( pos, nCol );
    if( str == QString::null )
        return;

    int nId;
    if( InterpretLink( str, "Agent:", nId ) )
    {
        Agent_ABC* pAgent = & App::GetApp().GetModel().GetAgent( nId );
        //$$$ can't do this here cauze it would destroy the listview item we clicked on...
        //$$$   emit ElementSelected( SelectedElement( *pAgent ) );
        emit CenterOnPoint( pAgent->GetPos() );
        return;
    }


    if( InterpretLink( str, "AgentKnowledge:", nId ) )
    {
        AgentKnowledge* pKnowledge = pAgent_->FindAgentKnowledge( nId );
        if( pKnowledge != 0 )
        {
            //$$$ can't do this here cauze it would destroy the listview item we clicked on...
            //$$$   emit ElementSelected( SelectedElement( *pKnowledge ) );
            emit CenterOnPoint( pKnowledge->GetPosition() );
        }
        return;
    }

    if( InterpretLink( str, "ObjectKnowledge:", nId ) )
    {
        ObjectKnowledge* pKnowledge = pAgent_->GetTeam().FindObjectKnowledge( nId );
        if( pKnowledge != 0 )
        {
            //$$$ can't do this here cauze it would destroy the listview item we clicked on...
            //$$$   emit ElementSelected( SelectedElement( *pKnowledge ) );
            emit CenterOnPoint( pKnowledge->GetCenter() );
        }
        return;
    }
 
}


// -----------------------------------------------------------------------------
// Name: ReportListView::OnRequestCenter
// Created: APE 2004-05-12
// -----------------------------------------------------------------------------
void ReportListView::OnRequestCenter()
{
    QListViewItem* pItem = selectedItem();
    if( pItem == 0 )
        return;

    MT_Vector2D vPos = GetItemValue( *pItem ).GetPos();

    emit CenterOnPoint( vPos );
}


// -----------------------------------------------------------------------------
// Name: ReportListView::OnRequestPopup
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ReportListView::OnRequestPopup( QListViewItem* pItem, const QPoint& pos, int /*nCol*/ )
{
    if( pAgent_ == 0 )
        return;

    pPopupItem_ = pItem;
    pPopupMenu_->clear();
    pPopupMenu_->insertItem( tr( "Tout effacer" ), this, SLOT( OnClearAll() ) );
    pPopupMenu_->insertItem( tr( "Effacer les msg debug" ), this, SLOT( OnClearTrace() ) );

    if( pItem != 0 )
    {
        pPopupMenu_->insertItem( tr( "Effacer jusqu'ici" ), this, SLOT( OnClearUpTo() ) );

        Report_ABC& report = GetItemValue( *pItem );
        if( report.IsRCType() )
        {
            RC& rc = (RC&)report;
            
            if( !rc.GetFragOrders().empty() )
            {
                SelectedElement selectedElement( rc );
                ActionContext context( selectedElement );
                emit NewPopupMenu( *pPopupMenu_, context );
            }
        }
    }

    pPopupMenu_->popup( pos );
}


// -----------------------------------------------------------------------------
// Name: ReportListView::OnClearAll
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ReportListView::OnClearAll()
{
    if ( pAgent_ != 0 )
    {
        pAgent_->DeleteAllRCs();
        pAgent_->DeleteAllTraces();
    }
    clear();
}


// -----------------------------------------------------------------------------
// Name: ReportListView::OnClearTrace
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ReportListView::OnClearTrace()
{
    if ( pAgent_ != 0 )
    {
        clear();
        pAgent_->DeleteAllTraces();
        SetAgent( pAgent_ );
    }
}


// -----------------------------------------------------------------------------
// Name: ReportListView::OnClearUpTo
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void ReportListView::OnClearUpTo()
{
    assert( pPopupItem_ != 0 );
    assert( pAgent_ != 0  );

    QListViewItem* pItem = pPopupItem_;
    while( pItem != 0 )
    {
        if ( pAgent_ != 0 )
            pAgent_->DeleteReport( GetItemValue( *pItem ) );
        pItem = pItem->nextSibling();
    }

    clear();
    if ( pAgent_ != 0 )
    {
        Agent_ABC* pAgent = pAgent_;
        pAgent_ = 0;
        SetAgent( pAgent );
    }
}



// -----------------------------------------------------------------------------
// Name: ReportListView::hideEvent
// Created: APE 2004-06-17
// -----------------------------------------------------------------------------
void ReportListView::hideEvent( QHideEvent* pEvent )
{
    QListView::hideEvent( pEvent );

    if( pEvent->spontaneous() )
        return;

    QListViewItem* pItem = firstChild();
    while( pItem != 0 )
    {
        if( pItem->rtti() == eRichItem )
            ((T_RichReportItem*)pItem)->SetBold( false );
        else
        {
            QFont font = this->font();
            ((T_ReportItem*)pItem)->SetFont( font );
        }
        GetItemValue( *pItem ).SetNew( false );

        pItem = pItem->nextSibling();
    }
}


// -----------------------------------------------------------------------------
// Name: ReportListView::showEvent
// Created: APE 2004-06-17
// -----------------------------------------------------------------------------
void ReportListView::showEvent( QShowEvent* pEvent )
{
    QListView::showEvent( pEvent );
    NotifyReadingReports();
}


// -----------------------------------------------------------------------------
// Name: ReportListView::InterpretLink
// Created: APE 2004-09-10
// -----------------------------------------------------------------------------
bool ReportListView::InterpretLink( const QString& strLink, const QString& strKeyword, int& nResultId )
{
    if( strLink.left( strKeyword.length() ) != strKeyword )
        return false;

    bool b = false;
    nResultId = strLink.right( strLink.length() - strKeyword.length() ).toInt( &b );
    return b;
}

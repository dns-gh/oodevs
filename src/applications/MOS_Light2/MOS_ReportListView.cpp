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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ReportListView.cpp $
// $Author: Age $
// $Modtime: 5/04/05 16:41 $
// $Revision: 5 $
// $Workfile: MOS_ReportListView.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_ReportListView.h"
#include "moc_MOS_ReportListView.cpp"

#include "MOS_App.h"
#include "MOS_AgentManager.h"
#include "MOS_ObjectManager.h"
#include "MOS_MainWindow.h"
#include "MOS_Agent.h"
//#include "MOS_Object.h"
#include "MOS_ObjectKnowledge.h"
#include "MOS_AgentKnowledge.h"
#include "MOS_Report_ABC.h"
#include "MOS_RC.h"
#include "MOS_Trace.h"
#include "MOS_ActionContext.h"
#include "MOS_Gtia.h"
#include "MOS_Team.h"

#ifndef MOS_USE_INLINE
#   include "MOS_ReportListView.inl"
#endif

#include "MT_RichListViewItem.h"

// -----------------------------------------------------------------------------
// Name: MOS_ReportListView constructor
/** @param  pParent 
*/
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
MOS_ReportListView::MOS_ReportListView( QWidget* pParent )
    : QListView  ( pParent )
    , pAgent_    ( 0 )
    , pPopupMenu_( new QPopupMenu( this ) )
{
    this->addColumn( tr( "Reçu" ) );
    this->addColumn( tr( "Compte-rendu" ) );

    // Set a descending sorting order, then disable user sorting.
    this->setSorting( 0, false );
    this->setSorting( -1, false );

    this->setResizeMode( QListView::LastColumn );
    this->setAllColumnsShowFocus ( true );

    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint&, int ) ) );
    connect( this, SIGNAL( clicked( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnClick( QListViewItem*, const QPoint&, int ) ) );
    connect( this, SIGNAL( doubleClicked( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( spacePressed( QListViewItem* ) ),  this, SLOT( OnRequestCenter() ) );

    connect( this, SIGNAL( CenterOnPoint( const MT_Vector2D& ) ),                   &MOS_MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ) );
    connect( this, SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ), &MOS_MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ) );
    connect( this, SIGNAL( ReadingReports( MOS_Agent& ) ),                          &MOS_MainWindow::GetMainWindow(), SIGNAL( ReadingReports( MOS_Agent& ) ) );

    connect( &MOS_App::GetApp(), SIGNAL( ReportCreated( MOS_Agent&, MOS_Report_ABC& ) ), this, SLOT( OnReportCreated( MOS_Agent&, MOS_Report_ABC& ) ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ReportListView destructor
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
MOS_ReportListView::~MOS_ReportListView()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_ReportListView::SetAgent
/** @param  pAgent 
*/
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
void MOS_ReportListView::SetAgent( MOS_Agent* pAgent )
{
    if( pAgent_ == pAgent )
        return;

    // Before we change the displayed reports, mark the old ones as read.
    QListViewItem* pItem = this->firstChild();
    while( pItem != 0 )
    {
        MOS_Report_ABC& report = this->GetItemValue( *pItem );
        report.SetNew( false );
        pItem = pItem->nextSibling();
    }

    this->clear();
    pPopupMenu_->hide();

    pAgent_ = pAgent;
    if( pAgent_ == 0 )
        return;

    emit ReadingReports( *pAgent_ );

    MOS_Agent::T_ReportVector& reports = pAgent_->GetReports();
    for( MOS_Agent::IT_ReportVector it = reports.begin(); it != reports.end(); ++it )
        this->OnReportCreated( *pAgent_, **it );
}


// -----------------------------------------------------------------------------
// Name: MOS_ReportListView::OnReportCreated
/** @param  agent 
    @param  report 
*/
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
void MOS_ReportListView::OnReportCreated( MOS_Agent& agent, MOS_Report_ABC& report )
{
    if( pAgent_ != &agent )
        return;

    std::string strTime = MT_FormatString( "%02d:%02d:%02d", ( report.GetTime() / 3600 ) % 24, ( report.GetTime() / 60 ) % 60 , report.GetTime() % 60  ).c_str();

    if( report.GetType() == MOS_Report_ABC::eRC )
    {
        T_RichReportItem* pItem = new T_RichReportItem( &report, this, strTime.c_str(), report.GetTitle().c_str() );
        pItem->setRenameEnabled( 0, false );
        pItem->SetBold( report.IsNew() );

        MOS_RC& rc = (MOS_RC&)report;
        if( ! rc.GetFragOrders().empty() )
            pItem->SetFontColor( QColor( 200, 0, 0 ) );
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

    // Trick for making sure the MOS_EventToolButton gets the ReadingReports event
    // after receiving the ReportCreated event.
    QTimer::singleShot( 0, this, SLOT( NotifyReadingReports() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ReportListView::NotifyReadingReports
// Created: APE 2004-09-30
// -----------------------------------------------------------------------------
void MOS_ReportListView::NotifyReadingReports()
{
    if( this->isVisible() && pAgent_ != 0 )
        emit ReadingReports( *pAgent_ );
}

// -----------------------------------------------------------------------------
// Name: MOS_ReportListView::OnClick
// Created: APE 2004-09-07
// -----------------------------------------------------------------------------
void MOS_ReportListView::OnClick( QListViewItem* pItem, const QPoint& pos, int nCol )
{
    if( pItem == 0 || pItem->rtti() != eRichItem )
        return;

    MT_RichListViewItem* pRItem = (MT_RichListViewItem*)pItem;
    QString str = pRItem->GetAnchorAt( pos, nCol );
    if( str == QString::null )
        return;

    int nId;
    if( this->InterpretLink( str, "Agent:", nId ) )
    {
        MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( nId );
        assert( pAgent != 0 );
        //$$$ can't do this here cauze it would destroy the listview item we clicked on...
        //$$$   emit ElementSelected( MOS_SelectedElement( *pAgent ) );
        emit CenterOnPoint( pAgent->GetPos() );
        return;
    }

    if( this->InterpretLink( str, "AgentKnowledge:", nId ) )
    {
        MOS_AgentKnowledge* pKnowledge = pAgent_->GetGtia().FindAgentKnowledge( nId );
        if( pKnowledge != 0 )
        {
            //$$$ can't do this here cauze it would destroy the listview item we clicked on...
            //$$$   emit ElementSelected( MOS_SelectedElement( *pKnowledge ) );
            emit CenterOnPoint( pKnowledge->GetPosition() );
        }
        return;
    }

    if( this->InterpretLink( str, "ObjectKnowledge:", nId ) )
    {
        MOS_ObjectKnowledge* pKnowledge = pAgent_->GetTeam().FindObjectKnowledge( nId );
        if( pKnowledge != 0 )
        {
            //$$$ can't do this here cauze it would destroy the listview item we clicked on...
            //$$$   emit ElementSelected( MOS_SelectedElement( *pKnowledge ) );
            emit CenterOnPoint( pKnowledge->GetCenter() );
        }
        return;
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_ReportListView::OnRequestCenter
/** @param  pItem 
    @param  pos 
    @param  nCol 
*/
// Created: APE 2004-05-12
// -----------------------------------------------------------------------------
void MOS_ReportListView::OnRequestCenter()
{
    QListViewItem* pItem = this->selectedItem();
    if( pItem == 0 )
        return;

    MT_Vector2D vPos = this->GetItemValue( *pItem ).GetPos();

    emit CenterOnPoint( vPos );
}


// -----------------------------------------------------------------------------
// Name: MOS_ReportListView::OnRequestPopup
/** @param  pItem 
    @param  pos 
    @param  nCol 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ReportListView::OnRequestPopup( QListViewItem* pItem, const QPoint& pos, int /*nCol*/ )
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

        MOS_Report_ABC& report = this->GetItemValue( *pItem );
        if( report.GetType() == MOS_Report_ABC::eRC )
        {
            MOS_RC& rc = (MOS_RC&)report;
            
            if( !rc.GetFragOrders().empty() )
            {
                MOS_SelectedElement selectedElement( rc );
                MOS_ActionContext context( selectedElement );
                emit NewPopupMenu( *pPopupMenu_, context );
            }
        }
    }

    pPopupMenu_->popup( pos );
}


// -----------------------------------------------------------------------------
// Name: MOS_ReportListView::OnClearAll
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ReportListView::OnClearAll()
{
    assert( pAgent_ != 0 );
    pAgent_->DeleteAllRCs();
    pAgent_->DeleteAllTraces();
    this->clear();
}


// -----------------------------------------------------------------------------
// Name: MOS_ReportListView::OnClearTrace
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ReportListView::OnClearTrace()
{
    assert( pAgent_ != 0 );

    this->clear();
    pAgent_->DeleteAllTraces();
    this->SetAgent( pAgent_ );
}


// -----------------------------------------------------------------------------
// Name: MOS_ReportListView::OnClearUpTo
/** @param  nTime 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_ReportListView::OnClearUpTo()
{
    assert( pAgent_ != 0 );
    assert( pPopupItem_ != 0 );

    QListViewItem* pItem = pPopupItem_;
    while( pItem != 0 )
    {
        pAgent_->DeleteReport( this->GetItemValue( *pItem ) );
        pItem = pItem->nextSibling();
    }

    this->clear();
    this->SetAgent( pAgent_ );
}



// -----------------------------------------------------------------------------
// Name: MOS_ReportListView::hideEvent
/** @param  pEvent 
*/
// Created: APE 2004-06-17
// -----------------------------------------------------------------------------
void MOS_ReportListView::hideEvent( QHideEvent* pEvent )
{
    QListView::hideEvent( pEvent );

    if( pEvent->spontaneous() )
        return;

    QListViewItem* pItem = this->firstChild();
    while( pItem != 0 )
    {
        if( pItem->rtti() == eRichItem )
            ((T_RichReportItem*)pItem)->SetBold( false );
        else
        {
            QFont font = this->font();
            ((T_ReportItem*)pItem)->SetFont( font );
        }
        this->GetItemValue( *pItem ).SetNew( false );

        pItem = pItem->nextSibling();
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_ReportListView::showEvent
/** @param  pEvent 
*/
// Created: APE 2004-06-17
// -----------------------------------------------------------------------------
void MOS_ReportListView::showEvent( QShowEvent* pEvent )
{
    QListView::showEvent( pEvent );

    if( pAgent_ != 0 )
        emit ReadingReports( *pAgent_ );
}


// -----------------------------------------------------------------------------
// Name: MOS_ReportListView::InterpretLink
/** @param  strLink 
    @param  strKeyword 
    @param  nResultId 
    @return 
*/
// Created: APE 2004-09-10
// -----------------------------------------------------------------------------
bool MOS_ReportListView::InterpretLink( const QString& strLink, const QString& strKeyword, int& nResultId )
{
    if( strLink.left( strKeyword.length() ) != strKeyword )
        return false;

    bool b = false;
    nResultId = strLink.right( strLink.length() - strKeyword.length() ).toInt( &b );
    return b;
}

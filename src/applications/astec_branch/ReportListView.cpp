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

#include "astec_pch.h"
#include "ReportListView.h"
#include "moc_ReportListView.cpp"

#include "Report_ABC.h"
#include "ReportFilterOptions.h"
#include "Reports.h"
#include "Agent_ABC.h"
#include "Controllers.h"
#include "ItemFactory_ABC.h"

// -----------------------------------------------------------------------------
// Name: ReportListView constructor
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
ReportListView::ReportListView( QWidget* pParent, Controllers& controllers, const ReportFilterOptions& filter, ItemFactory_ABC& factory )
    : ListDisplayer< ReportListView >( pParent, *this, factory )
    , controllers_( controllers )
    , filter_( filter )
    , factory_( factory )
    , selected_( 0 )
{
    AddColumn( "Reçu" );
    AddColumn( "Compte-rendu" );
    
    // Set a descending sorting order, then disable user sorting.
    setSorting( 0, false );
    setSorting( -1, false );

//    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint&, int ) ) );
//    connect( this, SIGNAL( clicked( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnClick( QListViewItem*, const QPoint&, int ) ) );
//    connect( this, SIGNAL( doubleClicked( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
//    connect( this, SIGNAL( spacePressed( QListViewItem* ) ),  this, SLOT( OnRequestCenter() ) );
//
//    connect( this, SIGNAL( CenterOnPoint( const MT_Vector2D& ) )                  , &MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ) );
//    connect( this, SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ), &MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ) );
//    connect( this, SIGNAL( ReadingReports( Agent_ABC& ) )                     , &MainWindow::GetMainWindow(), SIGNAL( ReadingReports( Agent_ABC& ) ) );
//
//    connect( &App::GetApp(), SIGNAL( ReportCreated( Agent_ABC&, Report_ABC& ) ), this, SLOT( OnReportCreated( Agent_ABC&, Report_ABC& ) ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ReportListView destructor
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
ReportListView::~ReportListView()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::showEvent
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void ReportListView::showEvent( QShowEvent* )
{
    const Agent_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::OnOptionsChanged
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
void ReportListView::OnOptionsChanged()
{
    const Agent_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::NotifySelected
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
void ReportListView::NotifySelected( const Agent_ABC* element )
{
    if( element != selected_ )
    {
        selected_ = element;
        if( selected_  )
            NotifyUpdated( selected_->Get< Reports >() );
    }
}

// -----------------------------------------------------------------------------
// Name: ReportListView::ShouldUpdate
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
bool ReportListView::ShouldUpdate( const Reports& reports )
{
    return isVisible()
        && selected_
        && selected_->Retrieve< Reports >() == &reports;
}

// -----------------------------------------------------------------------------
// Name: ReportListView::NotifyUpdated
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
void ReportListView::NotifyUpdated( const Reports& reports )
{
    if( ! ShouldUpdate( reports ) )
        return;

    DeleteTail( DisplayList( reports.reports_.begin(), reports.reports_.end() ) );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::Display
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
void ReportListView::Display( const Report_ABC* report, Displayer_ABC& displayer, ValuedListItem* item )
{
    if( report && & report->GetAgent() == selected_ )
    {
        if( filter_.ShouldDisplay( *report ) )
        {
            displayer.Display( "Reçu", QTime().addSecs( report->GetTime() ).toString( "hh:mm:ss" ) );
            displayer.Display( "Compte-rendu", report->GetTitle() );
        }
        else
            delete item;
    }
}

// -----------------------------------------------------------------------------
// Name: ReportListView::NotifyCreated
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
void ReportListView::NotifyCreated( const Report_ABC& report )
{
    if( ! isVisible() || & report.GetAgent() != selected_ )
        return;
    if( ! filter_.ShouldDisplay( report ) )
        return;

    ValuedListItem* item = factory_.CreateItem( this );
     // $$$$ AGE 2006-06-29: Display !
    item->Set( & report, QTime().addSecs( report.GetTime() ).toString( "hh:mm:ss" ), report.GetTitle().c_str() );
//    if( report.IsRCType() )
//    {
//        T_RichReportItem* pItem = new T_RichReportItem( &report, this, strTime.c_str(), report.GetTitle().c_str() );
//        pItem->setRenameEnabled( 0, false );
//        pItem->SetBold( report.IsNew() );
//
//        RC& rc = (RC&)report;
//        if( ! rc.GetFragOrders().empty() )
//            pItem->SetFontColor( QColor( 200, 0, 0 ) );
//        else if( rc.GetType() == Report_ABC::eMessage ) // blue
//            pItem->SetFontColor( QColor( 0, 0, 200 ) );
//        else if( rc.GetType() == Report_ABC::eWarning ) // orange
//            pItem->SetFontColor( QColor( 255, 128, 64 ) );
//        else if( rc.GetType() == Report_ABC::eEvent ) // green
//            pItem->SetFontColor( QColor( 32, 200, 64 ) );
//
//    }
//    else
//    {
//        T_ReportItem* pItem = new T_ReportItem( &report, this, strTime.c_str(), report.GetTitle().c_str() );
//        pItem->setRenameEnabled( 0, false );
//        QFont font = this->font();
//        font.setBold( report.IsNew() );
//        pItem->SetFont( font );
//        pItem->SetFontColor( QColor( 150, 150, 150 ) );
//    }

}

    // Before we change the displayed reports, mark the old ones as read.
//    QListViewItem* pItem = firstChild();
//    while( pItem != 0 )
//    {
//        Report_ABC& report = GetItemValue( *pItem );
//        report.SetNew( false );
//        pItem = pItem->nextSibling();
//    }

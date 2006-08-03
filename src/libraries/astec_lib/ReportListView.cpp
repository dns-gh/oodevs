// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ReportListView.h"
#include "moc_ReportListView.cpp"

#include "Report_ABC.h"
#include "ReportFilterOptions.h"
#include "Reports.h"
#include "Entity_ABC.h"
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
    const Entity_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::OnOptionsChanged
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
void ReportListView::OnOptionsChanged()
{
    const Entity_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::NotifySelected
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
void ReportListView::NotifySelected( const Entity_ABC* element )
{
    if( element != selected_ )
    {
        selected_ = element;
        const Reports* reports = selected_ ? selected_->Retrieve< Reports >() : 0;
        if( reports )
            NotifyUpdated( *reports );
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

    DeleteTail( DisplayList( reports.reports_.rbegin(), reports.reports_.rend() ) );
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
            report->Display( displayer );
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
    report.Display( GetItemDisplayer( item ) );
}

    // Before we change the displayed reports, mark the old ones as read.
//    QListViewItem* pItem = firstChild();
//    while( pItem != 0 )
//    {
//        Report_ABC& report = GetItemValue( *pItem );
//        report.SetNew( false );
//        pItem = pItem->nextSibling();
//    }

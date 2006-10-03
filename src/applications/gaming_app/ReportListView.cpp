// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ReportListView.h"

#include "gaming/Report_ABC.h"
#include "ReportFilterOptions.h"
#include "gaming/Reports.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/ItemFactory_ABC.h"

using namespace kernel;
using namespace gui;

#include "moc_ReportListView.cpp"

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
    , menu_( new QPopupMenu( this ) )
{
    AddColumn( "Reçu" );
    AddColumn( "Compte-rendu" );
    
    // Set a descending sorting order, then disable user sorting.
    setSorting( 0, false );
    setSorting( -1, false );

    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint&, int ) ) );
    connect( this, SIGNAL( doubleClicked( QListViewItem*, const QPoint&, int ) ),        this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( spacePressed( QListViewItem* ) ),                             this, SLOT( OnRequestCenter() ) );

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
        MarkReportsAsRead();
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

// -----------------------------------------------------------------------------
// Name: ReportListView::OnRequestPopup
// Created: AGE 2006-09-18
// -----------------------------------------------------------------------------
void ReportListView::OnRequestPopup( QListViewItem* item, const QPoint& pos, int /*column*/ )
{
    if( !selected_ )
        return;
    menu_->clear();
    menu_->insertItem( tr( "Tout effacer" ),          this, SLOT( OnClearAll() ) );
    menu_->insertItem( tr( "Effacer les msg debug" ), this, SLOT( OnClearTrace() ) );
//    if( item )
//        menu_->insertItem( tr( "Effacer jusqu'ici" ), this, SLOT( OnClearUpTo() ) );
    menu_->popup( pos );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::MarkReportsAsRead
// Created: AGE 2006-09-21
// -----------------------------------------------------------------------------
void ReportListView::MarkReportsAsRead()
{
    const Reports* reports = 0;
    if( selected_ && ( reports = selected_->Retrieve< Reports >() ) != 0 )
        const_cast< Reports* >( reports )->MarkAsRead(); // $$$$ AGE 2006-09-18: 
}

// -----------------------------------------------------------------------------
// Name: ReportListView::OnClearAll
// Created: AGE 2006-09-18
// -----------------------------------------------------------------------------
void ReportListView::OnClearAll()
{
    const Reports* reports = 0;
    if( selected_ && ( reports = selected_->Retrieve< Reports >() ) != 0 )
        const_cast< Reports* >( reports )->Clear(); // $$$$ AGE 2006-09-18: 
}

// -----------------------------------------------------------------------------
// Name: ReportListView::OnClearTrace
// Created: AGE 2006-09-18
// -----------------------------------------------------------------------------
void ReportListView::OnClearTrace()
{
    const Reports* reports = 0;
    if( selected_ && ( reports = selected_->Retrieve< Reports >() ) != 0 )
        const_cast< Reports* >( reports )->ClearTraces(); // $$$$ AGE 2006-09-18: 
}

// -----------------------------------------------------------------------------
// Name: ReportListView::OnRequestCenter
// Created: AGE 2006-09-18
// -----------------------------------------------------------------------------
void ReportListView::OnRequestCenter()
{
    if( selectedItem() )
    {
        ValuedListItem* item = (ValuedListItem*)( selectedItem() );
        item->Activate( controllers_.actions_ );
    }
}

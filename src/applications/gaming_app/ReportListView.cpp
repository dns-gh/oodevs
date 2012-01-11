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

#include "reports/Report.h"
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
    , selected_( controllers )
    , menu_( new kernel::ContextMenu( this ) )
    , readTimer_( new QTimer( this ) )
{
    setFrameStyle( Q3Frame::Plain );
    setMargin( 2 );
    AddColumn( tr( "ISO Date" ) );
    AddColumn( tr( "Received" ) );
    AddColumn( tr( "Report" ) );
    setColumnWidthMode( 0, Q3ListView::Manual );
    setColumnWidth( 0, -1 );
    setColumnWidth( 1, 130 );
    setColumnAlignment( 1, Qt::AlignTop );
    setColumnAlignment( 2, Qt::AlignTop );

    // Set a descending sorting order, then disable user sorting.
    setSorting( 0, false );

    connect( this,       SIGNAL( contextMenuRequested( Q3ListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( Q3ListViewItem*, const QPoint&, int ) ) );
    connect( this,       SIGNAL( doubleClicked( Q3ListViewItem*, const QPoint&, int ) ),        this, SLOT( OnRequestCenter() ) );
    connect( this,       SIGNAL( spacePressed( Q3ListViewItem* ) ),                             this, SLOT( OnRequestCenter() ) );
    connect( this,       SIGNAL( selectionChanged() ),                                         this, SLOT( OnSelectionChanged() ) );
    connect( readTimer_, SIGNAL( timeout() ),                                                  this, SLOT( OnReadTimerOut() ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ReportListView destructor
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
ReportListView::~ReportListView()
{
    controllers_.Unregister( *this );
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
        else
            clear();
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

    ValuedListItem* item = DisplayList( reports.CreateIterator() );
                    item = DisplayList( reports.traces_.rbegin(), reports.traces_.rend(), this, item );
    DeleteTail( item );
    sort();
    for( Q3ListViewItem* item = firstChild(); item != 0; item = item->nextSibling() )
        static_cast< gui::ValuedListItem* >( item )->SetBackgroundColor( Qt::white, QColor( 240, 240, 240 ) );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::Display
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
void ReportListView::Display( const Report* report, Displayer_ABC& displayer, ValuedListItem* item )
{
    if( report )
        Display( *report, displayer, item );
    else
        RemoveItem( item );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::Display
// Created: AGE 2007-10-22
// -----------------------------------------------------------------------------
void ReportListView::Display( const Report& report, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item )
{
    if( &report.GetOwner() == selected_ && filter_.ShouldDisplay( report ) )
    {
        item->SetValue( &report );
        report.Display( displayer );
    }
    else
        RemoveItem( item );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::NotifyCreated
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
void ReportListView::NotifyCreated( const Report& report )
{
    if( ! isVisible() || & report.GetOwner() != selected_ )
        return;
    if( ! filter_.ShouldDisplay( report ) )
        return;
    ValuedListItem* item = factory_.CreateItem( this );
    item->SetValue( &report );
    report.Display( GetItemDisplayer( item ) );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::OnSelectionChanged
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
void ReportListView::OnSelectionChanged()
{
    readTimer_->start( 1500, true );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::OnReadTimerOut
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
void ReportListView::OnReadTimerOut()
{
    ValuedListItem* item = (ValuedListItem*)( selectedItem() );
    if( item )
    {
        const Report* report = item->GetValue< const Report >();
        const_cast< Report* >( report )->Read();
        report->Display( GetItemDisplayer( item ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ReportListView::OnRequestPopup
// Created: AGE 2006-09-18
// -----------------------------------------------------------------------------
void ReportListView::OnRequestPopup( Q3ListViewItem* /*item*/, const QPoint& pos, int /*column*/ )
{
    if( !selected_ )
        return;
    menu_->clear();
    menu_->insertItem( tr( "Clear" ),        this, SLOT( OnClearAll() ) );
    menu_->insertItem( tr( "Clear traces" ), this, SLOT( OnClearTrace() ) );
    filter_.AddContextMenu( menu_ );
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

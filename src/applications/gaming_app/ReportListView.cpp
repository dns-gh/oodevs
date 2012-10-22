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
#include "moc_ReportListView.cpp"
#include "gaming/Reports.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/ItemFactory_ABC.h"

Q_DECLARE_METATYPE( const Report* )

using namespace kernel;
using namespace gui;

namespace
{
    class CustomSortFilterProxyModel : public QSortFilterProxyModel
    {
    public:
        CustomSortFilterProxyModel( QObject* parent )
            : QSortFilterProxyModel( parent )
        {
            // NOTHING
        }
        ~CustomSortFilterProxyModel()
        {
            // NOTHING
        }
    protected:
        virtual bool lessThan( const QModelIndex& left, const QModelIndex& right ) const
        {
            QStandardItemModel* model = static_cast< QStandardItemModel* >( sourceModel() );
            if( left.isValid() && right.isValid() )
            {
                //getting second column modelIndex for each 
                QModelIndex leftIndex = model->index( model->itemFromIndex( left )->row(), 1 );
                QModelIndex rightIndex = model->index( model->itemFromIndex( right )->row(), 1 );

                //getting report pointer and time value
                QDateTime timeLeft  = model->data( leftIndex, Report::ReportRole ).value< const Report* >()->GetDateTime();
                QDateTime timeRight = model->data( rightIndex, Report::ReportRole ).value< const Report* >()->GetDateTime();
                return timeLeft < timeRight;
            }
            return false;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: ReportListView constructor
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
ReportListView::ReportListView( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory )
    : QTreeView( pParent )
    , controllers_( controllers )
    , factory_( factory )
    , selected_( controllers )
    , menu_( new kernel::ContextMenu( this ) )
    , readTimer_( new QTimer( this ) )
    , proxyFilter_ ( new CustomSortFilterProxyModel( this ) )
{
    //filter regexp
    toDisplay_.insert( Report::eRC );
    toDisplay_.insert( Report::eTrace );
    toDisplay_.insert( Report::eEvent );
    toDisplay_.insert( Report::eMessage );
    toDisplay_.insert( Report::eWarning );

    //configure treeview
    setModel( proxyFilter_ );
    setRootIsDecorated( false );
    setEditTriggers( 0 );
    setAlternatingRowColors( true );
    proxyFilter_->setFilterKeyColumn( 1 );
    proxyFilter_->setSourceModel( &reportModel_ );
    proxyFilter_->setFilterRole( Report::TypeFilterRole );
    SetFilterRegexp();
    proxyFilter_->setDynamicSortFilter( true );
    setSortingEnabled( true );

    //Add alternative palette
    QPalette p( palette() );
    p.setColor( QPalette::Base              , Qt::white );
    p.setColor( QPalette::AlternateBase     , QColor( 240, 240, 240 ) );
    setPalette( p );

    //configure the model
    reportModel_.setColumnCount( 2 );
    setHeaderHidden( true );
    header()->setResizeMode( 0, QHeaderView::ResizeToContents );
    header()->setResizeMode( 1, QHeaderView::Stretch );

    //connections
    connect( this, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( OnRequestCenter() ) );
    connect( selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( OnSelectionChanged() ) );
    connect( readTimer_, SIGNAL( timeout() ), this, SLOT( OnReadTimerOut() ) );
    connect( menu_, SIGNAL( triggered( QAction* ) ), SLOT( OnToggle( QAction* ) ) );

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
// Name: ReportListView::contextMenuEvent
// Created: NPT 2012-10-05
// -----------------------------------------------------------------------------
void ReportListView::contextMenuEvent( QContextMenuEvent * e )
{
    OnRequestPopup( e->globalPos() );
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
            reportModel_.removeRows( 0, reportModel_.rowCount() );
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

    reportModel_.removeRows( 0, reportModel_.rowCount() );
    tools::Iterator< const Report& > iterator = reports.CreateIterator();
    while( iterator.HasMoreElements() )
        reportModel_.appendRow( iterator.NextElement().GetReportData() );
    const Reports::T_Reports& traces = reports.GetTraces();
    for( size_t i = 0; i < traces.size(); ++i )
        reportModel_.appendRow( traces[ i ]->GetReportData() );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::NotifyCreated
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
void ReportListView::NotifyCreated( const Report& report )
{
    if( !isVisible() || & report.GetOwner() != selected_ )
        return;
    if( toDisplay_.find( report.GetType() ) == toDisplay_.end() )
        return;
    reportModel_.appendRow( report.GetReportData() );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::OnSelectionChanged
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
void ReportListView::OnSelectionChanged()
{
    readTimer_->start( 1500 );
    readTimer_->setSingleShot( true );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::OnReadTimerOut
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
void ReportListView::OnReadTimerOut()
{
    const QModelIndex index = selectionModel()->currentIndex();
    if( !index.isValid() )
        return;
    QStandardItem* item1 = reportModel_.itemFromIndex( proxyFilter_->mapToSource( index ) );
    QStandardItem* item2 = reportModel_.item( item1->row(), item1->column() == 0 ? 1 : 0 );
    if( item1 && item2 )
    {
        const_cast< Report* >( reportModel_.item( item1->row(), 1 )->data( Report::ReportRole ).value< const Report* >() )->Read();
        item1->setFont( QFont( "Calibri", 10, QFont::Normal ) );
        item2->setFont( QFont( "Calibri", 10, QFont::Normal ) );
    }

}

// -----------------------------------------------------------------------------
// Name: ReportListView::OnRequestPopup
// Created: AGE 2006-09-18
// -----------------------------------------------------------------------------
void ReportListView::OnRequestPopup( const QPoint& pos )
{
    if( !selected_ )
        return;
    menu_->clear();
    menu_->insertItem( tr( "Clear" ),        this, SLOT( OnClearAll() ) );
    menu_->insertItem( tr( "Clear traces" ), this, SLOT( OnClearTrace() ) );
    AddContextMenu();
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
// Name: ReportListView::SetFilterRegexp
// Created: NPT 2012-10-08
// -----------------------------------------------------------------------------
void ReportListView::SetFilterRegexp()
{
    QString regexp( "\\b(" );
    if( toDisplay_.empty() )
        regexp += "noSelection";
    for( std::set< Report::E_Type >::const_iterator it = toDisplay_.begin(); it != toDisplay_.end(); ++it )
    {
        if( it != toDisplay_.begin() )
            regexp += '|';
        regexp += QString::number( *it );
    }
    regexp += ")\\b";
    proxyFilter_->setFilterRegExp( regexp );
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
    const QModelIndex index = selectionModel()->currentIndex();
    if( !index.isValid() )
        return;
    QStandardItem* item = reportModel_.itemFromIndex( proxyFilter_->mapToSource( index ) );
    if( item && item->data( Report::ReportRole ).isValid() )
        item->data().value< const Report* >()->GetOwner().Activate( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::OnToggle
// Created: NPT 2012-10-08
// -----------------------------------------------------------------------------
void ReportListView::OnToggle( QAction* action )
{
    if( !action->data().isValid() )
        return;
    Report::E_Type type = static_cast< Report::E_Type >( action->data().toInt() );
    if( toDisplay_.find( type ) == toDisplay_.end() )
        toDisplay_.insert( type );
    else
        toDisplay_.erase( type );
    SetFilterRegexp();

    const Entity_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::AddMenuItem
// Created: SBO 2007-02-06
// -----------------------------------------------------------------------------
void ReportListView::AddMenuItem( const QString& name, Report::E_Type type ) const
{
    QAction* action = menu_->addAction( name );
    action->setData( type );
    action->setCheckable( true );
    action ->setChecked( toDisplay_.find( type ) != toDisplay_.end() );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::AddContextMenu
// Created: SBO 2007-02-06
// -----------------------------------------------------------------------------
void ReportListView::AddContextMenu() const
{
    menu_->insertSeparator();
    AddMenuItem( tr( "Show reports" ) , Report::eRC      );
    AddMenuItem( tr( "Show traces" )  , Report::eTrace   );
    AddMenuItem( tr( "Show events" )  , Report::eEvent   );
    AddMenuItem( tr( "Show messages" ), Report::eMessage );
    AddMenuItem( tr( "Show warnings" ), Report::eWarning );
}

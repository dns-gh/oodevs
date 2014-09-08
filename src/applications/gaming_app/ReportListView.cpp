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
#include "clients_gui/DisplayExtractor.h"
#include "clients_gui/LinkItemDelegate.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Controllers.h"

#pragma warning( disable : 4355 )

Q_DECLARE_METATYPE( const Report* )

#define ReportRole ( Qt::UserRole + 1 )
#define TypeFilterRole ( Qt::UserRole + 2 )
#define OrderRole ( Qt::UserRole + 3 )

unsigned int ReportListView::sortOrder_ = 0;

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
                const QDateTime& timeLeft  = model->data( leftIndex, ReportRole ).value< const Report* >()->GetDateTime();
                const QDateTime& timeRight = model->data( rightIndex, ReportRole ).value< const Report* >()->GetDateTime();
                if( timeLeft == timeRight )
                    return model->data( leftIndex, OrderRole ).toUInt() < model->data( rightIndex, OrderRole ).toUInt();
                else
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
ReportListView::ReportListView( QWidget* pParent, kernel::Controllers& controllers, gui::DisplayExtractor& extractor )
    : QTreeView( pParent )
    , controllers_( controllers )
    , extractor_( extractor )
    , selected_( controllers )
    , readTimer_( new QTimer( this ) )
    , proxyFilter_ ( new CustomSortFilterProxyModel( this ) )
    , delegate_( new gui::LinkItemDelegate( this ) )
{
    setItemDelegateForColumn( 1, delegate_ );
    //filter regexp
    toDisplay_.insert( Report::eRC );
    toDisplay_.insert( Report::eEvent );
    toDisplay_.insert( Report::eMessage );
    toDisplay_.insert( Report::eWarning );

    //configure treeview
    setModel( proxyFilter_ );
    setRootIsDecorated( false );
    setEditTriggers( 0 );
    setAlternatingRowColors( true );
    setMouseTracking( true );
    setAllColumnsShowFocus( true );
    proxyFilter_->setFilterKeyColumn( 1 );
    proxyFilter_->setSourceModel( &reportModel_ );
    proxyFilter_->setFilterRole( TypeFilterRole );
    SetFilterRegexp();
    proxyFilter_->setDynamicSortFilter( true );
    setSortingEnabled( true );

    //Add alternative palette
    QPalette p( palette() );
    p.setColor( QPalette::Base, Qt::white );
    p.setColor( QPalette::AlternateBase, QColor( 240, 240, 240 ) );
    setPalette( p );

    //configure the model
    reportModel_.setColumnCount( 3 );
    setHeaderHidden( true );
    header()->setResizeMode( 0, QHeaderView::ResizeToContents );
    header()->setResizeMode( 1, QHeaderView::ResizeToContents );

    //connections
    connect( this, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( OnRequestCenter() ) );
    connect( selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( OnSelectionChanged() ) );
    connect( readTimer_, SIGNAL( timeout() ), this, SLOT( OnReadTimerOut() ) );
    connect( delegate_, SIGNAL( LinkClicked( const QString&, const QModelIndex& ) ), SLOT( OnLinkClicked( const QString&, const QModelIndex& ) ) );

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
    const kernel::Entity_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::contextMenuEvent
// Created: NPT 2012-10-05
// -----------------------------------------------------------------------------
void ReportListView::contextMenuEvent( QContextMenuEvent * e )
{
    if( !selected_ )
        return;
    kernel::ContextMenu* menu = new kernel::ContextMenu( this );
    connect( menu, SIGNAL( aboutToHide() ), menu, SLOT( deleteLater() ) );
    connect( menu, SIGNAL( triggered( QAction* ) ), SLOT( OnToggle( QAction* ) ) );
    menu->insertItem( tr( "Clear" ),        this, SLOT( OnClearAll() ) );
    menu->insertItem( tr( "Clear traces" ), this, SLOT( OnClearTrace() ) );
    menu->insertSeparator();
    AddMenuItem( menu, tr( "Show reports" ) , Report::eRC      );
    AddMenuItem( menu, tr( "Show traces" )  , Report::eTrace   );
    AddMenuItem( menu, tr( "Show events" )  , Report::eEvent   );
    AddMenuItem( menu, tr( "Show messages" ), Report::eMessage );
    AddMenuItem( menu, tr( "Show warnings" ), Report::eWarning );
    menu->popup( e->globalPos() );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::NotifySelected
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
void ReportListView::NotifySelected( const kernel::Entity_ABC* element )
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
    return isVisible() && selected_ && selected_->Retrieve< Reports >() == &reports;
}

// -----------------------------------------------------------------------------
// Name: ReportListView::GetItems
// Created: JSR 2012-10-19
// -----------------------------------------------------------------------------
QList< QStandardItem* > ReportListView::GetItems( const Report& report ) const
{
    if( sortOrder_ >= std::numeric_limits< unsigned int >::max() -1 )
        sortOrder_ = 0;
    QList< QStandardItem* > list;
    QStandardItem* dateItem = new QStandardItem( report.GetDateTime().toString( Qt::LocalDate ) );
    dateItem->setForeground( report.GetColor() );

    QStandardItem* reportItem = new QStandardItem( report.GetMessage() ) ;
    reportItem->setForeground( report.GetColor() );
    reportItem->setData( QString::number( report.GetType() ), TypeFilterRole );
    reportItem->setData( QVariant::fromValue( &report ), ReportRole );
    reportItem->setData( sortOrder_, OrderRole );

    if( report.IsNew() )
    {
        QFont font = dateItem->font();
        font.setBold( true );
        dateItem->setFont( font );
        reportItem->setFont( font );
    }

    ++sortOrder_;
    list.append( dateItem );
    list.append( reportItem );

    return list;
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

    const Reports::T_TextReports& textReports = reports.GetReports();
    for( auto it = textReports.begin(); it != textReports.end(); ++it )
        reportModel_.appendRow( GetItems( *it->second ) );

    const Reports::T_Reports& traces = reports.GetTraces();
    for( size_t i = 0; i < traces.size(); ++i )
        reportModel_.appendRow(  GetItems( *traces[ i ] ) );
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
    reportModel_.appendRow( GetItems( report ) );
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
        const_cast< Report* >( reportModel_.item( item1->row(), 1 )->data( ReportRole ).value< const Report* >() )->Read();
        QFont font = item1->font();
        font.setBold( false );
        item1->setFont( font );
        item2->setFont( font );
    }
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
    if( item && item->data( ReportRole ).isValid() )
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

    const kernel::Entity_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::OnLinkClicked
// Created: JSR 2012-10-23
// -----------------------------------------------------------------------------
void ReportListView::OnLinkClicked( const QString& url, const QModelIndex& index )
{
    selectionModel()->setCurrentIndex( index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );
    extractor_.NotifyLinkClicked( url );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::AddMenuItem
// Created: SBO 2007-02-06
// -----------------------------------------------------------------------------
void ReportListView::AddMenuItem( QMenu* menu, const QString& name, Report::E_Type type ) const
{
    QAction* action = menu->addAction( name );
    action->setData( type );
    action->setCheckable( true );
    action ->setChecked( toDisplay_.find( type ) != toDisplay_.end() );
}

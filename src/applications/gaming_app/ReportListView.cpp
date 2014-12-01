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
#include "clients_gui/DisplayExtractor.h"
#include "clients_gui/LinkItemDelegate.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "gaming/AgentsModel.h"
#include "gaming/ReportsModel.h"
#include "protocol/Protocol.h"
#include "protocol/ServerPublisher_ABC.h"
#include "reports/ReportFactory.h"
#include <boost/bind.hpp>

#pragma warning( disable : 4355 )

unsigned int ReportListView::sortOrder_ = 0;

namespace
{
    enum E_Roles
    {
        DateRole =       Qt::UserRole,
        TypeFilterRole = Qt::UserRole + 1,
        OrderRole =      Qt::UserRole + 2,
        IdRole =         Qt::UserRole + 3
    };

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
            if( left.isValid() && right.isValid() )
            {
                QStandardItemModel* model = static_cast< QStandardItemModel* >( sourceModel() );
                if( !model )
                    return false;

                //getting second column modelIndex for each
                const auto leftIndex = model->index( model->itemFromIndex( left )->row(), 1 );
                const auto rightIndex = model->index( model->itemFromIndex( right )->row(), 1 );
                const auto timeLeft = model->data( leftIndex, DateRole ).value< QDateTime >();
                const auto timeRight = model->data( rightIndex, DateRole ).value< QDateTime >();
                if( timeLeft == timeRight )
                {
                    const auto typeLeft = model->data( leftIndex, TypeFilterRole ).value< int >();
                    const auto typeRight = model->data( rightIndex, TypeFilterRole ).value< int >();
                    if( typeLeft != Report::eTrace && typeRight != Report::eTrace )
                        return model->data( leftIndex, IdRole ).toUInt() > model->data( rightIndex, IdRole ).toUInt();
                    return model->data( leftIndex, OrderRole ).toUInt() < model->data( rightIndex, OrderRole ).toUInt();
                }
                return timeLeft < timeRight;
            }
            return false;
        }
    };
    unsigned int GetId( const sword::Tasker& id )
    {
        if( id.has_unit() )
            return id.unit().id();
        else if( id.has_automat() )
            return id.automat().id();
        else if( id.has_crowd() )
            return id.crowd().id();
        return 0u;
    }
}

// -----------------------------------------------------------------------------
// Name: ReportListView constructor
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
ReportListView::ReportListView( QWidget* pParent, kernel::Controllers& controllers, gui::DisplayExtractor& extractor,
                                const ReportFactory& factory, Publisher_ABC& publisher, ReportsModel& model,
                                const AgentsModel& agents )
    : QTreeView( pParent )
    , controllers_( controllers )
    , extractor_  ( extractor )
    , factory_    ( factory )
    , readTimer_  ( new QTimer( this ) )
    , proxyFilter_( new CustomSortFilterProxyModel( this ) )
    , delegate_   ( new gui::LinkItemDelegate( this ) )
    , model_      ( model )
    , agents_     ( agents )
    , selected_   ( 0 )
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

    publisher.Register( Publisher_ABC::T_SimHandler( [&]( const sword::SimToClient& message, bool /*ack*/ ) {
        if( !selected_ || !isVisible() )
            return;
        if( message.message().has_report() )
            CreateReport( message.message().report() );
        else if( message.message().has_trace() )
            CreateTrace( message.message().trace() );
    } ) );
    publisher.Register( Publisher_ABC::T_ReplayHandler( [&]( const sword::ReplayToClient& message )
    {
        if( !message.message().has_list_reports_ack() )
            return;
        const auto& msg = message.message().list_reports_ack();
        if( msg.has_next_report() )
            return;
        FillReports();
    } ) );
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
    FillReports();
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
    if( !isVisible() )
        return;

    model_.ReadReports( selected_ );
    const auto id = element ? element->GetId() : 0u;
    if( selected_ == id )
        return;

    selected_ = id;
    FillReports();
}

void ReportListView::FillReports()
{
    reportModel_.removeRows( 0, reportModel_.rowCount() );
    if( selected_ != 0 )
        AddReports();
}

void ReportListView::AddReports()
{
    const auto* entity =  agents_.FindAllAgent( selected_ );
    if( !entity )
        return;
    const auto unreadMessages = model_.HaveUnreadReports( selected_ );

    const auto& reports = model_.GetReports( selected_ );
    for( auto it = reports.begin(); it != reports.end(); ++it )
        CreateItem< sword::Report >( *it, *entity, boost::bind( &ReportFactory::CreateReport, &factory_, _1, _2 ),
            unreadMessages );

    const auto& traces = model_.GetTraces( selected_ );
    for( auto it = traces.begin(); it != traces.end(); ++it )
        CreateItem< sword::Trace >( *it, *entity, boost::bind( &ReportFactory::CreateTrace, &factory_, _1, _2 ),
            unreadMessages );
}

void ReportListView::CreateReport( const sword::Report& report )
{
    if( selected_ != GetId( report.source() ) )
        return;
    const auto* entity = agents_.FindAllAgent( selected_ );
    if( !entity )
        return;
    CreateItem< sword::Report >( report, *entity, boost::bind( &ReportFactory::CreateReport, &factory_, _1, _2 ), true );
}

void ReportListView::CreateTrace( const sword::Trace& trace )
{
    if( selected_ != GetId( trace.source() ) )
        return;
    const auto* entity = agents_.FindAllAgent( selected_ );
    if( !entity )
        return;
    CreateItem< sword::Trace >( trace, *entity, boost::bind( &ReportFactory::CreateTrace, &factory_, _1, _2 ), true );
}

template< typename T >
void ReportListView::CreateItem( const T& message, const kernel::Entity_ABC& entity,
    const std::function< boost::shared_ptr< Report >( const kernel::Entity_ABC&, const T& fun ) >& func,
    bool unreadMessages )
{
    if( sortOrder_ >= std::numeric_limits< unsigned int >::max() -1 )
        sortOrder_ = 0;

    const auto report = func( entity, message );
    QList< QStandardItem* > list;
    QStandardItem* dateItem = new QStandardItem( report->GetDateTime().toString( Qt::LocalDate ) );
    dateItem->setForeground( report->GetColor() );

    QStandardItem* reportItem = new QStandardItem( report->GetMessage() ) ;
    reportItem->setForeground( report->GetColor() );
    reportItem->setData( report->GetType(), TypeFilterRole );
    reportItem->setData( report->GetDateTime(), DateRole );
    reportItem->setData( sortOrder_, OrderRole );
    reportItem->setData( report->GetId(), IdRole );

    if( unreadMessages )
    {
        QFont font = dateItem->font();
        font.setBold( true );
        dateItem->setFont( font );
        reportItem->setFont( font );
    }

    ++sortOrder_;
    list.append( dateItem );
    list.append( reportItem );

    reportModel_.appendRow( list );
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
        QFont font = item1->font();
        font.setBold( false );
        item1->setFont( font );
        item2->setFont( font );
    }
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
    model_.Clear( selected_ );
    FillReports();
}

// -----------------------------------------------------------------------------
// Name: ReportListView::OnClearTrace
// Created: AGE 2006-09-18
// -----------------------------------------------------------------------------
void ReportListView::OnClearTrace()
{
    model_.ClearTraces( selected_ );
    FillReports();
}

// -----------------------------------------------------------------------------
// Name: ReportListView::OnRequestCenter
// Created: AGE 2006-09-18
// -----------------------------------------------------------------------------
void ReportListView::OnRequestCenter()
{
    const auto* entity = agents_.FindAllAgent( selected_ );
    if( !entity )
        return;
    entity->Activate( controllers_.actions_ );
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
    FillReports();
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

void ReportListView::Purge()
{
    reportModel_.removeRows( 0, reportModel_.rowCount() );
}

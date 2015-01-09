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
#include "gaming/SimulationController.h"
#include "protocol/MessageParameters.h"
#include "protocol/Protocol.h"
#include "protocol/ServerPublisher_ABC.h"
#include "reports/ReportFactory.h"
#include <boost/optional.hpp>

#pragma warning( disable : 4355 )

// -----------------------------------------------------------------------------
// Name: ReportListView constructor
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
ReportListView::ReportListView( QWidget* pParent, kernel::Controllers& controllers, gui::DisplayExtractor& extractor,
                                const ReportFactory& factory, ReportsModel& model,
                                const AgentsModel& agents, SimulationController& simulation )
    : QTreeView( pParent )
    , controllers_   ( controllers )
    , extractor_     ( extractor )
    , factory_       ( factory )
    , readTimer_     ( new QTimer( this ) )
    , delegate_      ( new gui::LinkItemDelegate( this ) )
    , model_         ( model )
    , agents_        ( agents )
    , selected_      ( 0 )
    , simulation_    ( simulation )
    , unreadMessages_( false )
    , context_       ( 0 )
{
    setItemDelegateForColumn( 1, delegate_ );
    //filter regexp
    toDisplay_.insert( Report::eRC );
    toDisplay_.insert( Report::eEvent );
    toDisplay_.insert( Report::eMessage );
    toDisplay_.insert( Report::eWarning );

    //configure treeview
    setModel( &reportModel_ );
    setRootIsDecorated( false );
    setEditTriggers( 0 );
    setAlternatingRowColors( true );
    setMouseTracking( true );
    setAllColumnsShowFocus( true );
    setUniformRowHeights(true);
    setSortingEnabled( false );

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

    simulation_.RegisterSimHandler( [&]( const sword::SimToClient& message, bool ack ) {
        if( message.message().has_list_reports_ack() )
        {
            const auto context = message.context();
            if( !ack || context != context_ )
                return;
            FillReports( message.message().list_reports_ack() );
        }
        else if( message.message().has_report() )
            Create( message.message().report() );
        else if( message.message().has_trace() )
            Create( message.message().trace() );
    } );
    simulation_.RegisterReplayHandler( [&]( const sword::ReplayToClient& message )
    {
        if( message.message().has_list_reports_ack() )
        {
            const auto context = message.context();
            if( context != context_ )
                return;
            FillReports( message.message().list_reports_ack() );
        }
        else if( message.message().has_control_skip_to_tick_ack() )
            SendRequest( message.message().control_skip_to_tick_ack().tick() );
    } );
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
void ReportListView::showEvent( QShowEvent* event )
{
    if( event && !event->spontaneous() )
        SendRequest( simulation_.GetCurrentTick() );
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
    if( selected_ )
        SendRequest( simulation_.GetCurrentTick() );
    else
        Purge();
}

void ReportListView::SendRequest( unsigned int tick )
{
    if( !selected_ || !isVisible() )
        return;
    unreadMessages_ = model_.HasUnreadReports( selected_ );
    simulation_.SendReportsRequest( selected_, ++context_, tick );
}

void ReportListView::FillReports( const sword::ListReportsAck& ack )
{
    Purge();
    if( !selected_ || !isVisible() )
        return;
    reports_.clear();

    // Add reports
    for( int i = ack.reports_size() - 1; i >= 0; --i )
    {
        ReportsModel::Message message( selected_, ack.reports( i ), simulation_.GetDateTime() );
        reports_.push_back( message );
    }

    // Add traces
    const auto& traces = model_.GetTraces( selected_ );
    if( !traces.empty() )
    {
        for( auto it = traces.begin(); it != traces.end(); ++it )
            reports_.push_back( *it );
        std::sort( reports_.begin(), reports_.end(),
            [&]( const ReportsModel::Message& lhs, const ReportsModel::Message& rhs ) -> bool
            {
                return lhs.id_ > rhs.id_;
            } );
    }
    Refresh();
}

void ReportListView::Refresh()
{
    reportModel_.removeRows( 0, reportModel_.rowCount() );
    const auto* entity =  agents_.FindAllAgent( selected_ );
    if( !entity )
        return;

    for( auto it = reports_.begin(); it != reports_.end(); ++it )
        CreateItem( *it, *entity, unreadMessages_ );
}

template< typename T >
void ReportListView::Create( const T& report )
{
    const auto id = protocol::TryGetTasker( report.source() );
    if( !id )
        return;
    if( selected_ != *id )
        return;
    const auto* entity = agents_.FindAllAgent( selected_ );
    if( !entity )
        return;
    ReportsModel::Message message( selected_, report, simulation_.GetDateTime() );
    reports_.push_back( message );
    CreateItem( message, *entity, true );
}

void ReportListView::CreateItem( const ReportsModel::Message& message, const kernel::Entity_ABC& entity, bool unreadMessages )
{
    boost::shared_ptr< Report > report;
    if( message.report_.IsInitialized() )
        report = factory_.CreateReport( entity, message.report_ );
    else if( message.trace_.IsInitialized() )
        report = factory_.CreateTrace( entity, message.trace_, message.date_ );

    if( !report || toDisplay_.find( report->GetType() ) == toDisplay_.end() )
        return;

    QList< QStandardItem* > list;
    QStandardItem* dateItem = new QStandardItem( report->GetDateTime().toString( Qt::LocalDate ) );
    dateItem->setForeground( report->GetColor() );

    QStandardItem* reportItem = new QStandardItem( report->GetMessage() ) ;
    reportItem->setForeground( report->GetColor() );

    if( unreadMessages )
    {
        QFont font = dateItem->font();
        font.setBold( true );
        dateItem->setFont( font );
        reportItem->setFont( font );
    }

    list.append( dateItem );
    list.append( reportItem );

    reportModel_.insertRow( 0, list );
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

namespace
{
    void SetBold( QStandardItem* item, bool bold )
    {
        if( item )
        {
            QFont font = item->font();
            font.setBold( bold );
            item->setFont( font );
        }
    }
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
    SetBold( reportModel_.item( index.row(), index.column() ), false );
    SetBold( reportModel_.item( index.row(), index.column() == 0 ? 1 : 0 ), false );
}

// -----------------------------------------------------------------------------
// Name: ReportListView::OnClearTrace
// Created: AGE 2006-09-18
// -----------------------------------------------------------------------------
void ReportListView::OnClearTrace()
{
    model_.ClearTraces( selected_ );
    for( auto it = reports_.begin(); it != reports_.end(); )
    {
        if( it->trace_.IsInitialized() )
            it = reports_.erase( it );
        else
            ++it;
    }
    Refresh();
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
    Refresh();
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

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "PartsView.h"
#include "moc_PartsView.cpp"

#include "clients_kernel/BreakdownType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Entity_ABC.h"
#include "gaming/LogMaintenanceConsign.h"
#include "gaming/SupplyStates.h"

PartsView::PartsView( kernel::Controllers& controllers, QWidget* parent )
    : RichTableView( "manual_selection_repair_team_partsview", parent )
    , controller_  ( controllers.controller_ )
    , base_        ( controllers.controller_ )
    , valid_       ( false )
{
    model_ = new QStandardItemModel( parent );
    setEditTriggers( 0 );
    setModel( model_ );
    setSortingEnabled( false );
    setAlternatingRowColors( true );
    setSelectionMode( SingleSelection );
    setSelectionBehavior( SelectRows );
    setFocusPolicy( Qt::NoFocus );
    controller_.Register( *this );
}

PartsView::~PartsView()
{
    controller_.Unregister( *this );
}

void PartsView::Purge()
{
    model_->clear();
    const QStringList headers = QStringList()
        << tr( "Required Parts" )
        << tr( "Required" )
        << tr( "Available" );
    model_->setHorizontalHeaderLabels( headers );
    auto header = horizontalHeader();
    header->setResizeMode( 0, QHeaderView::Stretch );
    header->setResizeMode( 1, QHeaderView::ResizeToContents );
    header->setResizeMode( 2, QHeaderView::ResizeToContents );
    header->setHighlightSections( false );
    verticalHeader()->setVisible( false );
    parts_.clear();
}

void PartsView::Select( kernel::Entity_ABC* handler, const LogMaintenanceConsign& consign )
{
    base_ = handler;
    Purge();
    const auto& parts = consign.GetBreakdown()->GetParts();
    int row = 0;
    for( auto it = parts.begin(); it != parts.end(); ++it )
    {
        model_->setItem( row, 0, new QStandardItem( QString::fromStdString( it->resource ) ) );
        model_->setItem( row, 1, new QStandardItem( QString::number( it->quantity ) ) );
        model_->setItem( row, 2, new QStandardItem( "0" ) );
        parts_.insert( std::make_pair( it->resource, row ) );
        row++;
    }
    if( auto states = base_->Retrieve< SupplyStates >() )
        NotifyUpdated( *states );
}

void PartsView::setSelection( const QRect& /*rect*/, QItemSelectionModel::SelectionFlags /*flags*/ )
{
    // NOTHING
}

void PartsView::NotifyUpdated( const SupplyStates& states )
{
    if( !base_ || base_ != &states.entity_ )
        return;
    auto it = states.CreateIterator();
    QBrush red;
    red.setColor( Qt::red );
    bool valid = true;
    while( it.HasMoreElements() )
    {
        const auto& elem = it.NextElement();
        auto part = parts_.find( elem.type_->GetName() );
        if( part == parts_.end() )
            continue;
        const QModelIndex& available = model_->index( part->second, 2 );
        model_->setData( available, elem.quantity_ );
        const int required = model_->index( part->second, 1 ).data().toInt();
        if( elem.quantity_ >= required )
            continue;
        valid = false;
        for( int i = 0; i < 3; i++ )
            model_->setData( model_->index( part->second, i ), red, Qt::ForegroundRole );
    }
    valid_ = valid;
}

bool PartsView::IsValid() const
{
    return valid_;
}
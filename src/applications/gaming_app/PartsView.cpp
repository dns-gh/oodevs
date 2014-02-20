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

#include "clients_kernel/BreakdownPart.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Entity_ABC.h"
#include "gaming/Dotation.h"
#include "gaming/Dotations.h"
#include "gaming/LogMaintenanceConsign.h"

PartsView::PartsView( kernel::Controllers& controllers, QWidget* parent )
    : RichTableView( "manual_selection_repair_team_partsview", parent )
    , controller_  ( controllers.controller_ )
    , base_        ( controllers.controller_ )
    , valid_       ( false )
{
    const QStringList headers = QStringList()
        << tr( "Required Parts" )
        << tr( "Required" )
        << tr( "Available" );
    model_ = new QStandardItemModel( parent );
    model_->setHorizontalHeaderLabels( headers );
    setEditTriggers( 0 );
    setModel( model_ );
    setSortingEnabled( false );
    setAlternatingRowColors( true );
    setSelectionMode( SingleSelection );
    setSelectionBehavior( SelectRows );
    setFocusPolicy( Qt::NoFocus );
    auto header = horizontalHeader();
    header->setResizeMode( 0, QHeaderView::Stretch );
    header->setResizeMode( 1, QHeaderView::ResizeToContents );
    header->setResizeMode( 2, QHeaderView::ResizeToContents );
    header->setHighlightSections( false );
    verticalHeader()->setVisible( false );
    controller_.Register( *this );
}

PartsView::~PartsView()
{
    controller_.Unregister( *this );
}

void PartsView::Purge()
{
    model_->removeRows( 0, model_->rowCount() );
    parts_.clear();
}

void PartsView::Select( kernel::Entity_ABC* handler, const std::vector< kernel::BreakdownPart >& parts )
{
    base_ = handler;
    Purge();
    int row = 0;
    valid_ = true;
    for( auto it = parts.begin(); it != parts.end(); ++it )
    {
        const std::string resourceName = it->GetResource().GetName();
        unsigned int quantity = it->GetQuantity();
        model_->setItem( row, 0, new QStandardItem( QString::fromStdString( resourceName ) ) );
        model_->setItem( row, 1, new QStandardItem( QString::number( quantity ) ) );
        model_->setItem( row, 2, new QStandardItem( "0" ) );
        parts_.insert( std::make_pair( resourceName, row ) );
        valid_ &= !quantity;
        row++;
    }
    if( auto dotations = base_->Retrieve< kernel::Dotations_ABC >() )
        NotifyUpdated( *dotations );
}

void PartsView::setSelection( const QRect& /*rect*/, QItemSelectionModel::SelectionFlags /*flags*/ )
{
    // NOTHING
}

void PartsView::NotifyUpdated( const kernel::Dotations_ABC& dotations )
{
    const auto dots = dynamic_cast< const Dotations* >( &dotations );
    if( !dots )
        return;
    if( !base_ || base_ != &dots->entity_ )
        return;
    auto it = dots->CreateIterator();
    const QBrush red( Qt::red );
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
        const bool isRowValid = elem.quantity_ >= required;
        valid &= isRowValid;
        const QVariant next = isRowValid ? QVariant() : red;
        for( int i = 0; i < 3; i++ )
            model_->setData( model_->index( part->second, i ), next, Qt::ForegroundRole );
    }
    valid_ = valid;
    emit Updated();
}

bool PartsView::IsValid() const
{
    return valid_;
}

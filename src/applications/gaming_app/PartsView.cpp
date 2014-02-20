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
    , entity_      ( controllers.controller_ )
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
    setSelectionMode( NoSelection );
    setSelectionBehavior( SelectRows );
    setFocusPolicy( Qt::NoFocus );
    auto header = horizontalHeader();
    header->setResizeMode( eName, QHeaderView::Stretch );
    header->setResizeMode( eRequired, QHeaderView::ResizeToContents );
    header->setResizeMode( eAvailable, QHeaderView::ResizeToContents );
    header->setHighlightSections( false );
    verticalHeader()->setVisible( false );
    controller_.Register( *this );
}

PartsView::~PartsView()
{
    controller_.Unregister( *this );
}

void PartsView::Fill( const std::vector< kernel::BreakdownPart >& parts )
{
    valid_ = false;
    model_->removeRows( 0, model_->rowCount() );
    int row = 0;
    for( auto it = parts.begin(); it != parts.end(); ++it )
    {
        const std::string resourceName = it->GetResource().GetName();
        unsigned int quantity = it->GetQuantity();
        model_->setItem( row, eName, new QStandardItem( QString::fromStdString( resourceName ) ) );
        model_->setItem( row, eRequired, new QStandardItem( QString::number( quantity ) ) );
        SetAvailable( row, -1 );
        row++;
    }
}

void PartsView::SelectEntity( const kernel::Entity_ABC* entity )
{
    valid_ = false;
    entity_ = entity;
    auto dotations = entity_ ? entity_->Retrieve< kernel::Dotations_ABC >() : 0;
    if( dotations )
       NotifyUpdated( *dotations );
    else
        for( int row = 0; row < model_->rowCount(); ++row )
            SetAvailable( row, -1 );
}

bool PartsView::SetAvailable( int row, int available )
{
    if( available < 0 )
    {
        model_->setItem( row, eAvailable, new QStandardItem( tr( "N/A" ) ) );
        for( int col = 0; col < eNbrColumn; ++col )
            model_->setData( model_->index( row, col ), QVariant(), Qt::ForegroundRole );
        return false;
    }
    model_->item( row, eAvailable )->setText( QString::number( available ) );
    const bool isRowValid = available >= model_->item( row, eRequired )->text().toInt();
    const QVariant next = isRowValid ? QVariant() : QBrush( Qt::red );
    for( int col = 0; col < eNbrColumn; ++col )
        model_->setData( model_->index( row, col ), next, Qt::ForegroundRole );
    return isRowValid;
}


void PartsView::NotifyUpdated( const kernel::Dotations_ABC& dotations )
{
    const auto dots = dynamic_cast< const Dotations* >( &dotations );
    if( !dots || !entity_ || entity_ != &dots->entity_ )
        return;
    valid_ = true;
    for( int row = 0; row < model_->rowCount(); ++row )
    {
        auto it = dots->CreateIterator();
        bool isRowValid = false;
        bool hasDotation = false;
        while( it.HasMoreElements() && !hasDotation )
        {
            const auto& elem = it.NextElement();
            if( model_->item( row, eName )->text().toStdString() == elem.type_->GetName() )
            {
                isRowValid = SetAvailable( row, elem.quantity_ );
                hasDotation = true;
            }
        }
        if( !hasDotation )
            SetAvailable( row, 0 );
        valid_ &= isRowValid;
    }
    emit Updated();
}

bool PartsView::IsValid() const
{
    return valid_;
}

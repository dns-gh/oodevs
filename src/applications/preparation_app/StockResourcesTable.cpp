// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "StockResourcesTable.h"
#include "moc_StockResourcesTable.cpp"
#include "clients_gui/CommonDelegate.h"
#include "clients_kernel/DotationType.h"

// -----------------------------------------------------------------------------
// Name: StockResourcesTable Constructor
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
StockResourcesTable::StockResourcesTable( const QString& objectName, QWidget* parent, const kernel::Resolver2< kernel::DotationType >& dotationsType ) 
    : ResourcesEditorTable_ABC( objectName, parent, dotationsType )
{
    InitHeader();
    delegate_->AddDoubleSpinBoxOnColumn( 4, 0, std::numeric_limits< int >::max() );
}

// -----------------------------------------------------------------------------
// Name: StockResourcesTable Destructor
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
StockResourcesTable::~StockResourcesTable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StockResourcesTable::InitHeader
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void StockResourcesTable::InitHeader()
{
    dataModel_->setHorizontalHeaderLabels( QStringList() << tr( "Resource" ) << tr( "Nature" ) << tr( "Weight (T)" ) << tr( "Volume (m3)" ) << tr( "Quantity" ) );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    horizontalHeader()->setResizeMode( 0, QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeMode( 1, QHeaderView::ResizeToContents );
}

// -----------------------------------------------------------------------------
// Name: StockResourcesTable::OnValueChanged
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void StockResourcesTable::OnValueChanged( int row, double value )
{
    int dotationId = dataModel_->item( row, 0 )->data( Qt::UserRole ).toInt();
    if( auto pDotation = dotations_.Find( dotationId ) )
    {
        dataModel_->setData( dataModel_->index( row, 2 ), QString::number( value * pDotation->GetUnitWeight(), 'f', 2 ) );
        dataModel_->setData( dataModel_->index( row, 3 ), QString::number( value * pDotation->GetUnitVolume(), 'f', 2 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: StockResourcesTable::AddResource
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void StockResourcesTable::AddResource( const kernel::DotationType& resource, double value /*= 0.0*/ )
{
    int newRowIndex = dataModel_->rowCount();
    dataModel_->setRowCount( newRowIndex + 1 );
    dataModel_->setData( dataModel_->index( newRowIndex, 0 ), QString::fromStdString( resource.GetName() ) );
    dataModel_->setData( dataModel_->index( newRowIndex, 0 ), static_cast< int >( resource.GetId() ), Qt::UserRole );
    dataModel_->setData( dataModel_->index( newRowIndex, 1 ), QString::fromStdString( resource.GetNature() ) );
    dataModel_->setData( dataModel_->index( newRowIndex, 2 ), QString::number( value * resource.GetUnitWeight(), 'f', 2 ) );
    dataModel_->setData( dataModel_->index( newRowIndex, 3 ), QString::number( value * resource.GetUnitVolume(), 'f', 2 ) );
    dataModel_->setData( dataModel_->index( newRowIndex, 4 ), value );
    dataModel_->setData( dataModel_->index( newRowIndex, 4 ), value, Qt::UserRole );
    dataModel_->item( newRowIndex, 2 )->setTextAlignment( Qt::AlignRight | Qt::AlignVCenter );
    dataModel_->item( newRowIndex, 3 )->setTextAlignment( Qt::AlignRight | Qt::AlignVCenter );
    dataModel_->item( newRowIndex, 4 )->setTextAlignment( Qt::AlignRight | Qt::AlignVCenter );
}

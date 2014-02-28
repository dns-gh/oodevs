// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "QuotasResourcesTable.h"
#include "moc_QuotasResourcesTable.cpp"
#include "clients_gui/CommonDelegate.h"
#include "clients_kernel/DotationType.h"

// -----------------------------------------------------------------------------
// Name: QuotasResourcesTable Constructor
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
QuotasResourcesTable::QuotasResourcesTable( const QString& objectName, QWidget* parent, const kernel::Resolver2< kernel::DotationType >& dotationsType ) 
    : ResourcesEditorTable_ABC( objectName, parent, dotationsType )
{
    InitHeader();
    delegate_->AddDoubleSpinBoxOnColumn( 1, 0, std::numeric_limits< int >::max() );
}

// -----------------------------------------------------------------------------
// Name: QuotasResourcesTable Destructor
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
QuotasResourcesTable::~QuotasResourcesTable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: QuotasResourcesTable::InitHeader
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void QuotasResourcesTable::InitHeader()
{
    dataModel_->setHorizontalHeaderLabels( QStringList() <<  tr( "Resource" ) << tr( "Quantity" ) );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
}

// -----------------------------------------------------------------------------
// Name: QuotasResourcesTable::AddResource
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void QuotasResourcesTable::AddResource( const kernel::DotationType& resource, double value /* = 0 */ )
{
    int newRowIndex = dataModel_->rowCount();
    dataModel_->setRowCount( newRowIndex + 1 );
    dataModel_->setData( dataModel_->index( newRowIndex, 0 ), QString::fromStdString( resource.GetName() ) );
    dataModel_->setData( dataModel_->index( newRowIndex, 0 ), static_cast< int >( resource.GetId() ), Qt::UserRole );
    dataModel_->setData( dataModel_->index( newRowIndex, 1 ), value );
    dataModel_->setData( dataModel_->index( newRowIndex, 1 ), value, Qt::UserRole );
    dataModel_->item( newRowIndex, 1 )->setTextAlignment( Qt::AlignRight | Qt::AlignVCenter );
}

// -----------------------------------------------------------------------------
// Name: QuotasResourcesTable::OnValueChanged
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void QuotasResourcesTable::OnValueChanged( int /*row*/, double /*value*/ )
{
    // NOTHING
}

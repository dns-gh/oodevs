// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::UnitStateTableResource */

#include "clients_gui_pch.h"
#include "UnitStateTableResource.h"
#include "moc_UnitStateTableResource.cpp"
#include "SpinTableItem.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource constructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateTableResource::UnitStateTableResource( QWidget* parent, const QString maximalCapacityLabel )
    : UnitStateTable_ABC( parent, 7 )
{
    horizontalHeaders_ << tr( "Resources" )
                       << tr( "Category" )
                       << tr( "Quantity" )
                       << maximalCapacityLabel
                       << tr( "Percentage (%)" )
                       << tr( "Logistic threshold (%)" );
    delegate_.AddDoubleSpinBoxOnColumn( eThreshold, 0, 100, 0.5 );
    connect( &dataModel_, SIGNAL( itemChanged( QStandardItem* ) ), SLOT( OnItemChanged( QStandardItem* ) ) );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource destructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateTableResource::~UnitStateTableResource()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::MergeLine
// Created: ABR 2011-07-06
// -----------------------------------------------------------------------------
void UnitStateTableResource::MergeLine( const QString& name, const QString& category, unsigned quantity, unsigned maximum, double threshold )
{
    for( int row = 0; row < dataModel_.rowCount(); ++row )
        if( GetDisplayData( row, eName ) == name )
        {
            quantity += GetUserData( row, eQuantity ).toUInt();
            maximum += GetUserData( row, eMaximum ).toUInt();
            SetData( row, eMaximum, QString::number( maximum ), maximum );
            SetData( row, eQuantity, QString::number( quantity ), quantity );
            return;
        }
    AddLine( name, category, quantity, maximum, threshold );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::AddLine
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateTableResource::AddLine( const QString& name, const QString& category, unsigned quantity /*= 0*/, unsigned maximum /*= 0*/, double threshold /*= 0.*/ )
{
    int row = dataModel_.rowCount();
    AddItem( row, eName, name, name );
    AddItem( row, eCategory, category, category );
    AddItem( row, ePercentage, "", "" );
    AddItem( row, eMaximum, QString::number( maximum ), maximum );
    AddItem( row, eQuantity, QString::number( quantity ), quantity, Qt::ItemIsEditable );
    AddItem( row, eThreshold, QString::number( threshold, 'f', 2 ), threshold, Qt::ItemIsEditable );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::OnItemChanged
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateTableResource::OnItemChanged( QStandardItem* item )
{
    if( item && item->column() == eQuantity )
    {
        int quantity = GetUserData( item->row(), eQuantity ).toInt();
        int maximum = GetUserData( item->row(), eMaximum ).toInt();
        if( quantity == 0 && maximum == 0 )
            SetData( item->row(), ePercentage, "0.00", 0 );
        else
        {
            double percentage = std::min( quantity * 100. / static_cast< double >( maximum ), 100. );
            SetData( item->row(), ePercentage, QString::number( percentage, 'f', 2 ), percentage );
        }
        dataModel_.item( item->row(), eMaximum )->setBackground( ( quantity <= maximum ) ? item->background() : QBrush( Qt::gray ) );
        dataModel_.item( item->row(), ePercentage )->setBackground( ( quantity <= maximum ) ? item->background() : QBrush( Qt::gray ) );
    }
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UnitStateTableResource.h"
#include "moc_UnitStateTableResource.cpp"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource constructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateTableResource::UnitStateTableResource( QWidget* parent, const QString maximalCapacityLabel )
    : UnitStateTable_ABC( "UnitStateTableResource", parent, 7 )
{
    horizontalHeaders_ << tr( "Resources" )
                       << tr( "Category" )
                       << tr( "Quantity" )
                       << maximalCapacityLabel
                       << tr( "Percentage (%)" )
                       << tr( "Logistic threshold (%)" )
                       << tr( "Normalized consumption" );
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
void UnitStateTableResource::MergeLine( const QString& name, const QString& category, unsigned quantity, unsigned maximum, double threshold, double consumption )
{
    for( int row = 0; row < dataModel_.rowCount(); ++row )
        if( GetDisplayData( row, eName ) == name )
        {
            quantity += GetUserData( row, eQuantity ).toUInt();
            maximum += GetUserData( row, eMaximum ).toUInt();
            consumption += GetUserData( row, eConsumption ).toDouble();
            SetData( row, eMaximum, locale().toString( maximum ), maximum );
            SetData( row, eQuantity, locale().toString( quantity ), quantity );
            SetData( row, eConsumption, locale().toString( consumption ), consumption );
            return;
        }
    AddLine( name, category, quantity, maximum, threshold, consumption );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::AddLine
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateTableResource::AddLine( const QString& name, const QString& category, unsigned quantity /* = 0*/, unsigned maximum /* = 0*/, double threshold /* = 0.*/, double consumption /* = 0.*/ )
{
    int row = dataModel_.rowCount();
    AddItem( row, eName, name, name );
    AddItem( row, eCategory, category, category );
    AddItem( row, ePercentage, "", "" );
    AddItem( row, eMaximum, locale().toString( maximum ), maximum );
    AddItem( row, eQuantity, locale().toString( quantity ), quantity, Qt::ItemIsEditable );
    AddItem( row, eThreshold, locale().toString( threshold, 'f', 2 ), threshold, Qt::ItemIsEditable );
    AddItem( row, eConsumption, locale().toString( consumption ), consumption );
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
            SetData( item->row(), ePercentage, locale().toString( percentage, 'f', 2 ), percentage );
        }
        if( quantity <= maximum )
        {
            SetColor( item->row(), eMaximum, item->background(), -1 );
            SetColor( item->row(), ePercentage, item->background(), -1 );
        }
        else
        {
            SetColor( item->row(), eMaximum, Qt::gray, 0 );
            SetColor( item->row(), ePercentage, Qt::gray, 0 );
        }
        if( maximum == 0 )
        {
            QStandardItem* itemPercentage = dataModel_.item( item->row(), ePercentage );
            if( itemPercentage )
                itemPercentage->setFlags( Qt::ItemIsSelectable );
        }
    }
}

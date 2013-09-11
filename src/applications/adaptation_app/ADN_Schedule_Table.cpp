// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Schedule_Table.h"
#include "ADN_Gui_Tools.h"
#include "ADN_Inhabitants_Data.h"

// -----------------------------------------------------------------------------
// Name: ADN_Schedule_Table constructor
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
ADN_Schedule_Table::ADN_Schedule_Table( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent )
    : ADN_Table( objectName, connector, pParent )
{
    dataModel_.setColumnCount( 4 );
    QStringList horizontalHeaders;
    horizontalHeaders << tools::translate( "ADN_Schedule_Table", "Day" )
                      << tools::translate( "ADN_Schedule_Table", "From" )
                      << tools::translate( "ADN_Schedule_Table", "To" )
                      << tools::translate( "ADN_Schedule_Table", "Activity" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );

    for( unsigned int i = 0; i < unsigned int( eNbrDays ); ++i )
        days_ << ADN_Tr::ConvertFromDays( static_cast< E_Days>( i ), ENT_Tr_ABC::eToTr ).c_str();

    delegate_.AddComboBoxOnColumn( 0, days_ );
    delegate_.AddTimeEditOnColumn( 1 );
    delegate_.AddTimeEditOnColumn( 2 );
    delegate_.AddComboPtrInVectorOnColumn( 3 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Schedule_Table destructor
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
ADN_Schedule_Table::~ADN_Schedule_Table()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Schedule_Table::AddRow
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
void ADN_Schedule_Table::AddRow( int row, void* data )
{
    ADN_Inhabitants_Data::EventInfos* pEventInfos = static_cast< ADN_Inhabitants_Data::EventInfos* >( data );
    if( !pEventInfos )
        return;

    AddItem( row, 0, data, &pEventInfos->day_, days_, Qt::ItemIsEditable );
    AddItem( row, 1, data, &pEventInfos->from_, ADN_StandardItem::eString, Qt::ItemIsEditable );
    AddItem( row, 2, data, &pEventInfos->to_, ADN_StandardItem::eString, Qt::ItemIsEditable );
    AddItem( row, 3, data, &pEventInfos->ptrAccommodation_, Qt::ItemIsEditable );
}

// -----------------------------------------------------------------------------
// Name: ADN_Schedule_Table::OnContextMenu
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
void ADN_Schedule_Table::OnContextMenu( const QPoint& pt )
{
    ADN_Gui_Tools::GenerateStandardContextMenu< ADN_Inhabitants_Data::EventInfos >( *this, pt );
}

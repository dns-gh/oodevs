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
    Q3PopupMenu popup( this );

    popup.insertItem( tools::translate( "ADN_Schedule_Table", "Add"), 0 );
    if( GetSelectedData() != 0 )
        popup.insertItem( tools::translate( "ADN_Schedule_Table", "Remove"), 1 );

    int result = popup.exec( pt );
    if( result == 1 )
        RemoveCurrentElement();
    else if( result == 0 )
        AddNewElement();
}

// -----------------------------------------------------------------------------
// Name: ADN_Schedule_Table::AddNewElement
// Created: ABR 2012-10-25
// -----------------------------------------------------------------------------
void ADN_Schedule_Table::AddNewElement()
{
    ADN_Inhabitants_Data::EventInfos* newElement = new ADN_Inhabitants_Data::EventInfos();
    ADN_Connector_Vector_ABC* connector = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    connector->AddItem( newElement );
    connector->AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Schedule_Table::RemoveCurrentElement
// Created: ABR 2012-10-25
// -----------------------------------------------------------------------------
void ADN_Schedule_Table::RemoveCurrentElement()
{
    ADN_Inhabitants_Data::EventInfos* param = static_cast< ADN_Inhabitants_Data::EventInfos* >( GetSelectedData() );
    if( param )
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( param );
}

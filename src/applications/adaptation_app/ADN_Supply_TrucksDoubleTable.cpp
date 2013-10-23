// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Supply_TrucksDoubleTable.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:56 $
// $Revision: 4 $
// $Workfile: ADN_Supply_TrucksDoubleTable.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Supply_TrucksDoubleTable.h"
#include "moc_ADN_Supply_TrucksDoubleTable.cpp"
#include "ADN_Supply_Data.h"

typedef ADN_Supply_Data::ConvoyInfo< ADN_Type_Double > ConvoyInfo;

// -----------------------------------------------------------------------------
// Name: ADN_Supply_TrucksDoubleTable constructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Supply_TrucksDoubleTable::ADN_Supply_TrucksDoubleTable( const QString& objectName, ADN_Ref_ABC& vector, QWidget* pParent /* = 0 */ )
: ADN_Table( objectName, vector, pParent )
{
    setSortingEnabled( true );
    this->setMaximumHeight( 300 );
    dataModel_.setColumnCount( 2 );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Trucks qty" )
                      << tr( "Max speed modifier" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );
    delegate_.AddSpinBoxOnColumn( 0, 1, INT_MAX );
    delegate_.AddDoubleSpinBoxOnColumn( 1 );
    static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ ).AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Supply_TrucksDoubleTable destructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Supply_TrucksDoubleTable::~ADN_Supply_TrucksDoubleTable()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Supply_TrucksDoubleTable::OnContextMenu
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
void ADN_Supply_TrucksDoubleTable::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu menu( this );

    menu.insertItem( tr( "New" ), 0 );
    menu.insertItem( tr( "Delete" ), 1 );
    menu.setItemEnabled( 1, GetSelectedData() != 0 );

    int nMenuResult = menu.exec( pt );

    if( nMenuResult == -1 )
        return;
    else if( nMenuResult == 1 )
    {
        // Delete the current element.
        ConvoyInfo* pCurrentPart = (ConvoyInfo*)GetSelectedData();
        if( pCurrentPart != 0 )
            static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ ).RemItem( pCurrentPart );
    }
    else
    {
        assert( nMenuResult == 0 );
        // Create a new element
        ConvoyInfo* pNewInfo = new ConvoyInfo( 1. );
        ADN_Connector_Vector_ABC& pCTable = static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ );
        pCTable.AddItem( pNewInfo );
        pCTable.AddItem( 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Supply_TrucksDoubleTable::AddRow
// Created: NPT 2012-11-05
// -----------------------------------------------------------------------------
void ADN_Supply_TrucksDoubleTable::AddRow( int row, void* data )
{
    ConvoyInfo* info = static_cast< ConvoyInfo* >( data );
    if (!info )
        return;
    AddItem( row, 0, data, &info->nNbrTrucks_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
    AddItem( row, 1, data, &info->value_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}

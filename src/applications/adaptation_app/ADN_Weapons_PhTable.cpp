// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-07 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Weapons_PhTable.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:50 $
// $Revision: 6 $
// $Workfile: ADN_Weapons_PhTable.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Weapons_PhTable.h"
#include "ADN_Weapons_Data_PhInfos.h"

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_PhTable constructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Weapons_PhTable::ADN_Weapons_PhTable( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /*= 0*/ )
    : ADN_Table( objectName, connector, pParent )
{
    // Selection and sorting.
    setShowGrid( false );
    setMaximumHeight( 300 );
    dataModel_.setColumnCount( 2 );
    QStringList horizontalHeaders;
    horizontalHeaders << tools::translate( "ADN_Weapons_PhTable", "Distance" ) << tools::translate( "ADN_Weapons_PhTable", "Percentage (PH)" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    verticalHeader()->setVisible( false );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    delegate_.AddColorOnColumn( 1, 0., 100. );
    delegate_.AddSpinBoxOnColumn( 0, 0, std::numeric_limits< int >::max() );
    delegate_.AddDoubleSpinBoxOnColumn( 1, 0, 100, 1, 5 );
    proxyModel_.setDynamicSortFilter( true );
    proxyModel_.sort( 0, Qt::AscendingOrder );
    setSortingEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_PhTable destructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Weapons_PhTable::~ADN_Weapons_PhTable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_PhTable::OnContextMenu
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
void ADN_Weapons_PhTable::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu menu( this );
    menu.insertItem( tr( "New Ph"), 0 );
    menu.insertItem( tr( "Delete Ph"), 1 );
    menu.setItemEnabled( 1, GetSelectedData() != 0 );

    int nMenu = menu.exec( pt );

    switch ( nMenu )
    {
    case 0:
        {
            // create new munition & add it to the list
            CreateNewElement();
            break;
        }
    case 1:
        {
            DeleteCurrentElement();
            break;
        }
    default:
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_PhTable::AddRow
// Created: JSR 2012-10-31
// -----------------------------------------------------------------------------
void ADN_Weapons_PhTable::AddRow( int row, void* data )
{
    ADN_Weapons_Data_PhInfos* infos = static_cast< ADN_Weapons_Data_PhInfos* >( data );
    if( !infos )
        return;
    QStandardItem* item = AddItem( row, 0, data, &infos->nDistance_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
    item->setData( infos->nDistance_.GetData(), Qt::UserRole ); // sort
    AddItem( row, 1, data, &infos->rPerc_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_PhTable::dataChanged
// Created: JSR 2012-10-31
// -----------------------------------------------------------------------------
void ADN_Weapons_PhTable::dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight )
{
    ADN_Table::dataChanged( topLeft, bottomRight );
    if( topLeft == bottomRight )
    {
        if( ADN_Weapons_Data_PhInfos* pCurPh = static_cast< ADN_Weapons_Data_PhInfos* >( GetSelectedData() ) )
        {
            const QModelIndex index = topLeft.model()->index( topLeft.row(), 0 );
            const_cast< QAbstractItemModel* >( topLeft.model() )->setData( index, pCurPh->nDistance_.GetData(), Qt::UserRole ); // sort
            pCurPh->ApplyPhModifiers();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_PhTable::CreateNewElement
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
void ADN_Weapons_PhTable::CreateNewElement()
{
    ADN_Weapons_Data_PhInfos* pNewInfo = new ADN_Weapons_Data_PhInfos();
    ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    pCTable->AddItem( pNewInfo );
    pCTable->AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_PhTable::DeleteCurrentElement
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
void ADN_Weapons_PhTable::DeleteCurrentElement()
{
    if( ADN_Weapons_Data_PhInfos* pCurPh = static_cast< ADN_Weapons_Data_PhInfos* >( GetSelectedData() ) )
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurPh );
}

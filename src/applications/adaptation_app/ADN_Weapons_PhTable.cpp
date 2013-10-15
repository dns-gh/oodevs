// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Weapons_PhTable.h"
#include "ADN_Weapons_Data_PhInfos.h"
#include "ADN_Gui_Tools.h"

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_PhTable constructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Weapons_PhTable::ADN_Weapons_PhTable( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
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
    proxyModel_->setDynamicSortFilter( true );
    Sort();
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
    ADN_Gui_Tools::GenerateStandardContextMenu< ADN_Weapons_Data_PhInfos >( *this, pt );
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
    AddItem( row, 0, data, &infos->nDistance_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
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
        if( ADN_Weapons_Data_PhInfos* pCurPh = static_cast< ADN_Weapons_Data_PhInfos* >( GetSelectedData() ) )
            pCurPh->ApplyPhModifiers();
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipments_ConsumptionsTable.h"
#include "ADN_Equipments_Data.h"
#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_ConsumptionsTable constructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Equipments_ConsumptionsTable::ADN_Equipments_ConsumptionsTable( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    dataModel_.setColumnCount( eNbrConsumptionType );
    QStringList horizontalHeaders;
    horizontalHeaders << "Resource Name";
    for( int i = 0; i < eNbrConsumptionType; ++i )
    {
        horizontalHeaders << ADN_Tr::ConvertFromConsumptionType( static_cast< E_ConsumptionType >( i ), ENT_Tr::eToTr ).c_str();
        delegate_.AddDoubleSpinBoxOnColumn( i + 1, 0, INT_MAX );
    }
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    horizontalHeader()->setResizeMode( 0, QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeMode( 1, QHeaderView::ResizeToContents );
    verticalHeader()->setVisible( false );

    proxyModel_.setDynamicSortFilter( true );
    proxyModel_.sort( 0, Qt::DescendingOrder );
    setSortingEnabled( false );

    hideColumn( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_ConsumptionsTable destructor
// Created: APE 2005-01-25
// -----------------------------------------------------------------------------
ADN_Equipments_ConsumptionsTable::~ADN_Equipments_ConsumptionsTable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_ConsumptionsTable::AddRow
// Created: MMC 2012-11-06
// -----------------------------------------------------------------------------
void ADN_Equipments_ConsumptionsTable::AddRow( int /* row */, void* data )
{
    ADN_Equipments_Data::ConsumptionItem* pConsumption = static_cast< ADN_Equipments_Data::ConsumptionItem* >( data );
    if( !pConsumption )
        return;
    ADN_Equipments_Data::CategoryInfos* catInfos = pConsumption->GetCrossedElement();
    if( !catInfos || !catInfos->GetCrossedElement() )
        return;

    QString resourceName = catInfos->GetCrossedElement()->strName_.GetData().c_str();
    QStandardItem* resourceItem = 0;
    for( int i = 0; i < numRows(); ++i )
    {
        QStandardItem* item = GetItem( i, 0 );
        if( item && item->text() == resourceName )
        {
            resourceItem = item;
            break;
        }
    }
    if( !resourceItem )
        resourceItem = AddItem( numRows(), 0, data, &catInfos->GetCrossedElement()->strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );

    AddItem( resourceItem->row(), 1 + pConsumption->nConsumptionType_, data, &pConsumption->nQuantityUsedPerHour_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}

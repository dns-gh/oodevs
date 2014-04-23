// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipments_DisasterImpactTable.h"
#include "ADN_Equipments_Data.h"
#include "ADN_Gui_Tools.h"

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_DisasterImpactTable constructor
// Created: JSR 2014-04-22
// -----------------------------------------------------------------------------
ADN_Equipments_DisasterImpactTable::ADN_Equipments_DisasterImpactTable( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    setShowGrid( false );
    setMinimumHeight( 200 );
    dataModel_.setColumnCount( 2 );
    QStringList horizontalHeaders;
    horizontalHeaders << tools::translate( "ADN_Equipments_DisasterImpactTable", "Thresholds" ) << tools::translate( "ADN_Equipments_DisasterImpactTable", "Modifiers" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    verticalHeader()->setVisible( false );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    delegate_.AddColorOnColumn( 1, 0., 1. );
    delegate_.AddDoubleSpinBoxOnColumn( 0, 0, std::numeric_limits< double >::max() );
    delegate_.AddDoubleSpinBoxOnColumn( 1, 0, 1, 0.05, 2 );
    proxyModel_->setDynamicSortFilter( true );
    Sort();
    setSortingEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_DisasterImpactTable destructor
// Created: JSR 2014-04-22
// -----------------------------------------------------------------------------
ADN_Equipments_DisasterImpactTable::~ADN_Equipments_DisasterImpactTable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_DisasterImpactTable::OnContextMenu
// Created: JSR 2014-04-22
// -----------------------------------------------------------------------------
void ADN_Equipments_DisasterImpactTable::OnContextMenu( const QPoint& pt )
{
    ADN_Gui_Tools::GenerateStandardContextMenu< ADN_Equipments_Data::DisasterImpactInfos >( *this, pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_DisasterImpactTable::AddRow
// Created: JSR 2014-04-22
// -----------------------------------------------------------------------------
void ADN_Equipments_DisasterImpactTable::AddRow( int row, void* data )
{
    ADN_Equipments_Data::DisasterImpactInfos* infos = static_cast< ADN_Equipments_Data::DisasterImpactInfos* >( data );
    if( !infos )
        return;
    AddItem( row, 0, data, &infos->threshold_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
    AddItem( row, 1, data, &infos->modifier_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}

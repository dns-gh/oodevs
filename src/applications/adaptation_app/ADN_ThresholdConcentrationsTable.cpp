// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ThresholdConcentrationsTable.h"
#include "ADN_Disasters_Data.h"
#include "ADN_Gui_Tools.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: ADN_ThresholdConcentrationsTable constructor
// Created: LGY 2012-11-13
// -----------------------------------------------------------------------------
ADN_ThresholdConcentrationsTable::ADN_ThresholdConcentrationsTable( const QString& objectName, ADN_Connector_ABC*& connector,
                                                                    QWidget* pParent )
    : ADN_Table( objectName, connector, pParent )
{
    dataModel_.setColumnCount( 3 );
    setShowGrid( false );
    QStringList horizontalHeaders;
    horizontalHeaders << tools::translate( "ADN_ThresholdConcentrationsTable", "Name" )
                      << tools::translate( "ADN_ThresholdConcentrationsTable", "Color" )
                      << tools::translate( "ADN_ThresholdConcentrationsTable", "Threshold" )
                      << tools::translate( "ADN_ThresholdConcentrationsTable", "Display" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );

    delegate_.AddLineEditOnColumn( 0 );
    delegate_.AddColorEditOnColumn( 1 );
    delegate_.AddDoubleSpinBoxOnColumn( 2, 0., std::numeric_limits< double >::max(), 1, 5 );
    delegate_.AddCheckBoxOnColumn( 3 );

    proxyModel_.setDynamicSortFilter( true );
    proxyModel_.sort( 2, Qt::AscendingOrder );
    setSortingEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_ThresholdConcentrationsTable destructor
// Created: LGY 2012-11-13
// -----------------------------------------------------------------------------
ADN_ThresholdConcentrationsTable::~ADN_ThresholdConcentrationsTable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ThresholdConcentrationsTable::AddRow
// Created: LGY 2012-11-13
// -----------------------------------------------------------------------------
void ADN_ThresholdConcentrationsTable::AddRow( int row, void* data )
{
    ADN_Disasters_Data::ConcentrationThresholdInfos* pEventInfos = static_cast< ADN_Disasters_Data::ConcentrationThresholdInfos* >( data );
    if( !pEventInfos )
        return;

    AddItem( row, 0, data, &pEventInfos->name_, ADN_StandardItem::eString, Qt::ItemIsEditable );
    AddItem( row, 1, data, &pEventInfos->color_, ADN_StandardItem::eColor, Qt::ItemIsEditable );
    AddItem( row, 2, data, &pEventInfos->threshold_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
    AddItem( row, 3, data, &pEventInfos->display_, ADN_StandardItem::eBool, Qt::ItemIsUserCheckable );
}

// -----------------------------------------------------------------------------
// Name: ADN_ThresholdConcentrationsTable::OnContextMenu
// Created: LGY 2012-11-16
// -----------------------------------------------------------------------------
void ADN_ThresholdConcentrationsTable::OnContextMenu( const QPoint& pt )
{
    ADN_Gui_Tools::GenerateStandardContextMenu< ADN_Disasters_Data::ConcentrationThresholdInfos >( *this, pt );
}

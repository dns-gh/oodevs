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
                      << tools::translate( "ADN_ThresholdConcentrationsTable", "Threshold" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );

    delegate_.AddLineEditOnColumn( 0 );
    delegate_.AddColorEditOnColumn( 1 );
    delegate_.AddDoubleSpinBoxOnColumn( 2, 0., std::numeric_limits< double >::max() );
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
}

// -----------------------------------------------------------------------------
// Name: ADN_ThresholdConcentrationsTable::OnContextMenu
// Created: LGY 2012-11-16
// -----------------------------------------------------------------------------
void ADN_ThresholdConcentrationsTable::OnContextMenu( const QPoint& point )
{
    Q3PopupMenu popup( this );

    popup.insertItem( tools::translate( "ADN_ThresholdConcentrationsTable", "Add"), 0 );
    if( GetSelectedData() != 0 )
        popup.insertItem( tools::translate( "ADN_ThresholdConcentrationsTable", "Remove"), 1 );

    int result = popup.exec( point );
    if( result == 1 )
        RemoveCurrentElement();
    else if( result == 0 )
        AddNewElement();
}

// -----------------------------------------------------------------------------
// Name: ADN_ThresholdConcentrationsTable::AddNewElement
// Created: LGY 2012-11-16
// -----------------------------------------------------------------------------
void ADN_ThresholdConcentrationsTable::AddNewElement()
{
    ADN_Disasters_Data::ConcentrationThresholdInfos* newElement = new ADN_Disasters_Data::ConcentrationThresholdInfos();
    ADN_Connector_Vector_ABC* connector = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    connector->AddItem( newElement );
    connector->AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_ThresholdConcentrationsTable::RemoveCurrentElement
// Created: LGY 2012-11-16
// -----------------------------------------------------------------------------
void ADN_ThresholdConcentrationsTable::RemoveCurrentElement()
{
    ADN_Disasters_Data::ConcentrationThresholdInfos* param = static_cast< ADN_Disasters_Data::ConcentrationThresholdInfos* >( GetSelectedData() );
    if( param )
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( param );
}

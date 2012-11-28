// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ThresholdAttritionsTable.h"
#include "ADN_Disasters_Data.h"

// -----------------------------------------------------------------------------
// Name: ADN_ThresholdAttritionsTable constructor
// Created: LGY 2012-11-13
// -----------------------------------------------------------------------------
ADN_ThresholdAttritionsTable::ADN_ThresholdAttritionsTable( const QString& objectName, ADN_Connector_ABC*& connector,
                                                            QWidget* pParent )
    : ADN_Table( objectName, connector, pParent )
{
    dataModel_.setColumnCount( 8 );
    setShowGrid( false );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Name" )
                      << tr( "Threshold" )
                      << tr( "U1 (%)" )
                      << tr( "U2 (%)" )
                      << tr( "U3 (%)" )
                      << tr( "U4 (%)" )
                      << tr( "Dead (%)" )
                      << tr( "Contamination" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );

    delegate_.AddLineEditOnColumn( 0 );
    delegate_.AddDoubleSpinBoxOnColumn( 1, 0., std::numeric_limits< double >::max() );
    for( std::size_t i = 2; i<= 6; ++i )
        delegate_.AddSpinBoxOnColumn( static_cast< int >( i ) );
    delegate_.AddCheckBoxOnColumn( 7 );
    proxyModel_.setDynamicSortFilter( true );
    proxyModel_.sort( 1, Qt::AscendingOrder );
    setSortingEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_ThresholdAttritionsTable destructor
// Created: LGY 2012-11-13
// -----------------------------------------------------------------------------
ADN_ThresholdAttritionsTable::~ADN_ThresholdAttritionsTable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ThresholdAttritionsTable::AddRow
// Created: LGY 2012-11-13
// -----------------------------------------------------------------------------
void ADN_ThresholdAttritionsTable::AddRow( int row, void* data )
{
    ADN_Disasters_Data::AttritionThresholdInfos* pEventInfos = static_cast< ADN_Disasters_Data::AttritionThresholdInfos* >( data );
    if( !pEventInfos )
        return;

    AddItem( row, 0, data, &pEventInfos->name_, ADN_StandardItem::eString, Qt::ItemIsEditable );
    AddItem( row, 1, data, &pEventInfos->threshold_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
    AddItem( row, 2, data, &pEventInfos->u1_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
    AddItem( row, 3, data, &pEventInfos->u2_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
    AddItem( row, 4, data, &pEventInfos->u3_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
    AddItem( row, 5, data, &pEventInfos->ue_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
    AddItem( row, 6, data, &pEventInfos->dead_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
    AddItem( row, 7, data, &pEventInfos->contamination_, ADN_StandardItem::eBool, Qt::ItemIsUserCheckable );
}

// -----------------------------------------------------------------------------
// Name: ADN_ThresholdAttritionsTable::OnContextMenu
// Created: LGY 2012-11-16
// -----------------------------------------------------------------------------
void ADN_ThresholdAttritionsTable::OnContextMenu( const QPoint& point )
{
    Q3PopupMenu popup( this );

    popup.insertItem( tools::translate( "ADN_ThresholdAttritionsTable", "Add"), 0 );
    if( GetSelectedData() != 0 )
        popup.insertItem( tools::translate( "ADN_ThresholdAttritionsTable", "Remove"), 1 );

    int result = popup.exec( point );
    if( result == 1 )
        RemoveCurrentElement();
    else if( result == 0 )
        AddNewElement();
}

// -----------------------------------------------------------------------------
// Name: ADN_ThresholdAttritionsTable::AddNewElement
// Created: LGY 2012-11-16
// -----------------------------------------------------------------------------
void ADN_ThresholdAttritionsTable::AddNewElement()
{
    ADN_Disasters_Data::AttritionThresholdInfos* newElement = new ADN_Disasters_Data::AttritionThresholdInfos();
    ADN_Connector_Vector_ABC* connector = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    connector->AddItem( newElement );
    connector->AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_ThresholdAttritionsTable::RemoveCurrentElement
// Created: LGY 2012-11-16
// -----------------------------------------------------------------------------
void ADN_ThresholdAttritionsTable::RemoveCurrentElement()
{
    ADN_Disasters_Data::AttritionThresholdInfos* param = static_cast< ADN_Disasters_Data::AttritionThresholdInfos* >( GetSelectedData() );
    if( param )
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( param );
}

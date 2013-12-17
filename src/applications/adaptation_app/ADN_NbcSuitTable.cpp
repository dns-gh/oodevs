// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_NbcSuitTable.h"
#include "ADN_Disasters_Data.h"
#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_NbcSuitTable constructor
// Created: LGY 2012-11-23
// -----------------------------------------------------------------------------
ADN_NbcSuitTable::ADN_NbcSuitTable( const QString& objectName, ADN_Connector_ABC*& connector,
                                                            QWidget* pParent )
    : ADN_Table( objectName, connector, pParent )
{
    dataModel_.setColumnCount( 2 );
    setShowGrid( false );
    QStringList horizontalHeaders;
    horizontalHeaders << tools::translate( "ADN_NbcSuitTable", "Suit" )
                      << tools::translate( "ADN_NbcSuitTable", "Ratio" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );
    delegate_.AddDoubleSpinBoxOnColumn( 1, 0., std::numeric_limits< double >::max(), 1, 5 );
}

// -----------------------------------------------------------------------------
// Name: ADN_NbcSuitTable destructor
// Created: LGY 2012-11-23
// -----------------------------------------------------------------------------
ADN_NbcSuitTable::~ADN_NbcSuitTable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_NbcSuitTable::AddRow
// Created: LGY 2012-11-23
// -----------------------------------------------------------------------------
void ADN_NbcSuitTable::AddRow( int row, void* data )
{
    ADN_Disasters_Data::NbcSuitRatioInfos* pEventInfos = static_cast< ADN_Disasters_Data::NbcSuitRatioInfos* >( data );
    if( !pEventInfos )
        return;
    AddItem( row, 0, data, ENT_Tr::ConvertFromAgentNbcSuit( pEventInfos->eType_, ENT_Tr::eToTr ).c_str(), Qt::ItemIsSelectable);
    AddItem( row, 1, data, &pEventInfos->rCoeff_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
    setRowHidden( row, pEventInfos->eType_ == eAgentNone );
}

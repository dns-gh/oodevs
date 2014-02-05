// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ExtinguisherAgentInfos_Table.h"
#include "ADN_ExtinguisherAgentInfos.h"

//-----------------------------------------------------------------------------
// Name: ADN_ExtinguisherAgentInfos_Table constructor
// Created: BCI 2010-12-03
//-----------------------------------------------------------------------------
ADN_ExtinguisherAgentInfos_Table::ADN_ExtinguisherAgentInfos_Table( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    setShowGrid( false );
    dataModel_.setColumnCount( 2 );
    verticalHeader()->setVisible( false );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    QStringList horizontalHeaders;
    horizontalHeaders << tools::translate( "ADN_ExtinguisherAgentInfos_Table", "Agent" )
                      << tools::translate( "ADN_ExtinguisherAgentInfos_Table", "Heat decrease rate" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    delegate_.AddSpinBoxOnColumn( 1, std::numeric_limits< int >::min(), std::numeric_limits< int >::max() );
}

//-----------------------------------------------------------------------------
// Name: ADN_ExtinguisherAgentInfos_Table destructor
// Created: BCI 2010-12-03
//-----------------------------------------------------------------------------
ADN_ExtinguisherAgentInfos_Table::~ADN_ExtinguisherAgentInfos_Table()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ExtinguisherAgentInfos_Table::AddRow
// Created: JSR 2012-11-05
// -----------------------------------------------------------------------------
void ADN_ExtinguisherAgentInfos_Table::AddRow( int row, void* data )
{
    ADN_ExtinguisherAgentInfos* pInfos = static_cast< ADN_ExtinguisherAgentInfos* >( data );
    if( !pInfos )
        return;
    AddItem( row, 0, data, &pInfos->strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );
    AddItem( row, 1, data, &pInfos->heatDecreaseRate_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
}

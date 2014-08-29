// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_AiEngine_UrbanSpeeds_Table.h"
#include "ADN_AiEngine_Data.h"
#include "ADN_Connector_Table_ABC.h"
#include "moc_ADN_AiEngine_UrbanSpeeds_Table.cpp"

// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_UrbanSpeeds_Table constructor
// Created: JSR 2014-07-09
// -----------------------------------------------------------------------------
ADN_AiEngine_UrbanSpeeds_Table::ADN_AiEngine_UrbanSpeeds_Table(  const QString& objectName, ADN_Ref_ABC& vector, QWidget* parent /*= 0*/ )
    : ADN_Table( objectName, vector, parent )
{
    setShowGrid( false );
    verticalHeader()->hide();
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );

    dataModel_.setHorizontalHeaderLabels( QStringList() << tr( "Material" ) << tr( "Recon speed" ) << tr( "Search speed" ) );
        
    delegate_.AddDoubleSpinBoxOnColumn( 1, 0.01, std::numeric_limits< double >::max() );
    delegate_.AddDoubleSpinBoxOnColumn( 2, 0.01, std::numeric_limits< double >::max() );

    static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ ).AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_UrbanSpeeds_Table destructor
// Created: JSR 2014-07-09
// -----------------------------------------------------------------------------
ADN_AiEngine_UrbanSpeeds_Table::~ADN_AiEngine_UrbanSpeeds_Table()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_UrbanSpeeds_Table::AddRow
// Created: LDC 2014-08-26
// -----------------------------------------------------------------------------
void ADN_AiEngine_UrbanSpeeds_Table::AddRow( int row, void* data )
{
    auto infos = static_cast< ADN_AiEngine_Data::UrbanSpeedsInfos* >( data );
    if( !data )
        return;

    AddItem( row, 0, data, &infos->ptrMaterial_.GetData()->strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );
    AddItem( row, 1, data, &infos->reconSpeed_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
    AddItem( row, 2, data, &infos->searchSpeed_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}
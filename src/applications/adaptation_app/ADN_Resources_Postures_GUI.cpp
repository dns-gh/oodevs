// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Resources_Postures_GUI.h"
#include "ADN_CommonGfx.h"
#include "ADN_Resources_Data.h"
#include "ENT/ENT_Tr.h"

typedef ADN_Resources_Data::ModificatorPostureInfos ModificatorPostureInfos;

//-----------------------------------------------------------------------------
// Name: ADN_Resources_Postures_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Resources_Postures_GUI::ADN_Resources_Postures_GUI( const QString& objectName, const QString& strColCaption, ADN_Connector_ABC*& connector,  QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    dataModel_.setColumnCount( 2 );
    QStringList horizontalHeaders;
    horizontalHeaders << strColCaption
                      << tools::translate( "ADN_Resources_Postures_GUI", "PH factor" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );
    delegate_.AddDoubleSpinBoxOnColumn( 1, 0, 1, 0.01, 2 );
    setMinimumHeight( int( ( eNbrUnitPosture + 1.2 ) * 20 ) );
    setShowGrid( false );
}

//-----------------------------------------------------------------------------
// Name: ADN_Resources_Postures_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Resources_Postures_GUI::~ADN_Resources_Postures_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Postures_GUI::AddRow
// Created: MMC 2012-11-06
// -----------------------------------------------------------------------------
void ADN_Resources_Postures_GUI::AddRow( int row, void* data )
{
    ModificatorPostureInfos* pPostureInfo = static_cast< ModificatorPostureInfos* >( data );
    if( !pPostureInfo )
        return;
    AddItem( row, 0, data, QString::fromStdString( ENT_Tr::ConvertFromUnitPosture( pPostureInfo->eType_, ENT_Tr::eToTr ) ), Qt::ItemIsSelectable );
    AddItem( row, 1, data, &pPostureInfo->rCoeff_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}

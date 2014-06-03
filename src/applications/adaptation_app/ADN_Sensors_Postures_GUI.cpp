// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Sensors_Postures_GUI.h"
#include "ADN_Sensors_Data.h"
#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Postures_GUI constructor
// Created: ABR 2012-01-24
// -----------------------------------------------------------------------------
ADN_Sensors_Postures_GUI::ADN_Sensors_Postures_GUI( const QString& strColCaption, const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Sensors_ModificatorTable_ABC( strColCaption + tools::translate( "ADN_Sensors_Postures_GUI", "" ), tools::translate( "ADN_Sensors_Postures_GUI", "Modifiers" ), objectName, connector, pParent ) // hack : first translation not seen by Qt (unknown reason)
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Postures_GUI destructor
// Created: ABR 2012-01-24
// -----------------------------------------------------------------------------
ADN_Sensors_Postures_GUI::~ADN_Sensors_Postures_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Postures_GUI::InternalEmit
// Created: ABR 2012-01-24
// -----------------------------------------------------------------------------
void ADN_Sensors_Postures_GUI::InternalEmit( const QModelIndex& current )
{
    if( ADN_Sensors_Modificators::PostureInfos* data = static_cast< ADN_Sensors_Modificators::PostureInfos* >( GetDataFromIndex( current )  ) )
        emit ContentChanged(  ENT_Tr::ConvertFromUnitPosture( data->eType_, ENT_Tr::eToTr ), data->rCoeff_.GetData() );
    else
        emit ContentChanged( "", 1. );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Postures_GUI::AddRow
// Created: JSR 2012-11-05
// -----------------------------------------------------------------------------
void ADN_Sensors_Postures_GUI::AddRow( int row, void* data )
{
    ADN_Sensors_Modificators::PostureInfos* pInfos = static_cast< ADN_Sensors_Modificators::PostureInfos* >( data );
    if( !pInfos )
        return;

    AddItem( row, 0, data, ENT_Tr::ConvertFromUnitPosture( pInfos->eType_, ENT_Tr::eToTr ).c_str(), Qt::ItemIsSelectable );
    AddItem( row, 1, data, &pInfos->rCoeff_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}

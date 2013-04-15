// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
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
void ADN_Sensors_Postures_GUI::InternalEmit()
{
    if( ADN_Sensors_Data::ModificatorPostureInfos* data = static_cast< ADN_Sensors_Data::ModificatorPostureInfos* >( GetSelectedData() ) )
        emit ContentChanged(  ENT_Tr::ConvertFromUnitPosture( data->eType_, ENT_Tr_ABC::eToTr ), data->rCoeff_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Postures_GUI::AddRow
// Created: JSR 2012-11-05
// -----------------------------------------------------------------------------
void ADN_Sensors_Postures_GUI::AddRow( int row, void* data )
{
    ADN_Sensors_Data::ModificatorPostureInfos* pInfos = static_cast< ADN_Sensors_Data::ModificatorPostureInfos* >( data );
    if( !pInfos )
        return;

    AddItem( row, 0, data, ENT_Tr::ConvertFromUnitPosture( pInfos->eType_, ENT_Tr_ABC::eToTr ).c_str(), Qt::ItemIsSelectable );
    AddItem( row, 1, data, &pInfos->rCoeff_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}

//*****************************************************************************
//
// $Created: JDY 03-07-08 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Sensors_Meteos_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:51 $
// $Revision: 10 $
// $Workfile: ADN_Sensors_Meteos_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Sensors_Meteos_GUI.h"
#include "ADN_Sensors_Modificators.h"
#include "ADN_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Meteos_GUI constructor
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
ADN_Sensors_Meteos_GUI::ADN_Sensors_Meteos_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Sensors_ModificatorTable_ABC( tools::translate( "ADN_Sensors_Meteos_GUI", "Meteos"), tools::translate( "ADN_Sensors_Meteos_GUI", "Modifiers"), objectName, connector, pParent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Meteos_GUI destructor
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
ADN_Sensors_Meteos_GUI::~ADN_Sensors_Meteos_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Meteos_GUI::InternalEmit
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
void ADN_Sensors_Meteos_GUI::InternalEmit()
{
    if( ADN_Sensors_Modificators::MeteoInfos* data = static_cast< ADN_Sensors_Modificators::MeteoInfos* >( GetSelectedData() ) )
        emit ContentChanged( ADN_Tr::ConvertFromSensorWeatherModifiers( data->eType_, ENT_Tr::eToTr ), data->rCoeff_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Meteos_GUI::AddRow
// Created: JSR 2012-11-05
// -----------------------------------------------------------------------------
void ADN_Sensors_Meteos_GUI::AddRow( int row, void* data )
{
    ADN_Sensors_Modificators::MeteoInfos* pInfos = static_cast< ADN_Sensors_Modificators::MeteoInfos* >( data );
    if( !pInfos )
        return;

    AddItem( row, 0, data, ADN_Tr::ConvertFromSensorWeatherModifiers( pInfos->eType_, ENT_Tr::eToTr ).c_str(), Qt::ItemIsSelectable );
    AddItem( row, 1, data, &pInfos->rCoeff_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}

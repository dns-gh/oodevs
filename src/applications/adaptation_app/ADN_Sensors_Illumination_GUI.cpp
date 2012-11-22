//*****************************************************************************
//
// $Created: JDY 03-07-08 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Sensors_Illumination_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:51 $
// $Revision: 9 $
// $Workfile: ADN_Sensors_Illumination_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Sensors_Illumination_GUI.h"
#include "ADN_Sensors_Data.h"
#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Illumination_GUI constructor
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
ADN_Sensors_Illumination_GUI::ADN_Sensors_Illumination_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /*= 0*/ )
    : ADN_Sensors_ModificatorTable_ABC( tools::translate( "ADN_Sensors_Illumination_GUI", "Illumination" ), tools::translate( "ADN_Sensors_Illumination_GUI", "Modifiers" ), objectName, connector, pParent )
{
    setMinimumHeight( 250 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Illumination_GUI destructor
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
ADN_Sensors_Illumination_GUI::~ADN_Sensors_Illumination_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Illumination_GUI::InternalEmit
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
void ADN_Sensors_Illumination_GUI::InternalEmit()
{
    if( ADN_Sensors_Data::ModificatorIlluminationInfos* data = static_cast< ADN_Sensors_Data::ModificatorIlluminationInfos* >( GetSelectedData() ) )
        emit ContentChanged( ENT_Tr::ConvertFromLightingType( data->eType_, ENT_Tr_ABC::eToTr ), data->rCoeff_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Illumination_GUI::AddRow
// Created: JSR 2012-11-05
// -----------------------------------------------------------------------------
void ADN_Sensors_Illumination_GUI::AddRow( int row, void* data )
{
    ADN_Sensors_Data::ModificatorIlluminationInfos* pInfos = static_cast< ADN_Sensors_Data::ModificatorIlluminationInfos* >( data );
    if( !pInfos )
        return;

    AddItem( row, 0, data, ENT_Tr::ConvertFromLightingType( pInfos->eType_, ENT_Tr_ABC::eToTr ).c_str(), Qt::ItemIsSelectable );
    AddItem( row, 1, data, &pInfos->rCoeff_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}

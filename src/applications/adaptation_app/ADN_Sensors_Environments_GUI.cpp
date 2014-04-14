//*****************************************************************************
//
// $Created: JDY 03-07-08 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Sensors_Environments_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:50 $
// $Revision: 9 $
// $Workfile: ADN_Sensors_Environments_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Sensors_Environments_GUI.h"
#include "ADN_Sensors_Modificators.h"
#include "ADN_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Environments_GUI constructor
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
ADN_Sensors_Environments_GUI::ADN_Sensors_Environments_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Sensors_ModificatorTable_ABC( tools::translate( "ADN_Sensors_Environments_GUI", "Environments" ), tools::translate( "ADN_Sensors_Environments_GUI", "Modifiers" ), objectName, connector, pParent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Environments_GUI destructor
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
ADN_Sensors_Environments_GUI::~ADN_Sensors_Environments_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Environments_GUI::InternalEmit
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
void ADN_Sensors_Environments_GUI::InternalEmit()
{
    if( ADN_Sensors_Modificators::EnvironmentInfos* data = static_cast< ADN_Sensors_Modificators::EnvironmentInfos* >( GetSelectedData() ) )
        emit ContentChanged( ADN_Tr::ConvertFromVisionObject( data->eType_, ENT_Tr::eToTr ), data->rCoeff_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Environments_GUI::AddRow
// Created: JSR 2012-11-05
// -----------------------------------------------------------------------------
void ADN_Sensors_Environments_GUI::AddRow( int row, void* data )
{
    ADN_Sensors_Modificators::EnvironmentInfos* pInfos = static_cast< ADN_Sensors_Modificators::EnvironmentInfos* >( data );
    if( !pInfos )
        return;
    Qt::ItemFlags flags = pInfos->eType_ == eVisionEmpty ? 0 : Qt::ItemIsEditable;
    AddItem( row, 0, data, ADN_Tr::ConvertFromVisionObject( pInfos->eType_, ENT_Tr::eToTr ).c_str(), Qt::ItemIsSelectable );
    AddItem( row, 1, data, &pInfos->rCoeff_, ADN_StandardItem::eDouble, flags );
}

//*****************************************************************************
//
// $Created: SLG 2010-03-02
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Sensors_UrbanBlockMaterial_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:50 $
// $Revision: 9 $
// $Workfile: ADN_Sensors_UrbanBlockMaterial_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Sensors_UrbanBlockMaterial_GUI.h"
#include "ADN_Sensors_Modificators.h"

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_UrbanBlockMaterial_GUI constructor
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
ADN_Sensors_UrbanBlockMaterial_GUI::ADN_Sensors_UrbanBlockMaterial_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Sensors_ModificatorTable_ABC( tools::translate( "ADN_Sensors_UrbanBlockMaterial_GUI", "Material" ), tools::translate( "ADN_Sensors_UrbanBlockMaterial_GUI", "Modifiers" ), objectName, connector, pParent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_UrbanBlockMaterial_GUI destructor
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
ADN_Sensors_UrbanBlockMaterial_GUI::~ADN_Sensors_UrbanBlockMaterial_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_UrbanBlockMaterial_GUI::InternalEmit
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
void ADN_Sensors_UrbanBlockMaterial_GUI::InternalEmit()
{
    if( ADN_Sensors_Modificators::UrbanBlockInfos* data = static_cast< ADN_Sensors_Modificators::UrbanBlockInfos* >( GetSelectedData() ) )
        if( data  && data->GetCrossedElement() )
            emit ContentChanged( data->GetCrossedElement()->strName_.GetData(), data->rCoeff_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_UrbanBlockMaterial_GUI::AddRow
// Created: JSR 2012-11-05
// -----------------------------------------------------------------------------
void ADN_Sensors_UrbanBlockMaterial_GUI::AddRow( int row, void* data )
{
    ADN_Sensors_Modificators::UrbanBlockInfos* pInfos = static_cast< ADN_Sensors_Modificators::UrbanBlockInfos* >( data );
    if( !pInfos )
        return;
    AddItem( row, 0, data, &pInfos->strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );
    AddItem( row, 1, data, &pInfos->rCoeff_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}

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
#include "ADN_App.h"
#include "ADN_Tools.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Sensors_Data.h"
#include "ADN_Tr.h"

typedef ADN_Sensors_Data::SensorInfos                   SensorInfos;
typedef ADN_Sensors_Data::ModificatorUrbanBlockInfos    ModificatorUrbanBlockInfos;

// -----------------------------------------------------------------------------
// Name: ADN_CT_Sensors_UrbanBlock constructor
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
ADN_CT_Sensors_UrbanBlock::ADN_CT_Sensors_UrbanBlock( ADN_Table& table )
    : ADN_Connector_Table_ABC( table, false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_CT_Sensors_UrbanBlock::AddSubItems
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
void ADN_CT_Sensors_UrbanBlock::AddSubItems(int i,void *obj)
{
    assert( obj );
    ADN_TableItem_String* pItemString = 0;
    ADN_TableItem_Double* pItemDouble = 0;

    tab_.setItem( i, 0, pItemString = new ADN_TableItem_String( &tab_, obj ) );
    tab_.setItem( i, 1, pItemDouble = new ADN_TableItem_Double( &tab_, obj ) );

    pItemDouble->GetValidator().setRange( 0, 1, 2 );
    pItemString->GetConnector().Connect( &static_cast< ModificatorUrbanBlockInfos* >( obj )->ptrMaterial_.GetData()->strName_ );
    pItemDouble->GetConnector().Connect( &static_cast< ModificatorUrbanBlockInfos* >( obj )->rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_UrbanBlockMaterial_GUI constructor
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
ADN_Sensors_UrbanBlockMaterial_GUI::ADN_Sensors_UrbanBlockMaterial_GUI( QWidget * parent /*= 0*/ )
    : ADN_Sensors_MaterialsTable< ADN_CT_Sensors_UrbanBlock >( parent, tr( "ADN_Sensors_UrbanBlockMaterial_GUI" ), tr( "Material" ), tr( "Modifiers" ) )
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
    if( ModificatorUrbanBlockInfos* data = static_cast< ModificatorUrbanBlockInfos* >( GetCurrentData() ) )
        if( data  && data->ptrMaterial_.GetData() )
            emit ContentChanged( data->GetItemName(), data->rCoeff_.GetData() );
}

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
#include "ADN_App.h"
#include "ADN_Tools.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Sensors_Data.h"
#include "ENT/ENT_Tr.h"
#include "ADN_Tr.h"

typedef ADN_Sensors_Data::SensorInfos           SensorInfos;
typedef ADN_Sensors_Data::ModificatorMeteoInfos ModificatorMeteoInfos;

// -----------------------------------------------------------------------------
// Name: ADN_CT_Sensors_Meteos constructor
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
ADN_CT_Sensors_Meteos::ADN_CT_Sensors_Meteos( ADN_Table& tab )
    : ADN_Connector_Table_ABC( tab, false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_CT_Sensors_Meteos::AddSubItems
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
void ADN_CT_Sensors_Meteos::AddSubItems( int i, void *obj )
{
    assert( obj );
    ADN_TableItem_String *pItemString = 0;
    ADN_TableItem_Double *pItemDouble = 0;

    tab_.setItem( i, 0, pItemString = new ADN_TableItem_String( &tab_, obj ) );
    tab_.setItem( i, 1, pItemDouble = new ADN_TableItem_Double( &tab_, obj ) );

    pItemString->setEnabled( true );
    pItemString->setText( ADN_Tr::ConvertFromSensorWeatherModifiers( static_cast< ModificatorMeteoInfos* >( obj )->eType_, ENT_Tr_ABC::eToTr ).c_str() );
    pItemDouble->GetValidator().setRange( 0, 1, 3 );
    pItemDouble->GetConnector().Connect( &static_cast< ModificatorMeteoInfos* >( obj )->rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Meteos_GUI constructor
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
ADN_Sensors_Meteos_GUI::ADN_Sensors_Meteos_GUI( QWidget * parent /*= 0*/ )
    : ADN_Sensors_ModificatorTable( parent, tr( "ADN_Sensors_Meteos_GUI" ), tr( "Meteos"), tr( "Modifiers") )
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
    if( ModificatorMeteoInfos* data = static_cast< ModificatorMeteoInfos* >( GetCurrentData() ) )
        emit ContentChanged( data->GetItemName(), data->rCoeff_.GetData() );
}

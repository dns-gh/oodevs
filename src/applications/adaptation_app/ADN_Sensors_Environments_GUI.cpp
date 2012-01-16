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
#include "ADN_App.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Sensors_Data.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"

typedef ADN_Sensors_Data::SensorInfos                   SensorInfos;
typedef ADN_Sensors_Data::ModificatorEnvironmentInfos   ModificatorEnvironmentInfos;

// -----------------------------------------------------------------------------
// Name: ADN_CT_Sensors_Environments constructor
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
ADN_CT_Sensors_Environments::ADN_CT_Sensors_Environments( ADN_Table& tab )
    : ADN_Connector_Table_ABC( tab, false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_CT_Sensors_Environments::AddSubItems
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
void ADN_CT_Sensors_Environments::AddSubItems( int i ,void *obj )
{
    assert( obj );
    ADN_TableItem_String* pItemString = 0;
    ADN_TableItem_Double* pItemDouble = 0;

    tab_.setItem( i, 0, pItemString = new ADN_TableItem_String( &tab_, obj ) );
    tab_.setItem( i, 1, pItemDouble = new ADN_TableItem_Double( &tab_, obj ) );

    pItemString->setEnabled( true );
    pItemString->setText( ADN_Tr::ConvertFromVisionObject( static_cast< ModificatorEnvironmentInfos* >( obj )->eType_, ENT_Tr_ABC::eToTr ).c_str() );
    pItemDouble->GetValidator().setRange( 0, 1, 2 );
    pItemDouble->GetConnector().Connect( &static_cast<ModificatorEnvironmentInfos* >( obj )->rCoeff_ );

    if( static_cast< ModificatorEnvironmentInfos* >( obj )->eType_ == eVisionEmpty )
    {
        pItemDouble->setEnabled( false );
        static_cast< ModificatorEnvironmentInfos* >( obj )->rCoeff_ = 1;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Environments_GUI constructor
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
ADN_Sensors_Environments_GUI::ADN_Sensors_Environments_GUI( QWidget * parent /*= 0*/ )
    : ADN_Sensors_MaterialsTable< ADN_CT_Sensors_Environments >( parent, tr( "ADN_Sensors_Environments_GUI" ), tr( "Environments" ), tr( "Modifiers" ) )
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
    if( ModificatorEnvironmentInfos* data = static_cast< ModificatorEnvironmentInfos* >( GetCurrentData() ) )
        emit ContentChanged( data->GetItemName(), data->rCoeff_.GetData() );
}

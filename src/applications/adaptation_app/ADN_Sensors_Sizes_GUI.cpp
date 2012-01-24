//*****************************************************************************
//
// $Created: JDY 03-07-08 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Sensors_Sizes_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:53 $
// $Revision: 8 $
// $Workfile: ADN_Sensors_Sizes_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Sensors_Sizes_GUI.h"
#include "ADN_App.h"
#include "ADN_CommonGfx.h"
#include "ADN_Sensors_Data.h"
#include "ADN_Workspace.h"

typedef ADN_Sensors_Data::ModificatorSizeInfos ModificatorSizeInfos;

// -----------------------------------------------------------------------------
// Name: ADN_CT_Sensors_Sizes Constructor
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
ADN_CT_Sensors_Sizes::ADN_CT_Sensors_Sizes( ADN_Table& table )
    : ADN_Connector_Table_ABC( table, false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_CT_Sensors_Sizes::AddSubItems
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
void ADN_CT_Sensors_Sizes::AddSubItems(int i,void *obj)
{
    assert( obj );
    ADN_TableItem_String* pItemString = 0;
    ADN_TableItem_Double* pItemDouble = 0;

    tab_.setItem( i, 0, pItemString = new ADN_TableItem_String( &tab_, obj ) );
    tab_.setItem( i, 1, pItemDouble = new ADN_TableItem_Double( &tab_, obj ) );

    pItemDouble->GetValidator().setRange( 0, 1, 2 );
    pItemString->GetConnector().Connect( static_cast< ModificatorSizeInfos* >( obj )->ptrSize_.GetData() );
    pItemDouble->GetConnector().Connect( &static_cast< ModificatorSizeInfos* >( obj )->rCoeff_ );
}


// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Sizes_GUI constructor
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
ADN_Sensors_Sizes_GUI::ADN_Sensors_Sizes_GUI( QWidget * parent /*= 0*/ )
    : ADN_Sensors_ModificatorTable< ADN_CT_Sensors_Sizes >( parent, tr( "ADN_Sensors_Sizes_GUI" ), tr( "Volumes" ), tr( "Modifiers" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Sizes_GUI destructor
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
ADN_Sensors_Sizes_GUI::~ADN_Sensors_Sizes_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Sizes_GUI::InternalEmit
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
void ADN_Sensors_Sizes_GUI::InternalEmit()
{
    ModificatorSizeInfos* data = static_cast< ModificatorSizeInfos* >( GetCurrentData() );
    if( data  && data->ptrSize_.GetData() )
        emit ContentChanged( data->GetItemName(), data->rCoeff_.GetData() );
}

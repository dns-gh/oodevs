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
#include "moc_ADN_Sensors_Postures_GUI.cpp"
#include "ADN_App.h"
#include "ADN_CommonGfx.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Sensors_Data.h"
#include "ADN_Workspace.h"
#include "ENT/ENT_Tr.h"

typedef ADN_Sensors_Data::ModificatorPostureInfos    ModificatorPostureInfos;

// -----------------------------------------------------------------------------
// Name: ADN_CT_Sensors_Postures constructor
// Created: ABR 2012-01-24
// -----------------------------------------------------------------------------
ADN_CT_Sensors_Postures::ADN_CT_Sensors_Postures( ADN_Table& table )
    : ADN_Connector_Table_ABC( table, false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_CT_Sensors_Postures::AddSubItems
// Created: ABR 2012-01-24
// -----------------------------------------------------------------------------
void ADN_CT_Sensors_Postures::AddSubItems( int i, void *pObj )
{
    assert( pObj != 0 );
    ADN_TableItem_String* pItemString = new ADN_TableItem_String( &tab_, pObj );
    ADN_TableItem_Double* pItemDouble = new ADN_TableItem_Double( &tab_, pObj );

    // Add a new row & set the new values.
    tab_.setItem( i, 0, pItemString );
    tab_.setItem( i, 1, pItemDouble );

    // Setup the row header item.
    pItemString->setEnabled( true );
    pItemString->setText( ENT_Tr::ConvertFromUnitPosture( static_cast<ModificatorPostureInfos*>( pObj )->eType_, ENT_Tr_ABC::eToTr ).c_str() );

    // Setup the value item.
    pItemDouble->GetValidator().setRange( 0, 1, 3 );
    pItemDouble->GetConnector().Connect( &static_cast<ModificatorPostureInfos*>( pObj )->rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Postures_GUI constructor
// Created: ABR 2012-01-24
// -----------------------------------------------------------------------------
ADN_Sensors_Postures_GUI::ADN_Sensors_Postures_GUI( const QString& strColCaption, QWidget * parent /*= 0*/ )
    : ADN_Sensors_ModificatorTable< ADN_CT_Sensors_Postures >( parent, tr( "Sensors_Postures" ), strColCaption, tr( "Modifiers" ) )
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
    if( ModificatorPostureInfos* data = static_cast< ModificatorPostureInfos* >( GetCurrentData() ) )
        emit ContentChanged( data->GetItemName(), data->rCoeff_.GetData() );
}

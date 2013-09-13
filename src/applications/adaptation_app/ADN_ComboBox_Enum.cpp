// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ComboBox_Enum.h"

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox_Enum constructor
// Created: JDY 03-08-29
//-----------------------------------------------------------------------------
ADN_ComboBox_Enum::ADN_ComboBox_Enum( QWidget* parent )
    : ADN_ComboBox( parent )
{
    pConnector_ = new ADN_Connector_EnumComboBox( this );
    assert( pConnector_ );
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox_Enum destructor
// Created: JDY 03-08-29
//-----------------------------------------------------------------------------
ADN_ComboBox_Enum::~ADN_ComboBox_Enum()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ComboBox_Enum::SetCurrentData
// Created: ABR 2012-10-26
// -----------------------------------------------------------------------------
void ADN_ComboBox_Enum::SetCurrentData(void *data)
{
    if( !data )
        clear();
        //SetAutoClear( true );

    DisconnectItem();

    pCurData_ = data;

    ConnectItem();

    if( !data )
        clear();
        //SetAutoClear( false );

}

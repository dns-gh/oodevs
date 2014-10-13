// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_RadioButton.h"
#include "ADN_Connector_Bool.h"
#include "moc_ADN_RadioButton.cpp"

ADN_RadioButton::ADN_RadioButton( QWidget* pParent, const char* szName /* = 0*/ )
    : QRadioButton( pParent, szName )
{
    pConnector_.reset( new ADN_Connector_Bool< ADN_RadioButton >( this ) );
    connect( this, SIGNAL( toggled( bool ) ), this, SLOT( BoolChanged( bool ) ) );
}

ADN_RadioButton::~ADN_RadioButton()
{
    // NOTHING
}

void ADN_RadioButton::BoolChanged( bool b )
{
    static_cast< ADN_Connector_Bool< ADN_RadioButton >& >( *pConnector_ ).SetDataChanged( b );
}

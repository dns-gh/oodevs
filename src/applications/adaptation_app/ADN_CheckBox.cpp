//*****************************************************************************
//
// $Created: JDY 03-07-02 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_CheckBox.cpp $
// $Author: Ape $
// $Modtime: 7/04/05 11:21 $
// $Revision: 4 $
// $Workfile: ADN_CheckBox.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_CheckBox.h"

#include "moc_ADN_CheckBox.cpp"

#include "ADN_Connector_ABC.h"
#include "ADN_Connector_Bool.h"

//-----------------------------------------------------------------------------
// Name: ADN_CheckBox constructor
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
ADN_CheckBox::ADN_CheckBox( QWidget* parent, const char* name )
    : QCheckBox( parent, name )
{
    pConnector_.reset( new ADN_Connector_Bool< ADN_CheckBox >( this ) );
    connect( this, SIGNAL( toggled( bool ) ), this, SLOT( BoolChanged( bool ) ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_CheckBox constructor
// Created: JDY 03-07-02
//-----------------------------------------------------------------------------
ADN_CheckBox::ADN_CheckBox(const QString& text, QWidget* parent, const char* name )
    : QCheckBox( text, parent, name )
{
    pConnector_.reset( new ADN_Connector_Bool< ADN_CheckBox >( this ) );
    connect( this, SIGNAL( toggled( bool ) ), this, SLOT( BoolChanged( bool ) ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_CheckBox constructor
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
ADN_CheckBox::~ADN_CheckBox()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_CheckBox::BoolChanged
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_CheckBox::BoolChanged( bool b )
{
    static_cast< ADN_Connector_Bool< ADN_CheckBox >& >( *pConnector_ ).SetDataChanged( b );
}

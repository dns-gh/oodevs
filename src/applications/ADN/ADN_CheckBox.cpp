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
#include "ADN_Pch.h"
#include "ADN_CheckBox.h"

#include "moc_ADN_CheckBox.cpp"

#include "ADN_Connector_ABC.h"
#include "ADN_Connector_Bool.h"

#include "ADN_Workspace.h"
#include "ADN_Enums.h"
#include "ADN_App.h"
#include "ADN_MainWindow.h"

//-----------------------------------------------------------------------------
// Name: ADN_CheckBox constructor
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
ADN_CheckBox::ADN_CheckBox(QWidget* parent, const char * name)
: QCheckBox(parent,name)
, ADN_Gfx_ABC()
{
    pConnector_=new ADN_Connector_Bool<ADN_CheckBox>(this);
    connect(this        ,SIGNAL( toggled( bool ) )    ,this         , SLOT( BoolChanged(bool) ) );
    connect( static_cast< ADN_App* >( qApp )->GetMainWindow(), SIGNAL(OpenModeToggled()), this, SLOT(UpdateEnableState()) );
}


//-----------------------------------------------------------------------------
// Name: ADN_CheckBox constructor
// Created: JDY 03-07-02
//-----------------------------------------------------------------------------
ADN_CheckBox::ADN_CheckBox(const QString &text, QWidget *parent, const char* name)
: QCheckBox(text,parent,name)
, ADN_Gfx_ABC()
{
    pConnector_=new ADN_Connector_Bool<ADN_CheckBox>(this);
    connect(this        ,SIGNAL( toggled( bool ) )    ,this         , SLOT( BoolChanged(bool) ) );
    connect( static_cast< ADN_App* >( qApp )->GetMainWindow(), SIGNAL(OpenModeToggled()), this, SLOT(UpdateEnableState()) );
}

//-----------------------------------------------------------------------------
// Name: ADN_CheckBox constructor
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
ADN_CheckBox::~ADN_CheckBox()
{
    delete pConnector_;
}


//-----------------------------------------------------------------------------
// Name: ADN_CheckBox::BoolChanged
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_CheckBox::BoolChanged(bool b)
{
    static_cast<ADN_Connector_Bool<ADN_CheckBox>*>(pConnector_)->SetDataChanged(b);
}

// -----------------------------------------------------------------------------
// Name: ADN_CheckBox::setEnabled
/** @param  b 
*/
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
void ADN_CheckBox::setEnabled( bool b )
{
    if( bEnabledOnlyInAdminMode_ && b )
        QCheckBox::setEnabled( ADN_Workspace::GetWorkspace().GetOpenMode() == eOpenMode_Admin );
    else
        QCheckBox::setEnabled( b );
}

// -----------------------------------------------------------------------------
// Name: ADN_CheckBox::UpdateEnableState
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
void ADN_CheckBox::UpdateEnableState()
{
    if( bEnabledOnlyInAdminMode_ && IsAutoEnabled() )
        setEnabled( static_cast< ADN_Connector_Bool<ADN_CheckBox>* >( pConnector_ )->IsConnected() );
}

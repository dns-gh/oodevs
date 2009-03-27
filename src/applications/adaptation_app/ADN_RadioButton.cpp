//*****************************************************************************
//
// $Created: JDY 03-07-02 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_RadioButton.cpp $
// $Author: Ape $
// $Modtime: 7/04/05 11:21 $
// $Revision: 4 $
// $Workfile: ADN_RadioButton.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_RadioButton.h"

#include "moc_ADN_RadioButton.cpp"

#include "ADN_Connector_ABC.h"
#include "ADN_Connector_Bool.h"

#include "ADN_Workspace.h"
#include "ADN_Enums.h"
#include "ADN_App.h"
#include "ADN_MainWindow.h"

//-----------------------------------------------------------------------------
// Name: ADN_RadioButton constructor
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
ADN_RadioButton::ADN_RadioButton(QWidget* parent, const char * name)
: QRadioButton(parent,name)
, ADN_Gfx_ABC()
{
    pConnector_=new ADN_Connector_Bool<ADN_RadioButton>(this);
    connect(this        ,SIGNAL( toggled( bool ) )    ,this         , SLOT( BoolChanged(bool) ) );
    connect( static_cast< ADN_App* >( qApp )->GetMainWindow(), SIGNAL(OpenModeToggled()), this, SLOT(UpdateEnableState()) );
}


//-----------------------------------------------------------------------------
// Name: ADN_RadioButton constructor
// Created: JDY 03-07-02
//-----------------------------------------------------------------------------
ADN_RadioButton::ADN_RadioButton(const QString &text, QWidget *parent, const char* name)
: QRadioButton(text,parent,name)
, ADN_Gfx_ABC()
{
    pConnector_=new ADN_Connector_Bool<ADN_RadioButton>(this);
    connect(this        ,SIGNAL( toggled( bool ) )    ,this         , SLOT( BoolChanged(bool) ) );
    connect( static_cast< ADN_App* >( qApp )->GetMainWindow(), SIGNAL(OpenModeToggled()), this, SLOT(UpdateEnableState()) );
}

//-----------------------------------------------------------------------------
// Name: ADN_RadioButton constructor
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
ADN_RadioButton::~ADN_RadioButton()
{
    delete pConnector_;
}


//-----------------------------------------------------------------------------
// Name: ADN_RadioButton::BoolChanged
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_RadioButton::BoolChanged(bool b)
{
    static_cast<ADN_Connector_Bool<ADN_RadioButton>*>(pConnector_)->SetDataChanged(b);
}

// -----------------------------------------------------------------------------
// Name: ADN_RadioButton::setEnabled
/** @param  b 
*/
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
void ADN_RadioButton::setEnabled( bool b )
{
    if( bEnabledOnlyInAdminMode_ && b )
        QRadioButton::setEnabled( ADN_Workspace::GetWorkspace().GetOpenMode() == eOpenMode_Admin );
    else
        QRadioButton::setEnabled( b );
}

// -----------------------------------------------------------------------------
// Name: ADN_RadioButton::UpdateEnableState
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
void ADN_RadioButton::UpdateEnableState()
{
    if( bEnabledOnlyInAdminMode_ && IsAutoEnabled() )
        setEnabled( static_cast< ADN_Connector_Bool<ADN_RadioButton>* >( pConnector_ )->IsConnected() );
}

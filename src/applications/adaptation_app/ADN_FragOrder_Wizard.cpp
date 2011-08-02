// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_FragOrder_Wizard.h"
#include "ADN_Wizard_FirstPage_Default.h"
#include "ADN_FragOrder_WizardSecondPage.h"
#include "ADN_Wizard_LastPage.h"

typedef ADN_Missions_Data::FragOrder FragOrder;

// -----------------------------------------------------------------------------
// Name: ADN_FragOrder_Wizard constructor
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_FragOrder_Wizard::ADN_FragOrder_Wizard( ADN_Missions_Data::T_FragOrder_Vector& fragOrders, QWidget* pParent /* = 0*/, const char* szName /* = 0*/ )
    : ADN_Wizard_ABC< FragOrder >( pParent, szName )
    , secondPage_ ( 0 )
    , name_ ( "" )
{
    setCaption( qApp->translate( "ADN_FragOrder_Wizard", "New fragmentary order creation" ) );

    ADN_Wizard_FirstPage_Default< FragOrder >* pFirstPage = new ADN_Wizard_FirstPage_Default< FragOrder >( fragOrders, this );
    pFirstPage->SetCaptions( qApp->translate( "ADN_FragOrder_Wizard", "New fragmentary order creation" ), qApp->translate( "ADN_FragOrder_Wizard","Fragmentary orders" ) );
    pFirstPage_ = pFirstPage;
    secondPage_.reset( new ADN_FragOrder_WizardSecondPage( this ) );
    addPage( secondPage_.get() ,  qApp->translate( "ADN_FragOrder_Wizard", "Default assignation" ) );
    new ADN_Wizard_LastPage( this, qApp->translate( "ADN_FragOrder_Wizard", "Creation completed" ),
        qApp->translate( "ADN_FragOrder_Wizard", "Click \"Done\" to create the new fragmentary order." ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_FragOrder_Wizard destructor
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_FragOrder_Wizard::~ADN_FragOrder_Wizard()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_FragOrder_Wizard::ADN_FragOrder_Wizard::ValidateAll
// Created: HBD 2010-09-01
// -----------------------------------------------------------------------------
bool ADN_FragOrder_Wizard::ValidateAll()
{
    if(! ADN_Wizard_ABC_ADN_Missions_Data_FragOrder_::ValidateAll() )
        return false;
    name_ = pResult_->GetItemName();
    pResult_->isAvailableWithoutMission_ = true;
    return true;
}

// -----------------------------------------------------------------------------
// Name: ADN_FragOrder_Wizard::ADN_FragOrder_Wizard::ValidateAll
// Created: HBD 2010-09-01
// -----------------------------------------------------------------------------
void ADN_FragOrder_Wizard::Polish()
{
    secondPage_->AddFragOrderForAll( name_ );
}


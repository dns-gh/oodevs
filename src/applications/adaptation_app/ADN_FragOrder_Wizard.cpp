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
#include "ADN_Wizard_LastPage.h"

typedef ADN_Missions_Data::FragOrder FragOrder;

// -----------------------------------------------------------------------------
// Name: ADN_FragOrder_Wizard constructor
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_FragOrder_Wizard::ADN_FragOrder_Wizard( ADN_Missions_Data::T_FragOrder_Vector& fragOrders, QWidget* pParent /*= 0*/, const char* szName /*= 0*/ )
    : ADN_Wizard_ABC< FragOrder >( pParent, szName )
{
    setCaption( tr( "New fragmentary order creation" ) );

    ADN_Wizard_FirstPage_Default< FragOrder >* pFirstPage = new ADN_Wizard_FirstPage_Default< FragOrder >( fragOrders, this );
    pFirstPage->SetCaptions( tr( "New fragmentary order creation" ), tr( "Fragmentary orders" ) );
    pFirstPage_ = pFirstPage;
    new ADN_Wizard_LastPage( this, tr( "Creation completed" ), tr( "Click \"Done\" to create the new fragmentary order." ) );
}
    
// -----------------------------------------------------------------------------
// Name: ADN_FragOrder_Wizard destructor
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_FragOrder_Wizard::~ADN_FragOrder_Wizard()
{
    // NOTHING
}

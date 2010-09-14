// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ResourceNetworks_Wizard.h"
#include "ADN_Wizard_FirstPage_Default.h"
#include "ADN_Wizard_LastPage.h"

typedef ADN_ResourceNetworks_Data::ResourceNetworkInfos ResourceNetworkInfos;

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_Wizard constructor
// Created: JSR 2010-09-14
// -----------------------------------------------------------------------------
ADN_ResourceNetworks_Wizard::ADN_ResourceNetworks_Wizard( QWidget* pParent , const char* szName )
    : ADN_Wizard_ABC< ADN_ResourceNetworks_Data::ResourceNetworkInfos >( pParent, szName )
{
    setCaption( qApp->translate( "ADN_ResourceNetworks_Wizard", "New resource network creation" ) );

    ADN_ResourceNetworks_Data::T_ResourceNetworkInfosVector& resources = ADN_Workspace::GetWorkspace().GetResourceNetworks().GetData().GetResourceNetworksInfos();
    ADN_Wizard_FirstPage_Default< ResourceNetworkInfos >* pFirstPage = new ADN_Wizard_FirstPage_Default< ResourceNetworkInfos >( resources, this );
    pFirstPage->SetCaptions( qApp->translate( "ADN_ResourceNetworks_Wizard", "New resource network creation" ),
        qApp->translate( "ADN_ResourceNetworks_Wizard", "Resource networks" ) );
    pFirstPage_ = pFirstPage;

    new ADN_Wizard_LastPage( this, qApp->translate( "ADN_ResourceNetworks_Wizard", "Creation finished" ),
        qApp->translate( "ADN_ResourceNetworks_Wizard", "Click \"Done\" to create the new resource network." ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_Wizard destructor
// Created: JSR 2010-09-14
// -----------------------------------------------------------------------------
ADN_ResourceNetworks_Wizard::~ADN_ResourceNetworks_Wizard()
{
    // NOTHING
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Radars_Wizard.h"
#include "ADN_Workspace.h"
#include "ADN_Radars_Data.h"
#include "ADN_Wizard_FirstPage_Default.h"
#include "ADN_Wizard_LastPage.h"
#include "ADN_Sensors_Data.h"

typedef ADN_Radars_Data::RadarInfos RadarInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Radars_Wizard constructor
// Created: LGY 2010-08-18
// -----------------------------------------------------------------------------
ADN_Radars_Wizard::ADN_Radars_Wizard( QWidget* pParent, const char* szName )
    : ADN_Wizard_ABC< RadarInfos >( pParent, szName )
{
    setCaption( qApp->translate( "ADN_Radars_Wizard", "New special sensor creation" ) );

    ADN_Radars_Data::T_RadarInfos_Vector& radar = ADN_Workspace::GetWorkspace().GetSensors().GetData().radarData_.vRadars_;
    ADN_Wizard_FirstPage_Default< RadarInfos >* pFirstPage = new ADN_Wizard_FirstPage_Default< RadarInfos >( radar, this );
    pFirstPage->SetCaptions( qApp->translate( "ADN_Radars_Wizard", "New special sensor creation" ),qApp->translate( "ADN_Radars_Wizard", "Special sensor" ) );
    pFirstPage_ = pFirstPage;

    new ADN_Wizard_LastPage( this,qApp->translate( "ADN_Radars_Wizard", "Creation finished" ),
        qApp->translate( "ADN_Radars_Wizard", "Click \"Done\" to create the new special sensor." ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Radars_Wizard destructor
// Created: LGY 2010-08-18
// -----------------------------------------------------------------------------
ADN_Radars_Wizard::~ADN_Radars_Wizard()
{
    // NOTHING
}

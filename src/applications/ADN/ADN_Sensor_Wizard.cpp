// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2003-11-03 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Sensor_Wizard.cpp $
// $Author: Ape $
// $Modtime: 11/04/05 11:00 $
// $Revision: 7 $
// $Workfile: ADN_Sensor_Wizard.cpp $
//
// *****************************************************************************


#include "ADN_pch.h"
#include "ADN_Sensor_Wizard.h"

#include "ADN_Sensors_Data.h"
#include "ADN_Wizard_FirstPage_Default.h"
#include "ADN_Wizard_LastPage.h"

typedef ADN_Sensors_Data::SensorInfos SensorInfos;


// -----------------------------------------------------------------------------
// Name: ADN_Sensor_Wizard constructor
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
ADN_Sensor_Wizard::ADN_Sensor_Wizard( QWidget* pParent, const char* szName )
: ADN_Wizard_ABC<ADN_Sensors_Data::SensorInfos>( pParent, szName )
{
    setCaption( tr( "New sensor creation" ) );

    ADN_Sensors_Data::T_SensorsInfos_Vector& sensors = ADN_Workspace::GetWorkspace().GetSensors().GetData().GetSensorsInfos();
    ADN_Wizard_FirstPage_Default<SensorInfos>* pFirstPage = new ADN_Wizard_FirstPage_Default<SensorInfos>( sensors, this );
    pFirstPage->SetCaptions( tr( "New sensor creation" ), tr( "Sensors" ) );
    pFirstPage_ = pFirstPage;

    new ADN_Wizard_LastPage( this, tr( "Creation completed" ), tr( "Click \"Done\" to create the new sensor." ) );
}


// -----------------------------------------------------------------------------
// Name: ADN_Sensor_Wizard destructor
// Created: APE 2005-01-28
// -----------------------------------------------------------------------------
ADN_Sensor_Wizard::~ADN_Sensor_Wizard()
{
}

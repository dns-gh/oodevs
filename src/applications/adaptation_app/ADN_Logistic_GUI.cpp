// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Logistic_GUI.h"
#include "ADN_Logistic_Data.h"

#include "ADN_Maintenance_GUI.h"
#include "ADN_Supply_GUI.h"
#include "ADN_Health_GUI.h"
#include "ADN_Funeral_GUI.h"
#include "ADN_Maintenance_Data.h"
#include "ADN_Supply_Data.h"
#include "ADN_Health_Data.h"
#include "ADN_Funeral_Data.h"
#include "ADN_GuiBuilder.h"

// -----------------------------------------------------------------------------
// Name: ADN_Logistic_GUI constructor
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
ADN_Logistic_GUI::ADN_Logistic_GUI( ADN_Logistic_Data& data )
    : ADN_Tab_Container( "ADN_Logistic_GUI", data )
{
    AddElement< ADN_Maintenance_GUI, ADN_Maintenance_Data >( eMaintenance, tr( "Maintenance" ) );
    AddElement< ADN_Supply_GUI, ADN_Supply_Data >( eSupply, tr( "Supply" ) );
    AddElement< ADN_Health_GUI, ADN_Health_Data >( eHealth, tr( "Health" ) );
    AddElement< ADN_Funeral_GUI, ADN_Funeral_Data >( eFuneral, tr( "Funeral" ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Logistic_GUI destructor
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
ADN_Logistic_GUI::~ADN_Logistic_GUI()
{
    // NOTHING
}

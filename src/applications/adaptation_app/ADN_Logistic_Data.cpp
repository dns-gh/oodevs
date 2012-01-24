// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Logistic_Data.h"

#include "ADN_Maintenance_Data.h"
#include "ADN_Supply_Data.h"
#include "ADN_Health_Data.h"
#include "ADN_Funeral_Data.h"

// -----------------------------------------------------------------------------
// Name: ADN_Logistic_Data constructor
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
ADN_Logistic_Data::ADN_Logistic_Data()
    : ADN_Data_Container()
{
    AddElement< ADN_Maintenance_Data >( eMaintenance );
    AddElement< ADN_Supply_Data >( eSupply );
    AddElement< ADN_Health_Data >( eHealth );
    AddElement< ADN_Funeral_Data >( eFuneral );
}

// -----------------------------------------------------------------------------
// Name: ADN_Logistic_Data destructor
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
ADN_Logistic_Data::~ADN_Logistic_Data()
{
    // NOTHING
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Categories_Data.h"
#include "ADN_Armors_Data.h"
#include "ADN_Enums.h"
#include "ADN_LogisticSupplyClasses_Data.h"
#include "ADN_Natures_Data.h"
#include "ADN_Volumes_Data.h"

//-----------------------------------------------------------------------------
// Name: ADN_Categories_Data constructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_Categories_Data::ADN_Categories_Data()
    : ADN_Data_Container( eCategories )
{
    AddElement< ADN_Armors_Data >( eArmors );
    AddElement< ADN_Volumes_Data >( eVolumes );
    AddElement< ADN_Natures_Data >( eNatures );
    AddElement< ADN_LogisticSupplyClasses_Data >( eLogisticSupplyClasses );
}

//-----------------------------------------------------------------------------
// Name: ADN_Categories_Data destructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_Categories_Data::~ADN_Categories_Data()
{
    // NOTHING
}

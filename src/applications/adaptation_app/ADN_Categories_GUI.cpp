// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Categories_GUI.h"
#include "moc_ADN_Categories_GUI.cpp"
#include "ADN_Armors_Data.h"
#include "ADN_Armors_GUI.h"
#include "ADN_Categories_Data.h"
#include "ADN_LogisticSupplyClasses_Data.h"
#include "ADN_LogisticSupplyClasses_GUI.h"
#include "ADN_Natures_Data.h"
#include "ADN_Natures_GUI.h"
#include "ADN_Volumes_Data.h"
#include "ADN_Volumes_GUI.h"

//-----------------------------------------------------------------------------
// Name: ADN_Categories_GUI constructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_Categories_GUI::ADN_Categories_GUI( ADN_Categories_Data& data )
    : ADN_Grid_Container( eCategories, data )
{
    AddElement< ADN_Armors_GUI, ADN_Armors_Data >( eArmors, "", 0, 0, 3, 1 );
    AddElement< ADN_Volumes_GUI, ADN_Volumes_Data>( eVolumes, "", 0, 1 );
    AddElement< ADN_Natures_GUI, ADN_Natures_Data>( eNatures, "", 1, 1 );
    AddElement< ADN_LogisticSupplyClasses_GUI, ADN_LogisticSupplyClasses_Data>( eLogisticSupplyClasses, "", 2, 1 );
}

//-----------------------------------------------------------------------------
// Name: ADN_Categories_GUI destructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_Categories_GUI::~ADN_Categories_GUI()
{
    // NOTHING
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "AGR_pch.h"
#include "AGR_EquipmentType.h"

// -----------------------------------------------------------------------------
// Name: AGR_EquipmentType constructor
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
AGR_EquipmentType::AGR_EquipmentType()
    : AGR_Type_ABC( "ScipioBase:TypeEquipement", "TypeEquipement", "TypeEquipement", "TypeEquipement", true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_EquipmentType destructor
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
AGR_EquipmentType::~AGR_EquipmentType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_EquipmentType::MOS2RCCode
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
std::string AGR_EquipmentType::MOS2RCCode( const std::string& strAsnParam ) const
{
    std::stringstream strTmp;
    strTmp << "EquipmentLink( "
           << strAsnParam
           << ", equipmentResolver"
           << " )";
    return strTmp.str();
}

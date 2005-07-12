// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-29 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_FireInteraction.cpp $
// $Author: Age $
// $Modtime: 29/11/04 16:54 $
// $Revision: 1 $
// $Workfile: HLA_FireInteraction.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "HLA_FireInteraction.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"

// -----------------------------------------------------------------------------
// Name: HLA_FireInteraction constructor
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
HLA_FireInteraction::HLA_FireInteraction()
    : strAmmunition_( "" )
{
    //NOTHING
}
 
// -----------------------------------------------------------------------------
// Name: HLA_FireInteraction constructor
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
HLA_FireInteraction::HLA_FireInteraction( const PHY_DotationCategory& ammunition )
: strAmmunition_( ammunition.GetName() )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_FireInteraction destructor
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
HLA_FireInteraction::~HLA_FireInteraction()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_FireInteraction::GetAmmunition
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
const PHY_DotationCategory* HLA_FireInteraction::GetAmmunition() const
{
    static PHY_DotationType const * const pAmmoType = PHY_DotationType::FindDotationType( "munition" );
    assert( pAmmoType );
    return pAmmoType->FindDotationCategory( strAmmunition_ );
}

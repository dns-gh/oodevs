// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_ActiveProtection.h"

#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_ActiveProtection constructor
// Created: LDC 2010-01-11
// -----------------------------------------------------------------------------
PHY_RolePion_ActiveProtection::PHY_RolePion_ActiveProtection( PHY_RolePion_Composantes& pion )
    : pion_( pion )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_ActiveProtection destructor
// Created: LDC 2010-01-11
// -----------------------------------------------------------------------------
PHY_RolePion_ActiveProtection::~PHY_RolePion_ActiveProtection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_ActiveProtection::UseAmmunition
// Created: LDC 2010-01-11
// -----------------------------------------------------------------------------
void PHY_RolePion_ActiveProtection::UseAmmunition( const PHY_DotationCategory& category )
{
    pion_.UseAmmunition( category );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_ActiveProtection::GetPHModifier
// Created: LDC 2010-01-11
// -----------------------------------------------------------------------------
double PHY_RolePion_ActiveProtection::GetPHModifier( const PHY_DotationCategory& category ) const
{
    return pion_.GetPHModifier( category );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_ActiveProtection::CounterIndirectFire
// Created: LDC 2010-01-11
// -----------------------------------------------------------------------------
bool PHY_RolePion_ActiveProtection::CounterIndirectFire( const PHY_DotationCategory& category ) const
{
    return pion_.CounterIndirectFire( category );
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_ActiveProtection::DestroyIndirectFire
// Created: LDC 2010-01-11
// -----------------------------------------------------------------------------
bool PHY_RolePion_ActiveProtection::DestroyIndirectFire( const PHY_DotationCategory& category ) const
{
    return pion_.DestroyIndirectFire( category );
}


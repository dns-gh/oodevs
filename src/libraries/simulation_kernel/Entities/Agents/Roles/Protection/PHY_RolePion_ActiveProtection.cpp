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
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RolePion_ActiveProtection )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_ActiveProtection constructor
// Created: LDC 2010-01-11
// -----------------------------------------------------------------------------
PHY_RolePion_ActiveProtection::PHY_RolePion_ActiveProtection( PHY_RolePion_Composantes& pion )
    : owner_( pion )
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
    owner_.UseAmmunition( category );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_ActiveProtection::GetPHModifier
// Created: LDC 2010-01-11
// -----------------------------------------------------------------------------
double PHY_RolePion_ActiveProtection::GetPHModifier( const PHY_DotationCategory& category ) const
{
    return owner_.GetPHModifier( category );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_ActiveProtection::CounterIndirectFire
// Created: LDC 2010-01-11
// -----------------------------------------------------------------------------
bool PHY_RolePion_ActiveProtection::CounterIndirectFire( const PHY_DotationCategory& category ) const
{
    return owner_.CounterIndirectFire( category );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_ActiveProtection::DestroyIndirectFire
// Created: LDC 2010-01-11
// -----------------------------------------------------------------------------
bool PHY_RolePion_ActiveProtection::DestroyIndirectFire( const PHY_DotationCategory& category ) const
{
    return owner_.DestroyIndirectFire( category );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::serialize
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_ActiveProtection::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_RoleInterface_ActiveProtection >( *this );
}

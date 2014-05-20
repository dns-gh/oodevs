// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_AmmoEffect.h"
#include "Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "meteo/PHY_Lighting.h"
#include "meteo/PHY_Precipitation.h"

//-----------------------------------------------------------------------------
// Name: PHY_AmmoEffect constructor
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
PHY_AmmoEffect::PHY_AmmoEffect( const PHY_IndirectFireDotationClass& ammoClass, PHY_AmmoEffect* pNextEffect )
    : ammoClass_  ( ammoClass )
    , pNextEffect_( pNextEffect )
{
    assert( ammoClass_ == PHY_IndirectFireDotationClass::eclairant_ || ammoClass_ == PHY_IndirectFireDotationClass::fumigene_ );
}

//-----------------------------------------------------------------------------
// Name: PHY_AmmoEffect destructor
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
PHY_AmmoEffect::~PHY_AmmoEffect()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoEffect::GetLighting
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
const weather::PHY_Lighting& PHY_AmmoEffect::GetLighting( const weather::PHY_Lighting& mainLighting ) const
{
    return ammoClass_ == PHY_IndirectFireDotationClass::eclairant_ ? weather::PHY_Lighting::eclairant_
        : pNextEffect_ ? pNextEffect_->GetLighting( mainLighting )
        : mainLighting;
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoEffect::GetPrecipitation
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
const weather::PHY_Precipitation& PHY_AmmoEffect::GetPrecipitation( const weather::PHY_Precipitation& mainPrecipitation ) const
{
    return ammoClass_ == PHY_IndirectFireDotationClass::fumigene_ ? weather::PHY_Precipitation::smoke_
        : pNextEffect_ ? pNextEffect_->GetPrecipitation( mainPrecipitation )
        : mainPrecipitation;
}

//-----------------------------------------------------------------------------
// Name: PHY_AmmoEffect::SetNextEffect
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
void PHY_AmmoEffect::SetNextEffect( PHY_AmmoEffect* pNextEffect )
{
    pNextEffect_ = pNextEffect;
}

//-----------------------------------------------------------------------------
// Name: PHY_AmmoEffect::HandleAmmo
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
bool PHY_AmmoEffect::HandleAmmo( const PHY_IndirectFireDotationClass& ammoClass ) const
{
    return ammoClass_ == ammoClass;
}

//-----------------------------------------------------------------------------
// Name: PHY_AmmoEffect::GetNextEffect
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
PHY_AmmoEffect* PHY_AmmoEffect::GetNextEffect() const
{
    return pNextEffect_;
}


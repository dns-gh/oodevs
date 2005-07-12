//*****************************************************************************
//
// $Created: JVT 04-03-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Meteo/RawVisionData/PHY_AmmoEffect.cpp $
// $Author: Nld $
// $Modtime: 17/03/05 14:55 $
// $Revision: 2 $
// $Workfile: PHY_AmmoEffect.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"
#include "PHY_AmmoEffect.h"

#include "Meteo/PHY_Lighting.h"
#include "Meteo/PHY_Precipitation.h"

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
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoEffect::GetLighting
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
const PHY_Lighting& PHY_AmmoEffect::GetLighting( const PHY_Lighting& mainLighting ) const
{
    return ammoClass_ == PHY_IndirectFireDotationClass::eclairant_ ? PHY_Lighting::eclairant_
        : pNextEffect_ ? pNextEffect_->GetLighting( mainLighting )
        : mainLighting;
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoEffect::GetPrecipitation
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
const PHY_Precipitation& PHY_AmmoEffect::GetPrecipitation( const PHY_Precipitation& mainPrecipitation ) const
{
    return ammoClass_ == PHY_IndirectFireDotationClass::fumigene_ ? PHY_Precipitation::smoke_
        : pNextEffect_ ? pNextEffect_->GetPrecipitation( mainPrecipitation )
        : mainPrecipitation;
}


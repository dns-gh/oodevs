//*****************************************************************************
//
// $Created: JVT 04-03-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Meteo/RawVisionData/PHY_AmmoEffect.inl $
// $Author: Nld $
// $Modtime: 17/03/05 14:55 $
// $Revision: 2 $
// $Workfile: PHY_AmmoEffect.inl $
//
//*****************************************************************************

#include "Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"

//-----------------------------------------------------------------------------
// Name: PHY_AmmoEffect::SetNextEffect
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
inline
void PHY_AmmoEffect::SetNextEffect( PHY_AmmoEffect* pNextEffect )
{
    pNextEffect_ = pNextEffect;
}


//-----------------------------------------------------------------------------
// Name: PHY_AmmoEffect::HandleAmmo
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
inline
bool PHY_AmmoEffect::HandleAmmo( const PHY_IndirectFireDotationClass& ammoClass ) const
{
    return ammoClass_ == ammoClass;
}


//-----------------------------------------------------------------------------
// Name: PHY_AmmoEffect::GetNextEffect
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
inline
PHY_AmmoEffect* PHY_AmmoEffect::GetNextEffect() const
{
    return pNextEffect_;
}


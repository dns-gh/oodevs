// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

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


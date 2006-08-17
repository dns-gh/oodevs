// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/HumanFactors/PHY_RolePion_HumanFactors.inl $
// $Author: Jvt $
// $Modtime: 30/11/04 18:55 $
// $Revision: 2 $
// $Workfile: PHY_RolePion_HumanFactors.inl $
//
// *****************************************************************************

#include "Entities/Agents/Units/HumanFactors/PHY_Morale.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Experience.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Tiredness.h"

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_HumanFactors::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::Clean
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_HumanFactors::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::HasChanged
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_HumanFactors::HasChanged() const
{
    return bHasChanged_;
}

// =============================================================================
// SETTERS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::SetMorale
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_HumanFactors::SetMorale( const PHY_Morale& morale )
{
    if( morale == *pMorale_ )
        return;

    pMorale_     = &morale;
    bHasChanged_ = true;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::SetExperience
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_HumanFactors::SetExperience( const PHY_Experience& experience )
{
    if( experience == *pExperience_ )
        return;

    pExperience_ = &experience;
    bHasChanged_ = true;
}
 
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::SetTiredness
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_HumanFactors::SetTiredness( const PHY_Tiredness& tiredness )
{
    if( tiredness == *pTiredness_ )
        return;

    pTiredness_  = &tiredness;
    bHasChanged_ = true;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::ModifyMaxSpeed
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RolePion_HumanFactors::ModifyMaxSpeed( MT_Float rSpeed ) const
{
    return rSpeed * pExperience_->GetCoefMaxSpeedModificator() * pTiredness_->GetCoefMaxSpeedModificator();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::ModifyReloadingDuration
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RolePion_HumanFactors::ModifyReloadingDuration( MT_Float rDuration ) const
{
    return rDuration * pExperience_->GetCoefReloadingTimeModificator() * pTiredness_->GetCoefReloadingTimeModificator();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::ModifyPH
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RolePion_HumanFactors::ModifyPH( MT_Float rPH ) const
{
    return rPH * pExperience_->GetCoefPhModificator() * pTiredness_->GetCoefPhModificator();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::ModifyPostureTime
// Created: NLD 2004-11-29
inline
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_HumanFactors::ModifyPostureTime( MT_Float rTime ) const
{
    return rTime * pExperience_->GetCoefPostureTimeModificator() * pTiredness_->GetCoefPostureTimeModificator();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::GetSensorDistanceModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RolePion_HumanFactors::GetSensorDistanceModificator() const
{
    return pExperience_->GetCoefSensorDistanceModificator() * pTiredness_->GetCoefSensorDistanceModificator();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::GetMorale
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
inline
const PHY_Morale& PHY_RolePion_HumanFactors::GetMorale() const
{
    assert( pMorale_ );
    return *pMorale_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::GetExperience
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
inline
const PHY_Experience& PHY_RolePion_HumanFactors::GetExperience() const
{
    assert( pExperience_ );
    return *pExperience_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::GetTiredness
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
inline
const PHY_Tiredness& PHY_RolePion_HumanFactors::GetTiredness() const
{
    assert( pTiredness_ );
    return *pTiredness_;
}

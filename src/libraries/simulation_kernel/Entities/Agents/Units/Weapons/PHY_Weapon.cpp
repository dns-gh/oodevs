// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/entities/agents/units/weapons/PHY_Weapon.cpp $
// $Author: Jvt $
// $Modtime: 2/05/05 17:37 $
// $Revision: 12 $
// $Workfile: PHY_Weapon.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_Weapon.h"

#include "PHY_WeaponType.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Entities/Agents/Roles/NBC/PHY_RolePion_NBC.h"
#include "Entities/Agents/Roles/Communications/PHY_RolePion_Communications.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RolePion_HumanFactors.h"
#include "Entities/Agents/Roles/Population/PHY_RolePion_Population.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectFire_ABC.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Entities/Populations/MIL_PopulationType.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Effects/MIL_Effect_IndirectFire.h"
#include "MIL_AgentServer.h"

// -----------------------------------------------------------------------------
// Name: PHY_Weapon constructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_Weapon::PHY_Weapon( const PHY_WeaponType& type, bool bMajor )
    : type_                   ( type )
    , bMajor_                 ( bMajor )
    , nNbrAmmoFiredFromLoader_( 0 )
    , rNextTimeStepToFire_    ( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_Weapon destructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_Weapon::~PHY_Weapon()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_Weapon::CanDirectFire
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
bool PHY_Weapon::CanDirectFire() const
{
    return type_.CanDirectFire();
}

// -----------------------------------------------------------------------------
// Name: PHY_Weapon::CanIndirectFire
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
bool PHY_Weapon::CanIndirectFire() const
{
    return type_.CanIndirectFire();
}

// -----------------------------------------------------------------------------
// Name: PHY_Weapon::GetDotationCategory
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
const PHY_DotationCategory& PHY_Weapon::GetDotationCategory() const
{
    return type_.GetDotationCategory();
}

// -----------------------------------------------------------------------------
// Name: PHY_Weapon::GetMaxRangeToFireOn
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
MT_Float PHY_Weapon::GetMaxRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const
{
    return type_.GetMaxRangeToFireOn( firer, targetComposanteType, rWantedPH );
}

// -----------------------------------------------------------------------------
// Name: PHY_Weapon::GetMinRangeToFireOn
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
MT_Float PHY_Weapon::GetMinRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const
{
    return type_.GetMinRangeToFireOn( firer, targetComposanteType, rWantedPH );
}

// -----------------------------------------------------------------------------
// Name: PHY_Weapon::GetDangerosity
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
MT_Float PHY_Weapon::GetDangerosity( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& compTarget, MT_Float rDistBtwFirerAndTarget ) const
{
    return type_.GetDangerosity( firer, compTarget, rDistBtwFirerAndTarget );
}

// -----------------------------------------------------------------------------
// Name: PHY_Weapon::GetDangerosity
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
MT_Float PHY_Weapon::GetDangerosity( const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, bool bUsePH ) const
{
    return type_.GetDangerosity( firer, target, targetComposanteType, bUsePH );
}

// -----------------------------------------------------------------------------
// Name: PHY_Weapon::IsReady
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
bool PHY_Weapon::IsReady() const
{
    return (uint)rNextTimeStepToFire_ <= MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
}

// -----------------------------------------------------------------------------
// Name: PHY_Weapon::GetMaxRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
MT_Float PHY_Weapon::GetMaxRangeToIndirectFire() const
{
    return type_.GetMaxRangeToIndirectFire();
}

// -----------------------------------------------------------------------------
// Name: phy_weapon::GetMinRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
MT_Float PHY_Weapon::GetMinRangeToIndirectFire() const
{
    return type_.GetMinRangeToIndirectFire();
}

// -----------------------------------------------------------------------------
// Name: PHY_Weapon::ModifyReloadingDuration
// Created: NLD 2004-11-04
// -----------------------------------------------------------------------------
inline
MT_Float PHY_Weapon::ModifyReloadingDuration( const MIL_AgentPion& firer, MT_Float rDuration ) const
{
    rDuration = firer.GetRole< PHY_RolePion_NBC            >().ModifyReloadingDuration( rDuration );
    rDuration = firer.GetRole< PHY_RolePion_Communications >().ModifyReloadingDuration( rDuration );
    rDuration = firer.GetRole< PHY_RolePion_HumanFactors   >().ModifyReloadingDuration( rDuration );
    rDuration = firer.GetRole< PHY_RolePion_Population     >().ModifyReloadingDuration( rDuration );
    return rDuration;
}

// -----------------------------------------------------------------------------
// Name: PHY_Weapon::DirectFire
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
bool PHY_Weapon::DirectFire( MIL_AgentPion& firer, MIL_Agent_ABC& target, PHY_Composante_ABC& compTarget, PHY_FireResults_ABC& fireResult, bool bUsePH )
{
    assert( type_.CanDirectFire() && IsReady() );

          bool bHasFired        = false;
    const uint nCurrentTimeStep = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    const uint nNextTimeStep    = nCurrentTimeStep + 1;
    if( rNextTimeStepToFire_ < (float)nCurrentTimeStep )
        rNextTimeStepToFire_ = nCurrentTimeStep;
        
    while( (uint)rNextTimeStepToFire_ < nNextTimeStep )
    {
        uint nNbrAmmoToFire = type_.GetNbrAmmoPerBurst();
        
        if( type_.GetNbrAmmoPerLoader() != 0 )
            nNbrAmmoToFire = std::min( nNbrAmmoToFire, type_.GetNbrAmmoPerLoader() - nNbrAmmoFiredFromLoader_ );               

        assert( nNbrAmmoToFire > 0 );
       
        uint nNbrAmmoReserved = (uint)firer.GetRole< PHY_RolePion_Dotations >().AddFireReservation( type_.GetDotationCategory(), nNbrAmmoToFire );
        if( nNbrAmmoReserved )
        {
            nNbrAmmoFiredFromLoader_ += nNbrAmmoReserved;

            type_.DirectFire( firer, target, compTarget, fireResult, bUsePH );
            bHasFired = true;

            rNextTimeStepToFire_ += type_.GetBurstDuration();
            if( nNbrAmmoFiredFromLoader_ == type_.GetNbrAmmoPerLoader() )
            {
                rNextTimeStepToFire_ += ModifyReloadingDuration( firer, type_.GetReloadingDuration() );
                nNbrAmmoFiredFromLoader_  = 0;
            }
        }

        if( nNbrAmmoReserved < nNbrAmmoToFire ) // Soutes vide
            break;
    }
    return bHasFired;
}

// -----------------------------------------------------------------------------
// Name: PHY_Weapon::DirectFire
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
bool PHY_Weapon::DirectFire( MIL_AgentPion& firer, MIL_PopulationElement_ABC& target, PHY_FireResults_ABC& fireResult )
{
    assert( type_.CanDirectFire() && IsReady() );

    const uint nCurrentTimeStep = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    const uint nNextTimeStep    = nCurrentTimeStep + 1;
    if( rNextTimeStepToFire_ < (float)nCurrentTimeStep )
        rNextTimeStepToFire_ = nCurrentTimeStep;
        
    if( (uint)rNextTimeStepToFire_ < nNextTimeStep )
    {
        const PHY_RoePopulation& roe  = firer.GetRole< DEC_RolePion_Decision >().GetRoePopulation();
        const MT_Float rDamageSurface = target.GetPopulation().GetType().GetDamageSurface( roe );
        const uint     nKilledHumans  = (uint)ceil( rDamageSurface * target.GetDensity() );

        uint nNbrAmmoToFire = (uint)firer.GetRole< PHY_RolePion_Dotations >().AddFireReservation( type_.GetDotationCategory(), nKilledHumans );

        type_.DirectFire( firer, target, nNbrAmmoToFire, fireResult );

        rNextTimeStepToFire_ += type_.GetBurstDuration();
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_Weapon::IndirectFire
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
bool PHY_Weapon::IndirectFire( MIL_AgentPion& firer, MIL_Effect_IndirectFire& effect )
{
    assert( type_.CanIndirectFire() && IsReady() );
    assert( type_.GetDotationCategory() == effect.GetIndirectDotationCategory().GetDotationCategory() );
    
    bool bHasFired = false;
    const uint nCurrentTimeStep = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    const uint nNextTimeStep    = nCurrentTimeStep + 1;
    if( rNextTimeStepToFire_ < (float)nCurrentTimeStep )
        rNextTimeStepToFire_ = nCurrentTimeStep;
        
    while( (uint)rNextTimeStepToFire_ < nNextTimeStep && !effect.IsInterventionTypeFired() )
    {
        uint nNbrAmmoToFire = type_.GetNbrAmmoPerBurst();
        if( type_.GetNbrAmmoPerLoader() != 0 )
            nNbrAmmoToFire = std::min( nNbrAmmoToFire, type_.GetNbrAmmoPerLoader() - nNbrAmmoFiredFromLoader_ );       
        
        nNbrAmmoToFire = std::min( nNbrAmmoToFire, effect.GetNbrAmmoToCompleteInterventionType() );
        assert( nNbrAmmoToFire > 0 );
               
        uint nNbrAmmoReserved = (uint)firer.GetRole< PHY_RolePion_Dotations >().AddFireReservation( type_.GetDotationCategory(), nNbrAmmoToFire );
        if( nNbrAmmoReserved )
        {
            nNbrAmmoFiredFromLoader_ += nNbrAmmoReserved;

            type_.IndirectFire( firer, effect, nNbrAmmoReserved );
            bHasFired = true;

            rNextTimeStepToFire_ += type_.GetBurstDuration();
            if( nNbrAmmoFiredFromLoader_ == type_.GetNbrAmmoPerLoader() )
            {
                rNextTimeStepToFire_ += ModifyReloadingDuration( firer, type_.GetReloadingDuration() );
                nNbrAmmoFiredFromLoader_  = 0;
            }
        }

        if( nNbrAmmoReserved < nNbrAmmoToFire ) // Soutes vide
            break;
    }
    return bHasFired;
 }

// -----------------------------------------------------------------------------
// Name: PHY_Weapon::ThrowSmoke
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
void PHY_Weapon::ThrowSmoke( MIL_AgentPion& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, uint nNbrAmmo, PHY_FireResults_ABC& fireResult ) const
{
    uint nNbrAmmoReserved = (uint)firer.GetRole< PHY_RolePion_Dotations >().AddFireReservation( type_.GetDotationCategory(), nNbrAmmo );
    assert( nNbrAmmoReserved == nNbrAmmo );
    type_.ThrowSmoke( firer, vSourcePosition, vTargetPosition, nNbrAmmo, fireResult );
}

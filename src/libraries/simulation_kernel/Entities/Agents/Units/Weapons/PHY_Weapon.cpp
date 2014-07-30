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

#include "simulation_kernel_pch.h"
#include "PHY_Weapon.h"
#include "PHY_WeaponType.h"
#include "MIL_Time_ABC.h"
#include "DefaultWeaponReloadingComputer.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectFire_ABC.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Entities/Populations/MIL_PopulationType.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Effects/MIL_Effect_IndirectFire.h"

// -----------------------------------------------------------------------------
// Name: PHY_Weapon constructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_Weapon::PHY_Weapon( const MIL_Time_ABC& time, const PHY_WeaponType& type, bool bMajor )
    : time_( time )
    , type_( type )
    , bMajor_( bMajor )
    , nNbrAmmoFiredFromLoader_( 0 )
    , rNextTimeStepToFire_( time_.GetCurrentTimeStep() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Weapon destructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_Weapon::~PHY_Weapon()
{
    // NOTHING
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
// Name: PHY_Weapon::GetNumberOfDotationPerBurst
// Created: LDC 2011-06-23
// -----------------------------------------------------------------------------
int PHY_Weapon::GetNumberOfDotationPerBurst( const PHY_DotationCategory& dotation ) const
{
    if( dotation == GetDotationCategory() )
        return type_.GetNbrAmmoPerBurst();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_Weapon::GetDangerosity
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
double PHY_Weapon::GetDangerosity( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& compTarget, double rDistBtwFirerAndTarget, bool bUseAmmo ) const
{
    return type_.GetDangerosity( firer, compTarget, rDistBtwFirerAndTarget, bUseAmmo );
}

// -----------------------------------------------------------------------------
// Name: PHY_Weapon::GetDangerosity
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
double PHY_Weapon::GetDangerosity( const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, bool bUsePH, bool bUseAmmo ) const
{
    return type_.GetDangerosity( firer, target, targetComposanteType, bUsePH, bUseAmmo );
}

// -----------------------------------------------------------------------------
// Name: PHY_Weapon::IsReady
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
bool PHY_Weapon::IsReady() const
{
    return static_cast< unsigned int >( rNextTimeStepToFire_ ) <= time_.GetCurrentTimeStep();
}

// -----------------------------------------------------------------------------
// Name: PHY_Weapon::GetMaxRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
double PHY_Weapon::GetMaxRangeToIndirectFire() const
{
    return type_.GetMaxRangeToIndirectFire();
}

// -----------------------------------------------------------------------------
// Name: phy_weapon::GetMinRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
double PHY_Weapon::GetMinRangeToIndirectFire() const
{
    return type_.GetMinRangeToIndirectFire();
}

// -----------------------------------------------------------------------------
// Name: PHY_Weapon::ModifyReloadingDuration
// Created: NLD 2004-11-04
// -----------------------------------------------------------------------------
double PHY_Weapon::ModifyReloadingDuration( MIL_Agent_ABC& firer, double rDuration ) const
{
    firing::DefaultWeaponReloadingComputer computer( rDuration );
    firer.Execute< firing::WeaponReloadingComputer_ABC >( computer );
    return computer.GetDuration();
}

// -----------------------------------------------------------------------------
// Name: PHY_Weapon::DirectFire
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
bool PHY_Weapon::DirectFire( MIL_AgentPion& firer, MIL_Agent_ABC& target, PHY_Composante_ABC& compTarget, PHY_FireResults_ABC& fireResult, bool bUsePH )
{
    assert( type_.CanDirectFire() && IsReady() );

    bool bHasFired = false;
    const unsigned int nCurrentTimeStep = time_.GetCurrentTimeStep();
    const unsigned int nNextTimeStep = nCurrentTimeStep + 1;
    if( rNextTimeStepToFire_ < static_cast< float >( nCurrentTimeStep ) )
        rNextTimeStepToFire_ = nCurrentTimeStep;

    while( static_cast< unsigned int >( rNextTimeStepToFire_ ) < nNextTimeStep )
    {
        unsigned int nNbrAmmoToFire = type_.GetNbrAmmoPerBurst();

        if( type_.GetNbrAmmoPerLoader() != 0 )
            nNbrAmmoToFire = std::min( nNbrAmmoToFire, type_.GetNbrAmmoPerLoader() - nNbrAmmoFiredFromLoader_ );

        assert( nNbrAmmoToFire > 0 );

        unsigned int nNbrAmmoReserved = static_cast< unsigned int >( firer.GetRole< dotation::PHY_RoleInterface_Dotations >().AddFireReservation( type_.GetDotationCategory(), nNbrAmmoToFire ) );

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

    const unsigned int nCurrentTimeStep = time_.GetCurrentTimeStep();
    const unsigned int nNextTimeStep = nCurrentTimeStep + 1;
    if( rNextTimeStepToFire_ < static_cast< float >( nCurrentTimeStep ) )
        rNextTimeStepToFire_ = nCurrentTimeStep;

    if( static_cast< unsigned int >( rNextTimeStepToFire_ ) < nNextTimeStep )
    {
        const PHY_RoePopulation& roe = firer.GetRole< DEC_RolePion_Decision >().GetRoePopulation();
        const double rDamageSurface = target.GetPopulation().GetType().GetDamageSurface( roe );
        const unsigned int nKilledHumans = static_cast< unsigned int >( ceil( rDamageSurface * target.GetDensity() ) );

        unsigned int nNbrAmmoToFire = static_cast< unsigned int >( firer.GetRole< dotation::PHY_RoleInterface_Dotations >().AddFireReservation( type_.GetDotationCategory(), nKilledHumans ) );

        type_.DirectFire( firer, target, nNbrAmmoToFire, fireResult );

        rNextTimeStepToFire_ += type_.GetBurstDuration();
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_Weapon::IndirectFire
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
bool PHY_Weapon::IndirectFire( MIL_Agent_ABC& firer, MIL_Effect_IndirectFire& effect )
{
    assert( type_.CanIndirectFire() && IsReady() );
    assert( type_.GetDotationCategory() == effect.GetIndirectDotationCategory().GetDotationCategory() );

    bool bHasFired = false;
    const unsigned int nCurrentTimeStep = time_.GetCurrentTimeStep();
    const unsigned int nNextTimeStep = nCurrentTimeStep + 1;
    if( rNextTimeStepToFire_ < static_cast< float >( nCurrentTimeStep ) )
        rNextTimeStepToFire_ = nCurrentTimeStep;

    while( static_cast< unsigned int >( rNextTimeStepToFire_ ) < nNextTimeStep && !effect.IsInterventionTypeFired() )
    {
        unsigned int nNbrAmmoToFire = type_.GetNbrAmmoPerBurst();
        if( type_.GetNbrAmmoPerLoader() != 0 )
            nNbrAmmoToFire = std::min( nNbrAmmoToFire, type_.GetNbrAmmoPerLoader() - nNbrAmmoFiredFromLoader_ );

        nNbrAmmoToFire = std::min( nNbrAmmoToFire, effect.GetNbrAmmoToCompleteInterventionType() );
        assert( nNbrAmmoToFire > 0 );

        unsigned int nNbrAmmoReserved = static_cast< unsigned int >( firer.GetRole< dotation::PHY_RoleInterface_Dotations >().AddFireReservation( type_.GetDotationCategory(), nNbrAmmoToFire ) );
        if( nNbrAmmoReserved )
        {
            nNbrAmmoFiredFromLoader_ += nNbrAmmoReserved;

            type_.IndirectFire( effect, nNbrAmmoReserved );
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
double PHY_Weapon::ThrowSmoke( MIL_Agent_ABC& firer, const MT_Vector2D& vTargetPosition, PHY_FireResults_ABC& fireResult )
{
    const unsigned int nCurrentTimeStep = time_.GetCurrentTimeStep();
    const unsigned int nNextTimeStep = nCurrentTimeStep + 1;
    if( rNextTimeStepToFire_ < static_cast< float >( nCurrentTimeStep ) )
        rNextTimeStepToFire_ = nCurrentTimeStep;
    while( static_cast< unsigned int >( rNextTimeStepToFire_ ) < nNextTimeStep )
    {
        unsigned int nNbrAmmoToFire = type_.GetNbrAmmoPerBurst();
        if( type_.GetNbrAmmoPerLoader() != 0 )
            nNbrAmmoToFire = std::min( nNbrAmmoToFire, type_.GetNbrAmmoPerLoader() - nNbrAmmoFiredFromLoader_ );

        const MT_Vector2D& vSourcePosition = firer.GetRole< PHY_RoleInterface_Location >().GetPosition();
        unsigned int nNbrAmmoReserved = static_cast< unsigned int >( firer.GetRole< dotation::PHY_RoleInterface_Dotations >().AddFireReservation( type_.GetDotationCategory(), nNbrAmmoToFire ) );
        if( nNbrAmmoReserved )
        {
            nNbrAmmoFiredFromLoader_ += nNbrAmmoReserved;

            type_.ThrowSmoke( firer, vSourcePosition, vTargetPosition, nNbrAmmoReserved, fireResult );
            rNextTimeStepToFire_ += type_.GetBurstDuration();
            if( nNbrAmmoFiredFromLoader_ == type_.GetNbrAmmoPerLoader() )
            {
                rNextTimeStepToFire_ += ModifyReloadingDuration( firer, type_.GetReloadingDuration() );
                nNbrAmmoFiredFromLoader_  = 0;
            }
        }
        else
            break;
    }
    return type_.GetSmokeDuration();
}

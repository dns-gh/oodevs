// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Weapon.h"
#include "WeaponType.h"
#include <wrapper/Effect.h>
#include <wrapper/Hook.h>
#include <wrapper/View.h>

using namespace sword;
using namespace sword::fire;

DECLARE_HOOK( GetWeaponReloadingDuration, double, ( const SWORD_Model* firer, double rDuration ) )

// -----------------------------------------------------------------------------
// Name: Weapon constructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
Weapon::Weapon( const wrapper::View& model, const wrapper::View& weapon )
    : model_ ( model )
    , weapon_( weapon )
    , type_  ( WeaponType::FindWeaponType( weapon[ "type" ] ) )
{
     if( ! type_ )
         throw std::invalid_argument( "Unknown weapon type : " + weapon[ "type" ] );
}

// -----------------------------------------------------------------------------
// Name: Weapon::GetDangerosity
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
double Weapon::GetDangerosity( const wrapper::View& firer, const wrapper::View& target, const wrapper::View& compTarget, bool bUsePH, bool checkAmmo ) const
{
    return type_->GetDangerosity( firer, target, compTarget, bUsePH, checkAmmo );
}

// -----------------------------------------------------------------------------
// Name: Weapon::IsReady
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
bool Weapon::IsReady() const
{
    const std::size_t tick = model_[ "tick" ];
    return weapon_[ "next-time" ] <= tick; // $$$$ MCO 2012-08-28: this is buggy because next-time should be changed synchronously when firing
}

// -----------------------------------------------------------------------------
// Name: Weapon::ModifyReloadingDuration
// Created: NLD 2004-11-04
// -----------------------------------------------------------------------------
double Weapon::ModifyReloadingDuration( const wrapper::View& firer, double rDuration ) const
{
    return GET_HOOK( GetWeaponReloadingDuration )( firer, rDuration );
}

// -----------------------------------------------------------------------------
// Name: Weapon::DirectFire
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void Weapon::DirectFire( const wrapper::View& firer, const wrapper::View& target, const wrapper::View& compTarget, bool bUsePH ) const
{
    assert( IsReady() );
    const unsigned int nCurrentTimeStep = model_[ "tick" ];
    const unsigned int nNextTimeStep = nCurrentTimeStep + 1;
    double rNextTimeStepToFire = weapon_[ "next-time" ]; // $$$$ MCO 2012-08-28: this is buggy because next-time should be changed synchronously when firing
    if( rNextTimeStepToFire < (float)nCurrentTimeStep )
        rNextTimeStepToFire = nCurrentTimeStep;
    unsigned int nNbrAmmoFiredFromLoader = weapon_[ "fired-ammo" ]; // $$$$ MCO 2012-08-28: this is buggy because fired-ammo should be changed synchronously when firing
    while( (unsigned int)rNextTimeStepToFire < nNextTimeStep )
    {
        unsigned int nNbrAmmoToFire = type_->GetNbrAmmoPerBurst();
        if( type_->GetNbrAmmoPerLoader() != 0 )
            nNbrAmmoToFire = std::min( nNbrAmmoToFire, type_->GetNbrAmmoPerLoader() - nNbrAmmoFiredFromLoader );
        assert( nNbrAmmoToFire > 0 );
        unsigned int nNbrAmmoReserved = type_->ReserveAmmunition( firer, nNbrAmmoToFire );
        if( nNbrAmmoReserved )
        {
            nNbrAmmoFiredFromLoader += nNbrAmmoReserved;
            type_->DirectFire( firer, target, compTarget, bUsePH );
            rNextTimeStepToFire += type_->GetBurstDuration();
            if( nNbrAmmoFiredFromLoader == type_->GetNbrAmmoPerLoader() )
            {
                rNextTimeStepToFire += ModifyReloadingDuration( firer, type_->GetReloadingDuration() );
                nNbrAmmoFiredFromLoader  = 0;
            }
        }
        if( nNbrAmmoReserved < nNbrAmmoToFire ) // Soutes vide
            break;
    }
    {
        wrapper::Effect effect( weapon_ );
        effect[ "fired-ammo" ] = nNbrAmmoFiredFromLoader; // $$$$ MCO 2012-08-28: this is buggy because fired-ammo should be changed synchronously when firing
        effect.Post();
    }
    {
        wrapper::Effect effect( weapon_ );
        effect[ "next-time" ] = rNextTimeStepToFire; // $$$$ MCO 2012-08-28: this is buggy because next-time should be changed synchronously when firing
        effect.Post();
    }
}

//// -----------------------------------------------------------------------------
//// Name: Weapon::DirectFire
//// Created: NLD 2004-10-06
//// -----------------------------------------------------------------------------
//bool Weapon::DirectFire( MIL_AgentPion& firer, MIL_PopulationElement_ABC& target )
//{
//    assert( type_->CanDirectFire() && IsReady() );
//
//    const unsigned int nCurrentTimeStep = time_.GetCurrentTick();
//    const unsigned int nNextTimeStep    = nCurrentTimeStep + 1;
//    if( rNextTimeStepToFire_ < (float)nCurrentTimeStep )
//        rNextTimeStepToFire_ = nCurrentTimeStep;
//
//    if( (unsigned int)rNextTimeStepToFire_ < nNextTimeStep )
//    {
//        const PHY_RoePopulation& roe  = firer.GetRole< DEC_RolePion_Decision >().GetRoePopulation();
//        const double rDamageSurface = target.GetPopulation().GetType().GetDamageSurface( roe );
//        const unsigned int     nKilledHumans  = (unsigned int)ceil( rDamageSurface * target.GetDensity() );
//
//        unsigned int nNbrAmmoToFire = (unsigned int)firer.GetRole< dotation::PHY_RoleInterface_Dotations >().AddFireReservation( type_->GetDotationCategory(), nKilledHumans );
//
//        type_->DirectFire( firer, target, nNbrAmmoToFire, fireResult );
//
//        rNextTimeStepToFire_ += type_->GetBurstDuration();
//    }
//    return true;
//}
//
//// -----------------------------------------------------------------------------
//// Name: Weapon::IndirectFire
//// Created: NLD 2004-10-11
//// -----------------------------------------------------------------------------
//bool Weapon::IndirectFire( MIL_Agent_ABC& firer, MIL_Effect_IndirectFire& effect )
//{
//    assert( type_->CanIndirectFire() && IsReady() );
//    assert( type_->GetDotationCategory() == effect.GetIndirectDotationCategory().GetDotationCategory() );
//
//    bool bHasFired = false;
//    const unsigned int nCurrentTimeStep = time_.GetCurrentTick();
//    const unsigned int nNextTimeStep    = nCurrentTimeStep + 1;
//    if( rNextTimeStepToFire_ < (float)nCurrentTimeStep )
//        rNextTimeStepToFire_ = nCurrentTimeStep;
//
//    while( (unsigned int)rNextTimeStepToFire_ < nNextTimeStep && !effect.IsInterventionTypeFired() )
//    {
//        unsigned int nNbrAmmoToFire = type_->GetNbrAmmoPerBurst();
//        if( type_->GetNbrAmmoPerLoader() != 0 )
//            nNbrAmmoToFire = std::min( nNbrAmmoToFire, type_->GetNbrAmmoPerLoader() - nNbrAmmoFiredFromLoader_ );
//
//        nNbrAmmoToFire = std::min( nNbrAmmoToFire, effect.GetNbrAmmoToCompleteInterventionType() );
//        assert( nNbrAmmoToFire > 0 );
//
//        unsigned int nNbrAmmoReserved = (unsigned int)firer.GetRole< dotation::PHY_RoleInterface_Dotations >().AddFireReservation( type_->GetDotationCategory(), nNbrAmmoToFire );
//        if( nNbrAmmoReserved )
//        {
//            nNbrAmmoFiredFromLoader_ += nNbrAmmoReserved;
//
//            type_->IndirectFire( effect, nNbrAmmoReserved );
//            bHasFired = true;
//
//            rNextTimeStepToFire_ += type_->GetBurstDuration();
//            if( nNbrAmmoFiredFromLoader_ == type_->GetNbrAmmoPerLoader() )
//            {
//                rNextTimeStepToFire_ += ModifyReloadingDuration( firer, type_->GetReloadingDuration() );
//                nNbrAmmoFiredFromLoader_  = 0;
//            }
//        }
//
//        if( nNbrAmmoReserved < nNbrAmmoToFire ) // Soutes vide
//            break;
//    }
//    return bHasFired;
// }
//
//// -----------------------------------------------------------------------------
//// Name: Weapon::ThrowSmoke
//// Created: NLD 2004-10-21
//// -----------------------------------------------------------------------------
//void Weapon::ThrowSmoke( MIL_Agent_ABC& firer, const MT_Vector2D& vTargetPosition, unsigned int nNbrAmmo ) const
//{//@TODO MGD See with AHC if we remove this GetRole kind
//    const MT_Vector2D& vSourcePosition = firer.GetRole< PHY_RoleInterface_Location >().GetPosition();
//    unsigned int nNbrAmmoReserved = (unsigned int)firer.GetRole< dotation::PHY_RoleInterface_Dotations >().AddFireReservation( type_->GetDotationCategory(), nNbrAmmo );
//    assert( nNbrAmmoReserved == nNbrAmmo );
//    type_->ThrowSmoke( firer, vSourcePosition, vTargetPosition, nNbrAmmo, fireResult );
//}

// -----------------------------------------------------------------------------
// Name: Weapon::operator==
// Created: MCO 2012-06-14
// -----------------------------------------------------------------------------
bool Weapon::operator==( const Weapon& rhs ) const
{
    return weapon_ == rhs.weapon_;
}

// -----------------------------------------------------------------------------
// Name: Weapon::CanDirectFire
// Created: MCO 2012-06-21
// -----------------------------------------------------------------------------
bool Weapon::CanDirectFire( const wrapper::View& component, int nComposanteFiringType, int ammoDotationClass ) const
{
    return type_->CanDirectFire( component, nComposanteFiringType, ammoDotationClass );
}

// -----------------------------------------------------------------------------
// Name: Weapon::CanIndirectFire
// Created: MCO 2012-06-27
// -----------------------------------------------------------------------------
bool Weapon::CanIndirectFire( const wrapper::View& entity, const wrapper::View& component, const std::string& type, const MT_Vector2D* target ) const
{
    return type_->CanIndirectFire( entity, component, type, target );
}

// -----------------------------------------------------------------------------
// Name: Weapon::HasDotation
// Created: MCO 2012-06-21
// -----------------------------------------------------------------------------
bool Weapon::HasDotation( const wrapper::View& firer ) const
{
    return type_->HasDotation( firer );
}

// -----------------------------------------------------------------------------
// Name: Weapon::GetDotation
// Created: MCO 2012-06-28
// -----------------------------------------------------------------------------
const DotationCategory& Weapon::GetDotation() const
{
    return type_->GetDotation();
}

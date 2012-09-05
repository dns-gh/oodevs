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
#include "ModuleFacade.h"
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
Weapon::Weapon( ModuleFacade& module, const wrapper::View& model, const wrapper::View& weapon )
    : module_( &module )
    , model_ ( model )
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
    return module_->weapons[ weapon_ ].first <= tick;
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
    const double current = model_[ "tick" ];
    double next = std::max( current, module_->weapons[ weapon_ ].first );
    unsigned int fired = module_->weapons[ weapon_ ].second;
    while( next <= current )
    {
        unsigned int burst = type_->GetNbrAmmoPerBurst();
        if( type_->GetNbrAmmoPerLoader() != 0 )
            burst = std::min( burst, type_->GetNbrAmmoPerLoader() - fired );
        assert( burst > 0 );
        unsigned int reserved = type_->ReserveAmmunition( firer, burst );
        if( reserved )
        {
            fired += reserved;
            type_->DirectFire( firer, target, compTarget, bUsePH );
            next += type_->GetBurstDuration();
            if( fired == type_->GetNbrAmmoPerLoader() )
            {
                next += ModifyReloadingDuration( firer, type_->GetReloadingDuration() );
                fired = 0;
            }
        }
        if( reserved < burst ) // Soutes vide
            break;
    }
    module_->weapons[ weapon_ ] = std::make_pair( next, fired ); // $$$$ MCO 2012-08-30: yes we maintain a module global state, and no it's not that great...
}

//// -----------------------------------------------------------------------------
//// Name: Weapon::DirectFire
//// Created: NLD 2004-10-06
//// -----------------------------------------------------------------------------
//bool Weapon::DirectFire( MIL_AgentPion& firer, MIL_PopulationElement_ABC& target )
//{
//    assert( type_->CanDirectFire() && IsReady() );
//
//    const unsigned int current = time_.GetCurrentTick();
//    const unsigned int nNextTimeStep    = current + 1;
//    if( rNextTimeStepToFire_ < (float)current )
//        rNextTimeStepToFire_ = current;
//
//    if( (unsigned int)rNextTimeStepToFire_ < nNextTimeStep )
//    {
//        const PHY_RoePopulation& roe  = firer.GetRole< DEC_RolePion_Decision >().GetRoePopulation();
//        const double rDamageSurface = target.GetPopulation().GetType().GetDamageSurface( roe );
//        const unsigned int     nKilledHumans  = (unsigned int)ceil( rDamageSurface * target.GetDensity() );
//
//        unsigned int burst = (unsigned int)firer.GetRole< dotation::PHY_RoleInterface_Dotations >().AddFireReservation( type_->GetDotationCategory(), nKilledHumans );
//
//        type_->DirectFire( firer, target, burst, fireResult );
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
//    const unsigned int current = time_.GetCurrentTick();
//    const unsigned int nNextTimeStep    = current + 1;
//    if( rNextTimeStepToFire_ < (float)current )
//        rNextTimeStepToFire_ = current;
//
//    while( (unsigned int)rNextTimeStepToFire_ < nNextTimeStep && !effect.IsInterventionTypeFired() )
//    {
//        unsigned int burst = type_->GetNbrAmmoPerBurst();
//        if( type_->GetNbrAmmoPerLoader() != 0 )
//            burst = std::min( burst, type_->GetNbrAmmoPerLoader() - nNbrAmmoFiredFromLoader_ );
//
//        burst = std::min( burst, effect.GetNbrAmmoToCompleteInterventionType() );
//        assert( burst > 0 );
//
//        unsigned int reserved = (unsigned int)firer.GetRole< dotation::PHY_RoleInterface_Dotations >().AddFireReservation( type_->GetDotationCategory(), burst );
//        if( reserved )
//        {
//            nNbrAmmoFiredFromLoader_ += reserved;
//
//            type_->IndirectFire( effect, reserved );
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
//        if( reserved < burst ) // Soutes vide
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
//    unsigned int reserved = (unsigned int)firer.GetRole< dotation::PHY_RoleInterface_Dotations >().AddFireReservation( type_->GetDotationCategory(), nNbrAmmo );
//    assert( reserved == nNbrAmmo );
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

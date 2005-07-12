// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Effects/MIL_Effect_IndirectFire.inl $
// $Author: Jvt $
// $Modtime: 2/05/05 17:10 $
// $Revision: 4 $
// $Workfile: MIL_Effect_IndirectFire.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::GetWeaponDotationCategory
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
inline
const PHY_DotationCategory* MIL_Effect_IndirectFire::GetWeaponDotationCategory() const
{
    return pWeaponDotationCategory_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::SetWeaponDotationCategory
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
inline
void MIL_Effect_IndirectFire::SetWeaponDotationCategory( const PHY_DotationCategory& dotationCategory )
{
    assert( !pWeaponDotationCategory_ );
    pWeaponDotationCategory_ = &dotationCategory;
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::IsTargetValid
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
inline
bool MIL_Effect_IndirectFire::IsTargetValid() const
{
    return vTargetPosition_.rX_ != -1. && vTargetPosition_.rY_ != -1.;
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::IsInterventionTypeFired
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
inline
bool MIL_Effect_IndirectFire::IsInterventionTypeFired() const
{
    return bIsFlying_ || rInterventionTypeToFire_ == 0.;
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::IncRef
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
inline
void MIL_Effect_IndirectFire::IncRef()
{
    ++nNbrRefs_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::DecRef
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
inline
void MIL_Effect_IndirectFire::DecRef()
{
    assert( nNbrRefs_ > 0 );
    if( --nNbrRefs_ == 0 )
        delete this;
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::ForceFlying
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
inline
void MIL_Effect_IndirectFire::ForceFlying()
{
    bIsFlying_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::GetNbrAmmoFired
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
inline
uint MIL_Effect_IndirectFire::GetNbrAmmoFired() const
{
    return nNbrAmmoFired_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::GetFlyingDistance
/** @return 
*/
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
inline
MT_Float MIL_Effect_IndirectFire::GetFlyingDistance() const
{
    return vSourcePosition_.Distance( vTargetPosition_ );
}


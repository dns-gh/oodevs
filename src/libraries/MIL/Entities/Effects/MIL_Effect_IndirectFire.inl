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
// Name: MIL_Effect_IndirectFire::GetIndirectDotationCategory
// Created: NLD 2006-08-07
// -----------------------------------------------------------------------------
inline
const PHY_DotationCategory_IndirectFire_ABC& MIL_Effect_IndirectFire::GetIndirectDotationCategory() const
{
    return indirectDotationCategory_;
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
    return bFired_ || GetNbrAmmoToCompleteInterventionType() == 0;
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
    StartFlying();
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


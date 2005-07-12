// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/IndirectFiring/PHY_IndirectFireData.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: PHY_IndirectFireData.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::HasWeaponsReady
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
inline
bool PHY_IndirectFireData::HasWeaponsReady() const
{
    return bHasWeaponsReady_;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::HasWeaponsNotReady
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
inline
bool PHY_IndirectFireData::HasWeaponsNotReady() const
{
    return bHasWeaponsNotReady_;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::sComposanteWeapons::HasWeaponsNotReady
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
inline
bool PHY_IndirectFireData::sComposanteWeapons::HasWeaponsReady() const
{
    return !weaponsReady_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::IsFiring
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
inline
bool PHY_IndirectFireData::sComposanteWeapons::IsFiring() const
{
    return bIsFiring_;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::sComposanteWeapons::GetUnusedWeapon
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
inline
PHY_Weapon* PHY_IndirectFireData::sComposanteWeapons::GetUnusedWeapon() const
{
    if( weaponsReady_.empty() )
        return 0;
    return weaponsReady_.front();
}
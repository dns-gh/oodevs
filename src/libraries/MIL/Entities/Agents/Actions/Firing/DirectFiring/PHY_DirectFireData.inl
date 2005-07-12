// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Firing/DirectFiring/PHY_DirectFireData.inl $
// $Author: Nld $
// $Modtime: 30/11/04 15:54 $
// $Revision: 3 $
// $Workfile: PHY_DirectFireData.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::HasWeaponsReady
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
inline
bool PHY_DirectFireData::HasWeaponsReady() const
{
    return bHasWeaponsReady_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::HasWeaponsNotReady
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
inline
bool PHY_DirectFireData::HasWeaponsNotReady() const
{
    return bHasWeaponsNotReady_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::HasWeaponsAndNoAmmo
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
inline
bool PHY_DirectFireData::HasWeaponsAndNoAmmo() const
{
    return bHasWeaponsAndNoAmmo_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::sComposanteWeapons::HasWeaponsNotReady
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
inline
bool PHY_DirectFireData::sComposanteWeapons::HasWeaponsReady() const
{
    return !weaponsReady_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::GetNbrWeaponsUsable
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
inline
uint PHY_DirectFireData::sComposanteWeapons::GetNbrWeaponsUsable() const
{
    return weaponsReady_.size();
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::IsFiring
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
inline
bool PHY_DirectFireData::sComposanteWeapons::IsFiring() const
{
    return bIsFiring_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::sComposanteWeapons::GetUnusedWeapon
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
inline
PHY_Weapon* PHY_DirectFireData::sComposanteWeapons::GetUnusedWeapon() const
{
    if( weaponsReady_.empty() )
        return 0;
    return weaponsReady_.front();
}


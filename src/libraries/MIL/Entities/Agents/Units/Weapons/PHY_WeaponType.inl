// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Weapons/PHY_WeaponType.inl $
// $Author: Nld $
// $Modtime: 21/10/04 17:28 $
// $Revision: 2 $
// $Workfile: PHY_WeaponType.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::FindWeaponType
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
inline
const PHY_WeaponType* PHY_WeaponType::FindWeaponType( const std::string& strLauncher, const std::string& strAmmunition )
{
    CIT_WeaponTypeMap it = weaponTypes_.find( std::make_pair( strLauncher, strAmmunition ) );
    if( it == weaponTypes_.end() )
        return 0;
    return it->second;
}


// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetDotationCategory
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
inline
const PHY_DotationCategory& PHY_WeaponType::GetDotationCategory() const
{
    assert( pDotationCategory_ );
    return *pDotationCategory_;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetNbrAmmoPerLoader
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
inline
uint PHY_WeaponType::GetNbrAmmoPerLoader() const
{
    return nNbrAmmoPerLoader_;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetNbrAmmoPerBurst
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
inline
uint PHY_WeaponType::GetNbrAmmoPerBurst() const
{
    return nNbrAmmoPerBurst_;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetBurstDuration
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
inline
MT_Float PHY_WeaponType::GetBurstDuration() const
{
    return rBurstDuration_;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetReloadingDuration
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
inline
MT_Float PHY_WeaponType::GetReloadingDuration() const
{
    return rReloadingDuration_;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetName
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
inline
const std::string& PHY_WeaponType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::CanDirectFire
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
inline
bool PHY_WeaponType::CanDirectFire() const
{
    return pDirectFireData_ != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::CanIndirectFire
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
inline
bool PHY_WeaponType::CanIndirectFire() const
{
    return pIndirectFireData_ != 0;
}


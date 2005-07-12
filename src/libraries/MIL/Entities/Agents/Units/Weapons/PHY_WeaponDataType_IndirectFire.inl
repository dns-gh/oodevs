// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Weapons/PHY_WeaponDataType_IndirectFire.inl $
// $Author: Jvt $
// $Modtime: 2/05/05 13:41 $
// $Revision: 2 $
// $Workfile: PHY_WeaponDataType_IndirectFire.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_IndirectFire::GetAverageSpeed
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
inline
MT_Float PHY_WeaponDataType_IndirectFire::GetAverageSpeed() const
{
    return rAverageSpeed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_IndirectFire::GetMinRange
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
inline
MT_Float PHY_WeaponDataType_IndirectFire::GetMinRange() const
{
    return rMinRange_;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_IndirectFire::GetMaxRange
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
inline
MT_Float PHY_WeaponDataType_IndirectFire::GetMaxRange() const
{
    return rMaxRange_;
}

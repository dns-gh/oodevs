// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_NatureWeapon.inl $
// $Author: Jvt $
// $Modtime: 9/12/04 16:48 $
// $Revision: 2 $
// $Workfile: PHY_NatureWeapon.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_NatureWeapon::Find
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const PHY_NatureWeapon* PHY_NatureWeapon::Find( const std::string& strName )
{
    CIT_NatureWeaponMap it = natureWeapons_.find( strName );
    return it == natureWeapons_.end() ? 0 : it->second;
}


// -----------------------------------------------------------------------------
// Name: PHY_NatureWeapon::GetName
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const std::string& PHY_NatureWeapon::GetName() const
{
    return strName_;
}


// -----------------------------------------------------------------------------
// Name: PHY_NatureWeapon::GetID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
uint PHY_NatureWeapon::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureWeapon::GetAsnID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
ASN1T_EnumUnitNatureWeapon PHY_NatureWeapon::GetAsnID() const
{
    return nAsnID_;
}

// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Pawn/NatureWeapon.inl $
// $Author: Sbo $
// $Modtime: 13/07/05 15:04 $
// $Revision: 2 $
//
// *****************************************************************************

namespace TIC
{

// -----------------------------------------------------------------------------
// Name: NatureWeapon::Find
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const NatureWeapon* NatureWeapon::Find( const std::string& strName )
{
    CIT_NatureWeaponMap it = natureWeapons_.find( strName );
    return it == natureWeapons_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: NatureWeapon::Find
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
const std::string* NatureWeapon::Find( ASN1T_EnumUnitNatureWeapon asnId )
{
    for( CIT_NatureWeaponMap it = natureWeapons_.begin(); it != natureWeapons_.end(); ++it )
        if( ( *it ).second->GetAsnID() == asnId )
            return &( *it ).second->GetName();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: NatureWeapon::GetName
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const std::string& NatureWeapon::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: NatureCategory::GetKey
// Created: SBO 2005-06-03
// -----------------------------------------------------------------------------
inline
const std::string& NatureWeapon::GetKey() const
{
    return strKey_;
}

// -----------------------------------------------------------------------------
// Name: NatureWeapon::GetID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
uint NatureWeapon::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: NatureWeapon::GetAsnID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
ASN1T_EnumUnitNatureWeapon NatureWeapon::GetAsnID() const
{
    return nAsnID_;
}

} // end namespace TIC
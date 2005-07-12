// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_NatureCategory.inl $
// $Author: Jvt $
// $Modtime: 9/12/04 16:48 $
// $Revision: 2 $
// $Workfile: PHY_NatureCategory.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_NatureCategory::Find
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const PHY_NatureCategory* PHY_NatureCategory::Find( const std::string& strName )
{
    CIT_NatureCategoryMap it = natureCategory_.find( strName );
    return it == natureCategory_.end() ? 0 : it->second;
}


// -----------------------------------------------------------------------------
// Name: PHY_NatureCategory::GetName
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const std::string& PHY_NatureCategory::GetName() const
{
    return strName_;
}


// -----------------------------------------------------------------------------
// Name: PHY_NatureCategory::GetID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
uint PHY_NatureCategory::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureCategory::GetAsnID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
ASN1T_EnumUnitNatureCategory PHY_NatureCategory::GetAsnID() const
{
    return nAsnID_;
}

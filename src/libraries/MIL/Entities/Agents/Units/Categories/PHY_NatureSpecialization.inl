// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_NatureSpecialization.inl $
// $Author: Jvt $
// $Modtime: 9/12/04 16:48 $
// $Revision: 2 $
// $Workfile: PHY_NatureSpecialization.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_NatureSpecialization::Find
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const PHY_NatureSpecialization* PHY_NatureSpecialization::Find( const std::string& strName )
{
    CIT_NatureSpecializationMap it = natureSpecialization_.find( strName );
    return it == natureSpecialization_.end() ? 0 : it->second;
}


// -----------------------------------------------------------------------------
// Name: PHY_NatureSpecialization::GetName
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const std::string& PHY_NatureSpecialization::GetName() const
{
    return strName_;
}


// -----------------------------------------------------------------------------
// Name: PHY_NatureSpecialization::GetID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
uint PHY_NatureSpecialization::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureSpecialization::GetAsnID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
ASN1T_EnumUnitNatureSpecialization PHY_NatureSpecialization::GetAsnID() const
{
    return nAsnID_;
}

// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_NatureQualifier.inl $
// $Author: Jvt $
// $Modtime: 9/12/04 16:48 $
// $Revision: 2 $
// $Workfile: PHY_NatureQualifier.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_NatureQualifier::Find
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const PHY_NatureQualifier* PHY_NatureQualifier::Find( const std::string& strName )
{
    CIT_NatureQualifierMap it = natureQualifier_.find( strName );
    return it == natureQualifier_.end() ? 0 : it->second;
}


// -----------------------------------------------------------------------------
// Name: PHY_NatureQualifier::GetName
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const std::string& PHY_NatureQualifier::GetName() const
{
    return strName_;
}


// -----------------------------------------------------------------------------
// Name: PHY_NatureQualifier::GetID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
uint PHY_NatureQualifier::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureQualifier::GetAsnID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
ASN1T_EnumUnitNatureQualifier PHY_NatureQualifier::GetAsnID() const
{
    return nAsnID_;
}

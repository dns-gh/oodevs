// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_NatureMobility.inl $
// $Author: Jvt $
// $Modtime: 9/12/04 16:48 $
// $Revision: 2 $
// $Workfile: PHY_NatureMobility.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_NatureMobility::Find
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const PHY_NatureMobility* PHY_NatureMobility::Find( const std::string& strName )
{
    CIT_NatureMobilityMap it = natureMobility_.find( strName );
    return it == natureMobility_.end() ? 0 : it->second;
}


// -----------------------------------------------------------------------------
// Name: PHY_NatureMobility::GetName
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const std::string& PHY_NatureMobility::GetName() const
{
    return strName_;
}


// -----------------------------------------------------------------------------
// Name: PHY_NatureMobility::GetID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
uint PHY_NatureMobility::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureMobility::GetAsnID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
ASN1T_EnumUnitNatureMobility PHY_NatureMobility::GetAsnID() const
{
    return nAsnID_;
}

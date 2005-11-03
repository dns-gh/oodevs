// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Categories/PHY_Protection.inl $
// $Author: Nld $
// $Modtime: 11/03/05 14:04 $
// $Revision: 2 $
// $Workfile: PHY_Protection.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_Protection::Find
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
inline
const PHY_Protection* PHY_Protection::Find( const std::string& strName )
{
    CIT_ProtectionMap itProtection = protections_.find( strName );
    if( itProtection == protections_.end() )
        return 0;
    return itProtection->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const std::string& PHY_Protection::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::GetID
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
uint PHY_Protection::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::GetNeutralizationTime
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
inline
uint PHY_Protection::GetNeutralizationTime() const
{
    return (uint)( neutralizationTime_.rand() );
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::GetProtections
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const PHY_Protection::T_ProtectionMap& PHY_Protection::GetProtections()
{
    return protections_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::IsHuman
// Created: NLD 2005-03-11
// -----------------------------------------------------------------------------
inline
bool PHY_Protection::IsHuman() const
{
    return nType_ == eHuman;
}

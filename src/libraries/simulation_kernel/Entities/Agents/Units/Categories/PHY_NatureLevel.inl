// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_NatureLevel.inl $
// $Author: Jvt $
// $Modtime: 9/12/04 16:48 $
// $Revision: 2 $
// $Workfile: PHY_NatureLevel.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_NatureLevel::Find
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const PHY_NatureLevel* PHY_NatureLevel::Find( const std::string& strName )
{
    CIT_NatureLevelMap it = natureLevels_.find( strName );
    return it == natureLevels_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: phy_naturelevel::Find
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
inline
const PHY_NatureLevel* PHY_NatureLevel::Find( uint nID )
{
    for( CIT_NatureLevelMap it = natureLevels_.begin(); it != natureLevels_.end(); ++it )
        if( it->second->GetID() == nID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureLevel::GetName
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const std::string& PHY_NatureLevel::GetName() const
{
    return strName_;
}


// -----------------------------------------------------------------------------
// Name: PHY_NatureLevel::GetID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
uint PHY_NatureLevel::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureLevel::GetAsnID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
ASN1T_EnumNatureLevel PHY_NatureLevel::GetAsnID() const
{
    return nAsnID_;
}

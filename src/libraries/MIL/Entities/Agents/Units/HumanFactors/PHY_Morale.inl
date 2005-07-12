// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/HumanFactors/PHY_Morale.inl $
// $Author: Jvt $
// $Modtime: 14/04/05 16:27 $
// $Revision: 4 $
// $Workfile: PHY_Morale.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_Morale::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
inline
const PHY_Morale* PHY_Morale::Find( ASN1T_EnumUnitMoral nAsnID )
{
    CIT_MoraleMap it = std::find_if( morales_.begin(), morales_.end(), std::compose1( std::bind2nd( std::equal_to< ASN1T_EnumUnitMoral >(), nAsnID ), std::compose1( std::mem_fun( &PHY_Morale::GetAsnID ), std::select2nd< T_MoraleMap::value_type >() ) ) );

    return it == morales_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Morale::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
inline
const PHY_Morale* PHY_Morale::Find( uint nID )
{
    CIT_MoraleMap it = std::find_if( morales_.begin(), morales_.end(), std::compose1( std::bind2nd( std::equal_to< uint >(), nID ), std::compose1( std::mem_fun( &PHY_Morale::GetID ), std::select2nd< T_MoraleMap::value_type >() ) ) );

    return it == morales_.end() ? 0 : it->second;
}


// -----------------------------------------------------------------------------
// Name: PHY_Morale::Find
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
const PHY_Morale* PHY_Morale::Find( const std::string& strName )
{
    CIT_MoraleMap it = morales_.find( strName );
    
    return it == morales_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Morale::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const std::string& PHY_Morale::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Morale::GetAsnID
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
ASN1T_EnumUnitMoral PHY_Morale::GetAsnID() const
{
    return nAsnID_;
}

// =============================================================================
// OPËRATORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Morale::operator==
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
bool PHY_Morale::operator==( const PHY_Morale& rhs ) const
{
    return nType_ == rhs.nType_;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_Morale::operator!=
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
bool PHY_Morale::operator!=( const PHY_Morale& rhs ) const
{
    return nType_ != rhs.nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Morale::GetID
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
inline
uint PHY_Morale::GetID() const
{
    return (uint)nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Morale::GetWeight
// Created: NLD 2004-12-01
// -----------------------------------------------------------------------------
inline
MT_Float PHY_Morale::GetWeight() const
{
    return rDIAWeight_;
}

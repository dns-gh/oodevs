// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Humans/PHY_HumanRank.inl $
// $Author: Jvt $
// $Modtime: 14/04/05 17:06 $
// $Revision: 3 $
// $Workfile: PHY_HumanRank.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::GetHumanRanks
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
inline
const PHY_HumanRank::T_HumanRankMap& PHY_HumanRank::GetHumanRanks()
{
    return humanRanks_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::Find
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
inline
const PHY_HumanRank* PHY_HumanRank::Find( const std::string& strName )
{
    CIT_HumanRankMap it = humanRanks_.find( strName );
    
    return it == humanRanks_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
inline
const PHY_HumanRank* PHY_HumanRank::Find( uint nID )
{
    CIT_HumanRankMap it = std::find_if( humanRanks_.begin(), humanRanks_.end(), std::compose1( std::bind2nd( std::equal_to< uint >(), nID ), std::compose1( std::mem_fun( &PHY_HumanRank::GetID ), std::select2nd< T_HumanRankMap::value_type >() ) ) );
    return it == humanRanks_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::Find
// Created: NLD 2005-08-16
// -----------------------------------------------------------------------------
inline
const PHY_HumanRank* PHY_HumanRank::Find( ASN1T_EnumHumanRank nAsnID )
{
    CIT_HumanRankMap it = std::find_if( humanRanks_.begin(), humanRanks_.end(), std::compose1( std::bind2nd( std::equal_to< uint >(), nAsnID ), std::compose1( std::mem_fun( &PHY_HumanRank::GetAsnID ), std::select2nd< T_HumanRankMap::value_type >() ) ) );
    return it == humanRanks_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::GetName
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
inline
const std::string& PHY_HumanRank::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::GetAsnID
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
inline
ASN1T_EnumHumanRank PHY_HumanRank::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::GetID
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
inline
uint PHY_HumanRank::GetID() const
{
    return (uint)nRank_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::IsCommander
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
inline
bool PHY_HumanRank::IsCommander() const
{
    return bIsCommander_;
}

// =============================================================================
// OPERATORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::operator==
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
inline
bool PHY_HumanRank::operator==( const PHY_HumanRank& rhs ) const
{
    return nRank_ == rhs.nRank_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::operator!=
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
inline
bool PHY_HumanRank::operator!=( const PHY_HumanRank& rhs ) const
{
    return nRank_ != rhs.nRank_;
}

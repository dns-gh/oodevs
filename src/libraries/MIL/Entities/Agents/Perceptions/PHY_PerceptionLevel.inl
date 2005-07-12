// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionLevel.inl $
// $Author: Jvt $
// $Modtime: 16/03/05 19:05 $
// $Revision: 2 $
// $Workfile: PHY_PerceptionLevel.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::FindPerceptionLevel
// Created: JVT 2005-03-16
// -----------------------------------------------------------------------------
inline
const PHY_PerceptionLevel& PHY_PerceptionLevel::FindPerceptionLevel( uint nID )
{
    CIT_LevelMap it = levels_.find( nID );
    
    return it != levels_.end() ? *it->second : notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::IsBestLevel
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
inline
bool PHY_PerceptionLevel::IsBestLevel() const
{
    return nLevel_ == eIdentified;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::GetName
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
inline
const std::string& PHY_PerceptionLevel::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::GetID
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
inline
uint PHY_PerceptionLevel::GetID() const
{
    return (uint)nLevel_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::GetAsnID
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
inline
ASN1T_EnumUnitIdentificationLevel PHY_PerceptionLevel::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::operator==
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
inline
bool PHY_PerceptionLevel::operator==( const PHY_PerceptionLevel& rhs ) const
{
    return nLevel_ == rhs.nLevel_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::operator!=
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
inline
bool PHY_PerceptionLevel::operator!=( const PHY_PerceptionLevel& rhs ) const
{
    return nLevel_ != rhs.nLevel_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::operator<
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
inline
bool PHY_PerceptionLevel::operator<( const PHY_PerceptionLevel& rhs ) const
{
    return nLevel_ < rhs.nLevel_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::operator<=
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
inline
bool PHY_PerceptionLevel::operator<=( const PHY_PerceptionLevel& rhs ) const
{
    return nLevel_ <= rhs.nLevel_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::operator>
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
inline
bool PHY_PerceptionLevel::operator>( const PHY_PerceptionLevel& rhs ) const
{
    return nLevel_ > rhs.nLevel_;
}


// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::operator>=
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
inline
bool PHY_PerceptionLevel::operator>=( const PHY_PerceptionLevel& rhs ) const
{
    return nLevel_ >= rhs.nLevel_;
}

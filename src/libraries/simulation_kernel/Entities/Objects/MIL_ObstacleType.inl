// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_ObstacleType::Find
// Created: NLD 2007-05-22
// -----------------------------------------------------------------------------
inline
const MIL_ObstacleType* MIL_ObstacleType::Find( const std::string& strName )
{
    if( sCaseInsensitiveEqual()( strName, reserved_.GetName() ) )
        return &reserved_;
    else if( sCaseInsensitiveEqual()( strName, initial_.GetName() ) )
        return &initial_;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObstacleType::Find
// Created: NLD 2007-05-22
// -----------------------------------------------------------------------------
inline
const MIL_ObstacleType* MIL_ObstacleType::Find( uint nID )
{
    if( nID == reserved_.GetID() )
        return &reserved_;
    else if( nID == initial_.GetID() )
        return &initial_;
    return 0;
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ObstacleType::Find
// Created: NLD 2007-05-22
// -----------------------------------------------------------------------------
inline
const MIL_ObstacleType* MIL_ObstacleType::Find( ASN1T_EnumObstacleType asn )
{
    if( asn == reserved_.GetAsnID() )
        return &reserved_;
    else if( asn == initial_.GetAsnID() )
        return &initial_;
    return 0;
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ObstacleType::GetID
// Created: NLD 2007-05-22
// -----------------------------------------------------------------------------
inline
uint MIL_ObstacleType::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObstacleType::GetAsnID
// Created: NLD 2007-05-22
// -----------------------------------------------------------------------------
inline
ASN1T_EnumObstacleType MIL_ObstacleType::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObstacleType::GetName
// Created: NLD 2007-05-22
// -----------------------------------------------------------------------------
inline
const std::string& MIL_ObstacleType::GetName() const
{
    return strName_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ObstacleType::operator==
// Created: NLD 2007-05-22
// -----------------------------------------------------------------------------
inline
bool MIL_ObstacleType::operator==( const MIL_ObstacleType& rhs ) const
{
    return nType_ == rhs.nType_;
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ObstacleType::operator!=
// Created: NLD 2007-05-22
// -----------------------------------------------------------------------------
inline
bool MIL_ObstacleType::operator!=( const MIL_ObstacleType& rhs ) const
{
    return nType_ != rhs.nType_;
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ObstacleType::CouldBeActivated
// Created: NLD 2007-05-22
// -----------------------------------------------------------------------------
inline
bool MIL_ObstacleType::CouldBeActivated() const
{
    return *this == reserved_;
}

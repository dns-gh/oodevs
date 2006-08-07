// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Morale.inl $
// $Author: Age $
// $Modtime: 1/12/04 15:39 $
// $Revision: 1 $
// $Workfile: Morale.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: Morale::GetMorales
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
inline
const Morale::T_MoraleMap& Morale::GetMorales()
{
    return morales_;
}

// -----------------------------------------------------------------------------
// Name: Experience::Find
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
const Morale* Morale::Find( ASN1T_EnumUnitMoral nAsnID )
{
    for( CIT_MoraleMap it = morales_.begin(); it != morales_.end(); ++it )
    {
        const Morale& morale = *it->second;
        if( morale.GetAsnID() == nAsnID )
            return &morale;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Morale::Find
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
const Morale* Morale::Find( const std::string& strName )
{
    CIT_MoraleMap it = morales_.find( strName );
    if( it == morales_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: Morale::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const std::string& Morale::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: Morale::GetAsnID
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
ASN1T_EnumUnitMoral Morale::GetAsnID() const
{
    return nAsnID_;
}

// =============================================================================
// OPËRATORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Morale::operator==
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
bool Morale::operator==( const Morale& rhs ) const
{
    return nType_ == rhs.nType_;
}
    
// -----------------------------------------------------------------------------
// Name: Morale::operator!=
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
bool Morale::operator!=( const Morale& rhs ) const
{
    return nType_ != rhs.nType_;
}

// -----------------------------------------------------------------------------
// Name: Morale::GetID
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
inline
int Morale::GetID() const
{
    return nType_;
}

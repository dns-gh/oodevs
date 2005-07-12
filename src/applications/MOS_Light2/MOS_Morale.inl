// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Morale.inl $
// $Author: Age $
// $Modtime: 1/12/04 15:39 $
// $Revision: 1 $
// $Workfile: MOS_Morale.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_Morale::GetMorales
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
inline
const MOS_Morale::T_MoraleMap& MOS_Morale::GetMorales()
{
    return morales_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Experience::Find
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
const MOS_Morale* MOS_Morale::Find( ASN1T_EnumUnitMoral nAsnID )
{
    for( CIT_MoraleMap it = morales_.begin(); it != morales_.end(); ++it )
    {
        const MOS_Morale& morale = *it->second;
        if( morale.GetAsnID() == nAsnID )
            return &morale;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MOS_Morale::Find
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
const MOS_Morale* MOS_Morale::Find( const std::string& strName )
{
    CIT_MoraleMap it = morales_.find( strName );
    if( it == morales_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MOS_Morale::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const std::string& MOS_Morale::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Morale::GetAsnID
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
ASN1T_EnumUnitMoral MOS_Morale::GetAsnID() const
{
    return nAsnID_;
}

// =============================================================================
// OPËRATORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MOS_Morale::operator==
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
bool MOS_Morale::operator==( const MOS_Morale& rhs ) const
{
    return nType_ == rhs.nType_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Morale::operator!=
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
bool MOS_Morale::operator!=( const MOS_Morale& rhs ) const
{
    return nType_ != rhs.nType_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Morale::GetID
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
inline
int MOS_Morale::GetID() const
{
    return nType_;
}

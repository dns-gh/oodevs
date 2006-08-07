// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Experience.inl $
// $Author: Age $
// $Modtime: 1/12/04 15:39 $
// $Revision: 1 $
// $Workfile: Experience.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: Experience::GetExperiences
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
inline
const Experience::T_ExperienceMap& Experience::GetExperiences()
{
    return experiences_;
}

// -----------------------------------------------------------------------------
// Name: Experience::Find
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
const Experience* Experience::Find( ASN1T_EnumUnitExperience nAsnID )
{
    for( CIT_ExperienceMap it = experiences_.begin(); it != experiences_.end(); ++it )
    {
        const Experience& experience = *it->second;
        if( experience.GetAsnID() == nAsnID )
            return &experience;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Experience::Find
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
const Experience* Experience::Find( const std::string& strName )
{
    CIT_ExperienceMap it = experiences_.find( strName );
    if( it == experiences_.end() )
        return 0;
    return it->second;
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Experience::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const std::string& Experience::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: Experience::GetAsnID
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
ASN1T_EnumUnitExperience Experience::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: Experience::GetCoefMaxSpeedModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
float Experience::GetCoefMaxSpeedModificator() const
{
    return rCoefMaxSpeedModificator_;
}

// -----------------------------------------------------------------------------
// Name: Experience::GetCoefReloadingTimeModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
float Experience::GetCoefReloadingTimeModificator() const
{
    return rCoefReloadingTimeModificator_;
}

// -----------------------------------------------------------------------------
// Name: Experience::GetCoefPhModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
float Experience::GetCoefPhModificator() const
{
    return rCoefPhModificator_;
}

// -----------------------------------------------------------------------------
// Name: Experience::GetCoefPostureTimeModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
float Experience::GetCoefPostureTimeModificator() const
{
    return rCoefPostureTimeModificator_;
}

// -----------------------------------------------------------------------------
// Name: Experience::GetCoefSensorDistanceModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
float Experience::GetCoefSensorDistanceModificator() const
{
    return rCoefSensorDistanceModificator_;
}

// -----------------------------------------------------------------------------
// Name: Experience::GetID
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
inline
int Experience::GetID() const
{
    return nType_;
}

// =============================================================================
// OPËRATORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Experience::operator==
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
bool Experience::operator==( const Experience& rhs ) const
{
    return nType_ == rhs.nType_;
}
    
// -----------------------------------------------------------------------------
// Name: Experience::operator!=
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
bool Experience::operator!=( const Experience& rhs ) const
{
    return nType_ != rhs.nType_;
}


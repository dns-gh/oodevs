// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Experience.inl $
// $Author: Nld $
// $Modtime: 30/11/04 18:48 $
// $Revision: 1 $
// $Workfile: MOS_Experience.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_Experience::GetExperiences
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
inline
const MOS_Experience::T_ExperienceMap& MOS_Experience::GetExperiences()
{
    return experiences_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Experience::Find
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
const MOS_Experience* MOS_Experience::Find( ASN1T_EnumUnitExperience nAsnID )
{
    for( CIT_ExperienceMap it = experiences_.begin(); it != experiences_.end(); ++it )
    {
        const MOS_Experience& experience = *it->second;
        if( experience.GetAsnID() == nAsnID )
            return &experience;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MOS_Experience::Find
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
const MOS_Experience* MOS_Experience::Find( const std::string& strName )
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
// Name: MOS_Experience::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const std::string& MOS_Experience::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Experience::GetAsnID
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
ASN1T_EnumUnitExperience MOS_Experience::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Experience::GetCoefMaxSpeedModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float MOS_Experience::GetCoefMaxSpeedModificator() const
{
    return rCoefMaxSpeedModificator_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Experience::GetCoefReloadingTimeModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float MOS_Experience::GetCoefReloadingTimeModificator() const
{
    return rCoefReloadingTimeModificator_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Experience::GetCoefPhModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float MOS_Experience::GetCoefPhModificator() const
{
    return rCoefPhModificator_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Experience::GetCoefPostureTimeModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float MOS_Experience::GetCoefPostureTimeModificator() const
{
    return rCoefPostureTimeModificator_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Experience::GetCoefSensorDistanceModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float MOS_Experience::GetCoefSensorDistanceModificator() const
{
    return rCoefSensorDistanceModificator_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Experience::GetID
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
inline
int MOS_Experience::GetID() const
{
    return nType_;
}

// =============================================================================
// OPËRATORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MOS_Experience::operator==
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
bool MOS_Experience::operator==( const MOS_Experience& rhs ) const
{
    return nType_ == rhs.nType_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Experience::operator!=
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
bool MOS_Experience::operator!=( const MOS_Experience& rhs ) const
{
    return nType_ != rhs.nType_;
}


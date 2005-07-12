// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/HumanFactors/PHY_Experience.inl $
// $Author: Jvt $
// $Modtime: 14/04/05 16:29 $
// $Revision: 4 $
// $Workfile: PHY_Experience.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_Experience::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
inline
const PHY_Experience* PHY_Experience::Find( ASN1T_EnumUnitExperience nAsnID )
{
    CIT_ExperienceMap it = std::find_if( experiences_.begin(), experiences_.end(), std::compose1( std::bind2nd( std::equal_to< ASN1T_EnumUnitExperience >(), nAsnID ), std::compose1( std::mem_fun( &PHY_Experience::GetAsnID ), std::select2nd< T_ExperienceMap::value_type >() ) ) );

    return it == experiences_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
inline
const PHY_Experience* PHY_Experience::Find( uint nID )
{
    CIT_ExperienceMap it = std::find_if( experiences_.begin(), experiences_.end(), std::compose1( std::bind2nd( std::equal_to< uint >(), nID ), std::compose1( std::mem_fun( &PHY_Experience::GetID ), std::select2nd< T_ExperienceMap::value_type >() ) ) );

    return it == experiences_.end() ? 0 : it->second;
}


// -----------------------------------------------------------------------------
// Name: PHY_Experience::Find
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
const PHY_Experience* PHY_Experience::Find( const std::string& strName )
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
// Name: PHY_Experience::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const std::string& PHY_Experience::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::GetAsnID
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
ASN1T_EnumUnitExperience PHY_Experience::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::GetCoefMaxSpeedModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float PHY_Experience::GetCoefMaxSpeedModificator() const
{
    return rCoefMaxSpeedModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::GetCoefReloadingTimeModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float PHY_Experience::GetCoefReloadingTimeModificator() const
{
    return rCoefReloadingTimeModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::GetCoefPhModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float PHY_Experience::GetCoefPhModificator() const
{
    return rCoefPhModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::GetCoefPostureTimeModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float PHY_Experience::GetCoefPostureTimeModificator() const
{
    return rCoefPostureTimeModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::GetCoefSensorDistanceModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float PHY_Experience::GetCoefSensorDistanceModificator() const
{
    return rCoefSensorDistanceModificator_;
}

// =============================================================================
// OPËRATORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Experience::operator==
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
bool PHY_Experience::operator==( const PHY_Experience& rhs ) const
{
    return nType_ == rhs.nType_;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_Experience::operator!=
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
bool PHY_Experience::operator!=( const PHY_Experience& rhs ) const
{
    return nType_ != rhs.nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::GetID
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
inline
uint PHY_Experience::GetID() const
{
    return (uint)nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::GetWeight
/** @return 
*/
// Created: NLD 2004-12-01
// -----------------------------------------------------------------------------
inline
MT_Float PHY_Experience::GetWeight() const
{
    return rDIAWeight_;
}

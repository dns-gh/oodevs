// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/HumanFactors/PHY_Tiredness.inl $
// $Author: Jvt $
// $Modtime: 14/04/05 16:31 $
// $Revision: 4 $
// $Workfile: PHY_Tiredness.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
inline
const PHY_Tiredness* PHY_Tiredness::Find( ASN1T_EnumUnitFatigue nAsnID )
{
    CIT_TirednessMap it = std::find_if( tirednesses_.begin(), tirednesses_.end(), std::compose1( std::bind2nd( std::equal_to< ASN1T_EnumUnitFatigue >(), nAsnID ), std::compose1( std::mem_fun( &PHY_Tiredness::GetAsnID ), std::select2nd< T_TirednessMap::value_type >() ) ) );

    return it == tirednesses_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
inline
const PHY_Tiredness* PHY_Tiredness::Find( uint nID )
{
    CIT_TirednessMap it = std::find_if( tirednesses_.begin(), tirednesses_.end(), std::compose1( std::bind2nd( std::equal_to< uint >(), nID ), std::compose1( std::mem_fun( &PHY_Tiredness::GetID ), std::select2nd< T_TirednessMap::value_type >() ) ) );

    return it == tirednesses_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::Find
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
const PHY_Tiredness* PHY_Tiredness::Find( const std::string& strName )
{
    CIT_TirednessMap it = tirednesses_.find( strName );
    if( it == tirednesses_.end() )
        return 0;
    return it->second;
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const std::string& PHY_Tiredness::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::GetAsnID
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
ASN1T_EnumUnitFatigue PHY_Tiredness::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::GetCoefMaxSpeedModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float PHY_Tiredness::GetCoefMaxSpeedModificator() const
{
    return rCoefMaxSpeedModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::GetCoefReloadingTimeModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float PHY_Tiredness::GetCoefReloadingTimeModificator() const
{
    return rCoefReloadingTimeModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::GetCoefPhModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float PHY_Tiredness::GetCoefPhModificator() const
{
    return rCoefPhModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::GetCoefPostureTimeModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float PHY_Tiredness::GetCoefPostureTimeModificator() const
{
    return rCoefPostureTimeModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::GetCoefSensorDistanceModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float PHY_Tiredness::GetCoefSensorDistanceModificator() const
{
    return rCoefSensorDistanceModificator_;
}

// =============================================================================
// OPËRATORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::operator==
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
bool PHY_Tiredness::operator==( const PHY_Tiredness& rhs ) const
{
    return nType_ == rhs.nType_;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::operator!=
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
bool PHY_Tiredness::operator!=( const PHY_Tiredness& rhs ) const
{
    return nType_ != rhs.nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::GetID
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
inline
uint PHY_Tiredness::GetID() const
{
    return (uint)nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::GetWeight
/** @return 
*/
// Created: NLD 2004-12-01
// -----------------------------------------------------------------------------
inline
MT_Float PHY_Tiredness::GetWeight() const
{
    return rDIAWeight_;
}


// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Tiredness.inl $
// $Author: Nld $
// $Modtime: 30/11/04 18:50 $
// $Revision: 1 $
// $Workfile: MOS_Tiredness.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_Tiredness::GetTirednesses
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
inline
const MOS_Tiredness::T_TirednessMap& MOS_Tiredness::GetTirednesses()
{
    return tirednesses_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Tiredness::Find
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
const MOS_Tiredness* MOS_Tiredness::Find( ASN1T_EnumUnitFatigue nAsnID )
{
    for( CIT_TirednessMap it = tirednesses_.begin(); it != tirednesses_.end(); ++it )
    {
        const MOS_Tiredness& tiredness = *it->second;
        if( tiredness.GetAsnID() == nAsnID )
            return &tiredness;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MOS_Tiredness::Find
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
const MOS_Tiredness* MOS_Tiredness::Find( const std::string& strName )
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
// Name: MOS_Tiredness::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const std::string& MOS_Tiredness::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Tiredness::GetAsnID
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
ASN1T_EnumUnitFatigue MOS_Tiredness::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Tiredness::GetCoefMaxSpeedModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float MOS_Tiredness::GetCoefMaxSpeedModificator() const
{
    return rCoefMaxSpeedModificator_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Tiredness::GetCoefReloadingTimeModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float MOS_Tiredness::GetCoefReloadingTimeModificator() const
{
    return rCoefReloadingTimeModificator_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Tiredness::GetCoefPhModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float MOS_Tiredness::GetCoefPhModificator() const
{
    return rCoefPhModificator_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Tiredness::GetCoefPostureTimeModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float MOS_Tiredness::GetCoefPostureTimeModificator() const
{
    return rCoefPostureTimeModificator_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Tiredness::GetCoefSensorDistanceModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float MOS_Tiredness::GetCoefSensorDistanceModificator() const
{
    return rCoefSensorDistanceModificator_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Tiredness::GetID
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
inline
int MOS_Tiredness::GetID() const
{
    return nType_;
}


// =============================================================================
// OPËRATORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MOS_Tiredness::operator==
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
bool MOS_Tiredness::operator==( const MOS_Tiredness& rhs ) const
{
    return nType_ == rhs.nType_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Tiredness::operator!=
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
bool MOS_Tiredness::operator!=( const MOS_Tiredness& rhs ) const
{
    return nType_ != rhs.nType_;
}


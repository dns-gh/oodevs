// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Tiredness.inl $
// $Author: Age $
// $Modtime: 1/12/04 15:39 $
// $Revision: 1 $
// $Workfile: Tiredness.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: Tiredness::GetTirednesses
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
inline
const Tiredness::T_TirednessMap& Tiredness::GetTirednesses()
{
    return tirednesses_;
}

// -----------------------------------------------------------------------------
// Name: Tiredness::Find
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
const Tiredness* Tiredness::Find( ASN1T_EnumUnitFatigue nAsnID )
{
    for( CIT_TirednessMap it = tirednesses_.begin(); it != tirednesses_.end(); ++it )
    {
        const Tiredness& tiredness = *it->second;
        if( tiredness.GetAsnID() == nAsnID )
            return &tiredness;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Tiredness::Find
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
const Tiredness* Tiredness::Find( const std::string& strName )
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
// Name: Tiredness::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const std::string& Tiredness::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: Tiredness::GetAsnID
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
ASN1T_EnumUnitFatigue Tiredness::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: Tiredness::GetCoefMaxSpeedModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
float Tiredness::GetCoefMaxSpeedModificator() const
{
    return rCoefMaxSpeedModificator_;
}

// -----------------------------------------------------------------------------
// Name: Tiredness::GetCoefReloadingTimeModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
float Tiredness::GetCoefReloadingTimeModificator() const
{
    return rCoefReloadingTimeModificator_;
}

// -----------------------------------------------------------------------------
// Name: Tiredness::GetCoefPhModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
float Tiredness::GetCoefPhModificator() const
{
    return rCoefPhModificator_;
}

// -----------------------------------------------------------------------------
// Name: Tiredness::GetCoefPostureTimeModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
float Tiredness::GetCoefPostureTimeModificator() const
{
    return rCoefPostureTimeModificator_;
}

// -----------------------------------------------------------------------------
// Name: Tiredness::GetCoefSensorDistanceModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
float Tiredness::GetCoefSensorDistanceModificator() const
{
    return rCoefSensorDistanceModificator_;
}

// -----------------------------------------------------------------------------
// Name: Tiredness::GetID
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
inline
int Tiredness::GetID() const
{
    return nType_;
}


// =============================================================================
// OPËRATORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Tiredness::operator==
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
bool Tiredness::operator==( const Tiredness& rhs ) const
{
    return nType_ == rhs.nType_;
}
    
// -----------------------------------------------------------------------------
// Name: Tiredness::operator!=
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
bool Tiredness::operator!=( const Tiredness& rhs ) const
{
    return nType_ != rhs.nType_;
}


// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Humans/PHY_HumanWound.inl $
// $Author: Jvt $
// $Modtime: 14/04/05 17:11 $
// $Revision: 2 $
// $Workfile: PHY_HumanWound.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetDiagnosingTime
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
uint PHY_HumanWound::GetDiagnosticTime()
{
    return nDiagnosticTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetSortingTime
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
uint PHY_HumanWound::GetSortingTime()
{
    return nSortingTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetContaminatedHealingTime
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
inline
uint PHY_HumanWound::GetContaminatedHealingTime()
{
    return nContaminatedHealingTime_;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetMentalDiseasedHealingTime
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
inline
uint PHY_HumanWound::GetMentalDiseaseHealingTime()
{
    return nMentalDiseaseHealingTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetHumanWounds
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
inline
const PHY_HumanWound::T_HumanWoundMap& PHY_HumanWound::GetHumanWounds()
{
    return humanWounds_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::Find
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
inline
const PHY_HumanWound* PHY_HumanWound::Find( const std::string& strName )
{
    CIT_HumanWoundMap it = humanWounds_.find( strName );
    return it == humanWounds_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
inline
const PHY_HumanWound* PHY_HumanWound::Find( ASN1T_EnumHumanWound nAsnID )
{
    CIT_HumanWoundMap it = std::find_if( humanWounds_.begin(), humanWounds_.end(), std::compose1( std::bind2nd( std::equal_to< ASN1T_EnumHumanWound >(), nAsnID ), std::compose1( std::mem_fun( &PHY_HumanWound::GetAsnID ), std::select2nd< T_HumanWoundMap::value_type >() ) ) );

    return it == humanWounds_.end() ? 0 : it->second;   
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
inline
const PHY_HumanWound* PHY_HumanWound::Find( uint nID )
{
    CIT_HumanWoundMap it = std::find_if( humanWounds_.begin(), humanWounds_.end(), std::compose1( std::bind2nd( std::equal_to< uint >(), nID ), std::compose1( std::mem_fun( &PHY_HumanWound::GetID ), std::select2nd< T_HumanWoundMap::value_type >() ) ) );

    return it == humanWounds_.end() ? 0 : it->second;    
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetName
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
inline
const std::string& PHY_HumanWound::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetID
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
inline
uint PHY_HumanWound::GetID() const
{
    return (uint)nWound_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetAsnID
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
ASN1T_EnumHumanWound PHY_HumanWound::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::operator==
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
inline
bool PHY_HumanWound::operator==( const PHY_HumanWound& rhs ) const
{
    return nWound_ == rhs.nWound_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::operator!=
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
inline
bool PHY_HumanWound::operator!=( const PHY_HumanWound& rhs ) const
{
    return nWound_ != rhs.nWound_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetWoundedFactor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
inline
MT_Float PHY_HumanWound::GetWoundedFactor() const
{
    return rWoundedFactor_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::Aggravate
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
inline
const PHY_HumanWound& PHY_HumanWound::Aggravate() const
{
    return Aggravate( GetRandomWound() );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetLifeExpectancy
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
inline
uint PHY_HumanWound::GetLifeExpectancy() const
{
    return nLifeExpectancy_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetHealingTime
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
inline
uint PHY_HumanWound::GetHealingTime() const
{
    return nHealingTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::ChooseMentalDisease
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
inline
bool PHY_HumanWound::ChooseMentalDisease()
{
    return randomGenerator_.rand_oi( 0., 1. ) <= rMentalDiseaseFactor_;
}

// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Humans/PHY_Human.inl $
// $Author: Nld $
// $Modtime: 13/01/05 18:38 $
// $Revision: 2 $
// $Workfile: PHY_Human.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_Human::GetRank
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
inline
const PHY_HumanRank& PHY_Human::GetRank() const
{
    assert( pRank_ );
    return *pRank_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::GetWound
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
inline
const PHY_HumanWound& PHY_Human::GetWound() const
{
    assert( pWound_ );
    return *pWound_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::GetLocation
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
inline
PHY_Human::E_Location PHY_Human::GetLocation() const
{
    return nLocation_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::IsAlive
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
inline
bool PHY_Human::IsAlive() const
{
    return *pWound_ != PHY_HumanWound::killed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::IsAvailable
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
inline
bool PHY_Human::IsUsable() const
{
    if( pMedicalState_ && nLocation_ == eBattleField )
        return true;

    return IsAlive();
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::IsWounded
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
inline
bool PHY_Human::IsWounded() const
{
    return *pWound_ != PHY_HumanWound::killed_ && *pWound_ != PHY_HumanWound::notWounded_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::IsContaminated
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
inline
bool PHY_Human::IsContaminated() const
{
    return bContamined_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::IsMentalDiseased
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
inline
bool PHY_Human::IsMentalDiseased() const
{
    return bMentalDiseased_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::IsAnEmergency
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
bool PHY_Human::IsAnEmergency() const
{
    return *pWound_ == PHY_HumanWound::woundedU1_ || *pWound_ == PHY_HumanWound::woundedUE_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::NeedMedical
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
inline
bool PHY_Human::NeedMedical() const
{
    return IsWounded() || IsContaminated() || IsMentalDiseased();
}




// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Weapons/PHY_AttritionData.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: PHY_AttritionData.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_AttritionData::operator=
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
PHY_AttritionData& PHY_AttritionData::operator=( const PHY_AttritionData& rhs )
{
    rScore_                           = rhs.rScore_;
    rDestroyedBound_                  = rhs.rDestroyedBound_;
    rReparableWithEvacuationBound_    = rhs.rReparableWithEvacuationBound_;
    rReparableWithoutEvacuationBound_ = rhs.rReparableWithoutEvacuationBound_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: PHY_AttritionData::GetScore
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
inline
MT_Float PHY_AttritionData::GetScore() const
{
    return rScore_;
}

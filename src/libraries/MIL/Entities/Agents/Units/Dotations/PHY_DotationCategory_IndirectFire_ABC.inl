// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectFire_ABC.inl $
// $Author: Nld $
// $Modtime: 17/03/05 14:53 $
// $Revision: 2 $
// $Workfile: PHY_DotationCategory_IndirectFire_ABC.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC::GetIndirectFireDotationCategory
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
inline
const PHY_IndirectFireDotationClass& PHY_DotationCategory_IndirectFire_ABC::GetIndirectFireDotationCategory() const
{
    return category_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC::ConvertToInterventionType
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
inline
MT_Float PHY_DotationCategory_IndirectFire_ABC::ConvertToInterventionType( uint nNbr ) const
{
    return (float)nNbr / (float)nInterventionType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC::ConvertToNbrAmmo
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
inline
MT_Float PHY_DotationCategory_IndirectFire_ABC::ConvertToNbrAmmo( MT_Float rNbrIT ) const
{
    return nInterventionType_ * rNbrIT;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC::HasHit
// Created: NLD 2005-08-04
// -----------------------------------------------------------------------------
inline
bool PHY_DotationCategory_IndirectFire_ABC::HasHit( const MIL_Agent_ABC& /*target*/ ) const
{
    return true;
}

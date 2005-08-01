// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationCategory.inl $
// $Author: Nld $
// $Modtime: 17/03/05 15:15 $
// $Revision: 4 $
// $Workfile: PHY_DotationCategory.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::GetAmmoDotationClass
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
inline
const PHY_AmmoDotationClass* PHY_DotationCategory::GetAmmoDotationClass() const
{
    return pAmmoDotationClass_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::GetMosID
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
inline
uint PHY_DotationCategory::GetMosID() const
{
    return nMosID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::GetType
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
inline
const PHY_DotationType& PHY_DotationCategory::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::GetName
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
inline
const std::string& PHY_DotationCategory::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::CanBeUsedForDirectFire
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
inline
bool PHY_DotationCategory::CanBeUsedForDirectFire() const
{
    return !attritions_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::HasAttritions
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
inline
bool PHY_DotationCategory::HasAttritions() const
{
    return !attritions_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::CanBeUsedForIndirectFire
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
inline
bool PHY_DotationCategory::CanBeUsedForIndirectFire() const
{
    return pIndirectFireData_ != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::GetIndirectFireData
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
inline
const PHY_DotationCategory_IndirectFire_ABC* PHY_DotationCategory::GetIndirectFireData() const
{
    return pIndirectFireData_;
}

// =============================================================================
// OPERATORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::operator==
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
inline
bool PHY_DotationCategory::operator==( const PHY_DotationCategory& rhs ) const
{
    return this == &rhs;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::operator!=
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
inline
bool PHY_DotationCategory::operator!=( const PHY_DotationCategory& rhs ) const
{
    return this != &rhs;
}

// =============================================================================
// PACKAGING
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::GetWeight
// Created: NLD 2005-07-13
// -----------------------------------------------------------------------------
inline
MT_Float PHY_DotationCategory::GetWeight() const
{
    return rWeight_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::GetVolume
// Created: NLD 2005-07-13
// -----------------------------------------------------------------------------
inline
MT_Float PHY_DotationCategory::GetVolume() const
{
    return rVolume_;
}

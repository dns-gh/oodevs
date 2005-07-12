// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationCapacity.inl $
// $Author: Nld $
// $Modtime: 28/01/05 15:58 $
// $Revision: 3 $
// $Workfile: PHY_DotationCapacity.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacity::GetCapacity
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
inline
MT_Float PHY_DotationCapacity::GetCapacity() const
{
    return rCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacity::GetSupplyThreshold
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
inline
MT_Float PHY_DotationCapacity::GetSupplyThreshold() const
{
    return rSupplyThreshold_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacity::GetCategory
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
inline
const PHY_DotationCategory& PHY_DotationCapacity::GetCategory() const
{
    return category_;
}

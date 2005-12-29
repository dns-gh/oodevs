// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_DotationGroup.inl $
// $Author: Jvt $
// $Modtime: 31/03/05 17:43 $
// $Revision: 4 $
// $Workfile: PHY_DotationGroup.inl $
//
// *****************************************************************************

#include "PHY_DotationGroupContainer.h"

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::GetDotation
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
inline
PHY_Dotation* PHY_DotationGroup::GetDotation( const PHY_DotationCategory& dotationCategory ) const
{
    CIT_DotationMap it = dotations_.find( &dotationCategory );
    if( it == dotations_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::NotifyDotationChanged
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
inline
void PHY_DotationGroup::NotifyDotationChanged( const PHY_Dotation& dotation )
{
    assert( pGroupContainer_ );
    pGroupContainer_->NotifyDotationChanged( dotation );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::NotifySupplyNeeded
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
inline
void PHY_DotationGroup::NotifySupplyNeeded( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const
{
    assert( pGroupContainer_ );
    pGroupContainer_->NotifySupplyNeeded( dotationCategory, bNewNeed );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::GetDotations
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
const PHY_DotationGroup::T_DotationMap& PHY_DotationGroup::GetDotations() const
{
    return dotations_;
}

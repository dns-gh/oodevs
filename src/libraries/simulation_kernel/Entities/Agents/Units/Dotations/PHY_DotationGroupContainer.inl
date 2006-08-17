// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_DotationGroupContainer.inl $
// $Author: Mco $
// $Modtime: 14/02/05 15:08 $
// $Revision: 3 $
// $Workfile: PHY_DotationGroupContainer.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::GetDotationGroup
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
inline
PHY_DotationGroup* PHY_DotationGroupContainer::GetDotationGroup( const PHY_DotationType& dotationType ) const
{
    CIT_DotationGroupMap it = dotationGroups_.find( &dotationType );
    if( it == dotationGroups_.end() )
        return 0;
    return it->second;
}

// =============================================================================
// NOTIFICATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::NotifyDotationChanged
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
inline
void PHY_DotationGroupContainer::NotifyDotationChanged( const PHY_Dotation& dotation )
{
    dotationsChanged_.insert( &dotation );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::HasChanged
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
inline
bool PHY_DotationGroupContainer::HasChanged() const
{
    return !dotationsChanged_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::Clean
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
inline
void PHY_DotationGroupContainer::Clean()
{
    dotationsChanged_.clear();
}

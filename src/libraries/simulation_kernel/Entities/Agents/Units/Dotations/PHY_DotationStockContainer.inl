// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationStockContainer.inl $
// $Author: Nld $
// $Modtime: 27/01/05 18:18 $
// $Revision: 1 $
// $Workfile: PHY_DotationStockContainer.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::NotifyDotationChanged
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
inline
void PHY_DotationStockContainer::NotifyDotationChanged( const PHY_DotationStock& dotationStock, MT_Float rDelta )
{
    stocksChanged_.insert( &dotationStock );
    if( rDelta > 0 )
        bCheckStockCapacities_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::HasChanged
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
inline
bool PHY_DotationStockContainer::HasChanged() const
{
    return !stocksChanged_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::Clean
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
inline
void PHY_DotationStockContainer::Clean()
{
    stocksChanged_.clear();
    bCheckStockCapacities_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::Update
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
inline
void PHY_DotationStockContainer::Update()
{
    if( bCheckStockCapacities_ )
        CheckStockCapacities();
}

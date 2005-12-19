// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyStockState.inl $
// $Author: Jvt $
// $Modtime: 11/04/05 12:17 $
// $Revision: 3 $
// $Workfile: PHY_SupplyStockState.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::SetConsign
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
inline
void PHY_SupplyStockState::SetConsign( PHY_SupplyConsign_ABC* pConsign )
{
    if( pConsign == pConsign_ )
        return;
        
    pConsign_        = pConsign;
    bConsignChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::GetSuppliedAutomate
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
inline
MIL_AutomateLOG& PHY_SupplyStockState::GetSuppliedAutomate() const
{
    assert( pSuppliedAutomate_ );
    return *pSuppliedAutomate_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::IsPushedFlow
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
inline
bool PHY_SupplyStockState::IsPushedFlow() const
{
    return bPushedFlow_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::IsSupplying
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
inline
bool PHY_SupplyStockState::IsSupplying( const PHY_DotationCategory& dotationCategory ) const
{
    return requests_.find( &dotationCategory ) != requests_.end();    
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::AddRequest
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
inline
void PHY_SupplyStockState::AddRequest( const PHY_SupplyStockRequest& request )
{
    requests_[ &request.GetDotationCategory() ] = request;
    bRequestsChanged_ = true;
}

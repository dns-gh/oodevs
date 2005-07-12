// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyDotationState.inl $
// $Author: Jvt $
// $Modtime: 11/04/05 13:14 $
// $Revision: 2 $
// $Workfile: PHY_SupplyDotationState.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState::SetConsign
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
inline
void PHY_SupplyDotationState::SetConsign( PHY_SupplyConsign_ABC* pConsign )
{
    if( pConsign == pConsign_ )
        return;
        
    pConsign_    = pConsign;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState::GetSuppliedAutomate
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
inline
const MIL_Automate& PHY_SupplyDotationState::GetSuppliedAutomate() const
{
    assert( pSuppliedAutomate_ );
    return *pSuppliedAutomate_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState::IsSupplying
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
inline
bool PHY_SupplyDotationState::IsSupplying( const PHY_DotationCategory& dotationCategory ) const
{
    return requests_.find( &dotationCategory ) != requests_.end();    
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState::AddRequest
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
inline
void PHY_SupplyDotationState::AddRequest( const PHY_SupplyDotationRequest& request )
{
    requests_[ &request.GetDotationCategory() ] = request;
    bHasChanged_ = true;
}
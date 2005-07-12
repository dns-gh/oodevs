// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.inl $
// $Author: Nld $
// $Modtime: 11/05/05 18:18 $
// $Revision: 7 $
// $Workfile: PHY_RolePion_Dotations.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::HasChanged
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Dotations::HasChanged() const
{
    assert( pDotations_ );
    return pDotations_->HasChanged();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::Clean
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Dotations::Clean()
{
    assert( pDotations_ );
    pDotations_->Clean();
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::GetDotationValue
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RolePion_Dotations::GetDotationValue( const PHY_DotationCategory& category ) const
{
    assert( pDotations_ );
    return pDotations_->GetValue( category );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::HasDotation
// Created: NLD 2005-04-18
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Dotations::HasDotation( const PHY_DotationCategory& category ) const
{
    assert( pDotations_ );
    return pDotations_->GetValue( category ) > 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::GetDotationCapacity
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RolePion_Dotations::GetDotationCapacity( const PHY_DotationCategory& category ) const
{
    assert( pDotations_ );
    return pDotations_->GetCapacity( category );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::ConsumeDotation
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RolePion_Dotations::ConsumeDotation( const PHY_DotationCategory& category, MT_Float rNbr )
{
    assert( pDotations_ );
    return pDotations_->Consume( category, rNbr );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::SupplyDotation
// Created: NLD 2005-05-11
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RolePion_Dotations::SupplyDotation( const PHY_DotationCategory& category, MT_Float rNbr )
{
    assert( pDotations_ );
    return pDotations_->Supply( category, rNbr );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::ResupplyDotations
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Dotations::ResupplyDotations()
{
    assert( pDotations_ );
    pDotations_->Resupply();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::AddFireReservation
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RolePion_Dotations::AddFireReservation( const PHY_DotationCategory& category, MT_Float rNbr )
{
    assert( pDotations_ );
    return pDotations_->AddFireReservation( category, rNbr );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::FillSupplyRequest
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Dotations::FillSupplyRequest( PHY_SupplyDotationRequestContainer& supplyRequestContainer ) const
{
    assert( pDotations_ );
    pDotations_->FillSupplyRequest( supplyRequestContainer );
}


// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::SendChangedState
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Dotations::SendChangedState( NET_ASN_MsgUnitDotations& asn ) const
{
    assert( pDotations_ );
    pDotations_->SendChangedState( asn );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::SendFullState
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Dotations::SendFullState( NET_ASN_MsgUnitDotations& asn ) const
{
    assert( pDotations_ );
    pDotations_->SendFullState( asn );
}

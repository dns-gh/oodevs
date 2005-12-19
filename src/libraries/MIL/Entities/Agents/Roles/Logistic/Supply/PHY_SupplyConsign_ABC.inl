// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyConsign_ABC.inl $
// $Author: Jvt $
// $Modtime: 11/04/05 17:47 $
// $Revision: 4 $
// $Workfile: PHY_SupplyConsign_ABC.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::Clean
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
inline
void PHY_SupplyConsign_ABC::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::SetTimer
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
inline
void PHY_SupplyConsign_ABC::SetState( E_State nNewState )
{
    nState_ = nNewState;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::GetState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
inline
PHY_SupplyConsign_ABC::E_State PHY_SupplyConsign_ABC::GetState() const
{
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::HasChanged
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
inline
bool PHY_SupplyConsign_ABC::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::GetSupplyingAutomate
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
inline
MIL_AutomateLOG& PHY_SupplyConsign_ABC::GetSupplyingAutomate() const
{
    assert( pSupplyingAutomate_ );
    return *pSupplyingAutomate_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::GetConvoyingAutomate
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
inline
MIL_AutomateLOG& PHY_SupplyConsign_ABC::GetConvoyingAutomate() const
{
    assert( pConvoyingAutomate_ );
    return *pConvoyingAutomate_;
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_SupplyConsign_ABC::serialize( Archive& file, const uint )
{
    file & pSupplyingAutomate_
         & pConvoyingAutomate_
         & nTimer_
         & nState_;
}

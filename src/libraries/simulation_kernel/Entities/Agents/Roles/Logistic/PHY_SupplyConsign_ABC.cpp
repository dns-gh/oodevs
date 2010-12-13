// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_SupplyConsign_ABC.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 17:47 $
// $Revision: 5 $
// $Workfile: PHY_SupplyConsign_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_SupplyConsign_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "protocol/ClientSenders.h"
// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_SupplyConsign_ABC::PHY_SupplyConsign_ABC( MIL_AutomateLOG& supplyingAutomate, const MIL_Automate& suppliedAutomate, MIL_AutomateLOG& convoyingAutomate
        , MIL_Automate& stockSupplier, bool bExternalTransfert )
    : pSupplyingAutomate_( &supplyingAutomate )
    , pSuppliedAutomate_ ( &suppliedAutomate  )
    , pConvoyingAutomate_( &convoyingAutomate )
    , nTimer_            ( 0 )
    , bHasChanged_       ( true )
    , pStockSupplier_    ( &stockSupplier )
    , bExternalTransfert_( bExternalTransfert )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_SupplyConsign_ABC::PHY_SupplyConsign_ABC()
    : pSupplyingAutomate_( 0 )
    , pSuppliedAutomate_ ( 0 )
    , pConvoyingAutomate_( 0 )
    , nTimer_            ( 0 )
    , bHasChanged_       ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_SupplyConsign_ABC::~PHY_SupplyConsign_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::SendFullState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_SupplyConsign_ABC::SendFullState( client::LogSupplyHandlingUpdate& asn ) const
{
    assert( pSupplyingAutomate_ );
    assert( pConvoyingAutomate_ );
    assert( pSuppliedAutomate_ );

    asn().set_etat( (sword::EnumLogSupplyHandlingStatus ) nState_  );
    pSupplyingAutomate_->FillParentEntity(*asn().mutable_supplier());
    pConvoyingAutomate_->FillParentEntity(*asn().mutable_convoy_provider());
    asn().mutable_convoying_unit()->set_id( 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::SendChangedState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_SupplyConsign_ABC::SendChangedState( client::LogSupplyHandlingUpdate& asn ) const
{
    if( bHasChanged_ )
        SendFullState( asn );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::SendDefaultState
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void PHY_SupplyConsign_ABC::SendDefaultState( client::LogSupplyHandlingUpdate& asn )
{
    asn().mutable_convoying_unit()->set_id( 0 );
    asn().mutable_convoy_provider()->mutable_automat()->set_id( 0 );
    asn().mutable_supplier()->mutable_automat()->set_id( 0 );
    asn().set_etat( sword::termine );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::Clean
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_SupplyConsign_ABC::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::SetTimer
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_SupplyConsign_ABC::SetState( E_State nNewState )
{
    nState_ = nNewState;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::GetState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
PHY_SupplyConsign_ABC::E_State PHY_SupplyConsign_ABC::GetState() const
{
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::HasChanged
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
bool PHY_SupplyConsign_ABC::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::GetSupplier
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
MIL_AutomateLOG& PHY_SupplyConsign_ABC::GetSupplier() const
{
    assert( pSupplyingAutomate_ );
    return *pSupplyingAutomate_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::GetSupplied
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
const MIL_Automate& PHY_SupplyConsign_ABC::GetSupplied() const
{
    assert( pSuppliedAutomate_ );
    return *pSuppliedAutomate_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::GetConvoyer
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
MIL_AutomateLOG& PHY_SupplyConsign_ABC::GetConvoyer() const
{
    assert( pConvoyingAutomate_ );
    return *pConvoyingAutomate_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::GetSupplier
// Created: MGD 2009-02-11
// -----------------------------------------------------------------------------
MIL_Automate& PHY_SupplyConsign_ABC::GetStockSupplier() const
{
    assert( pStockSupplier_ );
    return *pStockSupplier_;
}

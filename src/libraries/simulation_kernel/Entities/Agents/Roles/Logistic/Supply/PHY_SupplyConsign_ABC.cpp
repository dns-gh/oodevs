// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyConsign_ABC.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 17:47 $
// $Revision: 5 $
// $Workfile: PHY_SupplyConsign_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_SupplyConsign_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Network/NET_ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_SupplyConsign_ABC::PHY_SupplyConsign_ABC( MIL_AutomateLOG& supplyingAutomate, const MIL_Automate& suppliedAutomate, MIL_AutomateLOG& convoyingAutomate )
    : pSupplyingAutomate_( &supplyingAutomate )
    , pSuppliedAutomate_ ( &suppliedAutomate  )
    , pConvoyingAutomate_( &convoyingAutomate )
    , nTimer_            ( 0 )
    , bHasChanged_       ( true )
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
void PHY_SupplyConsign_ABC::SendFullState( NET_ASN_MsgLogSupplyHandlingUpdate& asn ) const
{
    assert( pSupplyingAutomate_ );
    assert( pConvoyingAutomate_ );
    assert( pSuppliedAutomate_ );

    asn().m.etatPresent                                       = 1;
    asn().m.oid_automate_log_traitantPresent                  = 1;    
    asn().m.oid_automate_log_fournissant_moyens_convoiPresent = 1;
    asn().m.oid_pion_convoyantPresent                         = 1;
    
    asn().etat                                       = (ASN1T_EnumLogSupplyHandlingStatus)nState_;
    asn().oid_automate_log_traitant                  = pSupplyingAutomate_->GetID();
    asn().oid_automate_log_fournissant_moyens_convoi = pConvoyingAutomate_->GetID();
    asn().oid_pion_convoyant                         = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::SendChangedState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_SupplyConsign_ABC::SendChangedState( NET_ASN_MsgLogSupplyHandlingUpdate& asn ) const
{
    if( bHasChanged_ )
        SendFullState( asn );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::SendDefaultState
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void PHY_SupplyConsign_ABC::SendDefaultState( NET_ASN_MsgLogSupplyHandlingUpdate& asn )
{
    asn().m.oid_automate_log_traitantPresent                  = 1;
    asn().m.oid_automate_log_fournissant_moyens_convoiPresent = 1;
    asn().m.oid_pion_convoyantPresent                         = 1;
    asn().m.etatPresent                                       = 1;

    asn().oid_pion_convoyant                         = 0;
    asn().oid_automate_log_fournissant_moyens_convoi = 0;
    asn().oid_automate_log_traitant                  = 0;
    asn().etat                                       = EnumLogSupplyHandlingStatus::termine;
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
// Name: PHY_SupplyConsign_ABC::GetSupplyingAutomate
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
MIL_AutomateLOG& PHY_SupplyConsign_ABC::GetSupplyingAutomate() const
{
    assert( pSupplyingAutomate_ );
    return *pSupplyingAutomate_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::GetSuppliedAutomate
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
const MIL_Automate& PHY_SupplyConsign_ABC::GetSuppliedAutomate() const
{
    assert( pSuppliedAutomate_ );
    return *pSuppliedAutomate_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::GetConvoyingAutomate
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
MIL_AutomateLOG& PHY_SupplyConsign_ABC::GetConvoyingAutomate() const
{
    assert( pConvoyingAutomate_ );
    return *pConvoyingAutomate_;
}

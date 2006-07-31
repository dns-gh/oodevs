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

#include "MIL_pch.h"

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
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_SupplyConsign_ABC::~PHY_SupplyConsign_ABC()
{
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::SendFullState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_SupplyConsign_ABC::SendFullState( NET_ASN_MsgLogRavitaillementTraitementUpdate& asn ) const
{
    assert( pSupplyingAutomate_ );
    assert( pConvoyingAutomate_ );
    assert( pSuppliedAutomate_ );

    asn.GetAsnMsg().m.etatPresent                                       = 1;
    asn.GetAsnMsg().m.oid_automate_log_traitantPresent                  = 1;    
    asn.GetAsnMsg().m.oid_automate_log_fournissant_moyens_convoiPresent = 1;
    asn.GetAsnMsg().m.oid_pion_convoyantPresent                         = 1;
    
    asn.GetAsnMsg().etat                                       = (ASN1T_EnumLogRavitaillementTraitementEtat)nState_;
    asn.GetAsnMsg().oid_automate_log_traitant                  = pSupplyingAutomate_->GetID();
    asn.GetAsnMsg().oid_automate_log_fournissant_moyens_convoi = pConvoyingAutomate_->GetID();
    asn.GetAsnMsg().oid_pion_convoyant                         = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::SendChangedState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_SupplyConsign_ABC::SendChangedState( NET_ASN_MsgLogRavitaillementTraitementUpdate& asn ) const
{
    if( bHasChanged_ )
        SendFullState( asn );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::SendDefaultState
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void PHY_SupplyConsign_ABC::SendDefaultState( NET_ASN_MsgLogRavitaillementTraitementUpdate& asn )
{
    asn.GetAsnMsg().m.oid_automate_log_traitantPresent                  = 1;
    asn.GetAsnMsg().m.oid_automate_log_fournissant_moyens_convoiPresent = 1;
    asn.GetAsnMsg().m.oid_pion_convoyantPresent                         = 1;
    asn.GetAsnMsg().m.etatPresent                                       = 1;

    asn.GetAsnMsg().oid_pion_convoyant                         = 0;
    asn.GetAsnMsg().oid_automate_log_fournissant_moyens_convoi = 0;
    asn.GetAsnMsg().oid_automate_log_traitant                  = 0;
    asn.GetAsnMsg().etat                                       = EnumLogRavitaillementTraitementEtat::termine;
}
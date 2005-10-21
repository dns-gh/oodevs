// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_LogSupplyConsign.cpp $
// $Author: Age $
// $Modtime: 1/04/05 11:14 $
// $Revision: 1 $
// $Workfile: MOS_LogSupplyConsign.cpp $
//
// *****************************************************************************

#include "MOS_Light2_pch.h"

#include "MOS_LogSupplyConsign.h"

#include "MOS_App.h"
#include "MOS_AgentManager.h"
#include "MOS_Agent.h"
#include "MOS_LogSupplyConsign_ListView_Item.h"

// -----------------------------------------------------------------------------
// Name: MOS_LogSupplyConsign constructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogSupplyConsign::MOS_LogSupplyConsign( const ASN1T_MsgLogRavitaillementTraitementCreation& asn )
    : nID_              ( asn.oid_consigne )
    , pion_             ( *MOS_App::GetApp().GetAgentManager().FindAgent( asn.oid_automate ) )
    , pPionLogHandling_ ( 0 )
    , pPionLogConvoying_( 0 )
    , nState_           ( eFinished )
{
    assert( &pion_ );
    pion_.AddConsign( *this );

    for( uint i = 0; i < asn.dotations.n; ++i )
    {
        sDotationData& dotationData = dotations_[ asn.dotations.elem[i].ressource_id ];
        dotationData.nNbrRequested_ = asn.dotations.elem[i].quantite_demandee;
        dotationData.nNbrReserved_  = asn.dotations.elem[i].quantite_accordee;
        dotationData.nNbrConvoyed_  = asn.dotations.elem[i].quantite_en_transit;
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_LogSupplyConsign destructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogSupplyConsign::~MOS_LogSupplyConsign()
{
    pion_.RemoveConsign( *this );
    if( pPionLogHandling_ )
        pPionLogHandling_->TerminateConsign( *this );
    if( pPionLogConvoying_ )
        pPionLogConvoying_->TerminateConsign( *this );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MOS_LogSupplyConsign::OnReceiveMsgUpdate
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_LogSupplyConsign::OnReceiveMsgUpdate( const ASN1T_MsgLogRavitaillementTraitementUpdate& asn )
{
    assert( pion_.GetID() == asn.oid_automate );

    if( asn.m.oid_pion_log_traitantPresent )
    {
        if( pPionLogHandling_ )
            pPionLogHandling_->TerminateConsign( *this );
        if( asn.oid_pion_log_traitant != 0 )
        {
            pPionLogHandling_ = MOS_App::GetApp().GetAgentManager().FindAgent( asn.oid_pion_log_traitant );
            assert( pPionLogHandling_ );
            pPionLogHandling_->HandleConsign( *this );
        }        
        else 
            pPionLogHandling_ = 0;
    }
    
    if( asn.m.oid_pion_convoiPresent )
    {
        if( pPionLogConvoying_ )
            pPionLogConvoying_->TerminateConsign( *this );
        if( asn.oid_pion_convoi != 0 )
        {
            pPionLogConvoying_ = MOS_App::GetApp().GetAgentManager().FindAgent( asn.oid_pion_convoi );
            assert( pPionLogConvoying_ );
            pPionLogConvoying_->HandleConsign( *this );
        }        
        else 
            pPionLogConvoying_ = 0;
    }

    if( asn.m.etatPresent )
        nState_ = (E_State)asn.etat;

    if( asn.m.dotationsPresent )
    {
        for( uint i = 0; i < asn.dotations.n; ++i )
        {
            sDotationData& dotationData = dotations_[ asn.dotations.elem[i].ressource_id ];
            dotationData.nNbrRequested_ = asn.dotations.elem[i].quantite_demandee;
            dotationData.nNbrReserved_  = asn.dotations.elem[i].quantite_accordee;
            dotationData.nNbrConvoyed_  = asn.dotations.elem[i].quantite_en_transit;
        }
    }
}
